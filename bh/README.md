# how to use bh control programs

## pointwise FIFO measurements
1. Collect an image with SPCM

1. Process to generate targets for scanner

1. Run bh control program as follows:
```
 C:\Users\TCSPC\Documents\GitHub\2pscopecontrol\bh\FCS_engine\pointwise_FIFO\out\build\x64-debug\FCS_engine.exe <path to working directory> <module index> <collection time in seconds>
 ```
 Note that module index is 0/1 for M1/M2 resp. e.g.
 ```
 C:\Users\TCSPC\Documents\GitHub\2pscopecontrol\bh\FCS_engine\pointwise_FIFO\out\build\x64-debug\FCS_engine.exe . 0 0.6
 ```
