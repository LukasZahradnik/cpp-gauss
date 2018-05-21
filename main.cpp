#include <iostream>
#include "vectorclass/vectorclass.h"

using namespace std;

#define MATRIX_SIZE 4

class GaussElimination {

public:
    static void parallel(double* M, double* b, const int size) {
        return;
    }

public:
    static void serial(double* M, double* b, const int size) {
        double* res = new double[size];

        for(int i = 0; i < size - 1; i++) {
            for(int j = i + 1; j < size; j++) {
                double k = -M[j * size + i] / M[i * size + i];
                b[j] = b[i] * k + b[j];

                for(int l = i; l < size; l++) {
                    M[j * size + l] = M[i * size + l] * k + M[j * size + l];
                }
            }
        }

        b[size - 1] /= M[size * size - 1];
        for(int i = size - 2; i >= 0; i--) {
            double sum = 0;
            for(int j = i + 1; j < size; j++) {
                sum += M[i * size + j] * b[j];
            }

            b[i] = (b[i] - sum) / M[i * size + i];
        }

        return;
    }

};

int main()
{
    double* m = new double[MATRIX_SIZE * MATRIX_SIZE] {
        0.02, 0.01, 0,   0,
        1,    2,    1,   0,
        0,    1,    2,   1,
        0,    0,  100, 200
    };

    double* b = new double[MATRIX_SIZE] {
        0.02, 1, 4, 800
    };

    GaussElimination::serial(m, b, MATRIX_SIZE);

    return 0;
}
