using namespace std;

//+++++ FrontMatter.h +++++//
#define RL_SW 0
#define IC_SW 0
#define MTM_SW 0
#define ICLESS 0
#define TOTAL_PARAMETER 20
#define SAMPLE 200
#define FROM 0
//constant for evolution
#define EVALUATION 1
#define AGENTS 100
#define GENERATION 5000
#define MT_RATE 0.001

//constant for task
#define CHOICES 2
#define STIMULI 2
#define INFO_SIZE 10
#define WAITING_TIME 1
#define NOISE_RATE 0.1
#define NOISE_MAX 20
#define REWARD 1
#define PUNISH 0

//constant for agent
#define ACTION_LIMIT 3000
#define SOFTMAX 0
#define WTRANDOM 0
#define STM_DEV 0
#define STM_INIT 10
#define CB_COST 0.1
#define IC_COST 0.05
#define MTM_COST 0.1
#define STM_COST 0.01

#define GENE_LENGTH 133
#define TOTAL_PHENOTYPE 10
#define TOTAL_LN_RESULTS 7
#define TOTAL_EVO_RESULTS 24

//constant for Q-network
#define INPUTCELL 13
#define OUTPUTCELL 3

string parameter_name[20] = {"EVALUATION","AGENTS","GENERATION","MT_RATE","CHOICES","STIMULI","INFO_SIZE","WAITING_TIME","NOISE_RATE","REWARD","PUNISH","ACTION_LIMIT","SOFTMAX","WTRANDOM","STM_INIT","CB_COST","IC_COST","MTM_COST","STM_COST"};
double parameter_value[20] = {1,100,5000,0.001,2,2,10,1,0.1,1,0,3000,0,0,10,0.1,0.05,0.1,0.01};
string phenotype_name[10] = {"ic","mtm","stmsize","qleps","qlalpha","qlgamma","hiddencell","nnalpha","icinhiv","mtminhiv"};
string ln_results_name[7] = {"rewards","met","cbtime","cbloss","cbeffect","waiting","ambiguity"};
string evo_results_name[24] = {"AveFitness","MaxFitness","AveMet","FinalMet","RateCB","UseCB","EffectCB","LossCB","Ambiguity","ShareIC","ShareMTM","ShareICMTM","ShareBare","STMsize","QLeps","QLalp","QLgam","hidcell","nnalp","icinhiv","mtminhiv","hamming"};
string directory_name = "ev1_ag100_gn5000_mt0.001_inf10_act3000_sm0_st2_ch2_wt1_wtr0_nr0.1_stmi10_rw1_pn0_cbc0.1_icc0.05_mtmc0.1_stmc0.01";