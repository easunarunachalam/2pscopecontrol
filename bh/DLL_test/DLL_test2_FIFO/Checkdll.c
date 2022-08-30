// Checkdll.cpp : Point-and-shoot FLCS acquisitions
//

#include <conio.h>
#include <direct.h>
#include <io.h>
#include <math.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "Dcc_def.h"
#include "Gvd_def.h"
#include "Spcm_def.h"


#define MAX_LOADSTRING 100

#ifndef BZERO
#define BZERO(buf) memset(&(buf),0,sizeof(buf))
#define BCOPY(buf1,buf2) memcpy(&(buf1), &(buf2), sizeof(buf2))
#endif


char spc_ini_fname[255] = "C:\\Users\\TCSPC\\Documents\\GitHub\\2pscopecontrol\\bh\\config\\spcm.ini";
char dcc_ini_fname[255] = "C:\\Users\\TCSPC\\Documents\\GitHub\\2pscopecontrol\\bh\\config\\dcc100.ini";
char gvd_ini_fname[255] = "C:\\Users\\TCSPC\\Documents\\GitHub\\2pscopecontrol\\bh\\config\\gvd120.ini";

short  test_fill_state(void);
short  save_photons_in_file(void);
static void init_fifo_measurement(void); // init actions same for part 4 and 5

int initialize_SPC_modules();
int initialize_DCC_modules();
int initialize_GVD_modules();
void close_all_modules();
int read_rates(int);

// SPC modules
int total_no_of_spc, no_of_active_spc, mod_active[MAX_NO_OF_SPC];
SPCModInfo spc_mod_info[MAX_NO_OF_SPC];
SPCdata spc_data;
SPC_EEP_Data spc_eep_data;
SPCMemConfig  spc_mem_info;
rate_values rates[MAX_NO_OF_SPC];
short spc_state, armed, init_status, module_type, fifo_stopt_possible, first_write, spc_error, force_use, spc_act_mod, module_type, block_length, fifo_type, spc_ret, ret1, spc_ret;
short sync_state[MAX_NO_OF_SPC], mod_state[MAX_NO_OF_SPC];
int max_block_no, max_page, max_curve;
unsigned long page_size, fifo_size, photons_to_read, max_ph_to_read, words_to_read, words_left, words_in_buf, max_words_in_buf, current_cnt;
unsigned short *buffer, *ptr, fpga_version;
unsigned int spc_header;


// DCC modules
int total_no_of_dcc, no_of_active_dcc, hardware;
short dcc_act_mod, dcc_error, overload_state, curr_lmt_state, dcc_ret, in_use[MAX_NO_OF_DCC];
DCCModInfo dcc_mod_info[MAX_NO_OF_DCC];
DCCdata dcc_data;
DCC_EEP_Data dcc_eep_data;

// GVD modules
GVDModInfo gvd_mod_info[MAX_NO_OF_GVD];
GVDdata gvd_data;
GVD_EEP_Data gvd_eep_data;
GVDScanInfo  scan_info;
short gvd_error, gvd_ret, gvd_ret1, gvd_init_status, gvd_state, gvd_trigger, lim_scan, park_info;
int gvd_hardware, gvd_act_mod;
int total_no_of_gvd, no_of_active_gvd, gvd_in_use[MAX_NO_OF_GVD];
float park_offset_x, park_offset_y, fval;
unsigned short FPGAversion;
unsigned long fcnt;
char gvd_error_string[128];

int init_ok, ret, ncoords, ipoint;
char input_char;
float scanner_target_x, scanner_target_y;
float collection_time = 0.5;

char data_dir_path[255] = "C:\\Users\\TCSPC\\Desktop\\2022-08-20-21 -- FCS -- yEA68\\with flash\\";
char coords_fname[255] = "points.dat";
char coords_fpath[255];
char phot_basename[255] = "point_%03d.spc";
char phot_fpath[255], phot_fpath_[255];
short SPC_ACTIVE_MOD = 0;

