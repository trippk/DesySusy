import FWCore.ParameterSet.Config as cms


filterOneLepton	 = cms.EDFilter("SusyDESY_Filter",
                	        FilterResult = cms.InputTag('filterResultOneLepton:DESYfilterPassedOneLepton' )
                               )	
