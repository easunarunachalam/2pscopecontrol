# Setting up LabVIEW control on a fresh Windows installation

The root directory for all the necessary software is referred to as `<labview_control_root>`. I've put a copy in my folder on the server (`Lab\Arunachalam`).

## Install National Instruments software
1. Install LabVIEW: `<labview_control_root>\LabVIEW 2017\2017LV-64WinEng.exe`. Please see the email I sent you for the license information. You may need to call NI and provide the (site-wide?) serial number in that email to get a license code for each new installation. Request one for LV 2017, not LV 2017 SP1, if you use this installer.
1. Install NI-DAQmx: `<labview_control_root>\NIDAQ\setup.exe` - necessary to control the DAQ board used for triggering and for SRS scanner control
1. Install NI-VISA: `<labview_control_root>\NI VISA\ni-visa_19.5_online_repack.exe` - necessary for communication with the laser, the shutter/filter wheel controller, and the Z piezo

## Install device drivers
1. Laser: `<labview_control_root>\Spectra-Physics laser driver\CP210xVCPInstaller_x64.exe`
1. XY stage: `<labview_control_root>\Prior_XY_stage_driver\Prior Scientific 64 bit Installer.exe`
1. Z piezo: `<labview_control_root>\Piezo_PI\PI_E-816_CD_Mirror_V4_0_0_0\PI_E-816.CD_Setup.exe`
1. Filter wheel and shutter: `<labview_control_root>\su10x_lambda_10-3_labview_driver\usb_driver\install_x64.exe`
1. Thorlabs motor controllers for HWP and SYNC: `<labview_control_root>\Thorlab_HWP\kinesis_16838_setup_x64.exe` and `<labview_control_root>\Thorlab_HWP\setup.exe` (they are different models)

## Specify COM ports
1. Use Device Manager to identify the COM port for the laser (should appear as "Silicon Labs CP210x USB to UART Bridge (COM_)") and the Sutter Instrument Lambda controller (should appear as "Sutter Instrument Lambda 10-3 USB Serial Port (COM_)")
1. Use PIMikroMove (installed alongside PI drivers) to identify the COM port for the Z piezo
1. The shutter/filter wheel can be tested in LabVIEW using `<labview_control_root>\su10x_lambda_10-3_labview_driver\su10x\su10x Push Button Example.vi`
1. Specify these COM ports in the initialization section of the LabVIEW control program.

## Install TCSPC software
1. `<labview_control_root>\TCSPC package\*`: the different versions work best for different things: 9.82 is responsive, but on occasion has triggering issues especially if photon counts are too high, 9.86 is much slower but some people have had fewer triggering issues, 9.87 seems slightly faster than 9.86 but has not been tested thoroughly at this point.
1. If you want fine-grained control, DLLs installed alongside 9.82 are license protected while those installed with 9.86/9.87 are not. LabVIEW SPC drivers seem to work with the files installed with 9.87.

# Setting up MicroManager control

1. Add Sutter Lambda 10-2 Hub. Use these settings (the option to add the filter wheel and shutter will pop up after the hub is added):
![image](https://user-images.githubusercontent.com/45492958/179087317-2b2cd4b1-f650-4b35-af97-20d3abc558b5.png)