int main()
{
    /*
    If initialization of all modules does not occur properly, disable outputs and shut down
    */
    if ((initialize_SPC_modules() < 0) || (initialize_DCC_modules() < 0) || (initialize_GVD_modules() < 0)){
        DCC_enable_outputs(dcc_act_mod, 0);
        close_all_modules();
        return -1;
    }
    /*
    Otherwise, continue on...
    */

    // Outputs are disabled after DCC_init; enable outputs
    DCC_enable_outputs(dcc_act_mod, 1);
    Sleep(1000);

    // test for overload and cooler current limit
    DCC_get_overload_state(dcc_act_mod, &overload_state);
    DCC_get_curr_lmt_state(dcc_act_mod, &curr_lmt_state);

    // and shut down if there is an issue
    if ((overload_state != 0) || (curr_lmt_state != 0)) {
        printf("\nWarning: current limit reached or detector overload. Shutting down...\n");
        DCC_enable_outputs(dcc_act_mod, 0);  // disable outputs at the end
        close_all_modules();
        return -2;
    }

    // read rates for a few seconds to confirm things look normal
    ret = read_rates(3);
    if (ret < 0) {
        return ret;
    }

    // open file with coordinates for FCS acquisitions
    strcpy(coords_fpath, data_dir_path);
    strcat(coords_fpath, coords_fname);
    printf("\nReading coordinate file: %s\n", coords_fpath);

    FILE* coords_fptr = fopen(coords_fpath, "r");
    if (coords_fptr == NULL) {
        printf("\nError: unable to open coordinate file %s\n", coords_fname);
        fclose(coords_fptr);
        DCC_enable_outputs(dcc_act_mod, 0);  // disable outputs at the end
        close_all_modules();
        return -3;
    }
    else {

        printf("Ready to begin FCS acquisition. Open microscope shutter and then press `s` to start...\n");
        if (getchar() != 's') {
            printf("Did not receive start signal. Shutting down.\n");
            fclose(coords_fptr);
            DCC_enable_outputs(dcc_act_mod, 0);  // disable outputs at the end
            close_all_modules();
            return 0;
        }

        ipoint = 0;
        do {
            ncoords = fscanf(coords_fptr, "%f %f", &scanner_target_x, &scanner_target_y);
            if (ncoords != 2) {
                break;
            }
            printf("point %d    scanner target: x = %02.3f  y = %2.4f   ", ipoint, scanner_target_x, scanner_target_y);

            // park beam at specified location and confirm that scanner has moved
            gvd_ret = GVD_park_beam(gvd_act_mod, 1, 0, &scanner_target_x, &scanner_target_y);
            gvd_ret = GVD_get_parameter(gvd_act_mod, PARK_OFFS_X, &park_offset_x);
            gvd_ret = GVD_get_parameter(gvd_act_mod, PARK_OFFS_Y, &park_offset_y);
            // printf("park offset:   x = %lf   y = %lf   ", park_offset_x, park_offset_y);

            /*
            setup for FIFO measurement
            */

            // set filename for spc file
            strcpy(phot_fpath_, data_dir_path);
            strcat(phot_fpath_, phot_basename);
            sprintf(phot_fpath, phot_fpath_, ipoint++);
            printf("spc file = %s   ", phot_fpath);


            // 0 - set SPC parameters (required for memory fill to work on measurements 2, 3, 4, ...
            spc_ret = SPC_set_parameters(spc_act_mod, &spc_data);

            //  1 - SPC memory must be configured before the measurement
            short no_of_routing_bits = 0;  // simple 1 dimensional measurement
            // configure memory on all modules
            spc_ret = SPC_configure_memory(-1, spc_data.adc_resolution, no_of_routing_bits,
                &spc_mem_info);
            if (spc_ret == -SPC_BAD_FUNC)
                // for some modes ( for example scan modes)  you can't configure but only get current configuration
                spc_ret = SPC_configure_memory(spc_act_mod, -1, no_of_routing_bits, &spc_mem_info);
            if (spc_mem_info.maxpage == 0) {
                printf("\nError: spc_mem_info.maxpage = 0 (line %d).\n", __LINE__);
                fclose(coords_fptr);
                DCC_enable_outputs(dcc_act_mod, 0);  // disable outputs at the end
                close_all_modules();
                return -1;
            }

            max_block_no = spc_mem_info.max_block_no;
            max_page = spc_mem_info.maxpage;
            max_curve = max_block_no / max_page;
            block_length = spc_mem_info.block_length;

            page_size = spc_mem_info.blocks_per_frame * spc_mem_info.frames_per_page * block_length;

            //buffer = (unsigned short*)realloc(buffer, max_page * page_size * no_of_active_spc * sizeof(unsigned short));
            //memset(buffer, 0, sizeof buffer);

            //  3 - measurement  page must be set on all modules
            short meas_page = 0;
            short offset_value = 0;
            SPC_set_page(spc_act_mod, meas_page);

            //  2 - measured blocks in SPC memory must be filled (cleared )
            // spc_ret=SPC_fill_memory(spc_act_mod, 0, meas_page, offset_value);
            spc_ret = SPC_fill_memory(-1, -1, meas_page, offset_value);
            if (spc_ret > 0) {
                // fill started but not yet finished
                spc_ret = test_fill_state();
            }
            if (spc_ret < 0) { // errors during memory fill
                printf("\nError: error encountered during memory fill (line %d).\n", __LINE__);
                fclose(coords_fptr);
                DCC_enable_outputs(dcc_act_mod, 0);  // disable outputs at the end
                close_all_modules();
                return -1;
            }

            //  4 - rates should  be cleared, sync state can be checked
            for (int i = 0; i < MAX_NO_OF_SPC; i++) {
                if (mod_active[i]) {
                    SPC_clear_rates(i);  // it is needed one time only
                    SPC_get_sync_state(i, &sync_state[i]);
                }
            }

            /*
            FIFO measurement
            */
            init_fifo_measurement();

            buffer = (unsigned short*)realloc(buffer, max_words_in_buf * sizeof(unsigned short));
            //if (buffer) {
            //    puts("Zeroing out buffer");
            //    memset(buffer, 0, sizeof buffer);
            //}
            

            photons_to_read = 20000000;
            if (fifo_type == FIFO_48)
                words_to_read = 3 * photons_to_read;
            else
                words_to_read = 2 * photons_to_read;

            words_left = words_to_read;

            //SPC_set_parameter( -1, TRIGGER, 2 );   // trigger active high

            clock_t t;
            t = clock();

            spc_ret = SPC_start_measurement(spc_act_mod);

            while (!spc_ret) {
                // printf("in loop, spc_ret=%d\n", spc_ret);
                // now test SPC state and read photons
                SPC_test_state(spc_act_mod, &spc_state);
                // user must provide safety way out from this loop
                //    in case when trigger will not occur or required number of photons
                //          cannot be reached

                if (words_left > max_words_in_buf - words_in_buf)
                    // limit current_cnt to the free space in buffer
                    current_cnt = max_words_in_buf - words_in_buf;
                else
                    current_cnt = words_left;
                ptr = (unsigned short*)&buffer[words_in_buf];

                if (spc_state & SPC_ARMED) {  //  system armed
                    if (spc_state & SPC_FEMPTY)
                        continue;  // Fifo is empty - nothing to read

                      // before the call current_cnt contains required number of words to read from fifo
                    spc_ret = SPC_read_fifo(spc_act_mod, &current_cnt, ptr);
                    // after the call current_cnt contains number of words read from fifo

                    words_left -= current_cnt;
                    if (words_left <= 0)
                        break;   // required no of photons read already

                    if (spc_state & SPC_FOVFL) {
                        // Fifo overrun occured
                        //  - macro time information after the overrun is not consistent
                        //    consider to break the measurement and lower photon's rate
                        break;
                    }
                    // during the running measurement it is possible to check how occupied is FIFO
                    //  by calling SPC_get_fifo_usage function

                    words_in_buf += current_cnt;
                    if (words_in_buf == max_words_in_buf) {
                        // your buffer is full, but photons are still needed
                        // save buffer contents in the file and continue reading photons
                        spc_ret = save_photons_in_file();
                        words_in_buf = 0;
                    }
                }
                else {
                    if (fifo_stopt_possible && (spc_state & SPC_TIME_OVER) != 0) {
                        // measurement stopped after collection time
                        // read rest photons from the fifo
                        // before the call current_cnt contains required number of words to read from fifo
                        spc_ret = SPC_read_fifo(spc_act_mod, &current_cnt, ptr);
                        // after the call current_cnt contains number of words read from fifo

                        words_left -= current_cnt;
                        words_in_buf += current_cnt;
                        break;
                    }
                }
            }

            // SPC_stop_measurement should be called even if the measurement was stopped after collection time
            // in order to reset DLL internal variables
            SPC_stop_measurement(spc_act_mod);

            // note time elapsed
            t = clock() - t;
            double elapsed_time = ((double)t) / CLOCKS_PER_SEC; // in seconds
            printf("time = %3.2lf sec\n", elapsed_time);

            if (words_in_buf > 0) {
                // if (ipoint == 5){
                ret = save_photons_in_file();
                words_in_buf = 0;
                if (ret < 0)
                {
                    printf("\nError: issue writing photons to file.");
                    fclose(coords_fptr);
                    DCC_enable_outputs(dcc_act_mod, 0);  // disable outputs at the end
                    close_all_modules();
                    
                    return -1;
                }
                //}
            }
            
        } while (ncoords == 2);
    }

    fclose(coords_fptr);

    DCC_enable_outputs(dcc_act_mod, 0);  // disable outputs at the end

    close_all_modules();
	getchar();

    return 0;
}


