// tests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gtest/gtest.h"
#include <stdio.h>
#include "Messages.pb.h"
#include <windows.h>
#include <assert.h>
#include <string>
#include <vector>
#include <list>
#include <stopwatch.h>


class custom_request{
	
		public:

		std::list<std::string> m_ids;
		std::list<std::string> m_types;
		std::string  m_name;
		int idss;
		int typess;

		int getSize()
		{
			int size =0;

			idss = 0;			
			for (auto it=m_ids.begin(); it!=m_ids.end(); it++)
			{
				idss += //sizeof(int)+
					(*it).length()+1;
			}
			size += idss;
			typess = 0;
			for (auto it=m_types.begin(); it!=m_types.end(); it++)
			{
				typess += //sizeof(int)+
					(*it).length()+1;
			}
			size += typess;

			size+= m_name.length()+1;

			size+=sizeof(int)*3+2*sizeof(int); // to store 3 size of bytes and 2 for storing elements count
			return size;
		}
		void fromArray(byte* buffer){
			 memcpy(&idss,buffer,sizeof(int));
			 buffer += sizeof(int);

			  int cids = 0;//m_ids.size();
			  memcpy(&cids,buffer,sizeof(int));
			 buffer += sizeof(int);

			 memcpy(&typess,buffer,sizeof(int));
			buffer += sizeof(int);

			 int ctypes = 0;//m_types.size();
			  memcpy(&ctypes,buffer,sizeof(int));
			 buffer += sizeof(int);

			int ns = 0;//(m_name.length());
			memcpy(&ns,buffer,sizeof(int));
			buffer += sizeof(int);

		
			for (auto i=0;i<cids;i++){
				std::string s((char*)buffer) ;
				m_ids.insert(m_ids.end(),s);
				buffer+=s.length()+1;
			}
		
			for (auto i=0;i<ctypes;i++){
				std::string s((char*)buffer) ;
				m_types.insert(m_types.end(),s);
				buffer+=s.length()+1;
			}

		

			auto n  = (char*)malloc(ns);
			memcpy(n,buffer,ns);
			buffer+=ns;
			//TODO: free

		}
		
		void toArray(byte* buffer,int size){
			 memcpy(buffer,&idss,sizeof(int));
			 buffer += sizeof(int);

			 int cids = m_ids.size();
			 memcpy(buffer,&cids,sizeof(int));
			 buffer += sizeof(int);

			 memcpy(buffer,&typess,sizeof(int));
			buffer += sizeof(int);

			 int ctypes = m_types.size();
			 memcpy(buffer,&ctypes,sizeof(int));
			 buffer += sizeof(int);

			 int ns = (m_name.length()+1);
			memcpy(buffer,&ns,sizeof(int));
			buffer += sizeof(int);

	
			for (auto it=m_ids.begin(); it!=m_ids.end(); it++)
			{
				std::string s = *it;
				auto l = s.length()+1;
				//memcpy_s(buffer,sizeof(int),&l,sizeof(int));
				//buffer+=sizeof(int);
				memcpy_s(buffer,l,s.c_str(),l);
				buffer+=l;
			}

		
			for (auto it=m_types.begin(); it!=m_types.end(); it++)
			{
				std::string s = *it;
				auto l = s.length()+1;
				//memcpy_s(buffer,sizeof(int),&l,sizeof(int));
				//buffer+=sizeof(int);
				memcpy_s(buffer,l,s.c_str(),l);
				buffer+=l;
			}

				auto l = m_name.length()+1;
				memcpy_s(buffer,l,m_name.c_str(),l);
				buffer+=l;

				buffer-=size;
		}
};

TEST(SERIALIZATION, SPEED)
{
	auto name = "cool data";
	auto id = "very very cool id";
	auto type = "very very cool type";
	Stopwatch t;
	t.set_mode(REAL_TIME);
	auto r = "Protobuf serialization deserialization";
	t.start(r);
	request* s = new request();
	
	s->set_name((char*)name);
	for (int i = 0;i<5000;i++){

		s->add_ids(id);
	}
	for (int i = 0;i<10;i++){
	
		s->add_types(type);
	}
	auto size = s->ByteSize();
	void* data = malloc(size);
	s->SerializeToArray(data,size);

   request* d = new request();
   d->ParseFromArray(data,size);
   t.stop(r);
   
   custom_request sc;
  sc.m_name = std::string((char*)name);

	for (int i = 0;i<5000;i++){
		sc.m_ids.insert(sc.m_ids.end(),id);
	}
	for (int i = 0;i<10;i++){	
		sc.m_types.insert(sc.m_types.end(),type);
	}

	auto cr = "Custom object";

	auto cs = sc.getSize();
	byte* buffer = (byte*)malloc(cs);
	
	t.start(cr);
	sc.toArray(buffer,size);
	t.stop(cr);
	custom_request dc;
	dc.fromArray(buffer);


   t.report_all();



   EXPECT_EQ(1, 1);
}



int main(int argc, wchar_t* argv[])
{

	testing::InitGoogleTest(&argc,argv);
	
	RUN_ALL_TESTS();
	std::getchar();
	return 0;
}

