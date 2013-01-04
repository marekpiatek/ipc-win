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

typedef void (*resp_builder)(unsigned int size,unsigned char* resp_data);

typedef void (*makeRequest)(unsigned int req_size,req_builder b, resp_builder br,void** result,int* resultSize);
void makeSharedMemoryRequest(unsigned int req_size,req_builder b,resp_builder br,void** result,int* resultSize);
void makePipeRequest(unsigned int req_size,req_builder b,resp_builder br,void** result,int* resultSize);

char* name = "cool data";
char* id = "very very cool id";
char* type = "very very cool type";

int kb = 1024;
int req_size100 = 100*kb;
int req_size50 = 50*kb;
int req_size10 = 10*kb;
int req_size1 = 1*kb;


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

void resp_bytes(unsigned int size,unsigned char* resp_data){
	//do nothing
}

void resp_msg(unsigned int size,unsigned char* resp_data){
	response r;
	r.ParseFromArray(resp_data,size);
	//NOTE: we have to translate it to custom object in order to have more real mesurment
	//NOTE: and to satisfie public API
	custom_response cr;
	for (auto it = r.data().begin();it!=r.data().end();it++){
		cr.m_data.insert(cr.m_data.end(),*it);
	}

}

void resp_obj(unsigned int size,unsigned char* resp_data){
	custom_response r;
	int readS = 0;
	r.fromArray(resp_data,&readS);
	
}

void req_bytes(unsigned int size,unsigned char** rdataP,unsigned int* rmessageSizeP)
{	
	unsigned int rmessageSize = size;
	unsigned char* rdata = (unsigned char*)malloc(rmessageSize);
	char* rmsg = (char*)rdata;
	*rdataP = rdata;
	*rmessageSizeP = rmessageSize;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//TODO: use some lib (e.g. like in git)
	req_builder builder = req_msg;
	resp_builder br = resp_msg;
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
			builder = s == TEXT("bytes") ? req_bytes : (s == TEXT("object") ? req_obj : req_msg);
			br = s == TEXT("bytes") ? resp_bytes : (s == TEXT("object") ? resp_obj : resp_msg);
		}
	}


	init_reqs(req_size100,5000,10);
	init_reqs(req_size50,2500,10);
	init_reqs(req_size10,500,10);
	init_reqs(req_size1,50,10);

	Stopwatch sw;
	sw.set_mode(REAL_TIME);

	//TODO: clear response after each request
	void* response = NULL;
	int resp_size = -1;
	unsigned char* req = NULL;
	cout << "Client started"<< endl;
	unsigned int real_size = 0;


	char* msr100 = "Client process requests ~100kb and gets ~1000kb response of server process";
	char* msr50 = "Client process requests ~50kb and gets ~500kb response of server process";
	char* msr10 = "Client process requests ~10kb and gets ~100kb response  of server process";
	char* msr1 = "Client process requests ~1kb and gets ~10kb response  of server process";

	builder(req_size100,&req,&real_size);
	cout << "Request size :" << real_size/kb << " kb" << endl;
	for (int i=0;i<10;i++){
		
		sw.start(msr100);
		call(req_size100,builder,br,&response,&resp_size);
		sw.stop(msr100);
	}	
	cout << "Response size was :" << resp_size/kb << " kb" <<endl;


	builder(req_size50,&req,&real_size);
	cout << "Request size :" << real_size/kb << " kb" << endl;
	for (int i=0;i<20;i++){
		
		sw.start(msr50);
		call(req_size50,builder,br,&response,&resp_size);
		sw.stop(msr50);
	}	
	cout << "Response size was :" << resp_size/kb << " kb" <<endl;

	builder(req_size10,&req,&real_size);
	cout << "Message size :" << real_size/kb << " kb" << endl;
	for (int i=0;i<100;i++){
		
		sw.start(msr10);
		call(req_size10,builder,br,&response,&resp_size);
		sw.stop(msr10);
	}
	cout << "Response size was :" << resp_size/kb << " kb" <<endl;

	builder(req_size1,&req,&real_size);
	cout << "Message size :" << real_size/kb << " kb" << endl;
	for (int i=0;i<1000;i++){
		//Sleep(1);
		//BUG: sometimes hangs here when pipes used....
		sw.start(msr1);
		call(req_size1,builder,br,&response,&resp_size);
		sw.stop(msr1);
	}
	cout << "Response size was :" << resp_size/kb << " kb" <<endl;
	//TODO: make assertion of response

	sw.report(msr100);
	sw.report(msr50);
	sw.report(msr10);
	sw.report(msr1);

	char wait(' ');
	cin >> &wait;	
	return 0;
}

void makePipeRequest(unsigned int req_size,req_builder b,resp_builder br,void** result,int* resultSize){
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
	auto response = (unsigned char*) malloc(responseSize);
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
	br(rSizeRead,response);
	*resultSize = rSizeRead;
	//cout << response << endl;
CLEANUP:
	free(rdata);
}
void makeSharedMemoryRequest(unsigned int req_size,req_builder b,resp_builder br,void** result,int* resultSize){

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
	br(packet,data);
	*result = msg;
	*resultSize = packet;
}

