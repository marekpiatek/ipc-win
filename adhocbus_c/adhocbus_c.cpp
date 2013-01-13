// adhocbus_c.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "adhocbus_c.h"


// This is an example of an exported variable
ADHOCBUS_C_API int nadhocbus_c=0;

// This is an example of an exported function.
ADHOCBUS_C_API int fnadhocbus_c(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see adhocbus_c.h for the class definition
Cadhocbus_c::Cadhocbus_c()
{
	return;
}
