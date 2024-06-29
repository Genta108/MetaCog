#pragma once
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <string>
#include <limits.h>  //for ULLONG_MAX
#include <unordered_map>

//#include "FrontMatter.h"  //meta parameter macro
#include "./Bandit.hpp"  //Bandit task class
#include "./SCORE.hpp"   //Scoring class for statistics through generations
#include "./Agent.hpp"

//for Maintenance switch
#define CHKEVO 0
#define GEN_SKIP 1000 //generation skip
#define DELAY 0 //10-200

using std::unordered_map;
using std::string;

namespace{
  void delay(int x);   //delay function
  int tarai(int x, int y, int z);  //delay subfunction
}

//==================================== Evolution class declaration ====================================//
class GeneticAlgorithm{
private:
  //info. for GA
  int generation, agent_id;
  int gene[AGENTS][GENE_LENGTH];  //current gene pool
  int post_gene[AGENTS][GENE_LENGTH];  //post gene pool

  //phenotype set of an agent
  unordered_map<string, double> phenotype;

  //results of an agent in a generation
  unordered_map<string, double> results;
  double fitness[AGENTS];
  double fitness_max;

  //function for GA
  void init_epoch();
  void initialization();
  void expression(int gene[]);
  void evaluation(Score *evo_score, ContextBandit *cbandit, string& lndir);
  void fitness_functions(double rewards, double mets);
  int selection();
  void crossing();
  void remain();
  void mutation(); //Mutation & Alternation

public:
  void evolution(string& taskpath, string& evopath, string& genepath, string& lndir);
};
//======================================================================================================//



//========================================= GA class realization =======================================//
void GeneticAlgorithm::evolution(string& taskpath, string& evopath, string& genepath, string& lndir){
  //evolution score initialization
  Score *evo_score = new Score();
  ofstream evo_print(evopath.c_str());  //file io
  evo_score->init(evo_results_name, TOTAL_EVO_RESULTS);
  evo_score->heading(evo_print);

  //task initialization
  ContextBandit *cbandit = new ContextBandit();  //task class
  cbandit->init(); //loading parameter
  ofstream task_print(taskpath.c_str());
  cbandit->printing(task_print);
  if (CHKEVO) cbandit->disptemplete();

  initialization();  //all gene initialization

  //========== Evolution loop start ==========//
  for(generation = 0; generation < GENERATION; generation++){
    if(CHKEVO) cout << "Generation: " << generation+1 << endl;
    init_epoch();
    evo_score->init_line();  //initialize generation result

    evaluation(evo_score, cbandit, lndir);  //evaluate gene

    evo_score->average(AGENTS);
    evo_score->insert_line(2, fitness_max); //column, value
    evo_score->insert_line(22, cbandit->hamming());
    evo_score->print_line(evo_print, generation);
    if(CHKEVO) evo_score->display_line();

    crossing(); //half of population

    remain();

    mutation();  //mutation of the next gene with alternation
  }
  //=========== Evolution loop end ===========//


  evo_print.close();
  delete evo_score;
  delete cbandit;

  //gene data at last generation output
  ofstream gene_print(genepath.c_str());
  for(int a = 0; a < AGENTS; a++){
    gene_print << a + 1;
    for(int l = 0; l < GENE_LENGTH; l++) gene_print << ',' << gene[a][l];
    gene_print << endl;
  }
  gene_print.close();

}



void GeneticAlgorithm::initialization(){
  for(int a = 0; a < AGENTS; a++){
    for(int l = 0; l < GENE_LENGTH; l++){
      gene[a][l] = 0;  //for test point
      post_gene[a][l] = 0;
    }
  }
}


void GeneticAlgorithm::init_epoch(){
  for(int a = 0; a < AGENTS; a++) fitness[a] = 0;
  fitness_max = 0;
  for(int i = 0; i < TOTAL_EVO_RESULTS; ++i) results[evo_results_name[i]] = 0;
}


