#pragma once
#include <math.h>
#include <limits.h>  //for ULLONG_MAX
#include <stdlib.h>
#include <string>
#include <unordered_map>

//#include "FrontMatter.h"  //Numarical declaration
//#include "Xor.h"  //Xor shift random generater
#include "./Bandit.hpp"  //Bandit task class
#include "./MLP.hpp"

//for Maintenance switch
#define CHKRL 0
#define CHKMLP 0
#define DELAYTIME 0

using std::string;
using std::unordered_map;

namespace {
  int binary[20];  // stimulus binary

  void delay(int x);   //delay function
  int factorial(int k);
  inline int recpow(int x, int n);  // power method(hyspeed)
  inline int __recpow(int x, int r, int n);
  void dec2bin(int decimal);
}  // namespace

//====================== RL class declaration ===================//
class Reinforcement {
 private:
  // agent's parameter
  double alpha, gamma, epsiron;  // learning rate, discount rate, exploration rate
  double ic, mtm, stmsize;

  // agent's state & context
  int selected_action;
  double memory_state[INFO_SIZE];    // short-term memory
  double ambiguity, delay_count;
  int greedy_flg;


  double dataset[INPUTCELL + OUTPUTCELL];
  double next_dataset[INPUTCELL + OUTPUTCELL];

  void init_state();
  void interval(SOM &som);
  int greedy();
  int qsoftmax(double data[], NN &mlp);
  int qmax(double data[], NN &mlp);
  int choice_qmax(double data[], NN &mlp);
  double update_qnet(int action, double dataset[], NN &mlp);  // updating q-value
  void display_input();
  void display_noisedstm();

 public:
  double reinforcer;
  int met;
  int punish;
  int noise;

  // agent action score
  int cb_flg;
  double cb_repeat;
  double sum_ambiguity;
  double sum_delay;

  void qlearning(ContextBandit *cbandit, NN &mlp, SOM &som);
  void set_parameter(unordered_map<string, double> hyper_parameter);
};
//==========================================================================//

