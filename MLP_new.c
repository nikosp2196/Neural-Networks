#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define d 2 
#define K 3 
#define H1 8 // 5,7,8
#define H2 6 // 3,4,5
#define func 0 // 0: tanh   1: linear
#define n 0.1 
#define L  300 // 1, 300, 3000 Batch size


void open_files();
void load_data();
void forward_pass(float parameters[5]);
void reverse(float parameters[5]);
void backprop(float parameters[5]);
float logistic(float x);
float tanh_(float x);
float linear(float x);
void init_b();
void initialize_weights();
void train_network();



float train_set[3000][5];
float test_set[3000][5];
// Weights
float insert_hidden1_weights[H1][d]; 
float hidden1_hidden2_weights[H2][H1]; 
float hidden2_exit_weights[K][H2];
// Polarization
float b_hidden1[H1]; 
float b_hidden2[H2]; 
float bel[K];
// Neuron Outputs
float exit_hidden1[H1]; 
float exit_hidden2[H2]; 
float exit_level[K]; 
// Neuron errors
float delta_hidden1[H1]; 
float delta_hidden2[H2]; 
float delta_[K];

/*
float square_error[3000]; 
float general_error[3000]; 

int right = 0; 
int wrong = 0; 
int cntrTrain = 0; 
int cntrTest = 0; 
*/


FILE *ftrain = NULL; 
FILE *ftest = NULL; 
FILE *fp = NULL;
FILE *fright = NULL;
FILE *fwrong = NULL;

int main(int argc, char** argv) {
    srand(time(0));

    open_files();
    load_data();

}

void open_files(){

    ftrain = fopen("train.txt", "r");

    if(ftrain == NULL){
    
        fprintf(stderr, "File cannot open \n");
        exit(1);

    }

    ftest = fopen("test.txt", "r");

    if(ftest == NULL){
    
        fprintf(stderr, "File cannot open \n");
        exit(1);

    }

	fp = fopen("MLPresults.txt", "w");

    if(fp == NULL){
    
        fprintf(stderr, "File cannot open \n");
        exit(1);

    }


    fwrong = fopen("wrong_decisions.txt", "w");

    if(fwrong == NULL){
    
        fprintf(stderr, "File cannot open \n");
        exit(1);

    }

    fright = fopen("right_decisions.txt", "w");

    if(fright == NULL){
    
        fprintf(stderr, "File cannot open \n");
        exit(1);

    }

    fprintf(fp,"Entries: %d --- Hidden-Layer1 neurons: %d --- Hidden-Layer2 neurons: %d --- L: %d\n", d, H1, H2, L);
    
    if(func == 1){

        fprintf(fp, "Logistic function activated\n");
    
    }else{

        fprintf(fp, "Tanh function activated \n");

    }

    fprintf(fp,"\n\n");
}


void load_data(){

    int i;

    for(i=0; i<3000; i++){

        fscanf(ftrain, "%10f", &train_set[i][0]); 
        fgetc(ftrain);
        fscanf(ftrain, "%10f", &train_set[i][1]);
        fgetc(ftrain);
        fscanf(ftrain, "%10f", &train_set[i][2]);
        fgetc(ftrain);
        fgetc(ftrain); //to skip label C1,  C2  C3
        fgetc(ftrain);
        fscanf(ftrain, "%10f", &train_set[i][3]);
        fgetc(ftrain);
        fscanf(ftrain, "%10f", &train_set[i][4]);
        fgetc(ftrain);


        fscanf(ftest, "%10f", &test_set[i][0]); 
        fgetc(ftest);
        fscanf(ftest, "%10f", &test_set[i][1]);
        fgetc(ftest);
        fscanf(ftest, "%10f", &test_set[i][2]);
        fgetc(ftest);
        fgetc(ftest); //to skip label C1,  C2  C3 
        fgetc(ftest);
        fscanf(ftest, "%10f", &test_set[i][3]);
        fgetc(ftest);
        fscanf(ftest, "%10f", &test_set[i][4]);
        fgetc(ftest);
    }
}

void forward_pass(float parameters[5]){

    int i, j;
    float tmp;
    
    for(i = 0; i < H1; i++){
    
        tmp = b_hidden1[i];

        for(j = 0; j < d; j++){

           tmp = tmp + insert_hidden1_weights[i][j] * parameters[j+3];
        }


        exit_hidden1[i] = logistic(tmp);
        
        
    }

    for(i = 0; i < H2; i++){

        tmp = b_hidden2[i];

        for(j = 0; j < H1; j++){

           tmp = tmp + hidden1_hidden2_weights[i][j] * exit_hidden1[j] ;
        }
        if(func == 1){

        	exit_hidden2[i] = linear(tmp);
        
        }else{

        	exit_hidden2[i] = tanh_(tmp);
        }
    }

    for(i = 0; i < K; i++){

        tmp = bel[i];

        for(j = 0; j < H2; j++){

            tmp = tmp + hidden2_exit_weights[i][j] * exit_hidden2[j];

        }

        exit_level[i] = logistic(tmp);

    }

    // calculate_square_errors(parameters);

}

void reverse(float parameters[5]){

	int i, j;
	double tmp;

    for(i = 0; i < K; i++){
        
        delta_[i] = exit_level[i] * (1 - exit_level[i]) * (exit_level[i] - parameters[i]);

    }
    
    for(i = 0; i < H2; i++){

        tmp = 0;

        for(j = 0; j < K; j++){

            tmp = tmp + hidden2_exit_weights[j][i] * delta_[j];

        }
        // FIX THIS: Linear or Logistic function
        if(func == 1){

        	delta_hidden2[i] = tmp;

        }else{

        	delta_hidden2[i] = (1 - pow(exit_hidden2[i], 2)) * tmp;

		}
    }

    
    for(i = 0; i < H1; i++){

        tmp = 0;

        for(j = 0; j < H2; j++){

            tmp = tmp + hidden1_hidden2_weights[j][i] * delta_hidden2[j];

        }

        delta_hidden1[i] = exit_hidden1[i] * (1 - exit_hidden1[i])  * tmp;

    }
}

float logistic(float x){

    return 1 / (1 + exp(-x));

}

float tanh_(float x){

    return (exp(x) - exp(-x)) / (exp(x) + exp(-x));
    
}

float linear(float x){

    return x;
    
}

void backprop(float parameters[5]){

    forward_pass(parameters);
    reverse(parameters);

}

float random_weight_value(){

    int upper = 1, lower = -1;

    return ((float)rand() / ((float)RAND_MAX / (upper - lower))) + lower;

}

void init_b(){

    int i;

    for(i = 0; i < H1; i++){

        b_hidden1[i] = random_weight_value();
    }

    for(i = 0; i < H2; i++){

        b_hidden2[i] = random_weight_value();
    }

    for(i = 0; i < K; i++){

        bel[i] = random_weight_value();
    }
}

void train_network(){

    init_b();
    initialize_weights();
    gradient_descent();

}

void gradient_descent(){

    
}