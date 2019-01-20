Testing:
1. random full associative
2. random direct map
3. TODO REMOVE
IDEX offset convert num to make sure its 16 bits?
/afs/umich.edu/class/eecs370/bin/submit370 4 sim.c test1.txt.2.1.8 test2.txt.4.4.1 test3.txt.4.4.4 test4.txt.128.1.1 test5.txt.16.4.4 test6.16.16.16

 I caught them both in a test case that tested a cache with 1 set and 1 blockPerSet, but a number greater than 1 for blockSizeInWords. The contents was just a bunch of SWs and LWs that resulted in misses and hits. 

spec out
@@@ transferring word [0-3] from the memory to the cache
@@@ transferring word [0-0] from the cache to the processor
@@@ transferring word [4-7] from the memory to the cache
@@@ transferring word [6-6] from the processor to the cache
@@@ transferring word [1-1] from the cache to the processor
@@@ transferring word [4-7] from the cache to the memory
@@@ transferring word [20-23] from the memory to the cache
@@@ transferring word [23-23] from the cache to the processor
@@@ transferring word [2-2] from the cache to the processor
@@@ transferring word [20-23] from the cache to nowhere
@@@ transferring word [28-31] from the memory to the cache
@@@ transferring word [30-30] from the cache to the processor
@@@ transferring word [3-3] from the cache to the processor

