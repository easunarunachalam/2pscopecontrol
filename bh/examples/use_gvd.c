#include <windows.h>
#include <stdlib.h>

#include "gvd_def.h"

#ifndef BZERO
#define BZERO(buf) memset(&(buf),0,sizeof(buf))
#define BCOPY(buf1,buf2) memcpy(&(buf1), &(buf2), sizeof(buf2))
#endif


int WINAPI  WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,  int nShowCmd )
{

char fname[256]="gvd120.ini" , gvd_error_string[128] ;
//char fname[80]="c:\Program Files (x86)\BH\GVD\DLL\gvd120.ini" , gvd_error_string[128] ;
GVDModInfo  mod_info[MAX_NO_OF_GVD];
GVDdata gvddata;
GVD_EEP_Data eep;
GVDScanInfo  scan_info;
short gvd_error, ret, ret1, init_status, state, trigger, lim_scan;
int i, j, hardware, act_mod;
int total_no_of_gvd, no_of_active_gvd, in_use[MAX_NO_OF_GVD];
float fval;
unsigned short FPGAversion;
unsigned long fcnt;

BZERO ( mod_info );
      /* initialization must be done always at the beginning*/

ret = GVD_init(fname);


if(ret < 0 && (-ret < GVD_NO_ACT_MOD || -ret >= GVD_NO_LICENSE) )
  return -1;   // fatal error, maybe wrong ini file   or DLL not registered

total_no_of_gvd = no_of_active_gvd = 0;

for( i = 0; i < MAX_NO_OF_GVD; i++){
  GVD_get_module_info( i, &mod_info[i]);
  if( mod_info[i].module_type != M_GVD_WRONG_TYPE){
    total_no_of_gvd++;
    }
  if(mod_info[i].init == INIT_GVD_OK){
    no_of_active_gvd++; in_use[i] = 1;
    }
    else
      in_use[i] = 0;
  }

    
if( !total_no_of_gvd)
  return -1;

ret = GVD_get_mode();
hardware = ( ret == GVD_HARD);

if( total_no_of_gvd != no_of_active_gvd ){
      // forcing hardware mode when module is in use is not recommended
      //    because it can cause that other application will not work correctly
      //    but it can be needed e.g after the software crash
  for(i = 0; i < MAX_NO_OF_GVD; i++){
    if( mod_info[i].module_type == M_GVD120 && mod_info[i].in_use == -1)
      in_use[i] = 1;   // force using of the module used by other application
    }
  if(hardware)
    ret = GVD_set_mode(GVD_HARD, 1, in_use);

//  ret = GVD_set_mode(GVD_SIMUL120, 0, in_use);  // switch to simulation mode
  }

  //once more after GVD_set_mode
no_of_active_gvd = 0;
for( i = 0; i < MAX_NO_OF_GVD; i++){
  GVD_get_module_info( i, &mod_info[i]);
  if ( mod_info[i].init == INIT_GVD_OK){
    no_of_active_gvd++; in_use[i] = 1;
    }
    else
      in_use[i] = 0;
  }


if(no_of_active_gvd == 0)   // no active GVD
  return -1;
  
          // with assumption that only 1 module exists
act_mod = -1;
for(i = 0; i < MAX_NO_OF_GVD; i++){
  ret = GVD_test_if_active(i);
  if( ret && act_mod == -1){
    act_mod = i;
    break;
    }
  }

ret = GVD_get_mode();
hardware = ( ret == GVD_HARD);

GVD_get_init_status( act_mod, &init_status);


if ( init_status == INIT_GVD_OK ){
  gvd_error = GVD_get_parameters( act_mod, &gvddata);
  //  to get error string ( description
  GVD_get_error_string ( gvd_error, gvd_error_string, 128 );
  
  gvd_error = GVD_get_eeprom_data( act_mod, &eep);
  eep.serial_no[11]='\0';
 
/* during GVD initialization all hardware parameters are set to values
   taken from ini file ( or to default values )
   This parameter set can be overwritten by using
     GVD_set_parameters or GVD_set_parameter function.
   After setting the parameter  check return value (should be >= 0 ) and
   read the parameter again to get the value which was sent to the hardware 
    ( parameter value can be recalculated in DLL) */

  // for example,
  ret=GVD_set_parameter( act_mod, ZOOM_FACTOR, 2.); // change Zoom factor
  GVD_get_parameter( act_mod, ZOOM_FACTOR, &fval);
  gvddata.zoom_factor = fval;
 
  ret = GVD_get_version ( act_mod, &FPGAversion);
  
  trigger = 0;
  lim_scan = 1;
  fcnt = 5;
  GVD_set_parameter( act_mod, FRAME_COUNTER, fcnt);  // 
  GVD_set_parameter( act_mod, LIMIT_SCAN, lim_scan);  // 
  
  if ( FPGAversion > 0x200 ){   // B0
    // scan trigger available only with newest FPGA
    GVD_set_parameter( act_mod, SCAN_TRIGGER, trigger); // set trigger
    }
  
  // before scanning and after setting all parameters 
  //   scan curve must be prepared in GVD memory
  GVD_prepare_scan_curve ( act_mod );
  
  gvd_error = GVD_get_parameters( act_mod, &gvddata);

  GVD_get_scan_info ( act_mod, &scan_info);
  // to start scan
  ret = GVD_start_scan ( act_mod );
  //  to test scan state
  short not_ready = 1;
  while ( not_ready ){
    GVD_test_state ( act_mod, &state, 0);

    if ( state & GVD_RUNNING ){
      // scan is started by software
      }

    if ( state & GVD_WAIT_TRG ){
      // no scan signals yet - waiting for trigger
      }
  
    if ( state & GVD_SCAN ){
      // module delivers scan signals ( after the trigger appeared)
      }

    if ( lim_scan == 1 ){    // run one sequence of frames
      if ( state & GVD_EOF_SEQ ){
        // scan of sequence of frames is finished
        not_ready = 0; // stop the loop
        }
      }
      else{
        // continuos scanning - endless loop 
        // insert other break possibility
        }

    }
    
  // call GVD_stop_scan to stop scan when scanning continuously or 
  //     without waiting for the end of sequence of frames or
  //     after  scan of sequence of frames was finished
  
  //  !!! GVD_stop_scan must be called always at the end of scan to reset
  //        GVD hardware otherwise next scan will be not possible
  ret = GVD_stop_scan ( act_mod );

  GVD_test_state ( act_mod, &state, 1);   // test state + clear flags

  }
  
ret1 = GVD_close ();   // returns 2 when called after GVD_init


return 0;
}

