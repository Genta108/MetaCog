#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <string>
#include <stdlib.h>

using namespace std;

//================================ default constants and switch ===================================//
//mode
int MTM_SW = 0;
int IC_SW = 0;
int RL_SW = 0;
int ICLESS = 0;

//simulation
int TOTAL_PARAMETER = 20; //Total parameter of this simulation
int SAMPLE = 200;  //Number of simulation run
int FROM = 0;  //start point of simulation run

//constant for evolution
int EVALUATION = 1;  //defaulet type of evaluation function
int AGENTS = 100;  //Number of agents
int GENERATION = 5000;  //Time of generation
double MT_RATE = 0.001;  //mutation rate

//constant for task
int CHOICES = 2;  //n-armed bandit
int STIMULI = 2; //number of signal stimulus
int WAITING_TIME = 1;  //waiting time of responce: If you set to 0, the simulation crashes.
double PUNISH = 0;  //punish for incorrect responce
double REWARD = 1; //amount of reward
double NOISE_RATE = 0.1;  //Noise on short-term memory

//constant for agent
int ACTION_LIMIT = 3000;  //Limitation time of action among agent's life
int SOFTMAX = 0;  //switch of softmax: When 0, search using the Epsilon Greedy
int WTRANDOM = 0; //switch of wating time randomize
int INFO_SIZE = 10; //Information content of the stimulus
int STM_INIT = INFO_SIZE;  //Short-term memory capacity of the initial population.
int STM_DEV = INFO_SIZE-STM_INIT;  //Evolvable range of short-term memory capacity.
double CB_COST = 0.1; //cost of cheking behavior
double IC_COST = 0.05; //cost of delay count ability
double MTM_COST = 0.1;  //cost of metamemory
double STM_COST = 0.01;  //cost of short-term memory

int GENE_LENGTH = 133+STM_DEV;  //ic(0or1, 1), mtm(0or1, 1), stm_size(1-5, 4), epsiron greedy(0.1-1.0, 9), learning rate(0.1~1.0, 9), discount rate(0.1-1.0, 9), hidden cell of NN(3-30, 27), learning rate of NN(1-10, 9)
int TOTAL_PHENOTYPE = 10;  //ic, mtm, stmsize, epsiron-g, learningrate, discountrate, hiddencell, learningrateNN
int TOTAL_LN_RESULTS = 7;
int TOTAL_EVO_RESULTS = 24;
//===================================================================================================//


