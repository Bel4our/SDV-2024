#include "stdafx.h"
#include "LT.h"
#include "queue"
#include "Polish.h"


namespace Poliz
{
	bool Poliz(int i, LT::LexTable& lextab, IT::IdTable& idtable)
	{
		std::stack<LT::Entry> stack;
		std::queue<LT::Entry> queue;

		LT::Entry temp;
		temp.idxTi = LT_TI_NULLIDX;
		temp.lexema = '*';
		temp.sn = lextab.table[i].sn;

		LT::Entry func;
		func.idxTi = LT_TI_NULLIDX;
		func.lexema = '@';
		func.sn = lextab.table[i].sn;

		int countLex = 0, countComma = 0;
		int countParm = 1;
		int posLex = i;
		char* buf = new char[1];

		bool findComma = false;
		bool findParm = false;
		bool callFunc = false;
		bool result = true;

		for (i; lextab.table[i].lexema != LEX_SEMICOLON; i++, countLex++)
		{
			switch (lextab.table[i].lexema)
			{
			case LEX_ID:
			{
				queue.push(lextab.table[i]);
				continue;
			}
			case LEX_CONCAT:
			{
				queue.push(lextab.table[i]);
				continue;
			}
			case LEX_LENLINE:
			{
				queue.push(lextab.table[i]);
				continue;
			}
			case LEX_RANDOM:
			{
				queue.push(lextab.table[i]);
				continue;
			}
			case LEX_LITERAL:
			{
				queue.push(lextab.table[i]);
				continue;
			}
			case LEX_TRUE:
			{
				queue.push(lextab.table[i]);
				continue;
			}
			case LEX_UNARY_MINUS: 
				continue;

			case LEX_PLUS:
			{
				while (!stack.empty() && lextab.table[i].priority <= stack.top().priority)
				{
					queue.push(stack.top());
					stack.pop();
				}
				stack.push(lextab.table[i]);
				continue;
			}
			case LEX_LESS:
			{
				while (!stack.empty() && lextab.table[i].priority <= stack.top().priority)
				{
					queue.push(stack.top());
					stack.pop();
				}
				stack.push(lextab.table[i]);
				continue;
			}
			case LEX_COMMA:
			{
				findComma = true;
				countComma++;
				continue;
			}
			case LEX_LEFTHESIS:
			{
				stack.push(lextab.table[i]);
				if ((lextab.table[i + 1].lexema == LEX_ID ||( lextab.table[i + 1].lexema == LEX_LITERAL || lextab.table[i + 2].lexema == LEX_LITERAL)|| lextab.table[i+1].lexema == LEX_TRUE) && (lextab.table[i - 1].lexema == LEX_ID || lextab.table[i - 1].lexema == LEX_LENLINE || lextab.table[i - 1].lexema == LEX_RANDOM || lextab.table[i - 1].lexema == LEX_CONCAT))
				{
					findParm = true;
					callFunc = true;
				}
				continue;
			}
			case LEX_RIGHTHESIS:
			{
				while (stack.top().lexema != LEX_LEFTHESIS)
				{
					queue.push(stack.top());
					stack.pop();
					if (stack.empty())
						return false;
				}
				if (callFunc)
				{
					if (findComma)
						countParm = countComma + 1;
					else if (findParm)
						countParm = 1;
					else
						countParm = 0;
					countComma = 0;
					lextab.table[i] = func;
					queue.push(lextab.table[i]);
					_itoa_s(countParm, buf, 2, 10);
					stack.top().lexema = buf[0];
					stack.top().idxTi = func.idxTi;
					stack.top().sn = func.sn;
					queue.push(stack.top());
					callFunc = false;
				}
				stack.pop();
				continue;
			}
			}
		}

		while (!stack.empty())
		{
			if (stack.top().lexema == LEX_LEFTHESIS || stack.top().lexema == LEX_RIGHTHESIS || stack.top().lexema == LEX_LEFTBRACKET || stack.top().lexema == LEX_RIGHTBRACKET)
				return false;
			queue.push(stack.top());
			stack.pop();
		}

		while (countLex)
		{
			if (!queue.empty())
			{
				lextab.table[posLex++] = queue.front();
				queue.pop();
			}
			else
			{
				lextab.table[posLex++] = temp;
			}
			countLex--;
		}

		for (int i = 0; i < posLex; i++)
		{
			if (lextab.table[i].lexema == LEX_LITERAL || lextab.table[i].lexema == LEX_ID || lextab.table[i].lexema== LEX_TRUE)
				idtable.table[lextab.table[i].idxTi].idxfirstLE = i;
		}
		return true;
	}

	bool StartPoliz(LT::LexTable& lextab, IT::IdTable& idtable)
	{
		
		bool result = false;
		for (int i = 0; i < lextab.size; i++)
		{
			if (lextab.table[i].lexema == LEX_EQUALS ||
				(lextab.table[i].lexema == LEX_ID && lextab.table[i - 1].lexema != LEX_EQUALS
					&& lextab.table[i - 1].lexema != LEX_PLUS/* && lextab.table[i - 1].lexema != LEX_UNARY_MINUS*/ && lextab.table[i - 1].lexema != LEX_LESS
					&& idtable.table[lextab.table[i].idxTi].idtype == IT::IDTYPE::F
					&& lextab.table[i - 1].lexema != LEX_FUNCTION))
			{
				result = Poliz(i + 1, lextab, idtable);
				if (!result)
					return result;
			}
		}
		return result;
	}

}