/*---------------------------------------------------------------------------*/
/*                              GVD                                          */
/*---------------------------------------------------------------------------*/
/*    Copyright (c) Becker & Hickl GmbH  2006-2021  All Rights Reserved.     */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Title   : GVD_DEF.H                                                       */
/* Purpose : Include file for programs using GVD-120/140 32/64-bit DLL under */
/*              Windows 8/10                                                 */
/* Note    :                                                                 */
/*                                                                           */
/*   Functions listed here are exported in   gvd32(64).dll                   */
/*                                                                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/

/*- Defines -----------------------------------------------------------------*/

  /* needed if linking DLL in external compiler; harmless otherwise */
#include "cvirte.h"



#ifdef __cplusplus
  extern "C"{
#endif

#define  GVD_SOFT_REV   200  // important - always 3 digits
#define  GVD_REV_TEXT "Version 2.00 , 28 September 2021, for Windows 8/10"
#define  GVD_TREV_TEXT "v.2.00"

#pragma pack( push, 1)

#define MAX_NO_OF_GVD     4    /* max number of GVD modules controlled by DLL */

#ifndef GVD_DEFINITIONS
#define GVD_DEFINITIONS

/*- Error codes  ------------------------------------------------------------*/

#define OK                0    /* no error */

#define  GVDDLL_ERROR_KEYWORDS \
{ \
  keyword (GVD_NONE), \
  keyword (GVD_OPEN_FILE), \
  keyword (GVD_FILE_NVALID), \
  keyword (GVD_MEM_ALLOC), \
  keyword (GVD_READ_STR), \
  keyword (GVD_NO_ACT_MOD), \
  keyword (GVD_MOD_NO), \
  keyword (GVD_NOT_INIT), \
  keyword (GVD_NOT_ACTIVE), \
  keyword (GVD_IN_USE), \
  keyword (GVD_EEPROM_READ), \
  keyword (GVD_EEPROM_WRITE), \
  keyword (GVD_EEP_CHKSUM), \
  keyword (GVD_EEP_WR_DIS), \
  keyword (GVD_BAD_PAR_ID), \
  keyword (GVD_BAD_PAR_VAL), \
  keyword (GVD_WINDRV_ERR), \
  keyword (GVD_HARD_TEST), \
  keyword (GVD_CANT_START), \
  keyword (GVD_CANT_STOP), \
  keyword (DCS_INIT_ERR), \
  keyword (DCS_EEPROM_READ), \
  keyword (DCS_EEPROM_WRITE), \
  keyword (DCS_EEP_CHKSUM), \
  keyword (DCS_NOT_ACTIVE), \
  keyword (GVD_ERR_ID), \
  keyword (GVD_NO_LICENSE), \
  keyword (GVD_WRONG_LICENSE), \
  keyword (GVD_LICENSE_NOT_VALID), \
  keyword (GVD_LICENSE_DATE_EXP), \
  keyword (GVD_FIRMWARE_VER), \
  };

/*
   use function GVD_get_error_string to get error string programmatically

   GVD_NONE                 0       no error
   GVD_OPEN_FILE           -1       can't open configuration file
   GVD_FILE_NVALID         -2       not valid configuration file
   GVD_MEM_ALLOC           -3       can't allocate memory
   GVD_READ_STR            -4       can't read string from GVD configuration file
   GVD_NO_ACT_MOD          -5       no active modules
   GVD_MOD_NO              -6       module number out of range
   GVD_NOT_INIT            -7       GVD is not yet initialized or unknown module type
   GVD_NOT_ACTIVE          -8       can't execute function - module not active
   GVD_IN_USE              -9       Cannot initialize - GVD module already in use
   GVD_EEPROM_READ         -10      error during reading GVD EEPROM
   GVD_EEPROM_WRITE        -11      error during writing to GVD EEPROM
   GVD_EEP_CHKSUM          -12      wrong GVD EEPROM checksum
   GVD_EEP_WR_DIS          -13      cannot execute - write access to EEPROM denied
   GVD_BAD_PAR_ID          -14      unknown GVD parameter id 
   GVD_BAD_PAR_VAL         -15      wrong parameter value  
   GVD_WINDRV_ERR          -16      Error in WinDriver 
   GVD_HARD_TEST           -17      GVD hardware test error 
   GVD_CANT_START          -18      cannot start scanning on GVD module 
   GVD_CANT_STOP           -19      cannot stop scanning on GVD module  
   DCS_INIT_ERR            -20      DCS-BOX init error
   DCS_EEPROM_READ         -21      error during reading DCS EEPROM
   DCS_EEPROM_WRITE        -22      error during writing to DCS EEPROM
   DCS_EEP_CHKSUM          -23      wrong DCS EEPROM checksum
   DCS_NOT_ACTIVE          -24      DCS-BOX not active - init error or doesn't exist
   GVD_ERR_ID              -25      Error ID is out of range
   GVD_NO_LICENSE          -26      License key not read from registry
   GVD_WRONG_LICENSE       -27      Corrupted license key
   GVD_LICENSE_NOT_VALID   -28      License is not valid for GVD DLL
   GVD_LICENSE_DATE_EXP    -29      License date expired
   GVD_FIRMWARE_VER        -30      Incorrect firmware version of GVD module
*/


/*- GVD parameters IDs ------------------------------------------------------*/

#define  GVD_PARAMETERS_KEYWORDS \
{ \
  keyword (ACTIVE), \
  keyword (FRAME_SIZE), \
  keyword (LASERS_ACTIVE), \
  keyword (MULTIPLEX), \
  keyword (LIMIT_SCAN), \
  keyword (FRAME_COUNTER), \
  keyword (SCAN_POLAR), \
  keyword (SCAN_TYPE), \
  keyword (LINE_TIME), \
  keyword (SCAN_RATE), \
  keyword (ZOOM_FACTOR), \
  keyword (OFFSET_X), \
  keyword (OFFSET_Y), \
  keyword (PARK_OFFS_X), \
  keyword (PARK_OFFS_Y), \
  keyword (PARK_CENTER), \
  keyword (L1_POWER), \
  keyword (L2_POWER), \
  keyword (DCS_CTRL), \
  keyword (SCAN_TRIGGER), \
  keyword (RECT_ZOOM_X), \
  keyword (RECT_ZOOM_Y), \
  keyword (L3_POWER), \    
  keyword (L4_POWER), \
  keyword (MULTIPLEX2), \
  keyword (MULTIPLEX3), \
  keyword (LASERS_ACTIVE1_4), \
  keyword (CONTROL2), \
};

// new parameters for GVD-140 starting from L3_POWER

#ifdef keyword
#undef keyword
#endif

#define keyword(key) key

enum gvddll_error_enum     GVDDLL_ERROR_KEYWORDS
enum gvd_parameters_enum   GVD_PARAMETERS_KEYWORDS


#endif     // GVD_DEFINITIONS


/*---------------------------------------------------------------------------*/
/*- Defines -----------------------------------------------------------------*/



      /* possible modes of DLL operation - returned from GVD_set(get)_mode */
#define GVD_HARD             0     /* hardware mode */
#define GVD_SIMUL120         120   /* simulation mode of GVD-120 */
#define GVD_SIMUL140         140   /* simulation mode of GVD-140 */

    /* supported module types   */
#define M_GVD_WRONG_TYPE  -1
#define M_GVD120       120
#define M_GVD140       140


    
    /*  initialization error codes -
           - possible return values of function GVD_get_init_status */
#define INIT_GVD_OK                    0   /* initialization OK */
#define INIT_GVD_NOT_DONE             -1   /* active = 0  - initialization not done */
#define INIT_GVD_WRONG_EEP_CHKSUM     -2   /* wrong EEPROM checksum */
#define INIT_GVD_HARD_TEST_ERR        -4   /* hardware test failed */
#define INIT_GVD_CANT_OPEN_PCI_CARD   -5   /* cannot open PCI card */
#define INIT_GVD_MOD_IN_USE           -6   /* module already in use */
#define INIT_GVD_WRONG_LICENSE        -7   /* corrupted license key */
#define INIT_GVD_NO_LICENSE           -8   /* license key not read from registry */
#define INIT_GVD_LICENSE_NOT_VALID    -9   /* license is not valid for GVD DLL */
#define INIT_GVD_LICENSE_DATE_EXP     -10  /* license date expired */
#define INIT_GVD_FIRMWARE_VER         -11  /* incorrect firmware version of GVD module */



         /* masks for GVD module state  - function GVD_test_state */
#define  GVD_RUNNING    1     // started by software
#define  GVD_EOF_SEQ    2     // scan of sequence of frames is finished  
#define  GVD_WAIT_TRG   4     // no scan signals - waiting for trigger ( FPGA version > B0 )
#define  GVD_SCAN       8     // module delivers scan signals ( after the trigger appeared) ( FPGA version > B0 )
#define  GVD_PARKED     0x10  // beam is parked


#define  MAX_GVD_ZOOM   1024     // highest zoom value, zoom_factor is recalculated
                                 //                    in GVD_prepare_scan_curve 


/*---------------------------------------------------------------------------*/


typedef struct {          /* structure for GVD module data  */

  short active;         // most of the library functions are executed
                        //   only when module is active ( not 0 ), default 0
  unsigned short frame_size;        
                        // bits 0-3  - lg of frame size ( quadratic frame)
                        //   values in range: 4 (16x16) ... 12 (4096x4096),  
                        //            ( default 9, 512x512)
                        // bits 4-7  - lg of frame size X ( rectangular frame)
                        //   values in range: 4 (16) ... 12 (4096),  
                        //            ( default 9, 512)
                        // bits 8-11 - lg of frame size Y ( rectangular frame)
                        //   values in range: 4 (16) ... 14 (16384),  
                        //            ( default 9, 512)
                        // bit 15 = 0 - quadratic frame, = 1 - rectangular frame
                        
  short lasers_active;  // bit 0(8) - laser 0(1) active, default 0,  for GVD-120 module
                        // bit 1    - laser 0 & 1 off during flyback , default 0
                        // bit 2    -  = 0 - laser 0 is the first laser when multiplexing lasers 
                        //             = 1 - laser 1 is the first laser when multiplexing lasers ,
                        //              default 0, bit 2 valid for FPGA version > B2
                        //  for GVD-240  
                        //  bit 0(8) - laser 0(1) active, default 0, 
                        //  bit 1    - lasers 0-3 off during flyback , default 0
                        //  bit 2 is not used, instead lasers_active1-4 is used
                        //  bits 9,10 - lasers 2,3 active, default 0
                        //  when multiplexing is on, additionally lasers_active1-4 defines
                        //              which laser and in each phase is active
                        //  bit 12 - DCS-Box in use (1) or not ( 0 ), default 0
                        //  either DCS_BOX or Attenuator Down of Laser 4  can be used ( they use the same pin )
  unsigned short multiplex;    // lasers multiplexing mode ( bits 0-1): 0 - off, 
                        //           1 - within each pixel, 2 - after each line, 
                        //           3 - after each frame , default 0
                        //  bits 2-15 - length of first laser phase in 1/100th of percent
                        //                when mode 1 ( pixel multiplexing), default 5000 ( 50%)   
                        // for GVD-120 :  ( first laser = laser 0 ( FPGA version < B3 )
                        //                              = bit 2 of lasers_active  ( FPGA version > B2 )
                        // for GVD-140 : lasers_active1-4 defines sequence of switching lasers
                        
  short limit_scan;     // 0 - unlimited scan ( default ), 1 - scan specified no of frames
                        // 2 - repeated scan of specified no of frames ( used with trigger)
  unsigned short frame_counter;  // no of frames to scan when limit_scan > 0, default = 1
  unsigned short scan_polarity;  // default 0 ( all active low )   
                        //   bit 0 - pixel clock polarity, bit 1 - polarity of HSYNC (Line), 
                        //   bit 2 - polarity of VSYNC (Frame)
                        //          bit = 0 - falling edge(active low)
                        //          bit = 1 - rising  edge(active high)  
  
  short scan_type;      // bit 0  = 0 scan frame ( default ), = 1 - scan one line only
                        // bit 3-1  line scanning type 
                        //    = 0 ramp continuous ( default ) ( scans also during the pixel time ), 
                        //    = 1 ramp steps (same position during the pixel) - increases min line time
                        //    = 2 sinus continuous, 
  
  float line_time;      // line time in sec 
  float zoom_factor;    // zoom factor for quadratic frame 1( default ) .. 1024, 
                        //     max scan range ( amplitude)  is divided by the factor
                        //     max depends on frame size: 4 for frames 2048x2048,
                        //          8 for frames 1024x1024, 16 for frames 512x512
                        //          32 for frames smaller than 512x512
  float offset_x;       // offset X of the centre of zoomed area from the centre X
                        //     of the total scan range, -100 % .. 100 % ( default 0 )
  float offset_y;       // offset Y of the centre of zoomed area from the centre Y
                        //     of the total scan range, -100 % .. 100 % ( default 0 )
  float park_offs_x;    // offset X of the park position from the centre X
                        //     of the total scan range, -100 % .. 100 % ( default 0 )
  float park_offs_y;    // offset Y of the park position from the centre Y
                        //     of the total scan range, -100 % .. 100 % ( default 0 )
  float l1_power;       //  Laser 1 power control 0 - 100%, default 50
  float l2_power;       //  Laser 2 power control 0 - 100%, default 50
  float  rect_zoom_x;   //  X zoom factor for rectangular frame, 1( default ) .. 1024, 
  float  rect_zoom_y;   //  Y zoom factor for rectangular frame, 1( default ) .. 1024, 
  short scan_rate;      //  0 - auto,scan rate fastest possible ( default ), 
                        //  1 - scan rate defined by line_time
  short park_center;    //  bit 0 = 1 when parked, set beam position in the centre
                        //            of scanned area, default 0
                        //  bit 4  ( read only ) shows current state of beam parking
                        //           1 - beam is parked, 0 - beam is not parked
  short scan_trigger;   //  External trigger condition ( FPGA version > B0 )           
                        //           bits 1 & 0 mean :                                      
                        //             00 - ( value 0 ) external trigger disabled (default),
                        //             01 - ( value 1 ) external trigger active low,        
                        //             10 - ( value 2 ) external trigger active high        
  float l3_power;       //  for GVD-140, Laser 3 power control 0 - 100%, default 50
  float l4_power;       //  for GVD-140, Laser 4 power control 0 - 100%, default 50
  unsigned short  multiplex2;  // for GVD-140
                        //  bits 0-13 - start of the second laser phase in 1/100th of percent
                        //        when pixel multiplexing and 4 lasers sequence is used, default 0 ( 0%)   
                        //  lasers_active1-4 defines sequence of switching lasers
  unsigned short  multiplex3;  // for GVD-140
                        //  bits 0-13 - start of the third laser phase in 1/100th of percent
                        //        when pixel multiplexing and 4 lasers sequence is used, default 0 ( 0%)   
                        //  lasers_active1-4 defines sequence of switching lasers
                        
  unsigned short  lasers_active1_4;   //  for GVD-140, default 0
                        //  defines which laser is active in subsequent multiplexing phases
                        //  bits 0-3   Laser 1 activity in phases 1-4
                        //  bits 4-7   Laser 2 activity in phases 1-4
                        //  bits 8-11  Laser 3 activity in phases 1-4
                        //  bits 12-15 Laser 4 activity in phases 1-4
                        //  if multiplexing is off, laser is active when minimum one of its 4 bits is set to 1
                        //   always a proper bit in lasers_active must be set to really activate specific laser
                        
  unsigned short  control2;           //  for GVD-140, for internal use, default 0
  short  sreserve;      //  reserved for future changes
  }GVDdata;


typedef struct {   
  short    scan_valid;   // if = 0, GVD_prepare_scan_curve must be called 
                         //       to prepare and load scan data
  short    first_pix;
  short    pix_per_line;
  short    first_line;
  double   pixel_time;
  double   line_time;
  double   x_flyback_time;  // line flyback time
  double   frame_time;
  double   y_flyback_time;  // frame flyback time ( after last line )
  double   tick_time;
  short    line_per_frame;
  short    steps_per_pix;
  short    DAC_per_step;
  short    line_pulse_shift;
  }GVDScanInfo;

              
              /* structure for module info */
typedef struct {   
  short    module_type;      /* module type */
  short    bus_number;       /* PCI bus number */
  short    slot_number;      /* slot number on PCI bus */
  short    in_use;           /* -1 used and locked by other application, 0 - not used 
                                 1 - in use  */
  short    init;             /* set to initialisation result code */
  unsigned short base_adr;   /* lower 16 bits of base I/O address */
  char     serial_no[16];     /* module serial number */
  }GVDModInfo;



typedef struct {          // structure for GVD adjust data  
  double  tline_1;        //  min line time at zoom = 1
  double  tline_inf;      //  min line time at zoom = infinite
  double  tline_max;      //  max line time
  double  tfb_1;          //  flyback time at zoom = 1
  double  tfb_inf;        //  flyback time at zoom = infinite
  double  progression_par;
  unsigned short s0;             //  shift at a size 512*512 an pxl time = 12.8µs
  unsigned short DAC_per_step;  // no of DAC values per memory location( step)
  short   flybackY_line;  //  no of line_time multiples for flyback Y time ( default 1), 
  short   max_zoom;       //  max zoom factor 10 (default) - 1024
  unsigned short  sreserve[6];      //  reserve for future changes
  }GVDadjust;




typedef struct {     /* structure for GVD module EEPROM data  */
  char module_type[16];           /* GVD module type */
  char serial_no[16];             /* GVD module serial number */
  char date[16];                  /* GVD module production date */
  }GVD_EEP_Data;


typedef struct {     /* structure for GVD module EEPROM data  */
  char laser1[12];    /* Laser1 name */
  char laser2[12];    /* Laser1 name */
  char laser3[12];    /* Laser3 name,   for GVD-140 */
  char laser4[12];    /* Laser4 name,   for GVD-140 */
  char reserve[30];
  }GVD_EEP_Def;





/*   GVD_LV_.. versions of the functions are prepared especially for usage 
          in LabView environment in 'Call Library' function node to avoid 
          problems with different strings representation in C and LabView 
          when using strings within clusters */

#ifndef LVSTR_DEFINED

#define LVSTR_DEFINED
typedef struct {     /* string definition in LabView */
  int   cnt;              /* number of bytes that follow */
  unsigned char str[1];   /* cnt bytes */
  } LVStr, *LVStrPtr, **LVStrHandle;
#endif

              /* LabView version of the structure for module info */
              // no serial_no field - take it from GVD_LV_EEP_Data
typedef struct {   
  short    module_type;       /* module type */
  short    bus_number;        /* PCI bus number */
  short    slot_number;       /* slot number on PCI bus */
  short    in_use;            /* -1 used and locked by other application, 0 - not used 
                                  1 - in use - active */
  short    init;              /* set to initialisation result code */
  unsigned short base_adr;    /* base I/O address */
  }GVD_LV_ModInfo;


            /*  LabView version of the structure for GVD module EEPROM data  */
typedef struct {     
  LVStrHandle module_type;     /*  handle to module type LabView string */
  LVStrHandle serial_no;       /*  handle to module serial number LabView string */
  LVStrHandle date;            /*  handle to module production date LabView string */
  }GVD_LV_EEP_Data;






/*
 CVIDECL means C calling convention - default for C and C++ programs
 DLLSTDCALL means _stdcall calling convention which is required by Visual Basic
 
 The only difference between GVDstd_.. and GVD_.. functions is calling convention
                   
*/



/*-------------------------------------------------------------------------*/
/*- Initialisation functions  ---------------------------------------------*/
/*-------------------------------------------------------------------------*/
short  CVICDECL    GVD_init ( char *ini_file);
short  DLLSTDCALL  GVDstd_init ( char *ini_file);

short  CVICDECL    GVD_test_if_active ( short mod_no);
short  DLLSTDCALL  GVDstd_test_if_active ( short mod_no);

short  CVICDECL    GVD_get_init_status ( short mod_no,short * ini_status);
short  DLLSTDCALL  GVDstd_get_init_status ( short mod_no,short * ini_status);

short  CVICDECL    GVD_get_mode (void);
short  DLLSTDCALL  GVDstd_get_mode (void);

short  CVICDECL    GVD_set_mode ( short mode, short force_use, int *in_use);
short  DLLSTDCALL  GVDstd_set_mode ( short mode, short force_use, int *in_use);

short  CVICDECL    GVD_get_module_info ( short mod_no, GVDModInfo * mod_info);
short  DLLSTDCALL  GVDstd_get_module_info ( short mod_no, GVDModInfo * mod_info);
short  CVICDECL    GVD_LV_get_module_info ( short mod_no, GVD_LV_ModInfo * mod_info );

short  CVICDECL    GVD_get_error_string ( short error_id, 
                                       char * dest_string, short max_length);
short  DLLSTDCALL  GVDstd_get_error_string ( short error_id, 
                                       char * dest_string, short max_length);

short  CVICDECL    GVD_get_version ( short mod_no, unsigned short *version);
short  DLLSTDCALL  GVDstd_get_version ( short mod_no, unsigned short *version);


/*- Miscellaneous -----------------------------------------------------------*/

// set the DLL state as before GVD_init call 
//  free GVD DLL hardware and software resources
//      while using DLL for example in LabView environment 
//      returns: 1 when called before GVD_init,  2 when called after GVD_init  
short  CVICDECL    GVD_close (void);
short  DLLSTDCALL  GVDstd_close (void);


/*-------------------------------------------------------------------------*/
/*- Setup functions  ------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

short  CVICDECL    GVD_get_parameter(short mod_no, short par_id, float * value);
short  DLLSTDCALL  GVDstd_get_parameter(short mod_no, short par_id, float * value);

short  CVICDECL    GVD_set_parameter(short mod_no, short par_id, float value);
short  DLLSTDCALL  GVDstd_set_parameter(short mod_no, short par_id, float value);

short  CVICDECL    GVD_get_parameters(short mod_no,GVDdata *gvddata);
short  DLLSTDCALL  GVDstd_get_parameters(short mod_no,GVDdata *gvddata);

short  CVICDECL    GVD_set_parameters(short mod_no, GVDdata *gvddata);
short  DLLSTDCALL  GVDstd_set_parameters(short mod_no, GVDdata *gvddata);

short  CVICDECL    GVD_get_adjust_parameters (short mod_no, 
                                        GVDadjust * adjpara, short from );
short  DLLSTDCALL  GVDstd_get_adjust_parameters (short mod_no, 
                                        GVDadjust * adjpara, short from );

short  CVICDECL    GVD_set_adjust_parameters (short mod_no, 
                                        GVDadjust * adjpara, short save_to_eep );
short  DLLSTDCALL  GVDstd_set_adjust_parameters (short mod_no, 
                                        GVDadjust * adjpara, short save_to_eep );

short  CVICDECL    GVD_get_eeprom_data (short mod_no, GVD_EEP_Data *eep_data);
short  DLLSTDCALL  GVDstd_get_eeprom_data (short mod_no, GVD_EEP_Data *eep_data);
short  CVICDECL    GVD_LV_get_eeprom_data (short mod_no, GVD_LV_EEP_Data *eep_data);

short  CVICDECL    GVD_write_eeprom_data (short mod_no,unsigned short write_enable,
                                        GVD_EEP_Data *eep_data);
short  DLLSTDCALL  GVDstd_write_eeprom_data (short mod_no,unsigned short write_enable,
                                        GVD_EEP_Data *eep_data);
short  CVICDECL    GVD_get_eeprom_defines (short mod_no,GVD_EEP_Def *eep_def);
short  DLLSTDCALL  GVDstd_get_eeprom_defines (short mod_no,GVD_EEP_Def *eep_def);

short  CVICDECL    GVD_write_eeprom_defines (short mod_no,GVD_EEP_Def *eep_def);
short  DLLSTDCALL  GVDstd_write_eeprom_defines (short mod_no,GVD_EEP_Def *eep_def);
/*-------------------------------------------------------------------------*/
/*- General  functions  ----------------------------------------------------*/
/*-------------------------------------------------------------------------*/

short   CVICDECL     GVD_test_state ( short mod_no, short *state, short clear_flags);
short   DLLSTDCALL   GVDstd_test_state ( short mod_no, short *state, short clear_flags);

short   CVICDECL     GVD_start_scan ( short mod_no);
short   DLLSTDCALL   GVDstd_start_scan ( short mod_no);

short   CVICDECL     GVD_stop_scan ( short mod_no);
short   DLLSTDCALL   GVDstd_stop_scan ( short mod_no);

short   CVICDECL     GVD_prepare_scan_curve ( short mod_no);
short   DLLSTDCALL   GVDstd_prepare_scan_curve ( short mod_no);

short   CVICDECL     GVD_park_beam ( short mod_no, short active, short center, 
                                     float *offs_x, float *offs_y );
short   DLLSTDCALL   GVDstd_park_beam ( short mod_no, short active, short center, 
                                        float *offs_x, float *offs_y );

short   CVICDECL     GVD_get_scan_info ( short mod_no, GVDScanInfo * scan_info );
short   DLLSTDCALL   GVDstd_get_scan_info ( short mod_no, GVDScanInfo * scan_info );


/*- GVD-140 specific functions --------------------------------*/
short  CVICDECL    GVD_set_attenuator ( short mod_no, short las_no, short up_down,  float value);
short  DLLSTDCALL  GVDstd_set_attenuator ( short mod_no, short las_no, short up_down,  float value);



/*-------------------------------------------------------------------------*/
/*- DCS-BOX specific defines and functions --------------------------------*/
/*-------------------------------------------------------------------------*/

//  starting with FPGA version A7, GVD module can control DCS-BOX connected to it via cable
//  names of the functions used to control DCS-BOX start with DCS_ (DCSstd_)


    /*  initialization error codes ( set in DCS_init function) -
           - possible return values of function DCS_get_init_status */
#define INIT_DCS_OK                    0 /*  initialization OK */
#define INIT_DCS_WRONG_EEP_CHKSUM     -1 /* wrong DCS EEPROM checksum */
#define INIT_DCS_NOT_DONE             -2 /* initialization not done */
#define INIT_DCS_WRONG_GVD            -3 /* initialization not done, wrong FPGA version of GVD module */
#define INIT_DCS_NO_ACCESS_D0         -4 /* can't access DCS resources - data line hangs on 0 */
#define INIT_DCS_NO_ACCESS_D1         -5 /* can't access DCS resources - data line hangs on 1 */
#define INIT_DCS_EEP_ERR              -6 /* error when rd/wr DCS EEPROM */
#define INIT_DCS_FPGA_ERR             -7 /* error when rd/wr DCS FPGA */


         /* masks for DCS status  - function DCS_get_status */
#define  DCS_SWITCH_0   1
#define  DCS_SWITCH_1   2
#define  SPC_A_NOT_830  4
#define  SPC_B_NOT_830  8
#define  SCANNING       0x10


typedef struct {          /* structure for DCS control parameters  */

  unsigned char  ctrl_low;    //  low byte of control parameters ( = reg. 4)
                     //     bits 0..2 = SPC-A no. of detector routing bits,
                     //          max = 4 ( for SPC1x0 max = 4 when bit 3 = 0
                     //                               max = 3 when bit 3 = 1 )
                     //     bit  3    = SPC-A laser routing enabled    
                     //     bits 4..6 = SPC-B no. of detector routing bits,
                     //          max = 4 ( for SPC1x0 max = 4 when bit 7 = 0
                     //                               max = 3 when bit 7 = 1 )
                     //     bit  7    = SPC-B laser routing enabled    
  unsigned char  ctrl_high;   //  high byte of control parameters ( = reg. 5)
                     //     bit  0    = Red LED state if bit 1 = 1  - 0 = off, 1 = on
                     //     bit  1    = 0 - Red LED of Scanner shows filter switch state
                     //               = 1 - bit 0 controls red LED state
                     //     bit  2    = SPC-A MARK 3 enable
                     //     bit  3    = SPC-B MARK 3 enable
                     //     bit  4    = SPC-A DCS-Switch signals OVLD to DCC
                     //     bit  5    = SPC-B DCS-Switch signals OVLD to DCC
                     //     bit  7    = always 1 when DCS is controlled by software
  }DCSdata;



typedef struct {     /* structure for DCS-BOX EEPROM data  */
  char module_type[16];           /* DCS-BOX type */
  char serial_no[16];             /* DCS-BOX serial number */
  char date[16];                  /* DCS-BOX production date */
  unsigned char ctrl_low;                  /* DCS-BOX setup byte 1 */
  unsigned char ctrl_high;                 /* DCS-BOX setup byte 2 */
  char reserve[14];               /* DCS-BOX setup reserve bytes */
  }DCS_EEP_Data;


            /*  LabView version of the structure for DCS EEPROM data  */
typedef struct {     
  LVStrHandle module_type;     /*  handle to module type LabView string */
  LVStrHandle serial_no;       /*  handle to module serial number LabView string */
  LVStrHandle date;            /*  handle to module production date LabView string */
  }DCS_LV_EEP_Data;

            /*  structure for DCS EEPROM DCS-BOX setup - for use in LabView */
typedef struct {     
  unsigned char ctrl_low;               /*  DCS-BOX setup byte 1 */
  unsigned char ctrl_high;              /*  DCS-BOX setup byte 2 */
  }DCS_LV_EEP_Setup;


short  CVICDECL    DCS_init ( short mod_no );
short  DLLSTDCALL  DCSstd_init ( short mod_no );

short  CVICDECL    DCS_get_version ( short mod_no, unsigned char *version);
short  DLLSTDCALL  DCSstd_get_version ( short mod_no, unsigned char *version);

short  CVICDECL    DCS_get_init_status ( short mod_no,short * ini_status);
short  DLLSTDCALL  DCSstd_get_init_status ( short mod_no,short * ini_status);

short  CVICDECL    DCS_get_parameters ( short mod_no, short from, DCSdata *dcsdata);
short  DLLSTDCALL  DCSstd_get_parameters ( short mod_no, short from, DCSdata *dcsdata);

short  CVICDECL    DCS_set_parameters ( short mod_no, DCSdata *dcsdata);
short  DLLSTDCALL  DCSstd_set_parameters ( short mod_no, DCSdata *dcsdata);

short  CVICDECL    DCS_read_status ( short mod_no, unsigned short * status);
short  DLLSTDCALL  DCSstd_read_status ( short mod_no, unsigned short * status);

short  CVICDECL    DCS_get_eeprom_data ( short mod_no, DCS_EEP_Data *eep_data);
short  DLLSTDCALL  DCSstd_get_eeprom_data ( short mod_no, DCS_EEP_Data *eep_data);
short  CVICDECL    DCS_LV_get_eeprom_data ( short mod_no, DCS_LV_EEP_Data *eep_data);
short  CVICDECL    DCS_LV_get_eeprom_setup (short mod_no, DCS_LV_EEP_Setup *eep_setup);

short  CVICDECL    DCS_write_eeprom_data ( short mod_no, unsigned short write_enable,
                                        DCS_EEP_Data *eep_data);
short  DLLSTDCALL  DCSstd_write_eeprom_data ( short mod_no, unsigned short write_enable,
                                        DCS_EEP_Data *eep_data);

#pragma pack( pop )


/*- The End -----------------------------------------------------------------*/

#ifdef __cplusplus
  }
#endif
