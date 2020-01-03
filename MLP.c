
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define d 2 
#define K 3 
#define H1 8 // 5,7,8
#define H2 6 // 3,4,5
#define func 0 // 0: tanh   1: logistic
#define n 0.1 
#define L  300 // 1, 300, 3000

void open_files();
void load_data();
void initialize_training();
void initialize_weights();
void initB();
float random_weight_value();
void forward_pass(float parameters[6]);
void forward_pass_test_set(float parameters[6]);
void reverse(float parameters[6]);
void backprop(float parameters[6]);
void gradient_descent();

void calculate_square_errors(float parameters[6]);
void calculate_general_error(float parameters[6]);
void calculate_weights();
void final_results(int r,int w);


float train_set[3000][6]; 
float test_set[3000][6]; 

float insert_hidden1_weights[H1][d]; 
float hidden1_hidden2_weights[H2][H1]; 
float hidden2_exit_weights[K][H2]; 

float b_hidden1[H1]; 
float b_hidden2[H2]; 
float bel[K]; 

float exit_hidden1[H1]; 
float exit_hidden2[H2]; 
float exit_level[K]; 
float error[K]; 

float delta_hidden1[H1]; 
float delta_hidden2[H2]; 
float delta_[K];

float square_error[3000]; 
float general_error[3000]; 

int right = 0; 
int wrong = 0; 
int cntrTrain = 0; 
int cntrTest = 0; 

FILE *ftrain = NULL; 
FILE *ftest = NULL; 
FILE *fp = NULL;
FILE *fright = NULL;
FILE *fwrong = NULL;

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


float random_weight_value(){

    int random_num;
    float float_random_num;

	float_random_num = (RAND_MAX - rand()) / (float)(RAND_MAX); 
  	random_num = rand() % 2;
	if (random_num == 2) return float_random_num;
	return (-2 * float_random_num);
}

