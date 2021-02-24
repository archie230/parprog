#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "mpi.h"

double calc_e(int from, int to)
{
    long long fact = 1;

    for (int i = 1; i <= from; ++i)
        fact *= i;

    int i;
    double exp = 0;
    for(i = from; i <= to; ++i)
    {
        exp += 1.0/fact;
        fact *= (i + 1);
    }
    return exp;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Usage ./" << argv[0] <<  "<depth for each worker>" << std::endl;
        return 0;    
    }
    int depth = std::stoi(std::string(argv[1]));

    MPI_Init(NULL, NULL);
    int pid, np;

    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    if (pid == 0)
    {
        double tmp;
        MPI_Status status;
        float eu = 0;
        for (int i = 1; i < np; i++) { 
            MPI_Recv(&tmp, 1, MPI_DOUBLE, 
                     MPI_ANY_SOURCE, 0, 
                     MPI_COMM_WORLD, 
                     &status); 
            eu += tmp; 
        }
        std::cout << "e = " << std::setprecision(9) << eu << std::endl;
    }
    else
    {
        int from = (pid - 1) * depth;
        int to = pid * depth - 1;
        double part_sum = calc_e(from, to);
        MPI_Send(&part_sum, 1, MPI_DOUBLE, 
                 0, 0, MPI_COMM_WORLD); 
    }
    MPI_Finalize();
    return 0;
}
