#ifndef DAQVCONFIG_HH
#define DAQVCONFIG_HH


#include "utlConfigParser.hh"

#include <vector>
#include <string>


class DAQVConfig {
public:
  DAQVConfig(std::string &name);
  DAQVConfig(){;};
  ~DAQVConfig();
  utl::ConfigParser * GetConfigParser(){return fConfigParser;};
  
  int GetParOrDefault(std::string,std::string,int);
  double GetParOrDefault(std::string,std::string,double);
  std::string GetParOrDefault(std::string,std::string,std::string);
  
private:
  utl::ConfigParser *fConfigParser;
  
};






#endif
