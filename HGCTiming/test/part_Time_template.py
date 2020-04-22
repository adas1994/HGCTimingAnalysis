import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

process = cms.Process("Demo")
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtended2026D41Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
sample_prefile =  cms.untracked.vstring('file:/hadoop/store/user/adas/EfficiencyD41OldPreMixMethod/PreMixOld/Step4/2020_04_04_06_33_44/step4_RAW2DIGI_L1Reco_RECO_RECOSIM_PAT_VALIDATION_DQM122.root')
sample_stdfile = cms.untracked.vstring('file:/hadoop/store/user/adas/EfficiencyD41OldPreMixMethod/StandardMixOld/Step3/2020_03_26_07_31_03/step3_RAW2DIGI_L1Reco_RECO_RECOSIM_PAT_VALIDATION_DQM_PU106.root')

import FWCore.Utilities.FileUtils as FileUtils
#readFiles = cms.untracked.vstring()
#readFiles.extend(FileUtils.loadListFromFile ('INPUTFILELIST') )
readFiles = cms.untracked.vstring('file:INPUTFILELIST')

#readFiles = sample_prefile

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
                            fileNames = readFiles,
                            duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                            inputCommands=cms.untracked.vstring('keep *',
                                                  #'drop EcalEBTriggerPrimitiveDigisSorted_simEcalEBTriggerPrimitiveDigis_*_HLT',
                                                  'drop l1tEMTFHit2016Extras_simEmtfDigis_CSC_HLT',
                                                  'drop l1tEMTFHit2016Extras_simEmtfDigis_RPC_HLT',
                                                  'drop l1tEMTFHit2016s_simEmtfDigis__HLT',
                                                  'drop l1tEMTFTrack2016Extras_simEmtfDigis__HLT',
                                                  'drop l1tEMTFTrack2016s_simEmtfDigis__HLT'
                                                  )
                            )
        

from HGCTimingAnalysis.HGCTiming.timeRecHitEstimator_cfi import HGCalTimeEstimator
PGENPT = 5.
CALOPARTPDGID = 22#130/310 for K_S and K_L


process.ana = cms.EDAnalyzer('HGCalTimingAnalyzer',
                             detector = cms.string("all"),
                             rawRecHits = cms.bool(True),                              
                             particleGENPT = cms.double(PGENPT),
                             CaloPartPDGID = cms.int32(CALOPARTPDGID),
                             timeOffset = HGCalTimeEstimator.timeOFFSET,
                             HGCEEInput = cms.InputTag('HGCalRecHit:HGCEERecHits'),
                             HGCFHInput = cms.InputTag('HGCalRecHit:HGCHEFRecHits'),
                             HGCBHInput = cms.InputTag('HGCalRecHit:HGCHEBRecHits'),
                             caloClusterInput = cms.InputTag('hgcalLayerClusters'),
                             dEdXweights = HGCalTimeEstimator.dEdXweights,
                             thicknessCorrection = HGCalTimeEstimator.thicknessCorrection,
                             HGCEE_fCPerMIP = HGCalTimeEstimator.HGCEE_fCPerMIP,
                             HGCEE_noisefC = HGCalTimeEstimator.HGCEE_noisefC,
                             HGCEF_noisefC = HGCalTimeEstimator.HGCEF_noisefC,
                             HGCBH_noiseMIP = HGCalTimeEstimator.HGCBH_noiseMIP,
                             nonAgedNoises = HGCalTimeEstimator.nonAgedNoises,
                             endOfLifeNoises = HGCalTimeEstimator.endOfLifeNoises,
                             HGCEE_keV2fC  = HGCalTimeEstimator.HGCEE_keV2fC,
                             HGCHEF_keV2fC = HGCalTimeEstimator.HGCHEF_keV2fC,
                             HGCHB_keV2MIP = HGCalTimeEstimator.HGCHB_keV2MIP
                             )

#OUTFILE.root
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("file:TimingValidation.root")
                                   )
process.p = cms.Path(process.ana)
