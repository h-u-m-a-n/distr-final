#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MATSIZE 500
#define MASTER 0      /* taskid of first task */
#define FROM_MASTER 1 /* setting a message type */
#define FROM_WORKER 2 /* setting a message type */

void printMat(double mat[MATSIZE][MATSIZE]) {
    for (int i = 0; i < MATSIZE; i++) {
        for (int j = 0; j < MATSIZE; j++) {
            printf("%6.1f ", mat[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int numtasks,              
        taskid,               
        numworkers,           
        source,               
        dest,                 
        mtype,                
        rows,                  
        averow, extra, offset, 
        i, j, k, rc;           
    double a[MATSIZE][MATSIZE],       
        b[MATSIZE][MATSIZE],          
        c[MATSIZE][MATSIZE];          
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    if (numtasks < 2) {
        printf("Need at least two MPI tasks. Quitting...\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
        exit(1);
    }
    numworkers = numtasks - 1;

    if (taskid == MASTER) {
        for (i = 0; i < MATSIZE; i++) {
            for (j = 0; j < MATSIZE; j++) {
                a[i][j] = i + j;
            }
        }
        for (i = 0; i < MATSIZE; i++) {
            for (j = 0; j < MATSIZE; j++) {
                b[i][j] = i * j;
            }
        }

        double start = MPI_Wtime();

        averow = MATSIZE / numworkers;
        extra = MATSIZE % numworkers;
        offset = 0;
        mtype = FROM_MASTER;
        for (dest = 1; dest <= numworkers; dest++) {
            rows = (dest <= extra) ? averow + 1 : averow;
            MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&a[offset][0], rows * MATSIZE, MPI_DOUBLE, dest, mtype,
                     MPI_COMM_WORLD);
            MPI_Send(&b, MATSIZE * MATSIZE, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
            offset = offset + rows;
        }

        mtype = FROM_WORKER;
        for (i = 1; i <= numworkers; i++) {
            source = i;
            MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&c[offset][0], rows * MATSIZE, MPI_DOUBLE, source, mtype,
                     MPI_COMM_WORLD, &status);
        }

        double finish = MPI_Wtime();
        printf("Done in %f seconds.\n", finish - start);
        // printMat(a);
        // printf("\n");
        // printMat(b);
        // printf("\n");
        // printMat(c);
        
    }

    if (taskid > MASTER) {
        mtype = FROM_MASTER;
        MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&a, rows * MATSIZE, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, MATSIZE * MATSIZE, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);

        for (k = 0; k < MATSIZE; k++){
            for (i = 0; i < rows; i++) {
                c[i][k] = 0.0;
                for (j = 0; j < MATSIZE; j++)
                    c[i][k] = c[i][k] + a[i][j] * b[j][k];
            }
        }
        mtype = FROM_WORKER;
        MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(&c, rows * MATSIZE, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}