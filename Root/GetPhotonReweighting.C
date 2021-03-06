/*
-----------------------------------------------------------------------------------------------
This script takes the outputs from GetBaseLineEvents.C and GetPhotonEvents.C and GetPhotonSmearing.C, and makes photon reweighting factors.
The parameters of the function GetPhotonReweighting(string label, string ch, int isData, int smearing_method, int step) are:
        label: takes as an input the dataset year as data15-16, data17 or data18
 	ch: which dilepton channel you are smearing the photon events to (ee,mm)
 	isData: 0 (MC) or 1 (data)
	step: the latest version of this code is split into two steps (1) reweight with HT, then (2) reweight with Pt_z. First run with step = 1, then step = 2
 example of code running command: root -l -b 'GetPhotonReweighting.C+("data15-16","mm",0,0)'
-----------------------------------------------------------------------------------------------
*/

#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <string>
#include <iomanip> 

#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TChain.h"
#include "TBranch.h"
#include "TLorentzVector.h"
#include "TGraphAsymmErrors.h"

#include "BasicSetting.C"
#include "GetSimpleReweightingHistograms.C"

using namespace std;

vector<string> noSampleWeight;
vector<string> noEventWeight;

void GetPhotonReweighting(string label, string ch, int isData, int smearing_method, int step) {
  //void GetPhotonReweighting(string label, string ch, int isData, int smearing_method, string variable)
  //{

        string periodlabel = "";
        if     ( TString(label).Contains("Data15-16") ) periodlabel = "data15-16";
	else if( TString(label).Contains("Data17")    ) periodlabel = "data17";
	else if( TString(label).Contains("Data18")    ) periodlabel = "data18";

        if     ( TString(label).Contains("data15-16") ) periodlabel = "data15-16";
	else if( TString(label).Contains("data17")    ) periodlabel = "data17";
	else if( TString(label).Contains("data18")    ) periodlabel = "data18";

	if (smearing_method == 0) photon_tag = "_NoSmear";
	if (smearing_method == 1) photon_tag = "_McSmear";
	if (smearing_method == 2) photon_tag = "_DataSmear";
	if (smearing_method == 3) photon_tag = "_TruthSmear";

	//string typeflag = "";
        //if     ( TString(variable).Contains("Z_pt") ) typeflag  = "pt";
        //else if( TString(variable).Contains("HT")   ) typeflag = "ht";

	//----------------HT REWEIGHTING-----------------
	//------------ work in progress -----------------
	//-----------------------------------------------

	/*	
	std::vector<TH1F*>* reweightedValues = GetSimpleReweightingHistograms( label , periodlabel , ch );
	if     ( TString(variable).Contains("Z_pt") ){ //if (TString(typeflag).EqualTo('pt') ){
	  TH1F* ptreweight = reweightedValues->at(0);
	        cout << "Got reweighting pt histogram hratio with integral " << ptreweight->Integral() << endl;
	  }
	else if( TString(variable).Contains("HT")   ){ //if (TString(typeflag).EqualTo('ht') ){
	  TH1F* htreweight = reweightedValues->at(1);
	        cout << "Got reweighting ht histogram hratio with integral " << htreweight->Integral() << endl;
	  }
*/


		//cout << "Got reweighting histogram hratio with integral " << hreweight->Integral() << endl;
        //TH1F* hreweight2 = GetSimpleMETRewHistogram( label , periodlabel , ch , photon_tag );
        //cout << "Got MET reweighting histogram hratio with integral " << hreweight2->Integral() << endl;

        //---------------------------------------------
        // Standard 1-d reweighting array 
        //---------------------------------------------

	TH1F* hreweight = GetSimpleReweightingHistograms(isData, label , periodlabel , ch , photon_tag, step);
	cout << "Got reweighting histogram hratio with integral " << hreweight->Integral() << endl;

	//----------------------------------------
	//TWO-STEP REWEIGHTING CODE 
	//---------------------------------------
	/*	
	TH1F* hreweight = 0;

	if (step==1){
	  hreweight = GetSimpleReweightingHistograms(isData, label , periodlabel , ch , photon_tag, step);
	  cout << "Got reweighting histogram hratio with integral (for HT)" << hreweight->Integral() << endl;
	}

	else if (step==2){
	  hreweight = GetSimpleReweightingHistograms(isData, label , periodlabel , ch , photon_tag, step);
	  cout << "Got reweighting histogram hratio with integral (for Z_pt)" << hreweight->Integral() << endl;
	}
	*/


	//---------------------------------------------
	// open file, get Tree and EventCountHist
	//---------------------------------------------

	cout << "Getting photon data" << endl;
	
	TH1::SetDefaultSumw2();

	string  filename;
	if (isData==1) {
	  filename = TString(TString(outputPath) + "gdata/" + label + "_merged_processed"  + "_" +TString(ch) + TString(photon_tag) + ".root");
	  cout << "opening data file" << endl;
	}
	//cout << "bypassing gdata dir" <<  endl;
	if (isData==0) {
	  filename = TString(TString(outputPath)+"gmc/gmc_"+TString(ch)+TString(photon_tag)+".root");
	  cout << "bypassing gdata dir" <<  endl; 
	  cout << "opening MC file" << endl;
	}
	//cout << "opening MC file" << endl;

	TFile*  f              = new TFile(filename.c_str(),"update");          
	TTree*  outputTree              = (TTree*)f->Get("BaselineTree");

	cout << endl;
	cout << "Opening file           : " << filename        << endl;
	cout << "Events in ntuple       : " << outputTree->GetEntries() << endl;

	//-----------------------------
	// access existing branches
	//-----------------------------

	double totalWeight = 0.;
	int pt = 0;
	int ptsm = 0;
	int metsm = 0;
	int ht = 0;
	int njet = 0;
	int nbjet = 0;
	Int_t jet_n = 0;
	float mll = 0.;
	float HT = 0.;
	float gamma_pt = 0.;
	float gamma_pt_smear = 0.;
	float gamma_ht = 0.;
        float gamma_ht_smear = 0.;
	float MET_smear = 0.;
	//-------------------------------
	//Variables for 2019 RJR analysis
	//-------------------------------
        float MET_loose = 0.;
	float MET_tight = 0.;
	float MET_tighter = 0.;
	float MET_tenacious = 0.;
	Int_t trigMatch_2LTrigOR;
	Int_t is2Lep2Jet;
	Int_t is2L2JInt;
	int nBJet20_MV2c10_FixedCutBEff_77;
	float mjj;
	Float_t mll_RJ;
	Float_t R_minH2P_minH3P;
	Float_t RPT_HT5PP;
	Float_t dphiVP;
	Float_t H2PP;
	Float_t H5PP;
	int nJet20;
	Float_t minDphi;
	Float_t MZ;
	Int_t NjS;
	Int_t NjISR;
	Float_t dphiISRI;
	Float_t RISR;
	Float_t PTISR;
	Float_t PTI;
	Float_t PTCM;
	Float_t MJ;
	Int_t is3Lep3Jet;
	Int_t is4Lep3Jet;
	Int_t lept1sign_VR;
	Int_t lept2sign_VR;
	Int_t lept1Pt_VR;
	Int_t lept2Pt_VR;
	Float_t MZ_VR;
	Float_t MJ_VR;
	Float_t RISR_VR;
	Float_t PTISR_VR;
	Float_t PTI_VR;
	Float_t PTCM_VR;
	Float_t dphiISRI_VR;
	//-------------------------------------

	float gamma_dR = 0.;
	std::vector<int>*    lepFlavor = new std::vector<int>(10);
	std::vector<int>*    lepCharge = new std::vector<int>(10);
	std::vector<float>* jet_pT = new std::vector<float>(10);
	//std::vector<float>* lep_pT = new std::vector<float>(10);
	std::vector<float>* lep_pT = new std::vector<float>(10);
	outputTree->SetBranchAddress("totalWeight"     ,&totalWeight              );
	outputTree->SetBranchAddress("pt"              ,&pt               );
	outputTree->SetBranchAddress("pt_smear"   ,&ptsm               );
	outputTree->SetBranchAddress("met_smear"   ,&metsm               );
	outputTree->SetBranchAddress("ht"              ,&ht               );
	outputTree->SetBranchAddress("njet"            ,&njet             );
	outputTree->SetBranchAddress("nbjet"            ,&nbjet             );
	outputTree->SetBranchAddress("mll"              ,&mll               );
	outputTree->SetBranchAddress("HT"              ,&HT               );
	outputTree->SetBranchAddress("jet_n"           ,&jet_n            );
	outputTree->SetBranchAddress("gamma_pt", &gamma_pt);
	outputTree->SetBranchAddress("gamma_ht", &gamma_ht);
	outputTree->SetBranchAddress("Z_pt", &gamma_pt_smear);
	outputTree->SetBranchAddress("HT", &gamma_ht_smear);
	outputTree->SetBranchAddress("MET", &MET_smear);
        outputTree->SetBranchAddress("MET_loose", &MET_loose);
        outputTree->SetBranchAddress("MET_tight", &MET_tight);
        outputTree->SetBranchAddress("MET_tighter", &MET_tighter);
        outputTree->SetBranchAddress("MET_tenacious", &MET_tenacious);
	outputTree->SetBranchAddress("lep_pT"          ,&lep_pT           );
        outputTree->SetBranchAddress("jet_pT"          ,&jet_pT           );
        outputTree->SetBranchAddress("trigMatch_2LTrigOR"         ,&trigMatch_2LTrigOR          );
        outputTree->SetBranchAddress("is2Lep2Jet"         ,&is2Lep2Jet         );
        outputTree->SetBranchAddress("is2L2JInt"         ,&is2L2JInt         );
        outputTree->SetBranchAddress("nBJet20_MV2c10_FixedCutBEff_77"    ,&nBJet20_MV2c10_FixedCutBEff_77 );
        outputTree->SetBranchAddress("mjj"         ,&mjj          );
        outputTree->SetBranchAddress("mll_RJ"          ,&mll_RJ          );
        outputTree->SetBranchAddress("R_minH2P_minH3P"          ,&R_minH2P_minH3P           );
        outputTree->SetBranchAddress("RPT_HT5PP"              ,&RPT_HT5PP               );
        outputTree->SetBranchAddress("dphiVP"          ,&dphiVP            );
        outputTree->SetBranchAddress("H2PP"           ,&H2PP           );
        outputTree->SetBranchAddress("H5PP"           ,&H5PP           );
        outputTree->SetBranchAddress("nJet20"             ,&nJet20            );
        outputTree->SetBranchAddress("minDphi"          ,&minDphi           );
        outputTree->SetBranchAddress("MZ"         ,&MZ          );
        outputTree->SetBranchAddress("NjS"         ,&NjS        );
        outputTree->SetBranchAddress("NjISR"           ,&NjISR           );
        outputTree->SetBranchAddress("dphiISRI"           ,&dphiISRI           );
        outputTree->SetBranchAddress("RISR"          ,&RISR           );
        outputTree->SetBranchAddress("PTISR"          ,&PTISR           );
        outputTree->SetBranchAddress("PTI"         ,&PTI          );
        outputTree->SetBranchAddress("PTCM"         ,&PTCM          );
        outputTree->SetBranchAddress("MJ",            &MJ   );
        outputTree->SetBranchAddress("is3Lep3Jet",           &is3Lep3Jet  );
        outputTree->SetBranchAddress("is4Lep3Jet",           &is4Lep3Jet  );
        outputTree->SetBranchAddress("lept1sign_VR",           &lept1sign_VR  );
        outputTree->SetBranchAddress("lept2sign_VR",           &lept2sign_VR   );
        outputTree->SetBranchAddress("lept1Pt_VR",           &lept1Pt_VR );
        outputTree->SetBranchAddress("lept2Pt_VR",            &lept2Pt_VR );
        outputTree->SetBranchAddress("MZ_VR",                  &MZ_VR );
        outputTree->SetBranchAddress("MJ_VR",                 &MJ_VR );
        outputTree->SetBranchAddress("RISR_VR",           &RISR_VR  );
        outputTree->SetBranchAddress("PTISR_VR",           &PTISR_VR  );
        outputTree->SetBranchAddress("PTI_VR",           &PTI_VR  );
        outputTree->SetBranchAddress("PTCM_VR",           &PTCM_VR   );
        outputTree->SetBranchAddress("dphiISRI_VR",           &dphiISRI_VR );
        outputTree->SetBranchAddress("lepFlavor",                  &lepFlavor );
        outputTree->SetBranchAddress("lepCharge",                 &lepCharge );

	if (isData!=1) outputTree->SetBranchAddress("gamma_dR", &gamma_dR);


	//-----------------------------
	// add new branches
	//-----------------------------

	Float_t htreweight = 0.;
	Float_t ptreweight = 0.;

	TBranch *b_ptreweight = outputTree->Branch("ptreweight5",&ptreweight,"ptreweight5/F");

	//	if(step==1){
	  //TBranch *b_htreweight = outputTree->Branch("htreweight",&htreweight,"htreweight/F");
	//}
      
	//else if(step==2){
	   //TBranch *b_ptreweight = outputTree->Branch("ptreweight",&ptreweight,"ptreweight/F");
	//}

	//Float_t ptreweight2 = 0;
        //TBranch *b_ptreweight2 = outputTree->Branch("ptreweight4",&ptreweight2,"ptreweight4/F"); 
	
	float pt37_2j30_corr_met = 0;
	float pt37_2j30_corr_lpt = 0;
	float ptrw_2j30 = 0;
	float ptsmrw_2j30 = 0;
	float etrw_2j30 = 0;
	float etsmrw_2j30 = 0;
	float htrw_2j30 = 0;
	float njetrw_2j30 = 0;
	float nbjetrw_2j30 = 0;
	float ptrw_err_2j30 = 0;
	float ptsmrw_err_2j30 = 0;
	float etrw_err_2j30 = 0;
	float etsmrw_err_2j30 = 0;
	float htrw_err_2j30 = 0;
	float njetrw_err_2j30 = 0;
	float nbjetrw_err_2j30 = 0;
	
	float pt37_ht200_corr_met = 0;
	float pt37_ht200_corr_lpt = 0;
	float ptrw_ht200 = 0;
	float ptsmrw_ht200 = 0;
	float etrw_ht200 = 0;
	float etsmrw_ht200 = 0;
	float htrw_ht200 = 0;
	float njetrw_ht200 = 0;
	float nbjetrw_ht200 = 0;
	float ptrw_err_ht200 = 0;
	float ptsmrw_err_ht200 = 0;
	float etrw_err_ht200 = 0;
	float etsmrw_err_ht200 = 0;
	float htrw_err_ht200 = 0;
	float njetrw_err_ht200 = 0;
	float nbjetrw_err_ht200 = 0;
		
	float pt37_ht400_corr_met = 0;
	float pt37_ht400_corr_lpt = 0;
	float ptrw_ht400 = 0;
	float ptsmrw_ht400 = 0;
	float etrw_ht400 = 0;
	float etsmrw_ht400 = 0;
	float htrw_ht400 = 0;
	float njetrw_ht400 = 0;
	float nbjetrw_ht400 = 0;
	float ptrw_err_ht400 = 0;
	float ptsmrw_err_ht400 = 0;
	float etrw_err_ht400 = 0;
	float etsmrw_err_ht400 = 0;
	float htrw_err_ht400 = 0;
	float njetrw_err_ht400 = 0;
	float nbjetrw_err_ht400 = 0;
		
	float pt37_ht1200_corr_met = 0;
	float pt37_ht1200_corr_lpt = 0;
	float ptrw_ht1200 = 0;
	float ptsmrw_ht1200 = 0;
	float etrw_ht1200 = 0;
	float etsmrw_ht1200 = 0;
	float htrw_ht1200 = 0;
	float njetrw_ht1200 = 0;
	float nbjetrw_ht1200 = 0;
	float ptrw_err_ht1200 = 0;
	float ptsmrw_err_ht1200 = 0;
	float etrw_err_ht1200 = 0;
	float etsmrw_err_ht1200 = 0;
	float htrw_err_ht1200 = 0;
	float njetrw_err_ht1200 = 0;
	float nbjetrw_err_ht1200 = 0;
		
	float pt37_srhigh_corr_met = 0;
	float ptrw_srhigh = 0;
	float ptsmrw_srhigh = 0;
	float etrw_srhigh = 0;
	float etsmrw_srhigh = 0;
	float htrw_srhigh = 0;
	float njetrw_srhigh = 0;
	float nbjetrw_srhigh = 0;
	float ptrw_err_srhigh = 0;
	float ptsmrw_err_srhigh = 0;
	float etrw_err_srhigh = 0;
	float etsmrw_err_srhigh = 0;
	float htrw_err_srhigh = 0;
	float njetrw_err_srhigh = 0;
	float nbjetrw_err_srhigh = 0;
	
	
	//-----------------------------
	// loop over events
	//-----------------------------

	Long64_t nentries = outputTree->GetEntries();

	for (Long64_t i=0;i<nentries;i++) {

	  if (fmod(i,1e5)==0) std::cout << i << " events processed." << std::endl;
	  outputTree->GetEntry(i);

	  float gamma_pt_truncated = gamma_pt;
	  if( gamma_pt_truncated < 40   ) gamma_pt_truncated = 41;
	  if( gamma_pt_truncated > 1000 ) gamma_pt_truncated = 999;

	  int ptbin = hreweight->FindBin( gamma_pt_truncated );
        
	  ptreweight = hreweight->GetBinContent(ptbin);

	  //ptreweight = hreweight->Eval(gamma_pt_truncated);

	  //cout << "gamma_pt gamma_pt_truncated ptbin ptreweight " << gamma_pt << " " << gamma_pt_truncated << " " << ptbin << " " << ptreweight << endl;

	  b_ptreweight->Fill();

	  /*
	  if (step==1){
	    if (fmod(i,1e5)==0) std::cout << i << " events processed." << std::endl;
	    outputTree->GetEntry(i);

	    TBranch *b_htreweight = outputTree->Branch("htreweight",&htreweight,"htreweight/F");

	    float gamma_ht_truncated = gamma_ht;
	    if( gamma_ht_truncated < 40   ) gamma_ht_truncated = 41;
	    if( gamma_ht_truncated > 1000 ) gamma_ht_truncated = 999;

	    int htbin = hreweight->FindBin( gamma_ht_truncated );
		
	    htreweight = hreweight->GetBinContent(htbin);

		//ptreweight = hreweight->Eval(gamma_pt_truncated);

		//cout << "gamma_pt gamma_pt_truncated ptbin ptreweight " << gamma_pt << " " << gamma_pt_truncated << " " << ptbin << " " << ptreweight << endl;

	    b_htreweight->Fill();
	    }
	 
          else if (step==2){
            if (fmod(i,1e5)==0) std::cout << i << " events processed." << std::endl;
	    outputTree->GetEntry(i);

	    TBranch *b_ptreweight = outputTree->Branch("ptreweight",&ptreweight,"ptreweight/F");

            float gamma_pt_truncated = gamma_pt;
            if( gamma_pt_truncated < 40   ) gamma_pt_truncated = 41;
	    if( gamma_pt_truncated > 1000 ) gamma_pt_truncated = 999;

	    int ptbin = hreweight->FindBin( gamma_pt_truncated );

	    ptreweight = hreweight->GetBinContent(ptbin);

	    //ptreweight = hreweight->Eval(gamma_pt_truncated);

	    //cout << "gamma_pt gamma_pt_truncated ptbin ptreweight " << gamma_pt << " " << gamma_pt_truncated << " " << ptbin << " " << ptreweight << endl;

	    b_ptreweight->Fill();
          }
	*/



		//--------------------------------------------------------
		//-----------------WORK IN PROGRESS -HT-------------------
                //--------------------------------------------------------

		/*
		if     ( TString(variable).Contains("Z_pt") ){ //if (TString(typeflag).EqualTo('pt') ){
		  float gamma_pt_truncated = gamma_pt;
		  if( gamma_pt_truncated < 40   ) gamma_pt_truncated = 41;
		  if( gamma_pt_truncated > 1000 ) gamma_pt_truncated = 999;
		  int ptbin = ptreweight->FindBin( gamma_pt_smear );
		  ptreweight = ptreweight->GetBinContent(ptbin);
		  b_ptreweight->Fill();
		}

		else if( TString(variable).Contains("HT")   ){ //if (TString(typeflag).EqualTo('ht') ){
		  float gamma_ht_truncated = gamma_ht;
		  if( gamma_ht_truncated < 40   ) gamma_ht_truncated = 41;
		  if( gamma_ht_truncated > 1000 ) gamma_ht_truncated = 999;
		  int htbin = htreweight->FindBin( gamma_ht_smear );
		  htreweight = htreweight->GetBinContent(htbin);
		  b_htreweight->Fill();
		}
*/


		//--- REMOVE FOR NOW
		/* 
		
		// assess the reweighting factor according to the pT/smeared pT/Njets/HT value
		float gamma_et_smear = pow(gamma_pt_smear*gamma_pt_smear+mll*mll,0.5);
		//float gamma_et_smear = gamma_pt_smear;
		pt37_bveto_corr_met = gr_37_bveto_met_correction->Eval(MET_smear);
		pt37_bveto_corr_lpt = gr_37_bveto_lpt_correction->Eval(lep_pT->at(0));
		ptrw_bveto = gr_bveto_ptrw_correction->Eval(gamma_pt);
		ptsmrw_bveto = gr_bveto_ptsmrw_correction->Eval(gamma_pt_smear);
		etrw_bveto = gr_bveto_etrw_correction->Eval(gamma_pt);
		etsmrw_bveto = gr_bveto_etsmrw_correction->Eval(gamma_et_smear);
		htrw_bveto = gr_bveto_htrw_correction->Eval(HT);
		njetrw_bveto = hist_bveto_njetrw_correction->GetBinContent(njet+1);
		nbjetrw_bveto = hist_bveto_nbjetrw_correction->GetBinContent(nbjet+1);
		ptrw_err_bveto = hist_bveto_ptrw_correction->GetBinError(pt+1);
		ptsmrw_err_bveto = hist_bveto_ptsmrw_correction->GetBinError(ptsm+1);
		etrw_err_bveto = hist_bveto_etrw_correction->GetBinError(pt+1);
		etsmrw_err_bveto = hist_bveto_etsmrw_correction->GetBinError(ptsm+1);
		htrw_err_bveto = hist_bveto_htrw_correction->GetBinError(ht+1);
		njetrw_err_bveto = hist_bveto_njetrw_correction->GetBinError(njet+1);
		nbjetrw_err_bveto = hist_bveto_nbjetrw_correction->GetBinError(nbjet+1);

		pt37_2j30_corr_met = gr_37_2j30_met_correction->Eval(MET_smear);
		pt37_2j30_corr_lpt = gr_37_2j30_lpt_correction->Eval(lep_pT->at(1));
		ptrw_2j30 = gr_2j30_ptrw_correction->Eval(gamma_pt);
		ptsmrw_2j30 = gr_2j30_ptsmrw_correction->Eval(gamma_pt_smear);
		etrw_2j30 = gr_2j30_etrw_correction->Eval(gamma_pt);
		etsmrw_2j30 = gr_2j30_etsmrw_correction->Eval(gamma_et_smear);
		htrw_2j30 = gr_2j30_htrw_correction->Eval(HT);
		njetrw_2j30 = hist_2j30_njetrw_correction->GetBinContent(njet+1);
		nbjetrw_2j30 = hist_2j30_nbjetrw_correction->GetBinContent(nbjet+1);
		ptrw_err_2j30 = hist_2j30_ptrw_correction->GetBinError(pt+1);
		ptsmrw_err_2j30 = hist_2j30_ptsmrw_correction->GetBinError(ptsm+1);
		etrw_err_2j30 = hist_2j30_etrw_correction->GetBinError(pt+1);
		etsmrw_err_2j30 = hist_2j30_etsmrw_correction->GetBinError(ptsm+1);
		htrw_err_2j30 = hist_2j30_htrw_correction->GetBinError(ht+1);
		njetrw_err_2j30 = hist_2j30_njetrw_correction->GetBinError(njet+1);
		nbjetrw_err_2j30 = hist_2j30_nbjetrw_correction->GetBinError(nbjet+1);

		pt37_ht200_corr_met = gr_37_ht200_met_correction->Eval(MET_smear);
		pt37_ht200_corr_lpt = gr_37_ht200_lpt_correction->Eval(lep_pT->at(1));
		ptrw_ht200 = gr_ht200_ptrw_correction->Eval(gamma_pt);
		ptsmrw_ht200 = gr_ht200_ptsmrw_correction->Eval(gamma_pt_smear);
		etrw_ht200 = gr_ht200_etrw_correction->Eval(gamma_pt);
		etsmrw_ht200 = gr_ht200_etsmrw_correction->Eval(gamma_et_smear);
		htrw_ht200 = gr_ht200_htrw_correction->Eval(HT);
		njetrw_ht200 = hist_ht200_njetrw_correction->GetBinContent(njet+1);
		nbjetrw_ht200 = hist_ht200_nbjetrw_correction->GetBinContent(nbjet+1);
		ptrw_err_ht200 = hist_ht200_ptrw_correction->GetBinError(pt+1);
		ptsmrw_err_ht200 = hist_ht200_ptsmrw_correction->GetBinError(ptsm+1);
		etrw_err_ht200 = hist_ht200_etrw_correction->GetBinError(pt+1);
		etsmrw_err_ht200 = hist_ht200_etsmrw_correction->GetBinError(ptsm+1);
		htrw_err_ht200 = hist_ht200_htrw_correction->GetBinError(ht+1);
		njetrw_err_ht200 = hist_ht200_njetrw_correction->GetBinError(njet+1);
		nbjetrw_err_ht200 = hist_ht200_nbjetrw_correction->GetBinError(nbjet+1);

		pt37_ht400_corr_met = gr_37_ht400_met_correction->Eval(MET_smear);
		pt37_ht400_corr_lpt = gr_37_ht400_lpt_correction->Eval(lep_pT->at(1));
		ptrw_ht400 = gr_ht400_ptrw_correction->Eval(gamma_pt);
		ptsmrw_ht400 = gr_ht400_ptsmrw_correction->Eval(gamma_pt_smear);
		etrw_ht400 = gr_ht400_etrw_correction->Eval(gamma_pt);
		etsmrw_ht400 = gr_ht400_etsmrw_correction->Eval(gamma_et_smear);
		htrw_ht400 = gr_ht400_htrw_correction->Eval(HT);
		njetrw_ht400 = hist_ht400_njetrw_correction->GetBinContent(njet+1);
		nbjetrw_ht400 = hist_ht400_nbjetrw_correction->GetBinContent(nbjet+1);
		ptrw_err_ht400 = hist_ht400_ptrw_correction->GetBinError(pt+1);
		ptsmrw_err_ht400 = hist_ht400_ptsmrw_correction->GetBinError(ptsm+1);
		etrw_err_ht400 = hist_ht400_etrw_correction->GetBinError(pt+1);
		etsmrw_err_ht400 = hist_ht400_etsmrw_correction->GetBinError(ptsm+1);
		htrw_err_ht400 = hist_ht400_htrw_correction->GetBinError(ht+1);
		njetrw_err_ht400 = hist_ht400_njetrw_correction->GetBinError(njet+1);
		nbjetrw_err_ht400 = hist_ht400_nbjetrw_correction->GetBinError(nbjet+1);

		pt37_ht1200_corr_met = gr_37_ht1200_met_correction->Eval(MET_smear);
		pt37_ht1200_corr_lpt = gr_37_ht1200_lpt_correction->Eval(lep_pT->at(1));
		ptrw_ht1200 = gr_ht1200_ptrw_correction->Eval(gamma_pt);
		ptsmrw_ht1200 = gr_ht1200_ptsmrw_correction->Eval(gamma_pt_smear);
		etrw_ht1200 = gr_ht1200_etrw_correction->Eval(gamma_pt);
		etsmrw_ht1200 = gr_ht1200_etsmrw_correction->Eval(gamma_et_smear);
		htrw_ht1200 = gr_ht1200_htrw_correction->Eval(HT);
		njetrw_ht1200 = hist_ht1200_njetrw_correction->GetBinContent(njet+1);
		nbjetrw_ht1200 = hist_ht1200_nbjetrw_correction->GetBinContent(nbjet+1);
		ptrw_err_ht1200 = hist_ht1200_ptrw_correction->GetBinError(pt+1);
		ptsmrw_err_ht1200 = hist_ht1200_ptsmrw_correction->GetBinError(ptsm+1);
		etrw_err_ht1200 = hist_ht1200_etrw_correction->GetBinError(pt+1);
		etsmrw_err_ht1200 = hist_ht1200_etsmrw_correction->GetBinError(ptsm+1);
		htrw_err_ht1200 = hist_ht1200_htrw_correction->GetBinError(ht+1);
		njetrw_err_ht1200 = hist_ht1200_njetrw_correction->GetBinError(njet+1);
		nbjetrw_err_ht1200 = hist_ht1200_nbjetrw_correction->GetBinError(nbjet+1);

		pt37_srhigh_corr_met = gr_37_srhigh_met_correction->Eval(MET_smear);
		ptrw_srhigh = gr_srhigh_ptrw_correction->Eval(gamma_pt);
		ptsmrw_srhigh = gr_srhigh_ptsmrw_correction->Eval(gamma_pt_smear);
		etrw_srhigh = gr_srhigh_etrw_correction->Eval(gamma_pt);
		etsmrw_srhigh = gr_srhigh_etsmrw_correction->Eval(gamma_et_smear);
		htrw_srhigh = gr_srhigh_htrw_correction->Eval(HT);
		njetrw_srhigh = hist_srhigh_njetrw_correction->GetBinContent(njet+1);
		nbjetrw_srhigh = hist_srhigh_nbjetrw_correction->GetBinContent(nbjet+1);
		ptrw_err_srhigh = hist_srhigh_ptrw_correction->GetBinError(pt+1);
		ptsmrw_err_srhigh = hist_srhigh_ptsmrw_correction->GetBinError(ptsm+1);
		etrw_err_srhigh = hist_srhigh_etrw_correction->GetBinError(pt+1);
		etsmrw_err_srhigh = hist_srhigh_etsmrw_correction->GetBinError(ptsm+1);
		htrw_err_srhigh = hist_srhigh_htrw_correction->GetBinError(ht+1);
		njetrw_err_srhigh = hist_srhigh_njetrw_correction->GetBinError(njet+1);
		nbjetrw_err_srhigh = hist_srhigh_nbjetrw_correction->GetBinError(nbjet+1);

		//b_pt37_bveto_corr_met->Fill();     
		b_pt37_bveto_corr_lpt->Fill();     
		b_ptrw_bveto->Fill();     
		b_ptsmrw_bveto->Fill();     
		//b_etrw_bveto->Fill();     
		//b_etsmrw_bveto->Fill();     
		b_htrw_bveto->Fill();     
		//b_njetrw_bveto->Fill();     
		//b_nbjetrw_bveto->Fill();     
		//b_ptrw_err_bveto->Fill();     
		//b_ptsmrw_err_bveto->Fill();     
		//b_etrw_err_bveto->Fill();     
		//b_etsmrw_err_bveto->Fill();     
		//b_htrw_err_bveto->Fill();     
		//b_njetrw_err_bveto->Fill();     
		//b_nbjetrw_err_bveto->Fill();     

		//b_pt37_2j30_corr_met->Fill();     
		b_pt37_2j30_corr_lpt->Fill();     
		b_ptrw_2j30->Fill();     
		b_ptsmrw_2j30->Fill();     
		//b_etrw_2j30->Fill();     
		//b_etsmrw_2j30->Fill();     
		b_htrw_2j30->Fill();     
		//b_njetrw_2j30->Fill();     
		//b_nbjetrw_2j30->Fill();     
		//b_ptrw_err_2j30->Fill();     
		//b_ptsmrw_err_2j30->Fill();     
		//b_etrw_err_2j30->Fill();     
		//b_etsmrw_err_2j30->Fill();     
		//b_htrw_err_2j30->Fill();     
		//b_njetrw_err_2j30->Fill();     
		//b_nbjetrw_err_2j30->Fill();     

		//b_pt37_ht200_corr_met->Fill();     
		b_pt37_ht200_corr_lpt->Fill();     
		b_ptrw_ht200->Fill();     
		b_ptsmrw_ht200->Fill();     
		//b_etrw_ht200->Fill();     
		//b_etsmrw_ht200->Fill();     
		b_htrw_ht200->Fill();     
		//b_njetrw_ht200->Fill();     
		//b_nbjetrw_ht200->Fill();     
		//b_ptrw_err_ht200->Fill();     
		//b_ptsmrw_err_ht200->Fill();     
		//b_etrw_err_ht200->Fill();     
		//b_etsmrw_err_ht200->Fill();     
		//b_htrw_err_ht200->Fill();     
		//b_njetrw_err_ht200->Fill();     
		//b_nbjetrw_err_ht200->Fill();     

		//b_pt37_ht400_corr_met->Fill();     
		b_pt37_ht400_corr_lpt->Fill();     
		b_ptrw_ht400->Fill();     
		b_ptsmrw_ht400->Fill();     
		//b_etrw_ht400->Fill();     
		//b_etsmrw_ht400->Fill();     
		b_htrw_ht400->Fill();     
		//b_njetrw_ht400->Fill();     
		//b_nbjetrw_ht400->Fill();     
		//b_ptrw_err_ht400->Fill();     
		//b_ptsmrw_err_ht400->Fill();     
		//b_etrw_err_ht400->Fill();     
		//b_etsmrw_err_ht400->Fill();     
		//b_htrw_err_ht400->Fill();     
		//b_njetrw_err_ht400->Fill();     
		//b_nbjetrw_err_ht400->Fill();     

		//b_pt37_ht1200_corr_met->Fill();     
		b_pt37_ht1200_corr_lpt->Fill();     
		b_ptrw_ht1200->Fill();     
		b_ptsmrw_ht1200->Fill();     
		//b_etrw_ht1200->Fill();     
		//b_etsmrw_ht1200->Fill();     
		b_htrw_ht1200->Fill();     
		//b_njetrw_ht1200->Fill();     
		//b_nbjetrw_ht1200->Fill();     
		//b_ptrw_err_ht1200->Fill();     
		//b_ptsmrw_err_ht1200->Fill();     
		//b_etrw_err_ht1200->Fill();     
		//b_etsmrw_err_ht1200->Fill();     
		//b_htrw_err_ht1200->Fill();     
		//b_njetrw_err_ht1200->Fill();     
		//b_nbjetrw_err_ht1200->Fill();     

		//b_pt37_srhigh_corr_met->Fill();     
		b_ptrw_srhigh->Fill();     
		b_ptsmrw_srhigh->Fill();     
		//b_etrw_srhigh->Fill();     
		//b_etsmrw_srhigh->Fill();     
		b_htrw_srhigh->Fill();     
		//b_njetrw_srhigh->Fill();     
		//b_nbjetrw_srhigh->Fill();     
		//b_ptrw_err_srhigh->Fill();     
		//b_ptsmrw_err_srhigh->Fill();     
		//b_etrw_err_srhigh->Fill();     
		//b_etsmrw_err_srhigh->Fill();     
		//b_htrw_err_srhigh->Fill();     
		//b_njetrw_err_srhigh->Fill();     
		//b_nbjetrw_err_srhigh->Fill();     
		*/
	}

	outputTree->Write();

	//--- REMOVE FOR NOW
	/*
	hist_bveto_ptrw_correction->Write();
	hist_ht200_ptrw_correction->Write();
	hist_ht400_ptrw_correction->Write();
	hist_ht1200_ptrw_correction->Write();
	gr_bveto_ptrw_correction->Write();
	gr_ht200_ptrw_correction->Write();
	gr_ht400_ptrw_correction->Write();
	gr_ht1200_ptrw_correction->Write();
	*/
	
	std::cout << "done." << std::endl;
	delete f;


}
