#include "stdafx.h"
#include "Log.h"
#include <iostream>
#include <fstream>
#include "Error.h"
#include <cstdarg>
#include <ctime>

using namespace Log;
using namespace std;
using namespace Parm;
using namespace In;

LOG Log::getlog(wchar_t logfile[])
{
   
		LOG log;
		log.stream = new ofstream;
		log.stream->open(logfile);
		if (log.stream->fail())
             ERROR_THROW(103);
		wcscpy_s(log.logfile, logfile);
		return log;
}

void Log::WriteLine(LOG log, char* c, ...)
{
    char** p = &c;
    int j = 0;
    while (p[j] != "") {
        *log.stream << p[j];
        j++;
    }
}


void Log::WriteLine(LOG log, wchar_t* c, ...)
{
    wchar_t** p = &c;
    char buf[50];
    int j = 0;
    while (p[j] != L"") {
        wcstombs(buf, p[j++], 50);
        *log.stream << buf;
    }
}

void Log::WriteLog(LOG log)
{
    time_t rantime;
    struct tm timeinfo;
    char buffer[80];
    time(&rantime);
    localtime_s(&timeinfo, &rantime);

    strftime(buffer, sizeof(buffer), "%d.%m.%Y %I:%M:%S", &timeinfo);
    (*log.stream) << "----------------ПРОТОКОЛ----------------" << endl << "---------" << buffer << "-----------" << endl;
}

void Log::WriteParm(LOG log, PARM parm)
{
    char in_text[PARM_MAX_SIZE];
    char out_text[PARM_MAX_SIZE];
    char log_text[PARM_MAX_SIZE];
    wcstombs(in_text, parm.in, PARM_MAX_SIZE);
    wcstombs(out_text, parm.out, PARM_MAX_SIZE);
    wcstombs(log_text, parm.log, PARM_MAX_SIZE);

    *log.stream << "\t\tПАРАМЕТРЫ" << endl;
    *log.stream << setfill('-') << setw(100) << '-' << endl;
    *log.stream << "-in:\t" << in_text << "\n-out:\t" << out_text << "\n-log:\t" << log_text << endl;
    *log.stream << "Дополнительно: " << (parm.idtable ? "-id  " : "") << (parm.lextable ? "-lt  " : "")
        << (parm.poliz ? "-poliz  " : "") << endl;
    *log.stream << setfill('-') << setw(100) << '-' << endl;

    cout << "\t\tПАРАМЕТРЫ" << endl;
    cout << setfill('-') << setw(100) << '-' << endl;
    cout << "-in:\t" << in_text << "\n-out:\t" << out_text << "\n-log:\t" << log_text << endl;
    cout << "Дополнительно: " << (parm.idtable ? "-id  " : "") << (parm.lextable ? "-lt  " : "")
        << (parm.poliz ? "-poliz  " : "") << endl;
    cout << setfill('-') << setw(100) << '-' << endl;
}

void Log::WriteIn(LOG log, IN in)
{
    *log.stream << "\t\tИСХОДНЫЕ ДАННЫЕ" << endl;
    *log.stream << setfill('-') << setw(100) << '-' << endl;
    *log.stream << "Количество символов:\t" << in.size << endl;
    *log.stream << "Проигнорировано:\t" << in.ignor << endl;
    *log.stream << "Количество строк:\t" << in.lines + 1 << endl;
    *log.stream << setfill('=') << setw(100) << '=' << endl;

    cout << "\t\tИСХОДНЫЕ ДАННЫЕ" << endl;
    cout << setfill('-') << setw(100) << '-' << endl;
    cout << "Количество символов:\t" << in.size << endl;
    cout << "Проигнорировано:\t" << in.ignor << endl;
    cout << "Количество строк:\t" << in.lines + 1 << endl;
    cout << setfill('=') << setw(100) << '=' << endl;
}

