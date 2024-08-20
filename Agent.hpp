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

 public:
  //unordered_map<string, double> ln_results;
  double rewards, mets, finalmet;
  double cbrate, cbuse, cbeffect, cbloss, ambiguity;
  double bare_agent, ic_agent, mtm_agent, icmtm_agent;

  //agents score of each action
  double ep_reward[ACTION_LIMIT];
  double ep_met[ACTION_LIMIT];
  double ep_cbuse[ACTION_LIMIT];
  double ep_cbloss[ACTION_LIMIT];
  double ep_cbeffect[ACTION_LIMIT];
  double ep_waiting[ACTION_LIMIT];
  double ep_ambiguity[ACTION_LIMIT];

  //checking behavior score
  double noise[NOISE_MAX];
  double noise_met[NOISE_MAX];
  double noise_cbuse[NOISE_MAX];
  double noise_cbeffect[NOISE_MAX];
  double noise_cbloss[NOISE_MAX];
  double noise_delay[NOISE_MAX];

  double delay[WAITING_TIME+1];
  double delay_noise[WAITING_TIME+1];
  double delay_met[WAITING_TIME+1];
  double delay_cbuse[WAITING_TIME+1];
  double delay_cbeffect[WAITING_TIME+1];
  double delay_cbloss[WAITING_TIME+1];

  Agent(){}

  Agent(unordered_map<string, double> ag_parameter){
    // reset score of one agent
    rewards = 0, mets = 0, finalmet = 0;
    cbrate = 0, cbuse = 0, cbeffect = 0, cbloss = 0, ambiguity = 0;

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
  void cb_generation_ave();
  void display_results(int agent_id);
};
//=========================================================================================//

void Agent::init(ContextBandit *cbandit, unordered_map<string, double> ag_parameter){
    // reset score of one agent
    rewards = 0, mets = 0, finalmet = 0;
    cbrate = 0, cbuse = 0, cbeffect = 0, cbloss = 0, ambiguity = 0;

    // set the agent's parameter
    for (int i = 0; i < TOTAL_PHENOTYPE; ++i) phenotype[phenotype_name[i]] = ag_parameter[phenotype_name[i]];
    bare_agent = 0, ic_agent = 0, mtm_agent = 0, icmtm_agent = 0;
    if(phenotype["ic"] == 0 && phenotype["mtm"] == 0){bare_agent = 1;}
    else if(phenotype["ic"] == 1  && phenotype["mtm"] == 0){ic_agent = 1;}
    else if(phenotype["ic"] == 0  && phenotype["mtm"] == 1){mtm_agent = 1;}
    else if(phenotype["ic"] == 1  && phenotype["mtm"] == 1){icmtm_agent = 1;}

    ql.set_parameter(ag_parameter);
    mlp.init_nn(INPUTCELL, ag_parameter["hiddencell"], OUTPUTCELL, ag_parameter["nnalpha"]);
    if(CHKAG){
      for(int i = 0; i < TOTAL_PHENOTYPE; ++i){
        cout << phenotype_name[i] << ": " << ag_parameter[phenotype_name[i]] << endl;
      }
      cout << endl;
    }
}

