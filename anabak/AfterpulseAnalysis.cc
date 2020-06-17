/* ================================================================
 *
 *  AfterpulseAnalysis.cc
 *  Author: CHEN, Yu (Sun Yat-sen University)
 *  Version: V1.0
 *  Data: Feb.12, 2019.
 *  Description: Read the ROOT file and do the afterpulse analysis.
 *
 *  Modified by Zhao Rong  2019.05.05
 *  Read data from root file and draw histograms
 *
 * ===============================================================*/

#include <iostream>
#include <fstream>
#include "Config.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGaxis.h"
#include "string.h"
#include <string>
#include <cstring>
#include "TLegend.h"
#include "TLatex.h"
#include "TLine.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "RawDataProcess.cc"
//#include "zranalysis.cc"
#include "zranalysisna.cc"
//#include "zranalysisnaham.cc"
// 2019-02-13 General Test with Hamamatsu PMT to check with Matlab
 using namespace std;

Int_t HamamatsuTest()
{

    TFile *f1 = new TFile("Hama.root","READ");

    TCanvas *c1 = new TCanvas("c1","c1");
    TCanvas *c2 = new TCanvas("c2","c2");
    TCanvas *c3 = new TCanvas("c3","c3");

    // Fit the primary Pulse

    c1->cd();

    TTree * primaryPulseTree = (TTree*) f1->Get("primaryPulseTree");
    primaryPulseTree->Draw("primaryPulseCharge>>primaryPulseChargeHist");
    TH1F * primaryPulseChargeHist = (TH1F*) gDirectory->Get("primaryPulseChargeHist");
    primaryPulseChargeHist->DrawCopy();

//    primaryPulseChargeHist->Fit("gaus","","",10,400);

    // Plot the 2-D distribution of the charge-time of APs.

    TTree * afterPulseTreePerAP = (TTree*) f1->Get("afterPulseTreePerAP");

    c2->Divide(2,2);

    TPad * c2_1 = (TPad * ) c2->GetPrimitive("c2_1");
    c2_1->cd();

    afterPulseTreePerAP->Draw("(afterPulseCharge/1.6):afterPulseTpeak>>afterPulse2DHist");
    TH1F * afterPulse2DHist = (TH1F*) gDirectory->Get("afterPulse2DHist");
    afterPulse2DHist->GetYaxis()->SetRangeUser(0.5,100);

    afterPulse2DHist->DrawCopy();
    gPad->SetLogy(1);

    TPad * c2_2 = (TPad * ) c2->GetPrimitive("c2_2");
    c2_2->cd();

    TH2F * afterPulse2DHist2 = new TH2F("afterPulse2DHist2","afterPulse2DHist2",1000,0,21000,1000,0,100);
    afterPulseTreePerAP->Project("afterPulse2DHist2","(afterPulseCharge/1.6):afterPulseTpeak");
    afterPulse2DHist2->DrawCopy("COLZ");
    gPad->SetLogy(1);

    TPad * c2_3 = (TPad * ) c2->GetPrimitive("c2_3");
    c2_3->cd();

    afterPulseTreePerAP->Draw("afterPulseTpeak>>afterPulseTpeak");
    TH1F * afterPulseTpeak = (TH1F*) gDirectory->Get("afterPulseTpeak");
    afterPulseTpeak->DrawCopy();

    TPad * c2_4 = (TPad * ) c2->GetPrimitive("c2_4");
    c2_4->cd();

    TH1F * afterPulseCharge = new TH1F("afterPulseCharge","afterPulseCharge",1000,0,100);
    // Using Project Function to fill a pre-defined TH1.
    afterPulseTreePerAP->Project("afterPulseCharge","afterPulseCharge/1.6","afterPulseCharge<100");
    afterPulseCharge->GetXaxis()->SetTitle("Charge of each AP (p.e.)");
    afterPulseCharge->DrawCopy();

    gPad->SetLogy(1);



    // Plot the Total Charge of AP after each PP

    c3->cd();
    TTree * afterPulseTreePerPP = (TTree*) f1->Get("afterPulseTreePerPP");
    afterPulseTreePerPP->Draw("afterPulseTotalCharge>>afterPulseTotalCharge");
    TH1F * afterPulseTotalCharge = (TH1F*) gDirectory->Get("afterPulseTotalCharge");
    afterPulseTotalCharge->DrawCopy();
    gPad->SetLogy(1);

    // Close the File.

    f1->Close();

    return 0;
}