void GeneticAlgorithm::expression(int gene[]){
  double value = 0;
  int l = 0;  //locus

  phenotype["ic"] = gene[l++];  //ic ability, gene[0]
  phenotype["mtm"] = gene[l++];  //ic ability, gene[0]

  if(RL_SW){phenotype["ic"] = 0; phenotype["mtm"] = 0;}
  if(IC_SW){phenotype["ic"] = 1; phenotype["mtm"] = 0;}
  if(MTM_SW){phenotype["ic"] = 0; phenotype["mtm"] = 1;}
  if(IC_SW && MTM_SW){phenotype["ic"] = 1; phenotype["mtm"] = 1;}

  value = STM_INIT;
  while(l < STM_DEV+2) if(gene[l++]) value += 1;  //stm_size ability < INFO_SIZE
  phenotype[phenotype_name[2]] = value;

  value = 0.05;
  while(l < STM_DEV+21) if(gene[l++]) value += 0.05;  //random exploration rate(epsiron greedy)
  phenotype[phenotype_name[3]] = value;

  value = 0.05;
  while(l < STM_DEV+40) if(gene[l++]) value += 0.05;  //learning rate
  phenotype[phenotype_name[4]] = value;

  value = 0.05;
  while(l < STM_DEV+59) if(gene[l++]) value += 0.05;  //discount rate
  phenotype[phenotype_name[5]] = value;

  value = 3;
  while(l < STM_DEV+86) if(gene[l++]) value += 1;  //hidden cell of NN <= 30
  phenotype[phenotype_name[6]] = value;

  value = 1;
  while(l < STM_DEV+95) if(gene[l++]) value += 1;  //learning rate of NN < 10
  phenotype[phenotype_name[7]] = value;

  value = 6;
  while(l < STM_DEV+107) if(gene[l++]) value += 2;  //size of SOM <= 30
  phenotype[phenotype_name[8]] = value;

  value = 1;
  while(l < STM_DEV+116) if(gene[l++]) value += 1;  //learning rate of SOM < 10
  phenotype[phenotype_name[9]] = value;
}



void GeneticAlgorithm::evaluation(Score *evo_score, ContextBandit *cbandit, string& lndir){
  //learing score initialization
  Score *ln_score = new Score();
  ln_score->init(ln_results_name, TOTAL_LN_RESULTS);

  Score *cb_score = new Score();
  string cb_results_name[3] = {"cbuse", "cbeffect", "cbloss"};
  cb_score->init(cb_results_name, 3);

  ostringstream ln_file_oss;
  ln_file_oss << lndir << "/generation" << generation+1 << "_ln.csv";  //name of gene file
  string lnpath = ln_file_oss.str();

  ostringstream cb_file_oss;
  cb_file_oss << lndir << "/generation" << generation+1 << "_cb.csv";  //name of gene file
  string cbpath = cb_file_oss.str();

  Agent *agent = new Agent();  //learning census
  agent->init_episodes();

  //======== agent loop start ========//
  for(agent_id = 0; agent_id < AGENTS; agent_id++){
    ln_score->init_line();
    expression(gene[agent_id]);  //generate phenotype of agent
    agent->init(cbandit, phenotype);

    agent->lifetime(cbandit);  //performance of each agent

    if(CHKAG){
      agent->display_results(agent_id);
      delay(DELAY);
    }

    fitness_functions(agent->rewards, agent->mets);

    double results_container[TOTAL_EVO_RESULTS] = {fitness[agent_id], 0, agent->mets, agent->finalmet, agent->cbrate, agent->cbuse, agent->cbeffect, agent->cbloss, agent->ambiguity,
      agent->ic_agent, agent->mtm_agent, agent->icmtm_agent, agent->bare_agent,
      phenotype["stmsize"], phenotype["qleps"], phenotype["qlalpha"], phenotype["qlgamma"],
      phenotype["hiddencell"], phenotype["nnalpha"], phenotype["somsize"], phenotype["somalpha"], 0};
    evo_score->counting(results_container);

  }
  //========= agent loop end =========//

  if(((generation+1) % GEN_SKIP) == 0){
    ofstream ln_print(lnpath.c_str());  //file io
    ln_print << "episode," << "reward," << "met," << "cbuse," << "cbloss," << "cbeffect,";
    ln_print << "delay," << "ambiguity" << endl;
    agent->ep_generation_ave();
    for (int t = 0; t < ACTION_LIMIT; ++t){
      ln_print << t + 1;
      ln_print << "," << agent->ep_reward[t] << "," << agent->ep_met[t] << "," << agent->ep_cbuse[t];
      ln_print << "," << agent->ep_cbloss[t] << "," << agent->ep_cbeffect[t] << "," << agent->ep_waiting[t] << "," << agent->ep_ambiguity[t];
      ln_print << endl;
    }
    ln_print.close();

    ofstream cb_print(cbpath.c_str());  //file io
    cb_print << "noise," << "cbuse/noise," << "cbeffect/noise," << "cbloss/noise" << endl;
    for (int n = 0; n < WAITING_TIME; ++n){
      cb_print << n;
      cb_print << "," << agent->noise_cbuse[n] << "," << agent->noise_cbeffect[n] << "," << agent->noise_cbloss[n];
      cb_print << endl;
    }
    cb_print.close();
  }

  delete ln_score;
  delete cb_score;
  delete agent;
}