void Agent::lifetime(ContextBandit *cbandit) {
  //====================== learning start ========================//
  for (int act = 0; act < ACTION_LIMIT; act++) {
    if (CHKAG) cout << "------  action start  ------" << endl;
    ql.qlearning(cbandit, mlp);
    if (CHKAG){cout << "------   action finish   ------" << endl << endl;}

    //----- action results -----//
    rewards += ql.reinforcer;
    ep_reward[act] += ql.reinforcer;

    ambiguity += ql.sum_ambiguity;
    ep_ambiguity[act] += ql.sum_ambiguity;
    ep_waiting[act] += ql.sum_delay;
    noise[ql.noise]++;
    noise_delay[ql.noise] += ql.wait;
    delay[ql.wait]++;
    delay_noise[ql.wait] += ql.noise;
    if (ql.met) {
      ++mets;
      ++ep_met[act];
      ++noise_met[ql.noise];
      ++delay_met[ql.wait];
      if (ql.cb_flg) {
        cbuse += ql.cb_flg;
        ep_cbuse[act] += ql.cb_flg;
        delay_cbuse[ql.wait] += 1;
        noise_cbuse[ql.noise] += 1;
        cbeffect += 1;
        ep_cbeffect[act] += 1;
        noise_cbeffect[ql.noise] += 1;
        delay_cbeffect[ql.wait] += 1;
        cbloss += ql.cb_repeat - 1;
        ep_cbloss[act] += ql.cb_repeat - 1;
        noise_cbloss[ql.noise] += ql.cb_repeat - 1;
        delay_cbloss[ql.wait] += ql.cb_repeat - 1;
      }
    }else{
      if (ql.cb_flg) {
        cbuse += ql.cb_flg;
        ep_cbuse[act] += ql.cb_flg;
        noise_cbuse[ql.noise]++;
        delay_cbuse[ql.wait]++;
        cbloss += ql.cb_repeat - 1;
        ep_cbloss[act] += ql.cb_repeat - 1;
        noise_cbloss[ql.noise] += ql.cb_repeat - 1;
        delay_cbloss[ql.wait] += ql.cb_repeat - 1;
      }
    }
    //------------------------//
  }
  //======================== learning end ==========================//

  // average and final result
  cbrate = cbuse / ACTION_LIMIT;
  cbeffect = cbeffect / cbuse;
  cbloss /= ACTION_LIMIT;
  cbrate = cbuse / ACTION_LIMIT;

  mets /= ACTION_LIMIT;
  finalmet = ql.met;
  ambiguity /= ACTION_LIMIT;
}

void Agent::init_episodes(){
  for(int a = 0; a < ACTION_LIMIT; ++a){
    ep_reward[a] = 0;
    ep_met[a] = 0;
    ep_cbuse[a] = 0;
    ep_cbloss[a] = 0;
    ep_cbeffect[a] = 0;
    ep_waiting[a] = 0;
    ep_ambiguity[a] = 0;
  }

  for(int n = 0; n < NOISE_MAX; ++n){
    noise[n] = 0;
    noise_delay[n] = 0;
    noise_met[n] = 0; 
    noise_cbuse[n] = 0;
    noise_cbeffect[n] = 0;
    noise_cbloss[n] = 0;
  }

  for(int w = 0; w < WAITING_TIME+1; ++w){
    delay[w] = 0;
    delay_noise[w] = 0;
    delay_met[w] = 0;
    delay_cbuse[w] = 0;
    delay_cbeffect[w] = 0;
    delay_cbloss[w] = 0;
  }
}

//average episodes data
void Agent::ep_generation_ave(){
  for(int a = 0; a < ACTION_LIMIT; a++){
    ep_reward[a] /= AGENTS;
    ep_met[a] /= AGENTS;
    ep_cbuse[a] /= AGENTS;
    ep_cbloss[a] /= AGENTS;
    ep_cbeffect[a] /= AGENTS;
    ep_waiting[a] /= AGENTS;
    ep_ambiguity[a] /= AGENTS;
  }
}

//average episodes data
void Agent::cb_generation_ave(){
  for(int n = 0; n < NOISE_MAX; n++){
    if(noise_cbuse[n]){noise_cbeffect[n] /= noise_cbuse[n];}else{noise_cbeffect[n] = 0;}
    if(noise[n]){noise_met[n] /= noise[n];}else{noise_met[n] = 0;}
    if(noise[n]){noise_cbuse[n] /= noise[n];}else{noise_cbuse[n] = 0;}
    if(noise[n]){noise_cbloss[n] /= noise[n];}else{noise_cbloss[n] = 0;}
    if(noise[n]){noise_delay[n] /= noise[n];}else{noise_delay[n] = 0;}
  }

  for(int w = 0; w < WAITING_TIME+1; w++){
    if(delay_cbuse[w]){delay_cbeffect[w] /= delay_cbuse[w];}else{delay_cbeffect[w] = 0;}
    if(delay[w]){delay_noise[w] /= delay[w];}else{delay_noise[w] = 0;}
    if(delay[w]){delay_met[w] /= delay[w];}else{delay_met[w] = 0;}
    if(delay[w]){delay_cbuse[w] /= delay[w];}else{delay_cbuse[w] = 0;}
    if(delay[w]){delay_cbloss[w] /= delay[w];}else{delay_cbloss[w] = 0;}
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
	cout << "using CB: " << cbuse << endl;
	cout << "effect of CB: " << cbeffect << endl;
	cout << endl;
}
