#include "jacobi.h"

#include <mpi.h>
#include <vector>
#include <cmath>

using namespace std;

vector<double> executar_jacobi_mpi(
    vector<vector<double>>& A,
    vector<double>& b,
    int max_iter,
    double epsilon
) {

    int rank;
    int num_processos;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processos);

    int n = A.size();

    vector<double> x_old(n, 0.0);
    vector<double> x_new(n, 0.0);



    // Distribuição simples das linhas
    int linhas_por_processo = n / num_processos;

    int inicio = rank * linhas_por_processo;

    int fim;

    if(rank == num_processos - 1)
        fim = n;
    else
        fim = inicio + linhas_por_processo;



    for(int k = 0; k < max_iter; k++) {

        double erro_local = 0.0;
        double norma_local = 0.0;


        // Cada processo calcula suas linhas
        for(int i = inicio; i < fim; i++) {

            double soma = 0.0;

            for(int j = 0; j < n; j++) {

                if(j != i)
                    soma += A[i][j] * x_old[j];
            }

            x_new[i] =
                (b[i] - soma) / A[i][i];

            erro_local +=
                pow(x_new[i] - x_old[i], 2);

            norma_local +=
                pow(x_new[i], 2);
        }



        // trocar partes de x_new entre processos



        // calcular erro global



        // verificar convergência



        x_old = x_new;
    }

    return x_new;
}