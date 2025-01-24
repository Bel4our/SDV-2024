@echo off
call ..\Debug\SE_CourseProject.exe -in:..\SE_CourseProject\in.txt -out:program.asm
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
ml /c program.asm
link /OPT:NOREF /OUT:program.exe /SUBSYSTEM:CONSOLE libucrt.lib program.obj
call program.exe
pause