int initialize_SPC_modules() {
    /*
    Initialize SPC modules
    */

    if ((spc_ret = SPC_init(spc_ini_fname)) < 0) {
        if (-spc_ret < SPC_WRONG_ID || -spc_ret == SPC_WRONG_LICENSE || -spc_ret >= SPC_NO_LICENSE) {
            char text[255];
            SPC_get_error_string(spc_ret, text, 200);
            printf(text);
            return spc_ret;  // fatal error, maybe wrong ini file or DLL not registered
        }
    }

    total_no_of_spc = no_of_active_spc = 0;
    for (int i = 0; i < MAX_NO_OF_SPC; i++) {
        SPC_get_module_info(i, (SPCModInfo*)&spc_mod_info[i]);
        if (spc_mod_info[i].module_type != M_WRONG_TYPE) {
            total_no_of_spc++;
        }
        if (spc_mod_info[i].init == INIT_SPC_OK) {
            no_of_active_spc++; mod_active[i] = 1;
            SPC_get_eeprom_data(0, &spc_eep_data);
            printf("SPC-%i SN: %s Bus: %i Slot: %i \n", spc_mod_info[i].module_type, spc_eep_data.serial_no, spc_mod_info[i].bus_number, spc_mod_info[i].slot_number);
            spc_act_mod = i;
        }
        else {
            mod_active[i] = 0;
        }
    }

    if (!no_of_active_spc) {
        // no active SPC
        printf("\nError: no active SPC modules were found.\n");
        return -1;
    }
    else {
        printf("Total number of SPCs detected: %d\n", no_of_active_spc);

        module_type = spc_mod_info[0].module_type;
        printf("Module type set as: SPC%i\n", spc_mod_info[0].module_type);
    }

    /* if ((mod_active[0] == 1) && (mod_active[1] == 1)) {
        spc_act_mod = -1;
    }
    else */
    if (mod_active[SPC_ACTIVE_MOD] == 1) {
        spc_act_mod = SPC_ACTIVE_MOD;
    }
    else if (mod_active[0] == 1) {
        spc_act_mod = 0;
    }
    else {
        printf("Error: neither the specified module nor module 0 (M1) are active.\n");
        return -1;
    }

    printf("Active SPC module is: module %d\n", spc_act_mod);

    init_status = SPC_get_init_status(spc_act_mod);

    printf("Active SPC module status: %hu\n", init_status);

    spc_ret = SPC_get_parameters(spc_act_mod, &spc_data);
    printf("SPC parameters backed up (L%d)\n", __LINE__);

    return 0;
}

