#include "stdafx.h"
#include "SemAnalysis.h"

namespace Semantics
{
	void AnalyzeSem(LT::LexTable lextable, IT::IdTable idtable)
	{
		for (int i = 0; i < lextable.size; i++)
		{
			switch (lextable.table[i].lexema)
			{
		
			case LEX_LENLINE:
			case LEX_CONCAT:
			case LEX_RANDOM:
			case LEX_ID:
			{
				if ((idtable.table[lextable.table[i].idxTi].idtype == IT::IDTYPE::F || idtable.table[lextable.table[i].idxTi].idtype == IT::IDTYPE::S)
					&& lextable.table[i - 1].lexema != LEX_FUNCTION)
				{
					IT::Entry e = idtable.table[lextable.table[i].idxTi];
					int countParm = 0;
					if (lextable.table[i + 1].lexema != LEX_LEFTHESIS)
						 ERROR_THROW_IN(608, lextable.table[i].sn, -1);

					

					for (int j = i + 2; lextable.table[j].lexema != LEX_RIGHTHESIS; j++)
					{
						if (lextable.table[j].lexema == LEX_ID || lextable.table[j].lexema == LEX_LITERAL || lextable.table[j].lexema == LEX_TRUE)
						{
							countParm++;
							if (countParm > e.value.params.count)
								 ERROR_THROW_IN(316, lextable.table[j].sn, -1);
							if (idtable.table[lextable.table[j].idxTi].iddatatype == IT::IDDATATYPE::NUMB && e.value.params.types[countParm - 1] != IT::IDDATATYPE::NUMB)
								 ERROR_THROW_IN(315, lextable.table[j].sn, -1);
							if (idtable.table[lextable.table[j].idxTi].iddatatype == IT::IDDATATYPE::LINE && e.value.params.types[countParm - 1] != IT::IDDATATYPE::LINE)
								 ERROR_THROW_IN(315, lextable.table[j].sn, -1);
							if (idtable.table[lextable.table[j].idxTi].iddatatype == IT::IDDATATYPE::BOOL && e.value.params.types[countParm - 1] != IT::IDDATATYPE::BOOL)
								ERROR_THROW_IN(315, lextable.table[j].sn, -1);
							if (idtable.table[lextable.table[j].idxTi].iddatatype == IT::IDDATATYPE::SYMB && e.value.params.types[countParm - 1] != IT::IDDATATYPE::SYMB)
								ERROR_THROW_IN(315, lextable.table[j].sn, -1);
						}

					}
					if (countParm < e.value.params.count)
						 ERROR_THROW_IN(317, lextable.table[i].sn, -1);
				}
				break;
			}

			case LEX_EQUALS:
			{
				if (lextable.table[i - 1].lexema == LEX_ID || lextable.table[i - 1].lexema == LEX_LITERAL)
				{
					IT::IDDATATYPE lefttype = idtable.table[lextable.table[i - 1].idxTi].iddatatype;
					bool ignore = false;

					for (int k = i + 1; lextable.table[k].lexema != LEX_SEMICOLON; k++)
					{
						if (lextable.table[k].lexema == LEX_ID || lextable.table[k].lexema == LEX_LITERAL || lextable.table[k].lexema == LEX_TRUE ||
							lextable.table[k].lexema == LEX_CONCAT || lextable.table[k].lexema == LEX_LENLINE ||lextable.table[k].lexema == LEX_RANDOM)
						{
							if (!ignore)
							{
								IT::IDDATATYPE righttype = idtable.table[lextable.table[k].idxTi].iddatatype;
								if (lefttype == IT::IDDATATYPE::NUMB)
								{
									if (righttype != IT::IDDATATYPE::NUMB && righttype != IT::IDDATATYPE::BOOL)
										 ERROR_THROW_IN(320, lextable.table[i].sn, -1);
								} else 
								if (lefttype == IT::IDDATATYPE::BOOL)
								{
									if (righttype != IT::IDDATATYPE::BOOL/* && righttype != IT::IDDATATYPE::NUMB*/)
										ERROR_THROW_IN(320, lextable.table[i].sn, -1);
								}

								else if (lefttype != righttype)
									 ERROR_THROW_IN(320, lextable.table[i].sn, -1);
							}

							if (lextable.table[k + 1].lexema == LEX_LEFTHESIS)
							{
								ignore = true;
								continue;
							}

							if (ignore && lextable.table[k + 1].lexema == LEX_RIGHTHESIS)
							{
								ignore = false;
								continue;
							}
						}

						if (lefttype == IT::IDDATATYPE::LINE || lefttype == IT::IDDATATYPE::SYMB)
						{
							char l = lextable.table[k].lexema;
							if (l == LEX_PLUS)
								 ERROR_THROW_IN(321, lextable.table[i].sn, -1);
						}
					}
					break;
				}
				break;

			}

			case LEX_FUNCTION:
			{
			
				int j = i;
				if (lextable.table[i + 1].lexema == LEX_ID)
				{
					IT::Entry e = idtable.table[lextable.table[j + 1].idxTi];
					j++;
					while (j <= lextable.size && lextable.table[j].lexema != LEX_FUNCTION && lextable.table[j].lexema != LEX_MAIN)
					{
						if (lextable.table[j].lexema == LEX_RETURN && (lextable.table[j + 1].lexema == LEX_ID || lextable.table[j + 1].lexema == LEX_LITERAL || lextable.table[j + 1].lexema == LEX_TRUE))
						{
							int next = lextable.table[j + 1].idxTi;
							if (next != TI_NULLIDX)
							{
								if (idtable.table[next].iddatatype != e.iddatatype)
									 ERROR_THROW_IN(313, lextable.table[j].sn, -1);
							}
						}
						j++;
					}
				}
				break;
			}

			case LEX_LESS:
			{
				bool flag = true;
				if (lextable.table[i - 1].lexema == LEX_ID || lextable.table[i - 1].lexema == LEX_LITERAL || lextable.table[i - 1].lexema == LEX_TRUE)
				{
					if (idtable.table[lextable.table[i - 1].idxTi].iddatatype != IT::IDDATATYPE::NUMB && idtable.table[lextable.table[i - 1].idxTi].iddatatype != IT::IDDATATYPE::BOOL)
						flag = false;
				}
				if (lextable.table[i + 1].lexema == LEX_ID || lextable.table[i + 1].lexema == LEX_LITERAL || lextable.table[i - 1].lexema == LEX_TRUE)
				{
					if (idtable.table[lextable.table[i + 1].idxTi].iddatatype != IT::IDDATATYPE::NUMB && idtable.table[lextable.table[i + 1].idxTi].iddatatype != IT::IDDATATYPE::BOOL)
						flag = false;
				}
				if (lextable.table[i - 1].lexema == LEX_ID
					&& idtable.table[lextable.table[i - 1].idxTi].idtype == IT::IDTYPE::F)
					 ERROR_THROW_IN(615, lextable.table[i].sn, -1);
				if (lextable.table[i + 1].lexema == LEX_ID
					&& idtable.table[lextable.table[i + 1].idxTi].idtype == IT::IDTYPE::F)
					 ERROR_THROW_IN(615, lextable.table[i].sn, -1);

				if (!flag)
				{
					 ERROR_THROW_IN(322, lextable.table[i].sn, -1);
				}
				break;
			}

			case LEX_PLUS:
			{
				if (lextable.table[i + 1].lexema == LEX_ID || lextable.table[i + 1].lexema == LEX_LITERAL || lextable.table[i + 1].lexema == LEX_TRUE)
				{
					if (lextable.table[i].oper == LT::OPERATION::DIV || lextable.table[i].oper == LT::OPERATION::REST)
					{
						LT::Entry e;
						IT::Entry et;
						bool equ = false;
						if (lextable.table[i + 1].lexema == LEX_LITERAL || lextable.table[i + 1].lexema == LEX_ID || lextable.table[i + 1].lexema == LEX_TRUE)
							e = lextable.table[i + 1];
						else
							e = lextable.table[i + 2];
						et = idtable.table[e.idxTi];

						if (lextable.table[i + 1].lexema == LEX_ID && idtable.table[lextable.table[i + 1].idxTi].idtype == IT::IDTYPE::V)
						{
							for (int n = i; n >= 0; n--)
							{
								if (lextable.table[n].lexema == LEX_ID && !(idtable.table[lextable.table[n].idxTi].id == et.id)
									&& lextable.table[n + 1].lexema == LEX_EQUALS)
								{
									if (lextable.table[n + 2].lexema == LEX_LITERAL || lextable.table[n + 2].lexema == LEX_ID && (idtable.table[lextable.table[n + 2].idxTi].iddatatype == IT::IDDATATYPE::NUMB))
									{
										equ = true;
										if (idtable.table[lextable.table[n + 2].idxTi].value.vint == 0)
											 ERROR_THROW_IN(323, lextable.table[i].sn, -1);
										break;
									}
									if (lextable.table[n + 2].lexema == LEX_ID && (idtable.table[lextable.table[n + 2].idxTi].iddatatype == IT::IDDATATYPE::NUMB)
										&& idtable.table[lextable.table[n + 2].idxTi].idtype == IT::IDTYPE::V)
									{
										et.id = idtable.table[lextable.table[n + 2].idxTi].id;
										continue;
									}
								}
							}
							if (!equ && idtable.table[lextable.table[i + 1].idxTi].value.vint == 0)
								 ERROR_THROW_IN(323, lextable.table[i].sn, -1);
						}
						if (lextable.table[i + 1].lexema == LEX_LITERAL || lextable.table[i + 1].lexema == LEX_ID)
						{
							if (idtable.table[e.idxTi].value.vint == 0)
								 ERROR_THROW_IN(323, lextable.table[i].sn, -1);
						}
					}
				}
				break;
			}

			case LEX_CHECK:
			{
				if ((lextable.table[i + 1].lexema == LEX_LEFTHESIS && lextable.table[i + 3].lexema == LEX_RIGHTHESIS) ||(lextable.table[i + 1].lexema == LEX_LEFTHESIS && lextable.table[i + 4].lexema == LEX_RIGHTHESIS))
				{
					if ((lextable.table[i + 2].lexema == LEX_ID || lextable.table[i + 2].lexema == LEX_LITERAL) && (idtable.table[lextable.table[i + 2].idxTi].iddatatype == IT::IDDATATYPE::LINE || idtable.table[lextable.table[i + 2].idxTi].iddatatype == IT::IDDATATYPE::SYMB))
						 ERROR_THROW_IN(301, lextable.table[i].sn, -1);
				}
				if (lextable.table[i + 2].lexema == LEX_ID && idtable.table[lextable.table[i + 2].idxTi].idtype == IT::IDTYPE::F)
					 ERROR_THROW_IN(615, lextable.table[i].sn, -1);
			}
			}
		}
	}
}