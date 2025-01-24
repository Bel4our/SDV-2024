#include <iostream>
#include "Out.h"
#include "Log.h"
#include <fstream>
#include "Error.h"
#include <cstdarg>
#include <ctime>


using namespace Out;
using namespace std;
using namespace Parm;
using namespace In;


namespace Out {

	OUT getout(wchar_t outfile[])
	{
		OUT out;
		ofstream* file = new ofstream(outfile);
		if (!file->is_open())
		{
			delete file;
			ERROR_THROW(113);
		}
		wcscpy_s(out.outfile, outfile);
		out.stream = file;
		return out;
	}

	void Out::WriteToFile(OUT out, In::IN in)
	{
		*out.stream << in.text;
	}

	void CloseFile(OUT out)
	{
		out.stream->close();
		delete out.stream;
	}

}
