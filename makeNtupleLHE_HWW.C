#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TBenchmark.h>
#include <TSystem.h>
#include <TNtuple.h>
#include <TH1D.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <TLorentzVector.h>
#include <assert.h>
#include <iomanip>
#endif

double DeltaPhi(double phi1, double phi2);

// cmsStage /store/lhe/7420/SMHiggsWW_125_JHU_8TeV.lhe ./ 0.006758684611
// cmsStage /store/lhe/9744/WWSS_noH.lhe ./ 0.0215738
// cmsStage /store/lhe/9745/WWSS_wH.lhe ./  0.0286444
// /afs/cern.ch/user/a/anlevin/public/forGuillelmo18Nov2013/ww_to_ll_same_sign_anom_8_tev_0_tev-4.lhe
void makeNtupleLHE_HWW(TString pathDir="./",
	           TString infname="SMHiggsWW_125_JHU_8TeV.lhe",
		   double weight = 1.0, bool withTaus = kTRUE
	       )
{
  ifstream ifs(Form("%s/%s",pathDir.Data(),infname.Data()));
  assert(ifs.is_open());

  TString outNtuplename = Form("%s",infname.Data());
  outNtuplename.ReplaceAll(".lhe",".root");
  TFile *outtuple = TFile::Open(outNtuplename.Data(),"recreate");
  TNtuple *nt = new TNtuple("Events","Events","mh:ptl1:ptl2:ptn:mll:mt:dphill:ptj1:ptj2:detajj:dphijj:mjj:cent");

  // some weighted distributions
  TH1D *hDVar[100];
  hDVar[0] = new TH1D(Form("hDVar_0") ,";p_{T}^{j1} [GeV];events",100,0.,600.);
  hDVar[1] = new TH1D(Form("hDVar_1") ,";p_{T}^{j2} [GeV];events",100,0.,600.);
  hDVar[2] = new TH1D(Form("hDVar_2") ,";#eta_{jj};events",100,0.,10.);
  hDVar[3] = new TH1D(Form("hDVar_3") ,";#Delta #phi_{jj};events",100,0.,TMath::Pi());
  hDVar[4] = new TH1D(Form("hDVar_4") ,";m_{jj} [GeV];events",100,0.,4000.);
  hDVar[5] = new TH1D(Form("hDVar_5") ,";centrality;;events",2,-0.5,1.5);
  hDVar[6] = new TH1D(Form("hDVar_6") ,";pt_{T}^{l1} [GeV];events",100,0.,400.);
  hDVar[7] = new TH1D(Form("hDVar_7") ,";pt_{T}^{l2} [GeV];events",100,0.,400.);
  hDVar[8] = new TH1D(Form("hDVar_8") ,";m_{ll} [GeV];events",100,0.,500.);
  hDVar[9] = new TH1D(Form("hDVar_9") ,";m_{T}^{ll} [GeV];events",100,0.,500.);
  hDVar[10]= new TH1D(Form("hDVar_10"),";#Delta #phi_{ll};events",100,0.,TMath::Pi());
  for(UInt_t j=0; j<=10; j++) {hDVar[j]->Sumw2(); hDVar[j]->SetMinimum(0.0000001);} 

  //TString outfname = Form("%s/%s",pathDir.Data(),infname.Data());
  //ofstream ofs(outfname);
  
  int eventType[7] = {0,0,0,0,0,0,0};
  string line;
  // skip the intro up until <init>
  do {
    getline(ifs,line);
  } while (line.compare("<init>") != 0);

  // get the info in <init>
  getline(ifs,line);
  //ofs << line << endl;
  stringstream ssinit1(line);
  int idbmup1, idbmup2; 
  double ebmup1, ebmup2; 
  int pdfgup1, pdfgup2, pdfsup1, pdfsup2, idwtup, nprup;
  ssinit1 >> idbmup1 >> idbmup2 >> ebmup1 >> ebmup2 >> pdfgup1 >> pdfgup2 >> pdfsup1 >> pdfsup2 >> idwtup >> nprup;
  
  getline(ifs,line);
  //ofs << line << endl;
  stringstream ssinit2(line);
  double xsecup, xerrup, xmaxup;
  int lprup;
  ssinit2 >> xsecup >> xerrup >> xmaxup >> lprup;

  getline(ifs,line);
  //ofs << line << endl;
  assert(line.compare("</init>")==0);

  int nevents=0,npass[5]={0,0,0,0,0};
  // get the event info
  while(getline(ifs,line)) {
    if(line.compare("<event>")==0) {
      nevents++;
      if(nevents%10000 == 0) printf("--- reading event %7d\n",nevents);

      int idup, istup, mothup1, mothup2, icolup1, icolup2; 
      double  pupx, pupy, pupz, pupe, pupm, vtimup, spinup;

      TLorentzVector higgs,wp,wm,vl1,vl2,vn,vj1,vj2;
      higgs.SetPxPyPzE(0,0,0,0);
      wp.SetPxPyPzE   (0,0,0,0);
      wm.SetPxPyPzE   (0,0,0,0);
      vl1.SetPxPyPzE  (0,0,0,0);
      vl2.SetPxPyPzE  (0,0,0,0);
      vn.SetPxPyPzE   (0,0,0,0);
      vj1.SetPxPyPzE  (0,0,0,0);
      vj2.SetPxPyPzE  (0,0,0,0);

      // loop over the rest of them, skip initial line
      getline(ifs,line);
      getline(ifs,line);
      int pass[6] = {0,0,0,0,0,0};int lType[3] = {0,0,0};
      while(line.compare("</event>") != 0) {
	stringstream sstmp(line);
	sstmp >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pupx >> pupy >> pupz >> pupe >> pupm >> vtimup >> spinup;

	TLorentzVector vec;
	vec.SetPxPyPzE(pupx,pupy,pupz,pupe);

	if(idup==+25||idup==+39) {higgs = vec; pass[0]++;}

	if(idup==+24) {wp = vec; pass[1]++;}

	if(idup==-24) {wm = vec; pass[2]++;}
        	
	if(istup == 1){
          if(TMath::Abs(idup) ==  11 || TMath::Abs(idup) ==  13 || (TMath::Abs(idup) ==  15 && withTaus == kTRUE)) {
	    pass[3]++;
	    if     (vec.Pt() > vl1.Pt()){
	      vl2.SetPxPyPzE(vl1.Px(),vl1.Py(),vl1.Pz(),vl1.E());
	      vl1.SetPxPyPzE(vec.Px(),vec.Py(),vec.Pz(),vec.E());
	    }
	    else if(vec.Pt() > vl2.Pt()){
	      vl2.SetPxPyPzE(vec.Px(),vec.Py(),vec.Pz(),vec.E());
	    }
          }
          if(TMath::Abs(idup) == 12 || TMath::Abs(idup) == 14 || TMath::Abs(idup) == 16) {vn.SetPxPyPzE(vn.Px()+vec.Px(),vn.Py()+vec.Py(),vn.Pz()+vec.Pz(),vn.E()+vec.E()); pass[4]++;}
	  if(TMath::Abs(idup) == 11) lType[0]++;
	  if(TMath::Abs(idup) == 13) lType[1]++;
	  if(TMath::Abs(idup) == 15) lType[2]++;

	  if(TMath::Abs(idup) == 1 || TMath::Abs(idup) == 2 || TMath::Abs(idup) == 3 || 
	     TMath::Abs(idup) == 4 || TMath::Abs(idup) == 5 || TMath::Abs(idup) == 6) {
	    pass[5]++;
	    if     (vec.Pt() > vj1.Pt()){
	      vj2.SetPxPyPzE(vj1.Px(),vj1.Py(),vj1.Pz(),vj1.E());
	      vj1.SetPxPyPzE(vec.Px(),vec.Py(),vec.Pz(),vec.E());
	    }
	    else if(vec.Pt() > vj2.Pt()){
	      vj2.SetPxPyPzE(vec.Px(),vec.Py(),vec.Pz(),vec.E());
	    }
	  }
        }
	getline(ifs,line);
      }
      if     (lType[0] == 2 && lType[1] == 0 && lType[2] == 0) eventType[0]++;
      else if(lType[0] == 0 && lType[1] == 2 && lType[2] == 0) eventType[1]++;
      else if(lType[0] == 0 && lType[1] == 0 && lType[2] == 2) eventType[2]++;
      else if(lType[0] == 1 && lType[1] == 1 && lType[2] == 0) eventType[3]++;
      else if(lType[0] == 1 && lType[1] == 0 && lType[2] == 1) eventType[4]++;
      else if(lType[0] == 0 && lType[1] == 1 && lType[2] == 1) eventType[5]++;
      else                                                     eventType[6]++;
      double mh,pth,mwp,mwm,nlep,ptl1,etal1,phil1,ptl2,etal2,phil2,ptn,mll,mt,dphill,drll,ptj1,ptj2,detajj,dphijj,mjj,cent;
      // Higgs info
      if(pass[0] > 0){
        mh   = higgs.M();
        pth  = higgs.Pt();
      } else {
        mh   = -10.0;
        pth  = -10.0;
      }
      // W+ info
      if(pass[1] > 0){
        mwp  = wp.M();
      } else {
        mwp = -10.0;
      }
      // W- info
      if(pass[2] > 0){
        mwm  = wm.M();
      } else {
        mwm = -10.0;
      }
      // leptons info
      nlep = pass[3];
      if(pass[3] >= 2){
        ptl1   = vl1.Pt();
        etal1  = vl1.Eta();
        phil1  = vl1.Phi();
        ptl2   = vl2.Pt();
        etal2  = vl2.Eta();
        phil2  = vl2.Phi();
        ptn    = vn.Pt();
        mll    = (vl1+vl2).M();
        mt     = sqrt(2.0 * (vl1+vl2).Pt() * vn.Pt() * (1.0 - cos( DeltaPhi((vl1+vl2).Phi(), vn.Phi()))));
        dphill = DeltaPhi(vl1.Phi(),vl2.Phi());
        drll   = sqrt((vl1.Eta()-vl1.Eta())*(vl1.Eta()-vl1.Eta())+dphill*dphill);
      } else {
        ptl1   = -10.0;
	etal1  = -10.0;
	phil1  = -10.0;
	ptl2   = -10.0;
	etal2  = -10.0;
	phil2  = -10.0;
	ptn    = -10.0;
	mll    = -10.0;
	mt     = -10.0;
	dphill = -10.0;
	drll   = -10.0;
      }
      // jets info
      if(pass[5] >= 2){
        ptj1   = vj1.Pt();
        ptj2   = vj2.Pt();
        detajj = TMath::Abs(vj1.Eta()-vj2.Eta());
        dphijj = DeltaPhi(vj1.Phi(),vj2.Phi());
        mjj    = (vj1+vj2).M();
        cent   = -1;
	if(pass[3] >= 2){
          cent = 0;
          if(((vj1.Eta()-vl1.Eta() > 0 && vj2.Eta()-vl1.Eta() < 0) ||
              (vj2.Eta()-vl1.Eta() > 0 && vj1.Eta()-vl1.Eta() < 0)) &&
             ((vj1.Eta()-vl2.Eta() > 0 && vj2.Eta()-vl2.Eta() < 0) ||
              (vj2.Eta()-vl2.Eta() > 0 && vj1.Eta()-vl2.Eta() < 0))) cent = 1;
        }
      } else {
        ptj1   = -10.0;
        ptj2   = -10.0;
        detajj = -10.0;
        dphijj = -10.0;
        mjj    = -10.0;
        cent   = -10.0;
      }
      if(pass[3] >= 2) npass[0]++;
      if(pass[4] >  0) npass[1]++;
      if(pass[5] >= 2) npass[2]++;
      if(pass[3] >= 2 && pass[4] > 0 && pass[5] >= 2) npass[3]++;

      if(pass[3] >= 2 && pass[4] > 0 && pass[5] >= 2 &&
         TMath::Abs(vl1.Eta()) < 2.5 && TMath::Abs(vl2.Eta()) < 2.5 &&
	 mjj > 125.0){
        npass[4]++;
        nt->Fill(mh,ptl1,ptl2,ptn,mll,mt,dphill,ptj1,ptj2,detajj,dphijj,mjj,cent);
	hDVar[0] ->Fill(TMath::Min(ptj1,599.999),weight);
        hDVar[1] ->Fill(TMath::Min(ptj2,599.999),weight);
	hDVar[2] ->Fill(TMath::Min(detajj,9.999),weight);
	hDVar[3] ->Fill(dphijj,weight);
	hDVar[4] ->Fill(TMath::Min(mjj,3999.999),weight);
	hDVar[5] ->Fill((double)cent,weight);
	hDVar[6] ->Fill(TMath::Min(ptl1,399.999),weight);
	hDVar[7] ->Fill(TMath::Min(ptl2,399.999),weight);
	hDVar[8] ->Fill(TMath::Min(mll,499.999),weight);
	hDVar[9] ->Fill(TMath::Min(mt,499.999),weight);
	hDVar[10]->Fill(dphill,weight);
      }
    } else {
      if(!TString(line).Contains("Random number generator exit values")) {
	//ofs << line << endl;
      }
    }
  }
  ifs.close();
  nt->Write();
  if(nevents > 0) for(UInt_t j=0; j<=10; j++) hDVar[j]->Scale(1.0/(double)nevents);
                  for(UInt_t j=0; j<=10; j++) hDVar[j]->Write();
  outtuple->Close();
  if(nevents > 0) printf("pass / tot: (%d,%d,%d,%d)->%d / %d = %f\n",npass[0],npass[1],npass[2],npass[3],npass[4],nevents,double(npass[4])/nevents);
  else            printf("empty file\n");
  printf("eventType: %d %d %d %d %d %d - %d\n",eventType[0],eventType[1],eventType[2],eventType[3],eventType[4],eventType[5],eventType[6]);
}

double DeltaPhi(double phi1, double phi2)
{
  // Compute DeltaPhi between two given angles. Results is in [-pi/2,pi/2].
  double dphi = TMath::Abs(phi1-phi2);
  while (dphi>TMath::Pi())
    dphi = TMath::Abs(dphi - TMath::TwoPi());
  return(dphi);
}
