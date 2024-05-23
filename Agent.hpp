#pragma once
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <unordered_map>

//#include "FrontMatter.h"  //meta parameter macro
//#include "Xor.h"        //Xor shift random generater
#include "./SCORE.hpp"      //Scoring class for statistics through generations
#include "./Bandit.hpp"  //Bandit task class
#include "./QLearning.hpp"  //Q-Learning class
#include "./MLP.hpp"

//for Maintenance switch
#define CHKAG 0  // status cheking switch (for test)

using std::string;
using std::unordered_map;


//================================ Agent class declaration ================================//
class Agent {
 private:
  // phenotype of an agent
  unordered_map<string, double> phenotype;
  Reinforcement ql;
  NN mlp;
  SOM som;

 public:
  //unordered_map<string, double> ln_results;
  double rewards, mets, finalmet;
  double cbrate, cbtime, cbeffect, cbloss, ambiguity;
  double bare_agent, ic_agent, mtm_agent, icmtm_agent;

  //agents score of each action
  double ep_reward[ACTION_LIMIT];
  double ep_met[ACTION_LIMIT];
  double ep_cbtime[ACTION_LIMIT];
  double ep_cbloss[ACTION_LIMIT];
  double ep_cbeffect[ACTION_LIMIT];
  double ep_waiting[ACTION_LIMIT];
  double ep_ambiguity[ACTION_LIMIT];

  Agent(){}

  Agent(unordered_map<string, double> ag_parameter){
    // reset score of one agent
    rewards = 0, mets = 0, finalmet = 0;
    cbrate = 0, cbtime = 0, cbeffect = 0, cbloss = 0, ambiguity = 0;

    // set the agent's parameter
    for (int i = 0; i < TOTAL_PHENOTYPE; ++i) phenotype[phenotype_name[i]] = ag_parameter[phenotype_name[i]];
    ql.set_parameter(ag_parameter);
    mlp.init_nn(INPUTCELL, ag_parameter["hiddencell"], OUTPUTCELL, ag_parameter["nnalpha"]);
  }

  ~Agent(){
  }

  void init_episodes(); //generation loop
  void init(ContextBandit *cbandit, unordered_map<string, double> ag_parameter);
  void lifetime(ContextBandit *cbandit);
  void ep_generation_ave();
  void display_results(int agent_id);
};
//=========================================================================================//

void Agent::init(ContextBandit *cbandit, unordered_map<string, double> ag_parameter){
    // reset score of one agent
    rewards = 0, mets = 0, finalmet = 0;
    cbrate = 0, cbtime = 0, cbeffect = 0, cbloss = 0, ambiguity = 0;

    // set the agent's parameter
    for (int i = 0; i < TOTAL_PHENOTYPE; ++i) phenotype[phenotype_name[i]] = ag_parameter[phenotype_name[i]];
    bare_agent = 0, ic_agent = 0, mtm_agent = 0, icmtm_agent = 0;
    if(phenotype["ic"] == 0 && phenotype["mtm"] == 0){bare_agent = 1;}
    else if(phenotype["ic"] == 1  && phenotype["mtm"] == 0){ic_agent = 1;}
    else if(phenotype["ic"] == 0  && phenotype["mtm"] == 1){mtm_agent = 1;}
    else if(phenotype["ic"] == 1  && phenotype["mtm"] == 1){icmtm_agent = 1;}

    ql.set_parameter(ag_parameter);
    mlp.init_nn(INPUTCELL, ag_parameter["hiddencell"], OUTPUTCELL, ag_parameter["nnalpha"]);
    som.init_som(ag_parameter["stmsize"], ag_parameter["somsize"], ag_parameter["somalpha"]);
    if(CHKAG){
      for(int i = 0; i < TOTAL_PHENOTYPE; ++i){
        cout << phenotype_name[i] << ": " << ag_parameter[phenotype_name[i]] << endl;
      }
      cout << endl;
    }

    // som exposure for preparing
    for(int prexp = 0; prexp < EXPOSURE; ++prexp){
      cbandit->stimulus(); //input sample stimulus to stm
      double exp_state[(int)ag_parameter["stmsize"]];
      for(int i = 0; i < ag_parameter["stmsize"]; ++i) exp_state[i] = cbandit->stimulus_image[cbandit->stimulus_num][i];
      som.exposure(exp_state);
    }
}

void Agent::lifetime(ContextBandit *cbandit) {
  //====================== learning start ========================//
  for (int act = 0; act < ACTION_LIMIT; act++) {
    if (CHKAG) cout << "------  action start  ------" << endl;
    ql.qlearning(cbandit, mlp, som);
    if (CHKAG){cout << "------   action finish   ------" << endl << endl;}

    //----- action results -----//
    rewards += ql.reinforcer;
    ep_reward[act] += ql.reinforcer;

    ambiguity += ql.sum_ambiguity;
    ep_ambiguity[act] += ql.sum_ambiguity;
    ep_waiting[act] += ql.sum_delay;
    if (ql.met) {
      ++mets;
      ++ep_met[act];
      if (ql.cb_time) {
        cbeffect += 1 / ql.cb_flg;
        ep_cbeffect[act] += 1 / ql.cb_time;
      }
    }
    if (ql.cb_time) {
      cbtime += ql.cb_time;
      ep_cbtime[act] += ql.cb_time;
      cbloss += ql.cb_time - 1;
      ep_cbloss[act] += ql.cb_time - 1;
    }
    //------------------------//
  }
  //======================== learning end ==========================//

  // average and final result
  cbrate = cbtime / ACTION_LIMIT;
  cbloss /= ACTION_LIMIT;
  mets /= ACTION_LIMIT;
  finalmet = ql.met;
  ambiguity /= ACTION_LIMIT;
}

void Agent::init_episodes(){
  for(int a = 0; a < ACTION_LIMIT; ++a){
    ep_reward[a] = 0;
    ep_met[a] = 0;
    ep_cbtime[a] = 0;
    ep_cbloss[a] = 0;
    ep_cbeffect[a] = 0;
    ep_waiting[a] = 0;
    ep_ambiguity[a] = 0;
  }
}




//average episodes data
void Agent::ep_generation_ave(){
  for(int a = 0; a < ACTION_LIMIT; a++){
    ep_reward[a] /= AGENTS;
    ep_met[a] /= AGENTS;
    ep_cbtime[a] /= AGENTS;
    ep_cbloss[a] /= AGENTS;
    ep_cbeffect[a] /= AGENTS;
    ep_waiting[a] /= AGENTS;
    ep_ambiguity[a] /= AGENTS;
  }
}



void Agent::display_results(int agent_id){
	cout << "== agent No. " << agent_id << " ==" << endl;
  for(int v = 0; v < TOTAL_PHENOTYPE; ++v){
    cout << phenotype_name[v] << ": " << phenotype[phenotype_name[v]] << endl;
  }
  cout << endl;
	cout << "== Lifetime Results ==" << endl;
	cout << "rewards: " << rewards << endl;
	cout << "mets: " << mets << endl;
	cout << "using CB: " << cbtime << endl;
	cout << "effect of CB: " << cbeffect << endl;
	cout << endl;
}
