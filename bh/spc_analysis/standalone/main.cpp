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

    return 0;
}
