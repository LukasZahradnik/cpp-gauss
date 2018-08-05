#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <cstring>
#include <cstdlib>

#define MATRIX_SIZE 2048

using namespace std;


class Timer {

public:
    Timer(const char* header ="")
        : beg_(clock_::now()), header(header) {}
    ~Timer() {
            double e = elapsed();
            cout << header << ": " << e << " micros" << endl;
    }
    void reset() { beg_ = clock_::now(); }
    double elapsed() const {
        return std::chrono::duration_cast<second_>
            (clock_::now() - beg_).count(); }

private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1,1000000> >
                second_;
    std::chrono::time_point<clock_> beg_;
    const char* header;

};


double* rnd_array(int size) {
        double* a = new double[size];
        random_device rd;
        default_random_engine dre(rd());
        uniform_real_distribution<double> uid(0.0, 1.0);

        generate(a, a + size, [&] () { return uid(dre); });
        return a;
}


class GaussElimination {

public:
    static void serial(double* M, double* b, const int size) {
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

public:
    static void parallel(double* M, double* b, const int size) {
        double k;
        int i, j, l;

        for(i = 0; i < size - 1; i++) {
            #pragma omp parallel for shared(M, b) private(k, j, l)
            for(j = i + 1; j < size; j++) {
                double k = -M[j * size + i] / M[i * size + i];
                b[j] += b[i] * k;

                #pragma omp simd
                for(l = i; l < size; l++) {
                    M[j * size + l] += M[i * size + l] * k;
                }
            }
        }

        b[size - 1] /= M[size * size - 1];
        for(i = size - 2; i >= 0; i--) {
            double sum = 0;

            #pragma omp simd
            for(j = i + 1; j < size; j++) {
                sum += M[i * size + j] * b[j];
            }

        }
    }

};

int main()
{
    for(int i = 0; i < 5; i++) {

        double* parM = rnd_array(MATRIX_SIZE * MATRIX_SIZE);
        double* parB = rnd_array(MATRIX_SIZE);

        double* serM = rnd_array(MATRIX_SIZE * MATRIX_SIZE);
        double* serB = rnd_array(MATRIX_SIZE);


        {
            Timer t("PARALLEL\t");
            GaussElimination::parallel(parM, parB, MATRIX_SIZE);
        }

        {
            Timer t("SERIAL\t\t");
            GaussElimination::serial(serM, serB, MATRIX_SIZE);
        }

    }

    return 0;
}
