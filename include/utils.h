#pragma once

#include <vector>

using namespace std;

// Gera uma matriz diagonal dominante
void gerar_matriz(vector<vector<double>>& A, vector<double>& b, int n);

// Calcula diferença entre resultado sequencial e MPI
double calcular_diferenca(vector<double>& resultado_seq,
                          vector<double>& resultado_mpi,
                          int n);