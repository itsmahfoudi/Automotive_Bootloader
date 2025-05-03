#ifdef TRANSFERDATA_H
#define TRANSFERDATA_H

/*
TransferData (0x36) service

Descp : 
The TransferData service is used by the client to transfer data either from the client to the server (download) 
or from the server to the client (upload).
The data transfer direction is defined by the preceding RequestDownload or RequestUpload service. 
- RequestDownloadFlag = True (0x01u) when the Download from the client to the server is Requested.(False, not requested)
- RequestUploadFlag  = True (0x01u) when the Upload from the server to the client is requested. (False not requested)


Request message : 
0x36 | BlockSequenceCounetr | TransferRequestParamterRecord .

- The blockSequenceCounter of the server shall be initialized to one when receiving a RequestDownload (0x34)
*/

#define BlockSize     1024
// current states
typedef struct {
unsigned int MaxBlockSequence;
unsigned int CurrentBlockSequence;
unsigned int curreentMemoryAddress;
unsigned int ReceivedDataSize; // K Bytes
}BlocksParameter;

extern BlocksParameter blockSequence;

// NRC 
#define IMLOIF      0x13u
#define RSE         0x24u
#define ROR         0x31u
#define TDS         0x71u
#define GPF         0x72u
#define WBSC        0x73u
#define VTH         0x92u
#define VTL         0x93u

#define SHAX_DIGEST_SIZE       32


#endif