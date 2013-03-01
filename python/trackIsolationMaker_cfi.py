import FWCore.ParameterSet.Config as cms

susydesytrackIsolationMaker = cms.EDProducer("TrackIsolationMaker",
                                             PfCandidatesTag     = cms.InputTag("particleFlow"),
                                             VertexInputTag      = cms.InputTag("offlinePrimaryVertices"),
                                             dR_ConeSize         = cms.double(0.3),
                                             dz_CutValue         = cms.double(0.05),
                                             minPt_PFCandidate   = cms.double(10.),
                                             minLepPt_PFCandidate   = cms.double(5.),
                                             Prefix       = cms.string('DESYtracks'),
                                             Suffix       = cms.string('PF'),
                                             minTrkIso_PFCandidate   =  cms.double(0.2)
                                             )
