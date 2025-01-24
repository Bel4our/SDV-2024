#pragma once
#define ID_MAXSIZE		10
#define TI_MAXSIZE		4096
#define TI_INT_DEFAULT	0x00000001
#define TI_STR_DEFAULT	0x00
#define TI_CHAR_DEFAULT	0x00
#define TI_BOOL_DEFAULT	0x00000000
#define TI_NULLIDX		0xffffffff
#define TI_STR_MAXSIZE  257
#define TI_CHAR_MAXSIZE 3
#define CONCAT_PARAMS_CNT		2
#define LINELEN_PARAMS_CNT		1
#define RANDOM_PARAMS_CNT		2
#define MAX_PARAMS_COUNT		3


#define CONCAT_TYPE IT::IDDATATYPE::LINE
#define LINELEN_TYPE IT::IDDATATYPE::NUMB
#define RANDOM_TYPE IT::IDDATATYPE::NUMB

namespace IT
{
	enum IDDATATYPE { NUMB = 1, LINE = 2, SYMB = 3, BOOL = 4, UNKNOWN };
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, S = 5 };

	static const IDDATATYPE CONCAT_PARAMS[] = { IT::IDDATATYPE::LINE, IT::IDDATATYPE::LINE };
	static const IDDATATYPE LINELEN_PARAMS[] = { IT::IDDATATYPE::LINE };
	static const IDDATATYPE RANDOM_PARAMS[] = { IT::IDDATATYPE::NUMB, IT::IDDATATYPE::NUMB };

	struct Entry
	{
		int		idxfirstLE;
		std::string	id;
		IDDATATYPE iddatatype = IDDATATYPE::UNKNOWN;
		IDTYPE	idtype;
		std::string func;


		union
		{
			int vint;
			char vchar;
			struct
			{
				int len;
				char str[TI_STR_MAXSIZE - 1];
			} vstr[TI_STR_MAXSIZE];

			struct
			{
				int count;
				IDDATATYPE* types;
			} params;

		} value;
	};

	struct IdTable
	{
		int maxsize;
		int size;
		Entry* table;
	};


	IdTable Create(
		int size
	);

	void Add(
		IdTable& idtable,
		Entry entry
	);

	Entry GetEntry(
		IdTable& idtable,
		int n
	);

	int IsId(
		IdTable& idtable,
		std::string id
	);

	void Delete(IdTable& idtable);

	void WriteTable(IdTable& idtable);
};