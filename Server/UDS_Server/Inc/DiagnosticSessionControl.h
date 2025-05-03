/**
 * @file   DiagnosticSessionControl.h
 * @author Oussama elfahsi
 * @brief  : Header for UDS_SessionControlSrv.c file.
  *        This file contains the common defines of UDS Session Control Service.
  ******************************************************************************       
 * @version 0.1
 * @date 2024-11-09
 * 
 * @copyright Copyright (c) SIGMA EMBEDDED 2024
 * All rights reserved.
 * 
 */
#ifndef  __DIAGNOSTICSESSIONCONTROL_H
#define  __DIAGNOSTICSESSIONCONTROL_H

#include "fbl_diag_core.h"
#include "UDS_Shared.h"
#include "ECU_Reset.h"

// a tmp  variable used to point to the Received Data so we can copy it to local variable during Traet it 
extern unsigned char *RxData ;

/* Negative responses Codes NRC */
#define SFNS    0x12 /* Sub-function Not Supported ------------------------------------*/
#define IMLOIF  0x13 /* incorrect Message Length Or Invalid Format --------------------*/
#define CNC     0x22 /* Conditions Not Correct ----------------------------------------*/

// Supported Sessions
#define DEFAULTSESSION           0x01U
#define PROGRAMMINGSESSION       0x02U
#define EXTENDEDSESSION          0x03U


#define Diag_SessControl_NextSession()     tmpReceivedData[1]

            
/* Functions Prototype  -------------------- ------------------------------------------*/
void DiagSessCntrl_MainFct( void );
void SessionCnrtl_Init();
uint8_t DiagCheckSubFunctionCode(uint8_t);
uint8_t DiagSessionReInit(uint8_t);
uint8_t DiagSessionSwitch(uint8_t);
uint8_t  DiagSetResetHandlerAddr(uint8_t __layer );

#endif 