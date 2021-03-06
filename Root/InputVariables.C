bool trigMatch_1L2LTrig;
ULong64_t EventNumber;
Int_t RunNumber;
Float_t Mu;
Int_t nVtx;
Float_t mll;
//Float_t MT2Top;
//Float_t MT2W;
float MET;
float MET_phi;
float MET_softTerm;
float MET_softPhi;
//-------------------------------
//Variables for 2019 RJR analysis
//-------------------------------
Bool_t trigMatch_2LTrigOR;
float MET_loose;   
float MET_tight;
float MET_tighter;  
float MET_tenacious; 
Bool_t is2Lep2Jet;
Bool_t is2L2JInt;
int nBJet20_MV2c10_FixedCutBEff_77;
float mjj;
Double_t mll_RJ;
Double_t R_minH2P_minH3P;
Double_t RPT_HT5PP;
Double_t dphiVP;
Double_t H2PP; 
Double_t H5PP;
int nJet20;
Double_t minDphi; 
Double_t MZ;
Double_t NjS;
Double_t NjISR;
Double_t dphiISRI; 
Double_t RISR;
Double_t PTISR;
Double_t PTI;
Double_t PTCM;
Double_t MJ;
Bool_t is3Lep3Jet;
Bool_t is4Lep3Jet;
Double_t lept1sign_VR;
Double_t lept2sign_VR;
Double_t lept1Pt_VR;
Double_t lept2Pt_VR;
Double_t MZ_VR;
Double_t MJ_VR;
Double_t RISR_VR;
Double_t PTISR_VR;
Double_t PTI_VR;
Double_t PTCM_VR;
Double_t dphiISRI_VR;
//-----------------------------
//Float_t truthMET;
//Float_t truthMET_Phi;
float DPhi_METJetLeading; //might need to change back to Float_t
float DPhi_METJetSecond; //might need to change back to Float_t
float HT;
//float HTincl;
Float_t Z_pt;
//float Z_eta;
//float Z_phi;
// float SherpaNJetWeight;
// Int_t channel;
// Int_t is_OS;
// Int_t HLT_e60_lhmedium;
// Int_t HLT_2e17_lhvloose_nod0;
// Int_t HLT_mu50;
// Int_t HLT_mu18_mu8noL1;
// Int_t HLT_e17_lhloose_mu14;
// Int_t HLT_e7_lhmedium_mu24;
// Int_t eetrig;
// Int_t mmtrig;
// Int_t emtrig;
Int_t jet_n;
Int_t bjet_n;
//Int_t lep_n;
//Int_t el_n;
//Int_t mu_n;
Int_t nLep_signal;
Int_t nLep_base;
//std::vector<long>* lep_isPrompt = new std::vector<long>(10);
//std::vector<long>* lep_pdgId = new std::vector<long>(10);
std::vector<int>*    lepFlavor = new std::vector<int>(10);
std::vector<int>*    lepCharge = new std::vector<int>(10);
std::vector<int>*    lepSignal = new std::vector<int>(10);
std::vector<float>* lep_pT = new std::vector<float>(10);
std::vector<float>* lep_eta = new std::vector<float>(10);
std::vector<float>* lep_phi = new std::vector<float>(10);
std::vector<float>* jet_pT = new std::vector<float>(10);
std::vector<float>* jet_eta = new std::vector<float>(10);
std::vector<float>* jet_phi = new std::vector<float>(10);
std::vector<float>* jet_m = new std::vector<float>(10);
//std::vector<float>* jet_btag = new std::vector<float>(10);
//std::vector<int>* pass_trig_MET = new std::vector<int>(10);
//float sampleWeight;
//Float_t eventWeight;
//Float_t emtrigweight;
//Float_t eetrigweight;
//Float_t mmtrigweight;
//Float_t trig_sf;
Double_t genWeight;
Double_t eventWeight;
Double_t leptonWeight;
Double_t jvtWeight;
Double_t bTagWeight;
Double_t pileupWeight;
Double_t FFWeight;

