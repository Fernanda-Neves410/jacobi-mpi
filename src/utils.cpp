#include "utils.h"
#include <cmath>

void gerar_matriz(vector<vector<double>>& A,
                  vector<double>& b,
                  int n) {

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

double calcular_diferenca(
    vector<double>& resultado_seq,
    vector<double>& resultado_mpi,
    int n
) {

    double diferenca = 0.0;

    for (int i = 0; i < n; i++) {

        diferenca += abs(
            resultado_seq[i] -
            resultado_mpi[i]
        );
    }

    return diferenca;
}