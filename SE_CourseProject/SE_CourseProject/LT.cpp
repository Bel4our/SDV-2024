#include "stdafx.h"
#include "LT.h"

namespace LT
{
    LexTable Create(int size)
    {
        LexTable lextable;
        if (size > 0)
        {
            lextable.maxsize = size;
            lextable.size = 0;
            lextable.table = new Entry[size];
            return lextable;
        }
        ERROR_THROW(201);
    }

    void Add(LexTable& lextable, Entry entry)
    {
        if (lextable.size < lextable.maxsize) {
            lextable.table[lextable.size] = entry;
            lextable.size++;
        }
        else {
            ERROR_THROW(202);
        }
    }

    Entry GetEntry(LexTable& lextable, int n)
    {
        if (n >= 0 && n < lextable.size) {
            return lextable.table[n];
        }
        else {
            ERROR_THROW(203);
        }
    }


    void WritePoliz(LexTable lextable)
    {
        int numberOfLine = 1;
        int number = 0;
        cout << "\t\tÏÐÎÌÅÆÓÒÎ×ÍÎÅ ÏÐÅÄÑÒÀÂËÅÍÈÅ ÊÎÄÀ";
        for (int i = 0; i < lextable.size; i++)
        {
            if (lextable.table[i].sn != number && lextable.table[i].sn != 0)
            {
                while (lextable.table[i].sn - number > 1)
                    number++;

                cout << endl << setfill('0') << setw(4) << right << numberOfLine << "\t";
                number++;
                numberOfLine++;
            }
            cout << lextable.table[i].lexema;
            if (lextable.table[i].lexema == LEX_ID || lextable.table[i].lexema == LEX_LITERAL || lextable.table[i].lexema == LEX_TRUE)
                cout << "(" << lextable.table[i].idxTi << ")";
        }
        cout << endl;
        cout << setfill('=') << setw(98) << '=' << endl;
    }

    void WriteLexTable(LT::LexTable& lextable)
    {
        int i;
        cout << "\t\tÒÀÁËÈÖÀ ËÅÊÑÅÌ" << endl;
        cout << setfill('=') << setw(100) << '=' << endl;
        cout << "   ¹" << " | " << "Ëåêñåìà" << setfill(' ') << setw(20) << ' ' << left << " | " << "Ñòðîêà" << setw(21) << ' ' << left << " | " << "Èíäåêñ â ÒÈ" << endl;
        cout << setfill('-') << setw(100) << '-' << endl;
        for (i = 0; i < lextable.size; i++)
        {
            cout << setfill('0') << setw(4) << right << i << " | " << setfill(' ') << setw(24) << left << lextable.table[i].lexema << "    | " << setw(24) << left
                << lextable.table[i].sn + 1 << "    | ";
            if (lextable.table[i].idxTi == LT_TI_NULLIDX)
                cout << "-" << endl;
            else
                cout << setw(23) << lextable.table[i].idxTi << endl;
        }
        cout << setfill('-') << setw(100) << '-' << endl;
        cout << "\tÂñåãî ëåêñåì: " << i << endl;
        cout << setfill('=') << setw(100) << '=' << endl;
    }

    void Delete(LexTable& lextable)
    {
        delete[] lextable.table;
        lextable.maxsize = 0;
        lextable.size = 0;
    }
}
