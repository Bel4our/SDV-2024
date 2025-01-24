#pragma once
#define LEXEMA_FIXSIZE 1
#define LT_MAXSIZE 4096
#define LT_TI_NULLIDX 0xffffffff

#define LEX_INTEGER 't'
#define LEX_STRING 't'
#define LEX_CHAR 't'
#define LEX_BOOL 't'
#define LEX_ID 'i'
#define LEX_LITERAL 'l'
#define LEX_FUNCTION 'f'
#define LEX_DECLARE 'd'
#define LEX_RETURN 'r'
#define LEX_WRITE 'p'
#define LEX_WRITELN 'w'
#define LEX_SEMICOLON ';'
#define LEX_COMMA ','
#define LEX_LEFTBRACE '{'
#define LEX_BRACELET '}'
#define LEX_LEFTHESIS '('
#define LEX_RIGHTHESIS ')'
#define LEX_LEFTBRACKET		'['						
#define LEX_RIGHTBRACKET	']'		
#define LEX_PLUS 'v'
#define LEX_MINUS 'v'
#define LEX_UNARY_MINUS '-'
#define LEX_STAR 'v'
#define LEX_PERC 'v'
#define LEX_DIRSLASH 'v'
#define LEX_CHECK_EQ 's'
#define LEX_CHECK_UNEQ 's'
#define LEX_CHECK 'c'
#define LEX_TRUE 'y'
#define LEX_FALSE 'y'
#define LEX_LESS 's'
#define LEX_MORE 's'

#define LEX_LESS_OR_EQ 's'
#define LEX_MORE_OR_EQ 's'
#define LEX_MAIN 'm'
#define LEX_OTHERWISE '!'		

#define LEX_EQUALS '='

#define LEX_LENLINE '/'
#define LEX_CONCAT '+'
#define LEX_RANDOM	'z'

namespace LT
{
    enum class OPERATION { NOT = -1, PLUS = 1, MINUS, MUL, DIV, REST, MORE, LESS, EQU, NOEQU, MOREQ, LESSQ, UNMIN };





    struct Entry
    {
        char lexema;
        int sn;
        int idxTi = LT_TI_NULLIDX;
        int priority;
        OPERATION oper = OPERATION::NOT;
    };

    struct LexTable
    {
        int maxsize;
        int size;
        Entry* table;
    };





    LexTable Create(int size);
    void Add(LexTable& lextable, Entry entry);
    Entry GetEntry(LexTable& lextable, int n);
    void Delete(LexTable& lextable);

    void WritePoliz(LexTable lextable);
    void WriteLexTable(LexTable& lextable);
};