int initialize_DCC_modules()
{
    /*
    Initialize DCC modules
    */


    BZERO(dcc_mod_info);

    // initialization must be done always at the beginning
    dcc_ret = DCC_init(dcc_ini_fname);

    if (dcc_ret < 0 && (-dcc_ret < DCC_NO_ACT_MOD || -dcc_ret >= DCC_WRONG_LICENSE)) {
        printf("\nError: DCC initialization failed. ret = %d\n", dcc_ret);
        return -1;   // fatal error, maybe wrong ini file or DLL not registered
    }

    total_no_of_dcc = no_of_active_dcc = 0;


    for (int i = 0; i < MAX_NO_OF_DCC; i++) {
        DCC_get_module_info(i, &dcc_mod_info[i]);
        if (dcc_mod_info[i].module_type != M_WRONG_TYPE) {
            total_no_of_dcc++;
        }
        if (dcc_mod_info[i].init == INIT_DCC_OK) {
            no_of_active_dcc++; in_use[i] = 1;
            DCC_get_eeprom_data(0, &dcc_eep_data);
            printf("DCC-%i Bus: %i Slot: %i \n", dcc_mod_info[i].module_type, dcc_mod_info[i].bus_number, dcc_mod_info[i].slot_number);

        }
        else
            in_use[i] = 0;
    }
    printf("Total number of DCCs detected: %d\n", no_of_active_dcc);

    if (no_of_active_dcc == 0) {
        // no active DCC
        printf("\nError: no active DCC modules were found.\n");
        return -1;
    }

    // assuming that only 1 module exists
    dcc_act_mod = -1;
    for (int i = 0; i < MAX_NO_OF_DCC; i++) {
        dcc_ret = DCC_test_if_active(i);
        if (dcc_ret && dcc_act_mod == -1) {
            dcc_act_mod = i;
            break;
        }
    }

    dcc_ret = DCC_get_mode();
    hardware = (dcc_ret == DCC_HARD);

    printf("DCC mode: %d\n", dcc_ret);
    printf("DCC hardware status: %d\n", hardware);

    DCC_get_init_status(dcc_act_mod, &init_status);

    if (!(init_status == INIT_DCC_OK || !hardware)) {
        printf("\nError: unable to load DCC module(s) and/or enter hardware mode.\n");
        return -1;
    }

    printf("DCC initialized successfully.\n");
    dcc_error = DCC_get_parameters(dcc_act_mod, &dcc_data);
    dcc_error = DCC_get_eeprom_data(dcc_act_mod, &dcc_eep_data);
    dcc_eep_data.serial_no[11] = '\0';

    return 0;
}

int initialize_GVD_modules() {
    /*
    Initialize GVD
    */

    BZERO(gvd_mod_info);
    /* initialization must be done always at the beginning*/

    gvd_ret = GVD_init(gvd_ini_fname);

    

    if (gvd_ret < 0 && (-gvd_ret < GVD_NO_ACT_MOD || -gvd_ret >= GVD_NO_LICENSE)) {
        printf("GVD initialization ret: %d\n", gvd_ret);
        return -1;   // fatal error, maybe wrong ini file   or DLL not registered
    }

    total_no_of_gvd = no_of_active_gvd = 0;

    for (int i = 0; i < MAX_NO_OF_GVD; i++) {
        GVD_get_module_info(i, &gvd_mod_info[i]);
        if (gvd_mod_info[i].module_type != M_GVD_WRONG_TYPE) {
            total_no_of_gvd++;
        }
        if (gvd_mod_info[i].init == INIT_GVD_OK) {
            no_of_active_gvd++; gvd_in_use[i] = 1;
        }
        else
            gvd_in_use[i] = 0;
    }

    if (!total_no_of_gvd) {
        printf("\nError: no GVD detected.\n");
        return -1;
    }
    else {
        printf("Number of GVDs: %d\n", total_no_of_gvd);
    }

    gvd_ret = GVD_get_mode();
    gvd_hardware = (gvd_ret == GVD_HARD);

    if (total_no_of_gvd != no_of_active_gvd) {
        // forcing hardware mode when module is in use is not recommended
        //    because it can cause that other application will not work correctly
        //    but it can be needed e.g after the software crash
        for (int i = 0; i < MAX_NO_OF_GVD; i++) {
            if (gvd_mod_info[i].module_type == M_GVD120 && gvd_mod_info[i].in_use == -1)
                gvd_in_use[i] = 1;   // force using of the module used by other application
        }
        if (gvd_hardware)
            gvd_ret = GVD_set_mode(GVD_HARD, 1, gvd_in_use);
    }

    //once more after GVD_set_mode
    no_of_active_gvd = 0;
    for (int i = 0; i < MAX_NO_OF_GVD; i++) {
        GVD_get_module_info(i, &gvd_mod_info[i]);
        if (gvd_mod_info[i].init == INIT_GVD_OK) {
            no_of_active_gvd++; gvd_in_use[i] = 1;
        }
        else
            gvd_in_use[i] = 0;
    }


    if (no_of_active_gvd == 0) {
        printf("\nError: no active GVD modules.\n");
        return -1;
    }

    // with assumption that only 1 module exists
    gvd_act_mod = -1;
    for (int i = 0; i < MAX_NO_OF_GVD; i++) {
        gvd_ret = GVD_test_if_active(i);
        if (gvd_ret && gvd_act_mod == -1) {
            gvd_act_mod = i;
            break;
        }
    }

    gvd_ret = GVD_get_mode();
    gvd_hardware = (gvd_ret == GVD_HARD);

    GVD_get_init_status(gvd_act_mod, &gvd_init_status);

    if (gvd_init_status != INIT_GVD_OK) {
        printf("\nError: unable to properly initialize GVD.\n");
        return -1;
    }
    else {
        printf("GVD initialized successfully. \n");
        return 0;
    }
}

