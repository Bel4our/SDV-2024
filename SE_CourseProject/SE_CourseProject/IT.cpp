#include "stdafx.h"
#include "IT.h"

namespace IT
{
    IdTable Create(int size)
    {
        if (size > 0 && size < TI_MAXSIZE)
        {
            IdTable idtable;
            idtable.maxsize = size;
            idtable.size = 0;
            idtable.table = new Entry[size];
            return idtable;
        }
        ERROR_THROW(204);
    }

    void Add(IdTable& idtable, Entry entry)
    {
        if (entry.id.size() > ID_MAXSIZE)
        {
            ERROR_THROW(208);
        }
        if (idtable.size < idtable.maxsize) {
            idtable.table[idtable.size] = entry;
            idtable.size++;
        }
        else {
            ERROR_THROW(205);
        }
    }

    Entry GetEntry(IdTable& idtable, int n)
    {
        if (n >= 0 && n < idtable.size) {
            return idtable.table[n];
        }
        else {
            ERROR_THROW(206);
        }
    }

    int IsId(IdTable& idtable, std::string id)
    {
        for (int i = 0; i < idtable.size; i++)
        {
            if (idtable.table[i].id == id)
                return i;
        }
        return TI_NULLIDX;
    }

    void Delete(IdTable& idtable)
    {
        delete[] idtable.table;
        idtable.size = 0;
        idtable.maxsize = 0;
    }
	void WriteTable(IdTable& idtable)
	{
		int i, numberNoneId = 0;
		cout << "\t\tТАБЛИЦА ИДЕНТИФИКАТОРОВ" << endl;
		cout << setfill('=') << setw(100) << '=' << endl;
		cout << "   №" << " | " << "    Идентификатор    " << " | " << "Тип данных" << " | " << "Тип идентификатора" << " | " << "Индекс в ТЛ" << " | " << "Значение/Параметры" << endl;
		cout << setfill('-') << setw(100) << '-' << endl;
		for (i = 0; i < idtable.size; i++)
		{
			cout << setfill('0') << setw(4) << right << i << " | ";
			cout << setfill(' ') << setw(21) << left << idtable.table[i].id << " | ";

			switch (idtable.table[i].iddatatype)
			{
			case IT::IDDATATYPE::NUMB:
				cout << setw(10) << left;
				cout << "number" << " | ";
				break;
			case IT::IDDATATYPE::LINE:
				cout << setw(10) << left;
				cout << "line" << " | ";
				break;
			case IT::IDDATATYPE::SYMB:
				cout << setw(10) << left;
				cout << "symb" << " | ";
				break;
			case IT::IDDATATYPE::BOOL:
				cout << std::setw(10) << left;
				cout << "bool" << " | ";
				break;
			}

			switch (idtable.table[i].idtype)
			{
			case IT::IDTYPE::V:
				cout << setw(18) << left << "переменная" << " | ";
				break;
			case IT::IDTYPE::F:
				cout << setw(18) << left << "функция" << " | ";
				break;
			case IT::IDTYPE::P:
				cout << setw(18) << left << "параметр" << " | ";
				break;
			case IT::IDTYPE::L:
				cout << setw(18) << left << "литерал" << " | ";
				numberNoneId++;
				break;
			case IT::IDTYPE::S:
				cout << setw(18) << left << "функция ст. библ." << " | ";
				numberNoneId++;
				break;
			default:
				cout << setw(18) << left << "неизвестен" << " | "; break;
			}

			cout << setw(11) << left << idtable.table[i].idxfirstLE << " | ";

			if (idtable.table[i].iddatatype == IT::IDDATATYPE::NUMB && (idtable.table[i].idtype == IT::IDTYPE::V || idtable.table[i].idtype == IT::IDTYPE::L))
				cout << setw(18) << left << idtable.table[i].value.vint;
			else if (idtable.table[i].iddatatype == IT::IDDATATYPE::BOOL && (idtable.table[i].idtype == IT::IDTYPE::V || idtable.table[i].idtype == IT::IDTYPE::L))
			{
				if (idtable.table[i].value.vint == 0)
					cout << setw(18) << left << "false";
				else
					cout << setw(18) << left << "true";
			}
			else if (idtable.table[i].iddatatype == IT::IDDATATYPE::LINE && (idtable.table[i].idtype == IT::IDTYPE::V || idtable.table[i].idtype == IT::IDTYPE::L))
				  if (idtable.table[i].value.vstr->len == 0)
					  cout << "[" << idtable.table[i].value.vstr->len << "]" << idtable.table[i].value.vstr->str;
				  else
					cout << "[" << idtable.table[i].value.vstr->len-2 << "]" << idtable.table[i].value.vstr->str;
			else if (idtable.table[i].iddatatype == IT::IDDATATYPE::SYMB && (idtable.table[i].idtype == IT::IDTYPE::V || idtable.table[i].idtype == IT::IDTYPE::L))
					cout << "\'" << idtable.table[i].value.vchar << "\'";
			else if (idtable.table[i].idtype == IT::IDTYPE::F || idtable.table[i].idtype == IT::IDTYPE::S)
			{
				for (int k = 0; k < idtable.table[i].value.params.count; k++)
				{
					cout << left << "P" << k << ":";
					switch (idtable.table[i].value.params.types[k])
					{
					case IT::IDDATATYPE::NUMB:
						cout << left << "N|"; break;
					case IT::IDDATATYPE::LINE:
						cout << left << "L|"; break;
					case IT::IDDATATYPE::BOOL:
						cout << left << "B|"; break;
					case IT::IDDATATYPE::SYMB:
						cout << left << "S|"; break;
					}
				}
				if (idtable.table[i].value.params.count == NULL)
					cout << "-";
			}
			else
				cout << "-";
			cout << endl;
		}
		cout << setfill('-') << setw(100) << '-' << endl;
		cout << "\tВсего идентификаторов: " << i - numberNoneId << endl;
		cout << setfill('=') << setw(100) << '=' << endl;
	}
}
