
void avefilter(TH1F *h1,int np){
int nbins=h1->GetNbinsX();
//cout<<"d"<<nbins<<endl;
int *arr=new int[nbins];
int *arrnew=new int[nbins];
for(int i=0;i<nbins;i++)
{arr[i]=h1->GetBinContent(i);
arrnew[i]=0;
}
//int inp=int(np/2.);
for(int i=np;i<nbins-1-np;i++)
{
for(int j=0;j<2*np;j++){arrnew[i]+=arr[i-np+j];}//may need to divide the np
h1->SetBinContent(i,(int)arrnew[i]*.5/np);
}
}

// 
Int_t Drawwaveform(const char* date,const char* PMTid){
	 TFile *f1;
	TCanvas *c1 = new TCanvas("c1","c1",2500,3000);
	c1->Divide(1,5,1,0);
	gStyle->SetOptStat(0);
	gStyle->SetPalette(84);
	gStyle->SetLegendFont(52);
	TH1F * primaryPulseChargeHist[__NumOfSetpoint];
	TH1F * averageWaveform;
	TGraph *wave1d[__NumOfSetpoint];
	TH2F * wave2d[__NumOfSetpoint];
	char pad_name[10];
	TPad * p1;
	TLatex* ltx1 = new TLatex();

    for(int ii = 0; ii < __NumOfSetpoint; ii++)
    {
        string filename =  string("/home/pmthome/aptest/Resultsroot/APTest_") + string(date) + string("_") + string(PMTid) + string("_") + to_string(ii) + string("_OUT.root");
        cout<<"draw waveforms:: Current Processing File Name: "<<filename<<endl;
        f1 = TFile::Open(filename.c_str(),"READ");
//        TTree * primaryPulseTree = (TTree*) f1->Get("primaryPulseTree");
//        primaryPulseTree->Draw("primaryPulseCharge>>primaryPulseChargeHist");
//        primaryPulseChargeHist = (TH1F*) gDirectory->Get("primaryPulseChargeHist");
averageWaveform = (TH1F*) f1->Get("averageWaveform");
wave1d[ii]=new TGraph();
for(int kk=0;kk<__WaveLength;kk++)wave1d[ii]->SetPoint(kk,kk,averageWaveform->GetBinContent(kk)+ii*1500);
// the 2D waveform
wave2d[ii]=(TH2F*) f1->Get("Waveform2d");
//cout<<averageWaveform[ii]->GetSize()<<endl;
c1->cd(1);
 gPad->SetRightMargin(0.01);
wave1d[ii]->Draw();
wave1d[ii]->SetLineColor(kTeal+3);
wave1d[ii]->SetMarkerColor(kTeal+3);
wave1d[0]->GetYaxis()->SetRangeUser(-200,6000);
wave1d[0]->GetYaxis()->SetTitle("U[mV]");
wave1d[0]->GetXaxis()->SetTitle("t[ns]");
 ltx1->DrawLatex(18000,600+ii*1500,Form("light intensity %d",ii+1));
gPad->SetGridy();
//gPad->SetLogy();
gPad->Update();
c1->cd(ii+2);
gPad->SetRightMargin(0.04);
//gPad->SetBottomMargin(0.2);
   //wave2d[ii]->SetContour(nb);
wave2d[ii]->Draw("colz");
gPad->SetLogz();
wave2d[ii]->SetFillStyle(4050);
wave2d[ii]->GetYaxis()->SetTitle("U[mV]");
wave2d[ii]->GetXaxis()->SetTitle("t[ns]");
wave2d[ii]->SetTitle(Form("light intensity %d",ii+1));
string snname="SN: "+string(PMTid);
TLatex *bgtex1 = new TLatex(8563,.251,snname.c_str());
bgtex1->Draw();
bgtex1->SetTextColorAlpha(8, 0.276);
bgtex1->SetTextSize(0.191525);
//        primaryPulseChargeHist->Fit("gaus","","",10,400);
//
//	sprintf(pad_name, "c2_%d",ii+1);
//        p1 = (TPad *) c2->GetPrimitive(pad_name);
//        p1->cd();
//
//        TH1F * averageWaveform = (TH1F*) f1->Get("averageWaveform");
//        averageWaveform->GetXaxis()->SetRange(0,1000);
//        averageWaveform->Draw();
     }
string avecname(PMTid);
avecname+="_avewave.png";
c1->SaveAs(avecname.c_str());
//c2->SaveAs("test2.png");
return 0;
}
 