void close_all_modules() {

    //DCC_enable_outputs(dcc_act_mod, 0);  // disable outputs at the end

    //spc_ret = SPC_close();   // returns 0 when there are no errors
    //if (spc_ret == 0) {
    //    printf("SPC closed successfully.\n");
    //}
    //else {
    //    printf("SPC close result: %d\n", spc_ret);
    //}

    dcc_ret = DCC_close();   // returns 2 when called after DCC_init
    if (dcc_ret == 2) {
        printf("DCC closed successfully.\n");
    }
    else {
        printf("DCC close result: %d\n", dcc_ret);
    }
    
    gvd_ret = GVD_close();   // returns 2 when called after GVD_init
    if (gvd_ret == 2) {
        printf("GVD closed successfully.\n");
    }
    else {
        printf("GVD close result: %d\n", gvd_ret);
    }

    printf("\nClosed all modules with status: SPC=%d DCC=%d GVD=%d\n", spc_ret, dcc_ret, gvd_ret);

    if (buffer) {
        free(buffer);
    }
    return;
}

int read_rates(int n_seconds) {
    /*
    read rates
    */

    printf("\n");

    printf("SYNC      CFD       TAC       ADC       \n");
    for (int i = 0; i < n_seconds; ++i) {
        Sleep(1000);
        spc_ret = SPC_read_rates(spc_act_mod, &rates[spc_act_mod]);

        if (spc_ret == -SPC_RATES_NOT_RDY) {
            printf("%1.3e %1.3e %1.3e %1.3e\r", -1., -1., -1., -1.);
        }
        else {
            printf("%1.3e %1.3e %1.3e %1.3e\n", rates[spc_act_mod].sync_rate, rates[spc_act_mod].cfd_rate, rates[spc_act_mod].tac_rate, rates[spc_act_mod].adc_rate);
        }

        // test for overload and cooler current limit
        DCC_get_overload_state(spc_act_mod, &overload_state);
        DCC_get_curr_lmt_state(spc_act_mod, &curr_lmt_state);

        // and shut down if there is an issue
        if ((overload_state != 0) || (curr_lmt_state != 0)) {
            DCC_enable_outputs(dcc_act_mod, 0);  // disable outputs at the end
            printf("Warning: current limit reached or detector overload. Shutting down...");
            close_all_modules();
            return -2;
        }

        if (_kbhit()) {
            Sleep(3000);
            printf("%d Rate reading suspended. Waiting for input... (c)ontinue, (q)uit\n", i);

            input_char = getchar();
            putchar(input_char);

            if (input_char == 'c') {
                printf("%d Continuing in current mode...\n", i);
                continue;
            }
            else if (input_char == 's') {
                printf("%d Skipping rate reading...\n", i);
                continue;
            }
            else if (input_char == 'q') {
                printf("%d Exiting acquisition...\n", i);
                return -2;
                //break;
            }
        }
    }
    printf("\n");

    return 0;
}

short test_fill_state(void)
{
    short i, spc_ret, state;
    time_t starttime, endtime;

    for (i = 0; i < MAX_NO_OF_SPC; i++) {
        if (mod_active[i]) {
            time(&starttime);
            while (1) {
                spc_ret = SPC_test_state(i, &state);
                if (spc_ret < 0) return spc_ret;
                if ((state & SPC_HFILL_NRDY) == 0)
                    break;  // fill finished
                time(&endtime);
                if (difftime(endtime, starttime) > 2.5) {
                    spc_ret = -SPC_FILL_TOUT;
                    return spc_ret;
                }
            }
        }
    }
    return 0;
}

