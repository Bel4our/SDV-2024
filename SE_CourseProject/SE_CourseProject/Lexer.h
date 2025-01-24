#pragma once
#include <vector>   
#include "LT.h"
#include "IT.h"

namespace Lexer
{
    void Run(LT::LexTable& lextable, IT::IdTable& idtable, In::IN in);
}