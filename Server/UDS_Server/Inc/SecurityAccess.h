#ifndef SECURITYACCES_H
#define SECURITYACCES_H
/*

Security Access service : 0x27
The purpose of this service is to provide a means to access data and/or diagnostic services, which have 
restricted access for security, emissions, or safety reasons. Diagnostic services for downloading/uploading 
routines or data into a server and reading specific memory locations from a server are situations where 
security access may be required

------------------------------------------------------------------------------------------------------------
A typical example of the use of this service is as follows: 

⎯ client requests the “Seed”, 
⎯ server sends the “Seed”, 
⎯ client sends the “Key” (appropriate for the Seed received), 
⎯ server responds that the “Key” was valid and that it will unlock itself. 

------------------------------------------------------------------------------------------------------------
Request message 
SID | SubFct | SecurityAccess Data Record |

- SID : 0x27
- SubFct : Security Access Type 
            - RequestSeed
            - SendKey
            -
- DataRecords : 

------------------------------------------------------------------------------------------------------------
Positive Responce :
0x67 | SecurityAccessType | SecuritySeed |
------------------------------------------------------------------------------------------------------------
Process flow : 
Step 01: Client to Server, SeedRequest 
Step 02: Server to Client, send Seed Value
Step 03:  Client to Server : send key 
Step 04: Server to client : send flag (Valid or not )

------------------------------------------------------------------------------------------------------------
For this Version we will consider that we have just one security level.
*/

#include "BM_shared.h"
#include "Interfaces_accessing_var.h"


static uint8_t *RxData ;
//volatile uint8_t SecurityAccessState;


/* Positive Responce ID                                */
#define SA_POS_RS_ID                 0x67u
/* Security Access Flag                                */
#define SECURITYACCESS_VALID         0xFFu
#define SECURITYACCESS_DENIED        0x00u  

   

uint8_t seedKey[2]; // The Seed Requested
uint8_t Key_clc[6]; // The Key Calculated by the ECU 

/* Sub-Fonctions                                        */
#define RequetSeed                0x01u
#define Sendkey                   0x02u

/* Steps                                                */
#define SeedRequet               0x01u
#define Receivekey               0x02u

/* NRCs                                                 */
#define SFNS                     0x12u /* sub-function Not Supported                  */
#define IMLOIF                   0x13u  /* incorrect Message Length Or Invalid Format */
#define CNC                      0x22u  /* conditions Not Correct                     */
#define RSE                      0x22u  /* request Sequence Error                     */
#define ROOR                     0x31u  /* request Out Of Range                       */
#define IK                       0x35u  /* invalid Key                                */
#define ENOA                     0x36u  /* exceeded Number Of Attempts                */
#define RTDNE                    0x37u  /* required Time Delay Not Expired            */ 


unsigned char SecurityAccess_Denied();

#endif