set_num: 0
@@@ transferring word [0-127] from the memory to the cache
@@@ transferring word [0-0] from the cache to the processor
set_num: 0
@@@ transferring word [30-30] from the cache to the processor
set_num: 0
@@@ transferring word [1-1] from the cache to the processor
set_num: 0
@@@ transferring word [70-70] from the cache to the processor
set_num: 0
@@@ transferring word [2-2] from the cache to the processor
set_num: 0
@@@ transferring word [123-123] from the cache to the processor
set_num: 0
@@@ transferring word [3-3] from the cache to the processor
set_num: 0
@@@ transferring word [0-127] from the cache to nowhere
@@@ transferring word [384-511] from the memory to the cache
@@@ transferring word [429-429] from the processor to the cache
set_num: 0
@@@ transferring word [384-511] from the cache to the memory
@@@ transferring word [0-127] from the memory to the cache
@@@ transferring word [4-4] from the cache to the processor
set_num: 0
@@@ transferring word [73-73] from the processor to the cache
set_num: 0
@@@ transferring word [5-5] from the cache to the processor
set_num: 0
@@@ transferring word [55-55] from the cache to the processor
set_num: 0
@@@ transferring word [6-6] from the cache to the processor
set_num: 0
@@@ transferring word [0-127] from the cache to the memory
@@@ transferring word [12288-12415] from the memory to the cache
@@@ transferring word [12345-12345] from the cache to the processor
set_num: 0
@@@ transferring word [12288-12415] from the cache to nowhere
@@@ transferring word [0-127] from the memory to the cache
@@@ transferring word [7-7] from the cache to the processor
set_num: 0
@@@ transferring word [0-127] from the cache to nowhere
@@@ transferring word [640-767] from the memory to the cache
@@@ transferring word [678-678] from the cache to the processor
set_num: 0
@@@ transferring word [640-767] from the cache to nowhere
@@@ transferring word [0-127] from the memory to the cache
@@@ transferring word [8-8] from the cache to the processor
set_num: 0
@@@ transferring word [0-127] from the cache to nowhere
@@@ transferring word [128-255] from the memory to the cache
@@@ transferring word [234-234] from the cache to the processor
set_num: 0
@@@ transferring word [128-255] from the cache to nowhere
@@@ transferring word [0-127] from the memory to the cache
@@@ transferring word [9-9] from the cache to the processor
set_num: 0
@@@ transferring word [72-72] from the cache to the processor
set_num: 0
@@@ transferring word [10-10] from the cache to the processor
set_num: 0
@@@ transferring word [0-127] from the cache to nowhere
@@@ transferring word [128-255] from the memory to the cache
@@@ transferring word [227-227] from the processor to the cache
set_num: 0
@@@ transferring word [128-255] from the cache to the memory
@@@ transferring word [0-127] from the memory to the cache
@@@ transferring word [11-11] from the cache to the processor
set_num: 0
@@@ transferring word [0-127] from the cache to nowhere
@@@ transferring word [2304-2431] from the memory to the cache
@@@ transferring word [2341-2341] from the processor to the cache
set_num: 0
@@@ transferring word [2304-2431] from the cache to the memory
@@@ transferring word [0-127] from the memory to the cache
@@@ transferring word [12-12] from the cache to the processor
set_num: 0
@@@ transferring word [0-127] from the cache to nowhere
@@@ transferring word [384-511] from the memory to the cache
@@@ transferring word [432-432] from the processor to the cache
set_num: 0
@@@ transferring word [384-511] from the cache to the memory
@@@ transferring word [0-127] from the memory to the cache
@@@ transferring word [13-13] from the cache to the processor
set_num: 0
@@@ transferring word [0-127] from the cache to nowhere
@@@ transferring word [896-1023] from the memory to the cache
@@@ transferring word [912-912] from the processor to the cache
set_num: 0
@@@ transferring word [896-1023] from the cache to the memory
@@@ transferring word [0-127] from the memory to the cache
@@@ transferring word [14-14] from the cache to the processor
set_num: 0
@@@ transferring word [56-56] from the processor to the cache
set_num: 0
@@@ transferring word [15-15] from the cache to the processor
set_num: 0
@@@ transferring word [0-127] from the cache to the memory
@@@ transferring word [896-1023] from the memory to the cache
@@@ transferring word [991-991] from the cache to the processor
set_num: 0
@@@ transferring word [896-1023] from the cache to nowhere
@@@ transferring word [0-127] from the memory to the cache
@@@ transferring word [16-16] from the cache to the processor
set_num: 0
@@@ transferring word [0-127] from the cache to nowhere
@@@ transferring word [256-383] from the memory to the cache
@@@ transferring word [342-342] from the processor to the cache
set_num: 0
@@@ transferring word [256-383] from the cache to the memory
@@@ transferring word [0-127] from the memory to the cache
@@@ transferring word [17-17] from the cache to the processor
set_num: 0
@@@ transferring word [0-127] from the cache to nowhere
@@@ transferring word [11008-11135] from the memory to the cache
@@@ transferring word [11111-11111] from the cache to the processor
set_num: 0
@@@ transferring word [11008-11135] from the cache to nowhere
@@@ transferring word [0-127] from the memory to the cache
@@@ transferring word [18-18] from the cache to the processor
set_num: 0
@@@ transferring word [0-127] from the cache to nowhere
@@@ transferring word [1152-1279] from the memory to the cache
@@@ transferring word [1234-1234] from the cache to the processor
set_num: 0
@@@ transferring word [1152-1279] from the cache to nowhere
@@@ transferring word [0-127] from the memory to the cache
@@@ transferring word [19-19] from the cache to the processor
set_num: 0
@@@ transferring word [0-127] from the cache to nowhere
@@@ transferring word [128-255] from the memory to the cache
@@@ transferring word [234-234] from the cache to the processor
set_num: 0
@@@ transferring word [128-255] from the cache to nowhere
@@@ transferring word [0-127] from the memory to the cache
@@@ transferring word [20-20] from the cache to the processor
set_num: 0
@@@ transferring word [0-127] from the cache to nowhere
@@@ transferring word [1024-1151] from the memory to the cache
@@@ transferring word [1111-1111] from the cache to the processor
set_num: 0
@@@ transferring word [1024-1151] from the cache to nowhere
@@@ transferring word [0-127] from the memory to the cache
@@@ transferring word [21-21] from the cache to the processor
set_num: 0
@@@ transferring word [0-127] from the cache to nowhere
@@@ transferring word [256-383] from the memory to the cache
@@@ transferring word [342-342] from the cache to the processor
set_num: 0
@@@ transferring word [256-383] from the cache to nowhere
@@@ transferring word [0-127] from the memory to the cache
@@@ transferring word [22-22] from the cache to the processor
set_num: 0
@@@ transferring word [0-127] from the cache to nowhere
@@@ transferring word [30976-31103] from the memory to the cache
@@@ transferring word [31000-31000] from the processor to the cache
set_num: 0
@@@ transferring word [30976-31103] from the cache to the memory
@@@ transferring word [0-127] from the memory to the cache
@@@ transferring word [23-23] from the cache to the processor
set_num: 0
@@@ transferring word [30-30] from the processor to the cache
set_num: 0
@@@ transferring word [24-24] from the cache to the processor
set_num: 0
@@@ transferring word [31-31] from the processor to the cache
set_num: 0
@@@ transferring word [25-25] from the cache to the processor
set_num: 0
@@@ transferring word [32-32] from the processor to the cache
set_num: 0
@@@ transferring word [26-26] from the cache to the processor
set_num: 0
@@@ transferring word [33-33] from the processor to the cache
set_num: 0
@@@ transferring word [27-27] from the cache to the processor
set_num: 0
@@@ transferring word [0-127] from the cache to the memory
@@@ transferring word [128-255] from the memory to the cache
@@@ transferring word [129-129] from the cache to the processor
set_num: 0
@@@ transferring word [128-255] from the cache to nowhere
@@@ transferring word [0-127] from the memory to the cache
@@@ transferring word [28-28] from the cache to the processor


