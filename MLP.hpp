/**********************************/
/*                                */
/*             MLP.h              */
/*     typical neural network     */
/*                                */
/**********************************/
#pragma once
#include <iostream>
#include <stdlib.h>
#include <math.h>
//#include "FrontMatter.h"

//default
#define MAXINPUT 100
#define MAXHIDDEN 100
#define MAXOUTPUT 100

#define MAXSOM 30

//for Maintenance switch
#define CHKNN 0

using std::cout;
using std::endl;

//========================== nn class declaration =======================//
class NN{
private:
  int inputcell;
  int hiddencell;
  int outputcell;
  double nnalpha;
  double wh[MAXHIDDEN][MAXINPUT+1], wo[MAXOUTPUT][MAXHIDDEN+1];  //weight of neural network and threshold(+1)
  double hi[MAXHIDDEN+1];  //output of hidden layer and threshold(+1)

  void initwh();  //initialize weight of hidden layer;
  void initwo();  //initialize weight of output layer;
  double sigmoid(double u);  //sigmoid function

public:
  double output[MAXOUTPUT]; //output

  void init_nn(int i, int h, int o, double a);
  double forward(int a, double e[]);  //forward calliculation;
  void olearn(int a, double e[]);  //learning weight of output layer;
  void hlearn(int a, double e[]);  //learning weight of hidden layer;
  double drand();  //generating random number from -1 to 1
  void printweight();
};
//========================================================================//



//========================== som class declaration =======================//
class SOM{
private:
  int inputcell;
  int outputcell;
  int somsize;
  double somalpha;
  double ws[MAXSOM][MAXSOM][MAXINPUT];

  void initws();  //initialize weight of som;
  double sigmoid(double u);  //sigmoid function

public:
  int somout[MAXOUTPUT];
  void init_som(int i, int o, double a);
  void exposure(double e[]);
  void addnoise(double e[]);
  double maintain(double e[]);
  double drand();  //generating random number from -1 to 1
};
//========================================================================//



//========================== nn class realization =======================//
void NN::init_nn(int i, int h, int o, double a){
  //parameter setting of neural network
  inputcell = i;
  hiddencell = h;
  outputcell = o;
  nnalpha = a;


  initwh();
  initwo();
}


void NN::initwh(){
  for(int i = 0; i < hiddencell; ++i){
    for(int j = 0; j < inputcell+1; ++j){
      if(j == INFO_SIZE){
        wh[i][j] = 0.05*drand();
      }else{
        wh[i][j] = 0.1*drand();
      }
    }
  }
}

void NN::initwo(){
  for(int o = 0; o < outputcell; ++o){
    for(int i = 0; i < hiddencell+1; ++i){
      if(o == 0){
        wo[o][i] = 0.05*drand();
      }else{
        wo[o][i] = 0.1*drand();
      }
    }
  }
}

double NN::forward(int a, double e[]){
  double u;  //sum of weight calculation
  double o;  //output calculation
  int i, j;

  //hidden calculation
  for(i = 0; i < hiddencell; ++i){
    u = 0;
    for(j = 0; j < inputcell; ++j){
      u += e[j]*wh[i][j];
    }
    u -= wh[i][j];
    hi[i] = sigmoid(u);
  }

  //output calculation
  o = 0;
  for(i = 0; i < hiddencell; ++i){
    o += hi[i]*wo[a][i];
  }
  o -= wo[a][i]; //threshold
  if(CHKNN) cout << "qn_sigmoid(o): " << sigmoid(o) << endl;
  return sigmoid(o);
}


void NN::olearn(int a, double e[]){
  double d;
  int i,j;

  d = (e[inputcell+a] - output[a]) * output[a] * (1-output[a]);  //error calculation
  for(i = 0; i < hiddencell; ++i){
    wo[a][i] += nnalpha * hi[i] * d;  //learning weight
  }
  wo[a][i] += nnalpha*(-1.0)*d;  //learning threshold
}

void NN::hlearn(int a, double e[]){
  double dj; //for weight of hidden layer caliculation
  int i,j;

  for(j = 0; j < hiddencell; ++j){
    dj = hi[j]*(1-hi[j])*wo[a][j]*(e[inputcell+a]-output[a])*output[a]*(1-output[a]);
    for(i = 0; i < inputcell; ++i){
      wh[j][i] += nnalpha*e[i]*dj;
    }
    wh[j][i] += nnalpha*(-1.0)*dj; //learning threshold
  }
}

