// NativeServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <string>
#include <assert.h>
#include "Stopwatch.h"
#include <objects.h>
#include <Messages.pb.h>

#define RESP_BUILDER void (int size,void** req,int* resp_s,unsigned char** resp);


using namespace std;


typedef void (*resp_builder)(long requestPacket,unsigned char* request,long* packetSizeP,unsigned char** dataP);
typedef HANDLE (*client_init)();
typedef void (*reply)(HANDLE transport,resp_builder b,void** result);


void replyPipes(HANDLE transport,resp_builder b,void** result);
void replySharedMem(HANDLE transport,resp_builder b,void** result);


const int kb = 1024;
std::map<int,int> reqResp;
std::map<int,custom_response> reqRespObj;

void insert(int reqc){
	custom_response cresp;
			for (int i=0;i<reqc;i++){
			cresp.m_data.insert(cresp.m_data.end(), "123456789 123456789");
		}
			reqRespObj.insert(make_pair(reqc,cresp));
}

HANDLE empty_init(){return INVALID_HANDLE_VALUE;}

wchar_t* name = TEXT("\\\\.\\pipe\\FastDataServer");
HANDLE  pipe_init(){
			auto transport = INVALID_HANDLE_VALUE;
	    //NOTE: tried different sizes (e.g. 30 bytes) but found very small difference for running tests
		int inbuf = 500*kb;
		int outbuf = reqResp[outbuf];

   	while(transport == INVALID_HANDLE_VALUE)
	   {
		    transport = CreateNamedPipe(name,             // pipe name 
          PIPE_ACCESS_DUPLEX,       // read/write access 
		  PIPE_TYPE_BYTE |       // message type pipe 
		  PIPE_READMODE_BYTE    // message-read mode 
          | PIPE_WAIT,                // blocking mode 
          PIPE_UNLIMITED_INSTANCES, // max. instances  
		  outbuf,                  // output buffer size - tests
         inbuf,                  // input buffer size 
          0,                        // client time-out 
          NULL);                    // default security attribute 
		if (transport == INVALID_HANDLE_VALUE){
		    CloseHandle(transport);
		}
		}
	    while(ConnectNamedPipe(transport,NULL)== 0){		};
	return transport;
}

void resp_bytes(long requestPacket,unsigned char* request,long* packetSizeP,unsigned char** dataP){
		long packetSize = 0;
	    unsigned char* data = NULL;
    	packetSize = reqResp[requestPacket];
		data = (unsigned char*)malloc(packetSize);
		char* msg = (char*)data;
		memcpy_s(msg,packetSize,  "Hello from server!!!",1024);
		*packetSizeP = packetSize;
		*dataP = data;
}

void resp_msg(long requestPacket,unsigned char* req,long* packetSizeP,unsigned char** dataP){
		long packetSize = 0;
	    unsigned char* data = NULL;
		request cr;
		int sizeOfReq = 0;
		cr.ParseFromArray(req,requestPacket);

		// create response
		//NOTE: do not convert into custom object before usage because in end implementation can be also avoided
		int itemsc = cr.types().size()*cr.ids().size();
		custom_response ccresp = reqRespObj[itemsc];	
		response resp;
		for (auto it = ccresp.m_data.begin();it!=ccresp.m_data.end();it++){
			resp.add_data(*it);
		}
		packetSize = resp.ByteSize();
		*packetSizeP = packetSize;
		data = (unsigned char*)malloc(packetSize);
		resp.SerializeToArray(data,packetSize);
		*dataP = data;
}

void resp_obj(long requestPacket,unsigned char* request,long* packetSizeP,unsigned char** dataP){
		long packetSize = 0;
	    unsigned char* data = NULL;
		custom_request cr;
		int sizeOfReq = 0;
		cr.fromArray(request,&sizeOfReq);

		// create response
        int itemsc = cr.m_types.size()*cr.m_ids.size();
		custom_response cresp = reqRespObj[itemsc];		
		packetSize = cresp.getSize();
		*packetSizeP = packetSize;
		data = (unsigned char*)malloc(packetSize);
		cresp.toArray(data,packetSize);
		*dataP = data;
}

