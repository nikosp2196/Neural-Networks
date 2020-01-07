#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>


#define FILENAME "groups.txt"
#define DATA_SIZE 600
#define M 3 // Number of groups
#define D 2 // Dimension of data
#define R 5 // Number of executions


void loadLine(int i);
void initialize();
double getDistance(int i, int c);
void calculateAllDistances();



FILE* fp;
int groups[DATA_SIZE];              // True labels
double data[DATA_SIZE][D];          // Data
double centroids[R][M][D];          // Cluster centroids
int centroid_labels[R][M];          // Centroid labels
double distances[R][DATA_SIZE][M];  // Stores every distance that we calculated
int r;                              // Round counter
int t;                              // Season counter
float h;                            // Learning rate
double errors[R];                   // Clustering errors


int main(int argc, char** argv) {
    srand(time(0));


    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("Problem with the file.");
        exit(0);
    }

    for (int i = 0; i < DATA_SIZE; i++){
        loadLine(i);
        printf("Category: %d  X1: %f  X2: %f\n", groups[i], data[i][0], data[i][1]);
    }

    fclose(fp);


    for (r = 0; r < R; r++){
        printf("----------EXECUTION: %d\n", r);
        initialize();
    }

}

void loadLine(int i){

    fscanf(fp, "%d", &groups[i]);
    fscanf(fp, "%lf", &data[i][0]);
    fscanf(fp, "%lf", &data[i][1]);
}

void initialize(){
    int temp;
    for(int i = 0; i < M; i++){
        temp = rand() % DATA_SIZE;
        centroid_labels[r][i] = groups[temp]; // LVQ: set centroid labels
        centroids[r][i][0] = data[temp][0];
        centroids[r][i][1] = data[temp][1];
    }
    t = 0;
}

double getDistance(int i, int c){
    double diff_x1 = data[i][0] - centroids[r][c][0];
    double diff_x2 = data[i][1] - centroids[r][c][1];

    double sqr1 = pow(diff_x1, 2);
    double sqr2 = pow(diff_x2, 2);

    double distance = sqrt(sqr1 + sqr2);


    return distance;
}

void calculateAllDistances(){
    double mindist;
    int cluster;
    for (int i = 0; i < DATA_SIZE; i++){
        mindist = 5;
        cluster = -1;
        for (int j = 0; j < M; j++){
            distances[r][i][j] = getDistance(i,j); // Dist(i,j)
            if(mindist > distances[r][i][j]){      // Check for min
                mindist = distances[r][i][j];
                cluster = j;                       // The closest cluster
            }
        }
        // TODO: Turn this into a function.
        if (centroid_labels[r][cluster] == groups[i]){
            centroids[r][cluster][0] = centroids[r][cluster][0] + h * (data[i][0] - centroids[r][cluster][0]);
            centroids[r][cluster][1] = centroids[r][cluster][1] + h * (data[i][1] - centroids[r][cluster][1]);
        }else{
            centroids[r][cluster][0] = centroids[r][cluster][0] - h * (data[i][0] - centroids[r][cluster][0]);
            centroids[r][cluster][1] = centroids[r][cluster][1] - h * (data[i][1] - centroids[r][cluster][1]);
        }   
    }
}