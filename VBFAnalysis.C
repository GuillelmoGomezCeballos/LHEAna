#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>                  // access to gROOT, entry point to ROOT system
#include <TSystem.h>                // interface to OS
#include <TFile.h>                  // file handle class
#include <TTree.h>                  // class to access ntuples
#include <TCanvas.h>                // class for drawing
#include <TGraphErrors.h>           // Graph class
#include <TBenchmark.h>             // class to track macro running statistics
#include <TLorentzVector.h>         // 4-vector class
#include <TVector3.h>               // 3-vector class
#include <TMath.h>                  // ROOT math functions
#include <vector>                   // STL vector class
#include <iostream>                 // standard I/O
#include <iomanip>                  // functions to format standard I/O
#include <fstream>                  // functions for file I/O
#include <set>
#include <vector>
#include <utility>
#include "TH1D.h"
#endif

void VBFAnalysis(Int_t period = 0)
{

  TString filesPath   = "";
  if     (period == 0){
    filesPath  = "/afs/cern.ch/user/c/ceballos/releases/LHEAna/";
  }
  else {
    printf("Wrong period(%d)\n",period);
    return;
  }

  //*******************************************************
  //Input Files
  //*******************************************************
  vector<TString> infilenamev;  
  // order is critical
  infilenamev.push_back(Form("%s/s12-h125-vbf-ptsqmin.5-dkr_all.root",filesPath.Data()));
  infilenamev.push_back(Form("%s/s12-h125-vbf-ptsqmin1-dkr_all.root",filesPath.Data()));
  infilenamev.push_back(Form("%s/s12-h125-vbf-ptsqmin5-dkr_all.root",filesPath.Data()));
  infilenamev.push_back(Form("%s/s12-h125-vbf-ptsqmin15-dkr_all.root",filesPath.Data()));
  infilenamev.push_back(Form("%s/s12-h125-vbf-ptsqmin30-dkr_all.root",filesPath.Data()));  
  
  TH1D *hDVar[infilenamev.size()][100];
  for(UInt_t i=0; i<infilenamev.size(); i++){
    hDVar[i][0] = new TH1D(Form("hDVar_%d_0" ,i),Form("hDVar_%d_0" ,i),100,0.,200.);
    hDVar[i][1] = new TH1D(Form("hDVar_%d_1" ,i),Form("hDVar_%d_1" ,i),100,0.,200.);
    hDVar[i][2] = new TH1D(Form("hDVar_%d_2" ,i),Form("hDVar_%d_2" ,i),100,0.,200.);
    hDVar[i][3] = new TH1D(Form("hDVar_%d_3" ,i),Form("hDVar_%d_3" ,i),100,0.,2000.);
    hDVar[i][4] = new TH1D(Form("hDVar_%d_4" ,i),Form("hDVar_%d_4" ,i),100,0.,10.);
    hDVar[i][5] = new TH1D(Form("hDVar_%d_5" ,i),Form("hDVar_%d_5" ,i),2,-0.5,1.5);
  }

  Float_t mh;
  Float_t pth;
  Float_t etah;
  Float_t ptj1;
  Float_t ptj2;
  Float_t ptj3;
  Float_t mjj;
  Float_t detajj;
  Float_t dphijj;
  Float_t eta12;

  for(UInt_t ifile=0; ifile<infilenamev.size(); ifile++) {
    cout << "Processing " << infilenamev[ifile] << "..." << endl;
    TFile *file = TFile::Open(infilenamev[ifile]);
    file->cd();
    TTree *tree = dynamic_cast<TTree*>(file->Get("Events"));

    tree->SetBranchAddress( "mh"       , &mh	  );
    tree->SetBranchAddress( "pth"      , &pth     );
    tree->SetBranchAddress( "etah"     , &etah    );
    tree->SetBranchAddress( "ptj1"     , &ptj1    );
    tree->SetBranchAddress( "ptj2"     , &ptj2    );
    tree->SetBranchAddress( "ptj3"     , &ptj3    );
    tree->SetBranchAddress( "mjj"      , &mjj     );
    tree->SetBranchAddress( "detajj"   , &detajj  );
    tree->SetBranchAddress( "dphijj"   , &dphijj  );
    tree->SetBranchAddress( "eta12"    , &eta12   );

    Double_t weight  = 1.0;
    Int_t eventsPass = 0;
    UInt_t NEvents = tree->GetEntries();
    for(UInt_t ientry = 0; ientry <NEvents; ientry++){
      tree->GetEntry(ientry);
      if (ientry % 100000 == 0) printf("Event %d of %d\n",ientry,(int)NEvents);

      Int_t iHist  = ifile;
      if(TMath::Abs(etah) >= 2.5) continue;
      hDVar[iHist][0]->Fill(TMath::Min(ptj1,(float)199.999),weight);
      hDVar[iHist][1]->Fill(TMath::Min(ptj2,(float)199.999),weight);
      hDVar[iHist][2]->Fill(TMath::Min(ptj3,(float)199.999),weight);
      hDVar[iHist][3]->Fill(TMath::Min(mjj,(float)1999.999),weight);
      hDVar[iHist][4]->Fill(TMath::Min(detajj,(float)9.999),weight);
      Bool_t passCuts[5] = {ptj1 > 30,ptj2 > 30,ptj3 < 30,mjj > 500,detajj > 3.5};
      if(passCuts[0] && passCuts[1] && passCuts[2] && passCuts[3] && passCuts[4]){
        hDVar[iHist][5]->Fill(eta12,weight);
        eventsPass++;
      }
    } // end tree loop
    file->Close();
    printf("efficiency: %f\n",(double)eventsPass/NEvents);
  } // end chain process

  TFile* outFilePlotsNote = new TFile("output.root","recreate");
  outFilePlotsNote->cd();
  for(UInt_t i=0; i<infilenamev.size(); i++){
    for(UInt_t j=0; j<=5; j++){
      hDVar[i][j]->Write();
    }
  }
  outFilePlotsNote->Close();

}