short  save_photons_in_file()
{
    long spc_ret;
    int i;
    unsigned short first_frame[3], no_of_fifo_routing_bits;
    unsigned long lval;
    float fval;
    FILE* stream;

    if (first_write) {
        // in order to be compatible with BH .spc files 1st photon entry in the file
        //   should contain macro time clock & current no of rout bits information

        // for all 32bit fifo types
        //           bit  31  - invalid flag
        //           bits 30-27 = no of rout bits,
        //           bits 26-24 flags 26 - raw data file, 25 - markers enabled
        //           bits 23-0 macro time clock in 0.1ns units
        // for SPC6x0 in 48bit mode
        //    1st 16-bit word  bit  12  - invalid flag
        //                     bits 11-8 = no of rout bits,
        //    2nd 16-bit word  bits 15-0 macro time clock in 0.1ns units

        no_of_fifo_routing_bits = 3; // it means 8 routing channels - default value
                                     //  set to 0 if router is not used
        switch (fifo_type) {
        case FIFO_48:
            first_frame[0] = 0x1000 | (no_of_fifo_routing_bits << 8);
            first_frame[1] = 500;  // MT clock always 50ns , in 0.1ns units
            first_frame[2] = 0;
            break;
        case FIFO_32:
            lval = 0x80000000 | (no_of_fifo_routing_bits << 27) | 500; // MT clock always 50ns
            first_frame[0] = (unsigned short)lval;
            first_frame[1] = (unsigned short)(lval >> 16);
            break;
        default:
            // for all other modes Macro Time clock is defined by MACRO_TIME_CLK parameter
            if (spc_data.macro_time_clk) {
                // SYNC frequency  or part of it
                // read rates and use sync_rate to calculate it
                SPC_read_rates(spc_act_mod, &rates[spc_act_mod]);
                fval = rates[spc_act_mod].sync_rate;
                i = spc_data.macro_time_clk;
                while (i > 1) {
                    fval = fval / 2.; i--;
                }
                fval = 1. / fval; // macro time  clock
                lval = (unsigned long)(round(fval * 1e10));  // in 0.1ns units
                lval &= 0x00ffffff;
            }
            else {
                lval = 500; // 50ns in 0.1ns units
                if (module_type == M_SPC140) {
                    SPC_get_version(spc_act_mod, &fpga_version);
                    if ((fpga_version & 0xff00) >= 0x200) // SPC-140 with FPGA v. > B0
                        lval = 250; // 25ns in 0.1ns units
                }
                if (module_type == M_SPC150)
                    lval = 250; // 25ns in 0.1ns units
            }
            lval = 0x80000000 | (no_of_fifo_routing_bits << 27) | lval;
            if (fifo_type == FIFO_IMG)   // SPC-140 with FPGAv. > B0 , SPC-150
                lval |= 0x02000000;   // bit 25
            first_frame[0] = (unsigned short)lval;
            first_frame[1] = (unsigned short)(lval >> 16);
            break;
        }

        /////////////////////////////////////////////////////////////
        // there is new alternative method:
        //  call SPC_get_fifo_init_vars function to get:
        //      - values needed to init photons stream
        //      -  .spc file header

        first_frame[2] = 0;

        spc_ret = SPC_get_fifo_init_vars(spc_act_mod, NULL, NULL, NULL, &spc_header);
        if (!spc_ret) {
            first_frame[0] = (unsigned short)spc_header;
            first_frame[1] = (unsigned short)(spc_header >> 16);
        }
        else {
            printf("\nError: unable to get FIFO init vars.\n");
            return -1;
        }
        /////////////////////////////////////////////////////////////

        first_write = 0;
        // write 1st frame to the file
        stream = fopen(phot_fpath, "wb");
        if (!stream) {
            printf("\nError: unable to open spc file for writing.\n");
            return -1;
        }

        if (fifo_type == FIFO_48)
            fwrite((void*)&first_frame[0], 2, 3, stream); // write 3 words ( 48 bits )
        else
            fwrite((void*)&first_frame[0], 2, 2, stream); // write 2 words ( 32 bits )
    }
    else {
        stream = fopen(phot_fpath, "ab");
        if (!stream) {
            printf("\nError: unable to append to spc file.\n");
            return -1;
        }
        fseek(stream, 0, SEEK_END);     // set file pointer to the end
    }

    spc_ret = fwrite((void*)buffer, 1, 2 * words_in_buf, stream); // write photons buffer
    fclose(stream);
    if (spc_ret != 2 * words_in_buf) {
        printf("\nError: error writing photon buffer (line %d).\n", __LINE__);
        return -1;
    }     // error type in errno

    return 0;
}

