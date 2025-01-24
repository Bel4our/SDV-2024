#pragma once
#include "stdafx.h"
#include "Lexer.h"
#include "Error.h"

#define LEX_COUNT_PARM0 '0'
#define LEX_COUNT_PARM1 '1'
#define LEX_COUNT_PARM2 '2'
#define LEX_COUNT_PARM3 '3'

namespace Poliz
{
	bool StartPoliz(LT::LexTable &lextab, IT::IdTable &idtable);
	bool Poliz(int i, LT::LexTable& lextab, IT::IdTable& idtable);
}
