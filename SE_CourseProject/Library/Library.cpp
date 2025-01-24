#include <iostream>


extern "C"
{
	int RANDOM(int min, int max)
	{
		srand(time(0));
		if (min > max)
		{
			int temp = min;
			min = max;
			max = temp;
		}
		if ((max - min) == 0)
		{
			return max;
		}
		return rand() % (max - min) + min;
	}

	int LENLINE(char* str)
	{
		if (str == nullptr)
			return 0;

		int length = 0;
		for (int i = 0; str[i] != '\0'; i++)
		{
			if (i == 255)
				break;
			length++;
		}
		return length;
	}

	char* CONCAT(char* buffer, char* str1, char* str2)
	{

		if (str1 == NULL)
			str1 = (char*)"";

		if (str2 == NULL)
			str2 = (char*)"";

		int i = 0;

		for (int j = 0; str1[j] != '\0'; j++)
		{
			if (i == 255)
				break;
			buffer[i++] = str1[j];
		}

		for (int j = 0; str2[j] != '\0'; j++)
		{
			if (i == 255)
				break;
			buffer[i++] = str2[j];
		}

		buffer[i] = '\0';

		return buffer;
	}


	void WriteSymb(char *c)
	{
		setlocale(LC_ALL, "rus");
		if (c == nullptr)
		{
			std::cout << " ";
		}
		else
			std::cout << c[0];
	}

	void WriteSymbLn(char *c)
	{

		setlocale(LC_ALL, "rus");
		if (c == nullptr)
		{
			std::cout << std::endl;;
		}
		else
			std::cout << c[0] << std::endl;
	}

	void WriteNumbLn(int a)
	{
		std::cout << a << std::endl;
	}

	void WriteLineLn(char* ptr)
	{
		setlocale(LC_ALL, "rus");
		if (ptr == nullptr)
		{
			std::cout << std::endl;
			return;
		}
		for (int i = 0; ptr[i] != '\0'; i++)
		{
			std::cout << ptr[i];
		}
		std::cout << std::endl;
	}
	void WriteBoolLn(int b)
	{
		if (b)
			std::cout << "true" << std::endl;
		else
			std::cout << "false" << std::endl;
	}

	void WriteNumb(int a)
	{
		std::cout << a;
	}

	void WriteLine(char* ptr)
	{
		setlocale(LC_ALL, "rus");
		setlocale(LC_ALL, "Rus");
		if (ptr == nullptr)
		{
			std::cout << " ";
			return;
		}
		for (int i = 0; ptr[i] != '\0'; i++)
		{
			std::cout << ptr[i];
		}
	}
	void WriteBool(int b)
	{
		if (b)
			std::cout << "true";
		else
			std::cout << "false";
	}
	
}