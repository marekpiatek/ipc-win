// NativeClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <string>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	auto readEvent = CreateEvent(NULL,true,false,TEXT("ClientReadEvent"));
	while(true)
	{
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
		cout << *packet;
		SetEvent(readEvent);

	}
	return 0;
}

