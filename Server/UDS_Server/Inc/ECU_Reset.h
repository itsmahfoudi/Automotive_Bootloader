#ifndef  __ECU_RESET_H
#define  __ECU_RESET_H

#include "fbl_diag_core.h"
#include "UDS_Shared.h"

static unsigned char *RxData ;
extern volatile unsigned char DiagResetFlag;


/*        */
#define ECUReset_ID              0x11u
#define ECUReset_ReadData()    { RxData = UDS_GetRxFrame(); }
#define Diag_ECUReset_GetSID()             (RxData[0])
#define Diag_ECUReset_GetSubFct()          (RxData[1])
#define ECUReset_POSITIVE_RESPONSE_SID      (0x51u)

/* Reset Types           */
#define HardReset      0x01u
#define SoftReset      0x03u

/* Reset State Handler    */
#define ResetStateNone           0x00u
#define ResetStateReadyToRun     0x01u
#define ResetStateInProgress     0x02u

#define DiagSetResetNone()        (DiagResetFlag=ResetStateNone)
#define DiagSetResetReady()       (DiagResetFlag=ResetStateReadyToRun)
#define DiagSetResetInProgress() (DiagResetFlag=ResetStateInProgress) 
#define DiagGetResetReady()       (DiagResetFlag==ResetStateReadyToRun)

/* Negative responses Codes NRC         */
#define SFNS    0x12u  /* Sub-function Not Supported                */
#define IMLOIF  0x13u  /* Incorrect Message Length Or Invalid Format*/
#define CNC     0x22u  /* Conditions Not Correct                    */
#define SAD     0x33u  /*Security Acess Denied                      */


#define DiagClrError()                      ( diagErrorCode = DiagErrorNone )
#define FblDiagClrServiceInProgress()       ( diagServiceInProgress=DiagServiceStopped)
#define DiagConfigSetMSP()                  (__set_MSP(ECUReset_Config[0]);)
#define DiagGetResetHandler()               (*(void(*)(void))((volatile uint32_t*)ECUReset_Config[1]))

/*
#define __SoftReset()                       (Diag_EcuSoftReset())
#define __HardReset()                       (Diag_EcuHardReset())
*/

/* Function prototype   */
unsigned char ECUResetMain(void);
void ECUReset_Init(void);
void Diag_EcuHardReset(void);
void Diag_EcuSoftReset(void);
void DiagDeinit(void);
void DiagResetServiceFlags(void);
void ResetRxMessage(unsigned char* RxMssg[] );
void __SoftReset(void);
void __HardReset(void);

#endif