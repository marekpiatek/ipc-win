// NativeServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <string>
#include <assert.h>
#include "Stopwatch.h"

#define RESP_BUILDER void (int size,void** req,int* resp_s,unsigned char** resp);


using namespace std;


typedef void (*resp_builder)(long requestPacket,void** request,long* packetSizeP,unsigned char** dataP);
void reply(resp_builder b,void** request);
void replyPipes(resp_builder b,void** request);
void replySharedMem(resp_builder b,void** request);


const int kb = 1024;
std::map<int,int> reqResp;

void resp_bytes(long requestPacket,void** request,long* packetSizeP,unsigned char** dataP){
		long packetSize = 0;
	    unsigned char* data = NULL;
    	packetSize = reqResp[requestPacket];
		data = (unsigned char*)malloc(packetSize);
		char* msg = (char*)data;
		memcpy_s(msg,packetSize,  "Hello from server!!!",1024);
		*packetSizeP = packetSize;
		*dataP = data;
}



int _tmain(int argc, wchar_t* argv[])
{
	//TODO: use some lib (e.g. like in git)
	auto replier = replySharedMem;
	resp_builder builder = resp_bytes;
	for (int i = 0; i < argc; i++){
		std::wstring  s = argv[i];
		if (s == TEXT("-m")) 
		{
			std::wstring  s = argv[i+1];
			replier = s == TEXT("pipes") ? replyPipes : replySharedMem;
		}
		if (s == TEXT("-d")) 
		{
			std::wstring  s = argv[i+1];
			builder = s == TEXT("message") ? resp_bytes : resp_bytes;
		}
	}

	reqResp.insert(std::make_pair(500*kb,5000*kb)); //default and max
	reqResp.insert(std::make_pair(100*kb,1000*kb));
	reqResp.insert(make_pair(10*kb,100*kb));
	reqResp.insert(make_pair(1*kb,10*kb));

	void* request = NULL;
  


	cout << "Server started"<< endl;


	while(true){replier(builder,&request);}
	



	cout << "Last message: " << (char*)request<<endl;
	char wait(' ');
		cin >> &wait;
	return 0;
}

void replyPipes(resp_builder b,void** result){
				auto name = TEXT("\\\\.\\pipe\\FastDataServer");
		auto server = INVALID_HANDLE_VALUE;
		int inbuf = 500*1024;
		int outbuf = reqResp[outbuf];
		while(server == INVALID_HANDLE_VALUE)
	   {
		    server = CreateNamedPipe(name,             // pipe name 
          PIPE_ACCESS_DUPLEX,       // read/write access 
		  PIPE_TYPE_BYTE |       // message type pipe 
		  PIPE_READMODE_BYTE    // message-read mode 
          | PIPE_WAIT,                // blocking mode 
          PIPE_UNLIMITED_INSTANCES, // max. instances  
		  outbuf,                  // output buffer size 
         inbuf,                  // input buffer size 
          0,                        // client time-out 
          NULL);                    // default security attribute 
		if (server == INVALID_HANDLE_VALUE){
		    CloseHandle(server);
		}
		}

		while(ConnectNamedPipe(server,NULL)== 0){

		};
		long rSize = 0;
		 unsigned long	cbRead = 0;
		bool rSizeRead = ReadFile(server,&rSize,sizeof(long),&cbRead,NULL);
		//if (GetLastError() == ERROR_MORE_DATA) cout << "MORE DATA" << endl;
		//cout << rSizeRead << endl;
		auto request = (unsigned char*) malloc(rSize);
		bool rRead = ReadFile(server,request,rSize,&cbRead,NULL);
			//if (GetLastError() == ERROR_MORE_DATA) cout << "MORE DATA" << endl;
		//cout << rRead << endl;
		//cout << "Read request: " << cbRead << endl;
		//if (request !=NULL) cout << "Request:" << (char*)request << endl;
		*result = request;

		// create responsee data
		long packetSize = 0;
	    unsigned char* rData = NULL;
		b(rSize,result,&packetSize,&rData);

		unsigned char* packetData = (unsigned char*)malloc(sizeof(long)+packetSize);
	    memcpy(packetData,&packetSize,sizeof(long));
		auto messageData = packetData+ sizeof(long);
	    memcpy(messageData,rData,packetSize);
	     unsigned long	cbWritten = 0;	 
	 WriteFile(server,packetData,sizeof(long)+packetSize,&cbWritten,NULL);

		FlushFileBuffers(server);
		DisconnectNamedPipe(server);
		CloseHandle(server);
CLEANUP:
		free(packetData);
		free(rData);
}

