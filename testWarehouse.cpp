#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <Eigen/Eigen>
#include <yaml-cpp/yaml.h>

#include "Domains/Warehouse.h"
#include "Domains/WarehouseIntersectionsTime.h"
#include "Domains/WarehouseIntersections.h"
#include "Domains/WarehouseLinksTime.h"
#include "Domains/WarehouseLinks.h"
#include "Domains/WarehouseCentralisedTime.h"
#include "Domains/WarehouseCentralised.h"
#include "threadpool.hpp"

using std::vector ;
using std::string ;
using namespace Eigen ;

void WarehouseSimulationSingleRun(int r, YAML::Node configs){
  srand(r+1); // increment random seed

  size_t nEps = configs["neuroevo"]["epochs"].as<size_t>();
  string agentType = configs["simulation"]["agents"].as<string>();
  Warehouse * trainDomain ;
  if (agentType.compare("intersection_t") == 0){
    trainDomain = new WarehouseIntersectionsTime(configs) ;
  }
  else if (agentType.compare("intersection") == 0){
    trainDomain = new WarehouseIntersections(configs) ;
  }
  else if (agentType.compare("link_t") == 0){
    trainDomain = new WarehouseLinksTime(configs) ;
  }
  else if (agentType.compare("link") == 0){
    trainDomain = new WarehouseLinks(configs) ;
  }
  else if (agentType.compare("centralised_t") == 0){
    trainDomain = new WarehouseCentralisedTime(configs) ;
  }
  else if (agentType.compare("centralised") == 0){
    trainDomain = new WarehouseCentralised(configs) ;
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

void WarehouseSimulationTestSingleRun(int r, YAML::Node configs){
  srand(r+1); // increment random seed

  string agentType = configs["simulation"]["agents"].as<string>();
  Warehouse * testDomain ;
  if (agentType.compare("intersection_t") == 0){
    testDomain = new WarehouseIntersectionsTime(configs) ;
  }
  else if (agentType.compare("intersection") == 0){
    testDomain = new WarehouseIntersections(configs) ;
  }
  else if (agentType.compare("link_t") == 0){
    testDomain = new WarehouseLinksTime(configs) ;
  }
  else if (agentType.compare("link") == 0){
    testDomain = new WarehouseLinks(configs) ;
  }
  else if (agentType.compare("centralised_t") == 0){
    testDomain = new WarehouseCentralisedTime(configs) ;
  }
  else if (agentType.compare("centralised") == 0){
    testDomain = new WarehouseCentralised(configs) ;
  }
  else{
    std::cout << "ERROR: Currently only configured for 'intersection' or 'link' agents! Exiting.\n" ;
    exit(1) ;
  }
  testDomain->InitialiseMATeam() ;
  
  int runs = configs["neuroevo"]["runs"].as<int>();
  string domainDir = configs["domain"]["folder"].as<string>() ;
  string resFolder = configs["results"]["folder"].as<string>() ;
  std::stringstream ss_eval ;
  ss_eval << domainDir << resFolder << configs["results"]["evaluation"].as<string>() << "_" << r << ".csv" ;
  string eval_str = ss_eval.str() ;
  char mkdir[100] ;
  sprintf(mkdir,"mkdir -p %s",(domainDir + resFolder).c_str()) ;
  system(mkdir) ;
  testDomain->OutputPerformance(eval_str) ;
  
  if (r == runs-1){
    sprintf(mkdir,"mkdir -p %s",(domainDir + resFolder + "Replay/").c_str()) ;
    system(mkdir) ;
    std::stringstream ss_agv_s ;
    std::stringstream ss_agv_e ;
    ss_agv_s << domainDir << resFolder << "Replay/AGV_states.csv" ;
    ss_agv_e << domainDir << resFolder << "Replay/AGV_edges.csv" ;
    std::stringstream ss_a_s ;
    std::stringstream ss_a_a ;
    ss_a_s << domainDir << resFolder << "Replay/agent_states.csv" ;
    ss_a_a << domainDir << resFolder << "Replay/agent_actions.csv" ;
    testDomain->OutputEpisodeReplay(ss_agv_s.str(), ss_agv_e.str(), ss_a_s.str(), ss_a_a.str()) ;
  }
  
  cout << "Reading champion team from file: " ;
  string ev_str = configs["mode"]["eval_file"].as<string>() ;
  ifstream evalFile(ev_str.c_str()) ;
  cout << ev_str.c_str() << "..." ;
  if (!evalFile.is_open()){
    cout << "\nFile: " << ev_str.c_str() << " not found, exiting.\n" ;
    exit(1) ;
  }
  vector< vector<size_t> > evals ;
  std::string line ;
  while (getline(evalFile,line))
  {
    stringstream lineStream(line) ;
    string cell ;
    vector<size_t> ev ;
    while (getline(lineStream,cell,','))
    {
	    ev.push_back((size_t)atoi(cell.c_str())) ;
    }
    evals.push_back(ev) ;
  }
  vector<size_t> team ;
  for (size_t i = 7; i < evals[evals.size()-1].size(); i++){
    team.push_back(evals[evals.size()-1][i]) ;
  }
  cout << "complete.\n" ;
  
  testDomain->LoadPolicies(configs) ;
  testDomain->ResetEpochEvals() ;
  testDomain->SimulateEpoch(team) ;

  if (r == runs-1){
    string nn_str = domainDir + resFolder + configs["results"]["policies"].as<string>() ;
    std::cout << "Writing control policies to file: " << nn_str << "..." ;
    testDomain->OutputControlPolicies(nn_str) ;
    std::cout << "complete.\n" ;
  }
  
  delete testDomain ;
  testDomain = 0 ;
  
  std::cout << "Testing complete!\n" ;
}

void WarehouseSimulation(){
  string config_file = "config.yaml";
  YAML::Node configs = YAML::LoadFile(config_file);
  
  string mode = configs["mode"]["type"].as<string>() ;
  int runs = configs["neuroevo"]["runs"].as<int>();
  ThreadPool pool(6) ;
  
  if (mode.compare("train") == 0){
    // Start the runs
    for (int r = 0; r < runs; r++)
    {
      pool.schedule(std::bind(WarehouseSimulationSingleRun, r, configs));
    }
//    for (int r = 0; r < runs; r++){
//      WarehouseSimulationSingleRun(r, configs) ;
//    }
  }
  else if (mode.compare("test") == 0){
//    // Start the runs
//    for (int r = 0; r < runs; r++)
//    {
//      pool.schedule(std::bind(WarehouseSimulationTestSingleRun, r, configs));
//    }
    for (int r = 0; r < runs; r++){
      WarehouseSimulationTestSingleRun(r, configs) ;
    }
//    Warehouse * testDomain = new Warehouse(configs) ;
//    testDomain->ExecutePolicies(configs) ;
//    
//    string domainDir = configs["domain"]["folder"].as<string>() ;
//    string resFolder = configs["results"]["folder"].as<string>() ;
//    string nn_str = domainDir + resFolder + "neural_nets_check.txt" ;
//    std::cout << "Writing control policies to file: " << nn_str << "...\n" ;
//    testDomain->OutputControlPolicies(nn_str) ;
//    std::cout << "Test execution complete.\n" ;
//    
//    delete testDomain ;
//    testDomain = 0 ;
  }
  else{
    std::cout << "Error: unknown mode! Exiting.\n" ;
    exit(1) ;
  }
}

int main(){
  
  WarehouseSimulation() ;
  
  return 0 ;
}
