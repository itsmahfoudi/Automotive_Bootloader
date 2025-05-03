#ifndef _ROUTINECONTOL_H
#define _ROUTINECONTOL_H

/*
Routine Control Service 
is used by the client to execute a defined sequence of steps and obtain any relevant results.

----------------------------------------------------------------------
- this service is used by the client to (RoutineControl Type) :
    - start a routine              0x01
    - stop a routine               0x02
    - request routine results      0x03

-----------------------------------------------------------------------
- Routine ID : 
    - Erasing Mem : 0xFF 

-----------------------------------------------------------------------
UDS Frame (Request message): 
SID | RoutineControlType | RoutineID 2 Bytes |  RoutineControlOption Record |
-----------------------------------------------------------------------
Erasing mem :
0x31 | 01 | FF00 | StartAddr(4 Bytes ) | Size (1 Bytes) |
------------------------------------------------------------------------
Routine : Erasing mem 
31 | 01 | FF | 00 | 00 | 20 | 30 | 20 | 
Bytes[4::6] used for Strat Address
Bytes[7] used for size in KBytes : size = size*1024
------------------------------------------------------------------------
Memory : 
Flash bank 1 : 0x08100000 - 0x081FFFFF
Flash bank 2 : 0x08000000 -  0x080FFFFF

- using just 3 bytes for Start address because the flash range adress start by 0x08
- the range of address is for 000000 To 1FFFFF

-----------------------------------------------------------------------

*/
#include "BM_Shared.h"
static uint8_t *RxData ;


#define RC_PositiveResp_SID        0x71
#define MEM_Start                  0x000000u 
#define MEM_End                    0x1FFFFFu

/* supported Routines                */
#define Erasing_mem         0xFF00u   
#define RoutineFinished     0x01u

/* RoutineControlType                */
#define StartRoutine             0x01u
#define StopRoutine              0x02u
#define RequestRoutineRes        0x03u

/* NRCs                             */
#define SFNS                     0x12u /* Sub function not supported */
#define IMLOIF                   0x13u /* Incorrect Message Length or invalid format */
#define CNC                      0x22u /* Condition not correct */
#define RSE                      0x24u /* Request sequence error */
#define ROOR                     0x31u /* Request out of range */
#define SAD                      0x33u /* Security access denied */
#define GPF                      0x72u /* General Programming Failure */




#endif 