static void init_fifo_measurement(void) // init actions same for part 4 and 5
{
    float curr_mode;

    // in most of the modules types with FIFO mode it is possible to stop the fifo measurement
    //   after specified Collection time
    fifo_stopt_possible = 1;
    first_write = 1;

    SPC_get_version(spc_act_mod, &fpga_version);

    // SPC-150 and the newest SPC-140 & SPC-830 can record in FIFO modes
    //                   up to 4 external markers events
    //  predefined mode FIFO32_M is used for Fifo Imaging - it uses markers 0-2
    //     as  Pixel, Line & Frame clocks
    // in normal Fifo mode ( ROUT_OUT ) recording markers 0-3 can be enabled by
    //    setting a parameter ROUTING_MODE ( bits 8-11 )


  // before the measurement sequencer must be disabled
    SPC_enable_sequencer(spc_act_mod, 0);
    // set correct measurement mode

    SPC_get_parameter(spc_act_mod, MODE, &curr_mode);
    //printf("module type = %d %d\n", module_type, M_SPC150);
    switch (module_type) {
        //case M_SPC130:
        //case M_SPC131:
        //case M_SPC132:
        //    SPC_set_parameter(spc_act_mod, MODE, FIFO130);
        //    fifo_type = FIFO_130;
        //    fifo_size = 262144;  // 256K 16-bit words
        //    if (fpga_version < 0x306)  // < v.C6
        //        fifo_stopt_possible = 0;
        //    break;

        //case M_SPC600:
        //case M_SPC630:
        //    SPC_set_parameter(spc_act_mod, MODE, FIFO_32);  // or FIFO_48
        //    fifo_type = FIFO_32;  // or FIFO_48
        //    fifo_size = 2 * 262144;   // 512K 16-bit words
        //    if (fpga_version <= 0x207)  // <= v.B7
        //        fifo_stopt_possible = 0;
        //    break;

        //case M_SPC830:
        //    // ROUT_OUT for 830 == fifo
        //    // with FPGA v. > CO  also FIFO32_M possible
        //    SPC_set_parameter(spc_act_mod, MODE, ROUT_OUT);   // ROUT_OUT in 830 == fifo
        //                                                     // or FIFO_32M
        //    fifo_type = FIFO_830;    // or FIFO_IMG
        //    fifo_size = 64 * 262144; // 16777216 ( 16M )16-bit words
        //    break;

        //case M_SPC140:
        //    // ROUT_OUT for 140 == fifo
        //    // with FPGA v. > BO  also FIFO32_M possible
        //    SPC_set_parameter(spc_act_mod, MODE, ROUT_OUT);   // or FIFO_32M
        //    fifo_type = FIFO_140;  // or FIFO_IMG
        //    fifo_size = 16 * 262144;  // 4194304 ( 4M ) 16-bit words
        //    break;

        //case M_SPC151:
        //case M_SPC152:
        //case M_SPC160:
        //case M_SPC161:
        //case M_SPC162:
        case M_SPC150:
            // ROUT_OUT in 150 == fifo
            if (curr_mode != ROUT_OUT && curr_mode != FIFO_32M) {
                SPC_set_parameter(spc_act_mod, MODE, ROUT_OUT);
                curr_mode = ROUT_OUT;
            }
            fifo_size = 16 * 262144;  // 4194304 ( 4M ) 16-bit words
            if (curr_mode == ROUT_OUT)
                fifo_type = FIFO_150;
            else  // FIFO_IMG ,  marker 3 can be enabled via ROUTING_MODE
                fifo_type = FIFO_IMG;
            break;
        default:
            printf("Module not recognized.\n");
            return;

    }
    unsigned short rout_mode, scan_polarity;
    float fval;

    // ROUTING_MODE sets active markers and their polarity in Fifo mode ( not for FIFO32_M)
         // bits 8-11 - enable Markers0-3,  bits 12-15 - active edge of Markers0-3

    // SCAN_POLARITY sets markers polarity in FIFO32_M mode
    SPC_get_parameter(spc_act_mod, SCAN_POLARITY, &fval);
    scan_polarity = fval;
    SPC_get_parameter(spc_act_mod, ROUTING_MODE, &fval);
    rout_mode = fval;

    // use the same polarity of markers in Fifo_Img and Fifo mode
    rout_mode &= 0xfff8;
    rout_mode |= scan_polarity & 0x7;

    SPC_get_parameter(spc_act_mod, MODE, &curr_mode);
    if (curr_mode == ROUT_OUT) {
        rout_mode |= 0xf00;     // markers 0-3 enabled
        SPC_set_parameter(spc_act_mod, ROUTING_MODE, rout_mode);
    }
    if (curr_mode == FIFO_32M) {
        rout_mode |= 0x800;     // additionally enable marker 3
        SPC_set_parameter(spc_act_mod, ROUTING_MODE, rout_mode);
        SPC_set_parameter(spc_act_mod, SCAN_POLARITY, scan_polarity);
    }

    // switch off stop_on_overfl
    SPC_set_parameter(spc_act_mod, STOP_ON_OVFL, 0);
    SPC_set_parameter(spc_act_mod, STOP_ON_TIME, 0);
    if (fifo_stopt_possible) {
        // if Stop on time is possible, the measurement can be stopped automatically
        //     after Collection time
    /// !!!!!!!!!!!!!!
    //    the mode = FIFO32_M ( fifo_type = FIFO_IMG ) switches off (in hardware) Stop on time
    //        in order to make possible finishing current frame
    //     the measurement will not stop after expiration of collection time
    //     SPC_test_state sets flag SPC_WAIT_FR in the status - software should still read photons
    //       untill next frame marker appears and then should stop the measurement
    //
    //   to avoid this behaviour - set mode to normal fifo mode = 1 ( ROUT_OUT)
    /// !!!!!!!!!!!!!!
        SPC_set_parameter(spc_act_mod, STOP_ON_TIME, 1);
        SPC_set_parameter(spc_act_mod, COLLECT_TIME, collection_time);
    }


    // there is no need ( and also no way ) to clear FIFO memory before the measurement

    // In FIFO mode the whole SPC memory is a big buffer which is filled with photons.
    // From the user point of view it works like a fifo -
    //     you can read photon frames untill the fifo is empty
    //     ( or you reach required number of photons ).
    //  If your photon's rate is too high or you don't read photons fast enough,
    //    FIFO overrun can happen,
    //  it means that photons which were not read before are overwritten with the new ones.
    //  - macro time information after the overrun is not consistent
    // The photon's rate border at which overruns can appear depends on:
    //   - module type ( fifo size ),
    //   - your experiment ( photon's rate ),
    //   - your computer's speed, hard disk, disk cache, operating memory size
    //   - number of tasks running in the same time
    //  You can experiment using our measurement software to decide
    //       how big memory buffer to use to read photons and when to write to hard disk
    //  To increase the border :
    //     - close all unnecessary applications
    //     - do not write to the hard disk very big amount of data in one piece -
    //           it can slow down your measurement loop

    // buffer must be allocated for predefined max number of photons to read in one call
    // max number of photons to read in one call - hier max_ph_to_read variable
    // max_ph_to_read should also be defined carefully depending on the same aspects as
    //     overrun considerations above
    // if it is too big - you can block (slow down) your system during reading fifo
    //    ( for high photons rates)
    // if it is too small - you can decrease your photon' rate at which overrun occurs
    //    ( by big overhead for calling DLL function)
    // user can experiment with max_ph_to_read value to get the best performance of your
    //    system

    if (module_type == M_SPC830)
        max_ph_to_read = 2000000; // big fifo, fast DMA readout
    else
        max_ph_to_read = 200000;
    if (fifo_type == FIFO_48)
        max_words_in_buf = 3 * max_ph_to_read;
    else
        max_words_in_buf = 2 * max_ph_to_read;
}