void replySharedMem(resp_builder b,void** request){
		//pull implementation

		// listening for client request
		auto madeRequest = OpenEvent(SYNCHRONIZE,false,TEXT("ClientMadeRequestEvent"));
		while ( madeRequest == NULL){ 
			madeRequest = OpenEvent(SYNCHRONIZE,false,TEXT("ClientMadeRequestEvent"));
		}
		WaitForSingleObject(madeRequest,INFINITE);

		// reading client request size
		auto requestSizeMap = OpenFileMapping(
        FILE_MAP_READ,         
        FALSE,                  
        TEXT("SizeOfDataRequest")           
        );
		
		auto requestSizeMapView = MapViewOfFile(
        requestSizeMap,
        FILE_MAP_READ,          
        0,                     
        0,           
         sizeof(long)
        );

	    auto requestPacket = *(long*) requestSizeMapView;
		//cout << *packet<< endl;

		// copying request data
		auto requestDataMap = OpenFileMapping(
        FILE_MAP_READ,         
        FALSE,                  
        TEXT("ClientMessageData")           
        );
		
		auto requestDataView = MapViewOfFile(
        requestDataMap,
        FILE_MAP_READ,          
        0,                     
        0,           
        requestPacket
        );

		*request = malloc(requestPacket);
		memcpy(*request,requestDataView,requestPacket);
		
		// notify client that data was read
		auto readRequest = CreateEvent(NULL,false,false,TEXT("ServerReadRequestEvent"));
		SetEvent(readRequest);
	   
		//push implementation

		// create responsee data
		long packetSize = 0;
	    unsigned char* data = NULL;
		b(requestPacket,request,&packetSize,&data);
    	packetSize = reqResp[requestPacket];
		data = (unsigned char*)malloc(packetSize);
		char* msg = (char*)data;
		memcpy_s(msg,packetSize,  "Hello from server!!!",1024);
	
	// posting size of responce data
        auto sizeMap = CreateFileMapping(
        INVALID_HANDLE_VALUE,   
        NULL,                  
        PAGE_READWRITE,        
        0,                      
        sizeof(long),               
        TEXT("SizeOfData")
        );
	auto sizeMapView = MapViewOfFile(
        sizeMap,
        FILE_MAP_WRITE,          
        0,                     
        0,           
          sizeof(long)
        );

		//// post responce data
	auto dataMap = CreateFileMapping(
        INVALID_HANDLE_VALUE,   
        NULL,                  
        PAGE_READWRITE,        
        0,                      
        packetSize,               
        TEXT("ServerMessageData")
        );
		auto dataView = MapViewOfFile(
        dataMap,
        FILE_MAP_WRITE,          
        0,                     
        0,           
        packetSize
        );

		memcpy(sizeMapView,&packetSize,sizeof(long));
		memcpy(dataView,data,packetSize);
		// notify clinet about data

		auto madeResponce = CreateEvent(NULL,true,false,TEXT("ServerMadeResponse"));
	    SetEvent(madeResponce);
		//// wait for client to read it
		auto readResponse = OpenEvent(SYNCHRONIZE,false,TEXT("ClientReadResponseEvent"));
		while ( readResponse == NULL){ 
			readResponse = OpenEvent(SYNCHRONIZE,false,TEXT("ClientReadResponseEvent"));
		}
		WaitForSingleObject(readResponse,INFINITE);
CLEANUP:
		free(data);

		//cout << "Reader read";
}

	