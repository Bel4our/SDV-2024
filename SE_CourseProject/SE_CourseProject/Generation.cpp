#include "stdafx.h"
#include "Generation.h"


namespace GenerateAsm
{
	void Generation(LT::LexTable lextable, IT::IdTable idtable, wchar_t outfile[])
	{
		ofstream out(outfile);
		Head(&out);
		ConstSegment(idtable, &out);
		DataSegment(lextable, idtable, &out);
		CodeSegment(lextable, idtable, &out);
	}

	void Head(ofstream* out)
	{
		*out << ".586P\n";
		*out << ".model flat, stdcall\n";

		*out << "includelib libucrt.lib\n";
		*out << "includelib kernel32.lib\n";
		*out << "includelib ..\\Debug\\Library.lib\n";
		*out << "ExitProcess PROTO :DWORD\n\n";

		*out << "EXTRN CONCAT: proc\n";
		*out << "EXTRN LENLINE: proc\n";
		*out << "EXTRN RANDOM: proc\n";
		*out << "EXTRN WriteNumb: proc\n";
		*out << "EXTRN WriteLine: proc\n";
		*out << "EXTRN WriteBool: proc\n";
		*out << "EXTRN WriteSymb: proc\n";
		*out << "EXTRN WriteNumbLn: proc\n";
		*out << "EXTRN WriteLineLn: proc\n";
		*out << "EXTRN WriteBoolLn: proc\n";
		*out << "EXTRN WriteSymbLn: proc\n";
		*out << "\n.stack 4096\n\n";
	}

