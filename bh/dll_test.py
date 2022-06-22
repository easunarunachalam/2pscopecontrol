from ctypes import *

# from pathlib import Path

spcm_dll_path = r"C:\Program Files (x86)\BH\SPCM\DLL\spcm64.dll"
spcm_ini_path = r"C:\Program Files (x86)\BH\SPCM\DLL\spcm.ini"

libc = cdll.msvcrt
libspcm = cdll.LoadLibrary(spcm_dll_path)

print("hello")
print(windll.kernel32)
print(cdll.msvcrt)
print(libc.printf)

SPC_init = libspcm.SPC_init
SPC_errstr = libspcm.SPC_get_error_string
# print(SPC_errstr(0))
init_result = SPC_init(spcm_ini_path)
# init_result_s = SPC_errstr(init_result)

print(init_result)
print(init_result, init_result_s)
