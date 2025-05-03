#include "RequestTransfertExit.h"




void  RequestTransferExit_Main( unsigned char*RxData )
{
    SetCurrentServiceID(RxData[0]);

    if( (ClientRequest != RequestDownload)  || (ClientRequest != RequestUplaod))
    {
        SendDiagPositiveResponce(RSE);
    }
    else{
        ClientRequest = NoRequest;
        unsigned char UDS_Frame[8];
        ResetFrameValue(UDS_Frame);
        UDS_Frame[0] = 0x77U;
        SendDiagPositiveResponce(UDS_Frame);
    }

}
