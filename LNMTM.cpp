/*************************************/
/*                                   */
/*             LNMEM.cpp             */
/*          Learning simulation      */
/*      of Model-free Q-learning     */
/*        with Neural Network        */
/*                                   */
/*************************************/



/******************** Coding Style in this code *********************/
/*
  # Indent tab: 2 space
  # Naming style
  - PascalCase: class
  - snake_case: method, function, variable, file
  - camelCase: local constant
  - SNAKE_CASE: macro, global constant
*/
/********************************************************************/

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>

#include "Xor.h" //Xor shift random generater
#include "FrontMatter_LN.h"  //meta parameter macro
#include "./Bandit.hpp"  //Bandit task class
#include "./SCORE.hpp"   //Scoring class for statistics through generations
#include "./Agent.hpp"

#define CHKLN 0

//standerd class
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::ostringstream;

//========================================== Output directry =========================================//
//string myroute = "/TEST/"; //my computer in domitory
string myroute = "/mnt/data_complex/meta_cognition/";  //Ryzen PC
//string myroute = "/mnt/DataComplex/MetaCog/";  //Intel WS
//string myroute = "/Users/toyagenta/Dropbox/D02_DataComp/";  //mac

string version = "LNMTM_v4.1_SCORE";
//====================================================================================================//

namespace{
  //file stream
  string ln_dirpath;
  string task_filepath;
  string ln_filepath;

  unordered_map<string, double> ag_parameter;

  void make_directory();
  int make_file(int seed);
  void delay(int x);   //delay function
  int tarai(int x, int y, int z);  //delay subfunction
}


//#################################################### main ###################################################//
int main(){
  Agent *agent = new Agent();  //learning census
  ContextBandit *cbandit = new ContextBandit();  //task class
  Score *ln_score = new Score();
  make_directory();  //make a directory which correcpond a parameter

  //========== sample loop ===========//
  for(int seed = 0; seed < SAMPLE; seed++){
    //learning score initialization
    ofstream ln_print(ln_filepath.c_str());  //file io
    ln_score->init(ln_results_name, TOTAL_LN_RESULTS);
    ln_print << "episode," << "reward," << "met," << "cbtime," << "cbloss," << "cbeffect,";
    ln_print << "delay," << "ambiguity" << endl;

    initrand(seed); //random generator initialize

    //task initialization
    cbandit->init(); //loading parameter
    ofstream task_print(task_filepath.c_str());
    cbandit->printing(task_print);
    if (CHKLN) cbandit->disptemplete();

    if(make_file(seed)) continue;

    clock_t start = clock();  //start processing time measurement

    cout << endl << "[START - sample: " << seed << "]" << endl;

    agent->init_episodes();

    for(int agent_id = 0; agent_id < AGENTS; agent_id++){
      ln_score->init_line();
      agent->init(cbandit, phenotype);

      agent->lifetime(cbandit);

      if(CHKAG){
        agent->display_results(agent_id);
        delay(DELAY);
      }

    }

    agent->ep_generation_ave();
    for (int t = 0; t < ACTION_LIMIT; ++t){
      ln_print << t + 1;
      ln_print << "," << agent->ep_reward[t] << "," << agent->ep_met[t] << "," << agent->ep_cbtime[t];
      ln_print << "," << agent->ep_cbloss[t] << "," << agent->ep_cbeffect[t] << "," << agent->ep_waiting[t] << "," << agent->ep_ambiguity[t];
      ln_print << endl;
    }
    ln_print.close();
    
    clock_t end = clock(); cout << "1 sample duration:" << (double)(end - start) / CLOCKS_PER_SEC << "sec." << endl;
  }
  //========== end of sample loop ==========//

  delete ln_score;
  delete agent;
  delete cbandit;
  return 0;
}
//###############################################################################################################//




//========================================== no name function ============================================//
namespace{
  //making output location of files
  void make_directory(){
    ostringstream ln_dir_oss;
    if(MTM_SW && IC_SW){
      ln_dir_oss << myroute << version << "icmtm/" << directory_name;  //name of data directory
    }else if(IC_SW){
      ln_dir_oss << myroute << version << "ic/" << directory_name;  //name of data directory
    }else if(MTM_SW){
      ln_dir_oss << myroute << version << "mtm/" << directory_name;  //name of data directory
    }else if(RL_SW){
      ln_dir_oss << myroute << version << "rl/" << directory_name;  //name of data directory
    }else{
      ln_dir_oss << myroute << version << "/" << directory_name;  //name of data directory
    }
    ln_dirpath = ln_dir_oss.str();

    if(mkdir(ln_dirpath.c_str(), 0777)==0){  //string convert to char*
        cout << "success to make condition directry" << endl;
    }else{
        cout << "fail to make directry" << endl;
    }
  }

  //making result file
  int make_file(int seed){
    ostringstream ln_file_oss;
    ln_file_oss << ln_dirpath << "/smp" << seed << "_ln.csv";  //name of evolution result file
    ln_filepath = ln_file_oss.str();

    //sample skip for multithread execution
    ifstream fin(ln_filepath.c_str());
    if(fin.fail()){
      ofstream fout(ln_filepath.c_str());  //generate templete file
      fout.close();
      return 0;
    }else{
      cout << "skip " << seed << " | ";
      return 1;
    }
  }

  //delay method
  void delay(int x){for(int d = 0; d < x; ++d) tarai(13, 6, 1);}

  int tarai(int x, int y, int z){
    if(x <= y) return y;
    return tarai(tarai(x-1, y, z), tarai(y-1, z, x), tarai(z-1, x, y));
  }

}
//====================================================================================================//
