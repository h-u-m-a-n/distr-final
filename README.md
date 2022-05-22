# Compiling
`mpicc main_mpi.c -o mpi.out` - mpi version  
`gcc main.c -o seq.out` - serial version

# Running 
`mpirun -np 4 ./mpi.out` - mpi  
`./seq.out` - serial