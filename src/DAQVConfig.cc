#include "DAQVConfig.hh"
#include <iostream>
#include <sstream>


DAQVConfig::DAQVConfig(std::string &name){
  
  fConfigParser=0;
  fConfigParser = new utl::ConfigParser(std::string("conf/")+name+std::string("conf/.conf"));
  
}

DAQVConfig::~DAQVConfig(){
  if(fConfigParser) delete fConfigParser;
}


int DAQVConfig::GetParOrDefault(std::string group,std::string name,int val){
  return fConfigParser->HasConfig(group,name)?std::stoi(fConfigParser->GetSingleArg(group,name)):val;
}

double DAQVConfig::GetParOrDefault(std::string group,std::string name,double val){
  return fConfigParser->HasConfig(group,name)?std::stof(fConfigParser->GetSingleArg(group,name)):val;
}

std::string DAQVConfig::GetParOrDefault(std::string group,std::string name,std::string val){
  return fConfigParser->HasConfig(group,name)?fConfigParser->GetSingleArg(group,name):val;
}
