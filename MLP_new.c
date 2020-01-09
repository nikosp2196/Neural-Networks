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
#define L 3000 // 1, 30, 300, 3000 Batch size


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
void calculate_pd(float parameters[5]);
void calculate_square_errors();
void update_weights();
void test_network();
void gradient_descent();
void set_pd_to_zero();

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
// Partial derivatives
float pd_exit_h2[K][H2+1];
float pd_h2_h1[H2][H1+1];
float pd_h1_insert[H1][d+1];
// Square errors
float current_square_error = 100000;
float previous_square_error = 0;


FILE *ftrain = NULL; 
FILE *ftest = NULL; 
FILE *fp = NULL;
FILE *fright = NULL;
FILE *fwrong = NULL;

int main(int argc, char** argv) {
    srand(time(0));
    set_pd_to_zero();

    open_files();
    load_data();

    train_network();
    test_network();
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

        fprintf(fp, "Linear function activated\n");
    
    }else{

        fprintf(fp, "Tanh function activated \n");

    }

    fprintf(fp,"\n\n");
    
}


void load_data(){

    int i;
    char skip[5];

    for(i=0; i<3000; i++){

        fscanf(ftrain, "%f", &train_set[i][0]);
        fscanf(ftrain, "%f", &train_set[i][1]);
        fscanf(ftrain, "%f", &train_set[i][2]);
        fscanf(ftrain, "%s", skip);
        fscanf(ftrain, "%f", &train_set[i][3]);
        fscanf(ftrain, "%f", &train_set[i][4]);

        
        fscanf(ftest, "%f", &test_set[i][0]); 
        fscanf(ftest, "%f", &test_set[i][1]);
        fscanf(ftest, "%f", &test_set[i][2]);
        fscanf(ftest, "%s", skip);
        fscanf(ftest, "%f", &test_set[i][3]);
        fscanf(ftest, "%f", &test_set[i][4]);
        
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
        if (exit_hidden1[i] != exit_hidden1[i]){
            exit(0);
        }
        
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
    calculate_pd(parameters);

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

void initialize_weights(){

    int  i, j;

    for(i = 0; i < H1; i++){

        for(j = 0; j < d; j++){

            insert_hidden1_weights[i][j] = random_weight_value();
        }
    }

    for(i = 0; i < H2; i++){

        for(j = 0; j < H1; j++){

            hidden1_hidden2_weights[i][j] = random_weight_value();
        }
    }

    for(i = 0; i < K; i++){

        for(j = 0; j < H2; j++){

            hidden2_exit_weights[i][j] = random_weight_value();
        }
    }
}

void train_network(){

    init_b();
    initialize_weights();
    gradient_descent();

}

void gradient_descent(){
    
    double error_diff;
    int epoch_count = 0;
    int batch_count;

    while( epoch_count < 500 || error_diff >= 0.001 ) {
        
        batch_count = 0; 
        
        for (int i = 0; i < 3000; i++){
            printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>EPOCH: %d ID: %d\n", epoch_count, i);
            
            backprop(train_set[i]);

            if (batch_count == L-1){
                
                update_weights();
                batch_count = 0;
                set_pd_to_zero();

            }
            batch_count++;
        }
        
        
        calculate_square_errors();
        error_diff = abs(current_square_error - previous_square_error);
        epoch_count++;
        fprintf(fp," epoch %d - square error  : %f \n", epoch_count, current_square_error);

    }
    
}

void calculate_pd(float parameters[5]){

    for (int i = 0; i < K; i++) {

        pd_exit_h2[i][0] += delta_[i];
        
        for (int j = 1; j < H2+1; j++){
        
            pd_exit_h2[i][j] += delta_[i] * exit_hidden2[j];
        
        }
   
    }

    for (int i = 0; i < H2; i++) {

        pd_h2_h1[i][0] += delta_hidden2[i];
        
        for (int j = 1; j < H1+1; j++){
        
            pd_h2_h1[i][j] += delta_hidden2[i] * exit_hidden1[j];
        
        }
   
    }
    
    for (int i = 0; i < H1; i++) {

        pd_h1_insert[i][0] += delta_hidden1[i];
        
        for (int j = 1; j < d+1; j++){
        
            pd_h1_insert[i][j] += delta_hidden1[i] * parameters[j+3];
        
        }
   
    }

}

void update_weights(){
    
    for (int i = 0; i < K; i++) {

        bel[i] -= n * pd_exit_h2[i][0];
        
        for (int j = 1; j < H2+1; j++){
        
            hidden2_exit_weights[i][j-1] -= n * pd_exit_h2[i][j];
        
        }
   
    }

    for (int i = 0; i < H2; i++) {

        b_hidden2[i] -= n * pd_h2_h1[i][0];
        
        for (int j = 1; j < H1+1; j++){
        
            hidden1_hidden2_weights[i][j-1] -= n * pd_h2_h1[i][j];
        
        }
   
    }
    
    for (int i = 0; i < H1; i++) {

        b_hidden1[i] -= pd_h1_insert[i][0];
        
        for (int j = 1; j < d+1; j++){
        
            insert_hidden1_weights[i][j-1] -= n * pd_h1_insert[i][j];
        
        }
   
    }
}

void calculate_square_errors(){

    float tmp;
    float result = 0;
    
    for (int i = 0; i < 3000; i++){

        tmp  = 0;

        for (int j = 0; j < K; j++){

            tmp += train_set[i][j] - exit_level[j]; 

        }
        
        result += pow(tmp, 2) / 2.0;

    }
    previous_square_error = current_square_error;
    current_square_error = result;

    printf("PREVIOUS SQUARE ERROR:")
}

void test_network(){

    for (int i = 0; i < 3000; i++){

        forward_pass(test_set[i]); // Run one by one the examples
        //printf("EXIT NEURONS: %f %f %f\n", exit_level[0], exit_level[1], exit_level[2]);
        //printf("ACTUAL CATEGORY: %f %f %f\n", test_set[i][0], test_set[i][1], test_set[i][2]);

    }
}

void set_pd_to_zero(){

    for (int i = 0; i < K; i++) {
        for (int j = 0; j < H2+1; j++){
            pd_exit_h2[i][j] = 0;
        }
    }

    for (int i = 0; i < H2; i++) {
        for (int j = 0; j < H1+1; j++){
            pd_h2_h1[i][j] = 0;
        }
    }

    for (int i = 0; i < H1; i++) {
        for (int j = 0; j < d+1; j++){
            pd_h1_insert[i][j] = 0;
        }
    }

}