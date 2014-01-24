#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TBenchmark.h>
#include <TSystem.h>
#include <TNtuple.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <TLorentzVector.h>
#include <assert.h>
#include <iomanip>
#endif

void makeNtupleLHE(TString pathDir="/afs/cern.ch/work/d/dkralph/powheg/production/VBF_H/s12-h125-vbf-ptsqmin30-dkr/unmerged/",
	           TString infname="000015-pwgevents.lhe"
	       )
{
  ifstream ifs(Form("%s/%s",pathDir.Data(),infname.Data()));
  assert(ifs.is_open());

  TString outNtuplename = Form("%s",infname.Data());
  outNtuplename.ReplaceAll(".lhe",".root");
  TFile *outtuple = TFile::Open(outNtuplename.Data(),"recreate");
  TNtuple *nt = new TNtuple("Events","Events","mh:pth:etah:ptj1:ptj2:ptj3:mjj:detajj:dphijj:eta12");

  TString outfname = Form("%s/%s",pathDir.Data(),infname.Data());
  ofstream ofs(outfname);
  
  string line;
  // skip the intro up until <init>
  do {
    getline(ifs,line);
    ofs << line << endl;
  } while (line.compare("<init>") != 0);

  // get the info in <init>
  getline(ifs,line);
  ofs << line << endl;
  stringstream ssinit1(line);
  int idbmup1, idbmup2; 
  double ebmup1, ebmup2; 
  int pdfgup1, pdfgup2, pdfsup1, pdfsup2, idwtup, nprup;
  ssinit1 >> idbmup1 >> idbmup2 >> ebmup1 >> ebmup2 >> pdfgup1 >> pdfgup2 >> pdfsup1 >> pdfsup2 >> idwtup >> nprup;
  
  getline(ifs,line);
  ofs << line << endl;
  stringstream ssinit2(line);
  double xsecup, xerrup, xmaxup;
  int lprup;
  ssinit2 >> xsecup >> xerrup >> xmaxup >> lprup;

  getline(ifs,line);
  ofs << line << endl;
  assert(line.compare("</init>")==0);

  int nevents=0,npass=0;
  // get the event info
  while(getline(ifs,line)) {
    if(line.compare("<event>")==0) {
      nevents++;
      vector<string> linesThisEvent;
      linesThisEvent.push_back(line);
      // event header info
      getline(ifs,line);
      linesThisEvent.push_back(line);
      stringstream sshead(line);
      int nup, idprup;
      double xwgtup, scalup, aqedup, aqcdup;
      sshead >> nup >> idprup >> xwgtup >> scalup >> aqedup >> aqcdup;

      int idup, istup, mothup1, mothup2, icolup1, icolup2; 
      double  pup1, pup2, pup3, pup4, pup5, vtimup, spinup;
      TLorentzVector higgs;
      TLorentzVector vbf_jets[3];
      double PtMax[3] = {0.,0.,0};

      // parton 1:
      getline(ifs,line);
      linesThisEvent.push_back(line);
      stringstream ssp1(line);
      ssp1 >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;

      // parton 2:
      getline(ifs,line);
      linesThisEvent.push_back(line);
      stringstream ssp2(line);
      ssp2 >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;

      // loop over the rest of 'em
      getline(ifs,line);
      linesThisEvent.push_back(line);
      while(line.compare("</event>") != 0) {
	stringstream sstmp(line);
	sstmp >> idup >> istup >> mothup1 >> mothup2 >> icolup1 >> icolup2 >> pup1 >> pup2 >> pup3 >> pup4 >> pup5 >> vtimup >> spinup;

        if(istup != 1) continue;

	TLorentzVector vec;
	vec.SetPxPyPzE(pup1,pup2,pup3,pup4);
        
	if(idup==25) higgs = vec;
	
	if((TMath::Abs(idup) >= 1 && TMath::Abs(idup) <= 5) || TMath::Abs(idup) == 21){
	  if     (vec.Pt() > PtMax[0]) {
	    PtMax[2] = PtMax[1];
	    PtMax[1] = PtMax[0];
	    PtMax[0] = vec.Pt();
	    vbf_jets[2] = vbf_jets[1];
	    vbf_jets[1] = vbf_jets[0];
	    vbf_jets[0] = vec;
	  }
	  else if(vec.Pt() > PtMax[1]) {
	    PtMax[2] = PtMax[1];
	    PtMax[1] = vec.Pt();
	    vbf_jets[2] = vbf_jets[1];
	    vbf_jets[1] = vec;
	  }
	  else if(vec.Pt() > PtMax[2]) {
	    PtMax[2] = vec.Pt();
	    vbf_jets[2] = vec;
	  }
	}
	getline(ifs,line);
	linesThisEvent.push_back(line);
      }
      bool pass = false;
      if(PtMax[1] > 0) pass = true;
      if(pass == true){
        double mh,pth,etah,ptj1,ptj2,ptj3,mjj,detajj,dphijj;
	int eta12;
	mh     = higgs.M();
	pth    = higgs.Pt();
	etah   = higgs.Eta();
	ptj1   = vbf_jets[0].Pt();
	ptj2   = vbf_jets[1].Pt();
	ptj3   = PtMax[2];
	mjj    = (vbf_jets[0]+vbf_jets[1]).M();
	detajj = TMath::Abs(vbf_jets[0].Eta()-vbf_jets[1].Eta());
	dphijj = TMath::Abs(vbf_jets[0].DeltaPhi(vbf_jets[1]));
	if(vbf_jets[0].Eta()*vbf_jets[1].Eta() < 0) eta12 = 1; else eta12 = 0;
        npass++;
	nt->Fill(mh,pth,etah,ptj1,ptj2,ptj3,mjj,detajj,dphijj,eta12);
      }
    } else {
      if(!TString(line).Contains("Random number generator exit values")) {
	ofs << line << endl;
      }
    }
  }
  ifs.close();
  ofs.close();
  nt->Write();
  outtuple->Close();
  if(nevents > 0) cout << "pass / tot: " << npass << " / " << nevents << " = " << double(npass)/nevents << endl;
  else            cout << "empty file" << endl;
}
