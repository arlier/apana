
/* ================================================================
 *
 *  FileInput.cc
 *  Author: CHEN, Yu (Sun Yat-sen University)
 *  Version: V1.0
 *  Data: Feb.12, 2019.
 *  Description: Read the binary raw waveform into TTree ROOT file
 *              while each waveform is the a entry of TH1F.
 *
 *
 * ===============================================================*/

#include <iostream>
#include <fstream>
#include "Config.h"
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"

#define __DEBUG

using namespace std;

int BinFileReadToTreeFile(string inputFilePath,int numOfWaveforms, int lengthOfEachWave)
{

    cout<<endl;
    cout<<"===Process: Converting raw file input ROOT Tree File..."<<endl;

//    string outputFilePath = inputFilePath.substr(0,inputFilePath.find_last_of('.')) + ".root";
//    outputFilePath="raw2root/"+outputFilePath;
string outputFilePath ="/home/pmthome/aptest/raw2root/"+inputFilePath.substr(0,inputFilePath.find_last_of('.')) + ".root";
//inputFilePath="/home/pmthome/aptest/"+inputFilePath;
inputFilePath="/pmtfs/disk01/apdata/"+inputFilePath;

    cout<<"Output File Name: "<<outputFilePath<<endl;

    // Open input raw data file and create the read param.

    ifstream inputFile;
    inputFile.open(inputFilePath,ios::in);
    if (!inputFile.is_open())
    {
        cout<<"ERROR - BinFileReadToTreeFile() - Unable to open the Data File!"<<endl;
        return -1;
    }

    unsigned short oneSamplePoint;  // 10-bit sampling value takes 2 Bytes to store.

    // Open output ROOT file and create the TH1 and TTree object.

    TFile * outputFile = new TFile(outputFilePath.c_str(),"RECREATE");
    if (outputFile->IsOpen() == kFALSE)
    {
        cout<<"ERROR - BinFileReadToTreeFile() - Unable to open the ROOT File!"<<endl;
        return -1;
    }

    TH1F * readHist = new TH1F("readHist","Histogram for Raw Waveform",lengthOfEachWave,0,lengthOfEachWave);
    TTree * readTree = new TTree("rawDataTree","TTree for Raw waveform");
    readTree->Branch("rawWaveformHist",readHist);

    // Read the Data!!
int fcount=0;
    for (int jj = 0; jj < numOfWaveforms; jj++)
    {
if (!inputFile.eof()) {fcount++;}
//        cout<<"Current File Num:"<<jj<<endl;
        for (int ii = 0; ii < lengthOfEachWave; ii++)
        {
            inputFile.read(reinterpret_cast<char*>(&oneSamplePoint),2);     // Using reinterpret_cast to convert binary data.
    //        cout<<oneSamplePoint<<endl;

            // NOTE: Each histogram has extra Bins for overflow (Bin #0) and underflow (Bin #21001). Both of these two BIN content should be set to 1023!!

            readHist->SetBinContent(ii+1,oneSamplePoint);
        }
        readHist->SetBinContent(0,1023);
        readHist->SetBinContent(lengthOfEachWave+1,1023);

        readTree->Fill();
        inputFile.read(reinterpret_cast<char*>(&oneSamplePoint),2);     // Skip the Seperator.
    }
cout<<"the length is:"<<fcount<<endl;
    // Save the Output ROOT File
    readTree->Write();

    // Delete the local memory.

    delete readHist;
    delete readTree;

    inputFile.close();
    outputFile->Close();
    delete outputFile;

    cout<<"BinFileReadToTreeFile() is DONE!"<<endl;

    return 0;
}

// NOTE: output ROOT file shares the filename with the raw one.

int FileInput(string inputFilePath)
{

    // ----Setting up the Runtime global constant

    int numOfWaveforms = __NumOfWave;
    int lengthOfEachWave = __WaveLength;       // in 1GS/s points, i.e. 1ns

    cout<<"Input File Name: "<<inputFilePath<<endl;
    cout<<"Total Number of Waveforms: "<<numOfWaveforms<<endl;

    // ----Read the file.

    BinFileReadToTreeFile(inputFilePath,numOfWaveforms,lengthOfEachWave);

    return 0;
}

