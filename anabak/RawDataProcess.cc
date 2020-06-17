
/* ================================================================
 *
 *  FileInput.cc
 *  Author: CHEN, Yu (Sun Yat-sen University)
 *  Version: V1.0
 *  Data: Feb.12, 2019.
 *  Description: Read the ROOT file and Calculate primary Trees
 *
 *
 *
 * ===============================================================*/

#include <iostream>
#include <fstream>
#include "Config.h"
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include "FileInput.cc"


// =============================================

// NOTE: In this function, averageWaveform pointer will "new" a memory space, thus it SHOULDN'T
//      be allocated memory before this function is called.

TH2F* Generate2dWaveform(TTree* rawDataTree)
{

    cout<<endl;
    cout<<"===Process: Filling the 2D Waveform..."<<endl;

    // Get the First raw data histogram to allocate the memory of averageWaveform accordingly.
    TH1F * waveformCurrent = 0;         // Reading Pointer Must be initialized!!!! If it is 0, ROOT will automatically create the Heap Space.
                                        // If it is previously allocated the space, ROOT will always use this space to contain the data.
    rawDataTree->SetBranchAddress("rawWaveformHist",&waveformCurrent);
    TH2F * Waveform2d = new TH2F("wav","",__WaveLength,0,__WaveLength,2000,-500,1500);    
    Waveform2d->SetName("Waveform2d");
    Waveform2d->SetTitle("Waveform2d Waveform");

    Int_t numOfEntries = rawDataTree->GetEntries();
//    Int_t numOfEntries = 5000;

    for (int ii = 0; ii < numOfEntries; ii++)
    {
        rawDataTree->GetEntry(ii);
	for(int aa=1;aa<__WaveLength;aa++){Waveform2d->Fill(aa,waveformCurrent->GetBinContent(aa));}
//	if(ii==5){for(int aa=1;aa<__WaveLength;aa++){cout<<waveformCurrent->GetBinContent(aa)<<endl;}};
    }

    return Waveform2d;
}
TH1F* GenerateMeanWaveform(TTree* rawDataTree)
{

    cout<<endl;
    cout<<"===Process: Calculate the Average Waveform..."<<endl;

    // Get the First raw data histogram to allocate the memory of averageWaveform accordingly.
    TH1F * waveformCurrent = 0;         // Reading Pointer Must be initialized!!!! If it is 0, ROOT will automatically create the Heap Space.
                                        // If it is previously allocated the space, ROOT will always use this space to contain the data.
    rawDataTree->SetBranchAddress("rawWaveformHist",&waveformCurrent);
    rawDataTree->GetEntry(0);
    TH1F * averageWaveform = new TH1F(*waveformCurrent);       // Using Copy constructor to generate the TH1F averageWaveform
    averageWaveform->SetName("averageWaveform");
    averageWaveform->SetTitle("Average Waveform");

    // LOOP all the entries to accumulate the waveform. NOTE: averageWaveform has ALREADY contained the first entry!!
    Int_t numOfEntries = rawDataTree->GetEntries();
//    Int_t numOfEntries = 5000;

    for (int ii = 1; ii < numOfEntries; ii++)
    {
        rawDataTree->GetEntry(ii);
        averageWaveform->Add(waveformCurrent,1);
    }

    averageWaveform->Scale(1/(Double_t)numOfEntries);

//    averageWaveform->DrawClone();

    return averageWaveform;
}

// =============================================

// NOTE: Like the averageWaveform, averageWaveformAligned pointer will "new" a memory space, thus it SHOULDN'T
//      be allocated memory before this function is called.

TH1F* GenerateMeanWaveformAligned(TTree* rawDataTree, TTree* primaryPulseTree)
{

    cout<<endl;
    cout<<"===Process: Calculate the Peak Aligned Average Waveform..."<<endl;

    // The Aligned Waveform length from peak+__AlignedPeakLeft to peak+__AlignedPeakRight. Length: __AlignedPeakRight - __AlignedPeakLeft.
    TH1F * averageWaveformAligned = new TH1F("averageWaveformAligned","Peak aligned average waveform",
                                             __AlignedPeakRight-__AlignedPeakLeft,__AlignedPeakLeft,__AlignedPeakRight);

    TH1F * waveformCurrent = 0;         // Reading Pointer Must be initialized!!!! If it is 0, ROOT will automatically create the Heap Space.
                                        // If it is previously allocated the space, ROOT will always use this space to contain the data.
    rawDataTree->SetBranchAddress("rawWaveformHist",&waveformCurrent);

    Int_t primaryPulsePeakPosition;
    primaryPulseTree->SetBranchAddress("primaryPulsePeakPosition",&primaryPulsePeakPosition);

    // LOOP all the entries to accumulate the waveform. NOTE: averageWaveform has ALREADY contained the first entry!!
    Int_t numOfEntries = rawDataTree->GetEntries();
    for (int ii = 1; ii < numOfEntries; ii++)
    {
        rawDataTree->GetEntry(ii);
        primaryPulseTree->GetEntry(ii);

        for(int jj = 1; jj <= __AlignedPeakRight-__AlignedPeakLeft; jj++)
        {
            averageWaveformAligned->AddBinContent(jj,waveformCurrent->GetBinContent(primaryPulsePeakPosition+jj+__AlignedPeakLeft));

        }

    }

    averageWaveformAligned->Scale(1/(Double_t)numOfEntries);

    return averageWaveformAligned;

}


