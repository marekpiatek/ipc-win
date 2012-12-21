// NativeClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <string>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	auto times = 10;
	
	auto messageSize = 1024*1024;
	auto packetSize = messageSize;
	byte* data = (byte*)malloc(messageSize);
	char* msg = (char*)data;


	auto readEvent = CreateEvent(NULL,true,false,TEXT("ClientReadEvent"));
	for (int i=0;i<times;i++){
		auto wait = OpenEvent(SYNCHRONIZE,false,TEXT("ServerWriteEvent"));
		while (wait == NULL){
			wait = OpenEvent(SYNCHRONIZE,false,TEXT("ServerWriteEvent"));
		}
		WaitForSingleObject(wait,INFINITE);
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

	    auto packet = (long*) sizeMapView;
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
        packetSize
        );

		memcpy_s(msg,messageSize, dataView ,messageSize);
		
		SetEvent(readEvent);

	}

	cout << "Last message :" << msg << endl;
		char wait(' ');
		cin >> &wait;	
	return 0;
}

