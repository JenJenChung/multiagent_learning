#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <Eigen/Eigen>
#include <yaml-cpp/yaml.h>

#include "Domains/Warehouse.h"
#include "Domains/WarehouseIntersections.h"
#include "Domains/WarehouseLinks.h"
#include "threadpool.hpp"

using std::vector ;
using std::string ;
using namespace Eigen ;

void WarehouseSimulationSingleRun(int r, YAML::Node configs){
  srand(r+1); // increment random seed

  size_t nEps = configs["neuroevo"]["epochs"].as<size_t>();
  string agentType = configs["simulation"]["agents"].as<string>();
//  Warehouse * trainDomain = new WarehouseIntersections(configs) ;
//  trainDomain->InitialiseMATeam() ;
  Warehouse * trainDomain ;
  if (agentType.compare("intersection") == 0){
    trainDomain = new WarehouseIntersections(configs) ;
  }
  else if (agentType.compare("link") == 0){
    trainDomain = new WarehouseLinks(configs) ;
  }
  else{
    std::cout << "ERROR: Currently only configured for 'intersection' or 'link' agents! Exiting.\n" ;
    exit(1) ;
  }
  trainDomain->InitialiseMATeam() ;
  
  int runs = configs["neuroevo"]["runs"].as<int>();
  string domainDir = configs["domain"]["folder"].as<string>() ;
  string resFolder = configs["results"]["folder"].as<string>() ;
  std::stringstream ss_eval ;
  ss_eval << domainDir << resFolder << configs["results"]["evaluation"].as<string>() << "_" << r << ".csv" ;
  string eval_str = ss_eval.str() ;
  char mkdir[100] ;
  sprintf(mkdir,"mkdir -p %s",(domainDir + resFolder).c_str()) ;
  system(mkdir) ;
  trainDomain->OutputPerformance(eval_str) ;
  
  for (size_t n = 0; n < nEps; n++){
  
    if (r == runs-1){
      if (n == 0 || n == nEps-1){
        sprintf(mkdir,"mkdir -p %s",(domainDir + resFolder + "Replay/").c_str()) ;
        system(mkdir) ;
        std::stringstream ss_agv_s ;
        std::stringstream ss_agv_e ;
        ss_agv_s << domainDir << resFolder << "Replay/AGV_states_" << n << ".csv" ;
        ss_agv_e << domainDir << resFolder << "Replay/AGV_edges_" << n << ".csv" ;
        std::stringstream ss_a_s ;
        std::stringstream ss_a_a ;
        ss_a_s << domainDir << resFolder << "Replay/agent_states_" << n << ".csv" ;
        ss_a_a << domainDir << resFolder << "Replay/agent_actions_" << n << ".csv" ;
        trainDomain->OutputEpisodeReplay(ss_agv_s.str(), ss_agv_e.str(), ss_a_s.str(), ss_a_a.str()) ;
      }
      else{
        trainDomain->DisableEpisodeReplayOutput() ;
      }
    }
    
    std::cout << "Epoch " << n << "...\n" ;
    trainDomain->EvolvePolicies(n==0) ;
    trainDomain->ResetEpochEvals() ;
    trainDomain->SimulateEpoch() ;
  }

  if (r == runs-1){
    string nn_str = domainDir + resFolder + configs["results"]["policies"].as<string>() ;
    std::cout << "Writing control policies to file: " << nn_str << "..." ;
    trainDomain->OutputControlPolicies(nn_str) ;
    std::cout << "complete.\n" ;
  }
  
  delete trainDomain ;
  trainDomain = 0 ;
  
  std::cout << "Training complete!\n" ;
}

void WarehouseSimulation(){
  string config_file = "config.yaml";
  YAML::Node configs = YAML::LoadFile(config_file);
  
  int runs = configs["neuroevo"]["runs"].as<int>();
  
  ThreadPool pool(4) ;
  
  // Start the runs
  for (int r = 0; r < runs; r++)
  {
    pool.schedule(std::bind(WarehouseSimulationSingleRun, r, configs));
  }
//  for (int r = 0; r < runs; r++){
//    WarehouseSimulationSingleRun(r, configs) ;
//  }
  
//  Warehouse * testDomain = new Warehouse(configs) ;
//  testDomain->ExecutePolicies(configs) ;
//  
//  string domainDir = configs["domain"]["folder"].as<string>() ;
//  string resFolder = configs["results"]["folder"].as<string>() ;
//  string nn_str = domainDir + resFolder + "neural_nets_check.txt" ;
//  std::cout << "Writing control policies to file: " << nn_str << "...\n" ;
//  testDomain->OutputControlPolicies(nn_str) ;
//  std::cout << "Test execution complete.\n" ;
//  
//  delete testDomain ;
//  testDomain = 0 ;
}

int main(){
  
  WarehouseSimulation() ;
  
  return 0 ;
}
