// NativeServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <string>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	auto writeEvent = CreateEvent(NULL,true,false,TEXT("ServerWriteEvent"));
	cout << "Server started";
	while(true)
	{

	
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
		long packet = 100;
		memcpy(sizeMapView,&packet,sizeof(long));
		
		auto readerEvent = OpenEvent(SYNCHRONIZE,false,TEXT("ClientReadEvent"));
		while ( readerEvent == NULL){ 
			readerEvent = OpenEvent(SYNCHRONIZE,false,TEXT("ClientReadEvent"));
		}
	    SetEvent(writeEvent);
		WaitForSingleObject(readerEvent,INFINITE);
		ResetEvent(writeEvent);
		cout << "Reader read";
		char k(' ');
		cin >> &k;
	}

	return 0;
}

