#include "jacobi.h"
#include "utils.h"

#include <mpi.h>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

vector<double> executar_jacobi_mpi(
    vector<vector<double>>& A,
    vector<double>& b,
    int n,
    double epsilon,
    int max_iter
) {

    int rank;
    int num_processos;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processos);



    // ==========================
    // Conversão da matriz
    // ==========================

    double* A_linear = nullptr;

    if(rank == 0) {

        A_linear = new double[n * n];

        converter_matriz_para_vetor(
            A,
            A_linear,
            n
        );
    }



    // ==========================
    // Distribuição das linhas
    // ==========================

    vector<int> linhas_por_processo(
        num_processos
    );

    vector<int> deslocamentos_linhas(
        num_processos
    );

    int base = n / num_processos;
    int resto = n % num_processos;

    int deslocamento = 0;

    for(int p = 0; p < num_processos; p++) {

        linhas_por_processo[p] = base;

        if(p < resto)
            linhas_por_processo[p]++;

        deslocamentos_linhas[p] =
            deslocamento;

        deslocamento +=
            linhas_por_processo[p];
    }



    int linhas_locais =
        linhas_por_processo[rank];



    // ==========================
    // Scatter da matriz
    // ==========================

    vector<int> sendcounts_A(
        num_processos
    );

    vector<int> displs_A(
        num_processos
    );

    for(int p = 0; p < num_processos; p++) {

        sendcounts_A[p] =
            linhas_por_processo[p] * n;

        displs_A[p] =
            deslocamentos_linhas[p] * n;
    }



    vector<double> A_local(
        linhas_locais * n
    );

    MPI_Scatterv(
        A_linear,
        sendcounts_A.data(),
        displs_A.data(),
        MPI_DOUBLE,

        A_local.data(),
        linhas_locais * n,
        MPI_DOUBLE,

        0,
        MPI_COMM_WORLD
    );



    // ==========================
    // Scatter do vetor b
    // ==========================

    vector<double> b_local(
        linhas_locais
    );

    MPI_Scatterv(
        rank == 0 ? b.data() : nullptr,

        linhas_por_processo.data(),
        deslocamentos_linhas.data(),

        MPI_DOUBLE,

        b_local.data(),
        linhas_locais,

        MPI_DOUBLE,

        0,
        MPI_COMM_WORLD
    );



    // ==========================
    // Vetores Jacobi
    // ==========================

    vector<double> x_old(
        n,
        0.0
    );

    vector<double> x_new(
        n,
        0.0
    );

    vector<double> x_local(
        linhas_locais,
        0.0
    );



    // ==========================
    // Iterações
    // ==========================

    for(int k = 0; k < max_iter; k++) {

        double erro_local = 0.0;
        double norma_local = 0.0;



        // ======================
        // Cálculo local
        // ======================

        for(int linha_local = 0;
            linha_local < linhas_locais;
            linha_local++) {

            int linha_global =
                deslocamentos_linhas[rank]
                + linha_local;

            double soma = 0.0;

            for(int j = 0; j < n; j++) {

                if(j != linha_global) {

                    soma +=
                        A_local[
                            linha_local * n + j
                        ]
                        *
                        x_old[j];
                }
            }

            x_local[linha_local] =
                (
                    b_local[linha_local]
                    - soma
                )
                /
                A_local[
                    linha_local * n
                    + linha_global
                ];

            erro_local +=
                pow(
                    x_local[linha_local]
                    -
                    x_old[linha_global],
                    2
                );

            norma_local +=
                pow(
                    x_local[linha_local],
                    2
                );
        }



        // ======================
        // Reconstrói x completo
        // ======================

        MPI_Allgatherv(

            x_local.data(),
            linhas_locais,
            MPI_DOUBLE,

            x_new.data(),

            linhas_por_processo.data(),
            deslocamentos_linhas.data(),

            MPI_DOUBLE,

            MPI_COMM_WORLD
        );



        // ======================
        // Erro global
        // ======================

        double erro_global;
        double norma_global;

        MPI_Allreduce(
            &erro_local,
            &erro_global,
            1,
            MPI_DOUBLE,
            MPI_SUM,
            MPI_COMM_WORLD
        );

        MPI_Allreduce(
            &norma_local,
            &norma_global,
            1,
            MPI_DOUBLE,
            MPI_SUM,
            MPI_COMM_WORLD
        );



        double erro = 0.0;

        if(norma_global > 0.0)
        {
            erro =
                sqrt(erro_global)
                /
                sqrt(norma_global);
        }



        // ======================
        // Convergência
        // ======================

        if(erro < epsilon) {

            if(rank == 0) {

                cout
                    << "Convergiu em "
                    << k + 1
                    << " iteracoes.\n";
            }

            break;
        }



        x_old = x_new;
    }



    if(rank == 0 &&
       A_linear != nullptr) {

        delete[] A_linear;
    }

    return x_new;
}