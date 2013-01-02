// NativeClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <assert.h>
#include <string>
#include "Stopwatch.h"
#include <objects.h>
#include <Messages.pb.h>

using namespace std;
typedef void (*req_builder)(unsigned int size,unsigned char** dataP,unsigned int* sizeP);
typedef void (*makeRequest)(unsigned int req_size,req_builder b, char** result);
void makeSharedMemoryRequest(unsigned int req_size,req_builder b,char** result);
void makePipeRequest(unsigned int req_size,req_builder b,char** result);

char* name = "cool data";
char* id = "very very cool id";
char* type = "very very cool type";

int kb = 1024;
int req_size1 = 100*kb;
int req_size2 = 10*kb;
int req_size3 = 1*kb;

std::map<int,custom_request> req;

void init_reqs(int req_size,int ids,int types){
	custom_request sc;
	sc.m_name = std::string(name);
	for (int i = 0;i<ids;i++){
		sc.m_ids.insert(sc.m_ids.end(),id);
	}
	for (int i = 0;i<types;i++){	
		sc.m_types.insert(sc.m_types.end(),type);
	}

	req.insert(make_pair(req_size,sc));
}

void req_obj(unsigned int size,unsigned char** rdataP,unsigned int* rmessageSizeP)
{	
	auto r = req[size];
	unsigned int rmessageSize = r.getSize();
	unsigned char* rdata = (unsigned char*)malloc(rmessageSize);
	r.toArray(rdata,rmessageSize);
	*rdataP = rdata;
	*rmessageSizeP = rmessageSize;
}

void req_msg(unsigned int size,unsigned char** rdataP,unsigned int* rmessageSizeP)
{	
	auto cr = req[size];
	request r;
	r.set_name(cr.m_name);

	for (auto it=cr.m_types.begin(); it!=cr.m_types.end(); it++)
	{
		std::string s = *it;
		r.add_types(s);
	}
	for (auto it= cr.m_ids.begin(); it!=cr.m_ids.end();it++){
		std::string s = *it;
		r.add_ids(s);
	}
	unsigned int rmessageSize = r.ByteSize();
	unsigned char* rdata = (unsigned char*)malloc(rmessageSize);
	r.SerializeToArray(rdata,rmessageSize);
	*rdataP = rdata;
	*rmessageSizeP = rmessageSize;
}


void req_bytes(unsigned int size,unsigned char** rdataP,unsigned int* rmessageSizeP)
{	
	unsigned int rmessageSize = size;
	unsigned char* rdata = (unsigned char*)malloc(rmessageSize);
	char* rmsg = (char*)rdata;
	memcpy_s(rmsg,rmessageSize,  "Hello from client!!!",1024);
	*rdataP = rdata;
	*rmessageSizeP = rmessageSize;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//TODO: use some lib (e.g. like in git)
	req_builder builder = req_obj;
	makeRequest call = makeSharedMemoryRequest;
	for (int i = 0; i < argc; i++){
		std::wstring  s = argv[i];
		if (s == TEXT("-m")) 
		{
			std::wstring  s = argv[i+1];
			call = s == TEXT("pipes") ? makePipeRequest : makeSharedMemoryRequest;
		}
		if (s == TEXT("-d")) 
		{
			std::wstring  s = argv[i+1];
			builder = s == TEXT("bytes") ? req_bytes : req_msg;
			builder = s == TEXT("object") ? req_obj : req_msg;
		}
	}



	init_reqs(req_size1,6666,10);
	init_reqs(req_size2,666,10);
	init_reqs(req_size3,66,10);

	Stopwatch sw;
	sw.set_mode(REAL_TIME);



	//TODO: clear respince after each request
	char* response = NULL;
	unsigned char* req = NULL;
	cout << "Client started"<< endl;
	unsigned int real_size = 0;
	builder(req_size1,&req,&real_size);
	cout << "Message size :" << real_size/kb << " kb" << endl;

	for (int i=0;i<10;i++){
		char* msr = "Request 100kb -> 1000kb";


		sw.start(msr);
		call(req_size1,builder,&response);
		sw.stop(msr);
	}	


	builder(req_size2,&req,&real_size);
	cout << "Message size :" << real_size/kb << " kb" << endl;

	for (int i=0;i<100;i++){
		char* msr = "Request 10kb - 100kb";
		sw.start(msr);
		call(req_size2,builder,&response);
		sw.stop(msr);
	}

	builder(req_size3,&req,&real_size);
	cout << "Message size :" << real_size/kb << " kb" << endl;

	for (int i=0;i<1000;i++){
		//Sleep(1);//BUG: somthing hangs here when pipes used....
		char* msr = "Request 1kb - 10kb";
		response[0]=0;
		sw.start(msr);
		call(req_size3,builder,&response);

		sw.stop(msr);
		//assert(response[0]=='H');//TODO: assert whole message and added last character to check
	}



	sw.report_all(cout);
	//cout << "Last message :" << response << endl;
	char wait(' ');
	cin >> &wait;	
	return 0;
}

void makePipeRequest(unsigned int req_size,req_builder b,char** result){
	HANDLE	hPipe = INVALID_HANDLE_VALUE;
	auto name = TEXT("\\\\.\\pipe\\FastDataServer");
	while ( !WaitNamedPipe(name, 1)); 
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
	}    


	unsigned char* rdata = NULL;
	unsigned int rmessageSize = 0;
	b(req_size,&rdata,&rmessageSize);

	unsigned char* msg = (unsigned char*)malloc(sizeof(long)+rmessageSize);

	memcpy(msg,&rmessageSize,sizeof(long));
	unsigned char* rmsg = (unsigned char*)(msg+sizeof(long));
	memcpy(rmsg,rdata, rmessageSize);//NOTE: additional copy slows down pipes - can be avoided

	unsigned long cbWritten = 0;
	WriteFile(hPipe,msg,sizeof(long)+rmessageSize,&cbWritten,NULL);

	long responseSize = 0;
	unsigned long	cbRead = 0;
	bool rSizeRead = ReadFile(hPipe,&responseSize,sizeof(long),&cbRead,NULL);
	auto response = (char*) malloc(responseSize);
	//if (!rSizeRead) cout << cbRead << endl;
	//cout << cbRead << endl;
	//if (GetLastError() == ERROR_MORE_DATA) cout << "MORE DATA" << endl;
	bool rRead = ReadFile(hPipe,response,responseSize,&cbRead,NULL);
	*result = response;
	//cout << cbRead << endl;
	//if (!rRead) cout << cbRead << endl;
	//if (GetLastError() == ERROR_MORE_DATA) cout << "MORE DATA" << endl;
	//cout << rRead << endl;
	CloseHandle(hPipe);
	//cout << response << endl;
CLEANUP:
	free(rdata);
}
void makeSharedMemoryRequest(unsigned int req_size,req_builder b,char** result){

	unsigned char* rdata = NULL;
	unsigned int rmessageSize = 0;
	b(req_size,&rdata,&rmessageSize);


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