// Batch Reading of NNVT Files.


Int_t NNVTPostAnalysisProcess(const char* date, const char* PMTid)

{
    TFile *f1;
    TCanvas *c1 = new TCanvas("c1","c1");
    c1->Divide(2,2);
    TCanvas *c2 = new TCanvas("c2","c2");
    c2->Divide(2,2);
    TH1F * primaryPulseChargeHist;
    char pad_name[10];
    TPad * p1;

    for(int ii = 0; ii < 4; ii++)
    {
        sprintf(pad_name, "c1_%d",ii+1);
	p1 = (TPad *) c1->GetPrimitive(pad_name);
        p1->cd();

        string filename =  string("/home/pmthome/aptest/Resultsroot/APTest_") + string(date) + string("_") + string(PMTid) + string("_") + to_string(ii) + string("_OUT.root");
        cout<<"Current Processing File Name: "<<filename<<endl;

        f1 = TFile::Open(filename.c_str(),"READ");
        TTree * primaryPulseTree = (TTree*) f1->Get("primaryPulseTree");
        primaryPulseTree->Draw("primaryPulseCharge>>primaryPulseChargeHist");
        primaryPulseChargeHist = (TH1F*) gDirectory->Get("primaryPulseChargeHist");
//        primaryPulseChargeHist->DrawCopy();
        primaryPulseChargeHist->Fit("gaus","","",10,400);

	sprintf(pad_name, "c2_%d",ii+1);
        p1 = (TPad *) c2->GetPrimitive(pad_name);
        p1->cd();

        TH1F * averageWaveform = (TH1F*) f1->Get("averageWaveform");
        averageWaveform->GetXaxis()->SetRange(0,1000);
        averageWaveform->Draw();

	
     }
c1->SaveAs("test1.png");
c2->SaveAs("test2.png");
    return 0;
}

// Raw File Batch input
Int_t RawFileBatchInput(const char* path, const char* date, const char* PMTid)
{
    int numOfSetpoint = __NumOfSetpoint;
//    cout<<numOfSetpoint<<endl;
    for (int ii = 0; ii < numOfSetpoint; ii++)
    {
        string inputFullFilePath =  string("APTest_") + string(date) + string("_") + string(PMTid) + string("_") + to_string(ii) + string(".dat");
        FileInput(inputFullFilePath);
    }

    return 0;
}

// Afterpulse Analysis
Int_t RawFileBatchProcess(const char* path, const char* date, const char* PMTid)
{
    int numOfSetpoint = __NumOfSetpoint;
    for (int ii = 0; ii < numOfSetpoint; ii++)
    {
        string inputFullFilePath = string(path) + string("APTest_") + string(date) + string("_") + string(PMTid) + string("_") + to_string(ii) + string(".root");
        string outputFullFilePath = string("/home/pmthome/aptest/Resultsroot/APTest_") + string(date) + string("_") + string(PMTid) + string("_") + to_string(ii) + string("_OUT.root");
        RawDataProcess(inputFullFilePath.c_str(),outputFullFilePath.c_str());
    }

    return 0;
}

 int file_exists(const char *filename) 
 { 
   return (access(filename, 0) == 0); 
   } 
// Main Function

Int_t main(int argc,char  ** argv)
{
    const char * path = "/home/pmthome/aptest/raw2root/";
    const char * date = "20190717";
    const char * PMTid = "EA7224";
if (argc ==1){}             //do nothing
 else                    //read parameters as filenames
     {
    for(int i=1;i<argc;i++)
      {   if(strcmp(argv[i],"-s")== 0){i++;PMTid=argv[i];}      
    else if(strcmp(argv[i],"-d")== 0){i++;date=argv[i];}   
      }
     }
//    const char * date = "20190427";
//    const char * PMTid = "PA1712-1657";
        string outputFullFilePath = string("/home/pmthome/aptest/Resultsroot/APTest_") + string(date) + string("_") + string(PMTid) + string("_") + to_string(1) + string("_OUT.root");
if(file_exists(outputFullFilePath.c_str())){cout<<"file generated!"<<endl;}
else{
    RawFileBatchInput(path,date,PMTid);
    RawFileBatchProcess(path,date,PMTid);}
//    NNVTPostAnalysisProcess(date, PMTid);
//   Drawwaveform(date, PMTid);
//   Apanalysis(date,PMTid);
    return 0;

}


