#include "dvmSolver.h"
#include <vector>
#include <array>
#include <cstdlib>

#include <string>
#include <fstream>
#include <iostream>
#include <cmath>

DvmSolver::DvmSolver(double l[3], int dots) {
    Lx = l[0];
    Ly = l[1];
    Lz = l[2];

    N = dots;
    hx = Lx / dots;
    hy = Ly / dots;
    hz = Lz / dots;

    tau = 1.0 / 1000;
    alpha = M_PI * sqrt((4/(l[0]*l[0]) + 1/(l[1]*l[1]) + 1/(l[2]*l[2])));
}

double DvmSolver::laplasian(double u_ijk, double u_0jk, double u_1jk, double u_i0k, double u_i1k, double u_ij0, double u_ij1) {
    double dx = (u_0jk - 2*u_ijk + u_1jk) / (hx * hx);
    double dy = (u_i0k - 2*u_ijk + u_i1k) / (hy * hy);
    double dz = (u_ij0 - 2*u_ijk + u_ij1) / (hz * hz);
    return dx + dy + dz;
}

double DvmSolver::solve(int steps) {

    #pragma dvm array distrubute [block][block][block]
    double u0[N+1][N+1][N+1];    
    // shadow
    #pragma dvm array align([i][j][k] with u0[i][j][k]), shadow[1:1]
    double u1[N+1][N+1][N+1];
    #pragma dvm array align([i][j][k] with u0[i][j][k])
    double u2[N+1][N+1][N+1];
    double max_eps = 0;

    #pragma dvm parallel([i][j][k] on u0[i][j][k])
    for (int i = 0; i <= N; i++) {
        for (int j = 0; j <= N; j++) {
            for (int k = 0; k <= N; k++) {
                double dot[3] = {hx*i, hy*j, hz*k};
                u0[i][j][k] = phi(dot);
            }
        }
    }

    // find error u0
    // #pragma dvm parallel([i][j][k] on u0[i][j][k]) reduction(max(max_eps))
    // for (int i = 0; i <= N; i++) {
    //     for (int j = 0; j <= N; j++) {
    //         for (int k = 0; k <= N; k++) {
    //             double dot[3] = {hx*i, hy*j, hz*k};
    //             eps = fabs(u0[i][j][k] - u_analytical(dot, 0));
    //             if (eps > max_eps) {
    //                 max_eps = eps;
    //             }
    //         }
    //     }
    // }
    // std::cout << max_eps << std::endl;
    // max_eps = 0;

    // fill borders 1 and count inner 1
    // 1r y, 1r z, periodic x
#pragma dvm region
{
    #pragma dvm parallel([i][j][k] on u1[i][j][k]) shadow_renew(u0)
    for (int i = 0; i <= N; i++) {
        for (int j = 0; j <= N; j++) {
            for (int k = 0; k <= N; k++) {
                if (j == 0 || j == N || k == 0 || k == N) {
                    u1[i][j][k] = 0;
                } else
                if (i == 0 || i == N) {
                    double dot[3] = {0,j*hy,k*hz};
                    double u_here = u_analytical(dot, tau);
                    u1[i][j][k] = u_here;
                } else {
                    u1[i][j][k] = u0[i][j][k] + laplasian(u0[i][j][k], u0[i-1][j][k], u0[i+1][j][k], u0[i][j-1][k], u0[i][j+1][k], u0[i][j][k-1], u0[i][j][k+1])*tau*tau/2;
                }
            }
        }
    }
    
    // find error u1
    // #pragma dvm parallel([i][j][k] on u1[i][j][k]) reduction(max(max_eps))
    // for (int i = 0; i <= N; i++) {
    //     for (int j = 0; j <= N; j++) {
    //         for (int k = 0; k <= N; k++) {
    //             double dot[3] = {hx*i, hy*j, hz*k};
    //             eps = fabs(u1[i][j][k] - u_analytical(dot, tau));
    //             if (eps > max_eps) {
    //                 max_eps = eps;
    //             }
    //         }
    //     }
    // }
    // std::cout << max_eps << std::endl;
    // max_eps = 0;

    for (int t = 2; t < steps; t++) {
        // fill borders and count inner 2 (t)
        // 1r y, 1r z, periodic x
        #pragma dvm parallel([i][j][k] on u2[i][j][k]) shadow_renew(u1)
        for (int i = 0; i <= N; i++) {
            for (int j = 0; j <= N; j++) {
                for (int k = 0; k <= N; k++) {
                    if (j == 0 || j == N || k == 0 || k == N) {
                        u2[i][j][k] = 0;
                    } else
                    if (i == 0 || i == N) {
                        double dot[3] = {0,j*hy,k*hz};
                        double u_here = u_analytical(dot, tau*t);
                        u2[i][j][k] = u_here;
                    } else {
                        u2[i][j][k] = 2 * u1[i][j][k] - u0[i][j][k] + tau * tau * laplasian(u1[i][j][k], u1[i-1][j][k], u1[i+1][j][k], u1[i][j-1][k], u1[i][j+1][k], u1[i][j][k-1], u1[i][j][k+1]);
                    }
                }
            }
        }

        // find error 2 (t) and rearrange buffers
        if (t == steps - 1) {
            #pragma dvm parallel([i][j][k] on u2[i][j][k]) reduction(max(max_eps))
            for (int i = 0; i <= N; i++) {
                for (int j = 0; j <= N; j++) {
                    for (int k = 0; k <= N; k++) {
                        double dot[3] = {hx*i, hy*j, hz*k};
                        double eps = fabs(u2[i][j][k] - u_analytical(dot, tau));
                        if (eps > max_eps) {
                            max_eps = eps;
                        }
                        u0[i][j][k] = u1[i][j][k];
                        u1[i][j][k] = u2[i][j][k];
                    }
                }
            }
            // std::cout << max_eps << std::endl;
            // if (t < steps-1) max_eps = 0;
        }
    }
}
    #pragma dvm get_actual(max_eps)
    return max_eps;
}