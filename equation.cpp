#include <iostream>
#include <fstream>
#include <cstdlib>
#include "dvmSolver.h"

int main(int argc, char *argv[])
{
    int dots;
    double L;
    std::ofstream output;

    // parse arguments
    // arguments -- dots, L (assume Lx=Ly=Lz) (1 == 1, 2 == Pi), fname
    if (argc > 3) {
        dots = std::atoi(argv[1]);
        int c = std::atoi(argv[2]);
        if (c == 2) L = M_PI;
        else L = 1;
        if (dots <= 0) dots = 2;
        output.open(argv[3], std::ios_base::app);
    } else {
        dots = 2;
        L = 1;
    }

    double l[3] = {L,L,L};
    double error;
    double begin, time, maxTime;
    
    DvmSolver solver(l, dots);
    error = solver.solve(20);
    // if (!rank) output << dots << " " << size << " " << error << " " << time << std::endl;
    std::cout << error << std::endl;
    output.close();

    return 0;
}