void GeneticAlgorithm::fitness_functions(double rewards, double mets){
  if(EVALUATION == 1){
    fitness[agent_id] = rewards/(1+phenotype["ic"]*IC_COST+phenotype["mtm"]*MTM_COST+phenotype["stmsize"]*STM_COST);
  }
  else if(EVALUATION == 2){
    fitness[agent_id] = mets/(1+phenotype["ic"]*IC_COST+phenotype["mtm"]*MTM_COST+phenotype["stmsize"]*STM_COST);
  }
  if(fitness[agent_id] > fitness_max) fitness_max = fitness[agent_id];
}


void GeneticAlgorithm::crossing(){
  int a = 0;
  while(a < AGENTS/2){
    int parent1 = selection();
    int parent2 = selection();
    //cout << "parent1_fit: " << fitness[parent1] << endl;
    //cout << "parent2_fit: " << fitness[parent2] << endl;

    int crossover_point;
    double real_cp;

    while((real_cp=real_urand()) == 1.0);
    crossover_point = 1+(int)(real_cp*(GENE_LENGTH-1));
    //cout << "crossover_point: " << crossover_point << endl;

    int l;
    for(l = 0; l < crossover_point; l++){
      post_gene[a][l] = gene[parent1][l];
      post_gene[a+1][l] = gene[parent2][l];
    }
    for(l = crossover_point; l < GENE_LENGTH; l++){
      post_gene[a][l] = gene[parent2][l];
      post_gene[a+1][l] = gene[parent1][l];
    }
    a = a+2;
  }
}


int GeneticAlgorithm::selection(){
  double total = 0;
  double pointed = 0;  //roulette 0~totalFitness
  double cumulator = 0;
  double r;
  double r_fitness[AGENTS];

  for(int a = 0; a < AGENTS; ++a){
    r_fitness[a] = fitness[a]/fitness_max;
    total += r_fitness[a];
  }
  while((r=real_urand()) == 1.0);
  if(total == 0){return (int)(r*AGENTS);} //lucky
  pointed = ((double)real_urand() * total); //select
  //cout << "pointed: " << pointed <<endl;
  for(int a = 0; a < AGENTS; a++){
    cumulator = cumulator + r_fitness[a];
    if(cumulator >= pointed) return a;
  }
  return -1;
}


void GeneticAlgorithm::remain(){
  for(int a = AGENTS/2; a < AGENTS; a++){
    int remain_agent = selection();
    for(int l = 0; l < GENE_LENGTH; l++){
      post_gene[a][l] = gene[remain_agent][l];
    }
  }
}


void GeneticAlgorithm::mutation(){
  for(int a = 0; a < AGENTS; a++){
    for(int l = 0; l < GENE_LENGTH; l++){
      if(real_urand() < MT_RATE){
        if(post_gene[a][l]){
          post_gene[a][l] = 0;
        }else{
          post_gene[a][l] = 1;
        }
      }
      gene[a][l] = post_gene[a][l];  //alternation
    }
  }
}
//====================================================================================================//


namespace{
  //delay method
  void delay(int x){for(int d = 0; d < x; ++d) tarai(13, 6, 1);}

  int tarai(int x, int y, int z){
    if(x <= y) return y;
    return tarai(tarai(x-1, y, z), tarai(y-1, z, x), tarai(z-1, x, y));
  }
}
