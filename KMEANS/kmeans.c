#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>


#define FILENAME "groups.txt"
#define DATA_SIZE 600
#define M 5 // Number of clusters
#define D 2 // Dimension of data
#define R 5 // Number of executions


void loadLine(int i);
double getDistance(int i, int c);
void initialize();
void calculateAllDistances();
void updateCentroids();
int change();
double getClusteringError();
int getMinError();


FILE* fp;
int groups[DATA_SIZE];              // True labels
double data[DATA_SIZE][D];          // Data
int clusters[R][DATA_SIZE];         // Predicted labels
double previous_centroids[M][D];
double centroids[R][M][D];
int counter[M];                     // Counts the elements of every cluster
double distances[R][DATA_SIZE][M];  // Stores all distances from every cluster
int t;                              // Season counter
int r;                              // Round id
double errors[R];


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

    fclose(fp);


    for (r = 0; r < R; r++){
        printf("----------EXECUTION: %d\n", r);
        initialize();

        while(change()){
            printf("Season: %d\n", t);
            calculateAllDistances();
            updateCentroids();
            t++;
        }
        errors[r] = getClusteringError();
        printf("$$$$ ERROR = %lf $$$$\n", errors[r]);
    }
    
    r = getMinError();
    

    fp = fopen("clustered.txt", "w");
    for (int i = 0; i< DATA_SIZE; i++){
        fprintf(fp, "%d %f %f\n", clusters[r][i], data[i][0], data[i][1]);
    }
    for (int i = 0; i < M; i++){
        fprintf(fp, "%s %f %f\n", "C", centroids[r][i][0], centroids[r][i][1]);
    }
    fclose(fp);
    /*
    printf("DATA--> x1 = %lf  x2 = %lf\n", data[0][0], data[0][1]);
    printf("CENTROID--> x1 = %lf  x2 = %lf\n", centroids[0][0], centroids[0][1]);
    printf("Distance: %f\n", getDistance(0,0));
    */
    


    

}

void loadLine(int i){

    fscanf(fp, "%d", &groups[i]);


    //groups[i] = (int)buff - 48 - 1;


    fscanf(fp, "%lf", &data[i][0]);


    fscanf(fp, "%lf", &data[i][1]);
}

double getDistance(int i, int c){
    double diff_x1 = data[i][0] - centroids[r][c][0];
    double diff_x2 = data[i][1] - centroids[r][c][1];

    double sqr1 = pow(diff_x1, 2);
    double sqr2 = pow(diff_x2, 2);

    double distance = sqrt(sqr1 + sqr2);


    return distance;
}

void initialize(){
    int temp;
    for(int i = 0; i < M; i++){
        temp = rand() % DATA_SIZE;
        centroids[r][i][0] = data[temp][0];
        centroids[r][i][1] = data[temp][1];
    }
    t = 0;
}

void calculateAllDistances(){
    double mindist;
    int cluster;
    for (int i = 0; i < DATA_SIZE; i++){
        mindist = 5;
        cluster = -1;
        for (int j = 0; j < M; j++){
            distances[r][i][j] = getDistance(i,j);
            if(mindist > distances[r][i][j]){
                mindist = distances[r][i][j];
                cluster = j;
            }
        }
        clusters[r][i] = cluster;
        counter[cluster]++;
    }
}

void updateCentroids(){
    double sums[M][D];
    for (int i = 0; i < M; i++){
        sums[i][0]=0;
        sums[i][1]=0;
        //printf("%d, %d, %d\n", counter[0],counter[1],counter[2]);
    }
    for (int i = 0; i < DATA_SIZE; i++){
        sums[clusters[r][i]][0] += data[i][0];
        sums[clusters[r][i]][1] += data[i][1];
    }
    //printf("%f, %f, %f\n", sums[0][0],sums[1][0],sums[2][0]);
    //printf("%f, %f, %f\n", sums[0][1],sums[1][1],sums[2][1]);

    for (int i = 0; i<M; i++){
        //printf("OLD CENTROID--> x1 = %lf  x2 = %lf\n", centroids[r][i][0], centroids[r][i][1]);
        previous_centroids[i][0] = centroids[r][i][0];
        previous_centroids[i][1] = centroids[r][i][1];

        centroids[r][i][0] = sums[i][0] / counter[i];
        centroids[r][i][1] = sums[i][1] / counter[i];
        //printf("NEW CENTROID--> x1 = %lf  x2 = %lf\n", centroids[r][i][0], centroids[r][i][1]);
        counter[i] = 0;
    }
}

int change(){
    for(int i=0; i<M; i++){
        if(centroids[r][i][0] != previous_centroids[i][0] || centroids[r][i][1] != previous_centroids[i][1]){
            return 1;
        }
    }
    return 0;
}

double getClusteringError(){
    double error = 0;
    for (int m = 0; m < M; m++){
        for(int d = 0; d < DATA_SIZE; d++){
            error += distances[r][d][clusters[r][d]];
        }
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