//====================================== RL class realization =====================================//
void Reinforcement::qlearning(ContextBandit *cbandit, NN &mlp, SOM &som) {
  if(CHKRL) delay(DELAYTIME);
  init_state();  // all state initialize

  // vvv// Input&Waiting phase //vvv//
  cbandit->stimulus();  // input sample stimulus
  for (int i = 0; i < stmsize; ++i) memory_state[i] = cbandit->stimulus_image[cbandit->stimulus_num][i];
  if (CHKRL){display_input(); delay(DELAYTIME);}
  som.exposure(memory_state);

  // Waiting phase and bit invert noise
  if(WAITING_TIME) interval(som);
  //^^^// Input&Waiting phase phase //^^^//
  // vvv// Action selection phase //vvv//
  double q_or_greedy = real_urand();

  //***greedy search or decision making by q-value***//
  if (q_or_greedy < epsiron) {  // esilon greedy decision

    greedy_flg = 1;
    for (int i = 0; i < stmsize; ++i) dataset[i] = memory_state[i];  // current stm state
    if (cb_flg) dataset[INFO_SIZE] = 0;
    if (ic) dataset[INFO_SIZE+1] = delay_count;
    if (mtm) dataset[INFO_SIZE+2] = ambiguity;

    if (CHKRL){cout << "choice greedy" << endl; delay(DELAYTIME);}
    selected_action = greedy();

  } else {  // action set by q-value
  retake_point:
    greedy_flg = 0;
    for (int i = 0; i < INPUTCELL + OUTPUTCELL; ++i)  dataset[i] = 0;  // initialize

    // input NN dataset
    for (int i = 0; i < stmsize; ++i) dataset[i] = memory_state[i];  
    if (cb_flg) dataset[INFO_SIZE] = 1;
    if (ic) dataset[INFO_SIZE+1] = delay_count;
    if (mtm) dataset[INFO_SIZE+2] = ambiguity;

    if (CHKRL){cout << "choice action by q-value" << endl; delay(DELAYTIME);}
    if (SOFTMAX) {
      selected_action = qsoftmax(dataset, mlp);
    } else {
      selected_action = qmax(dataset, mlp);
    }
  }
  //^^^// Action selection phase //^^^//

  // v//state transition phase//v//
  if ((selected_action == 0) && (cb_repeat < 10)) {  // choosing checking behavior
    if (CHKRL){cout << "--- for now CB ---" << endl; delay(DELAYTIME);}

    cb_flg = 1; cb_repeat++; delay_count = 0; ambiguity = 0;
    for (int i = 0; i < stmsize; ++i) memory_state[i] = cbandit->stimulus_image[cbandit->stimulus_num][i];
    if (CHKRL){display_input(); delay(DELAYTIME);}
    som.exposure(memory_state);
    for(int i = 0; i < stmsize; ++i) memory_state[i] = som.somout[i];

    // input next dataset
    for (int i = 0; i < INPUTCELL + OUTPUTCELL; ++i) next_dataset[i] = 0;  // 0 reset
    for (int i = 0; i < stmsize; ++i) next_dataset[i] = memory_state[i];  // next stm state
    if (cb_flg) next_dataset[INFO_SIZE] = 1; 
    if (ic) next_dataset[INFO_SIZE+1] = delay_count;
    if (mtm) next_dataset[INFO_SIZE+2] = ambiguity;

    mlp.output[0] = mlp.forward(0, dataset);

    dataset[INPUTCELL + 0] = update_qnet(0, dataset, mlp);  // update Q-value of CB
    mlp.olearn(0, dataset);
    mlp.hlearn(0, dataset);

    goto retake_point;  // goto command is only this one

  } else if ((selected_action < CHOICES + 1) && (selected_action > 0)) {

    if (CHKRL){cout << "Select " << selected_action-1 << "!!" << endl; delay(DELAYTIME);}
    if (CHKRL&&(selected_action == 0)){cout << "Select CB!!" << endl; delay(DELAYTIME);}
    if (CHKRL){cout << "Result: " << cbandit->judge(selected_action - 1) << endl; delay(DELAYTIME);}

    //---------------- reward function -------------------//
    if(cbandit->judge(selected_action - 1) > 0){
      met = 1;
      reinforcer = cbandit->judge(selected_action - 1) / (1 + CB_COST * cb_repeat);  //-checking behavior
    }else if(cbandit->judge(selected_action - 1) < 0){
      punish = 1;
      reinforcer = cbandit->judge(selected_action - 1);
    }

    //----------------------------------------------------//

    if (met) {
      if (CHKRL){
        if (cb_flg == 0){cout << "Oh!!!! Success!!!!!!!! Reward: " << reinforcer << endl; delay(DELAYTIME);}
        else if(cb_flg){cout << "WOW!!!!! My god!!!!!!!! Success using CB!!!!!!!!!!! Nice!! Nice!!! Niceeeee!!!!!!! Reward: " << reinforcer << endl; delay(DELAYTIME);}
        }
    } else if (punish) {
      if (CHKRL){cout << "Punish!! Reward: " << reinforcer << endl; delay(DELAYTIME);}
    }else {
      if (CHKRL){cout << "Failed..." << endl; delay(DELAYTIME);}
    }

  } else if (cb_repeat < 10) {
    cout << "**ACTION ERROR**" << endl;
    cout << "Selected action: " << selected_action << endl;
    return;
  }
  //^//state transition phase//^//

  // v//update Q-network phase//v//
  if (selected_action && (cb_repeat < 10)) {
    if (CHKRL){cout << "--- for now action ---" << endl; delay(DELAYTIME);}

    mlp.output[selected_action] = mlp.forward(selected_action, dataset); //calc. currect dataset qv

    dataset[INPUTCELL + selected_action] = update_qnet(selected_action, dataset, mlp);  // update Q-value
    mlp.olearn(selected_action, dataset);
    mlp.hlearn(selected_action, dataset);

    if (CHKRL) { cout << "**QL POST CHECK**" << endl; qmax(dataset, mlp); delay(DELAYTIME);}
  } else {
    if (CHKRL) cout << "IS limit!!" << endl;
  }
  //^//update Q-network phase//^//

  if (CHKMLP){
    mlp.printweight();
    //delay(100);
  }
}

void Reinforcement::set_parameter(unordered_map<string, double> hyper_parameter) {
  alpha = hyper_parameter["qlalpha"];
  epsiron = hyper_parameter["qleps"];
  gamma = hyper_parameter["qlgamma"];

  ic = hyper_parameter["ic"];
  mtm = hyper_parameter["mtm"];
  stmsize = hyper_parameter["stmsize"];
}


void Reinforcement::interval(SOM &som){
  if(WTRANDOM){delay_count = abs(int_urand()) % WAITING_TIME + 1;}else{delay_count = WAITING_TIME;}
  sum_delay = delay_count;
  noise = 0;
  for (int w = 0; w < delay_count; ++w) {
    for (int i = 0; i < stmsize; ++i) {
      if (real_urand() < NOISE_RATE) {
        noise++;
        if (memory_state[i] == 0) {
          memory_state[i] = 1;
        } else if (memory_state[i] == 1) {
          memory_state[i] = 0;
        }
      }
    }
    ambiguity += som.maintain(memory_state);
    for (int i = 0; i < stmsize; ++i) {
      memory_state[i] = som.somout[i];
    }
  }
  sum_ambiguity = ambiguity;

  if (CHKRL){display_noisedstm();delay(DELAYTIME);}
}


int Reinforcement::greedy() {
  double r;
  while ((r = real_urand()) == 1.0){};
  return (int)(r * (CHOICES + 1));
}

