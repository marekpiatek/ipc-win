// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ADHOCBUS_C_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ADHOCBUS_C_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef ADHOCBUS_C_EXPORTS
#define ADHOCBUS_C_API __declspec(dllexport)
#else
#define ADHOCBUS_C_API __declspec(dllimport)
#endif

// This class is exported from the adhocbus_c.dll
class ADHOCBUS_C_API Cadhocbus_c {
public:
	Cadhocbus_c(void);
	// TODO: add your methods here.
};

extern ADHOCBUS_C_API int nadhocbus_c;

ADHOCBUS_C_API int fnadhocbus_c(void);
