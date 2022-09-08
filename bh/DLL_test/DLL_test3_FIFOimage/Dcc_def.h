/*---------------------------------------------------------------------------*/
/*                              DCC                                          */
/*---------------------------------------------------------------------------*/
/*    Copyright (c) Becker & Hickl GmbH  2021  All Rights Reserved.          */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Title   : DCC_DEF.H                                                       */
/* Purpose : Include file for programs using DCC-100/DCU-USB  32/64-bit DLL  */
/*              for Windows 8/10.                                            */
/* Note    :                                                                 */
/*                                                                           */
/*   Functions listed here are exported in   dcc32(64).dll                   */
/*                                                                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/

/*- Defines -----------------------------------------------------------------*/

  /* needed if linking DLL in external compiler; harmless otherwise */
#include "cvirte.h"



#ifdef __cplusplus
  extern "C"{
#endif


#pragma pack(push, 1)

#define MAX_NO_OF_DCC     8    /* max number of DCC-100 modules controlled by DLL */
#define MAX_NO_OF_DCCUSB  4    /* max number of DCU-USB modules controlled by DLL */

#ifndef DCC_DEFINITIONS
#define DCC_DEFINITIONS

/*- Error codes  ------------------------------------------------------------*/

#define OK                0    /* no error */

#define  DCCDLL_ERROR_KEYWORDS \
{ \
  keyword (DCC_NONE), \
  keyword (DCC_OPEN_FILE), \
  keyword (DCC_FILE_NVALID), \
  keyword (DCC_MEM_ALLOC), \
  keyword (DCC_READ_STR), \
  keyword (DCC_NO_ACT_MOD), \
  keyword (DCC_MOD_NO), \
  keyword (DCC_NOT_INIT), \
  keyword (DCC_NOT_ACTIVE), \
  keyword (DCC_IN_USE), \
  keyword (DCC_EEPROM_READ), \
  keyword (DCC_EEPROM_WRITE), \
  keyword (DCC_EEP_CHKSUM), \
  keyword (DCC_EEP_WR_DIS), \
  keyword (DCC_BAD_PAR_ID), \
  keyword (DCC_BAD_PAR_VAL), \
  keyword (DCC_WINDRV_ERR), \
  keyword (DCC_ERR_ID), \
  keyword (DCC_WRONG_LICENSE), \
  keyword (DCC_NO_LICENSE), \
  keyword (DCC_LICENSE_NOT_VALID), \
  keyword (DCC_LICENSE_DATE_EXP), \
  keyword (DCC_NO_USB_MOD), \
  keyword (DCC_CONNECT_USB_MOD), \
  keyword (DCC_ERR_USB), \
  keyword (DCC_FIRMWARE_VER), \
  };

/*
   use function DCC_get_error_string to get error string programmatically

   DCC_NONE            0       no error
   DCC_OPEN_FILE      -1       can't open configuration file
   DCC_FILE_NVALID    -2       not valid configuration file
   DCC_MEM_ALLOC      -3       can't allocate memory
   DCC_READ_STR       -4       can't read string from DCC configuration file
   DCC_NO_ACT_MOD     -5       no active modules
   DCC_MOD_NO         -6       module number out of range
   DCC_NOT_INIT       -7       DCC is not yet initialized or unknown module type
   DCC_NOT_ACTIVE     -8       can't execute function - module not active
   DCC_IN_USE         -9       Cannot initialize - DCC module already in use
   DCC_EEPROM_READ    -10      error during reading EEPROM
   DCC_EEPROM_WRITE   -11      error during writing to EEPROM
   DCC_EEP_CHKSUM     -12      wrong EEPROM checksum
   DCC_EEP_WR_DIS     -13      cannot execute - write access to EEPROM denied
   DCC_BAD_PAR_ID     -14      unknown DCC parameter id 
   DCC_BAD_PAR_VAL    -15      wrong parameter value  
   DCC_WINDRV_ERR     -16      Error in WinDriver 
   DCC_ERR_ID         -17      Error ID is out of range
   DCC_WRONG_LICENSE  -18    Corrupted license key
   DCC_NO_LICENSE     -19      License key not read from registry
   DCC_LICENSE_NOT_VALID -20   License is not valid for DCC DLL
   DCC_LICENSE_DATE_EXP  -21   License date expired
   DCC_NO_USB_MOD        -22   no DCC-USB modules found
   DCC_CONNECT_USB_MOD   -23   Cannot connect USB module
   DCC_ERR_USB           -24   error during access to USB resource 
   DCC_FIRMWARE_VER      -25   Incorrect firmware version of DCC module
*/


/*- DCC parameters IDs ------------------------------------------------------*/

#define  DCC_PARAMETERS_KEYWORDS \
{ \
  keyword (DCC_ACTIVE), \
  keyword (PCI_BUS_NO), \
  keyword (C1_P5V), \
  keyword (C1_M5V), \
  keyword (C1_P12V), \
  keyword (C1_GAIN_HV), \
  keyword (C2_P5V), \
  keyword (C2_M5V), \
  keyword (C2_P12V), \
  keyword (C2_DIGOUT), \
  keyword (C3_COOLING), \
  keyword (C3_COOLVOLT), \
  keyword (C3_COOLCURR), \
  keyword (C3_GAIN_HV), \
  keyword (C3_P5V), \
  keyword (C3_M5V), \
  keyword (C3_P12V), \
};


#define  DCCUSB_PARAMETERS_KEYWORDS \
{ \
  keyword (DCCUSB_ACTIVE), \
  keyword (C1U_P5V), \
  keyword (C1U_M5V), \
  keyword (C1U_P12V), \
  keyword (C1U_GAIN_HV), \
  keyword (C1U_COOLING), \
  keyword (C1U_COOLVOLT), \
  keyword (C1U_COOLCURR), \
  keyword (C2U_P5V), \
  keyword (C2U_M5V), \
  keyword (C2U_P12V), \
  keyword (C2U_GAIN_HV), \
  keyword (C2U_COOLING), \
  keyword (C2U_COOLVOLT), \
  keyword (C2U_COOLCURR), \
  keyword (C3U_P5V), \
  keyword (C3U_M5V), \
  keyword (C3U_P12V), \
  keyword (C3U_GAIN_HV), \
  keyword (C3U_COOLING), \
  keyword (C3U_COOLVOLT), \
  keyword (C3U_COOLCURR), \
  keyword (C4U_P5V), \
  keyword (C4U_M5V), \
  keyword (C4U_P12V), \
  keyword (C4U_GAIN_HV), \
  keyword (C4U_COOLING), \
  keyword (C4U_COOLVOLT), \
  keyword (C4U_COOLCURR), \
  keyword (C5U_DIGOUT), \
};

#ifdef keyword
#undef keyword
#endif

#define keyword(key) key

enum dccdll_error_enum     DCCDLL_ERROR_KEYWORDS
enum dcc_parameters_enum   DCC_PARAMETERS_KEYWORDS
enum dccusb_parameters_enum   DCCUSB_PARAMETERS_KEYWORDS


#endif     // DCC_DEFINITIONS


/*---------------------------------------------------------------------------*/
/*- Defines -----------------------------------------------------------------*/



      /* possible modes of DLL operation - returned from DCC_set(get)_mode */
#define DCC_HARD             0     /* hardware mode */
#define DCC_SIMUL100         100   /* simulation mode of DCC-100 */
#define DCC_SIMULUSB         200   /* simulation mode of DCU-USB ( USB module ) */

    /* supported module types   */
#define M_WRONG_TYPE  -1
#define M_DCC100    100
#define M_DCCUSB    200         /* DCU-USB  (USB Detector Control Unit)


    
    /*  initialization error codes -
           - possible return values of function DCC_get_init_status */
#define INIT_DCC_OK                    0
#define INIT_DCC_NOT_DONE             -1 /* active = 0  - initialization not done */
#define INIT_DCC_WRONG_EEP_CHKSUM     -2 /* wrong EEPROM checksum */
#define INIT_DCC_CANT_OPEN_PCI_CARD   -3 /* cannot open PCI card */
#define INIT_DCC_MOD_IN_USE           -4 /* module already in use */
#define INIT_DCC_WRONG_LICENSE        -5 /* corrupted license key */
#define INIT_DCC_NO_LICENSE           -6 /* license key not read from registry */
#define INIT_DCC_LICENSE_NOT_VALID    -7 /* license is not valid for DCC DLL */
#define INIT_DCC_LICENSE_DATE_EXP     -8 /* license date expired */
#define INIT_DCC_CANT_OPEN_USB        -9 /* cannot open DCU-USB */
#define INIT_DCC_FIRMWARE_VER         -10  /* incorrect firmware version of DCC module */




/*---------------------------------------------------------------------------*/


typedef struct Dccdata{          /* structure for DCC-100 module data  */

  unsigned short base_adr;  /* base I/O address on PCI bus */
  short active;            /* most of the library functions are executed
                              only when module is active ( not 0 )*/
  short c1_p5V;            /* Connector 1 +5V  On/Off  */
  short c1_m5V;            /* Connector 1 -5V  On/Off  */
  float c1_gain_HV;        /* Connector 1 Gain/HV [%]  */
  short c1_p12V;           /* Connector 1 +12V On/Off  */
  short c2_p5V;            /* Connector 2 +5V  On/Off  */
  short c2_m5V;            /* Connector 2 -5V  On/Off  */
  short c2_p12V;           /* Connector 2 +12V On/Off  */
  short c2_digout;         /* Connector 2 Digital Outputs State, 0 - ff(hex)  */
  short c3_p5V;            /* Connector 3 +5V  On/Off  */
  short c3_m5V;            /* Connector 3 -5V  On/Off  */
  short c3_p12V;           /* Connector 3 +12V On/Off  */
  float c3_coolVolt;       /* Connector 3 Cooler Voltage ( 0 - 5V)  */
  float c3_coolCurr;       /* Connector 3 Cooler Current Limit ( 0 - 2A)  */
  float c3_gain_HV;        /* Connector 3 Gain/HV [%]  */
  short c3_cooling;        /* Connector 3 Cooler On/Off  */
  }DCCdata;


typedef struct {          /* structure for DCU-USB module data  */

  short c1_p5V;            /* Connector 1 +5V  On/Off  */
  short c1_m5V;            /* Connector 1 -5V  On/Off  */
  short c1_p12V;           /* Connector 1 +12V On/Off  */
  short c1_cooling;        /* Connector 1 Cooler On/Off  */
  float c1_gain_HV;        /* Connector 1 Gain/HV [%]  */
  float c1_coolVolt;       /* Connector 1 Cooler Voltage ( 0 - 5V)  */
  float c1_coolCurr;       /* Connector 1 Cooler Current Limit ( 0 - 2A)  */
  short c2_p5V;            /* Connector 2 +5V  On/Off  */
  short c2_m5V;            /* Connector 2 -5V  On/Off  */
  short c2_p12V;           /* Connector 2 +12V On/Off  */
  short c2_cooling;        /* Connector 2 Cooler On/Off  */
  float c2_gain_HV;        /* Connector 2 Gain/HV [%]  */
  float c2_coolVolt;       /* Connector 2 Cooler Voltage ( 0 - 5V)  */
  float c2_coolCurr;       /* Connector 2 Cooler Current Limit ( 0 - 2A)  */
  short c3_p5V;            /* Connector 3 +5V  On/Off  */
  short c3_m5V;            /* Connector 3 -5V  On/Off  */
  short c3_p12V;           /* Connector 3 +12V On/Off  */
  short c3_cooling;        /* Connector 3 Cooler On/Off  */
  float c3_gain_HV;        /* Connector 3 Gain/HV [%]  */
  float c3_coolVolt;       /* Connector 3 Cooler Voltage ( 0 - 5V)  */
  float c3_coolCurr;       /* Connector 3 Cooler Current Limit ( 0 - 2A)  */
  short c4_p5V;            /* Connector 4 +5V  On/Off  */
  short c4_m5V;            /* Connector 4 -5V  On/Off  */
  short c4_p12V;           /* Connector 4 +12V On/Off  */
  short c4_cooling;        /* Connector 4 Cooler On/Off  */
  float c4_gain_HV;        /* Connector 4 Gain/HV [%]  */
  float c4_coolVolt;       /* Connector 4 Cooler Voltage ( 0 - 5V)  */
  float c4_coolCurr;       /* Connector 4 Cooler Current Limit ( 0 - 2A)  */
  short c5_digout;         /* Connector 5 Digital Outputs State, 0 - ff(hex)  */
  short active;            /* most of the library functions are executed
                              only when module is active ( not 0 )*/
  }DCCUSBdata;


              /* structure for module info */
typedef struct _DCCModInfo{   
  short    module_type;      /* module type */
  short    bus_number;       /* PCI bus number */
  short    slot_number;      /* slot number on PCI bus */
  short    in_use;           /* -1 used and locked by other application, 0 - not used 
                                 1 - in use  */
  short    init;             /* set to initialisation result code */
  unsigned short base_adr;   /* base I/O address */
  char     serial_no[12];     /* module serial number */
  }DCCModInfo;



              /* structure for DCC-USB module info */
typedef struct {   
  short    module_type;      /* module type */
  short    com_port_no;      /* COM port number */
  short    in_use;           /* -1 used and locked by other application, 0 - not used 
                                 1 - in use  */
  short    init;             /* set to initialisation result code */
  char     serial_no[12];     /* module serial number */
  }DCCUSBModInfo;





typedef struct _DCC_EEP_Data{     /* structure for DCC module EEPROM data  */
  char serial_no[12];             /* DCC module serial number */
  char date[12];                  /* DCC module production date */
  }DCC_EEP_Data;




typedef struct {                  /* structure for DCC-USB module EEPROM data  */
  char module_type[16];           /* DCC-USB module type */ 
  char serial_no[16];             /* DCC-USB module serial number */
  char date[16];                  /* DCC-USB module production date */
  }DCCUSB_EEP_Data;


#pragma pack(pop)

/*
 CVIDECL means C calling convention - default for C and C++ programs
 DLLSTDCALL means _stdcall calling convention which is required by Visual Basic
 
 The only difference between DCCstd_.. and DCC_.. functions is calling convention
                   
*/

       /* DCC-100 functions */

/*-------------------------------------------------------------------------*/
/*- Initialisation functions  ---------------------------------------------*/
/*-------------------------------------------------------------------------*/
short  CVICDECL    DCC_init (char *ini_file);
short  DLLSTDCALL  DCCstd_init (char *ini_file);

short  CVICDECL    DCC_test_if_active(short mod_no);
short  DLLSTDCALL  DCCstd_test_if_active(short mod_no);

short  CVICDECL    DCC_get_init_status(short mod_no,short * ini_status);
short  DLLSTDCALL  DCCstd_get_init_status(short mod_no,short * ini_status);

short  CVICDECL    DCC_get_mode (void);
short  DLLSTDCALL  DCCstd_get_mode (void);

short  CVICDECL    DCC_set_mode (short mode, short force_use, short *in_use);
short  DLLSTDCALL  DCCstd_set_mode (short mode, short force_use, short *in_use);

short  CVICDECL    DCC_get_module_info(short mod_no, DCCModInfo * mod_info);
short  DLLSTDCALL  DCCstd_get_module_info(short mod_no, DCCModInfo * mod_info);

short  CVICDECL    DCC_get_version ( short mod_no, unsigned short *version);
short  DLLSTDCALL  DCCstd_get_version ( short mod_no, unsigned short *version);

                 // also for DCU-USB
short  CVICDECL    DCC_get_error_string(short error_id, 
                                       char * dest_string, short max_length);
short  DLLSTDCALL  DCCstd_get_error_string(short error_id, 
                                       char * dest_string, short max_length);


/*-------------------------------------------------------------------------*/
/*- Setup functions  ------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

short  CVICDECL    DCC_get_parameter(short mod_no, short par_id, float * value);
short  DLLSTDCALL  DCCstd_get_parameter(short mod_no, short par_id, float * value);

short  CVICDECL    DCC_set_parameter(short mod_no, short par_id, 
                                     short send_to_hard, float value);
short  DLLSTDCALL  DCCstd_set_parameter(short mod_no, short par_id, 
                                        short send_to_hard, float value);

short  CVICDECL    DCC_get_parameters(short mod_no,DCCdata *dccdata);
short  DLLSTDCALL  DCCstd_get_parameters(short mod_no,DCCdata *dccdata);

short  CVICDECL    DCC_set_parameters(short mod_no, short send_to_hard, 
                                      DCCdata *dccdata);
short  DLLSTDCALL  DCCstd_set_parameters(short mod_no, short send_to_hard, 
                                         DCCdata *dccdata);

short  CVICDECL    DCC_get_eeprom_data (short mod_no,DCC_EEP_Data *eep_data);
short  DLLSTDCALL  DCCstd_get_eeprom_data (short mod_no,DCC_EEP_Data *eep_data);

short  CVICDECL    DCC_write_eeprom_data (short mod_no,unsigned short write_enable,
                                        DCC_EEP_Data *eep_data);
short  DLLSTDCALL  DCCstd_write_eeprom_data (short mod_no,unsigned short write_enable,
                                        DCC_EEP_Data *eep_data);

short  CVICDECL    DCC_get_gain_HV_limit(short mod_no, short lim_id, 
                                         short * value);
short  DLLSTDCALL  DCCstd_get_gain_HV_limit(short mod_no, short lim_id,
                                            short * value);

short  CVICDECL    DCC_set_gain_HV_limit(short mod_no, short lim_id, 
                                         short *value);
short  DLLSTDCALL  DCCstd_set_gain_HV_limit(short mod_no, short lim_id,
                                            short *value);

/*-------------------------------------------------------------------------*/
/*- Status  functions  ----------------------------------------------------*/
/*-------------------------------------------------------------------------*/

short  CVICDECL    DCC_enable_outputs(short mod_no, short enable);
short  DLLSTDCALL  DCCstd_enable_outputs(short mod_no, short enable);

short  CVICDECL    DCC_clear_overload(short mod_no);
short  DLLSTDCALL  DCCstd_clear_overload(short mod_no);

short  CVICDECL    DCC_get_overload_state(short mod_no, short *state);
short  DLLSTDCALL  DCCstd_get_overload_state(short mod_no, short *state);

short  CVICDECL    DCC_get_curr_lmt_state(short mod_no, short *state);
short  DLLSTDCALL  DCCstd_get_curr_lmt_state(short mod_no, short *state);




       /* DCU-USB (Detector Control Unit USB) functions */

/*-------------------------------------------------------------------------*/
/*- Initialisation functions  ---------------------------------------------*/
/*-------------------------------------------------------------------------*/
short  CVICDECL    DCCUSB_init (char *ini_file);
short  DLLSTDCALL  DCCUSBstd_init (char *ini_file);

short  CVICDECL    DCCUSB_test_if_active(short mod_no);
short  DLLSTDCALL  DCCUSBstd_test_if_active(short mod_no);

short  CVICDECL    DCCUSB_get_init_status(short mod_no,short * ini_status);
short  DLLSTDCALL  DCCUSBstd_get_init_status(short mod_no,short * ini_status);

short  CVICDECL    DCCUSB_get_mode (void);
short  DLLSTDCALL  DCCUSBstd_get_mode (void);

short  CVICDECL    DCCUSB_set_mode ( short mode, short *in_use);
short  DLLSTDCALL  DCCUSBstd_set_mode ( short mode, short *in_use);

short  CVICDECL    DCCUSB_reconnect ( short mod_no);
short  DLLSTDCALL  DCCUSBstd_reconnect ( short mod_no);

short  CVICDECL    DCCUSB_get_module_info ( short mod_no, DCCUSBModInfo * mod_info);
short  DLLSTDCALL  DCCUSBstd_get_module_info ( short mod_no, DCCUSBModInfo * mod_info);
          
          // to get error string use DCC(std)_get_error_string


/*-------------------------------------------------------------------------*/
/*- Setup functions  ------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

short  CVICDECL    DCCUSB_get_parameter(short mod_no, short par_id, float * value);
short  DLLSTDCALL  DCCUSBstd_get_parameter(short mod_no, short par_id, float * value);

short  CVICDECL    DCCUSB_set_parameter(short mod_no, short par_id, 
                                     short send_to_hard, float value);
short  DLLSTDCALL  DCCUSBstd_set_parameter(short mod_no, short par_id, 
                                        short send_to_hard, float value);

short  CVICDECL    DCCUSB_get_parameters(short mod_no,DCCUSBdata *dccdata);
short  DLLSTDCALL  DCCUSBstd_get_parameters(short mod_no,DCCUSBdata *dccdata);

short  CVICDECL    DCCUSB_set_parameters(short mod_no, short send_to_hard, 
                                      DCCUSBdata *dccdata);
short  DLLSTDCALL  DCCUSBstd_set_parameters(short mod_no, short send_to_hard, 
                                         DCCUSBdata *dccdata);

short  CVICDECL    DCCUSB_get_eeprom_data (short mod_no,DCCUSB_EEP_Data *eep_data);
short  DLLSTDCALL  DCCUSBstd_get_eeprom_data (short mod_no,DCCUSB_EEP_Data *eep_data);

short  CVICDECL    DCCUSB_write_eeprom_data (short mod_no,unsigned short write_enable,
                                        DCCUSB_EEP_Data *eep_data);
short  DLLSTDCALL  DCCUSBstd_write_eeprom_data (short mod_no,unsigned short write_enable,
                                        DCCUSB_EEP_Data *eep_data);

short  CVICDECL    DCCUSB_get_gain_HV_limit(short mod_no, short lim_id, 
                                         short * value);
short  DLLSTDCALL  DCCUSBstd_get_gain_HV_limit(short mod_no, short lim_id,
                                            short * value);

short  CVICDECL    DCCUSB_set_gain_HV_limit(short mod_no, short lim_id, 
                                         short *value);
short  DLLSTDCALL  DCCUSBstd_set_gain_HV_limit(short mod_no, short lim_id,
                                            short *value);

/*-------------------------------------------------------------------------*/
/*- Status  functions  ----------------------------------------------------*/
/*-------------------------------------------------------------------------*/

short  CVICDECL    DCCUSB_enable_outputs(short mod_no, short con_no, short enable);
short  DLLSTDCALL  DCCUSBstd_enable_outputs(short mod_no, short con_no, short enable);

short  CVICDECL    DCCUSB_clear_overload(short mod_no, short ov_no);
short  DLLSTDCALL  DCCUSBstd_clear_overload(short mod_no, short ov_no);

short  CVICDECL    DCCUSB_get_overload_state(short mod_no, short *state);
short  DLLSTDCALL  DCCUSBstd_get_overload_state(short mod_no, short *state);

short  CVICDECL    DCCUSB_get_curr_lmt_state(short mod_no, short *state);
short  DLLSTDCALL  DCCUSBstd_get_curr_lmt_state(short mod_no, short *state);




/*- Miscellaneous -----------------------------------------------------------*/

//  set the DLL state as before DCC_init call 
//  free DCC DLL hardware and software resources
//      while using DLL for example in LabView environment 
//      returns: 1 when called before DCC_init,  2 when called after DCC_init  

short   CVICDECL     DCC_close (void);
short   DLLSTDCALL   DCCstd_close (void);


//  set the DLL state as before DCCUSB_init call 
//  free DCC DLL hardware and software resources
//      while using DLL for example in LabView environment 
//      returns: 1 when called before DCCUSB_init,  2 when called after DCCUSB_init  

short   CVICDECL     DCCUSB_close (void);
short   DLLSTDCALL   DCCUSBstd_close (void);


/*- The End -----------------------------------------------------------------*/

#ifdef __cplusplus
  }
#endif