	void ConstSegment(IT::IdTable idtable, ofstream* out)
	{
		*out << ".const\n";
		*out << "\tnull_division BYTE \"Exception: деление на ноль\", 0\n";
		for (int i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].idtype == IT::IDTYPE::L)
			{
				*out << "\t" << idtable.table[i].id;

				if (idtable.table[i].iddatatype == IT::IDDATATYPE::LINE)
				{
					*out << "\tBYTE \"";
					if (strlen(idtable.table[i].value.vstr->str) > 2) {
						for (size_t j = 1; j < strlen(idtable.table[i].value.vstr->str) - 1; ++j) {
							*out << idtable.table[i].value.vstr->str[j];
						}
					}
					*out << "\", 0";

				}
				if (idtable.table[i].iddatatype == IT::IDDATATYPE::SYMB)
					*out << "\tBYTE \"" << idtable.table[i].value.vchar << "\", 0";
				if (idtable.table[i].iddatatype == IT::IDDATATYPE::NUMB || idtable.table[i].iddatatype == IT::IDDATATYPE::BOOL)
					*out << "\tSWORD " << idtable.table[i].value.vint;
				*out << '\n';
			}
		}
	}

	void DataSegment(LT::LexTable lextable, IT::IdTable idtable, ofstream* out)
	{
		*out << "\n.data\n";
		*out << "\tbuffer BYTE 256 dup(0)\n";
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema == LEX_DECLARE && lextable.table[i + 2].lexema != LEX_FUNCTION)
			{
				if (idtable.table[lextable.table[i + 2].idxTi].idtype == IT::IDTYPE::V)
				{
					*out << "\t" << idtable.table[lextable.table[i + 2].idxTi].func + "_" + idtable.table[lextable.table[i + 2].idxTi].id;
					if (idtable.table[lextable.table[i + 2].idxTi].iddatatype == IT::IDDATATYPE::SYMB)
					{
						*out << "\tDWORD ?\n";
					}
					if (idtable.table[lextable.table[i + 2].idxTi].iddatatype == IT::IDDATATYPE::LINE)
					{
						*out << "\tDWORD ?\n";
					}
					if (idtable.table[lextable.table[i + 2].idxTi].iddatatype == IT::IDDATATYPE::NUMB || idtable.table[lextable.table[i + 2].idxTi].iddatatype == IT::IDDATATYPE::BOOL)
					{
						*out << "\tSWORD 1\n";
					}
				}
			}
		}
	}

	void CodeSegment(LT::LexTable lextable, IT::IdTable idtable, ofstream* out)
	{
		stack<IT::Entry> stk;
		queue<string> queue;
		int numOfPoints = 0, numOfLogExpr = 0, numOfRet = 0,
			numOfEnds = 0, numOfCycles = 0, countParm = 0, numOfLogPoints = 0;

		string funcName = string();

		bool flagFunc = false, flagRet = false, flagBody = false, flagWhen = false,
			flag_else = false, flagOtherwise = false, flagLinelen = false, flagRand = false,
			flagConcat = false, flagCallfunc = false;

		*out << "\n.code\n\n";
		for (int i = 0; i < lextable.size; i++)
		{
			switch (lextable.table[i].lexema)
			{
			case LEX_FUNCTION:
			{
				while (lextable.table[i].lexema != LEX_RIGHTHESIS)
				{
					if (lextable.table[i].lexema == LEX_ID && idtable.table[lextable.table[i].idxTi].idtype == IT::IDTYPE::F)
					{
						*out << (funcName = "f_" + idtable.table[lextable.table[i].idxTi].id) << " PROC ";
					}
					if (lextable.table[i].lexema == LEX_ID && idtable.table[lextable.table[i].idxTi].idtype == IT::IDTYPE::P)
					{
						*out << idtable.table[lextable.table[i].idxTi].func + "_" + idtable.table[lextable.table[i].idxTi].id << " : ";
						if (idtable.table[lextable.table[i].idxTi].iddatatype == IT::IDDATATYPE::NUMB)
						{
							*out << "SWORD";
						}
						else
							if (idtable.table[lextable.table[i].idxTi].iddatatype == IT::IDDATATYPE::SYMB)
							{
								*out << "DWORD";
							}
							else if (idtable.table[lextable.table[i].idxTi].iddatatype == IT::IDDATATYPE::BOOL)
							{
								*out << "SWORD";
								queue.push(idtable.table[lextable.table[i].idxTi].func + "_" + idtable.table[lextable.table[i].idxTi].id);
							}
							else
							{
								*out << "DWORD";
							}
					}
					if (lextable.table[i].lexema == LEX_COMMA)
					{
						*out << ", ";
					}
					i++;
				}
				while (!queue.empty())
				{
					*out << "\n\tmov ax, " << queue.front() << "\n";
					*out << "\tcmp ax, 0\n";
					*out << "\tje l" << numOfLogPoints << "\n";
					*out << "\tjne l" << numOfLogPoints + 1 << "\n";

					*out << "l" << numOfLogPoints << ":\n";
					*out << "\tmov ax, 0\n\tpush ax\n\tjmp endofexpr" << numOfLogExpr << "\n";

					*out << "l" << numOfLogPoints + 1 << ":\n";
					*out << "\tmov ax, 1\n\tpush eax\n\n";

					*out << "endofexpr" << numOfLogExpr << ":\n";
					numOfLogExpr++;
					numOfLogPoints += 2;
					*out << "\tpop " << queue.front() << "\n";
					queue.pop();
				}
				flagFunc = true;
				*out << "\n";
				break;
			}
			case LEX_MAIN:
			{
				flagBody = true;
				*out << "main PROC\n";
				break;
			}
			case LEX_EQUALS:
			{
				int result_position = i - 1;
				while (lextable.table[i].lexema != LEX_SEMICOLON)
				{
					switch (lextable.table[i].lexema)
					{
					case LEX_ID:
					case LEX_LITERAL:
					case LEX_TRUE:
					{
						if (idtable.table[lextable.table[i].idxTi].idtype == IT::IDTYPE::F)
						{
							flagCallfunc = true;
						}
						else
						{
							if (idtable.table[lextable.table[i].idxTi].iddatatype == IT::IDDATATYPE::NUMB || idtable.table[lextable.table[i].idxTi].iddatatype == IT::IDDATATYPE::BOOL)
							{
								if (idtable.table[lextable.table[i].idxTi].idtype == IT::L)
									*out << "\tpush " << idtable.table[lextable.table[i].idxTi].id << "\n";
								else
									*out << "\tpush " << idtable.table[lextable.table[i].idxTi].func + "_" + idtable.table[lextable.table[i].idxTi].id << "\n";
								stk.push(idtable.table[lextable.table[i].idxTi]);
								break;
							}
							if (idtable.table[lextable.table[i].idxTi].iddatatype == IT::IDDATATYPE::SYMB)
							{
								if (idtable.table[lextable.table[i].idxTi].idtype == IT::L)
									*out << "\tpush offset " << idtable.table[lextable.table[i].idxTi].id << "\n";
								else
									*out << "\tpush " << idtable.table[lextable.table[i].idxTi].func + "_" + idtable.table[lextable.table[i].idxTi].id << "\n";
								stk.push(idtable.table[lextable.table[i].idxTi]);
								break;
							}
							if (idtable.table[lextable.table[i].idxTi].iddatatype == IT::IDDATATYPE::LINE)
							{
								if (idtable.table[lextable.table[i].idxTi].idtype == IT::IDTYPE::L)
								{
									*out << "\tpush offset " << idtable.table[lextable.table[i].idxTi].id << "\n";
								}
								else
								{
									*out << "\tpush " << idtable.table[lextable.table[i].idxTi].func + "_" + idtable.table[lextable.table[i].idxTi].id << "\n";
								}
								stk.push(idtable.table[lextable.table[i].idxTi]);
								break;
							}
						}
					}

					case LEX_PLUS:
					{
						switch (lextable.table[i].oper)
						{

						case LT::OPERATION::MUL:
						{
							*out << "\tpop ax\n\tpop bx\n";
							*out << "\tmul bx\n\tpush ax\n";
							break;
						}
						case LT::OPERATION::PLUS:
						{
							*out << "\tpop ax\n\tpop bx\n";
							*out << "\tadd ax, bx\n\tpush ax\n";
							break;
						}
						case LT::OPERATION::MINUS:
						{
							*out << "\tpop bx\n\tpop ax\n";
							*out << "\tsub ax, bx\n\tpush ax\n";
							break;
						}
						case LT::OPERATION::DIV:
						{
							*out << "\tpop bx\n\tpop ax\n";
							*out << "\tcmp bx, 0\n\tje nulldiv\n";
							*out << "\tcwd\n\tidiv bx\n\tpush ax\n";
							break;
						}
						case LT::OPERATION::REST:
						{
							*out << "\tpop bx\n\tpop ax\n";
							*out << "\tcmp bx, 0\n\tje nulldiv\n";
							*out << "\tcwd\n\tidiv bx\n\tpush dx\n";
							break;
						}
						}
						break;
					}
					case LEX_LESS:
					{
						switch (lextable.table[i].oper)
						{
						case LT::OPERATION::MORE:
						{
							*out << "\tpop bx\n\tpop ax\n";
							*out << "\tcmp ax, bx\n";
							*out << "\tjg l" << numOfLogPoints << "\n";
							*out << "\tjle l" << numOfLogPoints + 1 << "\n";

							*out << "l" << numOfLogPoints << ":\n";
							*out << "\tmov ax, 1\n\tpush ax\n\tjmp endofexpr" << numOfLogExpr << "\n";

							*out << "l" << numOfLogPoints + 1 << ":\n";
							*out << "\tmov ax, 0\n\tpush ax\n\n";

							*out << "endofexpr" << numOfLogExpr << ":\n";
							numOfLogExpr++;
							numOfLogPoints += 2;
							break;
						}
						case LT::OPERATION::LESS:
						{
							*out << "\tpop bx\n\tpop ax\n";
							*out << "\tcmp ax, bx\n";
							*out << "\tjl l" << numOfLogPoints << "\n";
							*out << "\tjge l" << numOfLogPoints + 1 << "\n";

							*out << "l" << numOfLogPoints << ":\n";
							*out << "\tmov ax, 1\n\tpush ax\n\tjmp endofexpr" << numOfLogExpr << "\n";

							*out << "l" << numOfLogPoints + 1 << ":\n";
							*out << "\tmov ax, 0\n\tpush ax\n\n";

							*out << "endofexpr" << numOfLogExpr << ":\n";
							numOfLogExpr++;
							numOfLogPoints += 2;
							break;
						}
						case LT::OPERATION::EQU:
						{
							*out << "\tpop bx\n\tpop ax\n";
							*out << "\tcmp ax, bx\n";
							*out << "\tje l" << numOfLogPoints << "\n";
							*out << "\tjne l" << numOfLogPoints + 1 << "\n";

							*out << "l" << numOfLogPoints << ":\n";
							*out << "\tmov ax, 1\n\tpush ax\n\tjmp endofexpr" << numOfLogExpr << "\n";

							*out << "l" << numOfLogPoints + 1 << ":\n";
							*out << "\tmov ax, 0\n\tpush ax\n\n";

							*out << "endofexpr" << numOfLogExpr << ":\n";
							numOfLogExpr++;
							numOfLogPoints += 2;
							break;
						}
						case LT::OPERATION::NOEQU:
						{
							*out << "\tpop bx\n\tpop ax\n";
							*out << "\tcmp ax, bx\n";
							*out << "\tjne l" << numOfLogPoints << "\n";
							*out << "\tje l" << numOfLogPoints + 1 << "\n";

							*out << "l" << numOfLogPoints << ":\n";
							*out << "\tmov ax, 1\n\tpush ax\n\tjmp endofexpr" << numOfLogExpr << "\n";

							*out << "l" << numOfLogPoints + 1 << ":\n";
							*out << "\tmov ax, 0\n\tpush ax\n\n";

							*out << "endofexpr" << numOfLogExpr << ":\n";
							numOfLogExpr++;
							numOfLogPoints += 2;
							break;
						}
						case LT::OPERATION::MOREQ:
						{
							*out << "\tpop bx\n\tpop ax\n";
							*out << "\tcmp ax, bx\n";
							*out << "\tjge l" << numOfLogPoints << "\n";
							*out << "\tjl l" << numOfLogPoints + 1 << "\n";

							*out << "l" << numOfLogPoints << ":\n";
							*out << "\tmov ax, 1\n\tpush ax\n\tjmp endofexpr" << numOfLogExpr << "\n";

							*out << "l" << numOfLogPoints + 1 << ":\n";
							*out << "\tmov ax, 0\n\tpush ax\n\n";

							*out << "endofexpr" << numOfLogExpr << ":\n";
							numOfLogExpr++;
							numOfLogPoints += 2;
							break;
						}
						case LT::OPERATION::LESSQ:
						{
							*out << "\tpop bx\n\tpop ax\n";
							*out << "\tcmp ax, bx\n";
							*out << "\tjle l" << numOfLogPoints << "\n";
							*out << "\tjg l" << numOfLogPoints + 1 << "\n";

							*out << "l" << numOfLogPoints << ":\n";
							*out << "\tmov ax, 1\n\tpush ax\n\tjmp endofexpr" << numOfLogExpr << "\n";

							*out << "l" << numOfLogPoints + 1 << ":\n";
							*out << "\tmov ax, 0\n\tpush ax\n\n";

							*out << "endofexpr" << numOfLogExpr << ":\n";
							numOfLogExpr++;
							numOfLogPoints += 2;
							break;
						}
						}
						break;
					}
					case LEX_LENLINE:
					{
						flagLinelen = true;
						break;
					}
					case LEX_CONCAT:
					{
						flagConcat = true;
						break;
					}
					case LEX_RANDOM:
					{
						flagRand = true;
						break;
					}

					case '@':
					{
						countParm = (char)lextable.table[i + 1].lexema - '0';

						for (int j = 1; j <= countParm; j++)
						{
							*out << "\tpop dx\n";
						}

						for (int j = 1; j <= countParm; j++)
						{
							if (stk.top().iddatatype == IT::IDDATATYPE::LINE || stk.top().iddatatype == IT::IDDATATYPE::SYMB)
							{
								if (stk.top().idtype == IT::IDTYPE::L)
									*out << "\tpush offset " << stk.top().id << "\n";
								else
									*out << "\tpush " << stk.top().func + "_" + stk.top().id << "\n";
							}
							else
							{
								if (stk.top().idtype == IT::IDTYPE::L)
								{
									*out << "\tmovsx eax, " << stk.top().id << "\n";
								}
								else
								{
									*out << "\tmovsx eax, " << stk.top().func + "_" + stk.top().id << "\n";
								}
								*out << "\tpush eax" << "\n";
							}
							stk.pop();
						}

						if (flagCallfunc && !flagConcat && !flagLinelen && !flagRand)
						{
							if (idtable.table[lextable.table[i - countParm - 1].idxTi].iddatatype == IT::IDDATATYPE::LINE
								|| idtable.table[lextable.table[i - countParm - 1].idxTi].iddatatype == IT::IDDATATYPE::SYMB)
								*out << "\tcall " << "f_" + idtable.table[lextable.table[i - countParm - 1].idxTi].id << "\n\tpush eax\n";
							else
								*out << "\tcall " << "f_" + idtable.table[lextable.table[i - countParm - 1].idxTi].id << "\n\tpush ax\n";
							flagCallfunc = false;
						}

						if (flagLinelen)
						{
							*out << "\tcall LENLINE" << "\n" << "\tpush ax\n";
							flagLinelen = false;
						}
						if (flagConcat)
						{
							*out << "\tpush offset buffer\n";
							*out << "\tcall CONCAT\n" << "\tpush eax\n";
							flagConcat = false;
						} 
						if (flagRand)
						{
							*out << "\tcall RANDOM" << "\n" << "\tpush ax\n";
							flagRand = false;
						}
						break;
					}
					}
					i++;
				}

				if (idtable.table[lextable.table[result_position].idxTi].iddatatype == IT::IDDATATYPE::BOOL)
				{
					*out << "\tpop ax\n";
					*out << "\tcmp ax, 0\n";
					*out << "\tje l" << numOfLogPoints << "\n";
					*out << "\tjne l" << numOfLogPoints + 1 << "\n";

					*out << "l" << numOfLogPoints << ":\n";
					*out << "\tmov ax, 0\n\tpush ax\n\tjmp endofexpr" << numOfLogExpr << "\n";

					*out << "l" << numOfLogPoints + 1 << ":\n";
					*out << "\tmov ax, 1\n\tpush ax\n\n";

					*out << "endofexpr" << numOfLogExpr << ":\n";
					numOfLogExpr++;
					numOfLogPoints += 2;
				}
				*out << "\tpop " << idtable.table[lextable.table[result_position].idxTi].func + "_" + idtable.table[lextable.table[result_position].idxTi].id << "\n";
				break;
			}



			case '@':
			{
				countParm = (char)lextable.table[i + 1].lexema - '0';
				for (int j = 1; j <= countParm; j++)
				{
					if (idtable.table[lextable.table[i - j].idxTi].iddatatype == IT::IDDATATYPE::NUMB || idtable.table[lextable.table[i - j].idxTi].iddatatype == IT::IDDATATYPE::BOOL)
					{
						*out << "\tmovsx eax, " << idtable.table[lextable.table[i - j].idxTi].func + "_" + idtable.table[lextable.table[i - j].idxTi].id << "\n";
						*out << "\tpush eax" << "\n";
					}
					else
					{

						if (idtable.table[lextable.table[i - j].idxTi].idtype == IT::IDTYPE::L)
							*out << "\tpush offset" << idtable.table[lextable.table[i - j].idxTi].id << "\n";
						else
							*out << "\tpush " << idtable.table[lextable.table[i - j].idxTi].func + "_" + idtable.table[lextable.table[i - j].idxTi].id << "\n";
					}
				}
				*out << "\tcall " << "f_" + idtable.table[lextable.table[i - countParm - 1].idxTi].id << "\n";
				break;
			}
			case LEX_RETURN:
			{
				*out << "\tpush ";
				i++;
				int index = i;
			
				if (idtable.table[lextable.table[index].idxTi].idtype == IT::IDTYPE::L)
				{
					if (idtable.table[lextable.table[index].idxTi].iddatatype == IT::IDDATATYPE::NUMB)
						*out << idtable.table[lextable.table[i].idxTi].value.vint << "\n";
					else if (idtable.table[lextable.table[index].idxTi].iddatatype == IT::IDDATATYPE::BOOL)
						*out << idtable.table[lextable.table[i].idxTi].value.vint << "\n";
					else if (idtable.table[lextable.table[index].idxTi].iddatatype == IT::IDDATATYPE::SYMB)
						*out << "offset " << idtable.table[lextable.table[i].idxTi].id << "\n";
					else if (idtable.table[lextable.table[index].idxTi].iddatatype == IT::IDDATATYPE::LINE)
						*out << "offset " << idtable.table[lextable.table[i].idxTi].id << "\n";
				}
				else
				{
					*out << idtable.table[lextable.table[index].idxTi].func + "_" + idtable.table[lextable.table[index].idxTi].id << "\n";
				}
				if (flagFunc)
				{
					*out << "\tjmp local" << numOfRet << "\n";
					flagRet = true;
				}
				if (flagBody)
				{
					*out << "\t\tjmp theend\n";
					flagRet = true;
				}
				break;
			}
			case LEX_RIGHTBRACKET:
			{
				if (flag_else)
				{
					flag_else = false;
					if (flagOtherwise)
					{
						*out << "\tjmp e" << numOfEnds << "\n";
						flagOtherwise = false;
					}
					*out << "m" << numOfPoints++ << ":\n";
				}
				if (flagOtherwise)
				{
					flagOtherwise = false;
					*out << "e" << numOfEnds++ << ":\n";
				}
				break;
			}
			case LEX_BRACELET:
			{
				if (flagBody && !flagFunc)
				{
					if (flagRet)
					{
						*out << "theend:\n";
						flagRet = false;
					}
					*out << "\tpush 0\n\tcall ExitProcess";
					*out << "\nnulldiv PROC\n\tpush offset null_division\n\tcall WriteLineLn\n\tpush -1\n\tcall ExitProcess\n\t nulldiv ENDP";
					*out << "\nmain ENDP\nend main";
				}
				if (flagFunc)
				{
					if (flagRet)
					{
						*out << "local" << numOfRet++ << ":\n";
						*out << "\tpop eax\n\tret\n";
					}
					*out << funcName << " ENDP\n\n";
					flagFunc = false;
				}

				break;
			}

			case LEX_CHECK:
			{
				flagWhen = true;
				break;
			}
			case LEX_OTHERWISE:
			{
				flagOtherwise = true;
				break;
			}
			case LEX_LEFTHESIS:
			{
				if (flagWhen)
				{
					if (lextable.table[i + 2].lexema == LEX_RIGHTHESIS)
					{
						if (idtable.table[lextable.table[i + 1].idxTi].idtype == IT::L)
							*out << "\tmov ax, " << idtable.table[lextable.table[i + 1].idxTi].id << "\n";
						else
							*out << "\tmov ax, " << idtable.table[lextable.table[i + 1].idxTi].func + "_" + idtable.table[lextable.table[i + 1].idxTi].id << "\n";

						*out << "\tcmp ax, 0\n";
						*out << "\tjnz m" << numOfPoints << "\n";
						*out << "\tjz m" << numOfPoints + 1 << "\n";
					}
					else
					{
						if (idtable.table[lextable.table[i + 1].idxTi].idtype == IT::L)
							*out << "\tmov ax, " << idtable.table[lextable.table[i + 1].idxTi].id << "\n";
						else
							*out << "\tmov ax, " << idtable.table[lextable.table[i + 1].idxTi].func + "_" + idtable.table[lextable.table[i + 1].idxTi].id << "\n";

						if (idtable.table[lextable.table[i + 3].idxTi].idtype == IT::L)
							*out << "\tcmp ax, " << idtable.table[lextable.table[i + 3].idxTi].id << "\n";
						else
							*out << "\tcmp ax, " << idtable.table[lextable.table[i + 3].idxTi].func + "_" + idtable.table[lextable.table[i + 3].idxTi].id << "\n";

						if (lextable.table[i + 2].oper == LT::OPERATION::MORE)
						{
							*out << "\tjg m" << numOfPoints << "\n";
							*out << "\tjl m" << numOfPoints + 1 << "\n";
						}
						else if (lextable.table[i + 2].oper == LT::OPERATION::LESS)
						{
							*out << "\tjl m" << numOfPoints << "\n";
							*out << "\tjg m" << numOfPoints + 1 << "\n";
						}
						else if (lextable.table[i + 2].oper == LT::OPERATION::EQU)
						{
							*out << "\tje m" << numOfPoints << "\n";
							*out << "\tjne m" << numOfPoints + 1 << "\n";
						}
						else if (lextable.table[i + 2].oper == LT::OPERATION::NOEQU)
						{
							*out << "\tjne m" << numOfPoints << "\n";
							*out << "\tje m" << numOfPoints + 1 << "\n";
						}
						else if (lextable.table[i + 2].oper == LT::OPERATION::MOREQ)
						{
							*out << "\tjge m" << numOfPoints << "\n";
							*out << "\tjle m" << numOfPoints + 1 << "\n";
						}
						else if (lextable.table[i + 2].oper == LT::OPERATION::LESSQ)
						{
							*out << "\tjle m" << numOfPoints << "\n";
							*out << "\tjge m" << numOfPoints + 1 << "\n";
						}
					}
					int j = i;
					while (lextable.table[j++].lexema != LEX_RIGHTBRACKET)
					{
						if (lextable.table[j + 1].lexema == LEX_OTHERWISE)
						{
							flagOtherwise = true;
							break;
						}
					}
				}
				break;
			}
			case LEX_RIGHTHESIS:
			{
				if (lextable.table[i + 1].lexema == LEX_LEFTBRACKET && flagWhen)
				{
					flag_else = true;
					*out << "m" << numOfPoints++ << ":\n";
					flagWhen = false;
				}
				break;
			}
			case LEX_WRITE:
			{
				if (idtable.table[lextable.table[i + 1].idxTi].iddatatype == IT::IDDATATYPE::NUMB)
				{
					if (idtable.table[lextable.table[i + 1].idxTi].idtype == IT::L)
						*out << "\tmovsx eax, " << idtable.table[lextable.table[i + 1].idxTi].id << "\n";
					else
						*out << "\tmovsx eax, " << idtable.table[lextable.table[i + 1].idxTi].func + "_" + idtable.table[lextable.table[i + 1].idxTi].id << "\n";
					*out << "\tpush eax" << "\n\tcall WriteNumb\n";
				}
				else if (idtable.table[lextable.table[i + 1].idxTi].iddatatype == IT::IDDATATYPE::BOOL)
				{
					if (idtable.table[lextable.table[i + 1].idxTi].idtype == IT::L)
						*out << "\tmovsx eax, " << idtable.table[lextable.table[i + 1].idxTi].id << "\n";
					else
						*out << "\tmovsx eax, " << idtable.table[lextable.table[i + 1].idxTi].func + "_" + idtable.table[lextable.table[i + 1].idxTi].id << "\n";
					*out << "\tpush eax" << "\n\tcall WriteBool\n";
				}
				else if (idtable.table[lextable.table[i + 1].idxTi].iddatatype == IT::IDDATATYPE::SYMB)
				{
					if (idtable.table[lextable.table[i + 1].idxTi].idtype == IT::IDTYPE::L)
					{
						*out << "\tpush offset ";
						*out << idtable.table[lextable.table[i + 1].idxTi].id << "\n\tcall WriteSymb\n";
						break;
					}
					else
						*out << "\tpush ";
					*out << idtable.table[lextable.table[i + 1].idxTi].func + "_" + idtable.table[lextable.table[i + 1].idxTi].id << "\n\tcall WriteSymb\n";
				}
				else
				{
					if (idtable.table[lextable.table[i + 1].idxTi].idtype == IT::IDTYPE::L)
					{
						*out << "\tpush offset ";
						*out << idtable.table[lextable.table[i + 1].idxTi].id << "\n\tcall WriteLine\n";
						break;
					}
					else
						*out << "\tpush ";
					*out << idtable.table[lextable.table[i + 1].idxTi].func + "_" + idtable.table[lextable.table[i + 1].idxTi].id << "\n\tcall WriteLine\n";
				}
				break;
			}
			case LEX_WRITELN:
			{
				if (idtable.table[lextable.table[i + 1].idxTi].iddatatype == IT::IDDATATYPE::NUMB)
				{
					if (idtable.table[lextable.table[i + 1].idxTi].idtype == IT::L)
						*out << "\tmovsx eax, " << idtable.table[lextable.table[i + 1].idxTi].id << "\n";
					else
						*out << "\tmovsx eax, " << idtable.table[lextable.table[i + 1].idxTi].func + "_" + idtable.table[lextable.table[i + 1].idxTi].id << "\n";
					*out << "\tpush eax" << "\n\tcall WriteNumbLn\n";
				}
				else if (idtable.table[lextable.table[i + 1].idxTi].iddatatype == IT::IDDATATYPE::BOOL)
				{
					if (idtable.table[lextable.table[i + 1].idxTi].idtype == IT::L)
						*out << "\tmovsx eax, " << idtable.table[lextable.table[i + 1].idxTi].id << "\n";
					else
						*out << "\tmovsx eax, " << idtable.table[lextable.table[i + 1].idxTi].func + "_" + idtable.table[lextable.table[i + 1].idxTi].id << "\n";
					*out << "\tpush eax" << "\n\tcall WriteBoolLn\n";
				}
				else if (idtable.table[lextable.table[i + 1].idxTi].iddatatype == IT::IDDATATYPE::SYMB)
				{
					if (idtable.table[lextable.table[i + 1].idxTi].idtype == IT::IDTYPE::L)
					{
						*out << "\tpush offset ";
						*out << idtable.table[lextable.table[i + 1].idxTi].id << "\n\tcall WriteSymbLn\n";
						break;
					}
					else
						*out << "\tpush ";
					*out << idtable.table[lextable.table[i + 1].idxTi].func + "_" + idtable.table[lextable.table[i + 1].idxTi].id << "\n\tcall WriteSymbLn\n";
				}
				else
				{
					if (idtable.table[lextable.table[i + 1].idxTi].idtype == IT::IDTYPE::L)
					{
						*out << "\tpush offset ";
						*out << idtable.table[lextable.table[i + 1].idxTi].id << "\n\tcall WriteLineLn\n";
						break;
					}
					else
						*out << "\tpush ";
					*out << idtable.table[lextable.table[i + 1].idxTi].func + "_" + idtable.table[lextable.table[i + 1].idxTi].id << "\n\tcall WriteLineLn\n";
				}
				break;
			}
			}
		}


	}
};