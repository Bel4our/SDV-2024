#pragma once
#pragma comment(linker, "/STACK:33554432")
#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "In.h"
#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "Out.h"
#include "GRB.h"
#include "MFST.h"
#include "FST.h"
#include "IT.h"
#include "LT.h"
#include "Lexer.h"
#include "Polish.h"

#include <iostream>
#include <typeinfo>
#include <string>
#include <locale>
#include <stdlib.h> 
#include <cwchar>
#include <iomanip>
#include <stdio.h>
#include <tchar.h>
#include <fstream>
#include <ctime>
#include <stack>
#include <queue>

#include <string.h>
using namespace std;