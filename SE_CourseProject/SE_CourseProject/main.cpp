#include "stdafx.h"
#include "iostream"
#include"Error.h"
#include"In.h"
#include"Log.h"
#include"Parm.h"
#include"Out.h"
#include"FST.h"
#include"Lexer.h"
#include"LT.h"
#include"MFST.h"
#include<cwchar>
#include<time.h>
#include "SemAnalysis.h"
#include "Generation.h"

using namespace std;
int _tmain(int argc, const _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");
	Log::LOG log = Log::INITLOG;
	Out::OUT out = Out::INIOUT;
	Parm::PARM parm;
	try 
	{
		 parm = Parm::getparm(argc, argv);
	}

	catch(Error::ERROR e)
	{
		cout << "Îøèáêà" << e.id << ':' << e.message << endl << endl;
		parm = Parm::createDefaultFiles();
		log = Log::getlog(parm.log);
		out = Out::getout(parm.out);
		Log::WriteError(log, e);
		
		return 0;
	}

	try
	{
		log = Log::getlog(parm.log);
		out = Out::getout(parm.out);
		In::IN in = In::getin(parm.in);

		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		Log::WriteIn(log, in);

		LT::LexTable lextab = LT::Create(LT_MAXSIZE - 1);
		IT::IdTable idtab = IT::Create(TI_MAXSIZE - 1);

		Lexer::Run(lextab, idtab, in);
	

		Log::WriteLexTableLog(log, lextab);
		Log::WriteIdTableLog(log, idtab);
		Log::WritePolizLog(log, lextab);
		

		unsigned int start_time = clock();

		MFST_TRACE_START(log)
			MFST::Mfst mfst(lextab, GRB::getGreibach());	

		bool syntaxAnalysisRes = mfst.start(log.stream);
	
		unsigned int end_time = clock();
		unsigned int search_time = end_time - start_time;

		mfst.savededucation();
		mfst.printrules(log.stream);


		*log.stream << "\tÐàçáîð âûïîëíåí çà " << ((float)search_time) / CLOCKS_PER_SEC << " ñåêóíä." << endl;
		*log.stream << setfill('=') << setw(90) << '=' << endl;
		cout << "\tÐàçáîð âûïîëíåí çà " << ((float)search_time) / CLOCKS_PER_SEC << " ñåêóíä." << endl;

		cout << setfill('=') << setw(100) << '=' << endl;


		if (parm.idtable)
			IT::WriteTable(idtab);
		if (parm.lextable)
			LT::WriteLexTable(lextab);

		Semantics::AnalyzeSem(lextab, idtab);

		if (syntaxAnalysisRes && Poliz::StartPoliz(lextab, idtab))
		{
			cout << "\t\tÏÎËÜÑÊÀß ÍÎÒÀÖÈß ÂÛÏÎËÍÅÍÀ" << endl;
			cout << setfill('=') << setw(100) << '=' << endl;
			if (parm.poliz)
				LT::WritePoliz(lextab);

			*log.stream << "\t\tÏÎËÜÑÊÀß ÍÎÒÀÖÈß ÂÛÏÎËÍÅÍÀ" << endl;
			*log.stream << setfill('=') << setw(90) << '=' << endl;
			Log::WritePolizLog(log, lextab);
			GenerateAsm::Generation(lextab, idtab, parm.out);
		}
		else
		{
			cout << "\t\tÏÎËÜÑÊÀß ÍÎÒÀÖÈß ÍÅ ÂÛÏÎËÍÅÍÀ. ÏÐÎÈÇÎØËÀ ÎØÈÁÊÀ" << endl;
			cout << setfill('=') << setw(100) << '=' << endl;
			*log.stream << "\t\tÏÎËÜÑÊÀß ÍÎÒÀÖÈß ÍÅ ÂÛÏÎËÍÅÍÀ. ÏÐÎÈÇÎØËÀ ÎØÈÁÊÀ" << endl;
			*log.stream << setfill('=') << setw(90) << '=' << endl;
		}

		IT::Delete(idtab);
		LT::Delete(lextab);
		Out::CloseFile(out);
		Log::Close(log);

	}
	catch (Error::ERROR e)
	{
		cout << "Îøèáêà" << e.id << ':' << e.message << endl;
	
		cout <<"Ñòðîêà " << e.inext.line << ", ïîçèöèÿ " << e.inext.col;

		Log::WriteError(log, e);
		Log::Close(log);
	}
	return 0;
};