// Float_t truth_ZpT;
// Float_t truth_Zphi;
// std::vector<float>* lep_truthPt = new std::vector<float>(10);
// std::vector<float>* lep_truthEta = new std::vector<float>(10);
// std::vector<float>* lep_truthPhi = new std::vector<float>(10);
// std::vector<float>* lep_truthGammaPt = new std::vector<float>(10);
// std::vector<float>* lep_weight = new std::vector<float>(10);
// Int_t truthJet_n;
// std::vector<float>* truthJet_pT = new std::vector<float>(10);
// std::vector<float>* truthJet_eta = new std::vector<float>(10);
// std::vector<float>* truthJet_phi = new std::vector<float>(10);

void GetBranches(TTree* inputTree, bool isData) {

	inputTree->SetBranchStatus("*", 0);
	inputTree->SetBranchStatus("EventNumber"            ,1);
	inputTree->SetBranchStatus("trigMatch_1L2LTrig"     ,1);
	inputTree->SetBranchStatus("RunNumber"              ,1);
	inputTree->SetBranchStatus("mu"                     ,1);
	inputTree->SetBranchStatus("nVtx"                   ,1);
	inputTree->SetBranchStatus("mll"                    ,1);
	//inputTree->SetBranchStatus("MT2Top"                 ,1);
	//inputTree->SetBranchStatus("MT2W"                   ,1);
	inputTree->SetBranchStatus("met_Et"                 ,1);
	inputTree->SetBranchStatus("met_Et_loose"           ,1);
        inputTree->SetBranchStatus("met_Et_tight"           ,1);
	inputTree->SetBranchStatus("met_Et_tighter"         ,1);
        inputTree->SetBranchStatus("met_Et_tenacious"       ,1);
        //------------------------------------------------------
	// 2019 RJR analysis variables -------------------------
	//------------------------------------------------------
        inputTree->SetBranchStatus("is2Lep2Jet"       ,1);
        inputTree->SetBranchStatus("is2L2JInt"       ,1);
        inputTree->SetBranchStatus("nBJet20_MV2c10_FixedCutBEff_77"       ,1);
        inputTree->SetBranchStatus("mjj"       ,1);
        inputTree->SetBranchStatus("mll_RJ"       ,1);
        inputTree->SetBranchStatus("R_minH2P_minH3P"       ,1);
        inputTree->SetBranchStatus("RPT_HT5PP"       ,1);
        inputTree->SetBranchStatus("dphiVP"       ,1);
        inputTree->SetBranchStatus("H2PP"       ,1);
        inputTree->SetBranchStatus("H5PP"       ,1);
        inputTree->SetBranchStatus("nJet20"       ,1);
        inputTree->SetBranchStatus("minDphi"       ,1);
        inputTree->SetBranchStatus("MZ"       ,1);
	inputTree->SetBranchStatus("NjS"       ,1);
	inputTree->SetBranchStatus("NjISR"       ,1);
	inputTree->SetBranchStatus("dphiISRI"       ,1);
	inputTree->SetBranchStatus("RISR"       ,1);
	inputTree->SetBranchStatus("PTISR"       ,1);
	inputTree->SetBranchStatus("PTI"       ,1);
	inputTree->SetBranchStatus("PTCM"       ,1);
	inputTree->SetBranchStatus("MJ"       ,1);
        inputTree->SetBranchStatus("is3Lep3Jet"       ,1);
	inputTree->SetBranchStatus("is4Lep3Jet"       ,1);
	inputTree->SetBranchStatus("lept1sign_VR"       ,1);
	inputTree->SetBranchStatus("lept2sign_VR"       ,1);
	inputTree->SetBranchStatus("lept1Pt_VR"       ,1);
	inputTree->SetBranchStatus("lept2Pt_VR"       ,1);
	inputTree->SetBranchStatus("MZ_VR"       ,1);
	inputTree->SetBranchStatus("MJ_VR"       ,1);
	inputTree->SetBranchStatus("RISR_VR"       ,1);
	inputTree->SetBranchStatus("PTISR_VR"       ,1);
	inputTree->SetBranchStatus("PTI_VR"       ,1);
	inputTree->SetBranchStatus("PTCM_VR"       ,1);
	inputTree->SetBranchStatus("dphiISRI_VR"       ,1);
        //------------------------------------------------------
	inputTree->SetBranchStatus("met_Phi"                ,1);
	inputTree->SetBranchStatus("TST_Et"                 ,1); 
	inputTree->SetBranchStatus("TST_Phi"                ,1); 
	//inputTree->SetBranchStatus("truthMET"               ,1);
	//inputTree->SetBranchStatus("truthMET_Phi"           ,1);
	inputTree->SetBranchStatus("DPhiJ1Met"              ,1);
	inputTree->SetBranchStatus("DPhiJ2Met"              ,1);
	inputTree->SetBranchStatus("Ht30"                   ,1);
	//inputTree->SetBranchStatus("HTincl"                 ,1);
	inputTree->SetBranchStatus("Ptll"                   ,1);
	//inputTree->SetBranchStatus("Z_eta"                  ,1);
	//inputTree->SetBranchStatus("Z_phi"                  ,1);
	//inputTree->SetBranchStatus("SherpaNJetWeight"       ,1);
	//inputTree->SetBranchStatus("channel"                ,1);
	//inputTree->SetBranchStatus("is_OS"                  ,1);
	//inputTree->SetBranchStatus("HLT_e60_lhmedium"       ,1);
	//inputTree->SetBranchStatus("HLT_2e17_lhvloose_nod0" ,1);
	//inputTree->SetBranchStatus("HLT_mu50"               ,1);
	//inputTree->SetBranchStatus("HLT_mu18_mu8noL1"       ,1);
	//inputTree->SetBranchStatus("HLT_e17_lhloose_mu14"   ,1);
	//inputTree->SetBranchStatus("HLT_e7_lhmedium_mu24"   ,1);
	//inputTree->SetBranchStatus("eetrig"                 ,1);
	//inputTree->SetBranchStatus("mmtrig"                 ,1);
	//inputTree->SetBranchStatus("emtrig"                 ,1);
	inputTree->SetBranchStatus("nBJet30_MV2c10_FixedCutBEff_77"        ,1);
	//inputTree->SetBranchStatus("lep_n"                  ,1);
	inputTree->SetBranchStatus("nLep_signal"            ,1);
	inputTree->SetBranchStatus("nLep_base"              ,1);
	//inputTree->SetBranchStatus("el_n"                   ,1);
	//inputTree->SetBranchStatus("mu_n"                   ,1);
	inputTree->SetBranchStatus("lepSignal"              ,1);
	inputTree->SetBranchStatus("lepFlavor"              ,1);
	inputTree->SetBranchStatus("lepCharge"              ,1);
	inputTree->SetBranchStatus("lepPt"                  ,1);
	inputTree->SetBranchStatus("lepEta"                 ,1);
	inputTree->SetBranchStatus("lepPhi"                 ,1);
	inputTree->SetBranchStatus("nJet30"                 ,1);
	inputTree->SetBranchStatus("jetPt"                  ,1);
	inputTree->SetBranchStatus("jetEta"                 ,1);
	inputTree->SetBranchStatus("jetPhi"                 ,1);
	inputTree->SetBranchStatus("jetM"                   ,1);
	//inputTree->SetBranchStatus("jet_btag"               ,1);
	//inputTree->SetBranchStatus("pass_trig_MET"          ,1);
	if (!isData) {
	  //inputTree->SetBranchStatus("sampleWeight"        ,1);
	  //inputTree->SetBranchStatus("eventWeight"         ,1);
	  //inputTree->SetBranchStatus("emtrigweight"        ,1);
	  //inputTree->SetBranchStatus("eetrigweight"        ,1);
	  //inputTree->SetBranchStatus("mmtrigweight"        ,1);
	  
	  inputTree->SetBranchStatus("genWeight"          ,1);
	  inputTree->SetBranchStatus("eventWeight"        ,1);
	  inputTree->SetBranchStatus("leptonWeight"       ,1);
	  inputTree->SetBranchStatus("jvtWeight"          ,1);
	  inputTree->SetBranchStatus("bTagWeight"         ,1);
	  inputTree->SetBranchStatus("pileupWeight"       ,1);
	  inputTree->SetBranchStatus("FFWeight"           ,1);

	  //inputTree->SetBranchStatus("trig_sf"             ,1);
	  //inputTree->SetBranchStatus("lep_truthPt"         ,1);
	  //inputTree->SetBranchStatus("lep_truthEta"        ,1);
	  //inputTree->SetBranchStatus("lep_truthPhi"        ,1);
	  //inputTree->SetBranchStatus("lep_truthGammaPt"    ,1);
	  //inputTree->SetBranchStatus("lep_weight"          ,1);
	  //inputTree->SetBranchStatus("truth_ZpT"           ,1);
	  //inputTree->SetBranchStatus("truth_Zphi"          ,1);
	  //inputTree->SetBranchStatus("truthJet_n"          ,1);
	  //inputTree->SetBranchStatus("truthJet_pT"         ,1);
	  //inputTree->SetBranchStatus("truthJet_eta"        ,1);
	  //inputTree->SetBranchStatus("truthJet_phi"        ,1);
	}

	// see original code, cut-and-pasted at end of file
	//std::cout << "testing-input1" << std::endl;
	inputTree->SetBranchAddress("trigMatch_1L2LTrig"    ,&trigMatch_1L2LTrig      );
	inputTree->SetBranchAddress("EventNumber"           ,&EventNumber             );
	inputTree->SetBranchAddress("RunNumber"             ,&RunNumber               );
	inputTree->SetBranchAddress("mu"                    ,&Mu                      );
	inputTree->SetBranchAddress("nVtx"                  ,&nVtx                    );
	inputTree->SetBranchAddress("mll"                   ,&mll                     );
	//std::cout << "testing-input11" << std::endl;
	//inputTree->SetBranchAddress("MT2Top"                ,&MT2Top                  );
	//inputTree->SetBranchAddress("MT2W"                  ,&MT2W                    );

        //------------------------------------------------------
        // 2019 RJR analysis variables -------------------------
        //------------------------------------------------------
	inputTree->SetBranchAddress("met_Et"                ,&MET                     );
        inputTree->SetBranchAddress("met_Et_loose"          ,&MET_loose               );
	inputTree->SetBranchAddress("met_Et_tight"          ,&MET_tight               );
        inputTree->SetBranchAddress("met_Et_tighter"        ,&MET_tighter             );
        inputTree->SetBranchAddress("met_Et_tenacious"      ,&MET_tenacious           );
        inputTree->SetBranchAddress("trigMatch_2LTrigOR"         ,&trigMatch_2LTrigOR          );
        inputTree->SetBranchAddress("is2Lep2Jet"         ,&is2Lep2Jet         );
        inputTree->SetBranchAddress("is2L2JInt"         ,&is2L2JInt         );
        inputTree->SetBranchAddress("nBJet20_MV2c10_FixedCutBEff_77"  ,&nBJet20_MV2c10_FixedCutBEff_77 );
        inputTree->SetBranchAddress("mjj"         ,&mjj          );
        inputTree->SetBranchAddress("mll_RJ"          ,&mll_RJ          );
        inputTree->SetBranchAddress("R_minH2P_minH3P"          ,&R_minH2P_minH3P           );
        inputTree->SetBranchAddress("RPT_HT5PP"              ,&RPT_HT5PP               );
        inputTree->SetBranchAddress("dphiVP"          ,&dphiVP            );
        inputTree->SetBranchAddress("H2PP"           ,&H2PP           );
        inputTree->SetBranchAddress("H5PP"           ,&H5PP           );
        inputTree->SetBranchAddress("nJet20"             ,&nJet20            );
        inputTree->SetBranchAddress("minDphi"          ,&minDphi           );
        inputTree->SetBranchAddress("MZ"         ,&MZ          );
        inputTree->SetBranchAddress("NjS"         ,&NjS        );
        inputTree->SetBranchAddress("NjISR"           ,&NjISR           );
        inputTree->SetBranchAddress("dphiISRI"           ,&dphiISRI           );
        inputTree->SetBranchAddress("RISR"          ,&RISR           );
        inputTree->SetBranchAddress("PTISR"          ,&PTISR           );
        inputTree->SetBranchAddress("PTI"         ,&PTI          );
        inputTree->SetBranchAddress("PTCM"         ,&PTCM          );
        inputTree->SetBranchAddress("MJ",            &MJ   );
        inputTree->SetBranchAddress("is3Lep3Jet",           &is3Lep3Jet  );
        inputTree->SetBranchAddress("is4Lep3Jet",           &is4Lep3Jet  );
        inputTree->SetBranchAddress("lept1sign_VR",           &lept1sign_VR  );
        inputTree->SetBranchAddress("lept2sign_VR",           &lept2sign_VR   );
        inputTree->SetBranchAddress("lept1Pt_VR",           &lept1Pt_VR );
        inputTree->SetBranchAddress("lept2Pt_VR",            &lept2Pt_VR );
        inputTree->SetBranchAddress("MZ_VR",                  &MZ_VR );
        inputTree->SetBranchAddress("MJ_VR",                 &MJ_VR );
        inputTree->SetBranchAddress("RISR_VR",           &RISR_VR  );
        inputTree->SetBranchAddress("PTISR_VR",           &PTISR_VR  );
        inputTree->SetBranchAddress("PTI_VR",           &PTI_VR  );
        inputTree->SetBranchAddress("PTCM_VR",           &PTCM_VR   );
        inputTree->SetBranchAddress("dphiISRI_VR",           &dphiISRI_VR );
	//------------------------------------------------------
	inputTree->SetBranchAddress("met_Phi"               ,&MET_phi                 );
	inputTree->SetBranchAddress("TST_Et"                ,&MET_softTerm            );
	inputTree->SetBranchAddress("TST_Phi"               ,&MET_softPhi             );
	//inputTree->SetBranchAddress("truthMET"              ,&truthMET                );
	//inputTree->SetBranchAddress("truthMET_Phi"          ,&truthMET_Phi            );
	inputTree->SetBranchAddress("DPhiJ1Met"             ,&DPhi_METJetLeading      );
	inputTree->SetBranchAddress("DPhiJ2Met"             ,&DPhi_METJetSecond       );
	inputTree->SetBranchAddress("Ht30"                  ,&HT                      );
	//inputTree->SetBranchAddress("HTincl"                ,&HTincl                  );
	inputTree->SetBranchAddress("Ptll"                  ,&Z_pt                    );
	//std::cout << "testing-input111" << std::endl;
	//inputTree->SetBranchAddress("Z_eta"                 ,&Z_eta                   );
	//inputTree->SetBranchAddress("Z_phi"                 ,&Z_phi                   );
	//inputTree->SetBranchAddress("SherpaNJetWeight"      ,&SherpaNJetWeight        );
	//inputTree->SetBranchAddress("channel"               ,&channel                 );
	//inputTree->SetBranchAddress("is_OS"                 ,&is_OS                   );
	//inputTree->SetBranchAddress("HLT_e60_lhmedium"      ,&HLT_e60_lhmedium        );
	//inputTree->SetBranchAddress("HLT_2e17_lhvloose_nod0",&HLT_2e17_lhvloose_nod0  );
	//inputTree->SetBranchAddress("HLT_mu50"              ,&HLT_mu50                );
	//inputTree->SetBranchAddress("HLT_mu18_mu8noL1"      ,&HLT_mu18_mu8noL1        );
	//inputTree->SetBranchAddress("HLT_e17_lhloose_mu14"  ,&HLT_e17_lhloose_mu14    );
	//inputTree->SetBranchAddress("HLT_e7_lhmedium_mu24"  ,&HLT_e7_lhmedium_mu24    );
	//inputTree->SetBranchAddress("eetrig"                ,&eetrig                  );
	//inputTree->SetBranchAddress("mmtrig"                ,&mmtrig                  );
	//inputTree->SetBranchAddress("emtrig"                ,&emtrig                  );
	inputTree->SetBranchAddress("nBJet30_MV2c10_FixedCutBEff_77" ,&bjet_n                  );
	//inputTree->SetBranchAddress("lep_n"                 ,&lep_n                   );
	//inputTree->SetBranchAddress("el_n"                  ,&el_n                    );
	//inputTree->SetBranchAddress("mu_n"                  ,&mu_n                    );
	//std::cout << "testing-lepsignal1" << std::endl;
	inputTree->SetBranchAddress("nLep_signal"           ,&nLep_signal             );
	//std::cout << "testing-lepsig2" << std::endl;
	inputTree->SetBranchAddress("nLep_base"             ,&nLep_base               );
	//std::cout << "testing-lepsig1" << std::endl;
	//inputTree->SetBranchAddress("lepSignal"             ,&lepSignal               );
	//std::cout << "testing-lepSig2" << std::endl;
	//inputTree->SetBranchAddress("lepSignal"             ,&lep_isPrompt            );
	//inputTree->SetBranchAddress("lepFlavor"             ,&lep_pdgId               );
	//std::cout << "testing-input1111" << std::endl;
	inputTree->SetBranchAddress("lepPt"                 ,&lep_pT                  );
	inputTree->SetBranchAddress("lepFlavor"             ,&lepFlavor               );
	inputTree->SetBranchAddress("lepCharge"             ,&lepCharge               );
	inputTree->SetBranchAddress("lepEta"                ,&lep_eta                 );
	inputTree->SetBranchAddress("lepPhi"                ,&lep_phi                 );
	inputTree->SetBranchAddress("nJet30"                ,&jet_n                   );
	inputTree->SetBranchAddress("jetPt"                 ,&jet_pT                  );
	inputTree->SetBranchAddress("jetEta"                ,&jet_eta                 );
	inputTree->SetBranchAddress("jetPhi"                ,&jet_phi                 );
	inputTree->SetBranchAddress("jetM"                  ,&jet_m                   );
	//inputTree->SetBranchAddress("jet_btag"              ,&jet_btag                );
	//inputTree->SetBranchAddress("pass_trig_MET"         ,&pass_trig_MET           );

	//std::cout << "testing-input2" <<std::endl;
	if (!isData) {
	  inputTree->SetBranchAddress("genWeight"         ,&genWeight     );
	  inputTree->SetBranchAddress("eventWeight"       ,&eventWeight   );
	  inputTree->SetBranchAddress("leptonWeight"      ,&leptonWeight  );
	  inputTree->SetBranchAddress("jvtWeight"         ,&jvtWeight     );
	  inputTree->SetBranchAddress("bTagWeight"        ,&bTagWeight    );
	  inputTree->SetBranchAddress("pileupWeight"      ,&pileupWeight  );
	  inputTree->SetBranchAddress("FFWeight"          ,&FFWeight      );
	  
	  std::cout << "testing-input3" <<std::endl;

	  //inputTree->SetBranchAddress("sampleWeight"     ,&sampleWeight);
	  //inputTree->SetBranchAddress("eventWeight"      ,&eventWeight);
	  //inputTree->SetBranchAddress("emtrigweight"     ,&emtrigweight);
	  //inputTree->SetBranchAddress("eetrigweight"     ,&eetrigweight);
	  //inputTree->SetBranchAddress("mmtrigweight"     ,&mmtrigweight);
	  //inputTree->SetBranchAddress("trig_sf",&trig_sf);
	  //inputTree->SetBranchAddress("lep_truthPt"      ,&lep_truthPt           );
	  //inputTree->SetBranchAddress("lep_truthEta"     ,&lep_truthEta           );
	  //inputTree->SetBranchAddress("lep_truthPhi"     ,&lep_truthPhi           );
	  //inputTree->SetBranchAddress("lep_truthGammaPt" ,&lep_truthGammaPt           );
	  //inputTree->SetBranchAddress("lep_weight"       ,&lep_weight);
	  //inputTree->SetBranchAddress("truth_ZpT"        ,&truth_ZpT);
	  //inputTree->SetBranchAddress("truth_Zphi"       ,&truth_Zphi);
	  //inputTree->SetBranchAddress("truthJet_n"       ,&truthJet_n       );
	  //inputTree->SetBranchAddress("truthJet_pT"      ,&truthJet_pT      );
	  //inputTree->SetBranchAddress("truthJet_eta"     ,&truthJet_eta     );
	  //inputTree->SetBranchAddress("truthJet_phi"     ,&truthJet_phi     );
	}


}









	// inputTree->SetBranchStatus("*", 0);
	// inputTree->SetBranchStatus("EventNumber"              ,1); 
	// inputTree->SetBranchStatus("RunNumber"              ,1); 
	// inputTree->SetBranchStatus("mu"              ,1); 
	// inputTree->SetBranchStatus("met_Et"             ,1); 
	// inputTree->SetBranchStatus("met_Phi"         ,1); 
	// inputTree->SetBranchStatus("TST_Et"         ,1); 
	// inputTree->SetBranchStatus("TST_Phi"         ,1); 
	// //inputTree->SetBranchStatus("truthMET"             ,1); 
	// //inputTree->SetBranchStatus("truthMET_Phi"         ,1); 
	// //inputTree->SetBranchStatus("DPhi_METJetLeading"         ,1); 
	// //inputTree->SetBranchStatus("DPhi_METJetSecond"          ,1); 
	// inputTree->SetBranchStatus("Ht30"              ,1); 
	// inputTree->SetBranchStatus("nBJet30_MV2c10_FixedCutBEff_77"          ,1); 
	// //inputTree->SetBranchStatus("nLep_signal"           ,1); 
	// inputTree->SetBranchStatus("lepPt"          ,1); 
	// inputTree->SetBranchStatus("lepEta"         ,1); 
	// inputTree->SetBranchStatus("lepPhi"         ,1); 
	// inputTree->SetBranchStatus("nJet30"           ,1); 
	// //inputTree->SetBranchStatus("jet_btag"          ,1); 
	// inputTree->SetBranchStatus("jetM"          ,1); 
	// inputTree->SetBranchStatus("jetPt"          ,1); 
	// inputTree->SetBranchStatus("jetEta"         ,1); 
	// inputTree->SetBranchStatus("jetPhi"         ,1); 
	// //inputTree->SetBranchStatus("photon_passAmbi", 1); 

	// inputTree->SetBranchAddress("EventNumber"              ,&EventNumber               );
	// inputTree->SetBranchAddress("RunNumber"              ,&RunNumber               );
	// inputTree->SetBranchAddress("mu"              ,&Mu               );
	// inputTree->SetBranchAddress("met_Et"             ,&MET              );
	// inputTree->SetBranchAddress("met_Phi"         ,&MET_phi          );
	// inputTree->SetBranchAddress("TST_Et"         ,&MET_softTerm          );
	// inputTree->SetBranchAddress("TST_Phi"         ,&MET_softPhi          );
	// //inputTree->SetBranchAddress("truthMET"             ,&truthMET              );
	// //inputTree->SetBranchAddress("truthMET_Phi"         ,&truthMET_Phi          );
	// //inputTree->SetBranchAddress("DPhi_METJetLeading"         ,&DPhi_METJetLeading          );
	// //inputTree->SetBranchAddress("DPhi_METJetSecond"          ,&DPhi_METJetSecond           );
	// inputTree->SetBranchAddress("Ht30"              ,&HT               );
	// inputTree->SetBranchAddress("nBJet30_MV2c10_FixedCutBEff_77"          ,&bjet_n            );
	// //inputTree->SetBranchAddress("nLep_signal"           ,&lep_n            );
	// //inputTree->SetBranchAddress("nLep_signal"           ,&nLep_signal            );
	// //inputTree->SetBranchAddress("nLep_base"             ,&nLep_base            );
	// inputTree->SetBranchAddress("lepPt"          ,&lep_pT           );
	// inputTree->SetBranchAddress("lepEta"         ,&lep_eta          );
	// inputTree->SetBranchAddress("lepPhi"         ,&lep_phi          );
	// inputTree->SetBranchAddress("nJet30"           ,&jet_n            );
	// //inputTree->SetBranchAddress("jet_btag"          ,&jet_btag           );
	// inputTree->SetBranchAddress("jetM"          ,&jet_m           );
	// inputTree->SetBranchAddress("jetPt"          ,&jet_pT           );
	// inputTree->SetBranchAddress("jetEta"         ,&jet_eta          );
	// inputTree->SetBranchAddress("jetPhi"         ,&jet_phi          );
	// //inputTree->SetBranchAddress("photon_passAmbi", &photon_passAmbi);

	// float sampleWeight;
	// Float_t eventWeight;
	// //std::vector<float>* truthPhoton_pT = new std::vector<float>(10);
	// //std::vector<float>* truthPhoton_eta = new std::vector<float>(10);
	// //std::vector<float>* truthPhoton_phi = new std::vector<float>(10);
	// //std::vector<float>* photon_truthPt = new std::vector<float>(10);
	// //std::vector<float>* photon_truthEta = new std::vector<float>(10);
	// //std::vector<float>* photon_truthPhi = new std::vector<float>(10);
	// if (isData!=1) {
	//   inputTree->SetBranchStatus("sampleWeight",1);
	//   inputTree->SetBranchStatus("eventWeight",1);
	//   //inputTree->SetBranchStatus("truthPhoton_pT", 1);
	//   //inputTree->SetBranchStatus("truthPhoton_eta", 1);
	//   //inputTree->SetBranchStatus("truthPhoton_phi", 1);
	//   //inputTree->SetBranchStatus("photon_truthPt", 1);
	//   //inputTree->SetBranchStatus("photon_truthEta", 1);
	//   //inputTree->SetBranchStatus("photon_truthPhi", 1);

	//   inputTree->SetBranchAddress("sampleWeight",&sampleWeight);
	//   inputTree->SetBranchAddress("eventWeight",&eventWeight);
	//   //inputTree->SetBranchAddress("truthPhoton_pT", &truthPhoton_pT);
	//   //inputTree->SetBranchAddress("truthPhoton_eta", &truthPhoton_eta);
	//   //inputTree->SetBranchAddress("truthPhoton_phi", &truthPhoton_phi);
	//   //inputTree->SetBranchAddress("photon_truthPt", &photon_truthPt);
	//   //inputTree->SetBranchAddress("photon_truthEta", &photon_truthEta);
	//   //inputTree->SetBranchAddress("photon_truthPhi", &photon_truthPhi);
	// }
