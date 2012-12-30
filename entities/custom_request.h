#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <string>
#include <vector>
#include <list>

class custom_request;

class custom_request {
	
		public:

		std::list<std::string> m_ids;
		std::list<std::string> m_types;
		std::string  m_name;
		int idss;
		int typess;

		int getSize();
		void fromArray(unsigned char * buffer);
		
		void toArray(unsigned char* buffer,int size);
};