16.4.4.4 victor
@@@ transferring word [0-15] from the memory to the cache
@@@ transferring word [0-0] from the cache to the processor
@@@ transferring word [16-31] from the memory to the cache
@@@ transferring word [30-30] from the cache to the processor
@@@ transferring word [1-1] from the cache to the processor
@@@ transferring word [64-79] from the memory to the cache
@@@ transferring word [70-70] from the cache to the processor
@@@ transferring word [2-2] from the cache to the processor
@@@ transferring word [112-127] from the memory to the cache
@@@ transferring word [123-123] from the cache to the processor
@@@ transferring word [3-3] from the cache to the processor
@@@ transferring word [416-431] from the memory to the cache
@@@ transferring word [429-429] from the processor to the cache
@@@ transferring word [4-4] from the cache to the processor
@@@ transferring word [73-73] from the processor to the cache
@@@ transferring word [5-5] from the cache to the processor
@@@ transferring word [48-63] from the memory to the cache
@@@ transferring word [55-55] from the cache to the processor
@@@ transferring word [6-6] from the cache to the processor
@@@ transferring word [12336-12351] from the memory to the cache
@@@ transferring word [12345-12345] from the cache to the processor
@@@ transferring word [7-7] from the cache to the processor
@@@ transferring word [672-687] from the memory to the cache
@@@ transferring word [678-678] from the cache to the processor
@@@ transferring word [8-8] from the cache to the processor
@@@ transferring word [224-239] from the memory to the cache
@@@ transferring word [234-234] from the cache to the processor
@@@ transferring word [9-9] from the cache to the processor
@@@ transferring word [72-72] from the cache to the processor
@@@ transferring word [10-10] from the cache to the processor
@@@ transferring word [227-227] from the processor to the cache
@@@ transferring word [11-11] from the cache to the processor
@@@ transferring word [2336-2351] from the memory to the cache
@@@ transferring word [2341-2341] from the processor to the cache
@@@ transferring word [12-12] from the cache to the processor
@@@ transferring word [432-447] from the memory to the cache
@@@ transferring word [432-432] from the processor to the cache
@@@ transferring word [13-13] from the cache to the processor
@@@ transferring word [912-927] from the memory to the cache
@@@ transferring word [912-912] from the processor to the cache
@@@ transferring word [14-14] from the cache to the processor
@@@ transferring word [56-56] from the processor to the cache
@@@ transferring word [15-15] from the cache to the processor
@@@ transferring word [976-991] from the memory to the cache
@@@ transferring word [991-991] from the cache to the processor
@@@ transferring word [16-16] from the cache to the processor
@@@ transferring word [336-351] from the memory to the cache
@@@ transferring word [342-342] from the processor to the cache
@@@ transferring word [17-17] from the cache to the processor
@@@ transferring word [416-431] from the cache to the memory
@@@ transferring word [11104-11119] from the memory to the cache
@@@ transferring word [11111-11111] from the cache to the processor
@@@ transferring word [18-18] from the cache to the processor
@@@ transferring word [912-927] from the cache to the memory
@@@ transferring word [1232-1247] from the memory to the cache
@@@ transferring word [1234-1234] from the cache to the processor
@@@ transferring word [19-19] from the cache to the processor
@@@ transferring word [234-234] from the cache to the processor
@@@ transferring word [20-20] from the cache to the processor
@@@ transferring word [976-991] from the cache to nowhere
@@@ transferring word [1104-1119] from the memory to the cache
@@@ transferring word [1111-1111] from the cache to the processor
@@@ transferring word [21-21] from the cache to the processor
@@@ transferring word [342-342] from the cache to the processor
@@@ transferring word [22-22] from the cache to the processor
@@@ transferring word [1232-1247] from the cache to nowhere
@@@ transferring word [30992-31007] from the memory to the cache
@@@ transferring word [31000-31000] from the processor to the cache
@@@ transferring word [23-23] from the cache to the processor
@@@ transferring word [30-30] from the processor to the cache
@@@ transferring word [24-24] from the cache to the processor
@@@ transferring word [31-31] from the processor to the cache
@@@ transferring word [25-25] from the cache to the processor
@@@ transferring word [672-687] from the cache to nowhere
@@@ transferring word [32-47] from the memory to the cache
@@@ transferring word [32-32] from the processor to the cache
@@@ transferring word [26-26] from the cache to the processor
@@@ transferring word [33-33] from the processor to the cache
@@@ transferring word [27-27] from the cache to the processor
@@@ transferring word [128-143] from the memory to the cache
@@@ transferring word [129-129] from the cache to the processor
@@@ transferring word [28-28] from the cache to the processor