// =============================================
// MAIN Function

Int_t RawDataProcess(const char * inputRawDataFileName, const char * outputAnalysedFileName)
{

// 1) -------- Prepare the output file and output Trees!

    TFile * outputAnalysedFile = new TFile(outputAnalysedFileName,"UPDATE");//why not recreate? 
    if (outputAnalysedFile->IsOpen() == kFALSE)
    {
        cout<<"ERROR-RawDataProcess(): Unable to open the output ROOT File!"<<endl;
        return -1;
    }

    outputAnalysedFile->cd();

    TTree * primaryPulseTree = new TTree("primaryPulseTree","Analysis Tree for primary pulses");
    Int_t primaryPulsePeakPosition;     // Peak Position of each primary pulse;
    Double_t primaryPulseBaseline;      // Baseline of each primary pulse;
    Double_t primaryPulsePeak;          // Peak value (after subtraction of baseline) of each primary pulse. In mV
    Double_t primaryPulseCharge;        // Charge of each primary pulse. in pC
    primaryPulseTree->Branch("primaryPulsePeakPosition",&primaryPulsePeakPosition);
    primaryPulseTree->Branch("primaryPulseBaseline",&primaryPulseBaseline);
    primaryPulseTree->Branch("primaryPulsePeak",&primaryPulsePeak);
    primaryPulseTree->Branch("primaryPulseCharge",&primaryPulseCharge);

    TTree * afterPulseTreePerAP = new TTree("afterPulseTreePerAP","Analysis Tree for afterpulses (Each entry relates to afterpulse)");
    Int_t afterPulseIndex;          // Which primary pulse does this afterpulse belongs to?
    Int_t afterPulseTcross;         // Time when AP cross the threshold
    Int_t afterPulseTpeak;          // Peak time of AP within the integration area
    Double_t afterPulsePeak;        // Peak value (after substraction of baseline) in mV.
    Double_t afterPulseCharge;      // Charge in pC.
    Double_t afterPulseRelCharge;   // Charge relative to primary pulse.
    afterPulseTreePerAP->Branch("afterPulseIndex",&afterPulseIndex);
    afterPulseTreePerAP->Branch("afterPulseTcross",&afterPulseTcross);
    afterPulseTreePerAP->Branch("afterPulseTpeak",&afterPulseTpeak);
    afterPulseTreePerAP->Branch("afterPulsePeak",&afterPulsePeak);
    afterPulseTreePerAP->Branch("afterPulseCharge",&afterPulseCharge);
    afterPulseTreePerAP->Branch("afterPulseRelCharge",&afterPulseRelCharge);

    TTree * afterPulseTreePerPP = new TTree("afterPulseTreePerPP","Analysis Tree for afterpulses (Each entry relates to primary pulse)");
    Int_t afterPulseRate;           // Number of afterpulses within 20 us
    Double_t afterPulseTotalCharge; // Total charge of afterpulses within 20 us
    afterPulseTreePerPP->Branch("afterPulseRate",&afterPulseRate);
    afterPulseTreePerPP->Branch("afterPulseTotalCharge",&afterPulseTotalCharge);

// 2) -------- Read the Raw File

    TFile * inputRawDataFile = new TFile(inputRawDataFileName,"READ");
    if (inputRawDataFile->IsOpen() == kFALSE)
    {
        cout<<"ERROR-RawDataProcess(): Unable to open the Input ROOT File!"<<endl;
        return -1;
    }

    inputRawDataFile->cd();

    TTree *rawDataTree = (TTree*) inputRawDataFile->Get("rawDataTree");     // It is recommend to use Get() to load the KET into OBJ.

// 3) -------- Calculated the Average Waveform

    // NOTE: Memory of averageWaveform is allocated within GenerageMeanwaveform(). Should be freed later!!
    //       Also the directory of averageWaveform should be changed to the output file!!
    TH1F * averageWaveform = GenerateMeanWaveform(rawDataTree);
    averageWaveform->SetDirectory(outputAnalysedFile);
// and the 2D wave form
	TH2F *WaveForm2d= Generate2dWaveform(rawDataTree);
    WaveForm2d->SetDirectory(outputAnalysedFile);

    Int_t averageWaveformPeakPosition = averageWaveform->GetMinimumBin();

    if (averageWaveformPeakPosition < __PrimaryPeakLeftMost)
    {
        cout<<"Warning: Peak position of the average waveform is TOO LEFT!!"<<averageWaveformPeakPosition <<endl;
        averageWaveformPeakPosition = __PrimaryPeakLeftMost;
    }
    else if(averageWaveformPeakPosition > __PrimaryPeakRightMost)
    {
        averageWaveformPeakPosition = __PrimaryPeakRightMost;
        cout<<"Warning: Peak position of the average waveform is TOO RIGHT!!";
    }

    cout<<"Position of the peak at the average waveform:"<<averageWaveformPeakPosition<<endl;



// 4) -------- Loop the rawDataTree to optain the primary pulse and afterpulse data

    cout<<endl;
    cout<<"===Process: Loop over each waveform..."<<endl;

    TH1F * currentWaveform = new TH1F(*averageWaveform);    // Create the currentWaveform as the same structure as the averageWaveform.
    rawDataTree->SetBranchAddress("rawWaveformHist",&currentWaveform);

    for(int ii = 0;ii<rawDataTree->GetEntries();ii++)
    {

        rawDataTree->GetEntry(ii);

        // ==========================================
        // ==== Search the peak of the primary pulse.

        primaryPulsePeakPosition = averageWaveformPeakPosition + __PrimaryPeakSearchLeft;
        primaryPulsePeak = currentWaveform->GetBinContent(primaryPulsePeakPosition);

        for(int jj = averageWaveformPeakPosition + __PrimaryPeakSearchLeft + 1; jj <= averageWaveformPeakPosition + __PrimaryPeakSearchRight; jj++)
        {
            Double_t newValue = currentWaveform->GetBinContent(jj);
            if( newValue < primaryPulsePeak)
            {
                primaryPulsePeakPosition = jj;
                primaryPulsePeak = newValue;
            }
        }

        // ==== Calculate the charge and baseline for the primary pulse.

        primaryPulseBaseline = 1.*currentWaveform->Integral(primaryPulsePeakPosition+__PrimaryBaselineLeft,primaryPulsePeakPosition+__PrimaryBaselineRight)
                / (__PrimaryBaselineRight-__PrimaryBaselineLeft+1);
            // Baseline in mV
        primaryPulseCharge = ( primaryPulseBaseline * (__PrimaryPeakIntRight-__PrimaryPeakIntLeft+1)
                              - currentWaveform->Integral(primaryPulsePeakPosition+__PrimaryPeakIntLeft,primaryPulsePeakPosition+__PrimaryPeakIntRight)
                             ) / 50.;
            // Charge in pC

        primaryPulseTree->Fill();       // Update the primary pulse Tree;


        // ==========================================
        // ==== Search the Afterpulse

        afterPulseIndex = ii;   //belongs to the ii primary pulse
        afterPulseTpeak = primaryPulsePeakPosition+__AfterpulseSearchLeft;

        afterPulseRate = 0;
        afterPulseTotalCharge = 0;

        Double_t afterpulseBaseline = primaryPulseBaseline;         // Originally the afterpulse baseline is set to the baseline of primary pulse.
        Double_t afterpulseThreshold = primaryPulseBaseline-__AfterpulseThreshold;      // AP threshold is fixed related to the

        Int_t searchIndex = primaryPulsePeakPosition+__AfterpulseSearchLeft;

        while(searchIndex < primaryPulsePeakPosition+__AfterpulseSearchRight)
        {
            // afterpulseThreshold updated
            //afterpulseThreshold = afterpulseBaseline - __AfterpulseThreshold;
            // Check whether successive 5 points is over the threshold.
            if((currentWaveform->GetBinContent(searchIndex) <= afterpulseThreshold)
                && (currentWaveform->GetBinContent(searchIndex+1) <= afterpulseThreshold)
                && (currentWaveform->GetBinContent(searchIndex+2) <= afterpulseThreshold)
                && (currentWaveform->GetBinContent(searchIndex+3) <= afterpulseThreshold)
                && (currentWaveform->GetBinContent(searchIndex+4) <= afterpulseThreshold) )
            {
                // Find the pulse peak within (searchIndex, searchIndex+__AfterpulseIntWindow)
                Double_t _Tpeak = searchIndex;
                Double_t _Peak = currentWaveform->GetBinContent(_Tpeak);
                Double_t _Charge = 0;

                for (int kk = searchIndex+1; kk<searchIndex+__AfterpulseIntWindow; kk++)
                {
                    if(currentWaveform->GetBinContent(kk)<_Peak)
                    {
                        _Tpeak = kk;
                        _Peak = currentWaveform->GetBinContent(kk);
                    }
                }

                // Check _Tpeak with the last peak. If it It is too close with the previous one, don't recalculate the baseline.
                if(_Tpeak > afterPulseTpeak + __AfterpulseIntWindow * 2)
                {
                    afterpulseBaseline = 1.*currentWaveform->Integral(_Tpeak+__AfterpulseBaselineLeft,_Tpeak+__AfterpulseBaselineRight)
                            / (__AfterpulseBaselineRight-__AfterpulseBaselineLeft+1);
                }   // end of if(_Tpeak > afterPulseTpeak + __AfterpulseIntWindow * 2)

                // Calculate the Charge of each afterpulse.

                _Charge = ( afterpulseBaseline * (__AfterpulsePeakIntRight-__AfterpulsePeakIntLeft+1)
                            - currentWaveform->Integral(_Tpeak+__AfterpulsePeakIntLeft,_Tpeak+__AfterpulsePeakIntRight)
                            ) / 50.;

                // Check if _Charge is positive. If so, confirm it as a valid afterpulse and fill into the tree.
                if(_Charge > 0)
                {
//                    afterPulseTcross = searchIndex; //zhaorong ... change to relative time.
                    afterPulseTcross = searchIndex-primaryPulsePeakPosition;
                    afterPulseTpeak = _Tpeak;
                    afterPulsePeak = afterpulseBaseline - _Peak;
                    afterPulseCharge = _Charge;
                    afterPulseRelCharge = (primaryPulseCharge > 0) ? (1.*afterPulseCharge/primaryPulseCharge) : 0;

                    afterPulseTreePerAP->Fill();

                    afterPulseTotalCharge = afterPulseTotalCharge + _Charge;
                    afterPulseRate = afterPulseRate + 1;

                    // After confirming, searchIndex is skipped by a __AfterpulseIntWindow

                    searchIndex = searchIndex + __AfterpulseIntWindow;
                }
                else
                {
                    searchIndex = searchIndex + 1;
                }   // end of if(_Charge>0)


            }
            else
            {
                searchIndex = searchIndex + 1;
            }   // end of if( 5 point over threshold )


        }

        afterPulseTreePerPP->Fill();

    }

    delete currentWaveform;

// 5) -------- Calculated the Peak Aligned Average Waveform

    // NOTE: Memory of averageWaveformAligned is allocated within GenerateMeanWaveformAligned(). Should be freed later!!
    //       Also the directory of averageWaveformAligned should be changed to the output file!!
    TH1F * averageWaveformAligned = GenerateMeanWaveformAligned(rawDataTree,primaryPulseTree);
    averageWaveformAligned->SetDirectory(outputAnalysedFile);





// 6) -------- Writing to Files

    cout<<endl;
    cout<<"===Process: Writing to File..."<<endl;

    outputAnalysedFile->cd();

    averageWaveform->Write("",TObject::kOverwrite);
    WaveForm2d->Write("",TObject::kOverwrite);
    averageWaveformAligned->Write("",TObject::kOverwrite);
    primaryPulseTree->Write("",TObject::kOverwrite);
    afterPulseTreePerAP->Write("",TObject::kOverwrite);
    afterPulseTreePerPP->Write("",TObject::kOverwrite);

    // Free the space of local parameter. Not that the Histgram and Tree will be deleted automatically after the file is closed!!

    delete averageWaveform;
    delete WaveForm2d;
    delete averageWaveformAligned;
    delete primaryPulseTree;
    delete afterPulseTreePerAP;
    delete afterPulseTreePerPP;

    inputRawDataFile->Close();
    outputAnalysedFile->Close();

    delete inputRawDataFile;
    delete outputAnalysedFile;

    return 0;
}






