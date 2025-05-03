#include "TransferData.h"

BlockSequence blockSequence;
blockSequence.CurrentBlockSequence = 0;
blockSequence.MemoryAddressWrtFrm = RequestDownloadParam.DownloadStartAddr;
blockSequence.ReceivedDataSize = 0 ;
unsigned int firmware_buffer[BlockSize] __attribute__((section(".firmware_buffer"))) = {0x0};

unsigned char TransferData_Main( TransferDataBuffer* ReveivedData ){
    TransferData_Init();
    if(ClientRequest != RequestDownload)
    {
        SendDiagNegativeResponce(RSE);
    }
    else if( (CheckSequenceRespected(ReveivedData->blockSequence) == UDS_NOK)  || ()){
    SendDiagNegativeResponce(WBSC); // Sequence is respected for SID?
    
    }
    else if(blockSequence.CurrentBlockSequence > blockSequence.MaxBlockSequence)
    {
    SendDiagNegativeResponce(TDS); // MemorySize is respected? 
    }
    else
    { // end of transfer Data
        for (int i = 0 ; i < BlockSize ; i++ )
        {
            firmware_buffer = ReveivedData->Data[i];   
        }
        // 02 : Writing the Firmware to Flash
        
        if ( WriteBlockToFlah( ReveivedData->Data , MemoryAddress ) != HAL_ERROR )
        {
            // 03 : Update Address to write the Firmware
            blockSequence.MemoryAddressWrtFrm  = blockSequence.MemoryAddressWrtFrm + (4*1024) ;
            // Calcul checkSum 
            unsigned CheckSumVal = CalcCheckSum();
            unsigned char UDS_Frame[8]; 
            UDS_Frame[0] = 0x76U;
            UDS_Frame[1] = ReveivedData->blockSequenc;
            UDS_Frame[2] = CheckSumVal & 0x00FF;
            UDS_Frame[3] = (CheckSumVal>>8) & 0x00FF;
            SendDiagPositiveResponce(UDS_Frame);
        }
        else 
        {
            SendDiagNegativeResponce(GPF);
        }
        // 04 : Updating the HASH Value 
        UpdateHashValue(calchashValue);
    }


}

unsigned int CalcCheckSum()
{
    unsigned checkSum = 0;
    for( int i = 0 ; i < BlockSize ; i++  )
    {
        checkSum += ReveivedData->Data[i];
    }
    return checkSum;
}

HAL_StatusTypeDef WriteBlockToFlah(  unsigned int curreentMemoryAddress )
{
    uint32_t counter = 0;
    uint32_t Value_Max = 1024;
	if (HAL_FLASH_Unlock() != HAL_OK) {
		printmsg("Unable to unlock the flash.!!\n");
		return UDS_NOK;
	}

	while (counter < Value_Max) {
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, (uint32_t)(curreentMemoryAddress), (uint32_t)&firmware_buffer[counter]) != HAL_OK) {
			printmsg("write to flash has failed!!\n");
			return HAL_ERROR;
		}
        curreentMemoryAddress+=4;
		counter += 1;
	}
	return HAL_FLASH_Lock();

}


void UpdateHashValue( unsigned char * hashValue[]){
    unsigned int carry  = 0;
    for(int i = 0 ; i < SHAX_DIGEST_SIZE ; i++  )
        {
            carry = carry>>8;   
            __SecDigestm.ClchashValue[i] = __SecDigestm.ClchashValue[i] + hashValue[i] + carry ;
            carry =  __SecDigestm.ClchashValue[i] + hashValue[i] + carry ;
        }
} 

void TransferData_Init(){
    blockSequence.MaxBlockSequence = (unsigned int)(RequestDownloadParam.DownloadMemorySize/4); // KByte 
    if(blockSequence.CurrentBlockSequence == 0 ){
        blockSequence.CurrentBlockSeq = 1;
    }
}

unsigned char CheckSequenceRespected(unsigned int CurrentBlockSeq){
    if( CurrentBlockSeq != (blockSequence.CurrentBlockSequence + 1)){
    return UDS_NOK; 
    }
    return UDS_Ok;
}