//int GVD_scan(){
//    gvd_error = GVD_get_parameters(gvd_act_mod, &gvd_data);
//    //  to get error string ( description
//    GVD_get_error_string(gvd_error, gvd_error_string, 128);
//
//    gvd_error = GVD_get_eeprom_data(gvd_act_mod, &gvd_eep_data);
//    gvd_eep_data.serial_no[11] = '\0';
//
//    /* during GVD initialization all hardware parameters are set to values
//       taken from ini file ( or to default values )
//       This parameter set can be overwritten by using
//         GVD_set_parameters or GVD_set_parameter function.
//       After setting the parameter  check return value (should be >= 0 ) and
//       read the parameter again to get the value which was sent to the hardware
//        ( parameter value can be recalculated in DLL) */
//
//        // for example,
//    gvd_ret = GVD_set_parameter(gvd_act_mod, ZOOM_FACTOR, 2.); // change Zoom factor
//    GVD_get_parameter(gvd_act_mod, ZOOM_FACTOR, &fval);
//    gvd_data.zoom_factor = fval;
//
//    gvd_ret = GVD_get_version(gvd_act_mod, &FPGAversion);
//
//    lim_scan = 1;
//    fcnt = 5;
//    GVD_set_parameter(gvd_act_mod, FRAME_COUNTER, fcnt);  //
//    GVD_set_parameter(gvd_act_mod, LIMIT_SCAN, lim_scan);  //
//
//    // before scanning and after setting all parameters
//    //   scan curve must be prepared in GVD memory
//    GVD_prepare_scan_curve(gvd_act_mod);
//
//    gvd_error = GVD_get_parameters(gvd_act_mod, &gvd_data);
//
//    GVD_get_scan_info(gvd_act_mod, &scan_info);
//    // to start scan
//    gvd_ret = GVD_start_scan(gvd_act_mod);
//    //  to test scan state
//    short not_ready = 1;
//    while (not_ready) {
//        GVD_test_state(gvd_act_mod, &gvd_state, 0);
//
//        if (gvd_state & GVD_RUNNING) {
//            // scan is started by software
//        }
//
//        if (gvd_state & GVD_SCAN) {
//            // module delivers scan signals ( after the trigger appeared)
//        }
//
//        if (lim_scan == 1) {    // run one sequence of frames
//            if (gvd_state & GVD_EOF_SEQ) {
//                // scan of sequence of frames is finished
//                not_ready = 0; // stop the loop
//            }
//        }
//        else {
//            // continuous scanning - endless loop
//            // insert other break possibility
//        }
//
//    }
//
//    // call GVD_stop_scan to stop scan when scanning continuously or
//    //     without waiting for the end of sequence of frames or
//    //     after  scan of sequence of frames was finished
//
//    //  !!! GVD_stop_scan must be called always at the end of scan to reset
//    //        GVD hardware otherwise next scan will be not possible
//    gvd_ret = GVD_stop_scan(gvd_act_mod);
//
//    GVD_test_state(gvd_act_mod, &gvd_state, 1); // test state + clear flags
//
//}