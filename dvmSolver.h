#ifndef DVMSOLVER
#define DVMSOLVER

#include <vector>
#include <cmath>

class DvmSolver {
private:
    int N, layerSize;
    double Lx, Ly, Lz;
    double hx, hy, hz;
    double tau, alpha;

    double u_analytical(double d[3], double t) {
        return (sin(2 * M_PI * d[0] / Lx) * sin(M_PI * d[1] / Ly) * sin(M_PI * d[2] / Lz) * cos(this->alpha * t + M_PI));
    };
    double phi(double d[3]) {
        // u_analytical(d, 0);
        return (-1 * sin(2 * M_PI * d[0] / Lx) * sin(M_PI * d[1] / Ly) * sin(M_PI * d[2] / Lz));
    };
    double laplasian(double u_ijk, double u_0jk, double u_1jk, double u_i0k, double u_i1k, double u_ij0, double u_ij1);

public:
    DvmSolver(double l[3], int N);
    double solve(int i);
    ~DvmSolver() {};
};

#endif
