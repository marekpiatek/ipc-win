// NativeClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <assert.h>
#include <string>
#include "Stopwatch.h"


using namespace std;
void makeSharedMemoryRequest(int rmessageSize,char** result);
void makePipeRequest(int rmessageSize,char** response);
void makeRequest(int messageSize, char* response);

int _tmain(int argc, _TCHAR* argv[])
{
		//TODO: use some lib (e.g. like in git)
	auto call = makeSharedMemoryRequest;
	for (int i = 0; i < argc; i++){
		std::wstring  s = argv[i];
		if (s == TEXT("-m")) 
		{
			std::wstring  s = argv[i+1];
			call = s == TEXT("pipes") ? makePipeRequest : makeSharedMemoryRequest;
		}
	}

	int kb = 1024;
	auto rmessageSize1 = 100*kb;
	auto rmessageSize2 = kb*10;
	auto msg_size_1 = 1*kb;

	Stopwatch sw;
	sw.set_mode(REAL_TIME);



	//TODO: clear respince after each request
	char* response = NULL;
	cout << "Client started"<< endl;
	cout << "Message size :" << rmessageSize1 << endl;
	
			for (int i=0;i<10;i++){
     		char* msr = "Memory Mapped Files 100kb -> 1000kb";


		sw.start(msr);
		call(rmessageSize1,&response);
		sw.stop(msr);
}
		


	cout << "Message size :" << rmessageSize2 << endl;
	for (int i=0;i<100;i++){
		char* msr = "Memory Mapped Files 10kb - 100kb";
		sw.start(msr);
		call(rmessageSize2,&response);
		sw.stop(msr);
	}
	
		cout << "Message size :" << msg_size_1 << endl;
	for (int i=0;i<1000;i++){
		//Sleep(1);//BUG: somthing hangs here when pipes used....
		char* msr = "Memory Mapped Files 1kb - 10kb";
		response[0]=0;
		sw.start(msr);
		call(msg_size_1,&response);

		sw.stop(msr);
		assert(response[0]=='H');//TODO: assert whole message and added last character to check
	}


	
		sw.report_all(cout);
	cout << "Last message :" << response << endl;
		char wait(' ');
		cin >> &wait;	
	return 0;
}

void makePipeRequest(int rmessageSize,char** result){
		HANDLE	hPipe;
				auto name = TEXT("\\\\.\\pipe\\FastDataServer");
   while (1) 
   { 

		 hPipe = CreateFile( 
         name,   
         GENERIC_READ | 
         GENERIC_WRITE, 
         0,              
         NULL,         
         OPEN_EXISTING,  
         FILE_ATTRIBUTE_NORMAL,              
         NULL);   
 

 
      if (hPipe != INVALID_HANDLE_VALUE) 
         break; 

	   while ( !WaitNamedPipe(name, 10)); 

   } 

    
      
  // 	    unsigned long	cbWritten = 0;
		//byte* msg = (byte*)malloc(rmessageSize);
		//char* rmsg = (char*)msg;
		//memcpy_s(rmsg,rmessageSize,  "Hello from client!!!",1024);
		//WriteFile(hPipe,&rmessageSize,sizeof(long),&cbWritten,NULL);
		//WriteFile(hPipe,rmsg,rmessageSize,&cbWritten,NULL);
	
	    unsigned long cbWritten = 0;
		byte* msg = (byte*)malloc(sizeof(long)+rmessageSize);
		memcpy(msg,&rmessageSize,sizeof(long));
		char* rmsg = (char*)(msg+sizeof(long));
		memcpy_s(rmsg,rmessageSize,  "Hello from client!!!",1024);
		WriteFile(hPipe,msg,sizeof(long)+rmessageSize,&cbWritten,NULL);

		long responseSize = 0;
		unsigned long	cbRead = 0;
		bool rSizeRead = ReadFile(hPipe,&responseSize,sizeof(long),&cbRead,NULL);
		auto response = (char*) malloc(responseSize);
		//cout << rSizeRead << endl;
		//if (GetLastError() == ERROR_MORE_DATA) cout << "MORE DATA" << endl;
		bool rRead = ReadFile(hPipe,response,responseSize,&cbRead,NULL);
		*result = response;
		//cout << response << endl;
			//if (GetLastError() == ERROR_MORE_DATA) cout << "MORE DATA" << endl;
		//cout << rRead << endl;
    	CloseHandle(hPipe);
		//cout << response << endl;
}
void makeSharedMemoryRequest(int rmessageSize,char** result){
	
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

