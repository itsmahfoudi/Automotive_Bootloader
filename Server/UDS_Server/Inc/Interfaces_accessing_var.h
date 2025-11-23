#ifndef INTERFACES_ACCESSING_VAR_H
#define INTERFACES_ACCESSING_VAR_H

// The common checking macros 
#define UDS_OK             0x00U
#define UDS_NOK            0x01U

// Security State
#define ACCESSDENIED      0x00U
#define ACCESSPERMETTED   0x10U

/*
Client Request 
*/
#define NoRequest           0x00U
#define RequestDownload     0x01U
#define RequestUplaod       0x02U
// Security Access
/* FblDiagSession : is like the ECU State 
*  FblDiagSession = FblDiagProgrammingSession in case of a Prog request
*/

typedef struct {
    unsigned char SecurityAccess;
    unsigned char FblDiagSession ;
}SessionParameters;

extern SessionParameters sessionParameters;


// this data structure is used to hold the parameters of the RequestDownload or RequestUpload Services
typedef struct {
    unsigned int DownloadMemorySize;  // Memory Size K Bytes
    unsigned int DownloadStartAddr;   // Start Address 
}TransferRequestParameter;


// Used to store the BlockSequence and 4 KBytes received on Communication Bus
typedef struct {
    unsigned int blockSequence;
    unsigned int Data[1024]; // 4 KBytes 
    //maybe we should declare as an array of chars, since we're manipulating bytes.
    // unsigned char Data[4096];
}TransferDataBuffer;

typedef struct {
    unsigned char ClchashValue[32];
    unsigned char ReceivedHashValue[32];
}SecureDigests;

// Holds the parameters from the request Download service 
TransferRequestParameter RequestDownloadParam;
extern unsigned int ClientRequest;

TransferDataBuffer TransferDataBuffer;

SecureDigests __SecDigest;



/* Bootloader States */
#define FblDiagStateDefaultSession     0U
#define FblDiagProgrammingSession      1U
#define FblDiagExtendedSession         2U


/* Negatif Responce */
#define NEGATIVE_RESPONSE_SID          0x7F
/* Diagnostic services */
#define DiagSidDiagnosticSessionControl                 0x10u    /**< Service ID - Diagnostic session control */
#define DiagSidEcuReset                                 0x11u    /**< Service ID - ECU reset */
#define DiagSidSecurityAccess                           0x27u    /**< Service ID - Security Access */
#define DiagSidRoutineControl                           0x31u    /**< Service ID - Routine Control */
#define DiagSidRequestDownload                          0x34u    /**< Service ID - Request Download */
#define DiagSidTransferData                             0x36u    /**< Service ID - Transfer Data */
#define DiagSidRequestTransferExit                      0x37u    /**< Service ID - Request Transfer Exit */

// Handling the Session flag
#define DiagStateIsProgrammingSession()          (sessionParameters.FblDiagSession == FblDiagProgrammingSession )
#define DiagStateIsDefaultSession()              (sessionParameters.FblDiagSession == FblDiagStateDefaultSession )
#define DiagStateIsExtendedSession()             (sessionParameters.FblDiagSession == FblDiagExtendedSession )
#define DiagSetStateSession(state)               (sessionParameters.FblDiagSession = state)
#define DiagGetCurrentStateSession()             (sessionParameters.FblDiagSession )

// Handling the Security state 
#define DiagSetSecurityDenied()                  (sessionParameters.SecurityAccess = ACCESSDENIED;)

/* Response Flag Handling       */
#define DiagSetNoResponse()             (diagResponseFlag |= DiagPutNoResponse)
#define DiagSetPositiveResponse()       (diagResponseFlag |= DiagPutNoResponse)

// Common Macro functions used to perform commun treatement
#define CopyDataBetwenTwoTables(RV_stored , PV_From )        \
        for(int i = 0 ; i < UDS_FrameLength ; i++ ){ \
            RV_stored[i] = PV_From[i]; }\

#define ResetFrameValue(UDS_Frame)              \
            for(int i = 0  i < UDS_FrameLength ; i++){\
                UDS_Frame[i] = 0xFFU;\
            }\

/*
- Data Element : DownloadStartAddr
Read Interface  : unsigned int RTE_Read_DataElement_DownloadStartAddr();
Write Interface : void   RTE_Write_DataElement_DownloadStartAddr(unsigned int newValue);

*/
extern volatile unsigned int DownloadStartAddr; // store the start address to performing the download 
unsigned int RTE_Read_DataElement_DownloadStartAddr();
void   RTE_Write_DataElement_DownloadStartAddr(unsigned int newValue);

/*

- Data Element : DownloadMemorySize
Read Interface  : unsigned int RTE_Read_DataElement_DownloadStartAddr();
Write Interface : void   RTE_Write_DataElement_DownloadStartAddr(unsigned int newValue);
*/
extern volatile unsigned int DownloadMemorySize; 
unsigned int RTE_Read_DataElement_DownloadMemorySize();
void   RTE_Write_DataElement_DownloadMemorySize(unsigned int newValue);

/*

- Data Element : SecurityAccessState
Read Interface  : unsigned char RTE_Read_DataElement_SecurityAccessState();
Write Interface : void RTE_Write_DataElement_SecurityAccessState(unsigned char newValue );
*/
extern  volatile unsigned char SecurityAccessState;
unsigned char RTE_Read_DataElement_SecurityAccessState();
void RTE_Write_DataElement_SecurityAccessState(unsigned char newValue );

/*

- Data Element : RequestDataDownloadFlag
Read Interface : unsigned char RTE_Read_DataELemnt_RequestDownloadFlag()
Write Interface : void RTE_Write_DataElemebt_RequestDownloadFlag(unsigned char NewValue)
*/
extern unsigned char RequestDownloadFlag;
unsigned char RTE_Read_DataELemnt_RequestDownloadFlag();
void  RTE_Write_DataElement_SecurityAccessState(unsigned char newValue );



#endif