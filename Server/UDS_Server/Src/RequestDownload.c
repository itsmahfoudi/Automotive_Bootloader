#include "RequestDownload.h"


unsigned char tmpReceivedData[8];


void RequestDownload_Main(){

    unsigned char checkFlag = 0x00u  ;
    unsigned char tmp_NRC;
    unsigned char MemAddrLength;
    unsigned char MemSizeLength;

    RequestDownload_Init();
    // Data Format id & Address And length FormatId : are Valid 
    MemAddrLength = tmpReceivedData[2] && 0x0F; // Address length 
    MemSizeLength = (tmpReceivedData[2] && 0xF0)>>4; // Memory size length
    {
    unsigned int tmp_DwdStartAddr = get_memoryStrat_address(MemAddrLength);
    RTE_Write_DataElement_DownloadStartAddr(tmp_DwdStartAddr);
    }

    if( tmpReceivedData[1] != NoMethodUsed & tmpReceivedData[1] != EncryptedMethod ){
        checkFlag = 0x01u;
        tmp_NRC = ROOR;
    }else if( MemAddrLength + MemSizeLength > 5  ){
        // Send an error Message  
        checkFlag = 0x01u;
        tmp_NRC = IMLOIF;
    }else if( SecurityAccess_Denied() ){
        checkFlag = 0x01;
        tmp_NRC = SAD; // Security Access denied 
    }
    
    unsigned char mem_flag = MEM_addr_is_Valid(  RTE_Read_DataElement_DownloadStartAddr()  , Application_Section ) | MEM_addr_is_Valid(  RTE_Read_DataElement_DownloadStartAddr()  , Calibration_Section );
    if( mem_flag != 0x01u ){
        checkFlag = 0x01u;
        tmp_NRC = ROOR;
    }else{
        // 0x50 : 5 is the number of Data bytes can the client send each time
        unsigned int tmp_DownloadMemorySize = tmpReceivedData[7];
        RTE_Write_DataElement_DownloadMemorySize(tmp_DownloadMemorySize);
        RTE_Write_DataElement_SecurityAccessState(DownloadRequested );
        unsigned char UDS_Frame[8] = { 0x74, 0x50, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        SendDiagPositiveResponce(UDS_Frame);
    }

    if(checkFlag == 0x01u){
        SendDiagNegativeResponce(tmp_NRC);
    }

    ClientRequest = RequestDownload;
}

void RequestDownload_Init(){
    static unsigned char *RxData;
    RxData = Intr_Read_ReceivedData_UDS_Rx_Frame();     //  Read Received Data 
    CopyDataBetwenTwoTables(tmpReceivedData , RxData ); //  Copy the Received Data into a Local Variable 
    SetCurrentServiceID(tmpReceivedData[0]);            //  Set the Current Service ID to the var CurrentServiceID
}

unsigned int get_memoryStrat_address(unsigned char addressLength){
    unsigned int MemAddr;
    if(addressLength == 4 ){
        MemAddr =  (tmpReceivedData[3]<<24)|(tmpReceivedData[4]<<16)|(tmpReceivedData[5]<<8)|tmpReceivedData[6];
    } 
    return MemAddr;
}
