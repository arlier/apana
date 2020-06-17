#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "TFile.h"
#include "TLine.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TGaxis.h"
#include <vector>

//#include "TString.h"
typedef unsigned short    uint16_t;  
typedef unsigned char   uint8_t;
using namespace std;
//typedef unsigned int uint16_t;  
std::vector<uint16_t> fileData(21504);
int main(){

//TString filename="/data/users/guest/afterpulse/ApData_20180529/20180509_AP4_L21504S";
TString filename="/home/pmthome/aptest/APTest_20190429_PA1812-1068_0";
TString pngname=filename+".png";
TString rootname=filename+".root";
filename+=".dat";
TFile *outroot_file=new TFile(rootname,"recreate");
const int npoint=21000;
const int threshold=3;
const int delaytime=500;
const int sig_skip=50;
int nevent=0, tcount=0;
int nap=0;
double aver[npoint]={0};
double tmpwave[npoint]={0};

int ap_rise_pos;
char  tem[20];
double baseline,tmp;
int main_peak_pos,main_peak_value;
int main_QDC;
double ap_QDC;
double ap_min;
double ap_QDC_sum;
double ap_QDC_sum2;
double ap_wave=0;
ifstream sigfile(filename,ios::binary);
TH2D * wave2d = new TH2D("wave2d","2 dimentional histogram",npoint,0,npoint,200,500,1000);
TH2D * wavecharge2d = new TH2D("wavecharge2d","2 dimentional chargehistogram",npoint,0,npoint,200,-200,5000);
TH2D * ap_amp_t = new TH2D("ap_amp_t","2 dimentional ap_amp_t",npoint,0,npoint,100,0,100);
TH2D * ap_QDC_t = new TH2D("ap_QDC_t","2 dimentional ap_QDC_t",npoint,0,npoint,200,0,1200);
TH2D * main_ap_QDC = new TH2D("main_ap_QDC","2 dimentional main_ap_QDC",200,2000,20000,200,0,.3);
TH2D * main_ap_QDC2 = new TH2D("main_ap_QDC2","2 dimentional main_ap_QDC2",200,2000,20000,200,0,.3);
TH2D * ap_QDC_amp = new TH2D("ap_QDC_amp","2 dimentional ap_QDC_amp",100,0,.3,200,0,1200);
TH1D * wave1d = new TH1D("wave1d","average waveform",npoint,0,npoint);
TH1D * Hist_ap_rise_pos = new TH1D("Hist_ap_rise_pos","ap_rise_position",200,0,npoint);
TH1D * Hist_min_pos = new TH1D("Hist_min_pos","min_position",npoint,0,npoint);
TH1D * Hist_main_amp = new TH1D("Hist_main_amp","main_amp",100,0,1000);
TH1D * Hist_main_QDC = new TH1D("Hist_main_QDC","main_QDC",100,2000,20500);
TH1D * Hist_ap_amp2 = new TH1D("Hist_ap_amp2","ap_amp2",100,0,100);
TH1D * Hist_ap_QDC2 = new TH1D("Hist_ap_QDC2","ap_QDC2",200,0,1200);
TH1D * Hist_ap_amp = new TH1D("Hist_ap_amp","ap_amp",100,0,100);
TH1D * Hist_ap_QDC = new TH1D("Hist_ap_QDC","ap_QDC",200,0,1200);
TH1D * Hist_ap_amp_norm = new TH1D("Hist_ap_amp_norm","ap_amp_norm",100,0,.1);
TH1D * Hist_ap_QDC_norm= new TH1D("Hist_ap_QDC_norm","ap_QDC_norm",200,0,.1);
TH1D * Hist_ap_QDC_sum = new TH1D("Hist_ap_QDC_sum","ap_QDC_sum",50,0,1500);
TH1D * Hist_ap_QDC_sum_norm = new TH1D("Hist_ap_QDC_sum_norm","ap_QDC_sum_norm",50,0,.3);
TH1D * Hist_nap = new TH1D("Hist_nap","ap_per_wave",10,0,10);


//initiation
for (int i=0; i<npoint; i++){aver[i]=0;}
//read data
while(sigfile.read(( char*)&fileData[0],npoint*sizeof(short)))
	{
		if (nevent>=11000) break;
		tmp=0;
		main_peak_pos=0;
		main_peak_value=fileData[0];
	for (int i=0; i<npoint; i++){   //this is the wave loop
		aver[i]+=fileData[i]; 
		wave2d->Fill(i,fileData[i]);
		if(i<50)tmp+=fileData[i];
		if(fileData[i]<main_peak_value){main_peak_value=fileData[i];main_peak_pos=i;}
		}
	baseline=tmp*1./50;
	ap_rise_pos=0;
	main_QDC=0;
//find the second signal
	ap_wave=0;
	for (int i=0; i<npoint; i++)tmpwave[i]=baseline;
//	for (int i=0; i<npoint; i++){ap_wave+=baseline-fileData[i];wavecharge2d->Fill(i,ap_wave);}
	if(nevent<10)cout<<"the baseline of event"<<nevent<<"is:"<<baseline<<"pos"<<ap_wave<<endl;
    for(int j=main_peak_pos-40;j<main_peak_pos+60;j++)
	{
	main_QDC+=baseline-fileData[j];	
	}
	Hist_main_QDC->Fill(main_QDC);
	Hist_min_pos->Fill(main_peak_pos);
	Hist_main_amp->Fill(baseline-main_peak_value);
	ap_QDC_sum=0;
	ap_QDC_sum2=0;
	nap=0;
	for(int j=(main_peak_pos+delaytime);j<(npoint-sig_skip);j++)
			{
			for(int k=0;k<5;k++){
			
			if(fileData[j+k]>=(baseline-threshold))break;
			if(k==4){   //another signal
					ap_rise_pos=j;
					ap_min=fileData[j];
					ap_QDC=0;
					for(int ic=-4;ic<sig_skip;ic++){	
						if(fileData[j+ic]<ap_min)ap_min=fileData[j+ic];
						ap_QDC+=baseline-fileData[j+ic];
						tmpwave[j+ic]=fileData[j+ic];
						}
					ap_QDC_sum+=ap_QDC;
					if(j>10000)ap_QDC_sum2+=ap_QDC;
					if(j>10000)Hist_ap_amp2->Fill(baseline-ap_min);
					if(j>10000)Hist_ap_QDC2->Fill(ap_QDC);
					nap++;
					Hist_ap_amp->Fill(baseline-ap_min);
					Hist_ap_amp_norm->Fill((baseline-ap_min)*1./(baseline-main_peak_value));
					Hist_ap_QDC->Fill(ap_QDC);
					Hist_ap_QDC_norm->Fill(ap_QDC*1./main_QDC);
					ap_amp_t->Fill(ap_rise_pos,baseline-ap_min);
					ap_QDC_amp->Fill((baseline-ap_min)*1./(baseline-main_peak_value),ap_QDC);
					ap_QDC_t->Fill(ap_rise_pos,ap_QDC);
					j=j+sig_skip;     //skip the sigwindow
					Hist_ap_rise_pos->Fill(ap_rise_pos);
					}
				}
			
			}
	for (int i=0; i<npoint; i++){ap_wave+=baseline-tmpwave[i];wavecharge2d->Fill(i,ap_wave);}
	Hist_ap_QDC_sum->Fill(ap_QDC_sum);
	Hist_ap_QDC_sum_norm->Fill(ap_QDC_sum*1./main_QDC);
	Hist_nap->Fill(nap);
	main_ap_QDC->Fill(main_QDC,ap_QDC_sum*1./main_QDC);
	main_ap_QDC2->Fill(main_QDC,ap_QDC_sum2*1./main_QDC);


	if(nevent<10)cout<<nevent<<"de ap is"<<main_QDC<<endl;
	if(nevent<10)cout<<fileData[0]<<endl;
		nevent++;
		sigfile.read(( char*)tem,sizeof(short));   //skip the tag
	}
cout<<nevent<<endl;

sigfile.close();
//the average waveform
for (int i=0; i<npoint; i++){wave1d->SetBinContent(i, (int)aver[i]/nevent);}//fill wave1d
cout<<"1dddd:"<<1.*aver[5]/nevent<<endl;

//drawing
TCanvas * mycan = new TCanvas("mycan", "waves", 4000, 3200);
TGaxis::SetMaxDigits(3);
mycan->Divide(4, 4);
mycan->cd(1);
wave2d->Draw("colz");
//wave2d->Write();
wave2d->SetStats(0);
wave2d->GetXaxis()->SetTitle("time[ns]");
wave2d->GetYaxis()->SetTitle("V[mV]");
mycan->SetLogz();
mycan->cd(2);
//wave1d->Draw("colz");
//wave1d->GetYaxis()->SetRangeUser(800,1100);
//wave1d->GetXaxis()->SetRangeUser(0,4000);
//wave1d->SetStats(0);
Hist_ap_rise_pos->Draw("colz");
//Hist_ap_rise_pos->Write();
Hist_ap_rise_pos->GetXaxis()->SetTitle("time[ns]");
Hist_ap_rise_pos->GetYaxis()->SetTitle("count");
gPad->SetLogz();
mycan->cd(3);
Hist_main_QDC->Draw();
//Hist_main_QDC->Write();
Hist_main_QDC->GetXaxis()->SetTitle("QDC[ns#times mV]");
Hist_main_QDC->GetYaxis()->SetTitle("count");
//mycan->SaveAs("test.eps");
mycan->cd(4);
Hist_main_amp->Draw();
//Hist_main_amp->Write();
Hist_main_amp->GetXaxis()->SetTitle("U[mV]");
Hist_main_amp->GetYaxis()->SetTitle("count");
mycan->cd(5);
Hist_ap_amp->Draw();
//Hist_ap_amp->Write();
Hist_ap_amp2->Draw("SAME");
Hist_ap_amp2->SetLineColor(kRed);
Hist_ap_amp->GetXaxis()->SetTitle("U[mV]");
Hist_ap_amp->GetYaxis()->SetTitle("count");
mycan->cd(6);
Hist_ap_QDC->Draw();
Hist_ap_QDC2->Draw("SAME");
Hist_ap_QDC2->SetLineColor(kRed);
Hist_ap_QDC->GetXaxis()->SetTitle("QDC[ns#times mV]");
Hist_ap_QDC->GetYaxis()->SetTitle("count");
mycan->cd(7);
ap_amp_t->Draw("colz");
ap_amp_t->GetXaxis()->SetTitle("time[ns]");
ap_amp_t->GetYaxis()->SetTitle("U[mV]");
gPad->SetLogz();
mycan->cd(8);
ap_QDC_t->Draw("colz");
ap_QDC_t->GetXaxis()->SetTitle("time[ns]");
ap_QDC_t->GetYaxis()->SetTitle("QDC[ns#times mV]");
gPad->SetLogz();
mycan->cd(9);
Hist_ap_amp_norm->Draw();
Hist_ap_amp_norm->GetXaxis()->SetTitle("normalized amplitede");
Hist_ap_amp_norm->GetYaxis()->SetTitle("count");
Hist_ap_QDC_norm->Draw("SAME");
Hist_ap_QDC_norm->SetLineColor(kRed);
Hist_ap_QDC_norm->GetXaxis()->SetTitle("normalized QDC");
Hist_ap_QDC_norm->GetYaxis()->SetTitle("count");
mycan->cd(10);
main_ap_QDC2->Draw("colz");
main_ap_QDC2->GetYaxis()->SetTitle("normalized after-pulse QDC");
main_ap_QDC2->GetXaxis()->SetTitle("main signal QDC[ns#times mV]");
gPad->SetLogz();
mycan->cd(11);
Hist_ap_QDC_sum->Draw();
Hist_ap_QDC_sum->GetXaxis()->SetTitle("after-pulse QDC sum[ns#times mV]");
Hist_ap_QDC_sum->GetYaxis()->SetTitle("count");
mycan->cd(12);
Hist_ap_QDC_sum_norm->Draw();
Hist_ap_QDC_sum_norm->GetXaxis()->SetTitle("after-pulse QDC sum normolized");
Hist_ap_QDC_sum_norm->GetYaxis()->SetTitle("count");
mycan->cd(13);
Hist_nap->Draw();
Hist_nap->GetXaxis()->SetTitle("afer-pulse number");
Hist_nap->GetYaxis()->SetTitle("count");
mycan->cd(14);
main_ap_QDC->Draw("colz");
main_ap_QDC->GetYaxis()->SetTitle("normalized after-pulse QDC in 1#mu s ");
main_ap_QDC->GetXaxis()->SetTitle("main signal QDC[ns#times mV]");
gPad->SetLogz();
mycan->cd(15);
ap_QDC_amp->Draw("colz");
ap_QDC_amp->GetXaxis()->SetTitle("normalized after-pulse QDC");
ap_QDC_amp->GetYaxis()->SetTitle("main signal QDC[ns#times mV]");
gPad->SetLogz();
mycan->cd(16);
wavecharge2d->Draw("colz");
wavecharge2d->GetXaxis()->SetTitle("time[ns]");
wavecharge2d->GetYaxis()->SetTitle("signal amplitude[mV]");
gPad->SetLogz();
//mycan->SaveAs("test.svg");
//wave1d->SetFillColor(0);
//wave2d->Write();

mycan->SaveAs(pngname);
outroot_file->Write();

return 0 ;

}
