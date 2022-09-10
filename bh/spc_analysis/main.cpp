#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>

#include <armadillo>

#include "bitmanip.h"
#include "spc2img.h"


int main( int argc, char** argv )
{

    if (argc != 6)
    {
        printf("Error: incorrect usage.\n");

        for (int i = 0; i < argc; ++i)
            printf("argv[%d] = %s\n", i, argv[i]);

        return 1;
    }

    std::string fn_spc(argv[1]);
    bool build_img = (bool) std::atoi(argv[2]);
    bool build_flim = (bool) std::atoi(argv[3]);
    int img_x = std::atoi(argv[4]);
    int img_y = std::atoi(argv[5]);

    int status;

    std::shared_ptr<spcimage> S = std::make_shared<spcimage>();
    status = loadspc(fn_spc, S, build_img, build_flim, img_x, img_y);

    // shift macrotimes so the first photon time is zero
    arma::vec macrotime = arma::conv_to<arma::vec>::from(S->macrotime);

    // set initial time parameters assuming 80 MHz pulsed laser
    double analysistime = std::floor(arma::max(macrotime)/4e7 - 0.2);
    double macrotimeclockpulse = 2.;
    double pulserate = 8e7/macrotimeclockpulse;             // set to value of laser pulse rate
    double totaltime = arma::max(macrotime)/pulserate;      // in seconds
    double sdttime   = analysistime * pulserate;            // convert to pulses

    macrotime /= pulserate;

    std::string fn_acf = fn_spc.substr(0, fn_spc.size()-4) + "_acf.dat";

    return 0;
}