int Reinforcement::qsoftmax(double data[], NN &mlp) {
  double e[INPUTCELL+1] = {};
  double action_q[CHOICES + 1];
  double sum, roulette_index;
  int action_index;
  double ps_a[CHOICES + 1];
  for(int i = 0; i < INPUTCELL; i++) e[i] = data[i];
  
  // comparing q-value
  sum = 0;
  for (int i = 0; i < CHOICES + 1; ++i) {  // i = 0: information seeking
    action_q[i] = 0;
    action_q[i] = mlp.forward(i, e);
    if (CHKRL) cout << "actionq[" << i << "]: " << action_q[i] << endl;
    sum += exp(action_q[i]);
  }
  for (int i = 0; i < CHOICES + 1; ++i) {
    ps_a[i] = exp(action_q[i]) / sum;
  }
  while ((roulette_index = real_urand()) == 1.0);

  sum = ps_a[0], action_index = 0;
  while (roulette_index > sum) {
    ++action_index;
    sum += ps_a[action_index];
  }
  return action_index;
}

int Reinforcement::qmax(double data[], NN &mlp) {
  double e[INPUTCELL+1] = {};
  double action_q = 0;
  double action_maxq = 0;
  int action_index = 0;

  if (CHKRL) cout << "dataset: ";
  for(int i = 0; i < INPUTCELL; i++){
    if (CHKRL) cout << data[i];
    e[i] = data[i];
  }
  if (CHKRL) cout << endl;

  // comparing q-value
  for (int i = 0; i < CHOICES + 1; ++i) {
    action_q = mlp.forward(i, e);
    if (CHKRL) cout << "actionq[" << i << "]: " << action_q << endl;

    if (action_maxq < action_q) {
      action_maxq = action_q;
      action_index = i;
    }
  }

  return action_index;
}


double Reinforcement::update_qnet(int action, double data[], NN &mlp) {
  double qv = 0;                     // updated q-value
  double qv_sa;                      // current q-value ath the state
  double qv_nextsa;                  // max q-value at next state
  double e[INPUTCELL] = {};  // dataset of input, 0 fill

  qv_sa = mlp.output[action];               // q_value of action

  if (CHKRL) cout << "action: " << action << ", qv: " << qv_sa << endl;

  // update q-value
  if (met) {
    qv = qv_sa + alpha * (reinforcer - qv_sa);
    if (CHKRL) cout << "q-value update by reward: " << qv << endl;
  } else if (action == 0) {  // checking behavior action
    // estimate max q-value at next_state
    qv_nextsa = mlp.forward(qmax(next_dataset, mlp), next_dataset); //best action of next state
    if (CHKRL) cout << "CB, qv_next: " << qv_nextsa << endl;
    qv = qv_sa + alpha * (gamma * qv_nextsa - qv_sa);
    if (CHKRL) cout << "q-value update by qv: " << qv << endl;
  } else if (punish){
    qv = reinforcer;
  }
  return qv;
}

void Reinforcement::init_state() {
  //agent state
  for (int i = 0; i < INPUTCELL + OUTPUTCELL; ++i) dataset[i] = 0;  // initialize
  for (int i = 0; i < INFO_SIZE; ++i)  memory_state[i] = 0;

  //agent score
  cb_flg = 0;
  cb_repeat = 0;
  reinforcer = 0;
  met = 0;
  punish = 0;
  delay_count = 0;
  noise = 0;
  ambiguity = 0;
  sum_ambiguity = 0;
  sum_delay = 0;
}

void Reinforcement::display_input() {
  cout << "recognize: ";
  for (int i = 0; i < stmsize; ++i) cout << memory_state[i];
  cout << endl;
}

void Reinforcement::display_noisedstm() {
  cout << "noised stm: ";
  for (int i = 0; i < stmsize; ++i) cout << memory_state[i];
  cout << endl;
  cout << "ambiguity: " << ambiguity << endl;
}
//======================================================================================================//

namespace {
  // factorial method
  int factorial(int k) {
    int sum = 1;
    for (int i = 1; i <= k; ++i) sum *= i;
    return sum;
  }

  // power method(hyspeed)
  inline int recpow(int x, int n) { return __recpow(x, 1, n); }
  inline int __recpow(int x, int r, int n) {
    if (n == 0) {
      return r;
    } else if ((n % 2) == 0) {
      return __recpow(x * x, r, n >> 1);
    } else {
      return __recpow(x, r * x, n - 1);
    }
  }

  // combination method
  int combine(int x) {
    int sum = 0;
    for (int i = 1; i <= x; ++i) sum += recpow(2, i);
    return sum;
  }

  void dec2bin(int decimal) {
    for (int i = 0; i < INFO_SIZE; i++) binary[i] = 0;
    for (int i = 0; decimal > 0; ++i) {
      binary[i] = decimal % 2;
      decimal = decimal / 2;
    }
  }

  int bin2dec(int input_binary[]) {
    int decimal = 0;
    for (int i = 0; i < 20; ++i) {
      decimal += input_binary[i] * pow(2, i);
    }
    return decimal;
  }
}  // namespace
