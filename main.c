#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MATSIZE 3
void printMat(double mat[MATSIZE][MATSIZE])
{
    for (int i = 0; i < MATSIZE; i++)
    {
        for (int j = 0; j < MATSIZE; j++)
        {
            printf("%6.1f ", mat[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    double a[MATSIZE][MATSIZE],        /* matrix A to be multiplied */
        b[MATSIZE][MATSIZE],           /* matrix B to be multiplied */
        c[MATSIZE][MATSIZE];           /* result matrix C */
       
    for (int i = 0; i < MATSIZE; i++){
            for (int j = 0; j < MATSIZE; j++){
                a[i][j] = i + j;
            }
    }
    for (int i = 0; i < MATSIZE; i++){
        for (int j = 0; j < MATSIZE; j++){
            b[i][j] = i * j;
        }
    }
    
    clock_t t;
    t = clock();    

     

    for (int k = 0; k < MATSIZE; k++) {
        for (int i = 0; i < MATSIZE; i++) {
            c[i][k] = 0.0;
            for (int j = 0; j < MATSIZE; j++) {
                c[i][k] = c[i][k] + a[i][j] * b[j][k];
            }
        }
    }

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("Done in %f seconds.\n", time_taken);

}