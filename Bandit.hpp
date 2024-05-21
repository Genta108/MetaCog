/**********************************/
/*                                */
/*           Bandit.h             */
/*          Armed bandit          */
/*                                */
/**********************************/

#pragma once
#include <math.h>
#include <stdlib.h>
#include <iostream>
//#include "Xor.h"  //Xor shift random generater
//#include "FrontMatter.h"

using std::cout;
using std::endl;
using std::string;

//===================================== class declaration =====================================//
class ContextBandit {
 private:
  int stimuli_templete[CHOICES][STIMULI];

  void makestimuli();  // generate stimuli image
  int imgcmp(const int*, const int*, const int);

 public:
  int stimulus_num;
  int stimulus_image[STIMULI][INFO_SIZE];

  void init();            // loading parameter and making stimulus
  void stimulus();        // input stimuli
  int judge(int action);  // judging agent choice
  double hamming();       // culcurate hamming distance
  void printing(ofstream &task_print);
  void disptemplete();
  void dispinput();
};
//==============================================================================================//

//==================================== Armed_bandit class realization ====================================//
void ContextBandit::init() {
  for (int s = 0; s < STIMULI; s++) {  // initialize S-R templete
    for (int c = 0; c < CHOICES; c++) {
      stimuli_templete[c][s] = 0;
    }
  }

  for (int s = 0; s < STIMULI; s++) {  // setting key stimuli
    int reward = abs(int_urand()) % CHOICES;
    stimuli_templete[reward][s] = REWARD;  // stim_indexに対してxを選んだときに報酬あり（各刺激に対して一つの正解）
    if(PUNISH){
      int punish = reward;
      while((punish = abs(int_urand()) % CHOICES) == reward);
      stimuli_templete[punish][s] = -PUNISH;
    }
  }

  makestimuli();
}

void ContextBandit::makestimuli() {
  int image[INFO_SIZE] = {};


  for (int i = 0; i < STIMULI; ++i) {
  remake:
    for (int j = 0; j < INFO_SIZE; ++j) image[j] = abs((int_urand()) % 2);
    for(int k = 0; k < i; ++k){
      if(imgcmp(stimulus_image[k], image, INFO_SIZE)) goto remake;
    }

    for (int j = 0; j < INFO_SIZE; ++j) {
      stimulus_image[i][j] = image[j];
    }
  }
}

double ContextBandit::hamming(){
  // calculate hamming
  double hamsum = 0;
  double d;
  for (int i = 0; i < STIMULI; ++i) {
    for (int j = 0; j < STIMULI; ++j) {
      d = 0;
      if (i != j) {
        for (int k = 0; k < INFO_SIZE; ++k) {
          d += stimulus_image[i][k] ^ stimulus_image[j][k];
        }
      }
      hamsum += d;
    }
  }
  return hamsum / ((STIMULI - 1) * (STIMULI - 1));
}

int ContextBandit::imgcmp(const int* array1, const int* array2, const int size){
  int match_flg = 1;

  for(int i = 0; i < size; ++i){
    if(array1[i] != array2[i]) match_flg = 0;
  }

  return match_flg;
}

void ContextBandit::stimulus() {
  stimulus_num = abs(int_urand()) % (STIMULI);  // select the stimulus
}

int ContextBandit::judge(int action) {
  return stimuli_templete[action][stimulus_num];  // reword value
}

void ContextBandit::printing(ofstream &task_print) {
  task_print << "stimulus index, stimulus image, correct choice, bad choice" << endl;
  for (int s = 0; s < STIMULI; s++) {
    task_print << s << ",";
    for (int i = 0; i < INFO_SIZE; ++i) task_print << stimulus_image[s][i];
    for (int c = 0; c < CHOICES; ++c) {
      if (stimuli_templete[c][s]){task_print << "," << c;}
      if (stimuli_templete[c][s] < 0){task_print << "," << c;}
      else if (PUNISH == 0){task_print << ", nan";}
    }
    task_print << endl;
  }
}

void ContextBandit::disptemplete() {
  cout << "+++stimuli template+++" << endl;
  cout << "stimulus index, stimulus image, correct choice, bad choice" << endl;
  for (int s = 0; s < STIMULI; s++) {

    cout << s << ",";
    for (int i = 0; i < INFO_SIZE; ++i) cout << stimulus_image[s][i];
    for (int c = 0; c < CHOICES; ++c) {
      if (stimuli_templete[c][s]){cout << "," << c;}
      if (PUNISH && stimuli_templete[c][s] < 0){cout << "," << c;}
      else if (PUNISH == 0){cout << ", nan";}
    }
    cout << endl;
  }
  cout << "+++++++++++++++++++++++" << endl;
}

void ContextBandit::dispinput() {
  cout << "stimulus_point: " << stimulus_num << endl;
  cout << "stimuli_image: ";
  for (int j = 0; j < INFO_SIZE; ++j) cout << stimulus_image[stimulus_num][j];
  cout << endl;
}
//======================================================================================================//
