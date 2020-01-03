#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>


#define FILENAME "groups.txt"
#define DATA_SIZE 600
#define M 3
#define D 2


void loadLine(int i);
double getDistance(int i, int c);
void initialize();


FILE* fp;
int groups[DATA_SIZE];
double data[DATA_SIZE][D];
int clusters[M][DATA_SIZE]; //If the dataset was larger we should malloc.
double centroids[M][D];
int t;


int main(int argc, char** argv) {
    srand(time(0));


    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("Problem with the file.");
        exit(0);
    }
    
    for (int i = 0; i < DATA_SIZE; i++){
        loadLine(i);
    }

    initialize();
    printf("DATA--> x1 = %lf  x2 = %lf\n", data[0][0], data[0][1]);
    printf("CENTROID--> x1 = %lf  x2 = %lf\n", centroids[0][0], centroids[0][1]);
    printf("Distance: %f\n", getDistance(0,0));
    
    fclose(fp);


    

}

void loadLine(int i){
    char buff[10];
    printf("%d\n",i);
    fscanf(fp, "%s", buff);
    printf("%s\n", buff);

    groups[i] = (int)buff[1] - 48 - 1;
    printf("%d\n", groups[i]);

    fscanf(fp, "%lf", &data[i][0]);
    printf("%lf\n", data[i][0]);

    fscanf(fp, "%lf", &data[i][1]);
    printf("%lf\n", data[i][1]);
}

double getDistance(int i, int c){
    double diff_x1 = data[i][0] - centroids[c][0];
    double diff_x2 = data[i][1] - centroids[c][1];

    double sqr1 = pow(diff_x1, 2);
    double sqr2 = pow(diff_x2, 2);

    double distance = sqrt(sqr1 + sqr2);


    return distance;
}

void initialize(){
    int temp;
    for(int i = 0; i < M; i++){
        temp = rand() % DATA_SIZE;
        centroids[i][0] = data[temp][0];
        centroids[i][1] = data[temp][1];
    }
    t = 0;
}