void Log::WriteError(LOG log, Error::ERROR er)
{
    cout << endl;
    if (log.stream) { 
        *log.stream << "Ошибка " << er.id << ": " << er.message;
        if (er.inext.line != -1) { 
            *log.stream << ", строка " << er.inext.line << ", позиция " << er.inext.col << '\n';
        }
        else {
            *log.stream << '\n';
        }
    }
    else { 
        cout << "Ошибка " << er.id << ": " << er.message << '\n';
        cout << "Строка " << er.inext.line << ", позиция " << er.inext.col << '\n';
    }

}

void Log::WriteLexTableLog(LOG log, LT::LexTable& lextable)
{
    int i;
    *log.stream << "\t\tТАБЛИЦА ЛЕКСЕМ" << endl;
    *log.stream << setfill('=') << setw(100) << '=' << endl;
    *log.stream << "   №" << " | " << "Лексема" << setfill(' ') << setw(20) << ' ' << left << " | " << "Строка" << setw(21) << ' ' << left << " | " << "Индекс в ТИ" << endl;
    *log.stream << setfill('-') << setw(100) << '-' << endl;
    for (i = 0; i < lextable.size; i++)
    {
        *log.stream << setfill('0') << setw(4) << right << i << " | " << setfill(' ') << setw(24) << left << lextable.table[i].lexema << "    | " << setw(24) << left << lextable.table[i].sn<< "    | ";
        if (lextable.table[i].idxTi == LT_TI_NULLIDX)
            *log.stream << "-" << endl;
        else
            *log.stream << setw(23) << lextable.table[i].idxTi << endl;
    }
    *log.stream << setfill('-') << setw(90) << '-' << endl;
    *log.stream << "\tВсего лексем: " << i << endl;
    *log.stream << setfill('=') << setw(90) << '=' << endl;
}

