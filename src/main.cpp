#include "jacobi.h"
#include "utils.h"

#include <mpi.h>
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);

    int rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double epsilon = 1e-6;
    int max_iter = 100000;
    int n = 2500;


    vector<vector<double>> A(n,vector<double>(n, 0.0));

    vector<double> b(n,0.0);

    gerar_matriz(A, b, n);

    vector<double> resultado_seq;

    double tempo_seq = 0.0;

    if(rank == 0) {

        auto inicio_seq = chrono::high_resolution_clock::now();

        resultado_seq = jacobi_sequencial(A,b,max_iter,epsilon);

        auto fim_seq = chrono::high_resolution_clock::now();

        chrono::duration<double> tempo_gasto_seq = fim_seq - inicio_seq;

        tempo_seq = tempo_gasto_seq.count();

        cout
            << "Tempo Sequencial: "
            << tempo_seq
            << " s\n\n";
    }

    MPI_Barrier(
        MPI_COMM_WORLD
    );

    auto inicio_mpi = chrono::high_resolution_clock::now();

    vector<double> resultado_mpi = executar_jacobi_mpi(A,b,n,epsilon,max_iter);

    auto fim_mpi = chrono::high_resolution_clock::now();

    if(rank == 0) {

        chrono::duration<double> tempo_gasto_mpi = fim_mpi - inicio_mpi;

        double tempo_mpi = tempo_gasto_mpi.count();

        cout
            << "Tempo MPI: "
            << tempo_mpi
            << " s\n\n";

        double speedup = tempo_seq / tempo_mpi;

        cout
            << "Speedup: "
            << speedup
            << endl;

        double diferenca = calcular_diferenca(resultado_seq,resultado_mpi,n);

        cout
            << "Diferenca Seq vs MPI: "
            << diferenca
            << endl;
    }

    MPI_Finalize();

    return 0;
}