#include "DiagnosticSessionControl.h"
#include "UDS_Shared.h"
#include "com.h"
#include "config.h"

static unsigned char tmpReceivedData[8];

void DiagSessCntrl_MainFct( unsigned char*ReceivedFrame ){

    unsigned char Resp = UDS_NOK; 
    SessionCnrtl_Init( ReceivedFrame);
    /* Verifing the Sub Fnct */
    if(  DiagCheckSubFunctionCode( Diag_SessControl_GetNextSession() ) != UDS_OK ){
        SendDiagNegativeResponce(SFNS); //  Defined in UDS_Shared File
    }
    //if( GetDaigSessionFlag() == UDS_NOK ){
    //    SendDiagNegativeResponce(CNC);
    //}
    // 
    if(DiagGetCurrentStateSession() == Diag_SessControl_NextSession()  ){
        Resp = DiagSessionReInit( Diag_SessControl_NextSession());
    }else {
        Resp = DiagSessionSwitch(Diag_SessControl_GetNextSession());
    }
    if(Resp !=UDS_OK){
        SendDiagNegativeResponce( CNC);
    }
    /**/
    return Resp;
}

// 
void SessionCnrtl_Init(unsigned char* ReceivedFrame){
    CopyDataBetwenTwoTables(tmpReceivedData , ReceivedFrame ); //  Copy the Received Data into a Local Variable 
    SetCurrentServiceID(tmpReceivedData[0]);            //  Set the Current Service ID to the var CurrentServiceID
}

unsigned char DiagCheckSubFunctionCode( unsigned char Sub_Fct_Code ){
    if(Sub_Fct_Code !=  FblDiagExtendedSession || Sub_Fct_Code != FblDiagStateDefaultSession ){
        return UDS_NOK;
    }
    return UDS_OK ;
}


unsigned char DiagSessionReInit(unsigned char __state){ 
    /*
    if the client has requested a diagnostic session, which is already running, then the server shall send a positive 
    response message and behave as follow : 
    i : When the server is in the defaultSession and the client requests to start the defaultSession then the 
    server shall re-initialize the defaultSession completely. 
    ii : When the server transitions from any diagnostic session other than the defaultSession to 
    another session other than the defaultSession (including the currently active diagnostic session) then the server shall 
    (re-) initialize the diagnostic session :
        -Security shall be relocked.  

    */
    unsigned int ;
    unsigned char _check=UDS_NOK;
    if(  __state == DEFAULTSESSION ) 
    {
        // Re-initialise Default Session
        _check = DefaultSession_Reinitialize();
    }
    else 
    {
        // Re-initialize parameters of Programming Session and Extended Session
        _check = DefaultSession_Reinitialize();

    }
    return UDS_OK;
}


/* 
| Transition from to Default Session | 

 - the server shall re-initialize the defaultSession completely.The server shall reset all activated/initiated/changed
settings/controls during the activated session.
*/

unsigned char DefaultSession_Reinitialize(){

} 


unsigned char DiagSessionSwitch(unsigned char __state ){
    unsigned char _check = UDS_NOK ;
    unsigned int  MSP_Value; 
    
    if( DiagGetCurrentStateSession() == DEFAULTSESSION )
    {
        // Transition from the application to BM
        MSP_Value = *((volatile unsigned int*)BM_MSP);
        _check=DiagSetResetHandlerAddr(IS_FOR_BM );
    } 
    else if ( (DiagGetCurrentStateSession() != DEFAULTSESSION) && (Diag_SessControl_NextSession() == DEFAULTSESSION )  ) 
    {
        DiagSetSecurityDenied();
        MSP_Value = *((volatile unsigned int*)BM_MSP);
        _check=DiagSetResetHandlerAddr(IS_FOR_BM );
    }  
    /* Set the MSP VALUE */
    DiagSetMSPValue(MSP_Value);

    /* Set the FLAG in the new session control */
    DiagSetStateSession(__state);
    if(_check == UDS_OK){
        // The UDS Frame to Send as a responce 
        unsigned char tmp_UDSFrame[8]={0x50u, Diag_SessControl_GetNextSession(),0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
        SendDiagPositiveResponce(tmp_UDSFrame);
        __SoftReset();
        }
    return _check;
}

/* This function will be implemented in ECUReset service */
unsigned char  DiagSetResetHandlerAddr(unsigned char __layer ){
    void (*reset_handler)(void) = ((void*)0);
    volatile unsigned int RESET_HANDLER_ADDR;
    if(__layer == IS_FOR_APP ){
        RESET_HANDLER_ADDR = APP_RESET_HANDLER_ADDR;
    }else if(__layer == IS_FOR_FBL){
        RESET_HANDLER_ADDR = FBL_RESET_HANDLER_ADDR;
    }else{
        RESET_HANDLER_ADDR = BM_RESET_HANDLER_ADDR;
    }
    reset_handler =*(void(*)(void))((volatile unsigned int*)RESET_HANDLER_ADDR);
    if(reset_handler != ((void*)0) ) {
        DiagSetResetHandAdrr(RESET_HANDLER_ADDR);
    }else{
        return UDS_NOK;
    }
    return UDS_OK;    
}