// resue as much resources as possible when doing responces, if not then release and auire them again and again
//KEEP-ALIVE kind of
bool reuse = false;
unsigned char* resp_data ;
unsigned char* request_data ;
int _tmain(int argc, wchar_t* argv[])
{
	//TODO: use some lib (e.g. like in git)
	reply replier = replySharedMem;
	resp_builder builder = resp_msg;
	client_init init  = empty_init;

	for (int i = 0; i < argc; i++){
		std::wstring  s = argv[i];
		if (s == TEXT("-m")) 
		{
			std::wstring  s = argv[i+1];
			replier = s == TEXT("pipes") ? replyPipes : replySharedMem;
			init = s == TEXT("pipes") ? pipe_init : empty_init;
		}
		if (s == TEXT("-d")) 
		{
			std::wstring  s = argv[i+1];
			builder = s == TEXT("bytes") ? resp_bytes : (s == TEXT("object") ? resp_obj : resp_msg);
		}
	    if (s == TEXT("-r")) 
		{
			reuse = true;
		}
	}

	reqResp.insert(std::make_pair(500*kb,5000*kb)); //default and max
	resp_data = (unsigned char*)malloc(sizeof(long)+reqResp[500*kb]*kb);
	request_data =  (unsigned char*)malloc(sizeof(long)+500*kb);
	reqResp.insert(std::make_pair(100*kb,1000*kb));
	reqResp.insert(std::make_pair(50*kb,500*kb));
	reqResp.insert(make_pair(10*kb,100*kb));
	reqResp.insert(make_pair(1*kb,10*kb));
	reqResp.insert(make_pair(0.1*kb,1*kb));

	insert(5000*10);
	insert(2500*10);
	insert(500*10);
	insert(50*10);
	insert(5*10);

	void* request = NULL;
  


	cout << "Server started"<< endl;

	HANDLE transport = INVALID_HANDLE_VALUE;
	if (reuse) transport = init();
	while(true){replier(transport,builder,&request);}
	



	cout << "Last message: " << (char*)request<<endl;
	char wait(' ');
		cin >> &wait;
	return 0;
}



void replyPipes(HANDLE transport,resp_builder b,void** result){
			
	     //TODO: handle client closed
	    if (!reuse) transport = pipe_init();
	
		long rSize = 0;
		 unsigned long	cbRead = 0;
		bool rSizeRead = ReadFile(transport,&rSize,sizeof(long),&cbRead,NULL);
		
		if (reuse && !rSizeRead  && GetLastError() == ERROR_BROKEN_PIPE){ cout << "CLIENT CLOSED" << endl; return;};
		
		//if (rSizeRead  == false && GetLastError() == ERROR_ABANDONED_WAIT_0+) cout << "MORE DATA" << endl;
		//cout << rSizeRead << endl;
		//if (!reuse) 
			request_data = (unsigned char*) malloc(rSize);
		bool rRead = ReadFile(transport,request_data,rSize,&cbRead,NULL);
			//if (GetLastError() == ERROR_MORE_DATA) cout << "MORE DATA" << endl;
		//cout << rRead << endl;
		//cout << "Read request: " << cbRead << endl;
		//if (request !=NULL) cout << "Request:" << (char*)request << endl;
		*result = request_data;

		// create responsee data
		long packetSize = 0;
	    unsigned char* rData = NULL;
		b(rSize,request_data,&packetSize,&rData);

		//if (!reuse)  //NOTE: reusing big allocated data increases time of many identical conversations
			resp_data = (unsigned char*)malloc(sizeof(long)+packetSize);
	    memcpy(resp_data,&packetSize,sizeof(long));
		auto messageData = resp_data+ sizeof(long);
	    memcpy(messageData,rData,packetSize);
	     unsigned long	cbWritten = 0;	 
	     WriteFile(transport,resp_data,sizeof(long)+packetSize,&cbWritten,NULL);

		FlushFileBuffers(transport);
		if (!reuse)
			DisconnectNamedPipe(transport);
		if (!reuse) CloseHandle(transport);
CLEANUP:
		//if (!reuse) 
			free(resp_data);
	//	if (!reuse)
			 free(rData);
		//	if (!reuse)
				free(request_data);
}

void replySharedMem(HANDLE transport,resp_builder b,void** result){
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

		unsigned char* request = (unsigned char* )malloc(requestPacket);
		*result = request;
		memcpy(request,requestDataView,requestPacket);
		
		// notify client that data was read
		auto readRequest = CreateEvent(NULL,false,false,TEXT("ServerReadRequestEvent"));
		SetEvent(readRequest);
	   
		//push implementation

		// create responsee data
		long packetSize = 0;
	    unsigned char* resp_data = NULL;
		b(requestPacket,request,&packetSize,&resp_data);
   /* 	packetSize = reqResp[requestPacket];
		data = (unsigned char*)malloc(packetSize);
		char* msg = (char*)data;
		memcpy_s(msg,packetSize,  "Hello from server!!!",1024);*/
	
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
		memcpy(dataView,resp_data,packetSize);
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
		free(request);
		free(resp_data);

		//cout << "Reader read";
}

	