void NN::printweight(){
  int i,j;

  cout << "=== wh value ===" << endl;
  for(i = 0; i < hiddencell; ++i){
    for(j = 0; j < inputcell+1; ++j){
      cout << "wh[" << i << "][" << j << "]:" << wh[i][j] << endl;
    }
  }
  cout << "=== wo value ===" << endl;
  for(i = 0; i < outputcell; ++i){
    for(j = 0; j < hiddencell+1; ++j){
      cout << "wo[" << i << "][" << j << "]:" << wo[i][j] << endl;
    }
  }
  cout << endl;
}

double NN::sigmoid(double u){
  return 1.0/(1.0+exp(-u));
}

double NN::drand(){
  double rndno;

  while((rndno=(double)abs(int_urand()*2)/RAND_MAX)==1.0);
  rndno=rndno*2-1; //generate real number from -1 to 1
  return rndno;
}

//========================================================================//



//========================== som class realization =======================//
void SOM::init_som(int i, int s, double a){
  //parameter setting of neural network
  inputcell = i;
  outputcell = i;
  somsize = s;
  somalpha = a;
  initws();
}

void SOM::initws(){
  for(int y = 0; y < somsize; ++y){
    for(int x = 0; x < somsize; ++x){
      if((y+x)%2 == 0){ //effective nodes
        for(int i = 0; i < inputcell; ++i) ws[y][x][i] = drand();  //Do not use constant value
      }else{
        for(int i = 0; i < inputcell; ++i) ws[y][x][i] = -100;
      }
    }
  }
}



void SOM::exposure(double e[]){
  double u, u_min;; //output calculation
  int win_x, win_y;
  int r = 2; //neighborhood
  int d;

  //som exposure
  u_min = 100000;
  for(int y = r; y < somsize-r; ++y){
    for(int x = r; x < somsize-r; ++x){
      if((y+x)%2 == 0){ //effective nodes
        u = 0;
        for(int i = 0; i < inputcell; ++i) u += pow(e[i] - sigmoid(ws[y][x][i]), 2);
        u = sqrt(u);
        if(u_min > u){
          u_min = u;
          win_x = x;
          win_y = y;
        }
      }
    }
  }

  for(int y = win_y - r; y <= win_y + r; y++){
    for (int x = win_x - r; x <= win_x + r; x++){
      if((y+x)%2 == 0){ //effective nodes
        //勝者ノードと現在注目しているノードとの距離
        d = abs(y - win_y) + abs(x - win_x);
        if(d <= r){  //update range
          // update node
          for(int i = 0; i < inputcell; ++i)ws[x][y][i] += somalpha*(e[i] - sigmoid(ws[x][y][i]))/(1+d);
        }
      }
    }
  }

  //output calculation
  for(int o = 0; o < outputcell; ++o){
    if(sigmoid(ws[win_y][win_x][o]) > 0.5){somout[o] = 1;}else{somout[o] = 0;}
  }
}


double SOM::maintain(double e[]){
  double u, u_min;
  int win_x, win_y;
  int r = 2; //neighborhood
  double ambiguity = 0;

  //output calculation
  u_min = 100000;
  for(int y = r; y < somsize-r; ++y){
    for(int x = r; x < somsize-r; ++x){
      if((y+x)%2 == 0){ //effective nodes
        u = 0;
        for(int i = 0; i < inputcell; ++i) u += pow(e[i] - sigmoid(ws[y][x][i]), 2);
        u = sqrt(u);
        if(u_min > u){
          u_min = u;
          win_x = x;
          win_y = y;
        }
      }
    }
  }

  for(int o = 0; o < outputcell; ++o){
    double distinction = sigmoid(ws[win_y][win_x][o]);
    if(distinction > 0.5){
        somout[o] = 1;
        ambiguity += abs(1-distinction);
    }else{
        somout[o] = 0;
        ambiguity += abs(0-distinction);
    }
  }

  return ambiguity;
}

double SOM::sigmoid(double u){
  return 1.0/(1.0+exp(-u));
}

double SOM::drand(){
  double rndno;

  while((rndno=(double)abs(int_urand()*2)/RAND_MAX)==1.0);
  rndno=rndno*2-1; //generate real number from -1 to 1
  return rndno;
}

//==========================================================================//
