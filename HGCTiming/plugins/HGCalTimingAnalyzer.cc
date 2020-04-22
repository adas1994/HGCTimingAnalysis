// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"



#include "Geometry/HGCalGeometry/interface/HGCalGeometry.h"
#include "DataFormats/ForwardDetId/interface/HGCEEDetId.h"
#include "DataFormats/ForwardDetId/interface/HGCHEDetId.h"
#include "DataFormats/ForwardDetId/interface/HGCalDetId.h"

#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HGCRecHit/interface/HGCRecHitCollections.h"
#include "DataFormats/CaloRecHit/interface/CaloClusterFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/CaloAnalysis/interface/SimCluster.h"
#include "SimDataFormats/CaloAnalysis/interface/CaloParticle.h"

#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/TruncatedPyramid.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "TLorentzVector.h"
#include "TRandom3.h"
#include "TTree.h"
#include "TH1F.h"
#include "TF1.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TH2F.h"
#include "TH3F.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"


#include "RecoLocalCalo/HGCalRecAlgos/interface/ClusterTools.h"
#include "RecoLocalCalo/HGCalRecAlgos/interface/HGCalDepthPreClusterer.h"
#include "RecoLocalCalo/HGCalRecAlgos/interface/RecHitTools.h"

//#include "RecoParticleFlow/PFClusterProducer/plugins/SimMappers/ComputeClusterTime.h"
#include "HGCTimingAnalysis/HGCTiming/interface/UtilClasses.h"
#include "RecoLocalCalo/HGCalRecProducers/interface/ComputeClusterTime.h"

#include <vector>
#include <string>
#include <map>

class HGCalTimingAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
public:
//
// constructors and destructor
//
  explicit HGCalTimingAnalyzer(const edm::ParameterSet&);
  ~HGCalTimingAnalyzer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;


  int  nEvents;
  int  nEventsGood;

 // ----------member data ---------------------------

  edm::EDGetTokenT<HGCRecHitCollection> _recHitsEE;
  edm::EDGetTokenT<HGCRecHitCollection> _recHitsFH;
  edm::EDGetTokenT<HGCRecHitCollection> _recHitsBH;
  edm::EDGetTokenT<std::vector<TrackingVertex> > _vtxMix;
  edm::EDGetTokenT<std::vector<TrackingParticle> > _partMix;
  edm::EDGetTokenT<std::vector<CaloParticle> > _caloParticlesMix;
  edm::EDGetTokenT<std::vector<TrackingVertex> > _vtxMixData;
  edm::EDGetTokenT<std::vector<TrackingParticle> > _partMixData;
  edm::EDGetTokenT<std::vector<CaloParticle> > _caloParticlesMixData;
  
  std::string                detector;
  int                        algo;
  HGCalDepthPreClusterer     pre;
  bool                       rawRecHits;
  float                      particleGenPt;
  int                        CaloPartPDGID;
  float                      timeOffset;
  hgcal::RecHitTools         recHitTools;

  std::vector<int> layersToSkip;

  std::unique_ptr<hgcal::ClusterTools> clusterTools;

  std::vector<float> scaleCorrection;
  std::vector<float> weights;

  float keV2GeV;
  float keV2MeV;

  double keV2fC[2];
  double keV2MIP;

  double noisefC[2];
  double noiseMIP;
  //for cell type
  double fCPerMIP[3];

  TH1F* h_Vtx_x;
  TH1F* h_Vtx_y;
  TH1F* h_Vtx_z;
  TH1F* h_VtxSurvived_z;

  TH1F* h_Vtx_dvx;
  TH1F* h_Vtx_dvy;
  TH1F* h_Vtx_dvz;
  TH1F* h_rhTime;
  TH1F* h_rhTimeFilter1;
  TH1F* h_rhTimeFilter2;

  TProfile2D* cellThick_Rvseta;
  TProfile2D* cellThick_RvsLayer;

  TH1F* h_allRH_TimesOfRadius;

  //eta bins 1.65 - 1.85   1.85-2.05   2.05-2.25   2.25-2.45   2.45-2.65   2.65-2.85
 
  TH2F* h_Charge_vs_Time[6][4];
  //per timing
  TH1F* hAverageTime_Eta_dRadius_AvgInt[6][4];
  TH1F* hAverageTime_Eta_dRadius_AvgIntCorr[6][4];

  TH1F* hTime_Eta_dRadius[6][4];

  TH1F* hTotHits_Eta_dRadius[6][4];
  TH1F* hTotHitsWithTime_Eta_dRadius[6][4];

  TH1F* hFractionHitsWithTime_Eta_dRadius[6][4];
  TH1F* hFractionEvents_HitsWithTime_Eta_dRadius[6][4];
  TH2F* h2_PhiVsEta_CaloParts;

  TH2F* h2_GenXY, h2_GenXYFilter1, h2_GenXYFilter2;
  //TH2F* h2_recHXY; 
  //TH2F* h2_recHXYFilter1;
  TH2F* h2_recHXYFilter2;
  TH1F* deltaRUnf;
  TH1F* deltaRFiltered;
  TH1F* h_rhZFiltered;
  TH1F* h_rhZUnf;
  TH1F* h_PhiUnfiltered;
  TH1F* h_PhiFiltered;
  TH1F* h_EtaUnfiltered;
  TH1F* h_EtaFiltered;
  int totEvtsEtaRadius[6][4];
  int totEvtsEtaRadius_withTime[6][4];

  float radiusEtaRad[6][4];

  int nBinsEta;
  int nBinsRad;
  float binWidth;
  float binStart;
  float binEnd;

  bool debugCOUT;
  bool debugCOUT2;
  bool debugCOUT3;
  bool debugCOUT4;

  TH1F* fromAxisX;
  TH1F* fromAxisY;
  TH1F* fromAxisZ;

  TH1F* toAxisX;
  TH1F* toAxisY;
  TH1F* toAxisZ;
};  



