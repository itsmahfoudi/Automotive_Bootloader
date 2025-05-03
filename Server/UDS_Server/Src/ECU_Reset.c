#include "ECU_Reset.h"
#include "UDS_Shared.h"

volatile unsigned char DiagResetFlag;
static unsigned char tmpReceivedData[8]; // holds the received data from a communication line. 
unsigned char UDS_Frame[8];

unsigned char ECUResetMain( unsigned char* RxData){
    ECUReset_Init(RxData );
    unsigned char subfct = tmpReceivedData[1]; // Reset Type
    if(subfct != HardReset && subfct != SoftReset ){
        SendDiagNegativeResponce(SFNS);
        return UDS_NOK;
    }
    ResetRxMessage(UDS_Frame);
    UDS_Frame[0] = ECUReset_POSITIVE_RESPONSE_SID;
    UDS_Frame[1] = Sub_Fct;
    SendDiagPositiveResponce(UDS_Frame);
    if(subfct == SoftReset ){
        __SoftReset();
    }else{
        __HardReset();
    }
    return 0;
}

void ECUReset_Init(unsigned char *RxData){
    CopyDataBetwenTwoTables(tmpReceivedData , RxData );
    SetCurrentServiceID(tmpReceivedData[0]); 
}

void Diag_EcuHardReset(){
    HAL_NVIC_SystemReset();
}

void Diag_EcuSoftReset() {
    DiagConfigSetMSP();
    DiagGetResetHandler();
}

void DiagDeinit(){
     /* Destroye allocated section in RAM : Memory Buffer */
    Memory_Deinit();
    /* Stop The Timer */
    HAL_TIM_Base_Stop(&htim17);
    /* Disable The Timer Interrupt */
    HAL_TIM_Base_Stop_IT(&htim17);
}


void Memory_Deinit(){

}

void ResetRxMessage(unsigned char* RxMssg[] ){
    RxMssg[0] = 0xFFu;
    RxMssg[1] = 0xFFu;
    RxMssg[2] = 0xFFu;
    RxMssg[3] = 0xFFu;
    RxMssg[4] = 0xFFu;
    RxMssg[5] = 0xFFu;
    RxMssg[6] = 0xFFu;
    RxMssg[7] = 0xFFu;

}

void __SoftReset(){
    Diag_EcuSoftReset();
}

void __HardReset(){
    Diag_EcuHardReset();
}