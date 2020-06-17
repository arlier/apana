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
#include "TStyle.h"
#include "TH1F.h"
#include "TF1.h"
using namespace std;

int main(){
TString filedir0="/home/pmthome/aptest/Analysis_cy/restxt/apt.txt";
TString filedir1="/home/pmthome/aptest/Analysis_cy/ap636test1.txt";
TString filedir2="/home/pmthome/aptest/Analysis_cy/ap636test2.txt";
TString filedir3="/home/pmthome/aptest/Analysis_cy/ap636test3.txt";
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
int hv=0;
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
  for(i=0;i<19;i++)
  {
	getline(filein0,ss0);
	getline(filein1,ss1);
	getline(filein2,ss2);
	getline(filein3,ss3);
	istringstream is0(ss0);
	istringstream is1(ss1);
	istringstream is2(ss2);
	istringstream is3(ss3);
	is0>>hv>>amp[0]>>amp[1]>>amp[2]>>amp[3];
cout<<hv<<" "<<amp[1]<<endl;
//	is1>>x>>y>>amp[1];
//	is2>>x>>y>>amp[2];
//	is3>>x>>y>>amp[3];
//	cout<<count<<" "<<x<<" "<<y<<" "<<z<<" "<<magnetic_field<<endl;
//    if(static_cast<int> ( magnetic_field)==0)magnetic_field=magnetic_field+1;
//	myhist[j]->SetBinContent(i+1,k+1,static_cast<int> ( magnetic_field ));
//	cout<<i<<k<<" "<<static_cast<int> ( magnetic_field)<<endl;
	dte0->SetPoint(count,hv,0.01*amp[0]);
	dte1->SetPoint(count,hv,0.01*amp[1]);
	dte2->SetPoint(count,hv,0.01*amp[2]);
	dte3->SetPoint(count,hv,0.01*amp[3]);
	dte0->SetPointError(count,5,0.01);
	dte1->SetPointError(count,5,0.01);
	dte2->SetPointError(count,5,0.01);
	dte3->SetPointError(count,5,0.01);
//	cout<<i<<k<<magnetic_field<<endl;
	count=count+1;
//	dte->SetPoint(count,x+1,y-1,1.2*magnetic_field);
  }

filein0.close();


TCanvas *mycanvas=new TCanvas("can_name","title",2300,2800);
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
TPad *p1 = new TPad("p1","p1",0.1,0.7,0.9,0.901); 
   p1->SetBottomMargin(0.);            
   p1->SetBorderMode(0);
   p1->Draw();

   TPad *p2 = new TPad("p2","p2",0.1,0.5,0.9,0.70);
   p2->SetTopMargin(0.);   
   p2->SetBottomMargin(0.);            

   p2->SetBorderMode(0);
   p2->Draw();

   TPad *p3 = new TPad("p3","p3",0.1,0.3,0.9,0.50);
   p3->SetTopMargin(0.);   
   p3->SetBottomMargin(0.);            
   p3->SetBorderMode(0);
   p3->Draw();
   TPad *p4 = new TPad("p4","p4",0.1,0.1,0.9,0.30);
   p4->SetTopMargin(0.);   
   p4->SetBorderMode(0);
   p3->SetBottomMargin(.0);            
   p4->Draw();
TF1 *f1=new TF1("f1","[0]*x+[1]",1710,1830);
f1->SetParameters(0,-.0025);
f1->SetParameters(1,5);
dte0->GetYaxis()->SetTitleSize(.07);
f1->SetParLimits(0,-3e-3,-2e-3);
f1->SetParLimits(1,0,20);
f1->SetLineColor(kTeal+3);
f1->SetLineStyle(2);
f1->SetLineWidth(3);
TF1 *f2=new TF1("f2","[0]*x+[1]",1710,1830);
f2->SetParameters(0,-.0015);
f2->SetParameters(1,5);
f2->SetParLimits(0,-2.3e-3,-1e-3);
f2->SetParLimits(1,0,20);
f2->SetLineColor(kTeal+3);
f2->SetLineStyle(2);
f2->SetLineWidth(3);
TF1 *f3=new TF1("f3","(-3.418e-4)*x+1.50039",1710,1830);
//TF1 *f3=new TF1("f3","[0]*x+[1]",1720,1820);
//f3->SetParameters(0,-0.00034);
//f3->SetParameters(1,1.5);
//f3->SetParLimits(0,-0.00033,-0.00035);
//f3->SetParLimits(1,0,2);
//f3->SetLineColor(kTeal+3);
//f3->SetLineStyle(2);
//f3->SetLineWidth(2);
 p1->cd();
dte3->Draw("AP");
dte3->Fit(f1,"FR+");
   dte3->GetHistogram()->SetMinimum(4.199);        
   dte3->GetHistogram()->SetMaximum(4.8);
dte3->GetXaxis()->SetRangeUser(1600,1900.);
 TLegend *aplegend = new TLegend(0.65,0.6,0.9,0.9);
// cnn1dlegend->SetHeader("Header","C");                // option "C" allows to center the header
//gStyle->SetLegendFont(92);
aplegend->AddEntry(dte3,"afterpulse Group4","l");
aplegend->AddEntry(f1,"linear fit","l");
//aplegend->AddEntry(dte1,"HV=1740V,wo EMF shielding","l");
aplegend->Draw();
   
   p2->cd();
   dte2->Draw("AP");
dte2->Fit(f2,"FR+");
   dte2->GetHistogram()->SetMaximum(3.4);
   dte2->GetHistogram()->SetMinimum(2.8);        
dte2->GetXaxis()->SetRangeUser(1600,1900.);
 TLegend *aplegend2 = new TLegend(0.65,0.6,0.9,0.8);
//gStyle->SetLegendFont(92);
aplegend2->AddEntry(dte2,"afterpulse Group3","l");
aplegend2->AddEntry(f2,"linear fit","l");
aplegend2->Draw();
 p3->cd();
dte1->Draw("AP");
//dte1->Fit(f3,"RF+");
f3->Draw("SAME AL");
f3->SetLineColor(kTeal+3);
f3->SetLineStyle(2);
f3->SetLineWidth(3);
   dte1->GetHistogram()->SetMinimum(0.6);        
   dte1->GetHistogram()->SetMaximum(1.2);
dte1->GetXaxis()->SetRangeUser(1600,1900.);
 TLegend *aplegend3 = new TLegend(0.65,0.6,0.9,0.8);
//gStyle->SetLegendFont(92);
aplegend3->AddEntry(dte1,"afterpulse Group2","l");
aplegend3->AddEntry(f3,"linear fit","l");
aplegend3->Draw();
 p4->cd();
dte0->Draw("AP");
   dte0->GetHistogram()->SetMinimum(0.0);        
   dte0->GetHistogram()->SetMaximum(0.6);
dte0->GetXaxis()->SetRangeUser(1600,1900.);
 TLegend *aplegend0 = new TLegend(0.65,0.7,0.9,0.8);
//gStyle->SetLegendFont(92);
aplegend0->AddEntry(dte0,"afterpulse Group1","l");
aplegend0->Draw();

mycanvas->cd();
   TPad *b = new TPad("b","b",0.102,0.673,0.8099,0.72);
   b->SetBorderMode(0);
   b->Draw();
   b->cd();
   TLine *line = new TLine(0.11,0,0.1105677,0.399656);
   line->Draw();
   line = new TLine(0.1105677,0.5860092,0.11,1);
   line->Draw();
   line = new TLine(0.096639,0.5143349,0.1324797,0.6863532);
   line->Draw();
   line = new TLine(0.096639,0.3423165,0.1324797,0.5143349);
   line->Draw();

mycanvas->cd();
   TPad *b1 = new TPad("b1","b1",0.102,0.475,0.8099,0.523);
   b1->SetBorderMode(0);
   b1->Draw();
   b1->cd();
   line = new TLine(0.11,0,0.1105677,0.399656);
   line->Draw();
   line = new TLine(0.1105677,0.5860092,0.11,1);
   line->Draw();
   line = new TLine(0.096639,0.5143349,0.1324797,0.6863532);
   line->Draw();
   line = new TLine(0.096639,0.3423165,0.1324797,0.5143349);
   line->Draw();
//dte1->Draw("same P");
//dte2->Draw("same P");
//dte0->Draw("same P");
//gPad->SetLogy();
//dte0->SetMarkerSize(3);
//dte2->SetMarkerSize(3);
//dte2->SetMarkerSize(3);
//dte3->SetMarkerSize(3);
//gStyle->SetTitleFontSize(.8);
dte0->SetMarkerColor(1);
dte1->SetMarkerColor(2);
dte2->SetMarkerColor(kTeal+3);
dte3->SetMarkerColor(kRed+1);
dte0->SetMarkerStyle(20);
dte1->SetMarkerStyle(21);
dte2->SetMarkerStyle(22);
dte3->SetMarkerStyle(23);
dte0->GetXaxis()->SetTitle("HV [V]");
dte3->GetYaxis()->SetTitle("AP signal time[#mus]");
dte2->GetYaxis()->SetTitle("AP signal time[#mus]");
dte1->GetYaxis()->SetTitle("AP signal time[#mus]");
dte0->GetYaxis()->SetTitle("AP signal time[#mus]");
dte3->GetYaxis()->SetLabelSize(.06);
dte2->GetYaxis()->SetLabelSize(.06);
dte1->GetYaxis()->SetLabelSize(.06);
dte0->GetYaxis()->SetLabelSize(.06);
dte3->GetYaxis()->SetTitleSize(.07);
dte2->GetYaxis()->SetTitleSize(.07);
dte1->GetYaxis()->SetTitleSize(.07);
dte0->GetYaxis()->SetTitleSize(.07);
dte0->GetXaxis()->SetTitleSize(.07);
dte0->GetXaxis()->SetLabelSize(.06);
mycanvas->Update();
dte0->GetYaxis()-> CenterTitle();
dte1->GetYaxis()-> CenterTitle();
dte2->GetYaxis()-> CenterTitle();
dte3->GetYaxis()-> CenterTitle();
dte3->GetYaxis()->SetTitleOffset(0.55);
dte2->GetYaxis()->SetTitleOffset(0.55);
dte1->GetYaxis()->SetTitleOffset(0.55);
dte0->GetYaxis()->SetTitleOffset(0.55);
dte0->GetXaxis()->SetTitleOffset(0.55);
//dte3->GetYaxis()->SetRangeUser(100,2.*16000);
//TPad *pad0=new TPad("pad0","pad0",0.102,0.1,.102+45./1000,.9);
//pad0->Draw();
//pad0->SetFillColorAlpha(1,.1);
//TPad *pad1=new TPad("pad1","pad1",0.102+45./1000,0.1,.102+160./1000,.9);
//pad1->Draw();
//pad1->SetBorderSize(2);
//pad1->SetFillColorAlpha(3,.1);
//TPad *pad2=new TPad("pad2","pad2",0.102+160./1000,0.1,.102+320./1000,.9);
//pad2->Draw();
//pad2->SetFillColorAlpha(1,.1);
//TPad *pad3=new TPad("pad3","pad3",0.102+320./1000,0.1,.102+450./1000,.9);
//pad3->Draw();
//pad3->SetFillColorAlpha(3,.1);
//TLine *hline=new TLine(0,13000,1000,13000);
//hline->Draw();
//hline->SetLineWidth(3);
//gPad->Update();
// TLatex *grouptex=new TLatex();
// grouptex->DrawLatex(0,17000,"Group1");
// grouptex->DrawLatex(100,17000,"Group2");
// grouptex->DrawLatex(260,17000,"Group3");
// grouptex->DrawLatex(440,17000,"Group4");
// TLegend *aplegend = new TLegend(0.7,0.586,0.9,0.775);
//// cnn1dlegend->SetHeader("Header","C");                // option "C" allows to center the header
////gStyle->SetLegendFont(52);
//aplegend->AddEntry(dte0,"HV=1940V,setpoint1000","l");
//aplegend->AddEntry(dte2,"HV=1940V,setpoint2000","l");
//aplegend->AddEntry(dte3,"HV=1940V,setpoint3000","l");
////aplegend->AddEntry(dte1,"HV=1740V,wo EMF shielding","l");
//aplegend->Draw();



//mycanvas->SaveAs("ap636.eps");
mycanvas->SaveAs("apts.eps");
mycanvas->SaveAs("apts.png");
//mycanvas->SaveAs("output/shieldmf.pdf");
return 0;
}

