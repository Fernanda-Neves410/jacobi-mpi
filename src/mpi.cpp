#include "jacobi.h"
#include "utils.h"

#include <mpi.h>
#include <vector>
#include <cmath>

using namespace std;

vector<double> executar_jacobi_mpi(
    vector<vector<double>>& A,
    vector<double>& b,
    int n,
    int max_iter,
    double epsilon
) {

    int rank;
    int num_processos;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processos);


    int n = n;


    // Converte a matriz para formato linear
    double* A_linear = nullptr;

    if(rank == 0) {

        A_linear = new double[n * n];

        converter_matriz_para_vetor(
            A,
            A_linear,
            n
        );
    }



    // Quantidade de linhas deste processo
    int linhas_locais = n / num_processos;

    if(rank == num_processos - 1) {

        linhas_locais += n % num_processos;
    }



    // Dados locais
    vector<double> A_local(
        linhas_locais * n
    );

    vector<double> b_local(
        linhas_locais
    );



    // Vetores do método de Jacobi
    vector<double> x_old(
        n,
        0.0
    );

    vector<double> x_new(
        n,
        0.0
    );



    // TODO:
    // MPI_Scatterv para distribuir A_local

    // TODO:
    // MPI_Scatterv para distribuir b_local



    for(int k = 0; k < max_iter; k++) {

        double erro_local = 0.0;

        double norma_local = 0.0;



        // TODO:
        // calcular apenas as linhas locais



        // TODO:
        // reconstruir x_new completo



        // TODO:
        // calcular erro global



        // TODO:
        // verificar convergência



        x_old = x_new;
    }



    if(rank == 0 && A_linear != nullptr) {

        delete[] A_linear;
    }

    return x_new;
}