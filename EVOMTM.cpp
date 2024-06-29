/*************************************/
/*                                   */
/*            EVOMEM.cpp             */
/*      Evolutionary simulation      */
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
#include "FrontMatter.h"  //meta parameter macro
#include "./GeneticAlgorithm.hpp"

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

string version = "EVOMTM_v4.5_SCORE";
//====================================================================================================//

namespace{
  //file stream
  string evo_dirpath;
  string ln_dirpath;
  string task_filepath;
  string evo_filepath;
  string gene_filepath;

  void make_directory();
  int make_file(int seed);
}


//#################################################### main ###################################################//
int main(){
  GeneticAlgorithm mtmevo;

  make_directory();  //make a directory which correcpond a parameter

  //========== sample loop ===========//
  for(int seed = FROM; seed < SAMPLE; seed++){

    initrand(seed); //random generator initialize

    if(make_file(seed)) continue;

    clock_t start = clock();  //start processing time measurement

    cout << endl << "[START - sample: " << seed << "]" << endl;

    mtmevo.evolution(task_filepath, evo_filepath, gene_filepath, ln_dirpath);  //evolution process

    clock_t end = clock(); cout << "1 sample duration:" << (double)(end - start) / CLOCKS_PER_SEC << "sec." << endl;
  }
  //========== end of sample loop ==========//

  return 0;
}
//###############################################################################################################//




//========================================== no name function ============================================//
namespace{
  //making output location of files
  void make_directory(){
    ostringstream evo_dir_oss;
    if(MTM_SW && IC_SW){
      evo_dir_oss << myroute << version << "icmtm/" << directory_name;  //name of data directory
    }else if(IC_SW){
      evo_dir_oss << myroute << version << "ic/" << directory_name;  //name of data directory
    }else if(MTM_SW){
      evo_dir_oss << myroute << version << "mtm/" << directory_name;  //name of data directory
    }else if(RL_SW){
      evo_dir_oss << myroute << version << "rl/" << directory_name;  //name of data directory
    }else{
      evo_dir_oss << myroute << version << "/" << directory_name;  //name of data directory
    }
    evo_dirpath = evo_dir_oss.str();

    if(mkdir(evo_dirpath.c_str(), 0777)==0){  //string convert to char*
        cout << "success to make condition directry" << endl;
    }else{
        cout << "fail to make directry" << endl;
    }
  }

  //
  int make_file(int seed){
    ostringstream evo_file_oss;
    evo_file_oss << evo_dirpath << "/smp" << seed << "_evo.csv";  //name of evolution result file
    evo_filepath = evo_file_oss.str();

    ostringstream gene_file_oss;
    gene_file_oss << evo_dirpath << "/smp" << seed << "_gene.csv";  //name of gene file
    gene_filepath = gene_file_oss.str();

    ostringstream task_file_oss;
    task_file_oss << evo_dirpath << "/smp" << seed << "_task.csv";  //name of gene file
    task_filepath = task_file_oss.str();

    ostringstream ln_dir_oss;
    ln_dir_oss << evo_dirpath << "/smp" << seed;  //name of q-learning result file
    ln_dirpath = ln_dir_oss.str();


    if(mkdir(ln_dirpath.c_str(), 0777)==0){
      cout << "success to make sample directry" << endl;
    }
    //sample skip for multithread execution
    ifstream fin(evo_filepath.c_str());
    if(fin.fail()){
      ofstream fout(evo_filepath.c_str());  //generate templete file
      fout.close();
      return 0;
    }else{
      cout << "skip " << seed << " | ";
      return 1;
    }
  }

}
//====================================================================================================//