Int_t Apanalysis(const char* date,const char* PMTid){
//risetime ,apQDC sum,ap amp,ap number, apratio
string sn="EA";
string pmtsn=string(PMTid);
TString outtxtname="./restxt/"+pmtsn+".txt";
string::size_type idx;
idx=pmtsn.find(sn);
int Eatag=0;
if(idx == string::npos ){cout << "EA not found\n";}
else{cout <<"EA found\n";Eatag=1; }
TFile *f1;
TCanvas *c1 = new TCanvas("c1","c1",3000,1800);
TCanvas *c2 = new TCanvas("c2","c2",3000,1000);
c1->Divide(4,3);
gStyle->SetOptStat(0);
gStyle->SetPalette(84);
double primaryPulseBaseline;
double primaryPulsePeak;
double primaryPulseCharge;
double afterPulsePeak;
double afterPulseCharge;
double afterPulseRelCharge;
double afterPulseTotalCharge;
double apqsum[__NumOfSetpoint]={0};
double ppqsum[__NumOfSetpoint]={0};
double apqvstsum[__NumOfSetpoint][__WaveLength]={0};
int afterPulseTcross;
int afterPulseRate;
int aprsum[4]={0};
int mycolor[3]={8,12,46};
    TH1F * Hist_bsl[__NumOfSetpoint];
    TH1F * Hist_ppp[__NumOfSetpoint];
    TH1F * Hist_ppc[__NumOfSetpoint];
    TH1F * Hist_aptc[__NumOfSetpoint];
    TH1F * Hist_app[__NumOfSetpoint];
    TH1F * Hist_apc[__NumOfSetpoint];
    TH1F * Hist_aprc[__NumOfSetpoint];
    TH1F * Hist_apcs[__NumOfSetpoint];
    TH1F * Hist_apr[__NumOfSetpoint];
    TH1F * Hist_apqsum[__NumOfSetpoint];
    TH1F * Hist_apqsumaf[__NumOfSetpoint];
TPaveStats* bslps[__NumOfSetpoint];
    TH2F * Hist_2damp;
    TH2F * Hist_2dqdc;
    TH2F * Hist_2daq;
TLegend *bsllegend = new TLegend(0.1,0.7,0.55,0.9);
TLegend *ppplegend = new TLegend(0.1,0.7,0.55,0.9);
TLegend *ppclegend = new TLegend(0.1,0.7,0.55,0.9);
TLegend *aptlegend = new TLegend(0.5,0.7,.9,0.9);
TLegend *applegend = new TLegend(0.5,0.7,.9,0.9);
TLegend *apclegend = new TLegend(0.5,0.7,.9,0.9);
TLegend *aprclegend = new TLegend(0.5,0.7,.9,0.9);
TLegend *aptclegend = new TLegend(0.5,0.7,.9,0.9);
TLegend *aprlegend = new TLegend(0.5,0.7,.9,0.9);
TLegend *apqslegend = new TLegend(0.5,0.7,.9,0.9);

    for(int ii = 0; ii < __NumOfSetpoint; ii++)
    {
        string filename =  string("/home/pmthome/aptest/Resultsroot/APTest_") + string(date) + string("_") + string(PMTid) + string("_") + to_string(ii) + string("_OUT.root");
        cout<<"draw after-pulse histograms:: Current Processing File Name: "<<filename<<endl;
        f1 = TFile::Open(filename.c_str(),"READ");
	Hist_bsl[ii]=new TH1F("bsl","baseline",200,910,930);
	if(Eatag==1){Hist_ppp[ii]=new TH1F("ppp","primary pulse peak",200,0,950);}
	else {Hist_ppp[ii]=new TH1F("ppp","primary pulse peak",200,0,850);}
	Hist_ppc[ii]=new TH1F("ppc","primary pulse charge",200,0,500);
	Hist_aptc[ii]=new TH1F("aptc","after pulse hit time",200,0,__WaveLength);
	Hist_app[ii]=new TH1F("app","after pulse Peak value",200,0,100);
	Hist_apc[ii]=new TH1F("apc","after pulse charge value",100,0,30);
	Hist_aprc[ii]=new TH1F("aprc","after pulse relative charge value",100,0,.304);
	Hist_apcs[ii]=new TH1F("apcs","after pulse total charge ",40,.2,60);
	Hist_apr[ii]=new TH1F("apr","after pulse rate ",11,0,11);
	Hist_apqsum[ii]=new TH1F("apqsum"," afeter-pulse charge sum",__WaveLength,0,__WaveLength);
	Hist_2damp=new TH2F("h2damp","amplitude and hit time",100,0,__WaveLength,100,0,150);
	Hist_2dqdc=new TH2F("h2dqdc","charge and hit time",100,0,__WaveLength,100,0,50);
	Hist_2daq=new TH2F("2daq","charge and amplitude",100,0,300,100,0,150);
        TTree * primaryPulseTree = (TTree*) f1->Get("primaryPulseTree");
primaryPulseTree->SetBranchAddress("primaryPulseBaseline",&primaryPulseBaseline);
primaryPulseTree->SetBranchAddress("primaryPulsePeak",&primaryPulsePeak);
primaryPulseTree->SetBranchAddress("primaryPulseCharge",&primaryPulseCharge);
        TTree * afterPulseTreePerAP = (TTree*) f1->Get("afterPulseTreePerAP");
afterPulseTreePerAP->SetBranchAddress("afterPulseTpeak",&afterPulseTcross);
//afterPulseTreePerAP->SetBranchAddress("afterPulseTcross",&afterPulseTcross);
afterPulseTreePerAP->SetBranchAddress("afterPulsePeak",&afterPulsePeak);
afterPulseTreePerAP->SetBranchAddress("afterPulseCharge",&afterPulseCharge);
afterPulseTreePerAP->SetBranchAddress("afterPulseRelCharge",&afterPulseRelCharge);
        TTree * afterPulseTreePerPP = (TTree*) f1->Get("afterPulseTreePerPP");
afterPulseTreePerPP->SetBranchAddress("afterPulseTotalCharge",&afterPulseTotalCharge);
afterPulseTreePerPP->SetBranchAddress("afterPulseRate",&afterPulseRate);
if(ii==0){
for(int ll=0;ll<afterPulseTreePerAP->GetEntries();ll++){
afterPulseTreePerAP->GetEntry(ll);
Hist_aptc[ii]->Fill(afterPulseTcross);
if(afterPulseTcross<20000)apqsum[ii]+=afterPulseCharge;
apqvstsum[ii][afterPulseTcross]+=afterPulseCharge;
}
for(int tc=0;tc<__WaveLength-50;tc++){
Hist_apqsum[ii]->SetBinContent(tc,(int)(apqvstsum[ii][tc]));
}
Hist_apr[ii]->Fill(afterPulseRate);
//aprsum[ii]+=Hist_apr[ii]->Integral();
aprsum[ii]+=afterPulseTreePerAP->GetEntries();
cout<<"aprsum1is:"<<aprsum[0]<<endl;
}
if (ii>0){
for(int ll=0;ll<__NumOfWave;ll++){
primaryPulseTree->GetEntry(ll);
afterPulseTreePerAP->GetEntry(ll);
afterPulseTreePerPP->GetEntry(ll);
Hist_bsl[ii]->Fill(primaryPulseBaseline);
Hist_ppp[ii]->Fill(primaryPulseBaseline-primaryPulsePeak);
Hist_ppc[ii]->Fill(primaryPulseCharge);
if(ll<afterPulseTreePerAP->GetEntries()){
Hist_aptc[ii]->Fill(afterPulseTcross);
Hist_app[ii]->Fill(afterPulsePeak);
Hist_apc[ii]->Fill(afterPulseCharge);
Hist_aprc[ii]->Fill(afterPulseRelCharge);
Hist_apcs[ii]->Fill(afterPulseTotalCharge);
apqvstsum[ii][afterPulseTcross]+=afterPulseCharge;
}
ppqsum[ii]+=primaryPulseCharge;

Hist_apr[ii]->Fill(afterPulseRate);
Hist_2damp->Fill(afterPulseTcross,afterPulsePeak);
Hist_2dqdc->Fill(afterPulseTcross,afterPulseCharge);
Hist_2daq->Fill(afterPulsePeak,afterPulseCharge);
}
//aprsum[ii]+=Hist_apr[ii]->Integral();
aprsum[ii]+=afterPulseTreePerAP->GetEntries();
cout<<"aprsumis:"<<aprsum[ii]<<endl;
for(int tc=0;tc<__WaveLength-50;tc++){
Hist_apqsum[ii]->SetBinContent(tc,(int)(apqvstsum[ii][tc]));
}
Hist_apqsum[ii]->SetLineColor(mycolor[ii-1]);
Hist_apqsum[ii]->SetLineWidth(1);
Hist_apqsum[ii]->SetName(Form("apqsum%d",ii));
for(int ll=0;ll<afterPulseTreePerAP->GetEntries();ll++){
afterPulseTreePerAP->GetEntry(ll);
if(afterPulseTcross<20000)apqsum[ii]+=afterPulseCharge;
}
//cout<<apqsum[0]<<" "<<apqsum[1]<<" "<<apqsum[2]<<" "<<apqsum[3]<<endl;
//cout<<apqsum[0]<<" "<<ppqsum[1]<<" "<<ppqsum[2]<<" "<<ppqsum[3]<<endl;
//cout<<"c1:"<<(apqsum[1]-apqsum[0])/ppqsum[1]<<endl;
//cout<<"c2:"<<(apqsum[2]-apqsum[0])/ppqsum[2]<<endl;
//cout<<"c3:"<<(apqsum[3]-apqsum[0])/ppqsum[3]<<endl;
Hist_aptc[ii]->Add(Hist_aptc[0],-1);


c1->cd(1);
gPad->SetGrid();
if(ii==1)Hist_bsl[ii]->Draw();
else Hist_bsl[ii]->Draw("SAME");
gPad->Update();
Hist_bsl[ii]->SetLineColor(mycolor[ii-1]);
Hist_bsl[ii]->SetLineWidth(3);
Hist_bsl[ii]->GetYaxis()->SetTitle("count");
Hist_bsl[ii]->GetXaxis()->SetTitle("U[mV]");
Hist_bsl[1]->GetYaxis()->SetRangeUser(0,1.2*Hist_bsl[1]->GetMaximum());
Hist_bsl[1]->GetXaxis()->SetRangeUser(Hist_bsl[1]->GetMean()-20,20+1.*Hist_bsl[1]->GetMean());
Hist_bsl[ii]->SetName(Form("bsl%d",ii));
bsllegend->AddEntry(Form("bsl%d",ii),Form("@setpoint %d",ii*1000),"l");
bsllegend->Draw();
bsllegend->SetTextSize(0.05);
//gPad->Update();
// bslps[ii-1]= (TPaveStats*)(Hist_bsl[ii]->FindObject("stats"));
// bslps[ii-1]->SetX1NDC(.1);
 //bslps[ii-1]->SetX2NDC(.3);
 //bslps[ii-1]->SetY1NDC(.7-(ii)*.1);
 //bslps[ii-1]->SetY2NDC(.9-(ii)*.1);
 //bslps[ii-1]->SetTextColor(kTeal+3);

c1->cd(2);
gPad->SetGrid();
if(ii==1)Hist_ppp[ii]->Draw();
else Hist_ppp[ii]->Draw("SAME");
gPad->Update();
Hist_ppp[ii]->SetLineColor(mycolor[ii-1]);
Hist_ppp[ii]->SetLineWidth(3);
Hist_ppp[ii]->GetYaxis()->SetTitle("count");
Hist_ppp[ii]->GetXaxis()->SetTitle("U[mV]");
Hist_ppp[1]->GetYaxis()->SetRangeUser(0,1.2*Hist_ppp[1]->GetMaximum());
Hist_ppp[ii]->SetName(Form("ppp%d",ii));
ppplegend->AddEntry(Form("ppp%d",ii),Form("@setpoint %d",ii*1000),"l");
ppplegend->Draw();
ppplegend->SetTextSize(0.05);
c1->cd(3);
gPad->SetGrid();
if(ii==1)Hist_ppc[ii]->Draw();
else Hist_ppc[ii]->Draw("SAME");
gPad->Update();
Hist_ppc[ii]->SetLineColor(mycolor[ii-1]);
Hist_ppc[ii]->SetLineWidth(3);
Hist_ppc[ii]->GetYaxis()->SetTitle("count");
Hist_ppc[ii]->GetXaxis()->SetTitle("QDC[pC]");
Hist_ppc[1]->GetYaxis()->SetRangeUser(0,1.2*Hist_ppc[1]->GetMaximum());
Hist_ppc[ii]->SetName(Form("ppc%d",ii));
ppclegend->AddEntry(Form("ppc%d",ii),Form("@setpoint %d",ii*1000),"l");
ppclegend->Draw();
ppclegend->SetTextSize(0.05);
c1->cd(4);
gPad->SetGrid();
//Hist_aptc[0]->Draw("PC");
if(ii==1)Hist_aptc[ii]->Draw("PC");
else Hist_aptc[ii]->Draw("SAME PC");
gPad->Update();
Hist_aptc[ii]->SetLineColor(mycolor[ii-1]);
Hist_aptc[ii]->SetLineWidth(1);
Hist_aptc[ii]->GetYaxis()->SetTitle("count");
Hist_aptc[ii]->GetXaxis()->SetTitle("t[ns]");
Hist_aptc[1]->GetYaxis()->SetRangeUser(0.001,1.5*Hist_aptc[1]->GetMaximum());
Hist_aptc[ii]->GetXaxis()->SetNdivisions(105,kTRUE);
Hist_aptc[ii]->SetName(Form("apt%d",ii));
//gPad->SetLogy();
aptlegend->AddEntry(Form("apt%d",ii),Form("@setpoint %d",ii*1000),"l");
aptlegend->Draw();
aptlegend->SetTextSize(0.05);
c1->cd(5);
gPad->SetGrid();
if(ii==1)Hist_app[ii]->Draw();
else Hist_app[ii]->Draw("SAME");
gPad->Update();
Hist_app[ii]->SetLineColor(mycolor[ii-1]);
Hist_app[ii]->SetLineWidth(3);
Hist_app[ii]->GetYaxis()->SetTitle("count");
Hist_app[ii]->GetXaxis()->SetTitle("U[mV]");
Hist_app[1]->GetYaxis()->SetRangeUser(0,1.3*Hist_app[1]->GetMaximum());
Hist_app[ii]->SetName(Form("app%d",ii));
applegend->AddEntry(Form("app%d",ii),Form("@setpoint %d",ii*1000),"l");
applegend->Draw();
applegend->SetTextSize(0.05);
c1->cd(6);
gPad->SetGrid();
if(ii==1)Hist_apc[ii]->Draw();
else Hist_apc[ii]->Draw("SAME");
gPad->Update();
Hist_apc[ii]->SetLineColor(mycolor[ii-1]);
Hist_apc[ii]->SetLineWidth(3);
Hist_apc[ii]->GetYaxis()->SetTitle("count");
Hist_apc[ii]->GetXaxis()->SetTitle("charge[pC]");
Hist_apc[1]->GetYaxis()->SetRangeUser(0,1.3*Hist_apc[1]->GetMaximum());
Hist_apc[ii]->SetName(Form("apc%d",ii));
apclegend->AddEntry(Form("apc%d",ii),Form("@setpoint %d",ii*1000),"l");
apclegend->Draw();
apclegend->SetTextSize(0.05);
c1->cd(7);
gPad->SetGrid();
if(ii==1)Hist_aprc[ii]->Draw();
else Hist_aprc[ii]->Draw("SAME");
gPad->Update();
Hist_aprc[ii]->SetLineColor(mycolor[ii-1]);
Hist_aprc[ii]->SetLineWidth(3);
Hist_aprc[ii]->GetYaxis()->SetTitle("count");
Hist_aprc[ii]->GetXaxis()->SetTitle("ratio");
Hist_aprc[1]->GetYaxis()->SetRangeUser(1,1.8*Hist_aprc[1]->GetMaximum());
//gPad->SetLogy();
Hist_aprc[ii]->SetName(Form("aprc%d",ii));
aprclegend->AddEntry(Form("aprc%d",ii),Form("@setpoint %d",ii*1000),"l");
aprclegend->Draw();
aprclegend->SetTextSize(0.05);
c1->cd(8);
gPad->SetGrid();
if(ii==1)Hist_apcs[ii]->Draw();
else Hist_apcs[ii]->Draw("SAME");
gPad->Update();
Hist_apcs[ii]->SetLineColor(mycolor[ii-1]);
Hist_apcs[ii]->SetLineWidth(3);
Hist_apcs[ii]->GetYaxis()->SetTitle("count");
Hist_apcs[ii]->GetXaxis()->SetTitle("charge[pC]");
Hist_apcs[1]->GetYaxis()->SetRangeUser(0,1.2*Hist_apcs[1]->GetMaximum());
Hist_apcs[ii]->SetName(Form("apcs%d",ii));
aptclegend->AddEntry(Form("apcs%d",ii),Form("@setpoint %d",ii*1000),"l");
aptclegend->Draw();
aptclegend->SetTextSize(0.05);
//c1->cd(11);
//gPad->SetGrid();
//if(ii==1)Hist_apr[ii]->Draw();
//else Hist_apr[ii]->Draw("SAME");
//gPad->Update();
//Hist_apr[ii]->SetLineColor(mycolor[ii-1]);
//Hist_apr[ii]->SetLineWidth(3);
//Hist_apr[ii]->GetYaxis()->SetTitle("count");
//Hist_apr[ii]->GetXaxis()->SetTitle("afterpulse count");
////Hist_apr[1]->GetYaxis()->SetRangeUser(0,1.2*Hist_apr[1]->GetMaximum());
//Hist_apr[ii]->SetName(Form("apr%d",ii));
}
	}// the setpoint loop
c2->cd();
gPad->SetGrid();
Hist_apqsumaf[3]=(TH1F*)Hist_apqsum[3]->Rebin(10)->Clone();
Hist_apqsumaf[2]=(TH1F*)Hist_apqsum[2]->Rebin(10)->Clone();
Hist_apqsumaf[1]=(TH1F*)Hist_apqsum[1]->Rebin(10)->Clone();
Hist_apqsumaf[0]=(TH1F*)Hist_apqsum[0]->Rebin(10)->Clone();
avefilter(Hist_apqsumaf[3],10);
avefilter(Hist_apqsumaf[2],10);
avefilter(Hist_apqsumaf[1],10);
avefilter(Hist_apqsumaf[0],10);
Hist_apqsumaf[3]->Add(Hist_apqsumaf[0],-1);
Hist_apqsumaf[2]->Add(Hist_apqsumaf[0],-1);
Hist_apqsumaf[1]->Add(Hist_apqsumaf[0],-1);
Hist_apqsumaf[3]->Draw("PC");
Hist_apqsumaf[3]->GetXaxis()->SetTitle("time[ns]");
Hist_apqsumaf[3]->GetXaxis()->SetRangeUser(0,10000);
Hist_apqsumaf[3]->GetYaxis()->SetTitle("charge-sumx50[pC]");
Hist_apqsumaf[3]->GetYaxis()->SetTitleOffset(1.2);
Hist_apqsumaf[3]->GetYaxis()->SetNdivisions(5, kFALSE);
TGaxis::SetMaxDigits(3);
Hist_apqsumaf[2]->Draw("SAME PC" );
Hist_apqsumaf[1]->Draw("SAME PC");
Hist_apqsumaf[1]->SetLineWidth(3);
Hist_apqsumaf[2]->SetLineWidth(3);
Hist_apqsumaf[3]->SetLineWidth(3);
int apgt[4]={0};
int apgv[4]={0};
for(int i=0;i<Hist_apqsumaf[3]->GetNbinsX();i++){
if(i<60&&Hist_apqsumaf[3]->GetBinContent(i)>apgv[0]){apgt[0]=i;apgv[0]=Hist_apqsumaf[3]->GetBinContent(i);}
if(i>60&&i<140&&Hist_apqsumaf[3]->GetBinContent(i)>apgv[1]){apgt[1]=i;apgv[1]=Hist_apqsumaf[3]->GetBinContent(i);}
if(i>140&&i<400&&Hist_apqsumaf[3]->GetBinContent(i)>apgv[2]){apgt[2]=i;apgv[2]=Hist_apqsumaf[3]->GetBinContent(i);}
if(i>400&&i<550&&Hist_apqsumaf[3]->GetBinContent(i)>apgv[3]){apgt[3]=i;apgv[3]=Hist_apqsumaf[3]->GetBinContent(i);}
}
TLine *l0=new TLine(apgt[0]*10,0,apgt[0]*10,apgv[0]*1.1);
l0->Draw();
TLine *l1=new TLine(apgt[1]*10,0,apgt[1]*10,apgv[1]*1.1);
l1->Draw();
TLine *l2=new TLine(apgt[2]*10,0,apgt[2]*10,apgv[2]*1.1);
l2->Draw();
TLine *l3=new TLine(apgt[3]*10,0,apgt[3]*10,apgv[3]*1.1);
l3->Draw();
apqslegend->AddEntry(Form("apqsum%d",1),Form("@setpoint %d",1*1000),"l");
apqslegend->AddEntry(Form("apqsum%d",2),Form("@setpoint %d",2*1000),"l");
apqslegend->AddEntry(Form("apqsum%d",3),Form("@setpoint %d",3*1000),"l");
apqslegend->Draw();
apqslegend->SetTextSize(0.05);
//gPad->SetLogy();
string avecname2(PMTid);
avecname2+="_apstatszoom.png";
c2->SaveAs(avecname2.c_str());
c1->cd(11);
gPad->SetGrid();
Hist_apqsum[3]->Rebin(10);
Hist_apqsum[2]->Rebin(10);
Hist_apqsum[1]->Rebin(10);
Hist_apqsum[0]->Rebin(10);
Hist_apqsum[3]->Add(Hist_apqsum[0],-1);
Hist_apqsum[2]->Add(Hist_apqsum[0],-1);
Hist_apqsum[1]->Add(Hist_apqsum[0],-1);
Hist_apqsum[3]->Draw("PC");
Hist_apqsum[3]->GetXaxis()->SetTitle("time[ns]");
Hist_apqsum[3]->GetYaxis()->SetTitle("charge-sumx50[pC]");
Hist_apqsum[3]->GetYaxis()->SetRangeUser(0,1.2*Hist_apqsum[3]->GetMaximum());
Hist_apqsum[3]->GetYaxis()->SetTitleOffset(1.2);
Hist_apqsum[3]->GetYaxis()->SetNdivisions(5, kFALSE);
//TGaxis *myX = (TGaxis*)Hist_apqsum[3]->GetYaxis();
//myX->SetMaxDigits(2);
//TGaxis::SetMaxDigits(2);
TGaxis::SetMaxDigits(3);
Hist_apqsum[2]->Draw("SAME PC" );
Hist_apqsum[1]->Draw("SAME PC");
//apqslegend->AddEntry(Form("apqsum%d",1),Form("@setpoint %d",1*1000),"l");
//apqslegend->AddEntry(Form("apqsum%d",2),Form("@setpoint %d",2*1000),"l");
//apqslegend->AddEntry(Form("apqsum%d",3),Form("@setpoint %d",3*1000),"l");
apqslegend->Draw();
apqslegend->SetTextSize(0.05);
//aprlegend->AddEntry(Form("apr%d",ii),Form("@setpoint %d",ii*1000),"l");
//aprlegend->Draw();
c1->cd(9);
Hist_2damp->Draw("colz");
gPad->SetLogz();
Hist_2damp->GetYaxis()->SetTitle("amplitude[mV]");
Hist_2damp->GetXaxis()->SetTitle("time[ns]");
Hist_2damp->GetXaxis()->SetNdivisions(105,kTRUE);
c1->cd(10);
Hist_2dqdc->Draw("colz");
gPad->SetLogz();
Hist_2dqdc->GetYaxis()->SetTitle("charge[pC]");
Hist_2dqdc->GetXaxis()->SetTitle("time[ns]");
Hist_2dqdc->GetXaxis()->SetNdivisions(105,kTRUE);
c1->cd(12);
Hist_2daq->Draw("colz");
gPad->SetLogz();
Hist_2daq->GetXaxis()->SetTitle("amplitude[mV]");
Hist_2daq->GetYaxis()->SetTitle("QDC[pC]");
Hist_2daq->GetXaxis()->SetNdivisions(105,kTRUE);
string mysnname(PMTid);
mysnname=" SN:"+mysnname;
c1->cd();
 TPad *mypad1 = new TPad("mypad1","This is mypad1",0.01,0.00,0.95,0.95);
 mypad1->Draw();
 mypad1->SetFillStyle(4000);
 mypad1->cd();
 gPad->SetFillStyle(0);
 gPad->SetFrameFillStyle(0);
//double apcratio=(apqsum[1]-apqsum[0]+apqsum[2]-apqsum[0]+apqsum[3]-apqsum[0])*1./(ppqsum[1]+ppqsum[2]+ppqsum[3]);
double apcratio=(apqsum[1]-apqsum[0])*1./ppqsum[1]+(apqsum[2]-apqsum[0])*1./ppqsum[2]+(apqsum[3]-apqsum[0])*1./ppqsum[3];
cout<<"the final afterpulse charge ratio is:"<<apcratio*100/3.<<"%"<<endl;
apcratio=apcratio/3.;
TLatex *bgtex = new TLatex(.23,.662,Form("After-pulse ratio:%3.2f %%",apcratio*100));
bgtex->Draw();
bgtex->SetTextColorAlpha(8, 0.276);
bgtex->SetTextSize(0.1091524);
 TLatex *bgtex1 = new TLatex(.263,.31,mysnname.c_str());
 bgtex1->Draw();
 bgtex1->SetTextColorAlpha(8, 0.276);
 bgtex1->SetTextSize(0.1191525);
ofstream outtxt(outtxtname);
outtxt<<apgt[0]<<" "<<apgt[1]<<" "<<apgt[2]<<" "<<apgt[3]<<" "<<apcratio<<" "<<(apqsum[1]-apqsum[0])*1./ppqsum[1]<<" "<<(apqsum[2]-apqsum[0])*1./ppqsum[2]<<" "<<(apqsum[3]-apqsum[0])*1./ppqsum[3]<<" "<<ppqsum[1]*1./__NumOfWave<<" "<<ppqsum[2]*1./__NumOfWave<<" "<<ppqsum[3]*1./__NumOfWave;
//outtxt<<apgt[0]<<" "<<apgt[1]<<" "<<apgt[2]<<" "<<apgt[3]<<" "<<apcratio<<" "<<(apqsum[1]-apqsum[0])*1./ppqsum[1]<<" "<<(apqsum[2]-apqsum[0])*1./ppqsum[2]<<" "<<(apqsum[3]-apqsum[0])*1./ppqsum[3]<<endl;
//ofstream outap("outap.txt");
//for(int aa=0;aa<Hist_apqsumaf[3]->GetNbinsX();aa++){
//outap<<Hist_apqsumaf[1]->GetBinContent(aa)<<" ";
//outap<<Hist_apqsumaf[2]->GetBinContent(aa)<<" ";
//outap<<Hist_apqsumaf[3]->GetBinContent(aa)<<" ";
//outap<<endl;
//}
outtxt<<" "<<(aprsum[1]-aprsum[0])*1.6/Hist_ppc[1]->GetMean()/__NumOfWave<<" "<<(aprsum[2]-aprsum[0])*1.6/Hist_ppc[2]->GetMean()/__NumOfWave<<" "<<(aprsum[3]-aprsum[0])*1.6/Hist_ppc[3]->GetMean()/__NumOfWave<<endl;
string avecname(PMTid);
avecname+="_apstats.png";
c1->SaveAs(avecname.c_str());

TFile *posterfile = new TFile("aptime.root","RECREATE");
posterfile->cd();
//Hist_aptc[1]->Draw("PC");
Hist_aptc[0]->Write();
Hist_aptc[1]->Write();
Hist_aptc[2]->Write();
Hist_aptc[3]->Write();
//Hist_2dqdc->Draw("colz");
Hist_2dqdc->Write();


  posterfile->Print();

return 0;
}