void initialize_training(){

	initialize_weights();
	initB();
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

void initB(){

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

void gradient_descent(){

    float parameters[6], square_error_after = 0.0, square_error_before = 0.0;
    float diff;
    double r;
    int i, j, k, epoch, l, s;

   	initialize_training();

    cntrTrain = 0;
    s = 0;

    for(k = 1; k <= (3000 / L); k++){

        for(i = s; i < s + L; i++){
 
            for(j = 0; j < 5; j++){

                parameters[j] = train_set[i][j];
            }

            backprop(parameters);

            square_error_after = square_error_after + square_error[i];
            cntrTrain++;
        }

        calculate_weights(); 
        s = i;
	}
    
    epoch = 1;
    fprintf(fp, " epoch %d --- square error: %12f \n", epoch, square_error_after);
    cntrTrain = 0;


    for(l = 2; l <= 500; l++){

        square_error_before = square_error_after;
        square_error_after = 0;
        cntrTrain = 0;
        s = 0;


        for(k = 1; k <= (3000 / L); k++){
   		
	        for(i = s; i < s + L; i++){
	        
	            for(j = 0; j < 5; j++){

	                parameters[j] = train_set[i][j];

                }
	            backprop(parameters);

	            square_error_after = square_error_after + square_error[i];
	            cntrTrain++;
	        }

	        calculate_weights(); 
			s = i;
    	}

        epoch++;
     	fprintf(fp," epoch %d --- square error: %12f \n",epoch,square_error_after);
        cntrTrain = 0;
    }

    diff = fabs(square_error_after - square_error_before);

    if( diff <= 0.000000001){

    	return;

	}

    while( diff > 0.0000000001){
    
        square_error_before = square_error_after;
        square_error_after = 0;
        cntrTrain = 0;
        s = 0;

        for(k = 1; k <= (3000 / L); k++){
   		
	        for(i = s; i < s+L; i++){
	        
	            for(j = 0; j < 5; j++){

	                parameters[j] = train_set[i][j];
                }
	            backprop(parameters);

	            square_error_after = square_error_after + square_error[i];
	            cntrTrain++;
	        }
	        calculate_weights(); 
    	}
        epoch++;
     	fprintf(fp," epoch %d - square error  : %f \n",epoch,square_error_after);

        diff = abs(square_error_after - square_error_before);
    }

}

void backprop(float parameters[6]){

    forward_pass(parameters);
    reverse(parameters);

}


void forward_pass(float parameters[6]){

    int i, j;
    float tmp;
    
    for(i = 0; i < H1; i++){
    
        tmp = b_hidden1[i];

        for(j = 0; j < d; j++){

           tmp = tmp + insert_hidden1_weights[i][j] * parameters[i];
        }
        if(func == 1){

        	exit_hidden1[i] = 1 / (1 + exp(-tmp));
        
        }else{

        	exit_hidden1[i] = (exp(tmp) - exp(-tmp)) / (exp(tmp) + exp(-tmp));
        }
    }

    for(i = 0; i < H2; i++){

        tmp = b_hidden2[i];

        for(j = 0; j < H1; j++){

           tmp = tmp + hidden1_hidden2_weights[i][j] * exit_hidden1[j] ;
        }
        if(func == 1){

        	exit_hidden2[i] = 1 / (1 + exp(-tmp));
        
        }else{

        	exit_hidden2[i] = (exp(tmp) - exp(-tmp))/(exp(tmp) + exp(-tmp));
        }
    }

    for(i = 0; i < K; i++){

        tmp = bel[i];

        for(j = 0; j < H2; j++){

            tmp = tmp + hidden2_exit_weights[i][j] * exit_hidden2[j];

        }

        exit_level[i] = 1 / (1 + exp(-tmp));

    }

    calculate_square_errors(parameters);

}


void calculate_square_errors(float parameters[6]){

	int i;
	
    square_error[cntrTrain] = 0;

    for(i = 0; i < K; i++){
   
        error[i] = parameters[i] - exit_level[i];
        square_error[cntrTrain] += error[i] * error[i] ;
    }

    square_error[cntrTrain] = square_error[cntrTrain] / 2.0;
}


void reverse(float parameters[6]){

	int i, j;
	double tmp;

    for(i = 0; i < K; i++){

        delta_[i] = exit_level[i] * (1 - exit_level[i]) * (exit_level[i] - parameters[i]);

    }
    
    for(i = 0; i < H2; i++){

        tmp = 0;

        for(j = 0; j < K; j++){
            tmp = tmp + hidden1_hidden2_weights[i][j] * delta_[j];
        }

        if(func == 1){
        	delta_hidden2[i] = exit_hidden2[i] * (1 - exit_hidden2[i]) * tmp;

        }else{

        	delta_hidden2[i] = (1 + exit_hidden2[i]) * (1 - exit_hidden2[i] ) * tmp;

		}
    }

    
    for(i = 0; i < H1; i++){

        tmp = 0;

        for(j = 0; j < H2; j++){

            tmp = tmp + insert_hidden1_weights[i][j] * delta_hidden2[j];

        }
        if(func == 1){

        	delta_hidden1[i] = exit_hidden1[i] * (1 - exit_hidden1[i] ) * tmp;

        }else{

        	delta_hidden1[i] = (1 + exit_hidden1[i]) * (1 - exit_hidden1[i] )  * tmp;
        }

    }

}


void calculate_weights(){

	int i, j;

    for(i = 0; i < K; i++){
    
        for(j = 0; j < H2; j++){
        
            
            hidden2_exit_weights[i][j] = hidden2_exit_weights[i][j] - n  * delta_[i] * exit_hidden2[j];
        }
       
        bel[i] = bel[i] - n * delta_[i];
    }

     
    for(i = 0; i < H2; i++){
    
        for(j = 0; j < H1; j++){

            hidden1_hidden2_weights[i][j] = hidden1_hidden2_weights[i][j] - n * delta_hidden2[i] * exit_hidden1[j];
        }

        b_hidden2[i] = b_hidden2[i] - n * delta_hidden2[i];
    }

    
    for(i = 0; i < H1; i++){

        for(j = 0; j < d; j++){
  

            insert_hidden1_weights[i][j] = insert_hidden1_weights[i][j] - n * delta_hidden1[i]; 
        }

        b_hidden1[i] = b_hidden1[i] - n * delta_hidden1[i];
    }
}



void forward_pass_test_set(float parameters[6]){

    int i,j;
    float tmp;

    for(i = 0; i < H1; i++){
    
        tmp = b_hidden1[i];

        for(j = 0; j < d; j++){

           tmp = tmp + insert_hidden1_weights[i][j] * parameters[i];
        }

        if(func == 1){

        	exit_hidden1[i] = 1 / (1 + exp(-tmp));

        }else{

        	exit_hidden1[i] = (exp(tmp) - exp(-tmp)) / (exp(tmp) + exp(-tmp));
        }
    }

    
    for(i = 0; i < H2; i++){
    
        tmp = b_hidden2[i];

        for(j = 0; j < H1; j++){

           tmp = tmp + hidden1_hidden2_weights[i][j] * exit_hidden1[j];
        }
        if(func == 1){

        	exit_hidden2[i] = 1 / (1 + exp(-tmp));

        }else{

        	exit_hidden2[i] = (exp(tmp) - exp(-tmp)) / (exp(tmp) + exp(-tmp));

        }
    }

    
    for(i = 0; i < K; i++){

        tmp = bel[i];

        for(j = 0; j < H2; j++){

            tmp = tmp + hidden2_exit_weights[i][j] * exit_hidden2[j];
        }

        exit_level[i] = 1 / (1 + exp(-tmp));

    }

 	calculate_general_error(parameters);
}


void calculate_general_error(float parameters[6]){

	int i;

    general_error[cntrTest] = 0;

    for(i = 0; i < K; i++){
    
        error[i] = parameters[i] - exit_level[i];
        general_error[cntrTest] += error[i] * error[i] ;
    }
    general_error[cntrTest] = general_error[cntrTest] / 2.0;
}


void final_results(int r, int w){

    int i;
    double final_err;

    for(i = 0; i < 3000; i++){

        final_err += general_error[i];
    }

    final_err /= 3000;

    fprintf(fp, "Average Error At Testing: %f \n", final_err);

    fprintf(fp, " Right patterns : %d, percentage: %f. Wrong patterns : %d, percentage: %f \n", r, ((100 * r) / 3000.0), w, 100 - ((100 * r) / 3000.0));
}


int main(int argc, char** argv) {

    int i, j, flag;
    float x[6];

    open_files();
    load_data();
    gradient_descent();

    cntrTest=0;
    
    for(i = 0; i < 3000; i++){
    
        for(j = 0; j < 5; j++){

            x[j] = test_set[i][j];

        }

        forward_pass_test_set(x);
        
        flag = 1; 

        for(j = 0; j < K; j++){

            if((x[j] == 0 && exit_level[j] >= 0.5) || (x[j] == 1 && exit_level[j] < 0.5)){

                flag = 0;    
            }
          
        }

        if(flag == 1){

            right++;
            fprintf(fright, "%f  %f \n", x[4], x[5]);

        }else{

            wrong++;
            fprintf(fwrong, "%f  %f \n", x[4], x[5]);
        }

        cntrTest++;
    }

    final_results(right, wrong);

    fclose(ftrain);
    fclose(ftest);

    return (EXIT_SUCCESS);
}