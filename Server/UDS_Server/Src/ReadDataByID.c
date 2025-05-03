#include "ReadDataByID.h"


static unsigned char tmpReceivedData[8];


unsigned char ReadDataByID_Main(){

    unsigned short tmp_id ; // Used to store the Received Data ID, ID size : 16 bits
    //unsigned char tmp_var.size;
    //unsigned char tmp_var.table;

    DIDs_Info tmp_var;

    ReadDataByID_Init(); // Initializaion 
    tmp_id = (unsigned short)( (tmpReceivedData[1]<<8)|tmpReceivedData[2] ) ; // set the Data ID in the tmp_id varaible   GET_DATA_ID();
    tmp_var = Init_Dids(tmp_id); // Initiliaze the info about each did 
    if( tmp_var.secure == 1  && SecurityAccess_Denied()){
        SendDiagNegativeResponce(SAD);
        return UDS_NOK;
    }

    if( is_DID_valid(tmp_id) ){
        /*Error Data ID is not valid or Unknown*/
        SendDiagNegativeResponce(ROOR);
        return UDS_NOK;
    }
    unsigned char UDS_Frame[8] = { 0xFFu,0xFFu,0xFFu,0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    if(tmp_var.table == 0 ){
        /* Variable's value is represneted in 4 Bytes  */
        unsigned int Value ;
        if(tmp_var.size == 1){
            Value = *((unsigned char*)DataToRead[tmp_id]);
        }else if(tmp_var.size == 2 ){
            Value = *((uint16_t*)DataToRead[tmp_id]);
        }else{
            Value = *((unsigned int*)DataToRead[tmp_id]);
        }
        UDS_Frame[0] = 0x62u; 
        UDS_Frame[1] = (tmp_id>>16)&0x00FFu; // ID MSB
        UDS_Frame[2] = (tmp_id) &0x00FFu;    // ID LSB
        UDS_Frame[3] =  (Value>>24) & 0xFF;  // Value MSB 
        UDS_Frame[4] =  (Value>>16) & 0xFF;
        UDS_Frame[5] =  (Value>>8) & 0xFF;
        UDS_Frame[6] =  (Value) & 0xFF;    // Value LSB
        SendDiagPositiveResponce(UDS_Frame);
    }else{
        unsigned char* fromData = NULL;
        if(tmp_id == DID_VIN_NUM ){
            fromData = ((unsigned char*)DataToRead[DID_VIN_NUM]);
        }
        /* if the variable is a table */
        if((tmp_var.size*tmp_var.table) > 5  ){
            /* Send a negative response to indicate that the Response Too Long */
            unsigned char pData[tmp_var.size * tmp_var.table];
            memcpy(pData , fromData  , tmp_var.size*tmp_var.table);
            TraitAndSendData( tmp_id,  pData ,  tmp_var.size*tmp_var.table);
        }else{
            /* tmp_var.size*tmp_var.table = 5 or less  */
            memcpy(UDS_Frame, fromData , tmp_var.size*tmp_var.table);
            SendDiagPositiveResponce(tmp_id , UDS_Frame  );
        }
    }
    return 0;
}

void ReadDataByID_Init(){
    unsigned char *RxData ;
    tmpReceivedData = Intr_Read_ReceivedData_UDS_Rx_Frame();     //  Read Received Data 
    CopyDataBetwenTwoTables(tmpReceivedData , RxData ); //  Copy the Received Data into a Local Variable 
    SetCurrentServiceID(tmpReceivedData[0]);            //  Set the Current Service ID to the var CurrentServiceID
}

/* Used in case we have Too long Data */
void TraitAndSendData( uint16_t DID,  unsigned char* pData , unsigned char size){
    //unsigned char UDS_Data[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    unsigned char UDS_Frame[8] = {0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    unsigned char offset = 0;
    UDS_Frame[0] = 0x62u; 
    UDS_Frame[1] = (DID>>16)&0x00FFu; // ID MSB
    UDS_Frame[2] = (DID) &0x00FFu;    // ID LSB
    for(int i = 0 ; i < size/5 ; i++){
        for(int j = 3 ; j < 8 ; j++){
            UDS_Frame[j] = pData[(i*5)+j];
            offset++;
        }
        SendDiagPositiveResponce(UDS_Frame);
    }
    if(offset != size ){
        int j = 3 ;
        for(int i = offset ; i <size ; i++ ){
            UDS_Frame[j] = pData[i];
            j++;
        }
        SendDiagPositiveResponce( UDS_Frame  );
    }

}
/* Used to initiate the Struct of trait Data*/
DIDs_Info Init_Dids(unsigned char DID){
        DIDs_Info tmp;
        switch( DID ){
        case DID_APP_VER :
            /* Application  */
            Str_Application_ver.size = GET_SIZE_APPL_VER();
            Str_Application_ver.table = 0;
            Str_Application_ver.secure = 0;
            tmp = Str_Application_ver;
            break;
        case DID_BL_VER:
            /*Bootloader version  */
            Str_Bootloader_ver.size = GET_SIZE_BL_VER();
            Str_Bootloader_ver.table = 0;
            Str_Bootloader_ver.secure = 0;
            tmp = Str_Bootloader_ver;
            break;
        case DID_VIN_NUM:
            /* VIN_NUMBER */
            Str_Vin_number.size  = GET_SIZE_VIN_NUM();
            Str_Vin_number.table = GET_SIZE_TABLE_VIN_NUM() ;
            Str_Vin_number.secure = 0;
            tmp = Str_Vin_number;
            break;
        case DID_ACTIVE_SESS:
            /* Active Session */
            Str_Active_session.size  = GET_SIZE_ACTIVE_SESS();
            Str_Active_session.table = 0;
            Str_Active_session.secure  = 0;
            tmp = Str_Active_session;
            break; 
        case DID_HW_VER:
            /* Hardware Version */
            Str_Hw_version.size = GET_SIZE_HW_VER();
            Str_Hw_version.table = 0;
            Str_Hw_version.secure = 0;
            tmp = Str_Hw_version;
            break; 
    }
    return tmp;
}

// 
unsigned char is_DID_valid(unsigned short did ){
    if(did != DID_APP_VER && did != DID_BL_VER && did != DID_VIN_NUM && did != DID_ACTIVE_SESS && did != DID_HW_VER ){
        return UDS_NOK;
    }
    return UDS_OK;
}
