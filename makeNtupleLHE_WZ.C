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
void makeNtupleLHE_WZ(TString pathDir="/afs/cern.ch/work/c/ceballos/public/samples/wzlhe8tev/",
	           TString infname="WZJetsTo3LNu_8TeV-madgraph_166134011.lhe",
		   double weight = 1.0, bool withTaus = kTRUE
	       )
{
  ifstream ifs(Form("%s/%s",pathDir.Data(),infname.Data()));
  assert(ifs.is_open());

  TString outNtuplename = Form("%s",infname.Data());
  outNtuplename.ReplaceAll(".lhe",".root");
  TFile *outtuple = TFile::Open(outNtuplename.Data(),"recreate");
  TNtuple *nt = new TNtuple("Events","Events","ptl1:ptl2:ptl3:ptn:njets:ptj1:ptj2:etaj1:etaj2:detajj:dphijj:mjj:wsign");

  // some weighted distributions
  TH1D *hDVar[20];
  const unsigned int nHist = 12;
  hDVar[ 0] = new TH1D(Form("hDVar_0") ,";N_{jets};events",5,-0.5,4.5);
  hDVar[ 1] = new TH1D(Form("hDVar_1") ,";N_{jets} W+;events",5,-0.5,4.5);
  hDVar[ 2] = new TH1D(Form("hDVar_2") ,";N_{jets} W-;events",5,-0.5,4.5);
  hDVar[ 3] = new TH1D(Form("hDVar_3") ,";pt_{T}^{l1} [GeV];events",100,0.,400.);
  hDVar[ 4] = new TH1D(Form("hDVar_4") ,";pt_{T}^{l2} [GeV];events",100,0.,400.);
  hDVar[ 5] = new TH1D(Form("hDVar_5") ,";pt_{T}^{l3} [GeV];events",100,0.,400.);
  hDVar[ 6] = new TH1D(Form("hDVar_6") ,";pt_{T}^{neutrino} [GeV];events",100,0.,400.);
  hDVar[ 7] = new TH1D(Form("hDVar_7") ,";p_{T}^{j1} [GeV];events",100,0.,600.);
  hDVar[ 8] = new TH1D(Form("hDVar_8") ,";p_{T}^{j2} [GeV];events",100,0.,600.);
  hDVar[ 9] = new TH1D(Form("hDVar_9") ,";#eta_{jj};events",100,0.,10.);
  hDVar[10] = new TH1D(Form("hDVar_10"),";#Delta #phi_{jj};events",100,0.,TMath::Pi());
  hDVar[11] = new TH1D(Form("hDVar_11"),";m_{jj} [GeV];events",100,0.,4000.);
  for(UInt_t j=0; j<nHist; j++) {hDVar[j]->Sumw2(); hDVar[j]->SetMinimum(0.0000001);} 

  int eventType[10] = {0,0,0,0,0,0,0,0,0,0};
  string line;
  // skip the intro up until <init>
  do {
    getline(ifs,line);
  } while (line.compare("</init>") != 0);

  getline(ifs,line);

  int nevents=0,npass[3]={0,0,0};
  // get the event info
  while(getline(ifs,line)) {
    if(line.compare("<event>")==0) {
      nevents++;
      if(nevents%10000 == 0) printf("--- reading event %7d\n",nevents);

      int idup, istup, mothup1, mothup2, icolup1, icolup2; 
      double  pupx, pupy, pupz, pupe, pupm, vtimup, spinup;

      TLorentzVector vl1,vl2,vl3,vn,vj1,vj2;
      vl1.SetPxPyPzE  (0,0,0,0);
      vl2.SetPxPyPzE  (0,0,0,0);
      vl3.SetPxPyPzE  (0,0,0,0);
      vn.SetPxPyPzE   (0,0,0,0);
      vj1.SetPxPyPzE  (0,0,0,0);
      vj2.SetPxPyPzE  (0,0,0,0);

      // loop over the rest of them, skip initial line
      getline(ifs,line);
      getline(ifs,line);
      int pass[3] = {0,0,0};int lType[3] = {0,0,0};int nJets = 0;
      while(line.compare("</event>") != 0) {
	stringstream sstmp(line);
	sstmp >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pupx >> pupy >> pupz >> pupe >> pupm >> vtimup >> spinup;

	TLorentzVector vec;
	vec.SetPxPyPzE(pupx,pupy,pupz,pupe);

	if(idup==+24) {pass[0]++;}

	if(idup==-24) {pass[1]++;}
        	
	if(istup == 1){
          if(TMath::Abs(idup) ==  11 || TMath::Abs(idup) ==  13 || (TMath::Abs(idup) ==  15 && withTaus == kTRUE)) {
	    pass[2]++;
	    if     (vec.Pt() > vl1.Pt()){
	      vl3.SetPxPyPzE(vl2.Px(),vl2.Py(),vl2.Pz(),vl2.E());
	      vl2.SetPxPyPzE(vl1.Px(),vl1.Py(),vl1.Pz(),vl1.E());
	      vl1.SetPxPyPzE(vec.Px(),vec.Py(),vec.Pz(),vec.E());
	    }
	    else if(vec.Pt() > vl2.Pt()){
	      vl3.SetPxPyPzE(vl2.Px(),vl2.Py(),vl2.Pz(),vl2.E());
	      vl2.SetPxPyPzE(vec.Px(),vec.Py(),vec.Pz(),vec.E());
	    }
	    else if(vec.Pt() > vl3.Pt()){
	      vl3.SetPxPyPzE(vec.Px(),vec.Py(),vec.Pz(),vec.E());
	    }
          }
          if(TMath::Abs(idup) == 12 || TMath::Abs(idup) == 14 || TMath::Abs(idup) == 16) {vn.SetPxPyPzE(vn.Px()+vec.Px(),vn.Py()+vec.Py(),vn.Pz()+vec.Pz(),vn.E()+vec.E()); pass[4]++;}
	  if(TMath::Abs(idup) == 11) lType[0]++;
	  if(TMath::Abs(idup) == 13) lType[1]++;
	  if(TMath::Abs(idup) == 15) lType[2]++;

	  if(TMath::Abs(idup) == 1 || TMath::Abs(idup) == 2 || TMath::Abs(idup) == 3 || 
	     TMath::Abs(idup) == 4 || TMath::Abs(idup) == 5 || TMath::Abs(idup) == 6) {
	    nJets++;
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
      if     (lType[0] == 3 && lType[1] == 0 && lType[2] == 0) eventType[0]++;
      else if(lType[0] == 2 && lType[1] == 1 && lType[2] == 0) eventType[1]++;
      else if(lType[0] == 2 && lType[1] == 0 && lType[2] == 1) eventType[2]++;
      else if(lType[0] == 1 && lType[1] == 2 && lType[2] == 0) eventType[3]++;
      else if(lType[0] == 1 && lType[1] == 0 && lType[2] == 2) eventType[4]++;
      else if(lType[0] == 1 && lType[1] == 1 && lType[2] == 1) eventType[5]++;
      else if(lType[0] == 0 && lType[1] == 3 && lType[2] == 0) eventType[6]++;
      else if(lType[0] == 0 && lType[1] == 0 && lType[2] == 3) eventType[7]++;
      else if(lType[0] == 0 && lType[1] == 2 && lType[2] == 1) eventType[8]++;
      else if(lType[0] == 0 && lType[1] == 1 && lType[2] == 2) eventType[9]++;
      else {
        printf("Impossible: %d %d %d\n",lType[0],lType[1],lType[2]); assert(0);
      }

      double ptl1,ptl2,ptl3,ptn,njets,ptj1,ptj2,etaj1,etaj2,detajj,dphijj,mjj,wsign;
      // leptons info
      ptl1   = vl1.Pt();
      ptl2   = vl2.Pt();
      ptl3   = vl3.Pt();
      ptn    = vn.Pt();
      njets  = (double)nJets;
      ptj1   = vj1.Pt();
      ptj2   = vj2.Pt();
      if(ptj1>0) etaj1 = vj1.Eta();
      else       etaj1 = -9.;
      if(ptj2>0) etaj2 = vj2.Eta();
      else       etaj2 = -9.;
      if(ptj2>0) detajj = TMath::Abs(vj1.Eta()-vj2.Eta());
      else       detajj = 0;
      dphijj = DeltaPhi(vj1.Phi(),vj2.Phi());
      mjj    = (vj1+vj2).M();
      
      wsign = (double)(pass[0]-pass[1]);
      //if(TMath::Abs(wsign) != 1) {printf("total W charge should be +/- 1 (%d)\n",(int)wsign); assert(0);}

      if(pass[0] == 1) npass[0]++;
      if(pass[1] == 1) npass[1]++;
      if(pass[2] == 3) npass[2]++;

      hDVar[0]->Fill(TMath::Min(njets,4.499),weight);
      if(wsign == 1) hDVar[1]->Fill(TMath::Min(njets,4.499),weight);
      else           hDVar[2]->Fill(TMath::Min(njets,4.499),weight);
      if(njets >= 2 && TMath::Abs(wsign) == 1){
        nt->Fill(ptl1,ptl2,ptl3,ptn,njets,ptj1,ptj2,etaj1,etaj2,detajj,dphijj,mjj,wsign);
	hDVar[3] ->Fill(TMath::Min(ptl1,399.999),weight);
	hDVar[4] ->Fill(TMath::Min(ptl2,399.999),weight);
	hDVar[5] ->Fill(TMath::Min(ptl3,399.999),weight);
	hDVar[6] ->Fill(TMath::Min(ptn,399.999),weight);
	hDVar[7] ->Fill(TMath::Min(ptj1,599.999),weight);
        hDVar[8] ->Fill(TMath::Min(ptj2,599.999),weight);
	hDVar[9] ->Fill(TMath::Min(detajj,9.999),weight);
	hDVar[10]->Fill(dphijj,weight);
	hDVar[11]->Fill(TMath::Min(mjj,3999.999),weight);
      }
    } else {
      if(!TString(line).Contains("Random number generator exit values")) {
	//ofs << line << endl;
      }
    }
  }
  
  ifs.close();
  nt->Write();
  for(UInt_t j=0; j<nHist; j++) hDVar[j]->Write();
  outtuple->Close();
  if(nevents > 0) printf("(N(W+),N(W-),N(W+)+N(W-),N(3l)) / tot: (%d,%d,%d,%d) -> %d\n",npass[0],npass[1],npass[0]+npass[1],npass[2],nevents);
  else            printf("empty file\n");
  printf("eventType: %d %d %d %d %d %d %d %d %d %d\n",eventType[0],eventType[1],eventType[2],eventType[3],eventType[4],
                                                      eventType[5],eventType[6],eventType[7],eventType[8],eventType[9]);
}

double DeltaPhi(double phi1, double phi2)
{
  // Compute DeltaPhi between two given angles. Results is in [-pi/2,pi/2].
  double dphi = TMath::Abs(phi1-phi2);
  while (dphi>TMath::Pi())
    dphi = TMath::Abs(dphi - TMath::TwoPi());
  return(dphi);
}
