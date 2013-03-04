#include "SUSYBSMAnalysis/DesySusy/interface/SusyDESY_Scan.h"
#include "FWCore/Framework/interface/Event.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "boost/lexical_cast.hpp"
#include "boost/foreach.hpp"


SusyDESY_Scan::SusyDESY_Scan(const edm::ParameterSet& cfg) :
  inputTag(cfg.getParameter<edm::InputTag>("InputTag")),
  Prefix  (cfg.getParameter<std::string>  ("Prefix"  )),
  Suffix  (cfg.getParameter<std::string>  ("Suffix"  )),
  scanFormat(cfg.getParameter<std::string>("ScanFormat")),
  scanPars(cfg.getParameter<std::vector<std::string> >("ScanParameters")),
  debug(cfg.getUntrackedParameter<bool>("Debug",false))
{
  BOOST_FOREACH(const std::string& par, scanPars) 
    //produces<double>( Prefix + par + Suffix );
    //produces <bool>   ( Prefix + "HandleValid" + Suffix );
  if(cfg.existsAs<edm::VParameterSet>("AdditionalParameters") &&\
     cfg.existsAs<edm::ParameterSet>("AdditionalParameterDefaults")){
      edm::VParameterSet allAddParams = cfg.getParameter<edm::VParameterSet>("AdditionalParameters");
      edm::ParameterSet defaultAddParams = cfg.getParameter<edm::ParameterSet>("AdditionalParameterDefaults");

      std::vector<std::string> addParamNames = defaultAddParams.getParameterNamesForType<double>();
      BOOST_FOREACH(const std::string paramName, addParamNames){
	additionalDoubleDefaultVars_[paramName] = defaultAddParams.getParameter<double>(paramName);
	//produces<double>( Prefix + paramName + Suffix );
	if(debug) std::cout << paramName <<"(default):"<< additionalDoubleDefaultVars_[paramName]<<std::endl;
      }
      BOOST_FOREACH(const edm::ParameterSet addParams, allAddParams){
	ScanPoint point = ScanPoint(scanPars, addParams);
	if(additionalDoubleVars_.count(point) == 0)
	  additionalDoubleVars_[point] = std::map< std::string, double>();
	BOOST_FOREACH(const std::string paramName, addParamNames){
	  additionalDoubleVars_[point][paramName] = addParams.getParameter<double>(paramName);
	  if(debug) std::cout << paramName <<":"<< additionalDoubleVars_[point][paramName]<<std::endl;
	}
      }
    }

  edm::Service<TFileService> fs;
  scan = fs->make<TH2I>("Scan","NEvents",200,0,2000, 200, 0,2000);
}


void SusyDESY_Scan::
produce(edm::Event& event, const edm::EventSetup&) {

  boost::smatch matches;
  edm::Handle<LHEEventProduct> lhe;
  event.getByLabel(inputTag, lhe);
  if(lhe.isValid()) {
    const std::vector<std::string> comments(lhe->comments_begin(),lhe->comments_end());
    BOOST_FOREACH(const std::string& comment, comments) {
      if (boost::regex_match(comment, matches, scanFormat)) break;
    }
  }

  bool valid = matches.size() && matches[0].matched;
  if(valid && debug) std::cout <<matches[0].str() << std::endl;
  //event.put( std::auto_ptr<bool>(new bool(valid)),  Prefix + "HandleValid" + Suffix );
  ScanPoint::PointMap pointMap =  ScanPoint::PointMap();
  for(unsigned i=0; i<scanPars.size(); ++i) {
    pointMap[scanPars[i]] = !valid ? 0.0 : convert(matches[i+1].str());
    //event.put(std::auto_ptr<double>(new double(pointMap[scanPars[i]])), \
      //      Prefix + scanPars[i] + Suffix );
    if(debug) std::cout << scanPars[i] <<": "<<pointMap[scanPars[i]] <<", ";
  }

  scan->Fill( pointMap[scanPars[0]], pointMap[scanPars[1]]); 


  ScanPoint point = ScanPoint(pointMap);
  BOOST_FOREACH(const DoubleParameterMap::value_type& defaultPair, additionalDoubleDefaultVars_) {				
    bool useDefault = (additionalDoubleVars_[point].count(defaultPair.first) == 0);
    double addParam = useDefault ? defaultPair.second : additionalDoubleVars_[point][defaultPair.first];
    //event.put(std::auto_ptr<double>( new double(addParam)), Prefix + defaultPair.first + Suffix );
    if(debug) std::cout << Prefix + defaultPair.first + Suffix << " -> "<< addParam;
    if(debug) std::cout << " (default n = "<< additionalDoubleVars_[point].count(defaultPair.first)<<", "<< useDefault<< ")"<<std::endl;
  }
}

double SusyDESY_Scan::convert(std::string s) {
  if(s[0]=='m') s[0] = '-';
  return boost::lexical_cast<double>(s);
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(SusyDESY_Scan);
