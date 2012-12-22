// NativeClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <string>
#include "Stopwatch.h"

using namespace std;
void makeSharedMemoryRequestResponce(int rmessageSize,char** result);

int _tmain(int argc, _TCHAR* argv[])
{
		
	Stopwatch sw;
	sw.set_mode(REAL_TIME);
	


	auto times = 10;
	auto rmessageSize = 1024*128;
	auto rpacketSize = rmessageSize;
	char* msg = NULL;
	cout << "Client started"<< endl;

	for (int i=0;i<times;i++){
		char* msr = "Memory Mapped Files";
		sw.start(msr);
        makeSharedMemoryRequestResponce(rpacketSize,&msg);
    	sw.stop(msr);
	}
	
	sw.report_all(cout);

	cout << "Last message :" << msg << endl;
		char wait(' ');
		cin >> &wait;	
	return 0;
}


void makeSharedMemoryRequestResponce(int rmessageSize,char** result){
	
	byte* rdata = (byte*)malloc(rmessageSize);

	char* rmsg = (char*)rdata;
	memcpy_s(rmsg,rmessageSize,  "Hello from client!!!",1024);
	
	// posting size of request data
        auto rsizeMap = CreateFileMapping(
        INVALID_HANDLE_VALUE,   
        NULL,                  
        PAGE_READWRITE,        
        0,                      
        sizeof(long),               
        TEXT("SizeOfDataRequest")
        );
		auto rsizeMapView = MapViewOfFile(
        rsizeMap,
        FILE_MAP_WRITE,          
        0,                     
        0,           
          sizeof(long)
        );

		// post responce data
		auto rdataMap = CreateFileMapping(
        INVALID_HANDLE_VALUE,   
        NULL,                  
        PAGE_READWRITE,        
        0,                      
        rmessageSize,               
        TEXT("ClientMessageData")
        );
		auto rdataView = MapViewOfFile(
        rdataMap,
        FILE_MAP_WRITE,          
        0,                     
        0,           
        rmessageSize
        );

		memcpy(rsizeMapView,&rmessageSize,sizeof(long));
		memcpy(rdataView,rdata,rmessageSize);

		
	    auto madeRequest = CreateEvent(NULL,false,false,TEXT("ClientMadeRequestEvent"));
	    SetEvent(madeRequest);
		// wait for server to read it
		auto readRequest = OpenEvent(SYNCHRONIZE,false,TEXT("ServerReadRequestEvent"));
		while (readRequest == NULL){
			readRequest = OpenEvent(SYNCHRONIZE,false,TEXT("ServerReadRequestEvent"));
		}
		WaitForSingleObject(readRequest,INFINITE);

		// read responce



		
		auto madeResponse = OpenEvent(SYNCHRONIZE,false,TEXT("ServerMadeResponse"));
		while (madeResponse == NULL){
			madeResponse = OpenEvent(SYNCHRONIZE,false,TEXT("ServerMadeResponse"));
		}
		WaitForSingleObject(madeResponse,INFINITE);
		auto sizeMap = OpenFileMapping(
        FILE_MAP_READ,         
        FALSE,                  
        TEXT("SizeOfData")           
        );
		
		auto sizeMapView = MapViewOfFile(
       sizeMap,
       FILE_MAP_READ,          
       0,                     
        0,           
         sizeof(long)
        );

	    auto packet = *(long*) sizeMapView;
		//cout << *packet<< endl;

		auto dataMap = OpenFileMapping(
       FILE_MAP_READ,         
        FALSE,                  
        TEXT("ServerMessageData")           
        );
		
		auto dataView = MapViewOfFile(
        dataMap,
        FILE_MAP_READ,          
        0,                     
        0,           
        packet
        );

		

		byte* data = (byte*)malloc(packet);
	    char* msg = (char*)data;
		memcpy_s(msg,packet, dataView ,packet);
	     auto readResponse = CreateEvent(NULL,true,false,TEXT("ClientReadResponseEvent"));
		SetEvent(readResponse);
		*result = msg;
}

