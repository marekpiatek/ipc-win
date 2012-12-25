// NativeServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <string>
#include <assert.h>
#include "Stopwatch.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	auto times = 10;

	void* request = NULL;

	auto messageSize = 1024*1024;
	auto packetSize = messageSize;

	auto name = TEXT("\\\\.\\pipe\\FastDataServer");

	cout << "Server started"<< endl;

	for (int i=0;i<times;i++){
		auto server = INVALID_HANDLE_VALUE;
	   while(server == INVALID_HANDLE_VALUE)
	   {
		    server = CreateNamedPipe(name,             // pipe name 
          PIPE_ACCESS_DUPLEX,       // read/write access 
		  PIPE_TYPE_BYTE |       // message type pipe 
		  PIPE_READMODE_BYTE |   // message-read mode 
          PIPE_WAIT,                // blocking mode 
          PIPE_UNLIMITED_INSTANCES, // max. instances  
          packetSize,                  // output buffer size 
          packetSize,                  // input buffer size 
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
		assert (ReadFile(server,&rSize,sizeof(long),&cbRead,NULL));
		cout << rSize<<endl;
	
	}


	cout << (char*)request<<endl;
	char wait(' ');
		cin >> &wait;
	return 0;
}

void replySharedMem(){
	//	//pull implementation

	//	// listening for client request
	//	auto madeRequest = OpenEvent(SYNCHRONIZE,false,TEXT("ClientMadeRequestEvent"));
	//	while ( madeRequest == NULL){ 
	//		madeRequest = OpenEvent(SYNCHRONIZE,false,TEXT("ClientMadeRequestEvent"));
	//	}
	//	WaitForSingleObject(madeRequest,INFINITE);

	//	// reading client request size
	//	auto requestSizeMap = OpenFileMapping(
 //       FILE_MAP_READ,         
 //       FALSE,                  
 //       TEXT("SizeOfDataRequest")           
 //       );
	//	
	//	auto requestSizeMapView = MapViewOfFile(
 //       requestSizeMap,
 //       FILE_MAP_READ,          
 //       0,                     
 //       0,           
 //        sizeof(long)
 //       );

	//    auto requestPacket = *(long*) requestSizeMapView;
	//	//cout << *packet<< endl;

	//	// copying request data
	//	auto requestDataMap = OpenFileMapping(
 //       FILE_MAP_READ,         
 //       FALSE,                  
 //       TEXT("ClientMessageData")           
 //       );
	//	
	//	auto requestDataView = MapViewOfFile(
 //       requestDataMap,
 //       FILE_MAP_READ,          
 //       0,                     
 //       0,           
 //       requestPacket
 //       );

	//	request = malloc(requestPacket);
	//	memcpy(request,requestDataView,requestPacket);
	//	
	//	// notify client that data was read
	//	auto readRequest = CreateEvent(NULL,false,false,TEXT("ServerReadRequestEvent"));
	//	SetEvent(readRequest);
	//   
	//	//push implementation

	//	// create responsee data

	//byte* data = (byte*)malloc(messageSize);

	//char* msg = (char*)data;
	//memcpy_s(msg,messageSize,  "Hello from server!!!",1024);
	//
	//// posting size of responce data
 //       auto sizeMap = CreateFileMapping(
 //       INVALID_HANDLE_VALUE,   
 //       NULL,                  
 //       PAGE_READWRITE,        
 //       0,                      
 //       sizeof(long),               
 //       TEXT("SizeOfData")
 //       );
	//auto sizeMapView = MapViewOfFile(
 //       sizeMap,
 //       FILE_MAP_WRITE,          
 //       0,                     
 //       0,           
 //         sizeof(long)
 //       );

	//	//// post responce data
	//auto dataMap = CreateFileMapping(
 //       INVALID_HANDLE_VALUE,   
 //       NULL,                  
 //       PAGE_READWRITE,        
 //       0,                      
 //       packetSize,               
 //       TEXT("ServerMessageData")
 //       );
	//	auto dataView = MapViewOfFile(
 //       dataMap,
 //       FILE_MAP_WRITE,          
 //       0,                     
 //       0,           
 //       packetSize
 //       );

	//	memcpy(sizeMapView,&packetSize,sizeof(long));
	//	memcpy(dataView,data,packetSize);
	//	// notify clinet about data

	//	auto madeResponce = CreateEvent(NULL,true,false,TEXT("ServerMadeResponse"));
	//    SetEvent(madeResponce);
	//	//// wait for client to read it
	//	auto readResponse = OpenEvent(SYNCHRONIZE,false,TEXT("ClientReadResponseEvent"));
	//	while ( readResponse == NULL){ 
	//		readResponse = OpenEvent(SYNCHRONIZE,false,TEXT("ClientReadResponseEvent"));
	//	}
	//	WaitForSingleObject(readResponse,INFINITE);

		////cout << "Reader read";
}

	