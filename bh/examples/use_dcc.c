#include <windows.h>
#include <stdlib.h>

#include "dcc_def.h"

#ifndef BZERO
#define BZERO(buf) memset(&(buf),0,sizeof(buf))
#define BCOPY(buf1,buf2) memcpy(&(buf1), &(buf2), sizeof(buf2))
#endif


int WINAPI  WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,  int nShowCmd )
{

char fname[80]="dcc100.ini" , dcc_error_string[128] ;
DCCModInfo  mod_info[MAX_NO_OF_DCC];
DCCdata dccdata;
DCC_EEP_Data eep;
short dcc_error, ret, ret1, init_status, in_use[MAX_NO_OF_DCC];
int i, j, hardware, act_mod;
int total_no_of_dcc, no_of_active_dcc;
float fval;

BZERO ( mod_info );

ret1 = DCC_close ();   // returns 1 when DLL is loaded and called before DCC_init

      /* initialization must be done always at the beginning*/

ret=DCC_init(fname);

if(ret < 0 && (-ret < DCC_NO_ACT_MOD || -ret >= DCC_WRONG_LICENSE) )
  return -1;   // fatal error, maybe wrong ini file   or DLL not registered

total_no_of_dcc = no_of_active_dcc = 0;

for( i = 0; i < MAX_NO_OF_DCC; i++){
  DCC_get_module_info( i, &mod_info[i]);
  if( mod_info[i].module_type != M_WRONG_TYPE){
    total_no_of_dcc++;
    }
  if(mod_info[i].init == INIT_DCC_OK){
    no_of_active_dcc++; in_use[i] = 1;
    }
    else
      in_use[i] = 0;
  }

    
if( !total_no_of_dcc)
  return -1;

ret = DCC_get_mode();
hardware = ( ret == DCC_HARD);

if( total_no_of_dcc != no_of_active_dcc ){
      // forcing hardware mode when module is in use is not recommended
      //    because it can cause that other application will not work correctly
      //    but it can be needed e.g after the software crash
  for(i = 0; i < MAX_NO_OF_DCC; i++){
    if( mod_info[i].module_type == M_DCC100 && mod_info[i].in_use == -1)
      in_use[i] = 1;   // force using of the module used by other application
    }
  if(hardware)
    ret=DCC_set_mode(DCC_HARD, 1, in_use);

//  ret=DCC_set_mode(DCC_SIMUL100, 0, in_use);  // switch to simulation mode
  }

  //once more after DCC_set_mode
no_of_active_dcc = 0;
for( i = 0; i < MAX_NO_OF_DCC; i++){
  DCC_get_module_info( i, &mod_info[i]);
  if(mod_info[i].init == INIT_DCC_OK){
    no_of_active_dcc++; in_use[i] = 1;
    }
    else
      in_use[i] = 0;
  }


if(no_of_active_dcc == 0)   // no active DCC
  return -1;
  
          // with assumption that only 1 module exists
act_mod = -1;
for(i = 0; i < MAX_NO_OF_DCC; i++){
  ret = DCC_test_if_active(i);
  if( ret && act_mod == -1){
    act_mod = i;
    break;
    }
  }

ret = DCC_get_mode();
hardware = ( ret == DCC_HARD);

DCC_get_init_status( act_mod, &init_status);


if(init_status == INIT_DCC_OK || !hardware){
  dcc_error = DCC_get_parameters( act_mod, &dccdata);
    
  dcc_error = DCC_get_eeprom_data( act_mod, &eep);
  eep.serial_no[11]='\0';
 
/* during DCC initialization all hardware parameters are set to values
   taken from ini file ( or to default values )
   This parameter set can be overwritten by using
     DCC_set_parameters or DCC_set_parameter function.
   After setting the parameter  check return value (should be >= 0 ) and
   read the parameter again to get the value which was sent to the hardware 
    ( parameter value can be recalculated in DLL) */

  // for example,
  ret=DCC_set_parameter( act_mod, C1_GAIN_HV, 0, 50.); // change Connector 1 Gain
  DCC_get_parameter( act_mod, C1_GAIN_HV, &fval);
  dccdata.c1_gain_HV = fval;

  // Outputs are disabled after DCC_init 
  // Check device cables befor enabling outputs 
  DCC_enable_outputs( act_mod, 1);
  
  // Now, depending on your connections, you should check in the loop whether 
  //      overload occurred by calling 
  //           DCC_get_overload_state and/or DCC_get_curr_lmt_state
  // 
  

  DCC_enable_outputs( act_mod, 0);  // disable outputs at the end
  }
  
ret1 = DCC_close ();   // returns 2 when called after DCC_init

return 0;
}