//============================================ Main function =========================================//
int main(int argc, char** argv){
  //get some initial values
  char cname[1024];   //name of executable file
  int opc;
  strcpy(cname, argv[0]);

	for(opc = 1; opc < argc; ++opc){
    char *s = argv[opc];
		int lgopt = strlen(s);
    if(strcmp(s, "-rsw") == 0){
      RL_SW = atoi(argv[opc+1]); opc++;
    }else if(strcmp(s, "-isw") == 0){
      IC_SW = atoi(argv[opc+1]); opc++;
    }else if(strcmp(s, "-icl") == 0){
      ICLESS = atoi(argv[opc+1]); opc++;
    }else if(strcmp(s, "-msw") == 0){
      MTM_SW = atoi(argv[opc+1]); opc++;
    }else if(strcmp(s, "-ev") == 0){       //constant for grobal
			EVALUATION = atoi(argv[opc+1]); opc++;
		}else if(strcmp(s, "-smp") == 0){
			SAMPLE = atoi(argv[opc+1]); opc++;
		}else if(strcmp(s, "-fr") == 0){
			FROM = atoi(argv[opc+1]); opc++;
		}else if(strcmp(s, "-ag") == 0){
			AGENTS = atoi(argv[opc+1]); opc++;
		}else if(strcmp(s, "-gn") == 0){
			GENERATION = atoi(argv[opc+1]); opc++;
		}else if(strcmp(s, "-mt") == 0){
			MT_RATE = atof(argv[opc+1]); opc++;
		}else if(strcmp(s, "-ch") == 0){    //constant for task
			CHOICES = atoi(argv[opc+1]); opc++;
		}else if(strcmp(s, "-st") == 0){    //constant for task
			STIMULI = atoi(argv[opc+1]); opc++;
		}else if(strcmp(s, "-wt") == 0){    //constant for task
			WAITING_TIME = atoi(argv[opc+1]); opc++;
    }else if(strcmp(s, "-wtr") == 0){   //constant for task
			WTRANDOM = atoi(argv[opc+1]); opc++;
		}else if(strcmp(s, "-nr") == 0){    //constant for task
			NOISE_RATE = atof(argv[opc+1]); opc++;
		}else if(strcmp(s, "-rw") == 0){    //constant for task
			REWARD = atof(argv[opc+1]); opc++;
		}else if(strcmp(s, "-pn") == 0){    //constant for task
			PUNISH = atof(argv[opc+1]); opc++;
		}else if(strcmp(s, "-act") == 0){   //constant for agent
			ACTION_LIMIT = atoi(argv[opc+1]); opc++;
		}else if(strcmp(s, "-sm") == 0){    //constant for agent
			SOFTMAX = atoi(argv[opc+1]); opc++;
		}else if(strcmp(s, "-inf") == 0){   //constant for agent
			INFO_SIZE = atoi(argv[opc+1]); opc++;
		}else if(strcmp(s, "-stmi") == 0){  //constant for agent
			STM_INIT = atoi(argv[opc+1]); opc++;
		}else if(strcmp(s, "-cbc") == 0){
			CB_COST = atof(argv[opc+1]); opc++;
		}else if(strcmp(s, "-icc") == 0){
			IC_COST = atof(argv[opc+1]); opc++;
		}else if(strcmp(s, "-mtmc") == 0){
			MTM_COST = atof(argv[opc+1]); opc++;
		}else if(strcmp(s, "-stmc") == 0){
			STM_COST = atof(argv[opc+1]); opc++;
    }
  }

  //file name
  ostringstream oss_frontmatter;
  oss_frontmatter << "FrontMatter.h";
  string frontmatter_file = oss_frontmatter.str();

  ofstream fout_FM(frontmatter_file.c_str());
  fout_FM << "using namespace std;" << endl;
  fout_FM << endl;

  fout_FM << "//+++++ FrontMatter.h +++++//" << endl;
  fout_FM << "#define RL_SW " << RL_SW << endl;
  fout_FM << "#define IC_SW " << IC_SW << endl;
  fout_FM << "#define MTM_SW " << MTM_SW << endl;
  fout_FM << "#define ICLESS " << ICLESS << endl;

  fout_FM << "#define TOTAL_PARAMETER " << TOTAL_PARAMETER << endl;
  fout_FM << "#define SAMPLE " << SAMPLE << endl;
  fout_FM << "#define FROM " << FROM << endl;

  fout_FM << "//constant for evolution" << endl;
  fout_FM << "#define EVALUATION " << EVALUATION << endl;
  fout_FM << "#define AGENTS " << AGENTS << endl;
  fout_FM << "#define GENERATION " << GENERATION << endl;
  fout_FM << "#define MT_RATE " << MT_RATE << endl;
  fout_FM << endl;

  fout_FM << "//constant for task" << endl;
  fout_FM << "#define CHOICES " << CHOICES  << endl;
  fout_FM << "#define STIMULI " << STIMULI << endl;
  fout_FM << "#define INFO_SIZE " << INFO_SIZE << endl;
  fout_FM << "#define WAITING_TIME " << WAITING_TIME << endl;
  fout_FM << "#define NOISE_RATE " << NOISE_RATE << endl;
  fout_FM << "#define NOISE_MAX " << (WAITING_TIME+1)*INFO_SIZE << endl;
  fout_FM << "#define REWARD " << REWARD << endl;
  fout_FM << "#define PUNISH " << PUNISH << endl;
  fout_FM << endl;

  fout_FM << "//constant for agent" << endl;
  fout_FM << "#define ACTION_LIMIT " << ACTION_LIMIT << endl;
  fout_FM << "#define SOFTMAX " << SOFTMAX << endl;
  fout_FM << "#define WTRANDOM " << WTRANDOM << endl;
  fout_FM << "#define STM_DEV " << INFO_SIZE-STM_INIT << endl;
  fout_FM << "#define STM_INIT " << STM_INIT << endl;
  fout_FM << "#define CB_COST " << CB_COST << endl;
  fout_FM << "#define IC_COST " << IC_COST << endl;
  fout_FM << "#define MTM_COST " << MTM_COST << endl;
  fout_FM << "#define STM_COST " << STM_COST << endl;
  fout_FM << endl;
  fout_FM << "#define GENE_LENGTH " << GENE_LENGTH << endl;
  fout_FM << "#define TOTAL_PHENOTYPE " << TOTAL_PHENOTYPE << endl;
  fout_FM << "#define TOTAL_LN_RESULTS " << TOTAL_LN_RESULTS << endl;
  fout_FM << "#define TOTAL_EVO_RESULTS " << TOTAL_EVO_RESULTS << endl;
  fout_FM << endl;

  fout_FM << "//constant for Q-network" << endl;
  fout_FM << "#define INPUTCELL "<< INFO_SIZE+3 << endl;  //+Stimuli+PreAction+METAMEM+INTERVALCONUT
  fout_FM << "#define OUTPUTCELL "<< CHOICES+1 << endl;  //+checking behavior
  fout_FM << endl;

	fout_FM << "string parameter_name[" << TOTAL_PARAMETER << "] = {";
	fout_FM << "\"EVALUATION\"" << "," << "\"AGENTS\"" << "," << "\"GENERATION\"" << "," << "\"MT_RATE\"" << ",";
	fout_FM << "\"CHOICES\"" << "," << "\"STIMULI\"" << "," << "\"INFO_SIZE\""<< "," << "\"WAITING_TIME\"" << ",";
	fout_FM << "\"NOISE_RATE\"" << "," << "\"REWARD\"" << "," << "\"PUNISH\"" << ",";
	fout_FM << "\"ACTION_LIMIT\"" << "," << "\"SOFTMAX\"" << "," << "\"WTRANDOM\"" << "," << "\"STM_INIT\"" << "," << "\"CB_COST\"" << ",";
	fout_FM << "\"IC_COST\"" << "," << "\"MTM_COST\"" << "," << "\"STM_COST\"";
	fout_FM << "};" << endl;
	fout_FM << "double parameter_value[" << TOTAL_PARAMETER << "] = {";
	fout_FM << EVALUATION << "," << AGENTS << "," << GENERATION << "," << MT_RATE << ",";
	fout_FM << CHOICES << "," << STIMULI << "," << INFO_SIZE << "," << WAITING_TIME << ",";
	fout_FM << NOISE_RATE << "," << REWARD << "," << PUNISH << ",";
	fout_FM << ACTION_LIMIT << "," << SOFTMAX << ","  << WTRANDOM << "," << STM_INIT << "," << CB_COST << ",";
	fout_FM << IC_COST << "," << MTM_COST << "," << STM_COST;
	fout_FM << "};" << endl;

  fout_FM << "string phenotype_name[" << TOTAL_PHENOTYPE << "] = {";
	fout_FM << "\"ic\"" << "," << "\"mtm\"" << "," << "\"stmsize\"" << ",";
	fout_FM << "\"qleps\"" << "," << "\"qlalpha\"" << "," << "\"qlgamma\""<< ",";
	fout_FM << "\"hiddencell\"" << "," << "\"nnalpha\"" << "," ;
	fout_FM << "\"icinhiv\"" << "," << "\"mtminhiv\"";
	fout_FM << "};" << endl;

  fout_FM << "string ln_results_name[" << TOTAL_LN_RESULTS << "] = {";
	fout_FM << "\"rewards\"" << "," << "\"met\"" << ",";
	fout_FM << "\"cbtime\"" << "," << "\"cbloss\"" << "," << "\"cbeffect\"" << ",";
	fout_FM << "\"waiting\"" << "," << "\"ambiguity\"";
	fout_FM << "};" << endl;

  fout_FM << "string evo_results_name[" << TOTAL_EVO_RESULTS << "] = {";
	fout_FM << "\"AveFitness\"" << "," << "\"MaxFitness\"" << "," << "\"AveMet\"" << "," << "\"FinalMet\"" << ",";
	fout_FM << "\"RateCB\"" << "," << "\"UseCB\"" << "," << "\"EffectCB\"" << "," << "\"LossCB\"" << "," << "\"Ambiguity\"" << ",";
  fout_FM << "\"ShareIC\"" << "," << "\"ShareMTM\"" << "," << "\"ShareICMTM\"" << "," << "\"ShareBare\"" << ",";
  fout_FM << "\"STMsize\"" << ",";
	fout_FM << "\"QLeps\"" << "," << "\"QLalp\"" << "," << "\"QLgam\"" << "," << "\"hidcell\"" << "," << "\"nnalp\"" << "," << "\"icinhiv\"" << "," << "\"mtminhiv\"" << ",";
  fout_FM << "\"hamming\"" << "};" << endl;

  fout_FM << "string directory_name = \"" << "ev" << EVALUATION << "_ag" << AGENTS << "_gn" << GENERATION << "_mt" << MT_RATE;
  fout_FM << "_inf" << INFO_SIZE << "_act" << ACTION_LIMIT << "_sm" << SOFTMAX << "_st" << STIMULI << "_ch" << CHOICES;
  fout_FM << "_wt" << WAITING_TIME << "_wtr" << WTRANDOM << "_nr" << NOISE_RATE << "_stmi" << STM_INIT << "_rw" << REWARD << "_pn" << PUNISH << "_cbc" << CB_COST;
  fout_FM << "_icc" << IC_COST << "_mtmc" << MTM_COST << "_stmc" << STM_COST << "\";";  //basic file name;

  fout_FM.close();

  cout << "//+++++ Macro.h +++++//" << endl;
  cout << "//constant for evolution" << endl;
  cout << "#define EVALUATION " << EVALUATION << endl;
  cout << "#define AGENTS " << AGENTS << endl;
  cout << "#define GENERATION " << GENERATION << endl;
  cout << "#define SAMPLE " << SAMPLE << endl;
  cout << "#define FROM " << FROM << endl;
  cout << "#define MT_RATE " << MT_RATE << endl;
  cout << endl;

  cout << "//constant for task" << endl;
  cout << "#define CHOICES " << CHOICES  << endl;
  cout << "#define STIMULI " << STIMULI << endl;
  cout << "#define INFO_SIZE " << INFO_SIZE << endl;
  cout << "#define STM_INIT " << STM_INIT << endl;
  cout << "#define WAITING_TIME " << WAITING_TIME << endl;
  cout << "#define NOISE_RATE " << NOISE_RATE << endl;
  cout << "#define NOISE_MAX " << (WAITING_TIME+1)*INFO_SIZE << endl;
  cout << "#define REWARD " << REWARD << endl;
  cout << "#define PUNISH " << PUNISH << endl;
  cout << endl;

  cout << "//constant for agent" << endl;
  cout << "#define ACTION_LIMIT " << ACTION_LIMIT << endl;
  cout << "#define SOFTMAX " << SOFTMAX << endl;
  cout << "#define WTRANDOM " << WTRANDOM << endl;
  cout << "#define STM_DEV " << INFO_SIZE-STM_INIT << endl;
  cout << "#define CB_COST " << CB_COST << endl;
  cout << "#define IC_COST " << IC_COST << endl;
  cout << "#define MTM_COST " << MTM_COST << endl;
  cout << "#define STM_COST " << STM_COST << endl;
  cout << endl;

  cout << "//constant for Q-network" << endl;
  cout << "#define INPUTCELL "<< INFO_SIZE+3 << endl;  //+Stimuli+PreAction+METAMEM+INTERVALCONUT
  cout << "#define OUTPUTCELL "<< CHOICES+1 << endl;  //+information seeking
  cout << endl;

  cout << "argument example is bellow:" << endl;
  cout << "./frontmatter";
  cout << " -rsw " << RL_SW << " -isw " << IC_SW << " -msw " << MTM_SW;
	cout << " -ev " << EVALUATION << " -ag " << AGENTS << " -gen " << GENERATION << " -mt " << MT_RATE;
	cout << " -ch " << CHOICES << " -st " << STIMULI << " -inf " << INFO_SIZE << " -wt " << WAITING_TIME;
	cout << " -nr " << NOISE_RATE << " -rw " << REWARD << " -pn " << PUNISH;
	cout << " -act " << ACTION_LIMIT << " -sm " << SOFTMAX << " -wtr " << WTRANDOM << " -stmi " << STM_INIT << " -cbc " << CB_COST;
	cout << " -icc " << IC_COST << " -mtmc " << MTM_COST << " -stmc " << STM_COST << endl;
  cout << endl;
}
//====================================================================================================//
