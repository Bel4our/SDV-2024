
#include "Error.h";
#include "In.h";
#include <fstream>
#include <iostream>
#include <string>

using namespace In;
using namespace std;

IN In::getin(wchar_t infile[])
{
    IN in;
    wifstream file(infile);
    if (!file.is_open())
    {
        ERROR_THROW(102);
    }

    in.size = 0;
    in.lines = 1;
    in.ignor = 0;
    in.text = new unsigned char[IN_MAX_LEN_TEXT];
    char prev = NULL;

    const int code_table[] = IN_CODE_TABLE;
    for (int i = 0; i < 256; i++)
    {
        in.code[i] = code_table[i];
    }

    wchar_t buf;
    int index = 0;

    while (file.get(buf) && index < IN_MAX_LEN_TEXT - 1)
    {
        if (buf == '\n')
        {
            in.lines++;
            in.text[index] = static_cast<unsigned char>(buf);
            index++;
        }
        else if (in.code[static_cast<unsigned int>(buf)] == IN::F)
        {
            ERROR_THROW_IN(200, in.lines, index);
        }
        else if (in.code[static_cast<unsigned int>(buf)] == IN::I)
        {
            in.ignor++;
        }
        else if (in.code[static_cast<unsigned int>(buf)] == IN::T)
        {
            in.text[index++] = static_cast<unsigned char>(buf);

        }
    }

    in.text[index++] = '\0';
    in.size = index;

    file.close();
   
    return in;
}