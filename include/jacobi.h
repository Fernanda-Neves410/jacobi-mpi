#pragma once

#include <vector>

using namespace std;

vector<double> jacobi_sequencial(vector<vector<double>>& A, vector<double>& b, 
                                int max_iter, double epsilon);

vector<double> executar_jacobi_mpi(vector<vector<double>>& A,vector<double>& b,
                                    int n, double epsilon, int max_iter);