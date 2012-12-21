// NativeServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <string>
#include "Stopwatch.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	auto times = 10;
	Stopwatch sw;
	sw.set_mode(REAL_TIME);
	sw.start("Memory Mapped Files");

	auto messageSize = 1024*1024;
	auto packetSize = messageSize;
	byte* data = (byte*)malloc(messageSize);
	char* msg = (char*)data;
	memcpy_s(msg,messageSize,  "Hello from server!!!",1024);

	auto writeEvent = CreateEvent(NULL,true,false,TEXT("ServerWriteEvent"));
	//cout << "Server started"<< endl;
	for (int i=0;i<times;i++){
	

	
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
		auto readerEvent = OpenEvent(SYNCHRONIZE,false,TEXT("ClientReadEvent"));
		while ( readerEvent == NULL){ 
			readerEvent = OpenEvent(SYNCHRONIZE,false,TEXT("ClientReadEvent"));
		}
	    SetEvent(writeEvent);
		WaitForSingleObject(readerEvent,INFINITE);
		ResetEvent(writeEvent);
		//cout << "Reader read";
	
	}

	sw.stop("Memory Mapped Files");
	sw.report_all(cout);
	char wait(' ');
		cin >> &wait;
	return 0;
}

