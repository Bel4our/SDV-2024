#include "stdafx.h"
#include "FST.h"
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>
#include <utility>
#include <mbstring.h>
#include "Nodes.h"


namespace  Lexer
{

	bool isFunc = false;
	string currentFunc;
	bool unaryMinus = false;



	IT::Entry createEntry(int index, const std::string& name, IT::IDTYPE idType, IT::IDDATATYPE dataType, int defaultVal, std::string currentFunc) 
	{
		return IT::Entry{ index, name, dataType, idType, currentFunc, defaultVal };
	}

	bool check(std::string s, FST::FST fst)
	{
		fst.string = s;

		return FST::execute(fst);
	}

	void checkElInIdTable(
		const std::vector<std::pair<std::string, int>>& words,
		int i,
		LT::LexTable& lextable,
		IT::IdTable& idtable,
		int& indIdTab)
	{
		
		int existingId = IT::IsId(idtable,   words[i].first);
		for (int k = 0; k < idtable.size; k++)
		{
			for (int k = 0; k < idtable.size; k++)
			{
				if (idtable.table[k].id == words[i].first && idtable.table[k].func == currentFunc)
				{
					existingId = k;
				}
			}
		}
		int existFunc = IT::IsId(idtable, words[i].first);


		if (i > 2 && LT::GetEntry(lextable, i - 1).lexema == 'f' && LT::GetEntry(lextable, i - 3).lexema == 'd') 
		{

			

			if (existFunc == TI_NULLIDX) {
				IT::IDDATATYPE dataType;
				int defaultVal;

				if (words[i - 2].first == "numb")
				{
					dataType = IT::NUMB; defaultVal = TI_INT_DEFAULT;
				}
				else if (words[i - 2].first == "line")
				{
					dataType = IT::LINE; defaultVal = TI_STR_DEFAULT;
				}
				else if (words[i - 2].first == "symb")
				{
					dataType = IT::SYMB; defaultVal = TI_CHAR_DEFAULT;
				}
				else if (words[i - 2].first == "bool")
				{
					dataType = IT::BOOL; defaultVal = TI_BOOL_DEFAULT;
				}
				else {
					ERROR_THROW_IN(311, words[i].second, i);
				}
				currentFunc = words[i].first;

				IT::Add(idtable, createEntry(i + 1, words[i].first, IT::F, dataType, defaultVal, "idOfFunc"));
				indIdTab++;
			}
			else
			{
				ERROR_THROW_IN(308, words[i].second, i);
			}
		}
		else if (i > 1 && LT::GetEntry(lextable, i - 1).lexema == 't' && LT::GetEntry(lextable, i - 2).lexema == 'd' ){
			if (existingId == TI_NULLIDX || ((existingId != TI_NULLIDX && ((idtable.table[existingId].idtype == IT::P && idtable.table[existingId].func != currentFunc) || IT::GetEntry(idtable, existingId).func != "idOfFunc"))
				&& (existingId != TI_NULLIDX && ((idtable.table[existingId].idtype == IT::P && idtable.table[existingId].func != currentFunc) || IT::GetEntry(idtable, existingId).func != currentFunc))))
			{
				IT::IDDATATYPE dataType;
				int defaultVal;

				if (words[i - 1].first == "numb")
				{
					dataType = IT::NUMB; defaultVal = TI_INT_DEFAULT;
				}
				else if (words[i - 1].first == "line")
				{
					dataType = IT::LINE; defaultVal = TI_STR_DEFAULT;
				}
				else if (words[i - 1].first == "symb")
				{
					dataType = IT::SYMB; defaultVal = TI_CHAR_DEFAULT;
				}
				else if (words[i - 1].first == "bool")
				{
					dataType = IT::BOOL; defaultVal = TI_BOOL_DEFAULT;
				}
				else
				{
					ERROR_THROW_IN(307, words[i].second, i);
				}

				IT::Add(idtable, createEntry(i + 1, words[i].first, IT::V, dataType, defaultVal, currentFunc));

				indIdTab++;
			}
			else
			{
				if (idtable.table[existingId].idtype == IT::P)
				{
					ERROR_THROW_IN(309, words[i].second, -1);
				}
				ERROR_THROW_IN(310, words[i].second, i);
			}
		}
		else if (i > 3 &&  (LT::GetEntry(lextable, i - 1).lexema == 't' && (LT::GetEntry(lextable, i - 2).lexema == ',' || LT::GetEntry(lextable, i - 4).lexema == 'f')))
		{
			if (existingId != TI_NULLIDX)
			{
				if(idtable.table[existingId].idtype == IT::P && idtable.table[existingId].func == currentFunc)
				{
					ERROR_THROW_IN(306, words[i].second, i);
				}

			}
			IT::IDDATATYPE dataType;
			int defaultVal;


			if (words[i - 1].first == "numb") { dataType = IT::NUMB; defaultVal = TI_INT_DEFAULT; }
			else if (words[i - 1].first == "line")
			{
				dataType = IT::LINE; defaultVal = TI_STR_DEFAULT; 
			}
			else if (words[i - 1].first == "symb") 
			{
				dataType = IT::SYMB; defaultVal = TI_CHAR_DEFAULT;
			}
			else if (words[i - 1].first == "bool")
			{
				dataType = IT::BOOL; defaultVal = TI_BOOL_DEFAULT;
			}
			else { 
				ERROR_THROW_IN(307, words[i].second, i);
			}

			IT::Add(idtable, createEntry(i + 1,   words[i].first, IT::P, dataType, defaultVal, currentFunc));
			
			indIdTab++;
		}
		else 
		{

			if (lextable.table[lextable.size - 2].lexema == LEX_INTEGER && lextable.table[lextable.size - 3].lexema != LEX_DECLARE && lextable.table[lextable.size - 1].lexema == LEX_FUNCTION)
				ERROR_THROW_IN(305, words[i].second, i);
			if (lextable.table[lextable.size - 2].lexema == LEX_DECLARE && lextable.table[lextable.size - 1].lexema != LEX_INTEGER && lextable.table[lextable.size - 1].lexema == LEX_FUNCTION)
				ERROR_THROW_IN(311, words[i].second, i);
			if (lextable.table[lextable.size - 1].lexema == LEX_INTEGER && lextable.table[lextable.size - 2].lexema != LEX_DECLARE )
				ERROR_THROW_IN(305, words[i].second, i);
			if (lextable.table[lextable.size - 1].lexema == LEX_DECLARE && lextable.table[lextable.size - 1].lexema != LEX_INTEGER)
				ERROR_THROW_IN(307, words[i].second, i);

			
			if (existingId == TI_NULLIDX)
			{
				ERROR_THROW_IN(306, words[i].second, i);
			}
			else if ((idtable.table[existingId].func != currentFunc) && !(idtable.table[existingId].func == "idOfFunc"))
			{
				ERROR_THROW_IN(306, words[i].second, i );
			}
		}

		
	}


