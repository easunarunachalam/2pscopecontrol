// Checkdll.cpp : Defines the entry point for the application.
//

#include "Dcc_def.h"
#include "Spcm_def.h"
#include "stdio.h"


#define MAX_LOADSTRING 100



int main()
{
    char buf[255] = "spcm.ini";

    short err = SPC_init(buf);
    
    if(err < 0)
    {

        char text[255];

        SPC_get_error_string(err, text, 200);

		printf(text);

		printf(" (%i)", err);

		return 1;

    }

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

    int total_no_of_dcc = 0;
    int no_of_active_dcc = 0;
    int in_use[MAX_NO_OF_DCC];
    DCCModInfo dcc_mod_info[MAX_NO_OF_DCC];
    DCCdata dcc_data;
    DCC_EEP_Data dcc_eep;

    for (int i = 0; i < MAX_NO_OF_DCC; i++) {
        DCC_get_module_info(i, &dcc_mod_info[i]);
        if (dcc_mod_info[i].module_type != M_WRONG_TYPE) {
            total_no_of_dcc++;
        }
        if (dcc_mod_info[i].init == INIT_DCC_OK) {
            no_of_active_dcc++; in_use[i] = 1;
        }
        else
            in_use[i] = 0;
    }

	getchar();
}

