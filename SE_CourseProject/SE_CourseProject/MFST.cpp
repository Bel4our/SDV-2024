#include "stdafx.h"

#include "MFST.h"
#include "Error.h"

#include <fstream>
#include <iomanip>
#include "Log.h"	

using namespace std;
int FST_TRACE_n = -1;
char rbuf[205], sbuf[205], lbuf[1024];
namespace MFST
{
	MfstState::MfstState()
	{
		lenta_position = 0;
		nrule = -1;
		nrulechain = -1;
	}
	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain)
	{
		lenta_position = pposition;
		st = pst;
		nrulechain = pnrulechain;
	}
	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain)
	{
		lenta_position = pposition;
		st = pst;
		nrule = pnrule;
		nrulechain = pnrulechain;
	}
	Mfst::MfstDiagnosis::MfstDiagnosis()
	{
		lenta_position = -1;
		rc_step = SURPRISE;
		nrule = -1;
		nrule_chain = -1;
	}
	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain)
	{
		lenta_position = plenta_position;
		rc_step = prc_step;
		nrule = pnrule;
		nrule_chain = pnrule_chain;
	}
	Mfst::Mfst()
	{
		lenta = 0;
		lenta_size = lenta_position = 0;
	}

	Mfst::Mfst(LT::LexTable plex, GRB::Greibach pgrebach)
	{
		grebach = pgrebach;
		lexTable = plex;
		lenta = new short[lenta_size = lexTable.size];
		for (int k = 0; k < lenta_size; k++) {
			lenta[k] = TS(lexTable.table[k].lexema);
		}
		lenta_position = 0;
		st.push(grebach.stbottomT);
		st.push(grebach.startN);
		nrulechain = -1;
	};


	Mfst::RC_STEP Mfst::step(ofstream *stream)
	{
		RC_STEP rc = SURPRISE;
		if (lenta_position < lenta_size)
		{
			if (ISNS(st.top()))
			{
				GRB::Rule rule;
				if ((nrule = GRB::getGreibach().getRule(st.top(), rule)) >= 0)
				{
					GRB::Rule::Chain chain;
					if ((nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0)
					{
						MFST_TRACE1(*stream);
						savestate(stream);
						st.pop();
						push_chain(chain);
						rc = NS_OK;
						MFST_TRACE2(*stream);
					}
					else
					{
						MFST_TRACE4("TNS_NORULECHAIN/NS_NORULE", *stream)
							savediagnosis(NS_NORULECHAIN);
						rc = reststate(stream) ? NS_NORULECHAIN : NS_NORULE;
					}
				}
				else
					rc = NS_ERROR;
			}
			else if ((st.top() == lenta[lenta_position]))
			{
				lenta_position++;
				st.pop();
				nrulechain = -1;
				rc = TS_OK;
				MFST_TRACE3(*stream);
			}
			else
			{
				MFST_TRACE4("TS_NOK/NS_NORULECHAIN", *stream)
					rc = reststate(stream) ? TS_NOK : NS_NORULECHAIN;
			}
		}
		else
		{
			rc = LENTA_END;
			MFST_TRACE4("LENTA_END", *stream)
		}
		return rc;
	}

	bool Mfst::push_chain(GRB::Rule::Chain chain)
	{
		for (int i = chain.size - 1; i >= 0; i--)
			st.push(chain.nt[i]);
		return true;
	}
	bool Mfst::savestate(ofstream *stream)
	{
		storestate.push(MfstState(lenta_position, st, nrule, nrulechain));
		MFST_TRACE6("SAVESTATE:", storestate.size(), *stream);
		return true;
	}
	bool Mfst::reststate(ofstream *stream)
	{
		bool rc = false;
		MfstState state;
		if (rc = (storestate.size() > 0))
		{
			state = storestate.top();
			lenta_position = state.lenta_position;
			st = state.st;
			nrule = state.nrule;
			nrulechain = state.nrulechain;
			storestate.pop();
			MFST_TRACE5("RESSTATE", *stream)
				MFST_TRACE2(*stream);
		}
		return rc;
	}
	bool Mfst::savediagnosis(RC_STEP prc_step)
	{
		bool rc = false;
		short k = 0;
		while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position)
			k++;
		if (rc = (k < MFST_DIAGN_NUMBER))
		{
			diagnosis[k] = MfstDiagnosis(lenta_position, prc_step, nrule, nrulechain);
			for (short j = k + 1; j < MFST_DIAGN_NUMBER; j++)
				diagnosis[j].lenta_position = -1;
		}
		return rc;
	}
	
	bool Mfst::start(ofstream *stream)
	{
		bool rc = false;
		RC_STEP rc_step = SURPRISE;
		char buf[MFST_DIAGN_MAXSIZE];
		rc_step = step(stream);
		while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
			rc_step = step(stream);
		switch (rc_step)
		{
		case NS_NORULE:
			MFST_TRACE4("-------> NS_NORULE", *stream);
			cout << "--------------------------------------------------------------" << endl;
			cout << getDiagnosis(0, buf) << endl;
			*stream << getDiagnosis(0, buf) << "\n";
			cout << getDiagnosis(1, buf) << endl;
			*stream << getDiagnosis(1, buf) << "\n";
			cout << getDiagnosis(2, buf) << endl;
			*stream << getDiagnosis(2, buf) << "\n";
			break;
		case NS_NORULECHAIN:
			MFST_TRACE4("------> NS_NORULECHAIN", *stream)
				break;
		case NS_ERROR:
			MFST_TRACE4("------> NS_ERROR", *stream)
				break;
		case LENTA_END:
			MFST_TRACE4("-------> NS_LENTA_END", *stream)
				cout << "--------------------------------------------------------------" << endl;
			cout << setw(4) << left << "Всего строк " << lenta_size << ", Синтаксический анализ выполнен без ошибок" << endl;
			rc = true;
			break;
		case SURPRISE:
			MFST_TRACE4("------> SURPRISE", *stream)
				break;
		}
		return rc;
	}
	char* Mfst::getCSt(char* buf)
	{
		for (int i = (signed)st.size() - 1; i >= 0; --i)
		{
			short p = st._Get_container()[i];
			buf[st.size() - 1 - i] = GRB::Rule::Chain::alphabet_to_char(p);
		}
		buf[st.size()] = 0x00;
		return buf;
	}
	char* Mfst::getCLenta(char* buf, short pos, short n)
	{
		short i, k = (pos + n < lenta_size) ? pos + n : lenta_size;
		for (i = pos; i < k; i++)
			buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);
		buf[i - pos] = 0x00;
		return buf;
	}
	char* Mfst::getDiagnosis(short n, char* buf)
	{
		buf[0] = '\0';  
		char* rc = buf;
		int errid = 0;
		int lpos = -1;
		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
		{
			errid = grebach.getRule(diagnosis[n].nrule).iderror;
			Error::ERROR err = Error::geterror(errid);
			cout << "Ошибка " << err.id << ":" << err.message << " строка " << lexTable.table[lpos].sn << endl;
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d, %s", err.id, lexTable.table[lpos].sn, err.message);
			rc = buf;
		}
		return rc;
	}
	void Mfst::printrules(ofstream *stream)
	{
		*stream << "\tДерево разбора" << endl;
		*stream << setfill('-') << endl;
		MfstState state;
		GRB::Rule rule;
		for (unsigned short i = 0; i < storestate.size(); i++)
		{
			state = storestate._Get_container()[i];
			rule = grebach.getRule(state.nrule);
			MFST_TRACE7(*stream);
		}
		*stream << setfill('=') << setw(90) << '=' << endl;
	}
	bool Mfst::savededucation()
	{
		MfstState state;
		GRB::Rule rule;
		deducation.nrules = new short[deducation.size = storestate.size()];
		deducation.nrulechains = new short[deducation.size];
		for (unsigned short i = 0; i < storestate.size(); i++)
		{
			state = storestate._Get_container()[i];
			deducation.nrules[i] = state.nrule;
			deducation.nrulechains[i] = state.nrulechain;
		}
		return true;
	}
}