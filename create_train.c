#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define UPPER 2
#define LOWER -2

/**
 * Returns a string with the category and the pair
 * format: category x1 x2
 **/
char* getCategory(float x1, float x2, FILE* fptr);

/**
 * Generates a pair of random floats.
 **/
void generateRandomPair(float* x1, float* x2);

int c1 = 0;
int c2 = 0;
int c3 = 0;

int main(){
    
    char buff[50];
    float x,y;
    FILE* fptr;
    srand(time(0));

    /* First create a train set and then a the test set.
       For the train set just remove the noise if from the comments.
    */
    fptr = fopen("train.txt", "w");

    for(int i = 0; i<3000; i++){

        generateRandomPair(&x, &y);
        strcpy(buff, getCategory(x, y, fptr));
        fputs(buff, fptr);


    }
    printf("c1: %d\nc2: %d\nc3: %d\n", c1,c2,c3);
    fclose(fptr);
}

char* getCategory(float x1, float x2, FILE* fptr){

    static char category[50];
    char str1[15];
    char str2[15];

    gcvt(x1, 8, str1);
    gcvt(x2, 8, str2);

    if ((powf(x1-1, 2) + powf(x2-1, 2)) <= 0.49){

        fprintf(fptr, "0 ");
        fprintf(fptr, "1 ");
        fprintf(fptr, "0 ");
        strcpy(category, "C2");
        c2++;

    }else if ((powf(x1+1, 2) + powf(x2+1, 2)) <= 0.49){

        fprintf(fptr, "0 ");
        fprintf(fptr, "1 ");
        fprintf(fptr, "0 ");
        strcpy(category, "C2");
        c2++;

    }else if ((powf(x1+1, 2) + powf(x2-1, 2)) <= 0.49){

        fprintf(fptr, "1 ");
        fprintf(fptr, "0 ");
        fprintf(fptr, "0 ");
        strcpy(category, "C3");
        c3++;

    }else if((powf(x1-1, 2) + powf(x2+1, 2)) <= 0.49){

        fprintf(fptr, "1 ");
        fprintf(fptr, "0 ");
        fprintf(fptr, "0 ");
        strcpy(category, "C3");
        c3++;

    }else{

        fprintf(fptr, "0 ");
        fprintf(fptr, "0 ");
        fprintf(fptr, "1 ");
        strcpy(category, "C1");
        c1++;

    }

    //NOISE IF
    
    if(!strcmp(category, "C1")){

        float probability = rand() % 100;

        if (probability<10){
            //printf("%f\n", probability);
            strcpy(category,"C1");
        }
    }
  

    gcvt(x1, 8, str1);
    gcvt(x2, 8, str2);
    
    strcat(category, " ");
    strcat(category, str1);
    strcat(category, " ");
    strcat(category, str2);
    strcat(category, "\n");

    printf("%s", category);

    return category;
}

void generateRandomPair(float* x1, float* x2){

    *x1 = ((float)rand() /((float)RAND_MAX / (UPPER - LOWER))) - 2;
    *x2 = ((float)rand() /((float)RAND_MAX / (UPPER - LOWER))) - 2;
}