void Log::WriteIdTableLog(LOG log, IT::IdTable& idtable)
{
    int i, numberNoneId = 0;
    *log.stream << "\t\tТАБЛИЦА ИДЕНТИФИКАТОРОВ" << endl;
    *log.stream << setfill('=') << setw(100) << '=' << endl;
    *log.stream << "   №" << " | " << "    Идентификатор    " << " | " << "Тип данных" << " | " << "Тип идентификатора" << " | " << "Индекс в ТЛ" << " | " << "Значение/Параметры" << endl;
    *log.stream << setfill('-') << setw(100) << '-' << endl;
    for (i = 0; i < idtable.size; i++)
    {
        *log.stream << setfill('0') << setw(4) << right << i << " | ";
        if (idtable.table[i].func != "idOfFunc" && idtable.table[i].idtype != IT::L && idtable.table[i].idtype != IT::S)
            *log.stream << setfill(' ') << setw(21) << left << idtable.table[i].func +  "_" + idtable.table[i].id << " | ";
        else
            *log.stream << setfill(' ') << setw(21) << left << idtable.table[i].id << " | ";

        switch (idtable.table[i].iddatatype)
        {
        case IT::IDDATATYPE::NUMB:
            *log.stream << setw(10) << left;
            *log.stream << "number" << " | ";
            break;
        case IT::IDDATATYPE::LINE:
            *log.stream << setw(10) << left;
            *log.stream << "line" << " | ";
            break;
        case IT::IDDATATYPE::SYMB:
            *log.stream << setw(10) << left;
            *log.stream << "symb" << " | ";
            break;
        case IT::IDDATATYPE::BOOL:
            *log.stream << setw(10) << left;
            *log.stream << "bool" << " | ";
            break;
        }
        switch (idtable.table[i].idtype)
        {
        case IT::IDTYPE::V:
            *log.stream << setw(18) << left << "переменная" << " | ";
            break;
        case IT::IDTYPE::F:
            *log.stream << setw(18) << left << "функция" << " | ";
            break;
        case IT::IDTYPE::P:
            *log.stream << setw(18) << left << "параметр" << " | ";
            break;
        case IT::IDTYPE::L:
            *log.stream << setw(18) << left << "литерал" << " | ";
            numberNoneId++;
            break;
        case IT::IDTYPE::S: *log.stream << setw(18) << left << "функция ст. библ." << " | ";
            numberNoneId++;
            break;
        }
        *log.stream << setw(11) << left << idtable.table[i].idxfirstLE << " | ";

        if (idtable.table[i].iddatatype == IT::IDDATATYPE::NUMB && (idtable.table[i].idtype == IT::IDTYPE::V || idtable.table[i].idtype == IT::IDTYPE::L))
            *log.stream << setw(18) << left << idtable.table[i].value.vint;
        else if (idtable.table[i].iddatatype == IT::IDDATATYPE::BOOL && (idtable.table[i].idtype == IT::IDTYPE::V || idtable.table[i].idtype == IT::IDTYPE::L))
        {
            if (idtable.table[i].value.vint == 0)
                *log.stream << setw(18) << left << "false";
            else
                *log.stream << setw(18) << left << "true";
        }
        else if (idtable.table[i].iddatatype == IT::IDDATATYPE::LINE && (idtable.table[i].idtype == IT::IDTYPE::V || idtable.table[i].idtype == IT::IDTYPE::L))
            if (idtable.table[i].value.vstr->len == 0)
              *log.stream << "[" << idtable.table[i].value.vstr->len << "]" << idtable.table[i].value.vstr->str;
            else
              *log.stream << "[" << idtable.table[i].value.vstr->len-2 << "]" << idtable.table[i].value.vstr->str;
        else if (idtable.table[i].iddatatype == IT::IDDATATYPE::SYMB && (idtable.table[i].idtype == IT::IDTYPE::V || idtable.table[i].idtype == IT::IDTYPE::L))
            *log.stream << "\'" << idtable.table[i].value.vchar <<"\'";
        else if (idtable.table[i].idtype == IT::IDTYPE::F || idtable.table[i].idtype == IT::IDTYPE::S)
        {
            for (int k = 0; k < idtable.table[i].value.params.count; k++)
            {
                *log.stream << left << "P" << k << ":";
                switch (idtable.table[i].value.params.types[k])
                {
                case IT::IDDATATYPE::NUMB:
                    *log.stream << left << "N|"; break;
                case IT::IDDATATYPE::LINE:
                    *log.stream << left << "L|"; break;
                case IT::IDDATATYPE::BOOL:
                    *log.stream << left << "B|"; break;
                case IT::IDDATATYPE::SYMB:
                    *log.stream << left << "S|"; break;
                }
            }
            if (idtable.table[i].value.params.count == NULL)
                *log.stream << "-";
        }
        else
            *log.stream << "-";
        *log.stream << endl;
    }
    *log.stream << setfill('-') << setw(100) << '-' << endl;
    *log.stream << "\tВсего идентификаторов: " << i - numberNoneId << endl;
    *log.stream << setfill('=') << setw(100) << '=' << endl;
}

void Log::WritePolizLog(LOG log, LT::LexTable& lextable)
{

    int numberOfLine = 1;
    int number = 0;
    *log.stream << "\t\tПРОМЕЖУТОЧНОЕ ПРЕДСТАВЛЕНИЕ КОДА" << endl;
    *log.stream << setfill('=') << setw(90) << '=';
    for (int i = 0; i < lextable.size; i++)
    {
        if (lextable.table[i].sn != number)
        {
            while (lextable.table[i].sn - number > 1)
                number++;
            *log.stream << endl << setfill('0') << setw(4) << right << numberOfLine << "\t";
            number++;
            numberOfLine++;
        }
        *log.stream << lextable.table[i].lexema;
        if (lextable.table[i].lexema == LEX_ID || lextable.table[i].lexema == LEX_LITERAL)
            *log.stream << "(" << lextable.table[i].idxTi << ")";
    }
    *log.stream << endl;
    *log.stream << setfill('=') << setw(90) << '=' << endl;
}




void Log::Close(LOG log)
{
    if (log.stream->is_open())
    {
        log.stream->close();
        delete log.stream;
    }
}

