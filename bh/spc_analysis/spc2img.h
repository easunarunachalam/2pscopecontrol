#ifndef SPC2IMG_H
#define SPC2IMG_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <sys/stat.h>

#include <armadillo>

#include "bitmanip.h"


class spcfile
{
    public:

        // arrays for micro and macrotime, pixel, line, and frame values
        // pixel, line, and frame are only used in derived class spcimage
        arma::vec microtime;
        arma::vec macrotime;
        arma::vec pixel;
        arma::vec line;
        arma::vec frame;

        spcfile() {};
        virtual ~spcfile() {};

        virtual void set_size(int nphotons)
        {
            microtime.set_size(nphotons);
            macrotime.set_size(nphotons);
        }
};

class spcimage : public spcfile
{
    public:

        spcimage() {};
        virtual ~spcimage() {};

        void set_size(int nphotons)
        {
            spcfile::set_size(nphotons);
            pixel.set_size(nphotons);
            line.set_size(nphotons);
            frame.set_size(nphotons);
        }
};


int loadspc(std::string fn_spc, std::shared_ptr<spcfile> spc, bool build_img, bool build_flim, int img_x = 1, int img_y = 1)
{

    // check that file is ok
    std::string extension = fn_spc.substr(fn_spc.size()-4, fn_spc.size()-0);

    if (extension != ".spc")
    {
        printf("Error: input file does not have extension .spc.\n");
        return 1;
    }


    // flim image
    arma::Cube<int> flimimg(img_x, img_y, 4096/16);

    // intensity image
    arma::Mat<int>  iimg(img_x, img_y);


    // find size of file, i.e. number of photons (4 bytes/photon)
    struct stat st;
    stat(fn_spc.c_str(), &st);
    int size = st.st_size;

    if (size%4 != 0)
    {
        printf("Error: number of bytes in file not evenly divisible by 4.\n");
        return 1;
    }

    int nphotons = st.st_size / 4;
    std::cout << nphotons << " photons" << std::endl;
    

    // resize arrays for micro and macrotime
    spc->set_size(nphotons);
    
    // read in data from file

    int overflow = 0;
    int ipixel = 0,
        iline  = 0,
        iframe = 0;

    std::ifstream ifs( fn_spc, std::ios::binary ); // open in binary mode
    for (int iphoton = 0; iphoton < nphotons; ++iphoton)
    {
        std::bitset<BITS_PER_BYTE*4> bits = read_bits<4>(ifs);

        auto MT       = (int) range<BITS_PER_BYTE*4,12>(bits,0).to_ulong();
        auto ROUT     = range<BITS_PER_BYTE*4,4>(bits,12);
        auto ADC      = (int) range<BITS_PER_BYTE*4,12>(bits,16).to_ulong();
        auto MARK     = bits[28];
        auto GAP      = bits[29];
        auto MTOV     = bits[30];
        auto INVALID  = bits[31];

        if (ROUT[0] != 0) { ipixel += 1; }
        if (ROUT[1] != 0) { ipixel  = 0; iline += 1; }
        if (ROUT[2] != 0) { ipixel  = 0; iline  = 0; iframe += 1; }

        spc->macrotime(iphoton) = MT + overflow;
        spc->microtime(iphoton) = 4095 - ADC;
        spc->pixel(iphoton) = ipixel;
        spc->line(iphoton)  = iline;
        spc->frame(iphoton) = iframe;

        if (INVALID)
        {
            spc->macrotime(iphoton) = arma::datum::nan;
            spc->microtime(iphoton) = arma::datum::nan;
        }
        else
        {
            if ((ipixel < img_x) && (iline < img_y))
            {
                if (build_img)
                    iimg(ipixel, iline) += 1;
                if (build_flim)
                    flimimg(ipixel, iline, (int) spc->microtime(iphoton)/16) += 1;
            }
        }

        if (MTOV)
        {
            if (INVALID)
            {
                overflow                += MT*std::pow(2,12);
                spc->macrotime(iphoton) += MT*std::pow(2,12);
            }
            else
            {
                overflow                += std::pow(2,12);
                spc->macrotime(iphoton) += std::pow(2,12);
            }
        }
        //if (!INVALID)
            //std::cout << macrotime(iphoton) << " " << microtime(iphoton) << std::endl;

        //std::cout << bits << std::endl;
        //std::cout << 
            //MT << " " << 
            //ROUT << " " <<
            //ADC << " " <<
            //MARK << " " <<
            //GAP << " " << 
            //MTOV << " " << 
            //INVALID << " " << 
            //std::endl;

        //if (iphoton == 10) return 1;

        //break;
    }
    ifs.close();

    // remove NaN elements
    spc->macrotime = spc->macrotime.elem( arma::find_finite(spc->macrotime) );
    spc->microtime = spc->microtime.elem( arma::find_finite(spc->microtime) );

    // replace file extension spc -> csv
    std::string fn_iimg = fn_spc.substr(0, fn_spc.size()-4) + "_int.h5";
    std::string fn_flimimg = fn_spc.substr(0, fn_spc.size()-4) + "_flim.h5";

    if (build_img)
        iimg.save(fn_iimg, arma::hdf5_binary);

    if (build_flim)
        flimimg.save(fn_flimimg, arma::hdf5_binary);

    return 0;
}

#endif
