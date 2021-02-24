#include <cstdlib>
#include <iostream>
#include <gmpxx.h>
#include "mpi.h"

void send_mpz_num(mpz_class num)
{
    std::string as_str = num.get_str(); 
    long len = as_str.size();

    // Sending buffer size
    MPI_Send(&len, 1, MPI_LONG,
              0, 0, MPI_COMM_WORLD); 

    // Sending buffer
    MPI_Send(as_str.c_str(), len, MPI_BYTE, 
              0, 0, MPI_COMM_WORLD); 
    return;
}

mpz_class recv_mpz_num() 
{
    MPI_Status status;
    long len = 0;    

    MPI_Recv(&len, 1, MPI_LONG, 
              MPI_ANY_SOURCE, 0, 
              MPI_COMM_WORLD, 
               &status); 
    std::string num(len, '0');

    MPI_Recv(&num[0], len, MPI_BYTE, 
              MPI_ANY_SOURCE, 0, 
              MPI_COMM_WORLD, 
              &status);
    
    return mpz_class(num); 
}


mpz_class calc_e(int from, int to)
{
    mpz_class fact = 1;

    for (int i = 1; i <= from; ++i)
        fact *= i;

    mpz_class exp = 0;
    for(int i = from; i <= to; ++i)
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
        mpz_class eu = 0;        
        for (int i = 1; i < np; i++) { 
            eu += recv_mpz_num(); 
        }
        std::cout << "e = " << eu << std::endl;
    }
    else
    {
        int from = (pid - 1) * depth;
        int to = pid * depth - 1;
        mpz_class part_sum = calc_e(from, to);
        send_mpz_num(part_sum);
    }
    MPI_Finalize();
    return 0;
}