	In::IN clearCode(In::IN in)
	{
		int j = 0;
		unsigned char* textWithoutSpaces = new unsigned char[IN_MAX_LEN_TEXT];
		
		int countSpaces = 0;
		bool strLiteralIsOpen = false;
		bool symbLiteralIsOpen = false;

		for (int i = 0; i < in.size; i++)
		{
			if (in.text[i] == '\0')
			{
				break;
			}
			else {
				if (i == 0 && in.text[i] == ' ')
				{
					int k = 0;
					while (in.text[k] == ' ')
					{
						in.countSpaces++;
						k++;
						continue;
					}
					i = k - 1;
				}
				else
					if (in.text[i] == ' ' && in.text[i + 1] == ' ' && !strLiteralIsOpen && !symbLiteralIsOpen)
					{
						in.countSpaces++;
						continue;
					}
					else if (in.text[i] == ' ' && in.text[i - 1] == '\n')
					{
						in.countSpaces++;
						in.countSpaces++;

						continue;
					}
					else if (in.text[i] == '\"')
					{
						if (strLiteralIsOpen)
						{
							strLiteralIsOpen = false;
						}
						else {
							strLiteralIsOpen = true;
						}
						textWithoutSpaces[j++] = in.text[i];
					}

					else if (in.text[i] == '\'')
					{
						if (symbLiteralIsOpen)
						{
							symbLiteralIsOpen = false;
						}
						else {
							symbLiteralIsOpen = true;
						}
						textWithoutSpaces[j++] = in.text[i];
					}

					else if (in.text[i] == ' ' && !strLiteralIsOpen && !symbLiteralIsOpen && (in.text[i + 1] == '(' || in.text[i + 1] == ';' || in.text[i + 1] == ',' || in.text[i + 1] == '{' || in.text[i + 1] == ')' || in.text[i + 1] == '}' || in.text[i + 1] == '=' || in.text[i + 1] == '+' || in.text[i + 1] == '-' || in.text[i + 1] == '*' || in.text[i + 1] == '/' || in.text[i + 1] == '%' || in.text[i + 1] == '\n' || in.text[i + 1] == '\0'))
					{
						in.countSpaces++;
						continue;
					}
					else if (in.text[i] == ' ' &&  !strLiteralIsOpen && !symbLiteralIsOpen && (textWithoutSpaces[j - 1] == '(' || in.text[i + 1] == ';' || in.text[i - 1] == ',' || textWithoutSpaces[j - 1] == '{' || textWithoutSpaces[j - 1] == ')' || textWithoutSpaces[j - 1] == '}' || textWithoutSpaces[j - 1] == '=' || textWithoutSpaces[j - 1] == '+' || textWithoutSpaces[j - 1] == '-' || textWithoutSpaces[j - 1] == '*' || textWithoutSpaces[j - 1] == '/' || textWithoutSpaces[j - 1] == '%'))
					{
						in.countSpaces++;
						continue;
					}
			
					else {
						textWithoutSpaces[j++] = in.text[i];
					}
			}
		}

		in.text = textWithoutSpaces;
		in.text[j++] = '\0';
		in.size = j;

		return in;
	}

