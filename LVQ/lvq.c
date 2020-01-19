#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>


#define FILENAME "groups.txt"
#define DATA_SIZE 600
#define M 5 // Number of groups
#define D 2 // Dimension of data
#define R 5 // Number of executions
#define S 5// Number of seasons

void loadLine(int i);
void initialize();
double getDistance(int i, int c);
void calculateAllDistances();
int getMinDist(int i);
void updateCentroids(int i, int cluster);
double getClusteringError();
int getMinError();


FILE* fp;
double data[DATA_SIZE][D];          // Data
double centroids[R][M][D];          // Cluster centroids
int data_labels[R][DATA_SIZE];      // Data Clusters
// TODO: Map the new labels to the data and make the necessary changes to the script.
// TODO: Change the data scripts we dont need labels. These are clustering algorithms.
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
        printf("X1: %f  X2: %f\n", data[i][0], data[i][1]);
    }

    fclose(fp);


    for (r = 0; r < R; r++){
        printf("----------EXECUTION: %d\n", r);
        initialize();

        while(t < S){
            printf("Season: %d\n", t);
            calculateAllDistances();
            h *= 0.95;
            t++;
        }
        errors[r] = getClusteringError();
        printf("$$$$ ERROR = %lf $$$$\n", errors[r]);
    }

    r = getMinError();

    fp = fopen("lvq_clustered.txt", "w");
    for (int i = 0; i< DATA_SIZE; i++){
        fprintf(fp, "%d %f %f\n",data_labels[r][i], data[i][0], data[i][1]);
    }
    for (int i = 0; i < M; i++){
        fprintf(fp, "%s %f %f\n", "C", centroids[r][i][0], centroids[r][i][1]);
    }
    fclose(fp);


}

void loadLine(int i){
    fscanf(fp, "%lf", &data[i][0]);
    fscanf(fp, "%lf", &data[i][1]);
}

void initialize(){
    int temp;
    for(int i = 0; i < M; i++){

        temp = rand() % DATA_SIZE;
        centroids[r][i][0] = data[temp][0];
        centroids[r][i][1] = data[temp][1];

    }
    t = 0;
    h = 0.1;
}

double getDistance(int i, int c){
    double diff_x1 = data[i][0] - centroids[r][c][0];
    double diff_x2 = data[i][1] - centroids[r][c][1];

    double sqr1 = pow(diff_x1, 2);
    double sqr2 = pow(diff_x2, 2);

    double distance = sqrt(sqr1 + sqr2);
    //printf("POINT: %d CLUSTER: %d\n", i, c);
    //printf("DATA X1: %f X2: %f  DIST: %f\n",data[i][0], data[i][1], distance);
    //printf("CENTER X1: %f X2: %f  DIST: %f\n",centroids[r][c][1], centroids[r][c][1], distance);
    return distance;
}

void calculateAllDistances(){
    for (int i = 0; i < DATA_SIZE; i++){
        updateCentroids(i,getMinDist(i));
    }
}

int getMinDist(int i){
    double mindist;
    int cluster;

    distances[r][i][0] = getDistance(i,0);
    mindist = distances[r][i][0];
    cluster = 0;
    //printf("BEFORE SEASON: %d DATA ID: %d  Min Dist: %f \n", t, i, mindist);
    for (int j = 1; j < M; j++){
        distances[r][i][j] = getDistance(i,j); // Dist( i,j)
        if(mindist > distances[r][i][j]){      // Check for min
            mindist = distances[r][i][j];
            cluster = j;                       // The closest cluster
        }
    }
    data_labels[r][i] = cluster;
    return cluster;
    //printf("AFTER  DATA ID: %d  Min Dist: %f \n", i, mindist);
}

void updateCentroids(int i, int cluster) {
    //printf("CLUSTER LABEL: %d  ACTUAL LABEL: %d \n",centroid_labels[r][cluster], groups[i]);
    centroids[r][cluster][0] = centroids[r][cluster][0] + (h * (data[i][0] - centroids[r][cluster][0]));
    centroids[r][cluster][1] = centroids[r][cluster][1] + (h * (data[i][1] - centroids[r][cluster][1]));

}

double getClusteringError(){
    double error = 0;
    for(int d = 0; d < DATA_SIZE; d++){
        
        error += distances[r][d][data_labels[r][d]];
        
    }
    return error;
}

int getMinError(){
    double minValue=10000;
    int minIndex = 0;
    for (int i = 0; i < R; i++){
        if (minValue > errors[i]){
            minValue = errors[i];
            minIndex = i;
        }
    }
    return minIndex;
}

