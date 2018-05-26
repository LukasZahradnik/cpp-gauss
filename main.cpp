#include <iostream>
#include "vectorclass/vectorclass.h"
#include "timer.h"

using namespace std;

#define MATRIX_SIZE 4

class GaussElimination {

public:
    static void parallel(double* M, double* b, const int size) {
        Vec4d vecs[size];

        for(int i = 0; i < size; i++) {
            vecs[i] = Vec4d().load(M + i*4);
        }

        for(int i = 0; i < size - 1; i++) {
            for(int j = i + 1; j < size; j++) {
                double k = -vecs[j][i] / vecs[i][i];
                b[j] += b[i] * k;
                vecs[j] = vecs[j] + vecs[i] * k;
            }
        }

        b[size - 1] /= vecs[size - 1][size - 1];

        for(int i = size - 2; i >= 0; i--) {
            Vec4d res = Vec4d().load(b);

            double sum = horizontal_add(vecs[i] * res) - vecs[i][i] * res[i];
            b[i] = (b[i] - sum) / vecs[i][i];
        }

        return;
    }

public:
    static void serial(double* M, double* b, const int size) {
        double* res = new double[size];
        for(int i = 0; i < size - 1; i++) {
            for(int j = i + 1; j < size; j++) {
                double k = -M[j * size + i] / M[i * size + i];
                b[j] += b[i] * k;

                for(int l = i; l < size; l++) {
                    M[j * size + l] += M[i * size + l] * k;
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
    double m[] = {
        0.02, 0.01, 0,   0,
        1,    2,    1,   0,
        0,    1,    2,   1,
        0,    0,  100, 200
    };

    double b[] = {
        0.02, 1, 4, 800
    };

    GaussElimination::parallel(m, b, MATRIX_SIZE);

    double mm[] = {
        0.02, 0.01, 0,   0,
        1,    2,    1,   0,
        0,    1,    2,   1,
        0,    0,  100, 200
    };

    double bb[] = {
        0.02, 1, 4, 800
    };

    GaussElimination::serial(mm, bb, MATRIX_SIZE);

    return 0;
}