	void Run(LT::LexTable& lextable, IT::IdTable& idtable, In::IN in) 
	{

		int lineNumber = 1;
		string bufWord = "";
		int indLexTab = 0;
		int indIdTab = 0;


		int size = 0;
		bool doubleQuoteIsFinded = false;
		bool singleQuoteIsFinded = false;
		int countS = 0;
		int countD = 0;
		bool fEnd = false;

		int main = 0, open = 0, close = 0;
		
		in = clearCode(in);

		std::vector<std::pair<std::string, int>> words;

		for (int i = 0; !fEnd;i++)
		{
			if (in.text[i] == '\0')
			{
				fEnd = true;
				break;
			}
			if (in.text[i] == '\n')
			{
				if (bufWord != "")
				{
					words.push_back({ bufWord,lineNumber });
					bufWord = "";
					size++;
				}
				lineNumber++;
			}
			else if (in.text[i] == ' ' && !doubleQuoteIsFinded && !singleQuoteIsFinded)
			{

				if (bufWord != "")
				{
					words.push_back({ bufWord,lineNumber });
					bufWord = "";
					size++;
				}

			}
			else if(!doubleQuoteIsFinded && !singleQuoteIsFinded &&((in.text[i] == '!' && in.text[i + 1] == '=' )||(in.text[i] == '=' && in.text[i+1] == '=') || (in.text[i] == '=' && in.text[i + 1] == '<') || (in.text[i] == '>' && in.text[i + 1] == '=')))
			{
				if (bufWord != "")
				{
					words.push_back({ bufWord,lineNumber });
					bufWord = "";
					size++;
				}
				bufWord += in.text[i];
				bufWord += in.text[i+1];
				i++;
				words.push_back({ bufWord, lineNumber });
				bufWord = "";
				size++;
			}

			else if (!doubleQuoteIsFinded && !singleQuoteIsFinded &&((in.text[i] == ',' || in.text[i] == ';' || in.text[i] == '{' || in.text[i] == '}' || in.text[i] == '(' || in.text[i] == ')'
				|| in.text[i] == '*' || in.text[i] == '/' || in.text[i] == '%' || in.text[i] == '+' || in.text[i] == '-' || in.text[i] == '=' || in.text[i] == '<' || in.text[i] == '>') &&(!singleQuoteIsFinded && !doubleQuoteIsFinded) ))
			{
				if (bufWord != "")
				{
					words.push_back({ bufWord,lineNumber });
					bufWord = "";
					size++;
				}
				bufWord += in.text[i];
				words.push_back({ bufWord, lineNumber });
				bufWord = "";
				size++;
			}
			
			else if (in.text[i] == '\'')
			{
				if (singleQuoteIsFinded)
				{
					singleQuoteIsFinded = false;
					countS++;
				}
				else
				{
					singleQuoteIsFinded = true;
					countS++;
					
				}
				bufWord += in.text[i];
			}
			else if (in.text[i] == '\"')
			{
				if (doubleQuoteIsFinded)
				{
					doubleQuoteIsFinded = false;
					countD++;
				}
				else
				{
					doubleQuoteIsFinded = true;
					countD++;
				}
				bufWord += in.text[i];
			}
			else
			{
				bufWord += in.text[i];
			}
		}
		if (countS % 2 != 0)
		{
			ERROR_THROW(324);
		}

		if (countD % 2 != 0)
		{
			ERROR_THROW(300);
		}



		int currLine = 0;
		int lineNumb = 1;
		
		IT::Entry bufEnt = {};
		bool findParm = false;
		int idParam = -1;
		int idFunc = -1;
		int litCount = 0;

		for (int i = 0;i < size;i++)
		{
			if (check(words[i].first, typeInteger))
			{
				LT::Entry ent = { LEX_INTEGER, words[i].second };
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeString))
			{
				LT::Entry ent = { LEX_STRING,  words[i].second };
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeChar))
			{
				LT::Entry ent = { LEX_CHAR,  words[i].second };
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeBool))
			{
				LT::Entry ent = { LEX_BOOL,  words[i].second };
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeFunction))
			{
				LT::Entry ent = { LEX_FUNCTION,  words[i].second };

				idFunc = indIdTab;
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeDeclare))
			{
				LT::Entry ent = { LEX_DECLARE,  words[i].second };
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeWrite))
			{
				LT::Entry ent = { LEX_WRITE,  words[i].second };
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeWriteln))
			{
				LT::Entry ent = { LEX_WRITELN,  words[i].second };
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeReturn))
			{
				LT::Entry ent = { LEX_RETURN,  words[i].second };
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeMain))
			{
				LT::Entry ent = { LEX_MAIN,  words[i].second };
				if (lextable.table[i - 2].lexema == LEX_DECLARE || lextable.table[i - 3].lexema == LEX_DECLARE
					|| lextable.table[i - 4].lexema == LEX_FUNCTION || lextable.table[i - 2].lexema == LEX_COMMA)
				{
					ERROR_THROW_IN(327, words[i].second, -1)
				}
				LT::Add(lextable, ent);
				main++;
				currentFunc = "main";
			}


			else if (check(words[i].first, typeIf))
			{
				LT::Entry ent = { LEX_CHECK,  words[i].second };
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeElse))
			{
				LT::Entry ent = { LEX_OTHERWISE,  words[i].second };
				LT::Add(lextable, ent);
			}

			else if (check(words[i].first, typeTrue))
			{
				
				LT::Entry ent = { LEX_TRUE,  words[i].second };

				if (lextable.table[i - 1].lexema == '=' && lextable.table[i - 2].lexema == 'i')
				{
					int buf = IT::IsId(idtable, words[i - 2].first);
					idtable.table[buf].value.vint = 1;

				}
				IT::Entry entryIT;
				entryIT.id = "Lit" + std::to_string(++litCount);
				entryIT.idtype = IT::IDTYPE::L;
				entryIT.iddatatype = IT::IDDATATYPE::BOOL;
				entryIT.value.vint = 1;
				entryIT.idxfirstLE = i + 1;
				entryIT.func = currentFunc;
				ent.idxTi = indIdTab++;
				IT::Add(idtable, entryIT);
				LT::Add(lextable, ent);
				
			}
			else if (check(words[i].first, typeFalse))
			{
				LT::Entry ent = { LEX_FALSE,  words[i].second };

				if (lextable.table[i - 1].lexema == '=' && lextable.table[i - 2].lexema == 'i')
				{
					int buf = IT::IsId(idtable, words[i - 2].first);
					idtable.table[buf].value.vint = 1;
				}
				IT::Entry entryIT;
				entryIT.id = "Lit" + std::to_string(++litCount);
				entryIT.idtype = IT::IDTYPE::L;
				entryIT.iddatatype = IT::IDDATATYPE::BOOL;
				entryIT.value.vint = 0;
				entryIT.idxfirstLE = i + 1;
				entryIT.func = currentFunc;
				ent.idxTi = indIdTab++;

				IT::Add(idtable, entryIT);
				LT::Add(lextable, ent);
			}
		



			else if (check(words[i].first, typeCONCAT))
			{
				LT::Entry ent = { LEX_CONCAT,  words[i].second, indIdTab++ };
				IT::Entry entryIT;
				entryIT.id = words[i].first;
				entryIT.idtype = IT::IDTYPE::S;
				entryIT.idxfirstLE = i;
				entryIT.iddatatype = CONCAT_TYPE;
				entryIT.value.params.count = CONCAT_PARAMS_CNT;
				entryIT.value.params.types = new IT::IDDATATYPE[CONCAT_PARAMS_CNT];
				if (lextable.table[i - 2].lexema == LEX_DECLARE || lextable.table[i - 3].lexema == LEX_DECLARE
					|| lextable.table[i - 4].lexema == LEX_FUNCTION || lextable.table[i - 2].lexema == LEX_COMMA)
				{
					ERROR_THROW_IN(326, words[i].second, -1)
				}
				for (int k = 0; k < CONCAT_PARAMS_CNT; k++)
					entryIT.value.params.types[k] = IT::CONCAT_PARAMS[k];
				IT::Add(idtable, entryIT);

				LT::Add(lextable, ent);
			}

			else if (check(words[i].first, typeLENLINE))
			{
				LT::Entry ent = { LEX_LENLINE,  words[i].second, indIdTab++ };
				IT::Entry entryIT;
				entryIT.id = words[i].first;
				entryIT.idtype = IT::IDTYPE::S;
				entryIT.iddatatype = LINELEN_TYPE;
				entryIT.value.params.count = LINELEN_PARAMS_CNT;
				entryIT.value.params.types = new IT::IDDATATYPE[LINELEN_PARAMS_CNT];
				entryIT.idxfirstLE = i;
				if (lextable.table[i - 2].lexema == LEX_DECLARE || lextable.table[i - 3].lexema == LEX_DECLARE
					|| lextable.table[i - 4].lexema == LEX_FUNCTION || lextable.table[i - 2].lexema == LEX_COMMA)
				{
					ERROR_THROW_IN(326, words[i].second, -1)
				}
				for (int k = 0; k < LINELEN_PARAMS_CNT; k++)
					entryIT.value.params.types[k] = IT::LINELEN_PARAMS[k];

				IT::Add(idtable, entryIT);
				LT::Add(lextable, ent);
			}

			else if (check(words[i].first, typeRANDOM))
			{
				LT::Entry ent = { LEX_RANDOM,  words[i].second, indIdTab++ };
				IT::Entry entryIT;
				entryIT.id = words[i].first;
				entryIT.idtype = IT::IDTYPE::S;
				entryIT.iddatatype = RANDOM_TYPE;;
				entryIT.value.params.count = RANDOM_PARAMS_CNT;
				entryIT.value.params.types = new IT::IDDATATYPE[RANDOM_PARAMS_CNT];
				entryIT.idxfirstLE = i;

				if (lextable.table[i - 2].lexema == LEX_DECLARE || lextable.table[i - 3].lexema == LEX_DECLARE
					|| lextable.table[i - 4].lexema == LEX_FUNCTION || lextable.table[i - 2].lexema == LEX_COMMA)
				{
					ERROR_THROW_IN(326, words[i].second, -1)
				}

				for (int k = 0; k < RANDOM_PARAMS_CNT; k++)
					entryIT.value.params.types[k] = IT::RANDOM_PARAMS[k];
				IT::Add(idtable, entryIT);
				LT::Add(lextable, ent);

			}


			else if (check(words[i].first, typeIntLiteral))
			{
				LT::Entry ent = { LEX_LITERAL,  words[i].second };
				if (words[i].first.length() > 10)
				{
					ERROR_THROW_IN(319, words[i].second, i);
				}
				long long value = std::stoll(words[i].first);
				if (unaryMinus)
				{
					value = -value;
				}
				if (value < -32768 || value > 32767)
				{
					ERROR_THROW_IN(319, words[i].second, i);
				}
				IT::Entry entryIT;
				entryIT.id = "Lit" + std::to_string(++litCount);
				entryIT.idtype = IT::IDTYPE::L;
				entryIT.iddatatype = IT::IDDATATYPE::NUMB;
				entryIT.value.vint = static_cast<int>(value);
				entryIT.idxfirstLE = i+1;
				entryIT.func = currentFunc;

				IT::Add(idtable, entryIT);
				ent.idxTi = indIdTab++;
	


				unaryMinus = false;
				LT::Add(lextable, ent);
				}

		
			else if (check(words[i].first, typeCharLiteral))
			{
				LT::Entry ent = { LEX_LITERAL,  words[i].second };

				if (words[i].first.size() > TI_CHAR_MAXSIZE)
				{
					ERROR_THROW_IN(312, words[i].second, i);
				}
				if (words[i].first.size() == 2)
				{
					ERROR_THROW_IN(318, words[i].second, i);
				}
				IT::Entry entryIT;
				entryIT.id = "Lit" + std::to_string(++litCount);
				entryIT.idtype = IT::IDTYPE::L;
				entryIT.iddatatype = IT::IDDATATYPE::SYMB;
				entryIT.value.vchar = words[i].first[1];
				entryIT.idxfirstLE = i+1;
				entryIT.func = currentFunc;
				IT::Add(idtable, entryIT);
				ent.idxTi = indIdTab++;
				LT::Add(lextable, ent);
			}

			else if (check(words[i].first, typeRightBrace))
			{
				LT::Entry ent = { LEX_BRACELET,  words[i].second };
				isFunc = false;
				currentFunc = "global";
				close++;
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeLeftBrace))
			{
				LT::Entry ent = { LEX_LEFTBRACE,  words[i].second };
				isFunc = true;
				open++;
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeRightThesis))
			{
				LT::Entry ent = { LEX_RIGHTHESIS,  words[i].second };
				findParm = false;
				idParam = -1;
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeLeftThesis))
			{
				LT::Entry ent = { LEX_LEFTHESIS,  words[i].second };
				findParm = true;
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeRIGHTBRACKET))
			{
				LT::Entry ent = { LEX_RIGHTBRACKET,  words[i].second };
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeLEFTTBRACKET))
			{
				LT::Entry ent = { LEX_LEFTBRACKET,  words[i].second };
				LT::Add(lextable, ent);
			}

			else if (check(words[i].first, typeSemicolon))
			{
				LT::Entry ent = { LEX_SEMICOLON,  words[i].second };
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeComma))
			{
				LT::Entry ent = { LEX_COMMA,  words[i].second };
				LT::Add(lextable, ent);
			}
			



			else if (check(words[i].first, typePlus))
			{
				LT::Entry ent = { LEX_PLUS,  words[i].second };
				ent.oper = LT::OPERATION::PLUS;
				ent.priority = 2;
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeMinus))
			{
				LT::Entry ent = { LEX_MINUS,  words[i].second };
				if (!(lextable.table[lextable.size - 1].lexema == LEX_LITERAL || lextable.table[lextable.size - 1].lexema == LEX_ID || lextable.table[lextable.size - 1].lexema == LEX_TRUE || lextable.table[lextable.size - 1].lexema == LEX_RIGHTHESIS))
				{
					unaryMinus = true;
					ent.lexema = LEX_UNARY_MINUS;
					ent.oper = LT::OPERATION::UNMIN;
				}
				else
				{
					ent.lexema = LEX_MINUS;
					ent.oper = LT::OPERATION::MINUS;
					ent.priority = 2;
				}

				LT::Add(lextable, ent);

			}
			else if (check(words[i].first, typeMulti))
			{
				LT::Entry ent = { LEX_STAR,  words[i].second };
				ent.oper = LT::OPERATION::MUL;
				ent.priority = 3;
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeDel))
			{
				LT::Entry ent = { LEX_DIRSLASH,  words[i].second };
				ent.oper = LT::OPERATION::DIV;
				ent.priority = 3;
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typePerc))
			{
				LT::Entry ent = { LEX_PERC,  words[i].second };
				ent.oper = LT::OPERATION::REST;
				ent.priority = 3;
				LT::Add(lextable, ent);
			}



			else if (check(words[i].first, typeEquals))
			{
				LT::Entry ent = { LEX_EQUALS,  words[i].second };
	
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeCheckEquals))
			{
				LT::Entry ent = { LEX_CHECK_EQ,  words[i].second };
				ent.oper = LT::OPERATION::EQU;
				ent.priority = 3;
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeCheckUnequals))
			{
				LT::Entry ent = { LEX_CHECK_UNEQ,  words[i].second };
				ent.oper = LT::OPERATION::NOEQU;
				ent.priority = -1;
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeMoreOrEquals))
			{
				LT::Entry ent = { LEX_MORE_OR_EQ,  words[i].second };
				ent.oper = LT::OPERATION::MOREQ;
				ent.priority = -1;
				LT::Add(lextable, ent);
				}
			else if (check(words[i].first, typeLessOrEquals))
			{
				LT::Entry ent = { LEX_LESS_OR_EQ,  words[i].second };
				ent.oper = LT::OPERATION::LESSQ;
				ent.priority = -1;
				LT::Add(lextable, ent);
				}
			else if (check(words[i].first, typeMore))
			{
				LT::Entry ent = { LEX_MORE,  words[i].second };
				ent.oper = LT::OPERATION::MORE;
				ent.priority = -1;
				LT::Add(lextable, ent);
			}
			else if (check(words[i].first, typeLess))
			{
				LT::Entry ent = { LEX_LESS,  words[i].second };
				ent.oper = LT::OPERATION::LESS;
				ent.priority = -1;
				LT::Add(lextable, ent);
			}



			else if (check(words[i].first, typeIdentificator))
			{
				LT::Entry ent = { LEX_ID,  words[i].second };

				checkElInIdTable(words, i, lextable, idtable, indIdTab);
				int index = IT::IsId(idtable,   words[i].first);
				
				for (int k = 0; k < idtable.size; k++)
				{
					if (idtable.table[k].id == words[i].first && idtable.table[k].func == currentFunc)
					{
						index = k;
					}
				}
				ent.idxTi = index;

				if (idtable.table[idtable.size-1].idtype == IT::F)
				{
					idtable.table[idtable.size-1].value.params.count = 0;
					idtable.table[idtable.size-1].value.params.types = new IT::IDDATATYPE[MAX_PARAMS_COUNT];
				}

				if (findParm && lextable.table[i-1].lexema == 't')
				{
					bufEnt.idtype = IT::IDTYPE::P;
					idtable.table[idFunc].value.params.count++;
					idParam++;

					if (idtable.table[idFunc].value.params.count > 3)
						ERROR_THROW_IN(314, -1, -1);

					idtable.table[idFunc].value.params.types[idParam] = idtable.table[idtable.size - 1].iddatatype;
				}

				
	

				LT::Add(lextable, ent);
				}

			else if (check(words[i].first, typeStringLiteral))
			{
				LT::Entry ent = { LEX_LITERAL,  words[i].second };
				if (words[i].first.size() > TI_STR_MAXSIZE)
				{
					ERROR_THROW_IN(304, words[i].second, i);
				}

				if (words[i].first.size() == 2)
				{
					ERROR_THROW_IN(318, words[i].second, i);
				}

				IT::Entry entryIT;
				entryIT.id = "Lit" + std::to_string(++litCount);
				entryIT.idtype = IT::IDTYPE::L;
				entryIT.iddatatype = IT::IDDATATYPE::LINE;
				strncpy(entryIT.value.vstr->str, words[i].first.c_str(), TI_STR_MAXSIZE - 1);
				entryIT.value.vstr->len = (int)words[i].first.size();
				entryIT.idxfirstLE = i + 1;
				entryIT.func = currentFunc;


				IT::Add(idtable, entryIT);
				ent.idxTi = indIdTab++;

				LT::Add(lextable, ent);
				}
			else {
				ERROR_THROW_IN(207,words[i].second,i);
			}
		}


		if (main == NULL)
			ERROR_THROW(302);
		if (main > 1)
			ERROR_THROW(303);
		if (open > close)
			ERROR_THROW(612);
		if (close > open)
			ERROR_THROW(613);

	}
}



