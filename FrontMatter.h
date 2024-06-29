using namespace std;

//+++++ FrontMatter.h +++++//
#define RL_SW 1
#define IC_SW 0
#define MTM_SW 0
#define TOTAL_PARAMETER 20
#define SAMPLE 200
#define FROM 0
//constant for evolution
#define EVALUATION 2
#define AGENTS 100
#define GENERATION 5000
#define MT_RATE 0.001

//constant for task
#define CHOICES 4
#define STIMULI 4
#define INFO_SIZE 5
#define WAITING_TIME 5
#define NOISE_RATE 0.1
#define NOISE_MAX 100
#define REWARD 2
#define PUNISH 1

//constant for agent
#define ACTION_LIMIT 6000
#define SOFTMAX 0
#define WTRANDOM 0
#define STM_DEV 0
#define STM_INIT 5
#define CB_COST 0.1
#define EXPOSURE 0
#define IC_COST 0.05
#define MTM_COST 0.1
#define STM_COST 0.01

#define GENE_LENGTH 116
#define TOTAL_PHENOTYPE 10
#define TOTAL_LN_RESULTS 7
#define TOTAL_EVO_RESULTS 22

//constant for Q-network
#define INPUTCELL 8
#define OUTPUTCELL 5

string parameter_name[20] = {"EVALUATION","AGENTS","GENERATION","MT_RATE","CHOICES","STIMULI","INFO_SIZE","WAITING_TIME","NOISE_RATE","REWARD","PUNISH","ACTION_LIMIT","SOFTMAX","WTRANDOM","STM_INIT","CB_COST","EXPOSURE","IC_COST","MTM_COST","STM_COST"};
double parameter_value[20] = {2,100,5000,0.001,4,4,5,5,0.1,2,1,6000,0,0,5,0.1,0,0.05,0.1,0.01};
string phenotype_name[10] = {"ic","mtm","stmsize","qleps","qlalpha","qlgamma","hiddencell","nnalpha","somsize","somalpha"};
string ln_results_name[7] = {"rewards","met","cbtime","cbloss","cbeffect","waiting","ambiguity"};
string evo_results_name[22] = {"AveFitness","MaxFitness","AveMet","FinalMet","RateCB","UseCB","EffectCB","LossCB","Ambiguity","ShareIC","ShareMTM","ShareICMTM","ShareBare","STMsize","QLeps","QLalp","QLgam","hidcell","nnalp","somsize","somalp","hamming"};
string directory_name = "ev2_ag100_gn5000_mt0.001_inf5_act6000_sm0_st4_ch4_wt5_nr0.1_stmi5_rw2_pn1_cbc0.1_exp0_icc0.05_mtmc0.1_stmc0.01";