// Checkdll.cpp : Defines the entry point for the application.
//

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <process.h>
#include <direct.h>
#include <windows.h>

#include "Dcc_def.h"
#include "Spcm_def.h"


#define MAX_LOADSTRING 100

#ifndef BZERO
#define BZERO(buf) memset(&(buf),0,sizeof(buf))
#define BCOPY(buf1,buf2) memcpy(&(buf1), &(buf2), sizeof(buf2))
#endif


int main()
{
    char spc_ini_fname[255] = "C:\\Users\\TCSPC\\Documents\\GitHub\\2pscopecontrol\\bh\\config\\spcm.ini";
    char dcc_ini_fname[255] = "C:\\Users\\TCSPC\\Documents\\GitHub\\2pscopecontrol\\bh\\config\\dcc100.ini";
    char gvd_ini_fname[255] = "C:\\Users\\TCSPC\\Documents\\GitHub\\2pscopecontrol\\bh\\config\\gvd.ini";

    short err = SPC_init(spc_ini_fname);

    if(err < 0)
    {

        char text[255];

        SPC_get_error_string(err, text, 200);

		printf(text);

		printf(" (%i)", err);

		return 1;

    }

    /*
    Initialize SPC modules
    */

    int total_no_of_spc = 0;
    int no_of_active_spc = 0;
    int   mod_active[MAX_NO_OF_SPC];
    SPCModInfo  spc_mod_info[MAX_NO_OF_SPC];
    SPC_EEP_Data spc_data;
    short spc_error, force_use, act_mod;


    for (int i = 0; i < MAX_NO_OF_SPC; i++) {
        SPC_get_module_info(i, (SPCModInfo*)&spc_mod_info[i]);
        if (spc_mod_info[i].module_type != M_WRONG_TYPE) {
            total_no_of_spc++;
        }
        if (spc_mod_info[i].init == INIT_SPC_OK) {
            no_of_active_spc++; mod_active[i] = 1;
            SPC_get_eeprom_data(0, &spc_data);
            printf("SPC-%i SN: %s Bus: %i Slot: %i \n", spc_mod_info[i].module_type, spc_data.serial_no, spc_mod_info[i].bus_number, spc_mod_info[i].slot_number);
            act_mod = i;
        }
        else {
            mod_active[i] = 0;
        }
    }

    printf("Total number of SPCs detected: %d\n", no_of_active_spc);

    /*
    Initialize DCC modules
    */

    int total_no_of_dcc, no_of_active_dcc;
    int hardware;
    short ret, ret1, init_status, dcc_error, overload_state, curr_lmt_state, in_use[MAX_NO_OF_DCC];
    DCCModInfo dcc_mod_info[MAX_NO_OF_DCC];
    DCCdata dcc_data;
    DCC_EEP_Data dcc_eep;


    BZERO(dcc_mod_info);

    /* initialization must be done always at the beginning*/

    ret = DCC_init(dcc_ini_fname);

    if (ret < 0 && (-ret < DCC_NO_ACT_MOD || -ret >= DCC_WRONG_LICENSE))
        return -1;   // fatal error, maybe wrong ini file or DLL not registered

    printf("ret: %d\n", ret);

    total_no_of_dcc = no_of_active_dcc = 0;


    for (int i = 0; i < MAX_NO_OF_DCC; i++) {
        DCC_get_module_info(i, &dcc_mod_info[i]);
        if (dcc_mod_info[i].module_type != M_WRONG_TYPE) {
            total_no_of_dcc++;
        }
        if (dcc_mod_info[i].init == INIT_DCC_OK) {
            no_of_active_dcc++; in_use[i] = 1;
            DCC_get_eeprom_data(0, &dcc_data);
            printf("DCC-%i Bus: %i Slot: %i \n", dcc_mod_info[i].module_type, dcc_mod_info[i].bus_number, dcc_mod_info[i].slot_number);

        }
        else
            in_use[i] = 0;
    }
    printf("total number of DCCs detected: %d\n", no_of_active_dcc);

    if (no_of_active_dcc == 0) {
        // no active DCC
        printf("Error: no active DCC modules were found.\n");
        return -1;
    }

    // assuming that only 1 module exists
    act_mod = -1;
    for (int i = 0; i < MAX_NO_OF_DCC; i++) {
        ret = DCC_test_if_active(i);
        if (ret && act_mod == -1) {
            act_mod = i;
            break;
        }
    }

    ret = DCC_get_mode();
    hardware = (ret == DCC_HARD);

    printf("line %d -- ret: %d\n", __LINE__, ret);
    printf("line %d -- hardware: %d\n", __LINE__, hardware);

    DCC_get_init_status(act_mod, &init_status);

    if (init_status == INIT_DCC_OK || !hardware) {
        printf("line  %d -- DCC initialized successfully. \n", __LINE__);
        dcc_error = DCC_get_parameters(act_mod, &dcc_data);
        dcc_error = DCC_get_eeprom_data(act_mod, &dcc_eep);
        dcc_eep.serial_no[11] = '\0';

        // Outputs are disabled after DCC_init 
        // Enable outputs
        DCC_enable_outputs(act_mod, 1);

        Sleep(1000);

        // test for overload and cooler current limit
        DCC_get_overload_state(act_mod, &overload_state);
        DCC_get_curr_lmt_state(act_mod, &curr_lmt_state);
        printf("line %d -- overload: %d\n", __LINE__, overload_state);
        printf("line %d -- curr_lmt: %d\n", __LINE__, curr_lmt_state);

        DCC_enable_outputs(act_mod, 0);  // disable outputs at the end
    }

    ret1 = DCC_close();   // returns 2 when called after DCC_init
    printf("line %d -- DCC close result: %d\n", __LINE__, ret1);


	//getchar();
}
