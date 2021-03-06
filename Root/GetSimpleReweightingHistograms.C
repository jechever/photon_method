#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <fstream>
#include <sstream>

#include "TCanvas.h"
#include "TLegend.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TPad.h"
#include "TCut.h"
#include "TProfile.h"
#include "THStack.h"
#include "TLatex.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TLine.h"
#include "TMath.h"

using namespace std;

TH1F* GetSimpleReweightingHistograms(int isData, string label , string period, string channel, string smearing_mode, int step ){


  cout << "Doing histograms for period and year " << period << " " << channel << endl;
  gStyle->SetOptStat(0);
  
  //--- set up labels
  string mcdir      = "";
  string gdatalabel = "";
  if     ( TString(period).Contains("data15-16") ){
    mcdir    = "MC16a/";
    gdatalabel = "data15-16";
  }
  else if( TString(period).Contains("data17")    ){
    mcdir    = "MC16cd/";
    gdatalabel = "data17";
  }
  else if( TString(period).Contains("data18")    ){
    mcdir    = "MC16cd_2018/";
    gdatalabel = "data18";
  }

  //--- set up MC period
  string mcperiod = "";
  if( TString(period).Contains("data15-16") ) mcperiod = "ZMC16a/";
  if( TString(period).Contains("data17")    ) mcperiod = "ZMC16cd/";
  if( TString(period).Contains("data18")    ) mcperiod = "ZMC16cd/";
        
  // set up filenames
  string data_filename   = smearingPath + "zdata/" + period + "_merged_processed.root";
  string tt_filename     = smearingPath + mcperiod + "ttbar_merged_processed.root";
  string vv_filename     = smearingPath + mcperiod + "diboson_merged_processed.root";
  string zjets_filename  = smearingPath + mcperiod + "Zjets_merged_processed.root";
  //string photon_filename = smearingPath + "gdata/" + label + "_merged_processed" + "_" + channel + smearing_mode + ".root";
  string photon_filename = smearingPath + "gdata/" + label + "_merged_processed" + "_" + channel + smearing_mode + ".root";//Vg subtracted 
  
  cout << "Opening data file    " << data_filename   << endl;
  cout << "Opening ttbar file   " << tt_filename     << endl;
  cout << "Opening diboson file " << vv_filename     << endl;
  cout << "Opening Z+jets file  " << zjets_filename << endl;
  cout << "Opening photon file  " << photon_filename << endl;

  // create TChains
  TChain* chdata  = new TChain("BaselineTree");
  TChain* chtt    = new TChain("BaselineTree");
  TChain* chvv    = new TChain("BaselineTree");
  TChain* chzjets = new TChain("BaselineTree");
  TChain* chg     = new TChain("BaselineTree");

  // add files to TChain and get entries
  chdata->Add( data_filename.c_str() );
  chtt->Add( tt_filename.c_str() );
  chvv->Add( vv_filename.c_str() );
  chzjets->Add( zjets_filename.c_str() );
  chg->Add( photon_filename.c_str() );
  
  cout << "data entries         " << chdata->GetEntries()   << endl;
  cout << "ttbar entries        " << chtt->GetEntries()     << endl;
  cout << "diboson entries      " << chvv->GetEntries()     << endl;
  cout << "Z+jets entries       " << chzjets->GetEntries()  << endl;
  cout << "photon entries       " << chg->GetEntries()      << endl;


  // define selections and weights
  TCut Zselection("mll>81 && mll<101 && jet_n >= 2 && is_OS && lep_pT[0]>25.0 && lep_pT[1]>25.0 && bjet_n==0");
  //TCut Zweight("totalWeight");

  TCut ee("channel==1");
  TCut mm("channel==0");

  if     ( TString(channel).EqualTo("ee") ) Zselection += ee;
  else if( TString(channel).EqualTo("mm") ) Zselection += mm;
  else{
    cout << "Unrecognized channel! quitting   " << channel << endl;
    exit(0);
  }

  TCut gselection("lep_pT[0]>25 && lep_pT[1]>25 && jet_n>=2 && bjet_n == 0");
  TCut weight("totalWeight");
  TCut g_rw("ptreweight3");
  TCut lumi("1.0");

  if( TString(period).EqualTo("data15-16") ) lumi = TCut("36200");
  if( TString(period).EqualTo("data17")    ) lumi = TCut("43800");
  //if( TString(period).EqualTo("data18")    ) lumi = TCut("(36200/43800)*36200");
  if( TString(period).EqualTo("data18")    ) lumi = TCut("36200");
     
  cout << "Z selection          " << Zselection.GetTitle() << endl;
  cout << "g selection          " << gselection.GetTitle() << endl;
  cout << "weight               " << weight.GetTitle()     << endl;
  cout << "lumi                 " << lumi.GetTitle()       << endl;
  
  // define histograms
  const unsigned int nptbins = 16;
  double ptbins[nptbins+1] = {40, 75, 100, 125, 150, 175, 200, 250, 300, 350, 400, 450, 500, 600, 700, 850, 1000};

  TH1F* hdata  = new TH1F("hdata"  ,"",nptbins,ptbins);
  TH1F* htt    = new TH1F("htt"    ,"",nptbins,ptbins);
  TH1F* hvv    = new TH1F("hvv"    ,"",nptbins,ptbins);
  TH1F* hz     = new TH1F("hz"     ,"",nptbins,ptbins);
  TH1F* histoG = new TH1F("histoG" ,"",nptbins,ptbins);    

  TCut RunRange("");
  if( TString(period).EqualTo("data17")    ){
    RunRange = TCut("RandomRunNumber < 348000");  
    cout << "Data17! adding cut " << RunRange.GetTitle() << endl;
  }
  
  /*
  // fill histograms: HT -step1
  chdata-> Draw("min(HT,999)>>hdata"  ,Zselection       ,"goff");
  chtt->   Draw("min(HT,999)>>htt"    ,Zselection*RunRange*weight*lumi,"goff");
  chvv->   Draw("min(HT,999)>>hvv"    ,Zselection*RunRange*weight*lumi,"goff");
  chzjets->Draw("min(HT,999)>>hz"     ,Zselection*RunRange*weight*lumi,"goff");
  chg    ->Draw("min(HT,999)>>histoG" ,gselection*weight,"goff");
  */
 
  
  // fill histograms: Z_pt -step2
  chdata-> Draw("min(Z_pt,999)>>hdata"  ,Zselection       ,"goff");
  chtt->   Draw("min(Z_pt,999)>>htt"    ,Zselection*RunRange*weight*lumi,"goff");
  chvv->   Draw("min(Z_pt,999)>>hvv"    ,Zselection*RunRange*weight*lumi,"goff");
  chzjets->Draw("min(Z_pt,999)>>hz"     ,Zselection*RunRange*weight*lumi,"goff");
  chg    ->Draw("min(Z_pt,999)>>histoG" ,gselection*weight*g_rw,"goff");
  //chg    ->Draw("min(HT,999)>>histoG" ,gselection*weight*g_rw,"goff");
  

  cout << "data integral        " << hdata->Integral()   << endl;
  cout << "ttbar integral       " << htt->Integral()     << endl;
  cout << "diboson integral     " << hvv->Integral()     << endl;
  cout << "Z+jets integral      " << hz->Integral()      << endl;
  cout << "photon integral      " << histoG->Integral()  << endl;

  // make canvas and draw 2L data vs. MC plot
  TCanvas *can = new TCanvas("can","can",600,600);
  can->cd();

  gPad->SetLogy();
  
  hdata->SetLineColor(1);
  hdata->SetLineWidth(2);
  hdata->SetMarkerStyle(20);

  hdata->GetXaxis()->SetTitle("Z p_{T} [GeV]");
  hdata->GetYaxis()->SetTitle("entries / bin");
  hdata->Draw("E1");

  htt->SetLineColor(1);
  htt->SetFillColor(kRed-2);

  hvv->SetLineColor(1);
  hvv->SetFillColor(kGreen-2);

  hz->SetLineColor(1);
  hz->SetFillColor(kOrange-2);
  
  THStack *mcstack = new THStack("mcstack","mcstack");
  mcstack->Add(htt);
  mcstack->Add(hvv);
  mcstack->Add(hz);
  mcstack->Draw("samehist");
  hdata->Draw("sameE1");
  hdata->Draw("axissame");

  TLegend* leg = new TLegend(0.65,0.65,0.88,0.88);
  leg->AddEntry(hdata,"data","lp");
  leg->AddEntry(hz,"Z+jets","f");
  leg->AddEntry(hvv,"VV","f");
  leg->AddEntry(htt,"t#bar{t}","f");
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->Draw();
  can->Print(Form("plots/GetSimpleReweightingHistograms_%s_%s%s_2L.pdf",label.c_str(),channel.c_str(),smearing_mode.c_str()));

  TH1F* histoZ = (TH1F*) hdata->Clone("histoZ");
  histoZ->Add( htt , -1.0 );
  histoZ->Add( hvv , -1.0 );
  //histoZ->Add( hz  , -1.0 );

  TCanvas *can2 = new TCanvas("can2","can2",600,600);
  can2->cd();

  // float nZ = histoZ->Integral();
  // histoZ->Scale( 1.0 / nZ );

  // float nG = histoG->Integral();
  // histoG->Scale( 1.0 / nG );

  TH1F* hratio = (TH1F*) histoZ->Clone("hratio");
  hratio->Divide( histoG );

  can2->Divide(1,2);
  can2->cd(2);

  histoG->GetXaxis()->SetTitle("Z p_{T} [GeV]");
  histoG->GetYaxis()->SetTitle("entries / bin");

  gPad->SetLogy();
  
  histoZ->SetLineColor(2);
  histoG->SetLineColor(4);

  histoG->Draw("hist");
  histoZ->Draw("samehist");
  
  TLegend* leg2 = new TLegend(0.7,0.7,0.88,0.88);
  leg2->AddEntry(histoZ,"2L data - t#bar{t} - VV","f");
  leg2->AddEntry(histoG,"photon","f");
  leg2->SetBorderSize(0);
  leg2->SetFillColor(0);
  leg2->Draw();

  can2->cd(1);
  hratio->SetLineColor(1);
  hratio->Draw("hist");
  
  can2->Print(Form("plots/GetSimpleReweightingHistograms_%s_%s%s_Z_vs_g.pdf",label.c_str(),channel.c_str(),smearing_mode.c_str()));

  cout << "histoG->Integral() " << histoG->Integral() << endl;
  cout << "histoZ->Integral() " << histoZ->Integral() << endl;
  cout << "hratio->Integral() " << hratio->Integral() << endl;
  


//std::vector<TH1F*>* GetSimpleReweightingHistograms( string label , string period = "data15-16" , string channel = "mm", string smearing_mode = "NoSmear" ){

//-----------------------
//TWO STEP REWIGHT CODE 
//-----------------------
/*
  TH1F* ptRatio = 0; 
    if (isData == 0) {
	  cout << "Doing histograms for period and year " << period << " " << channel << endl;
	  gStyle->SetOptStat(0);
	  
	  //--- set up labels
	  string mcdir      = "";
	  string gdatalabel = "";
	  if     ( TString(period).EqualTo("data15-16") ){
	    mcdir    = "MC16a/";
	    gdatalabel = "data15-16";
	  }
	  else if( TString(period).EqualTo("data17")    ){
	    mcdir    = "MC16cd/";
	    gdatalabel = "data17";
	  }
	  else if( TString(period).EqualTo("data18")    ){
	    mcdir    = "MC16cd_2018/";
	    gdatalabel = "data18";
	  }

	  //--- set up MC period
	  string mcperiod = "";
	  if( TString(period).EqualTo("data15-16") ) mcperiod = "ZMC16a/";
	  if( TString(period).EqualTo("data17")    ) mcperiod = "ZMC16cd/";
	  if( TString(period).EqualTo("data18")    ) mcperiod = "ZMC16cd/";

	  //--- set up photon data period
	  string gperiod = "";
	  if( TString(period).EqualTo("data15-16") ) gperiod = "Data15-16";
	  if( TString(period).EqualTo("data17")    ) gperiod = "Data17";
	  
	  string zjets_filename  = smearingPath + mcperiod + "Zjets_merged_processed.root";
	  string photon_filename = smearingPath + "gmc/" + "gmc"+ "_" + channel  + smearing_mode + ".root";
	    
	  cout << "Opening Z+jets file  " << zjets_filename << endl;
	  cout << "Opening photon file  " << photon_filename << endl;

	  TChain* chzjets = new TChain("BaselineTree");
	  TChain* chg     = new TChain("BaselineTree");

	  chzjets->Add( zjets_filename.c_str() );
	  chg->Add( photon_filename.c_str() );
	  
	  cout << "Z+jets entries       " << chzjets->GetEntries()  << endl;
	  cout << "photon entries       " << chg->GetEntries()      << endl;

	  // define selections and weights
	  TCut Zselection("mll>81 && mll<101 && is_OS && jet_n>=2 && lep_pT[0]>25.0 && lep_pT[1]>25.0 && bjet_n==0");//removed jet_n>=2 for now
	  //TCut Zweight("totalWeight");

	  TCut ee("channel==1");
	  TCut mm("channel==0");

	  if     ( TString(channel).EqualTo("ee") ) Zselection += ee;
	  else if( TString(channel).EqualTo("mm") ) Zselection += mm;
	  else{
	    cout << "Unrecognized channel! quitting   " << channel << endl;
	    exit(0);
	  }

	  TCut gselection("jet_n>=2 && lep_pT[0]>25 && lep_pT[1]>25  && bjet_n == 0");//removed jet_n>=2 for now
	  TCut weight("totalWeight");///////CHANGE HERE FOR HT+Zpt REWEIGHT
	  TCut g_rw("1.0");

	  if(step==2){
	    TCut g_rw("htreweight");
	  }

	  TCut lumi("1.0");

	  if( TString(period).EqualTo("data15-16") ) lumi = TCut("36100");
	  if( TString(period).EqualTo("data17")    ) lumi = TCut("44100");
	  if( TString(period).EqualTo("data18")    ) lumi = TCut("64610");
	     
	  cout << "Z selection          " << Zselection.GetTitle() << endl;
	  cout << "g selection          " << gselection.GetTitle() << endl;
	  cout << "weight               " << weight.GetTitle()     << endl;
	  cout << "lumi                 " << lumi.GetTitle()       << endl;
	  
	  // define histograms for Pt reweighting 
	  const unsigned int nptbins = 16;
	  double ptbins[nptbins+1] = {40, 75, 100, 125, 150, 175, 200, 250, 300, 350, 400, 450, 500, 600, 700, 850, 1000};

	  TH1F* histoG = new TH1F("histoG" ,"",nptbins,ptbins);    
	  TH1F* histoZ = new TH1F("histoZ" ,"",nptbins,ptbins); 


	  //chzjets->Draw("min(Z_pt,999)>>histoZ"     ,Zselection*weight*lumi,"goff");
	  //chg    ->Draw("min(Z_pt,999)>>histoG" ,gselection*lumi*weight,"goff");
	  if(step==1){
	    chzjets->Draw("HT>>histoZ" ,Zselection*lumi*weight,"goff");
	    chg    ->Draw("HT>>histoG" ,gselection*lumi*weight,"goff");
	  }

	  else if(step==2){
	    chzjets->Draw("Z_pt>>histoZ" ,Zselection*weight*lumi,"goff");
	    chg    ->Draw("Z_pt>>histoG" ,gselection*lumi*weight*g_rw,"goff");
	  }
		  
	  //chg    ->Draw("Z_pt>>histoG" ,gselection*lumi*weight,"goff");//STEP 1

	  cout << "Z+jets integral      " << histoZ->Integral()      << endl;
	  cout << "photon integral      " << histoG->Integral()  << endl;


	  // Make pT histo as a first check 
	  TCanvas *can2 = new TCanvas("can2","can2",600,600);
	  can2->cd();

	  ptRatio = (TH1F*) histoZ->Clone("ptRatio");
	  ptRatio->Divide( histoG );


	  can2->Divide(1,2);
	  can2->cd(2);

	  if(step==1){
	    histoG->GetXaxis()->SetTitle("HT [GeV]");
	  }
	  else if(step==2){
	    histoG->GetXaxis()->SetTitle("p_{T} [GeV]");
          }
	  //histoG->GetXaxis()->SetTitle("Z p_{T} [GeV]");
	  histoG->GetYaxis()->SetTitle("entries / bin");

	  gPad->SetLogy();
	  
	  histoZ->SetLineColor(2);
	  histoG->SetLineColor(4);

	  histoG->Draw("hist");
	  histoZ->Draw("samehist");

	  
	  TLegend* leg2 = new TLegend(0.7,0.7,0.88,0.88);
	  leg2->AddEntry(histoZ,"Z+jets","f");
	  leg2->AddEntry(histoG,"photon","f");
	  leg2->SetBorderSize(0);
	  leg2->SetFillColor(0);
	  leg2->Draw();

	  can2->cd(1);
	  ptRatio->SetLineColor(1);
	  ptRatio->Draw("hist");
	  
	  can2->Print(Form("plots/v1.3/GetSimpleReweightingHistograms_%s_%s%s_Z_vs_g.pdf",label.c_str(),channel.c_str(),smearing_mode.c_str()));

	  cout << "histoG->Integral() " << histoG->Integral() << endl;
	  cout << "histoZ->Integral() " << histoZ->Integral() << endl;
	  cout << "ptRatio->Integral() " << ptRatio->Integral() << endl;
	  //cout << "htRatio->Integral() " << htRatio->Integral() << endl;
    }


    else if (isData == 1) {
      cout << "Doing histograms for period and year " << period << " " << channel << endl;
      gStyle->SetOptStat(0);
  
      //--- set up labels
      string mcdir      = "";
      string gdatalabel = "";
      if     ( TString(period).Contains("data15-16") ){
	mcdir    = "MC16a/";
	gdatalabel = "data15-16";
      }
      else if( TString(period).Contains("data17")    ){
	mcdir    = "MC16cd/";
	gdatalabel = "data17";
      }
      else if( TString(period).Contains("data18")    ){
	mcdir    = "MC16cd_2018/";
	gdatalabel = "data18";
      }

      //--- set up MC period
      string mcperiod = "";
      if( TString(period).Contains("data15-16") ) mcperiod = "ZMC16a/";
      if( TString(period).Contains("data17")    ) mcperiod = "ZMC16cd/";
      if( TString(period).Contains("data18")    ) mcperiod = "ZMC16cd/";
      
      //Set up filenames 
      string data_filename   = smearingPath + "zdata/" + period + "_merged_processed.root";
      string tt_filename     = smearingPath + mcperiod + "ttbar_merged_processed.root";
      string vv_filename     = smearingPath + mcperiod + "diboson_merged_processed.root";
      string zjets_filename  = smearingPath + mcperiod + "Zjets_merged_processed.root";
      string photon_filename = smearingPath + "gdata/" + label + "_merged_processed" +  "_" + channel + smearing_mode + ".root";
  
      cout << "Opening data file    " << data_filename   << endl;
      cout << "Opening ttbar file   " << tt_filename     << endl;
      cout << "Opening diboson file " << vv_filename     << endl;
      cout << "Opening Z+jets file  " << zjets_filename << endl;
      cout << "Opening photon file  " << photon_filename << endl;

      // create TChains
      TChain* chdata  = new TChain("BaselineTree");
      TChain* chtt    = new TChain("BaselineTree");//ttbar_NoSys
      TChain* chvv    = new TChain("BaselineTree");//diboson_NoSys
      TChain* chzjets = new TChain("BaselineTree");
      TChain* chg     = new TChain("BaselineTree");

      // add files to TChain and get entries
      chdata->Add( data_filename.c_str() );
      chtt->Add( tt_filename.c_str() );
      chvv->Add( vv_filename.c_str() );
      chzjets->Add( zjets_filename.c_str() );
      chg->Add( photon_filename.c_str() );
  
      cout << "data entries         " << chdata->GetEntries()   << endl;
      cout << "ttbar entries        " << chtt->GetEntries()     << endl;
      cout << "diboson entries      " << chvv->GetEntries()     << endl;
      cout << "Z+jets entries       " << chzjets->GetEntries()  << endl;
      cout << "photon entries       " << chg->GetEntries()      << endl;

      // define selections and weights
      TCut Zselection("mll>81 && mll<101 && jet_n>=2 && is_OS && lep_pT[0]>25.0 && lep_pT[1]>25.0 && bjet_n==0");//removed jet_n>=2 for now
      //TCut TTbarSelection("mll>81 && mll<101 && nTotalJet>=2 && lepCharge[1]!=lepCharge[0] && lepPt[0]>25.0 && lepPt[1]>25.0");
      //TCut Zweight("totalWeight");

      TCut ee("channel==1");
      TCut mm("channel==0");
      
      if     ( TString(channel).EqualTo("ee") ) Zselection += ee;
      else if( TString(channel).EqualTo("mm") ) Zselection += mm;
      else{
	cout << "Unrecognized channel! quitting   " << channel << endl;
	exit(0);
      }

      TCut gselection("jet_n>=2 && lep_pT[0]>25 && lep_pT[1]>25 && bjet_n == 0");//removed jet_n>=2 for now
      TCut weight("totalWeight");
      TCut lumi("1.0");
      TCut g_rw("1.0"); 

      if(step==2){
	TCut g_rw("htreweight");
      }

      if( TString(period).EqualTo("data15-16") ) lumi = TCut("36200");
      if( TString(period).EqualTo("data17")    ) lumi = TCut("43800");
      //if( TString(period).EqualTo("data18")    ) lumi = TCut("(36200/43800)*36200");
      if( TString(period).EqualTo("data18")    ) lumi = TCut("36200");
     
      cout << "Z selection          " << Zselection.GetTitle() << endl;
      cout << "g selection          " << gselection.GetTitle() << endl;
      cout << "weight               " << weight.GetTitle()     << endl;
      cout << "lumi                 " << lumi.GetTitle()       << endl;
  
      // define histograms
      const unsigned int nptbins = 16;
      double ptbins[nptbins+1] = {40, 75, 100, 125, 150, 175, 200, 250, 300, 350, 400, 450, 500, 600, 700, 850, 1000};

      TH1F* hdata  = new TH1F("hdata"  ,"",nptbins,ptbins);
      TH1F* htt    = new TH1F("htt"    ,"",nptbins,ptbins);
      TH1F* hvv    = new TH1F("hvv"    ,"",nptbins,ptbins);
      TH1F* hz     = new TH1F("hz"     ,"",nptbins,ptbins);
      TH1F* histoG = new TH1F("histoG" ,"",nptbins,ptbins);    

      TCut RunRange("");
      if( TString(period).EqualTo("data17")    ){
	RunRange = TCut("RandomRunNumber < 348000");  
	cout << "Data17! adding cut " << RunRange.GetTitle() << endl;
      }

      // fill histograms
      if (step==1){
	chdata-> Draw("min(HT,999)>>hdata"  ,Zselection       ,"goff");
	chtt->   Draw("min(HT,999)>>htt"    ,Zselection*RunRange*weight*lumi,"goff");
	chvv->   Draw("min(HT,999)>>hvv"    ,Zselection*RunRange*weight*lumi,"goff");
	chzjets->Draw("min(HT,999)>>hz"     ,Zselection*RunRange*weight*lumi,"goff");
	chg    ->Draw("min(HT,999)>>histoG" ,gselection*weight,"goff");
      }

      else if (step==2){
	chdata-> Draw("min(Z_pt,999)>>hdata"  ,Zselection       ,"goff");
	chtt->   Draw("min(Z_pt,999)>>htt"    ,Zselection*RunRange*weight*lumi*g_rw,"goff");
	chvv->   Draw("min(Z_pt,999)>>hvv"    ,Zselection*RunRange*weight*lumi*g_rw,"goff");
	chzjets->Draw("min(Z_pt,999)>>hz"     ,Zselection*RunRange*weight*lumi*g_rw,"goff");
	chg    ->Draw("min(Z_pt,999)>>histoG" ,gselection*weight*g_rw,"goff");
      }

      cout << "data integral        " << hdata->Integral()   << endl;
      cout << "ttbar integral       " << htt->Integral()     << endl;
      cout << "diboson integral     " << hvv->Integral()     << endl;
      cout << "Z+jets integral      " << hz->Integral()      << endl;
      cout << "photon integral      " << histoG->Integral()  << endl;

      // make canvas and draw 2L data vs. MC plot
      TCanvas *can = new TCanvas("can","can",600,600);
      can->cd();

      gPad->SetLogy();
  
      hdata->SetLineColor(1);
      hdata->SetLineWidth(2);
      hdata->SetMarkerStyle(20);

      if(step==1){
	hdata->GetXaxis()->SetTitle("HT [GeV]");
      }
      else if(step==2){
        hdata->GetXaxis()->SetTitle("Z p_{T} [GeV]");
      }
      hdata->GetYaxis()->SetTitle("entries / bin");
      hdata->Draw("E1");

      htt->SetLineColor(1);
      htt->SetFillColor(kRed-2);

      hvv->SetLineColor(1);
      hvv->SetFillColor(kGreen-2);

      hz->SetLineColor(1);
      hz->SetFillColor(kOrange-2);
  
      THStack *mcstack = new THStack("mcstack","mcstack");
      mcstack->Add(htt);
      mcstack->Add(hvv);
      mcstack->Add(hz);
      mcstack->Draw("samehist");
      hdata->Draw("sameE1");
      hdata->Draw("axissame");

      TLegend* leg = new TLegend(0.65,0.65,0.88,0.88);
      leg->AddEntry(hdata,"data","lp");
      leg->AddEntry(hz,"Z+jets","f");
      leg->AddEntry(hvv,"VV","f");
      leg->AddEntry(htt,"t#bar{t}","f");
      leg->SetBorderSize(0);
      leg->SetFillColor(0);
      leg->Draw();

      can->Print(Form("plots/GetSimpleReweightingHistograms_%s_%s%s_2L.pdf",label.c_str(),channel.c_str(),smearing_mode.c_str()));

      TH1F* histoZ = (TH1F*) hdata->Clone("histoZ");
      histoZ->Add( htt , -1.0 );
      histoZ->Add( hvv , -1.0 );
      //histoZ->Add( hz  , -1.0 );

      TCanvas *can2 = new TCanvas("can2","can2",600,600);
      can2->cd();

      // float nZ = histoZ->Integral();
      // histoZ->Scale( 1.0 / nZ );

      // float nG = histoG->Integral();
      // histoG->Scale( 1.0 / nG );

      ptRatio = (TH1F*) histoZ->Clone("ptRatio");
      ptRatio->Divide( histoG );

      can2->Divide(1,2);
      can2->cd(2);

      if (step==1){
        histoG->GetXaxis()->SetTitle("HT [GeV]");
      }
      else if (step==2){
	histoG->GetXaxis()->SetTitle("Z p_{T} [GeV]");
      }
      histoG->GetYaxis()->SetTitle("entries / bin");

      gPad->SetLogy();
  
      histoZ->SetLineColor(2);
      histoG->SetLineColor(4);

      histoG->Draw("hist");
      histoZ->Draw("samehist");

  
      TLegend* leg2 = new TLegend(0.7,0.7,0.88,0.88);
      leg2->AddEntry(histoZ,"2L data - t#bar{t} - VV","f");
      leg2->AddEntry(histoG,"photon","f");
      leg2->SetBorderSize(0);
      leg2->SetFillColor(0);
      leg2->Draw();

      can2->cd(1);
      ptRatio->SetLineColor(1);
      ptRatio->Draw("hist");
  
      can2->Print(Form("plots/GetSimpleReweightingHistograms_%s_%s%s_Z_vs_g.pdf",label.c_str(),channel.c_str(),smearing_mode.c_str()));

      cout << "histoG->Integral() " << histoG->Integral() << endl;
      cout << "histoZ->Integral() " << histoZ->Integral() << endl;
      cout << "ptRatio->Integral() " << ptRatio->Integral() << endl;
  
      //  string rootfilename = Form("rootfiles/GetSimpleReweightingHistograms_%s_%s%s.root",period.c_str(),channel.c_str(),smearing_mode.c_str());
      // cout << "Saving to root file :" << rootfilename << endl;
      // TFile* f = TFile::Open(rootfilename.c_str(),"RECREATE");
      // f->cd();
      // hratio->Write();
      // f->Close();
      //return ptRatio;
    }

*/
  return hratio;
  //return ptRatio;
}
