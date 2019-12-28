#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int upper = 2;
    int lower = -2;
    srand(time(0));

    for(int i = 0; i<100; i++) {
        // Return random float between [-2,2]
        float random_num = ((float)rand() /((float)RAND_MAX /(upper - lower))) + lower;
        printf("%f\n", random_num);
    }
}