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

//simulation
int TOTAL_PARAMETER = 20; //Total parameter of this simulation
int SAMPLE = 1;
int AGENTS = 100;  //Number of agents

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
double CB_COST = 0.1; //cost of cheking behavior
int EXPOSURE = 0; //time of pre-exposure

int TOTAL_PHENOTYPE = 10;  //ic, mtm, stmsize, epsiron-g, learningrate, discountrate, hiddencell, learningrateNN, somsize, learningrateSOM
int STM = INFO_SIZE;
double QLeps = 0.1;
double QLalp = 0.5;
double QLgam = 0.5;
int hidcell = 5;
int nnalp = 1;
int somsize = 10;
int somalp = 1;

int TOTAL_LN_RESULTS = 7;
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
    }else if(strcmp(s, "-msw") == 0){
      MTM_SW = atoi(argv[opc+1]); opc++;
    }else if(strcmp(s, "-smp") == 0){
			SAMPLE = atoi(argv[opc+1]); opc++;
		}else if(strcmp(s, "-ag") == 0){
			AGENTS = atoi(argv[opc+1]); opc++;
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
		}else if(strcmp(s, "-exp") == 0){   //constant for agent
			EXPOSURE = atoi(argv[opc+1]); opc++;
		}else if(strcmp(s, "-cbc") == 0){
			CB_COST = atof(argv[opc+1]); opc++;
		}else if(strcmp(s, "-qle") == 0){
			QLeps = atof(argv[opc+1]); opc++;
		}else if(strcmp(s, "-qla") == 0){
			QLalp = atof(argv[opc+1]); opc++;
		}else if(strcmp(s, "-qlg") == 0){
			QLgam = atof(argv[opc+1]); opc++;
		}else if(strcmp(s, "-hid") == 0){   //constant for agent
			hidcell = atoi(argv[opc+1]); opc++;
		}else if(strcmp(s, "-nna") == 0){   //constant for agent
			nnalp = atoi(argv[opc+1]); opc++;
		}else if(strcmp(s, "-soms") == 0){   //constant for agent
			somsize = atoi(argv[opc+1]); opc++;
		}else if(strcmp(s, "-soma") == 0){   //constant for agent
			somalp = atoi(argv[opc+1]); opc++;
		}
  }

  //file name
  ostringstream oss_frontmatter;
  oss_frontmatter << "FrontMatter_LN.h";
  string frontmatter_file = oss_frontmatter.str();

  ofstream fout_FM(frontmatter_file.c_str());
  fout_FM << "using namespace std;" << endl;
  fout_FM << endl;

  fout_FM << "//+++++ FrontMatter.h +++++//" << endl;
  fout_FM << "#define RL_SW " << RL_SW << endl;
  fout_FM << "#define IC_SW " << IC_SW << endl;
  fout_FM << "#define MTM_SW " << MTM_SW << endl;

  fout_FM << "#define TOTAL_PARAMETER " << TOTAL_PARAMETER << endl;
  fout_FM << "#define SAMPLE " << SAMPLE << endl;
  fout_FM << "#define AGENTS " << AGENTS << endl;
  fout_FM << endl;

  fout_FM << "//constant for task" << endl;
  fout_FM << "#define CHOICES " << CHOICES  << endl;
  fout_FM << "#define STIMULI " << STIMULI << endl;
  fout_FM << "#define INFO_SIZE " << INFO_SIZE << endl;
  fout_FM << "#define WAITING_TIME " << WAITING_TIME << endl;
  fout_FM << "#define NOISE_RATE " << NOISE_RATE << endl;
  fout_FM << "#define REWARD " << REWARD << endl;
  fout_FM << "#define PUNISH " << PUNISH << endl;
  fout_FM << endl;

  fout_FM << "//constant for agent" << endl;
  fout_FM << "#define ACTION_LIMIT " << ACTION_LIMIT << endl;
  fout_FM << "#define SOFTMAX " << SOFTMAX << endl;
  fout_FM << "#define WTRANDOM " << WTRANDOM << endl;
  fout_FM << "#define CB_COST " << CB_COST << endl;
  fout_FM << "#define EXPOSURE " << EXPOSURE << endl;
  fout_FM << endl;
  fout_FM << "#define TOTAL_PHENOTYPE " << TOTAL_PHENOTYPE << endl;
  fout_FM << "#define QLeps " << QLeps << endl;
  fout_FM << "#define QLalp " << QLalp << endl;
  fout_FM << "#define QLgam " << QLgam << endl;
  fout_FM << "#define hidcell " << hidcell << endl;
  fout_FM << "#define nnalp " << nnalp << endl;
  fout_FM << "#define somsize " << somsize << endl;
  fout_FM << "#define somalp " << somalp << endl;
  fout_FM << "#define TOTAL_LN_RESULTS " << TOTAL_LN_RESULTS << endl;
  fout_FM << endl;

  fout_FM << "//constant for Q-network" << endl;
  fout_FM << "#define INPUTCELL "<< INFO_SIZE+2 << endl;  //+Stimuli+METAMEM+INTERVALCONUT
  fout_FM << "#define OUTPUTCELL "<< CHOICES+1 << endl;  //+checking behavior
  fout_FM << endl;

	fout_FM << "string parameter_name[" << TOTAL_PARAMETER << "] = {";
	fout_FM << "\"EVALUATION\"" << "," << "\"AGENTS\"" << "," << "\"GENERATION\"" << "," << "\"MT_RATE\"" << ",";
	fout_FM << "\"CHOICES\"" << "," << "\"STIMULI\"" << "," << "\"INFO_SIZE\""<< "," << "\"WAITING_TIME\"" << ",";
	fout_FM << "\"NOISE_RATE\"" << "," << "\"REWARD\"" << "," << "\"PUNISH\"" << ",";
	fout_FM << "\"ACTION_LIMIT\"" << "," << "\"SOFTMAX\"" << "," << "\"WTRANDOM\"" << "," << "\"CB_COST\"" << "," << "\"EXPOSURE\"" << ",";
	fout_FM << "\"QLeps\"" << "," << "\"QLalpha\"" << "," << "\"QLgamma\"" << "," << "\"hiddencell\"" << "," << "\"NNalpha\"" << "," << "\"SOMsize\"" << "," << "\"SOMalpha\"";
	fout_FM << "};" << endl;
	fout_FM << "double parameter_value[" << TOTAL_PARAMETER << "] = {";
	fout_FM << AGENTS << "," << CHOICES << "," << STIMULI << "," << INFO_SIZE << "," << WAITING_TIME << ",";
	fout_FM << NOISE_RATE << "," << REWARD << "," << PUNISH << ",";
	fout_FM << ACTION_LIMIT << "," << SOFTMAX << ","  << WTRANDOM << "," << CB_COST << "," << EXPOSURE << ",";
	fout_FM << QLeps << "," << QLalp << "," << QLgam << "," << hidcell << "," << nnalp << "," << somsize << "," << somalp;
	fout_FM << "};" << endl;

  fout_FM << "string phenotype_name[" << TOTAL_PHENOTYPE << "] = {";
	fout_FM << "\"ic\"" << "," << "\"mtm\"" << "," << "\"stmsize\"" << ",";
	fout_FM << "\"qleps\"" << "," << "\"qlalpha\"" << "," << "\"qlgamma\""<< ",";
	fout_FM << "\"hiddencell\"" << "," << "\"nnalpha\"" << "," << "\"somsize\"" << "," << "\"somalpha\"";
	fout_FM << "};" << endl;

  fout_FM << "string ln_results_name[" << TOTAL_LN_RESULTS << "] = {";
	fout_FM << "\"rewards\"" << "," << "\"met\"" << ",";
	fout_FM << "\"cbtime\"" << "," << "\"cbloss\"" << "," << "\"cbeffect\"" << ",";
	fout_FM << "\"waiting\"" << "," << "\"ambiguity\"";
	fout_FM << "};" << endl;


  fout_FM << "string directory_name = \""<< "ag" << AGENTS;
  fout_FM << "_inf" << INFO_SIZE << "_act" << ACTION_LIMIT << "_sm" << SOFTMAX << "_st" << STIMULI << "_ch" << CHOICES;
  fout_FM << "_wt" << WAITING_TIME << "_nr" << NOISE_RATE << "_rw" << REWARD << "_pn" << PUNISH << "_cbc" << CB_COST << "_exp" << EXPOSURE;
  fout_FM << "_qle" << QLeps << "_qla" << QLalp << "_qlg" << QLgam << "_hid" << hidcell << "_nna" << nnalp << "_soms" << somsize << "_soma" << somalp << "\";";  //basic file name;

  fout_FM.close();

  cout << "//+++++ Macro.h +++++//" << endl;
  cout << "//constant for evolution" << endl;
  cout << "#define SAMPLE " << SAMPLE << endl;
  cout << "#define AGENTS " << AGENTS << endl;
  cout << endl;

  cout << "//constant for task" << endl;
  cout << "#define CHOICES " << CHOICES  << endl;
  cout << "#define STIMULI " << STIMULI << endl;
  cout << "#define INFO_SIZE " << INFO_SIZE << endl;
  cout << "#define WAITING_TIME " << WAITING_TIME << endl;
  cout << "#define NOISE_RATE " << NOISE_RATE << endl;
  cout << "#define REWARD " << REWARD << endl;
  cout << "#define PUNISH " << PUNISH << endl;
  cout << endl;

  cout << "//constant for agent" << endl;
  cout << "#define ACTION_LIMIT " << ACTION_LIMIT << endl;
  cout << "#define SOFTMAX " << SOFTMAX << endl;
  cout << "#define WTRANDOM " << WTRANDOM << endl;
  cout << "#define CB_COST " << CB_COST << endl;
  cout << "#define EXPOSURE " << EXPOSURE << endl;
  cout << "#define QLeps " << QLeps << endl;
  cout << "#define QLalp " << QLalp << endl;
  cout << "#define QLgam " << QLgam << endl;
  cout << endl;

  cout << "//constant for Q-network" << endl;
  cout << "#define INPUTCELL "<< INFO_SIZE+2 << endl;  //+Stimuli+METAMEM+INTERVALCONUT
  cout << "#define OUTPUTCELL "<< CHOICES+1 << endl;  //+information seeking
  cout << endl;

  cout << "argument example is bellow:" << endl;
  cout << "./frontmatter";
  cout << " -rsw " << RL_SW << " -isw " << IC_SW << " -msw " << MTM_SW;
	cout << " -ag " << AGENTS << " -ch " << CHOICES << " -st " << STIMULI << " -inf " << INFO_SIZE << " -wt " << WAITING_TIME;
	cout << " -nr " << NOISE_RATE << " -rw " << REWARD << " -pn " << PUNISH;
	cout << " -act " << ACTION_LIMIT << " -sm " << SOFTMAX << " -wtr " << WTRANDOM << " -cbc " << CB_COST << " -exp " << EXPOSURE;
	cout << " -qle " << QLeps << " -qla " << QLalp << " -qlg " << QLgam << " -hid " << hidcell << " -nna " << nnalp << " -soms " << somsize << " -soma " << somalp << endl;
  cout << endl;
}
//====================================================================================================//
