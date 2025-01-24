#pragma once
#include <tchar.h>
#define PARM_IN L"-in:"							
#define PARM_OUT L"-out:"						
#define PARM_LOG L"-log:"						



#define PARM_LT		L"-lt"
#define PARM_ID		L"-id"
#define PARM_POLIZ	L"-poliz"


#define PARM_MAX_SIZE 300						
#define PARM_OUT_DEFAULT_EXT L".asm"			
#define PARM_LOG_DEFAULT_EXT L".log"			


namespace Parm
{
	struct PARM
	{
		wchar_t in[PARM_MAX_SIZE];				
		wchar_t out[PARM_MAX_SIZE];				
		wchar_t log[PARM_MAX_SIZE];				

		bool lextable = false;
		bool idtable = false;
		bool poliz = false;
	};


	PARM getparm(int argc, const _TCHAR* argv[]);
	PARM createDefaultFiles();
};

