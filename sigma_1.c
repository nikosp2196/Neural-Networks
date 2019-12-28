#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(){
    FILE* fptr;
    char buf[50];
    float number = 123.90342;
    
    gcvt(number,6, buf);
    strcat(buf, "\n");
    fptr = fopen("data_1.txt", "w");
    fputs(buf, fptr);
    fputs("Molly", fptr);
    fclose(fptr);
}