using namespace std;

//+++++ FrontMatter.h +++++//
#define RL_SW 0
#define IC_SW 0
#define MTM_SW 1
#define TOTAL_PARAMETER 20
#define SAMPLE 1
#define AGENTS 100

//constant for task
#define CHOICES 3
#define STIMULI 3
#define INFO_SIZE 10
#define WAITING_TIME 5
#define NOISE_RATE 0.1
#define REWARD 2
#define PUNISH 0

//constant for agent
#define ACTION_LIMIT 6000
#define SOFTMAX 0
#define WTRANDOM 0
#define CB_COST 0.1
#define EXPOSURE 0

#define TOTAL_PHENOTYPE 10
#define TOTAL_LN_RESULTS 7

//constant for Q-network
#define INPUTCELL 13
#define OUTPUTCELL 4

string parameter_name[20] = {"AGENTS","CHOICES","STIMULI","INFO_SIZE","WAITING_TIME","NOISE_RATE","REWARD","PUNISH","ACTION_LIMIT","SOFTMAX","WTRANDOM","CB_COST","EXPOSURE","QLeps","QLalpha","QLgamma","hiddencell","NNalpha","SOMsize","SOMalpha"};
double parameter_value[20] = {100,3,3,10,5,0.1,2,0,6000,0,0,0.1,0,0.1,0.5,0.5,5,1,10,1};
string phenotype_name[10] = {"ic","mtm","stmsize","qleps","qlalpha","qlgamma","hiddencell","nnalpha","somsize","somalpha"};
double phenotype[10] = {0,1,10,0.1,0.5,0.5,5,1,10,1};
string ln_results_name[7] = {"rewards","met","cbtime","cbloss","cbeffect","waiting","ambiguity"};
string directory_name = "ag100_inf10_act6000_sm0_st3_ch3_wt5_nr0.1_rw2_pn0_cbc0.1_exp0_qle0.1_qla0.5_qlg0.5_hid5_nna1_soms10_soma1";