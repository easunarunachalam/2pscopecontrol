# 2pscopecontrol

control software for the Needleman lab 2-photon microscope

## Contents:

`bh`: C/C++ code for low-level control of Becker & Hickl FLIM system (detectors, SPC cards, and scanner) for point-by-point FCS measurements (NOTE: this is only intended for use with SPC-150 cards, and does not perform compatibility checks)

`doc`: documentation for generic setup (driver installation, etc.) and setup for custom FCS code

`notebooks`: jupyter notebooks with
1. widget-based control for non-BH microscope hardware (laser, XY stage, Z piezo, wave plates, ND filters, shutter, filter wheel, and NI DAQ trigger); and 
2. random snippets that will hopefully eventually be integrated into a real program
