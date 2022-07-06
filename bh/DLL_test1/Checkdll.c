// Checkdll.cpp : Defines the entry point for the application.
//

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

    }
	else
	{

		SPCModInfo info;

		SPC_EEP_Data data;

		SPC_get_module_info(0, &info);

		SPC_get_eeprom_data(0, &data);

		printf("SPC-%i SN: %s Bus: %i Slot: %i ", info.module_type, data.serial_no, info.bus_number, info.slot_number);
	}
	getchar();
}

