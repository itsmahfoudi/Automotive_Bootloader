#include "RoutineControl.h"
#include "config.h"


uint8_t RoutineControl_Main(){
    HAL_StatusTypeDef      status          = HAL_OK;
    uint16_t Routine_run ;
    uint32_t mem_start_addr ;
    uint32_t mem_size; 
    Routine_run = (RxData[2]<<8)| RxData[3] ;
    /* */
    switch(Routine_run){
        case(Erasing_mem): 
            /*RID Supported in active session */
            if( ACTIVE_SESSION != FblDiagProgrammingSession ){ /* RID Supported in active session */
                SendDiagNegativeResponce(ROOR);/* Request out of Range : */
                status = HAL_ERROR ;
                return status;
            }
            /* RID Security Check */
            if(!GET_SECURITYACCESS_VALID()){
                SendDiagNegativeResponce(SAD);
                status = HAL_ERROR ;
                return status;
            }
            /* SubFunction supported for RID */
            if(RxData[1] != StartRoutine ){
                SendDiagNegativeResponce(SFNS);
                status = HAL_ERROR ;
                return status;
            }
            /* Get MEM Address   */
            mem_start_addr =(RxData[4]<<16) | (RxData[5]<<8) | RxData[6];
            if(mem_start_addr >= MEM_End | mem_start_addr <MEM_Start ){
                SendDiagNegativeResponce(CNC);
                status = HAL_ERROR ;
                return status;
            }
            /* Get Size          */
            mem_size = RxData[7]; 
            mem_start_addr = 0x08000000 | mem_start_addr;
            mem_size = mem_size*1024;
            status = Erasing_mem_routine(mem_start_addr , mem_size);
            if(status != HAL_OK){
                SendDiagNegativeResponce(CNC);
            }
        break;
        default :
        /* Send negative resp */
            SendDiagNegativeResponce(ROOR);
            status = HAL_ERROR ;
        break;
    }

    return status;
}




/* Erasing Memory Routine */
uint8_t Erasing_mem_routine(uint32_t MemStartAddr , uint32_t MemSize){
    HAL_StatusTypeDef      status          = HAL_OK;
    FLASH_EraseInitTypeDef EraseInitStruct = {};
    uint32_t mem_end   = 0x08000000 | MEM_End ; 
    
    if((MemStartAddr + MemSize) > mem_end ){
        MemSize = MemSize - ((MemStartAddr + MemSize) - mem_end );
    }
    /* Erasing the memory */
    /*
    1 - get mem banks
    2 - get the number of sectors to erase 
    3 - erase sector by sector 
    */
    uint32_t tmp_bank_num = 0x00000001U ;
    uint8_t NbOfSectors;
    if(MCU_Flash.bank_num > 1 ){
        tmp_bank_num = Get_bank_num( MCU_FLASH , MemStartAddr )
    }
    FirstSector =  Get_Number_Of_Sectors(MCU_FLASH,&NbOfSectors,MemStartAddr,tmp_bank_num);
    if(FirstSector == 0xFFu){
        /*The enter address is not alligned with sector's addresses */
        status = HAL_ERROR;
        return status;
    }
    HAL_FLASH_Unlock();


    EraseInitStruct.banks         = tmp_bank_num;
    EraseInitStruct.TypeErase     = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.NbPages       = NbOfSectors;
	EraseInitStruct.Page          = FirstSector;
    if(HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK) {
		status = HAL_ERROR;
    }else {
        SendDiagPositiveResponce(RoutineFinished);
    }
    HAL_FLASH_lock();
    
    return status;
}





uint8_t Get_Number_Of_Sectors(MemoryOrganization MCU_Flash,uint8_t*num_sectors,uint32_t ErasingStartAddr,uint32_t bank_num  ){
    /*Start address and end address of the bank */
    uint32_t tmp_start_addr = (MCU_FLASH.Flash_start_addr + (MCU_FLASH.bank_size*1024*(bank_num-1))); 
    uint32_t tmp_end_addr   = tmp_start_addr + (MCU_FLASH.bank_size*1024);
    uint8_t FirstSector = 0xFFu;
    /* get FirstSector */
    *num_sectors = 0;

    for(int i = 0 ; tmp_start_addr < tmp_end_addr ; i++){
        /* Getting the First sector number*/
        if(ErasingStartAddr == tmp_start_addr){
            FirstSector = i;
        }
        if(FirstSector != 0xFFu){
            *num_sectors = *num_sectors + 1;
        }
    
        tmp_start_addr = tmp_start_addr + (MCU_FLASH.sector_size*1024);
    }

/* return the first sector */
    return FirstSector;
}


uint32_t Get_bank_num(  MemoryOrganization MCU_Flash , uint32_t address  ){
    uint32_t tmp_addr = MCU_Flash.Flash_start_addr;
    uint32_t bank_num = 0x00000001U;
    while(tmp_addr != MCU_Flash.Flash_end_addr){
        tmp_addr = tmp_addr + (MCU_Flash.bank_size*1024) ;
        if(address  < tmp_addr  ){
            return bank_num;
        }
        bank_num++;
    }
    return bank_num;
}

void SendDiagPositiveResponce(  uint16_t DID , uint8_t Value  ){
    uint8_t UDS_Frame[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    UDS_Frame[0] = RC_PositiveResp_SID;
    UDS_Frame[1] = StartRoutine;
    UDS_Frame[2] = 0xFFu; /* RID MSB */
    UDS_Frame[3] = 0x00u; /* RID LSB */
    UDS_Frame[4] = RoutineFinished; /* Routine info */
    UDS_SetTxFrame(UDS_Frame);
    Diag_Send_Responce();
}

void SendDiagNegativeResponce(  uint8_t NRC  ){
    uint8_t UDS_Frame[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    UDS_Frame[0] = 0x7Fu;
    UDS_Frame[1] = 0x31u;
    UDS_Frame[2] = NRC;
    UDS_SetTxFrame(UDS_Frame);
    Diag_Send_Responce();
}