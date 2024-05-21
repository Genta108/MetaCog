/**********************************/
/*                                */
/*            SCORE.h             */
/*         scoring object         */
/*                                */
/**********************************/
#pragma once
#include <stdlib.h>
#include <iostream>
//#include "FrontMatter.h"

#define CACHE 50

using std::cout;
using std::endl;
using std::ofstream;
using std::string;

//===================================== class declaration =====================================//
class Score {
 private:
  double v_ave[CACHE];   // score average
  string v_name[CACHE];  // score name
  int v_num;             // number of score

 public:
  void init(string name_container[], int imax);
  void init_line();
  void heading(ofstream &score_print);
  void counting(double v_container[]);
  void average(double ag_num);
  void print_line(ofstream &score_print, int generation);
  void insert_line(int column, double value);
  void display_line();
};
//==============================================================================================//



//====================================== Scoring class realization =====================================//
void Score::init(string name_container[], int imax){
  v_num = imax;
  for (int i = 0; i < v_num; ++i) v_name[i] = name_container[i];
}

void Score::heading(ofstream &score_print) {
  // print parameter
  score_print << "#" << parameter_name[0] << ":" << parameter_value[0];
  for (int i = 1; i < TOTAL_PARAMETER; ++i) score_print << "," << parameter_name[i] << ":" << parameter_value[i];
  score_print << endl;
  score_print << "#time, " << v_name[0];
  for (int i = 1; i < v_num; ++i) score_print << "," << v_name[i];
  score_print << endl;
}

void Score::init_line() {
  for (int i = 0; i < v_num; ++i) v_ave[i] = 0;
}

void Score::counting(double v_container[]) {
  for (int i = 0; i < v_num; ++i) v_ave[i] += v_container[i];
}

void Score::average(double ag_num) {
  if (ag_num > 0) {
    for (int i = 0; i < v_num; ++i) v_ave[i] /= ag_num;
  } else {
    for (int i = 0; i < v_num; ++i) v_ave[i] = 0;
  }
}

void Score::print_line(ofstream &score_print, int time) {
  score_print << time + 1;
  for (int i = 0; i < v_num; ++i) score_print << "," << v_ave[i];
  score_print << endl;
}

void Score::insert_line(int column, double value){
  v_ave[column-1] = value;
}

void Score::display_line() {
  cout << "#########################" << endl;
  for (int i = 0; i < v_num; ++i) {
    cout << "# " << v_name[i] << ": " << v_ave[i] << endl;
  }
  cout << "#########################" << endl;
  cout << endl;
  cout << endl;
}
//======================================================================================================//
