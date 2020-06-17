#include <TCanvas.h>
#include <TFile.h>
#include <TMath.h>
#include <TGraph.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TLine.h"
#include "TPad.h"
#include "TLatex.h"
#include "TLegend.h"
using namespace std;

int main(){
TString filedir0="/home/pmthome/aptest/Analysis_cy/ap3369test0nbs.txt";
TString filedir1="/home/pmthome/aptest/Analysis_cy/ap3369test1nbs.txt";
TString filedir2="/home/pmthome/aptest/Analysis_cy/ap3369test2nbs.txt";
TString filedir3="/home/pmthome/aptest/Analysis_cy/ap3369test3nbs.txt";
//TString filename="shield.txt";
//TString dir=filedir+"/"+filename;
TGraphErrors *dte0=new TGraphErrors();
TGraphErrors *dte1=new TGraphErrors();
TGraphErrors *dte2=new TGraphErrors();
TGraphErrors *dte3=new TGraphErrors();
string ss0,ss1,ss2,ss3,tmp;    //not TString!!!!!!!!!!!!!
double x,y;
double amp[4];
int i,j,k,count=0;
TString name[5]={"name1","name2","name3","name4","name5"};
TString height[5]={"height=-0.6m (#muT)","height=-0.35m (#muT)","height=-0.1m (#muT)","height=0.15m (#muT)","height=0.4m (#muT)"};
//TH2D *myhist1=new TH2D("myhist1","heignt=0cm",length,-0.5,4.5,width,-0.5,4.5);
//TH2D *myhist2=new TH2D("myhist2","heignt=40cm",length,-0.5,4.5,width,-0.5,4.5);
//TH2D *myhist3=new TH2D("myhist3","height=100cm",length,-0.5,4.5,width,-0.5,4.5);
//TH2D *myhist4=new TH2D("myhist4","height=140cm",length,-0.5,4.5,width,-0.5,4.5);
ifstream filein0(filedir0);
ifstream filein1(filedir1);
ifstream filein2(filedir2);
ifstream filein3(filedir3);
//  myhist[j]=new TH2D(name[j],height[j],length,-0.4,0.4,width,-0.7,0.3);
  for(i=0;i<2000;i++)
  {
	getline(filein0,ss0);
	getline(filein1,ss1);
	getline(filein2,ss2);
	getline(filein3,ss3);
	istringstream is0(ss0);
	istringstream is1(ss1);
	istringstream is2(ss2);
	istringstream is3(ss3);
	is0>>x>>y>>amp[0];
	is1>>x>>y>>amp[1];
	is2>>x>>y>>amp[2];
	is3>>x>>y>>amp[3];
//	cout<<count<<" "<<x<<" "<<y<<" "<<z<<" "<<magnetic_field<<endl;
//    if(static_cast<int> ( magnetic_field)==0)magnetic_field=magnetic_field+1;
//	myhist[j]->SetBinContent(i+1,k+1,static_cast<int> ( magnetic_field ));
//	cout<<i<<k<<" "<<static_cast<int> ( magnetic_field)<<endl;
	dte0->SetPoint(count,i,amp[0]);
	dte1->SetPoint(count,i,amp[1]);
	dte2->SetPoint(count,i,amp[2]);
	dte3->SetPoint(count,i,amp[3]);
//	cout<<i<<k<<magnetic_field<<endl;
	count=count+1;
//	dte->SetPoint(count,x+1,y-1,1.2*magnetic_field);
  }

filein0.close();


TCanvas *mycanvas=new TCanvas("can_name","title",2500,800);
//mycanvas->Divide(1,5);
//for(int i=1;i<=5;i++){
//mycanvas->cd(i);
////dte->GetXaxis()->SetRangeUser(-1,5);
////dte->GetYaxis()->SetRangeUser(-1,5);
////dte->SetMarkerStyle(20);
////dte->Draw("pcol");
//myhist[i-1]->Draw("colz TEXT");
//myhist[i-1]->SetMarkerSize(3.0);
//myhist[i-1]->SetContour(20);
//myhist[i-1]->GetZaxis()->SetRangeUser(0,10);
//myhist[i-1]->GetXaxis()->SetTitle("Sampled Position(m)");
//myhist[i-1]->GetYaxis()->SetTitle("Sampled Position(m)");
//myhist[i-1]->GetYaxis()->SetTitleSize(.06);
//myhist[i-1]->GetYaxis()->SetLabelSize(.04);
//myhist[i-1]->GetXaxis()->SetLabelSize(.04);
//myhist[i-1]->GetYaxis()->SetTitleOffset(0.45);
//myhist[i-1]->GetXaxis()->SetTitleOffset(0.85);
//myhist[i-1]->GetXaxis()->SetTitleSize(.06);
//myhist[i-1]->SetStats(0);
////dte->Draw("PCOL");
////mycanvas->DrawFrame(0,0,4,1,"globaltitle;x lable;y lable");
//}
//mycanvas->cd(6);
//TLatex* txt= new TLatex(20, 100, "hello");
//txt->Draw();
mycanvas->cd();
dte3->Draw("AL");
dte1->Draw("same");
dte2->Draw("same");
dte0->Draw("same");
gPad->SetLogy();
dte0->SetLineWidth(3);
dte2->SetLineWidth(3);
dte2->SetLineWidth(3);
dte3->SetLineWidth(3);
dte0->SetLineColor(1);
dte1->SetLineColor(1);
dte1->SetLineStyle(2);
dte2->SetLineColor(kTeal+3);
dte3->SetLineColor(kRed+1);
dte3->GetXaxis()->SetTitle("time [ns]");
dte3->GetYaxis()->SetTitle("AP charge sum#times 0.5[fC]");
dte3->GetYaxis()->SetTitleOffset(0.55);
dte3->GetXaxis()->SetRangeUser(0,2000);
dte3->GetYaxis()->SetRangeUser(10,2.*16000);
TPad *pad0=new TPad("pad0","pad0",0.102,0.1,.102+5./1000,.9);
pad0->Draw();
pad0->SetFillColorAlpha(1,.1);
TPad *pad1=new TPad("pad1","pad1",0.102+5./1000,0.1,.102+50./1000,.9);
pad1->Draw();
pad1->SetBorderSize(2);
pad1->SetFillColorAlpha(3,.1);
TPad *pad2=new TPad("pad2","pad2",0.102+50./1000,0.1,.102+400./1000,.9);
pad2->Draw();
pad2->SetFillColorAlpha(1,.1);
TPad *pad3=new TPad("pad3","pad3",0.102+400./1000,0.1,.10+800./1000,.9);
pad3->Draw();
pad3->SetFillColorAlpha(3,.1);
TLine *hline=new TLine(0,13000,2000,13000);
hline->Draw();
hline->SetLineWidth(3);
gPad->Update();
 TLatex *grouptex=new TLatex();
// grouptex->DrawLatex(0,17000,"Group1");
 grouptex->DrawLatex(10,17000,"Group1");
 grouptex->DrawLatex(400,17000,"Group2");
 grouptex->DrawLatex(1240,17000,"Group3");
 TLegend *aplegend = new TLegend(0.7,0.6,0.9,0.81);
// cnn1dlegend->SetHeader("Header","C");                // option "C" allows to center the header
//gStyle->SetLegendFont(52);
aplegend->AddEntry(dte0,"HV=2328V","l");
aplegend->AddEntry(dte1,"HV=2328V,wo EMF shielding","l");
aplegend->AddEntry(dte2,"HV=2128V","l");
aplegend->AddEntry(dte3,"HV=2528V","l");
aplegend->Draw();



//mycanvas->SaveAs("ap636.eps");
mycanvas->SaveAs("ap3369.pdf");
mycanvas->SaveAs("ap3369.png");
//mycanvas->SaveAs("output/shieldmf.pdf");
return 0;
}