HGCalTimingAnalyzer::HGCalTimingAnalyzer(const edm::ParameterSet& iConfig) :
  detector(iConfig.getParameter<std::string >("detector")),
  rawRecHits(iConfig.getParameter<bool>("rawRecHits")),
  particleGenPt(iConfig.getParameter<double>("particleGENPT")),
  CaloPartPDGID(iConfig.getParameter<int>("CaloPartPDGID")),
  timeOffset(iConfig.getParameter<double>("timeOffset"))
{
  nEvents = 0;
  nEventsGood = 0;

  debugCOUT = false;
  debugCOUT2 = false;
  debugCOUT3 = false;
  debugCOUT4 = false;

  //relevant HGCAL acceptance
  nBinsEta = 2;
  binWidth = 0.6;
  binStart = 1.6;
  binEnd = 2.8;
  //  nBinsRad = 4;
  nBinsRad = 2;


  //now do what ever initialization is needed
  usesResource("TFileService");

  if(detector=="all") {
    _recHitsEE = consumes<HGCRecHitCollection>(iConfig.getParameter<edm::InputTag>("HGCEEInput"));
    _recHitsFH = consumes<HGCRecHitCollection>(iConfig.getParameter<edm::InputTag>("HGCFHInput"));
    _recHitsBH = consumes<HGCRecHitCollection>(iConfig.getParameter<edm::InputTag>("HGCBHInput"));
    algo = 1;
  }else if(detector=="EM") {
    _recHitsEE = consumes<HGCRecHitCollection>(iConfig.getParameter<edm::InputTag>("HGCEEInput"));
    algo = 2;
  }else if(detector=="HAD") {
    _recHitsFH = consumes<HGCRecHitCollection>(iConfig.getParameter<edm::InputTag>("HGCFHInput"));
    _recHitsBH = consumes<HGCRecHitCollection>(iConfig.getParameter<edm::InputTag>("HGCBHInput"));
    algo = 3;
  }
  _vtxMix = consumes<std::vector<TrackingVertex> >(edm::InputTag("mix","MergedTrackTruth"));
  _partMix = consumes<std::vector<TrackingParticle> >(edm::InputTag("mix","MergedTrackTruth"));
  _caloParticlesMix = consumes<std::vector<CaloParticle> >(edm::InputTag("mix","MergedCaloTruth"));
  
  _vtxMixData = consumes<std::vector<TrackingVertex> >(edm::InputTag("mixData","MergedTrackTruth"));
  _partMixData = consumes<std::vector<TrackingParticle> >(edm::InputTag("mixData","MergedTrackTruth"));
  _caloParticlesMixData = consumes<std::vector<CaloParticle> >(edm::InputTag("mixData","MergedCaloTruth"));
  

  //parameters to provide conversion GeV - MIP
  keV2fC[0] =  iConfig.getParameter<double>("HGCEE_keV2fC");
  keV2fC[1] =  iConfig.getParameter<double>("HGCHEF_keV2fC");
  keV2MIP = iConfig.getParameter<double>("HGCHB_keV2MIP");
  noisefC[0] = (iConfig.getParameter<std::vector<double> >("HGCEE_noisefC")).at(0);
  noisefC[1] = (iConfig.getParameter<std::vector<double> >("HGCEE_noisefC")).at(1);
  noisefC[2] = (iConfig.getParameter<std::vector<double> >("HGCEE_noisefC")).at(2);
  noiseMIP = iConfig.getParameter<double>("HGCBH_noiseMIP");
  fCPerMIP[0] =  (iConfig.getParameter<std::vector<double> >("HGCEE_fCPerMIP")).at(0);
  fCPerMIP[1] =  (iConfig.getParameter<std::vector<double> >("HGCEE_fCPerMIP")).at(1);
  fCPerMIP[2] =  (iConfig.getParameter<std::vector<double> >("HGCEE_fCPerMIP")).at(2);

  const auto& rcorr = iConfig.getParameter<std::vector<double> >("thicknessCorrection");
  scaleCorrection.clear();
  for( auto corr : rcorr ) {
    scaleCorrection.push_back(1.0/corr);
  }

  const auto& dweights = iConfig.getParameter<std::vector<double> >("dEdXweights");
  for( auto weight : dweights ) {
    weights.push_back(weight);
 }

  keV2GeV = 1e-6;
  keV2MeV = 1e-3;
  //end param conversion


  auto sumes = consumesCollector();
  clusterTools = std::make_unique<hgcal::ClusterTools>(iConfig,sumes);

  edm::Service<TFileService> fs;

  cellThick_Rvseta = fs->make<TProfile2D>("cellThick_Rvseta", "", 500, 1.5, 3., 200, 0., 200.);
  cellThick_RvsLayer = fs->make<TProfile2D>("cellThick_RvsLayer", "", 45, 0., 45., 200, 0., 200.);

  h_Vtx_x = fs->make<TH1F>("h_Vtx_x", "", 1000, -15., 15.);
  h_Vtx_y = fs->make<TH1F>("h_Vtx_y", "", 1000, -15., 15.);
  h_Vtx_z = fs->make<TH1F>("h_Vtx_z", "", 1000, -15., 15.);
  h_VtxSurvived_z = fs->make<TH1F>("h_VtxSurvived_z", "", 1000, -15., 15.);

  h_Vtx_dvx = fs->make<TH1F>("h_Vtx_dvx", "", 1000, -10., 10.);
  h_Vtx_dvy = fs->make<TH1F>("h_Vtx_dvy", "", 1000, -10., 10.);
  h_Vtx_dvz = fs->make<TH1F>("h_Vtx_dvz", "", 1000, -10., 10.);
  h2_PhiVsEta_CaloParts = fs->make<TH2F>("h2_PhiVsEta_CaloParts", "", 500, -2., 2., 500, 1., 3.);
  h_rhTime  = fs->make<TH1F>("rhTime", "", 100, -10., 10.);
  h_rhTimeFilter1 = fs->make<TH1F>("rhTimeFilter1", "", 100, -10., 10.);
  h_rhTimeFilter2 = fs->make<TH1F>("rhTimeFilter2", "", 100, -10., 10.);
  
  deltaRFiltered  = fs->make<TH1F>("deltaRFiltered", "", 15, -5., 10.);
  deltaRUnf       = fs->make<TH1F>("deltaRUnf", "", 100, -5., 95.);
  //h_rhZFiltered
  h_rhZFiltered   = fs->make<TH1F>("rhZFiltered", "", 200, 270., 470.);
  h_rhZUnf        = fs->make<TH1F>("rhZUnf", "", 501, -1., 500.);

  h_PhiUnfiltered = fs->make<TH1F>("PhiUnfiltered", "", 120, -3., 3.);
  h_PhiFiltered   = fs->make<TH1F>("PhiFiltered", "", 120, -3., 3.);
  h_EtaUnfiltered = fs->make<TH1F>("EtaUnfiltered", "", 120., -3., 3.);
  h_EtaFiltered   = fs->make<TH1F>("EtaFiltered", "", 120., -3., 3.);

  fromAxisX       = fs->make<TH1F>("fromAxisX", "", 200, -100., 100.);
  fromAxisY       = fs->make<TH1F>("fromAxisY", "", 200, -100., 100.);
  fromAxisZ       = fs->make<TH1F>("fromAxisZ", "", 1000, -500., 500.);
  toAxisX         = fs->make<TH1F>("toAxisX", "", 200, -100., 100.);
  toAxisY         = fs->make<TH1F>("toAxisY", "", 200, -100., 100.);
  toAxisZ         = fs->make<TH1F>("toAxisZ", "", 1000, -500., 500.);
  //h2_GENXY        = fs->make<TH2F>("GenXY", "", 200, -2., 2., 200, -2., 2.);
  //h2_GENXYFilter1 = fs->make<TH2F>("GenXYFilter1", "", 200, -2., 2., 200, -2., 2.);
  //h2_GENXYFilter2 = fs->make<TH2F>("GenXYFilter2", "", 200, -2., 2., 200, -2., 2.);

  //h2_recHXY        = fs->make<TH2F>("recHXY", "", 200, -2., 2., 200, 1., 5.);
  //h2_recHXYFilter1 = fs->make<TH2F>("recHXYFilter1", "", 200, -2., 2., 200, 1., 5.);
  h2_recHXYFilter2 = fs->make<TH2F>("recHXYFilter2", "", 200, -2., 2., 200, 1., 5.);

  for(int ieta=0; ieta<nBinsEta; ++ieta){    
    if(debugCOUT) std::cout<< " ieta from = " << (binStart+ieta*binWidth) << " to " << binStart+binWidth+ieta*binWidth << std::endl;
    for(int iRad=0; iRad<nBinsRad; ++iRad){

      h_Charge_vs_Time[ieta][iRad] = fs->make<TH2F>(Form("h_Charge_vs_Time_Eta%.2f-%.2f_dRadius%d", 
							 (binStart+ieta*binWidth), binStart+binWidth+ieta*binWidth, iRad), "", 1000, -5., 20., 100, 0., 100.);

      hTime_Eta_dRadius[ieta][iRad] = fs->make<TH1F>(Form("hTime_Eta_dRadius_Eta%.2f-%.2f_dRadius%d", 
							  (binStart+ieta*binWidth), binStart+binWidth+ieta*binWidth, iRad), "", 1000, -5., 20.);

      hAverageTime_Eta_dRadius_AvgInt[ieta][iRad] = fs->make<TH1F>(Form("hAverageTime_Eta%.2f-%.2f_dRadius%d_AvgInt", 
									(binStart+ieta*binWidth), binStart+binWidth+ieta*binWidth, iRad), "", 2000, -1., 1.);
      hAverageTime_Eta_dRadius_AvgIntCorr[ieta][iRad] = fs->make<TH1F>(Form("hAverageTime_Eta%.2f-%.2f_dRadius%d_AvgIntCorr", 
									    (binStart+ieta*binWidth), binStart+binWidth+ieta*binWidth, iRad), "", 2000, -1., 1.);
      
      hFractionHitsWithTime_Eta_dRadius[ieta][iRad] = fs->make<TH1F>(Form("hFractionHits_Eta%.2f-%.2f_dRadius%d", 
									  (binStart+ieta*binWidth), binStart+binWidth+ieta*binWidth, iRad), "", 1100, 0, 1.1);

      hFractionEvents_HitsWithTime_Eta_dRadius[ieta][iRad] = fs->make<TH1F>(Form("hFractionEvents_Hits_Eta%.2f-%.2f_dRadius%d", 
										 (binStart+ieta*binWidth), binStart+binWidth+ieta*binWidth, iRad), "", 1100, 0, 1.1);

      hTotHits_Eta_dRadius[ieta][iRad] = fs->make<TH1F>(Form("hTotHits_Eta_%.2f-%.2f_dRadius%d", 
							     (binStart+ieta*binWidth), binStart+binWidth+ieta*binWidth, iRad), "", 2000, 0, 2000.);
      hTotHitsWithTime_Eta_dRadius[ieta][iRad] = fs->make<TH1F>(Form("hTotHitsWithTime_Eta_%.2f-%.2f_dRadius%d", 
								     (binStart+ieta*binWidth), binStart+binWidth+ieta*binWidth, iRad), "", 2000, 0, 2000.);

    }
  }


  //study timing in bins of eta and radius from the shower axis
  for(int iEta=0; iEta<nBinsEta; ++iEta){
    for(int iRad=0; iRad<nBinsRad; ++iRad){
      totEvtsEtaRadius[iEta][iRad] = 0;
      totEvtsEtaRadius_withTime[iEta][iRad] = 0;
    }
    radiusEtaRad[iEta][0] = 2.;
    radiusEtaRad[iEta][1] = 5.;
    radiusEtaRad[iEta][2] = 10.;
    radiusEtaRad[iEta][3] = 6000.;
  }
  
}

