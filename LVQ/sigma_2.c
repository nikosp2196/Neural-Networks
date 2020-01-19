#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define PI 3.14

void getPairInCircle(double *x, double *y, double R);
void getPairInRectangle(double *x, double *y, double min_x, double max_x, double min_y, double max_y);

int main(){
    char team[50];
    char str1[20];
    char str2[20];
    double x1,x2;
    FILE* fptr;
    srand(time(0));

    fptr = fopen("groups.txt", "w");
    for (int i = 0; i < 600; i++){
        
        if(i<100){
            getPairInCircle(&x1, &x2, 0.3);
            
        }else if(i>99 && i<200){
            getPairInRectangle(&x1, &x2, -1.1, -0.5, 0.5, 1.1);
            
        }else if(i>199 && i<300){
            getPairInRectangle(&x1, &x2, -1.1, -0.5, -1.1, -0.5);
            
        }else if(i>299 && i<400){
            getPairInRectangle(&x1, &x2, 0.5, 1.1, -1.1, -0.5);
            
        }else if(i>399 && i<500){
            getPairInRectangle(&x1, &x2, 0.5, 1.1, 0.5, 1.1);
            
        }else if(i>499 && i<600){
            getPairInRectangle(&x1, &x2, -1, 1, -1, 1);
            
        }else{
            printf("Oh lord!!! It got out of the loop.");
            exit(0);
        }


        gcvt(x1, 8, str1);
        gcvt(x2, 8, str2);

        strcpy(team, str1);
        strcat(team, " ");
        strcat(team, str2);
        strcat(team, "\n");

        fputs(team, fptr);
    }
    fclose(fptr);

}

void getPairInCircle(double *x, double *y, double R){
    double a, r;

    a = rand() * 2 * PI;

    r = ((float)rand()) / ((float)RAND_MAX / (2 * R)) - R;

    *x = r * cos(a);
    *y = r * sin(a);
}

void getPairInRectangle(double *x, double *y, double min_x, double max_x, double min_y, double max_y){

    *x = ((float)rand() / ((float)RAND_MAX / (max_x - min_x))) + min_x;
    *y =  ((float)rand() / ((float)RAND_MAX / (max_y - min_y))) + min_y;
}