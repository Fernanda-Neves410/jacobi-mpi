#include "utils.h"

#include <cmath>

using namespace std;


// Gera uma matriz diagonal dominante
void gerar_matriz(
    vector<vector<double>>& A,
    vector<double>& b,
    int n
) {

    for (int i = 0; i < n; i++) {

        double soma_linha = 0.0;

        for (int j = 0; j < n; j++) {

            if (i != j) {

                A[i][j] = 1.0;

                soma_linha += A[i][j];
            }
        }

        A[i][i] = soma_linha + 1.0;

        b[i] = (i + 1) * 2.0;
    }
}


// Converte matriz 2D para vetor linear
void converter_matriz_para_vetor(
    vector<vector<double>>& A,
    double* A_linear,
    int n
) {

    for (int linha = 0; linha < n; linha++) {

        for (int coluna = 0; coluna < n; coluna++) {

            A_linear[linha * n + coluna] =
                A[linha][coluna];
        }
    }
}


// Calcula diferença entre resultado sequencial e MPI
double calcular_diferenca(
    vector<double>& resultado_seq,
    vector<double>& resultado_mpi,
    int n
) {

    double diferenca = 0.0;

    for (int i = 0; i < n; i++) {

        diferenca += fabs(resultado_seq[i] - resultado_mpi[i]);
    }

    return diferenca;
}