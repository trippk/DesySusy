import FWCore.ParameterSet.Config as cms

susydesytotakinematicsfilter = cms.EDProducer('SusyDESY_TotalKinematicsFilterResult',
  src             = cms.InputTag("genParticles"),
  tolerance       = cms.double(0.5),
  verbose         = cms.untracked.bool(False),
  Prefix          = cms.string("DESYtotalKinematicsFilter"),
  Suffix          = cms.string("")
)