HGCalTimingAnalyzer::~HGCalTimingAnalyzer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


void
HGCalTimingAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  ++nEvents;
  // if(nEvents != 383) return;
  // if(nEvents == 383) debugCOUT4 = true;
  if(debugCOUT) std::cout<< " >>> analyzer " << std::endl;
  using namespace edm;


  recHitTools.getEventSetup(iSetup);

  Handle<HGCRecHitCollection> recHitHandleEE;
  Handle<HGCRecHitCollection> recHitHandleFH;
  Handle<HGCRecHitCollection> recHitHandleBH;

  Handle<std::vector<TrackingVertex> > vtxHandleMix;
  Handle<std::vector<TrackingParticle> > partHandleMix;
  iEvent.getByToken(_vtxMix,vtxHandleMix);
  iEvent.getByToken(_partMix,partHandleMix);
  const std::vector<TrackingVertex>& vtxsMix = *vtxHandleMix;
  const std::vector<TrackingParticle>& partMix = *partHandleMix;

  Handle<std::vector<CaloParticle> > caloParticleHandleMix;
  iEvent.getByToken(_caloParticlesMix, caloParticleHandleMix);
  const std::vector<CaloParticle>& caloParticlesMix = *caloParticleHandleMix;
  
  Handle<std::vector<TrackingVertex> > vtxHandleMixData;
  Handle<std::vector<TrackingParticle> > parthandleMixData;
  Handle<std::vector<CaloParticle> > caloParticleHandleMixData;
  /*
  iEvent.getByToken(_vtxMixData, vtxHandleMixData);
  iEvent.getByToken(_partMixData, parthandleMixData);
  iEvent.getByToken(_caloParticlesMixData, caloParticleHandleMixData);
  */
  const std::vector<TrackingVertex>& vtxsMixData = *vtxHandleMixData;
  const std::vector<TrackingParticle>& partMixData = *parthandleMixData;
  const std::vector<CaloParticle>& caloPartsMixData = *caloParticleHandleMixData;
  
  std::vector<TrackingVertex> vtxsCombined;
  std::vector<TrackingParticle> partsCombined;
  std::vector<CaloParticle> caloPartsCombined;

  // --- Combine TrackingVertex from Mix and MixData label
  /*for(std::vector<TrackingVertex>::const_iterator itv = vtxsMixData.begin(); itv!=vtxsMixData.end(); ++itv){
    vtxsCombined.push_back(*itv);
    }*/
  for(std::vector<TrackingVertex>::const_iterator itv = vtxsMix.begin(); itv!=vtxsMix.end(); ++itv){
    vtxsCombined.push_back(*itv);
    }
  /*for(std::vector<TrackingVertex>::const_iterator itv = vtxsMixData.begin(); itv!=vtxsMixData.end(); ++itv){
    vtxsCombined.push_back(*itv);
    }*/
  // --- Combine TrackingParticle from Mix and MixData label
  /*for(std::vector<TrackingParticle>::const_iterator itp = partMixData.begin(); itp!=partMixData.end(); ++itp){
    partsCombined.push_back(*itp);
    }*/
  for(std::vector<TrackingParticle>::const_iterator itp = partMix.begin(); itp!=partMix.end(); ++itp){
    partsCombined.push_back(*itp);
  }
  /*for(std::vector<TrackingParticle>::const_iterator itp = partMixData.begin(); itp!=partMixData.end(); ++itp){
    partsCombined.push_back(*itp);
    }*/
  // --- Combine CaloParticle from Mix and MixData label

  /*for(std::vector<CaloParticle>::const_iterator itc = caloPartsMixData.begin(); itc!=caloPartsMixData.end(); ++itc){
    caloPartsCombined.push_back(*itc);
    }*/
  for(std::vector<CaloParticle>::const_iterator itc = caloParticlesMix.begin(); itc!=caloParticlesMix.end(); ++itc){
    caloPartsCombined.push_back(*itc);
  }
  /*for(std::vector<CaloParticle>::const_iterator itc = caloPartsMixData.begin(); itc!=caloPartsMixData.end(); ++itc){
    caloPartsCombined.push_back(*itc);
    }*/
  const std::vector<TrackingVertex>& vtxs        = (vtxsCombined) ;
  const std::vector<TrackingParticle>& part      = (partsCombined) ;
  const std::vector<CaloParticle>& caloParticles = (caloPartsCombined) ;
  


  float vx = 0.;
  float vy = 0.;
  float vz = 0.;
  if(vtxs.size()!=0){
    vx = vtxs[0].position().x();
    vy = vtxs[0].position().y();
    vz = vtxs[0].position().z();
  }

  vz = 0.;

  h_Vtx_x->Fill(vx);
  h_Vtx_y->Fill(vy);
  h_Vtx_z->Fill(vz);


  for(unsigned int i=0;i<part.size();++i){
    float dvx=0.;
    float dvy=0.;
    float dvz=0.;
    

    if(part[i].decayVertices().size()>=1){   
      dvx=part[i].decayVertices()[0]->position().x();
      dvy=part[i].decayVertices()[0]->position().y();
      dvz=part[i].decayVertices()[0]->position().z();

      h_Vtx_dvx->Fill(dvx);
      h_Vtx_dvy->Fill(dvy);
      h_Vtx_dvz->Fill(dvz);
    }
  }
  

  HGCRecHitCollection NewrechitsEE;
  HGCRecHitCollection NewrechitsFH;

  //make a map detid-rechit
  std::map<DetId,const HGCRecHit*> hitmap;
  switch(algo){
  case 1:
    {
      iEvent.getByToken(_recHitsEE,recHitHandleEE);
      iEvent.getByToken(_recHitsFH,recHitHandleFH);
      iEvent.getByToken(_recHitsBH,recHitHandleBH);

      NewrechitsEE = *recHitHandleEE;
      NewrechitsFH = *recHitHandleFH;
      for(unsigned int i = 0; i < NewrechitsEE.size(); ++i){
	
	const HGCalDetId hitid = NewrechitsEE[i].detid();
	int rhL = recHitTools.getLayerWithOffset(hitid);
	hitmap[NewrechitsEE[i].detid()] = &NewrechitsEE[i];
      }
      for(unsigned int i = 0; i < NewrechitsFH.size(); ++i){
	
	const HGCalDetId hitid = NewrechitsFH[i].detid();
	int rhL = recHitTools.getLayerWithOffset(hitid);
	hitmap[NewrechitsFH[i].detid()] = &NewrechitsFH[i];
      }
      break;
    }
  case 2:
    {
      iEvent.getByToken(_recHitsEE,recHitHandleEE);
      NewrechitsEE = *recHitHandleEE;
      for(unsigned int i = 0; i < NewrechitsEE.size(); i++){
	hitmap[NewrechitsEE[i].detid()] = &NewrechitsEE[i];
      }
      break;
    }
  case 3:
    {
      iEvent.getByToken(_recHitsFH,recHitHandleFH);
      iEvent.getByToken(_recHitsBH,recHitHandleBH);
      NewrechitsFH = *recHitHandleFH;
      for(unsigned int i = 0; i < NewrechitsFH.size(); i++){
	hitmap[NewrechitsFH[i].detid()] = &NewrechitsFH[i];
      }
      break;
    }
  default:
    break;
  }

  ////////////////////
  float etaGen = -1;
  float ptGen = -1;
  float eGen = -1;
  float phiGen = -1;
  float xGen = -1;
  float yGen = -1;
  float zGen = -1;
  bool evtGood = false;
  float axX = -1;
  float axY = -1;
  float axZ = -1;


  SimClusterRefVector simClusterRefVectorChosen;
  std::map<DetId,float> hitsAndFractionChosen;

  if(debugCOUT) std::cout<< " >>> now caloparticles " << std::endl;

  std::array<double,3> vtx{ {vx, vy, vz}};


  int numbCaloPart = 0;
  if(debugCOUT4) std::cout << " >>> caloParticles.size() = " << caloParticles.size() << std::endl;
  // loop over caloParticles
  for (std::vector<CaloParticle>::const_iterator it_caloPart = caloParticles.begin(); it_caloPart != caloParticles.end(); ++it_caloPart){
    const SimClusterRefVector simClusterRefVector = it_caloPart->simClusters();

    //look at event with genParticle non interacting 
    if(debugCOUT4)    std::cout << " caloParticles loop 1 simClusterRefVector.size() = " << simClusterRefVector.size() << " eta = " << it_caloPart->eta() << " pdgID = " 
				<< it_caloPart->pdgId() << " energy = " << it_caloPart->pt() << " eventId().event() = " << it_caloPart->eventId().event() 
				<< " eventId().bunchCrossing() = " << it_caloPart->eventId().bunchCrossing() << " Z vtx = " << vz << std::endl; 
    if(CaloPartPDGID == 22 && (simClusterRefVector.size() != 2 || std::abs(it_caloPart->pdgId()) != 22 || it_caloPart->pt() != particleGenPt) && 
       (it_caloPart->eta() < 1.75 || it_caloPart->eta() > 2.75)) continue;
    if(CaloPartPDGID == 211 && (simClusterRefVector.size() != 1 || std::abs(it_caloPart->pdgId()) != 211 || it_caloPart->pt() != particleGenPt) && 
       (it_caloPart->eta() != 1.75 || it_caloPart->eta() != 2.7)) continue;
    if(CaloPartPDGID == 130 && (simClusterRefVector.size() > 1 || std::abs(it_caloPart->pdgId()) != 130 || it_caloPart->pt() != particleGenPt) && 
       (it_caloPart->eta() != 1.75 || it_caloPart->eta() != 2.7)) continue;
    if(CaloPartPDGID == -22 && (simClusterRefVector.size() > 1 || it_caloPart->pdgId() != 22 || it_caloPart->pt() != particleGenPt)) continue;
    if(CaloPartPDGID == -130 && (simClusterRefVector.size() > 1 || it_caloPart->pdgId() != 130 || it_caloPart->pt() != particleGenPt)) continue;

    etaGen = it_caloPart->eta();
    ptGen = it_caloPart->pt();
    eGen = it_caloPart->energy();
    phiGen = it_caloPart->phi();
    xGen = it_caloPart->momentum().x();
    yGen = it_caloPart->momentum().y();
    zGen = it_caloPart->momentum().z();

    //events produced at eta > 0, reject if in other direction 
    if(etaGen < 0) continue;
    if(etaGen < binStart || etaGen > binEnd) continue;
    ++numbCaloPart;
    if(numbCaloPart > 1) continue;

    evtGood = true;
    simClusterRefVectorChosen = simClusterRefVector;
    const SimCluster simClusterChosen = (*(*(simClusterRefVectorChosen.begin())));
    const std::vector<std::pair<uint32_t,float> > hits_and_fractions = simClusterChosen.hits_and_fractions();

    if(debugCOUT4) std::cout<< " caloparticles survived zGen = " << zGen << " xGen = " << xGen << " yGen = " << yGen << std::endl;

    //shower axis by recHits
    float axisX = 0;
    float axisY = 0;
    float axisZ = 0;
    float sumEnergyToNorm = 0;
    GlobalPoint showerAxis;


    /*    
    if(debugCOUT) std::cout<< " before showerAxis from recHits" << std::endl;
  
    //loop on rechit - matched to gen => shower axis (not really used anyway)
    for (std::vector<std::pair<uint32_t,float> >::const_iterator it_haf = hits_and_fractions.begin(); it_haf != hits_and_fractions.end(); ++it_haf) {
      hitsAndFractionChosen[it_haf->first] = it_haf->second;
      DetId hitid = (it_haf->first);
      float rhEnergy = 0;
	
      std::map<DetId, const HGCRecHit*>::iterator trovatore = hitmap.find(hitid);
      if(trovatore == hitmap.end()){
	continue;
      }
      else if(recHitTools.getEta(hitid)*it_caloPart->eta() < 0){
	continue;
      }
      else{
	const HGCRecHit *hit = hitmap[hitid];

	rhEnergy = hit->energy();
	sumEnergyToNorm += rhEnergy*it_haf->second;
	axisX += rhEnergy*it_haf->second * recHitTools.getPosition(hitid).x();
	axisY += rhEnergy*it_haf->second * recHitTools.getPosition(hitid).y();
	axisZ += rhEnergy*it_haf->second * recHitTools.getPosition(hitid).z();
      }
    }
    showerAxis = GlobalPoint(axisX/sumEnergyToNorm, axisY/sumEnergyToNorm, (axisZ - vz)/sumEnergyToNorm);
    axX = showerAxis.x();
    axY = showerAxis.y();
    axZ = showerAxis.z();
    */


    if(evtGood) break;
  }


  if(!evtGood) return;
  ++nEventsGood;

  //  std::cout << " Xvtx = " << vx << " Yvtx = " << vy << " Zvtx = " << vz << std::endl;

  if(debugCOUT4){
    const SimCluster simClusterBeg = (*(*(simClusterRefVectorChosen.begin())));
    std::cout<< " evtGood = " << evtGood << " simClusterRefVectorChosen.size = " << simClusterRefVectorChosen.size() << " eta = " << simClusterBeg.eta() << std::endl;
  }

 
  //need to build a vector with time of recHits
  //then compute time on the vector
  std::vector<float> timePerEtaRadiusDistr[6][4];
  TH1F* timePerEtaRadiusHisto[6][4];
  for(int iet=0; iet<nBinsEta; ++iet){
    for(int irad=0; irad<nBinsRad; ++irad){
      timePerEtaRadiusDistr[iet][irad].clear();
      timePerEtaRadiusHisto[iet][irad] = new TH1F(Form("timePerEtaRadiusHisto_eta%d_iR%d", iet, irad), "", 5000, -3., 22.);
    }
  }

  
  if(debugCOUT) std::cout<< " after showerAxis " << std::endl;
  /////////////////////////////////////////////////////////////////
  UtilClasses utilsMet = UtilClasses(etaGen, phiGen);
  std::array<double,3> fromAxis;
  if(CaloPartPDGID > 0) {
    fromAxis[0] = (xGen);
    fromAxis[1] = (yGen);
    fromAxis[2] = (zGen);
  }    
  //use a trick to check direction in pure PU, without signal
  //look in direction opposite to genParticle if CaloPartPDGID < 0 (= -1.*  particle PDGID)
  if(CaloPartPDGID < 0) {
    fromAxis[0] = (-1.*xGen);
    fromAxis[1] = (-1.*yGen);
    fromAxis[2] = (-1.*zGen);
  }
  fromAxisX->Fill(fromAxis[0]);
  fromAxisY->Fill(fromAxis[1]);
  fromAxisZ->Fill(fromAxis[2]);

  //check all the hits in the cone around the gen direction  
  
  int totRHPerEtaRadius_allTime[6][4];
  int totRHPerEtaRadius[6][4];
  float timePerEtaRadius[6][2];
  float timePerEtaRadiusAvgInt[6][2];

  for(int iet=0; iet<nBinsEta; ++iet){
    for(int irad=0; irad<nBinsRad; ++irad){
      totRHPerEtaRadius_allTime[iet][irad] = 0;
      totRHPerEtaRadius[iet][irad] = 0;
      timePerEtaRadius[iet][irad] = 0.;
      timePerEtaRadiusAvgInt[iet][irad] = 0.;
    }
  }
  

  TLorentzVector testBetaCorrection;
  h2_PhiVsEta_CaloParts->Fill(phiGen, etaGen);
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  for(std::map<DetId, const HGCRecHit*>::iterator iop=hitmap.begin(); iop != hitmap.end(); ++iop){
    const HGCalDetId hitid = iop->first;
    const HGCRecHit* hit = iop->second;
    
    if(hit->time() == -1) continue;

    bool found = false;
    float rhEnergy = hit->energy();
    float rhTime = hit->time() - timeOffset;
    float CPfraction = 0.;
    float rhX   = recHitTools.getPosition(hitid).x();
    float rhY   = recHitTools.getPosition(hitid).y();
    int rhL     = recHitTools.getLayerWithOffset(hitid);
    float rhEta = recHitTools.getEta(recHitTools.getPosition(hitid));
    float rhPhi = recHitTools.getPhi(recHitTools.getPosition(hitid));
    float rhZ   = utilsMet.layerToZ(rhL, rhEta);
    float rhPt  = rhEnergy/cosh(rhEta);

    h_rhTime->Fill(rhTime);
    //h_rhZUnf->Fill(rhZ);
    //h_PhiUnfiltered->Fill(rhPhi);
    //h_EtaUnfiltered->Fill(rhEta);
    //if(rhZ > 300) std::cout<<rhZ<<std::endl;
    //h2_recHXY->Fill(rhPhi, rhEta);
    if(debugCOUT3)    std::cout << " loop over hits eta = " << rhEta  << std::endl;
    if(rhEta < 0 && etaGen > 0) continue;
    if(rhEta > 0 && etaGen < 0) continue;
    h_rhTimeFilter1->Fill(rhTime);
    //h_rhZUnf->Fill(rhZ);
    //h_PhiUnfiltered->Fill(rhPhi);
    //h_EtaUnfiltered->Fill(rhEta);
    
    std::array<double,3> to{ {0., 0., rhZ} };
    utilsMet.layerIntersection(to, fromAxis, vtx);
    float deltaR = sqrt(pow(to[0] - rhX, 2) + pow(to[1] - rhY, 2));
    toAxisX->Fill(to[0]);
    toAxisY->Fill(to[1]);
    toAxisZ->Fill(to[2]);
    deltaRUnf->Fill(deltaR);
    h_rhZUnf->Fill(rhZ);
    h_PhiUnfiltered->Fill(rhPhi);
    h_EtaUnfiltered->Fill(rhEta);


    //save time => no interest wrt large radii
    if(deltaR > 5) continue;
    h_PhiFiltered->Fill(rhPhi);
    h_EtaFiltered->Fill(rhEta);
    deltaRFiltered->Fill(deltaR);
    h_rhTimeFilter2->Fill(rhTime);
    h2_recHXYFilter2->Fill(rhPhi, rhEta);
    h_rhZFiltered->Fill(rhZ);
    //std::cout<<rhZ<<std::endl;
    int etaBin = int((std::abs(etaGen) - binStart) / binWidth);
    if(debugCOUT4) std::cout << " etaBin = " << etaBin << std::endl;
    int iRadBin = -1;
    //identify bin of radius around the gen axis
    for(int ir=0; ir<nBinsRad; ++ir){
      if(deltaR < radiusEtaRad[etaBin][ir]) {
	iRadBin = ir;
	break;
      }
    }
  
    if(debugCOUT4)    std::cout << " radius = " << deltaR  << std::endl;
    
    if(debugCOUT4) std::cout << " >>> rhPt = " << rhPt << " rhEnergy = " << rhEnergy << " rhEta = " << rhEta << " found = " << found << " time = " << rhTime << std::endl;

    //to extract some conversion factors
    unsigned int layer = recHitTools.getLayerWithOffset(hitid);
    int thick = int(recHitTools.getSiThickness(hitid)) / 100 - 1;
    
    int sectionType = -1;
    if(hitid.det() == DetId::HGCalEE || hitid.det() == DetId::HGCalHSi) sectionType = 0;
    else sectionType = 2;
    if(sectionType == 0 && rhL > 28) sectionType = 1;
    
    
    if(debugCOUT4)  std::cout << " thick = " << thick << " sectionType = " << sectionType << " rhL = " << rhL << std::endl;

    int energyMIP = 0.;
    if(sectionType == 2) energyMIP = hit->energy()/keV2GeV * keV2MIP;
    else if(sectionType == 0 || sectionType == 1) energyMIP = hit->energy()/scaleCorrection.at(thick)/keV2GeV / (weights.at(layer)/keV2MeV);
    
    float energyCharge = 0.;
    if(sectionType == 2) energyCharge = energyMIP * 1.;
    else if(sectionType == 0 || sectionType == 1) energyCharge = energyMIP * fCPerMIP[thick];
    
    double sigmaNoiseMIP = 1.;
    if(sectionType == 2) sigmaNoiseMIP = noiseMIP;
    else if(sectionType == 0 || sectionType == 1) sigmaNoiseMIP = noisefC[thick]/fCPerMIP[thick];
    
    float charge = energyCharge;
    float MIP = energyMIP;
    float SoverN = energyMIP / sigmaNoiseMIP;

    cellThick_Rvseta->Fill(rhEta, sqrt(rhX*rhY+rhX*rhY), thick+1);
    cellThick_RvsLayer->Fill(rhL, sqrt(rhX*rhY+rhX*rhY), thick+1);

    if(debugCOUT4) std::cout << " >>> iRadBin = " << iRadBin << " MIP = " << MIP << " sectionType = " << sectionType 
			     << " thick = " << thick << " CPfraction = " << CPfraction << std::endl;

    if(iRadBin != -1) {
      for(int ir=iRadBin; ir<nBinsRad; ++ir){
	totRHPerEtaRadius_allTime[etaBin][ir] += 1;

	h_Charge_vs_Time[etaBin][ir]->Fill(rhTime, charge);
	h_VtxSurvived_z->Fill(vz);
	
	//time is computed wrt  0-25ns + offset and set to -1 if no time
	if(rhTime + timeOffset > 0.){
	  timePerEtaRadius[etaBin][ir] += rhTime;
	  totRHPerEtaRadius[etaBin][ir] += 1;
	  
	  timePerEtaRadiusDistr[etaBin][ir].push_back(rhTime);
	  timePerEtaRadiusHisto[etaBin][ir]->Fill(rhTime);
	  hTime_Eta_dRadius[etaBin][ir]->Fill(rhTime);
	}//with Time

      }//loop over radii
    }//radius ok
  }// loop over rechits

  hgcalsimclustertime::ComputeClusterTime compute_clustertime;
  for(int iet=0; iet<nBinsEta; ++iet){
    for(int irad=0; irad<2; ++irad){
      
      float time  = -99.;
      std::pair<float, float> time_pair;
      //hgcalsimclustertime
      if(timePerEtaRadiusDistr[iet][irad].size() >= 3) time_pair = compute_clustertime.fixSizeHighestDensity(timePerEtaRadiusDistr[iet][irad]);
      time = time_pair.first;
      timePerEtaRadiusAvgInt[iet][irad] = time;
    }
  }

  
  if(debugCOUT4){
    std::cout << " >>> end time computation " << std::endl;
  }
  
  
  //fill average distribution per eta - rad bin
  for(int iet=0; iet<nBinsEta; ++iet){
    for(int irad=0; irad<nBinsRad; ++irad){
	if(totRHPerEtaRadius_allTime[iet][irad] == 0.) continue;
	totEvtsEtaRadius[iet][irad] += 1;
	
	testBetaCorrection.SetPtEtaPhiE(ptGen, etaGen, phiGen, eGen);
	
	if(timePerEtaRadiusAvgInt[iet][irad] != -99.){
	  totEvtsEtaRadius_withTime[iet][irad] += 1;
	  float distVtx = sqrt(pow(axX-vx, 2) + pow(axY-vy, 2) + pow(axZ-vz, 2));
	  //should use the distance up to the start of the shower => at least barycenter of hits on the cluster                                                      
	  float correction = distVtx * (testBetaCorrection.Beta() - 1.)/ testBetaCorrection.Beta() / (0.1*CLHEP::c_light); //cm/ns                                   

	  hAverageTime_Eta_dRadius_AvgInt[iet][irad]->Fill(timePerEtaRadiusAvgInt[iet][irad]);
	  hAverageTime_Eta_dRadius_AvgIntCorr[iet][irad]->Fill(timePerEtaRadiusAvgInt[iet][irad]+correction);
	}
	hFractionHitsWithTime_Eta_dRadius[iet][irad]->Fill(1.*totRHPerEtaRadius[iet][irad]/totRHPerEtaRadius_allTime[iet][irad]);

	hTotHits_Eta_dRadius[iet][irad]->Fill(totRHPerEtaRadius_allTime[iet][irad]);
	hTotHitsWithTime_Eta_dRadius[iet][irad]->Fill(totRHPerEtaRadius[iet][irad]);

    }
  }
  
  if(debugCOUT) std::cout<< " end of event process " << std::endl;
}

void
HGCalTimingAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
HGCalTimingAnalyzer::endJob()
{
  //  if(debugCOUT) std::cout<< " bau 7 " << std::endl;
  for(int iet=0; iet<nBinsEta; ++iet){
    for(int irad=0; irad<nBinsRad; ++irad){
      if(totEvtsEtaRadius[iet][irad] != 0)
        hFractionEvents_HitsWithTime_Eta_dRadius[iet][irad]->Fill(1.*totEvtsEtaRadius_withTime[iet][irad]/totEvtsEtaRadius[iet][irad]);
    }
  }

  std::cout << " totEvents = " << nEvents << " events good = " << nEventsGood << " fraction non interacting = " << 1.*nEventsGood/nEvents << std::endl;

}


// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HGCalTimingAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HGCalTimingAnalyzer);
