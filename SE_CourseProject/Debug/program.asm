.586P
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib ..\Debug\Library.lib
ExitProcess PROTO :DWORD

EXTRN CONCAT: proc
EXTRN LENLINE: proc
EXTRN RANDOM: proc
EXTRN OutNumber: proc
EXTRN OutLine: proc
EXTRN OutBool: proc
EXTRN OutSymb: proc
EXTRN OutNumberLn: proc
EXTRN OutLineLn: proc
EXTRN OutBoolLn: proc
EXTRN OutSymbLn: proc

.stack 4096

.const
	null_division BYTE "Exception: деление на ноль", 0
	Lit1	BYTE "с", 0
	Lit2	SWORD 5
	Lit3	SWORD 0
	Lit4	BYTE "2", 0
	Lit5	SWORD -52
	Lit6	SWORD 1
	Lit7	SWORD -45
	Lit8	SWORD 5
	Lit9	SWORD 1
	Lit10	SWORD 0
	Lit11	SWORD 3
	Lit12	SWORD 6
	Lit13	SWORD 25
	Lit14	SWORD 1
	Lit15	BYTE "флаги не равны", 0
	Lit16	BYTE "флаги равны", 0
	Lit17	BYTE " ", 0
	Lit18	BYTE "word", 0
	Lit19	SWORD 1
	Lit20	SWORD 1
	Lit21	SWORD 2
	Lit22	SWORD -5
	Lit23	SWORD -3
	Lit24	SWORD 28
	Lit25	SWORD 6
	Lit26	SWORD 0

.data
	buffer BYTE 256 dup(0)
	sum_sumbol	SWORD 0
	sum_x	SWORD 0
	sum_flagok	SWORD 0
	asdf_sumbol	SWORD 0
	asdf_x	SWORD 0
	asdf_assfaf	SWORD 0
	main_first	SWORD 0
	main_second	SWORD 0
	main_s	SWORD 0
	main_flag1	SWORD 0
	main_flag2	SWORD 0
	main_num	SWORD 0
	main_word1	DWORD ?
	main_word2	DWORD ?
	main_word3	DWORD ?
	main_length1	SWORD 0
	main_length2	SWORD 0
	main_res	SWORD 0
	main_as	SWORD 0
	main_zxc	SWORD 0
	main_jopa	SWORD 0
	main_pisun	SWORD 0
	main_kav	SWORD 0

.code

sum PROC sum_a : SWORD, sum_b : SWORD
	push offset Lit1
	pop sum_sumbol
	push Lit2
	pop sum_x
	movsx eax, sum_flagok
	push eax
	call OutBoolLn
	push sum_a
	push sum_b
	pop ax
	pop bx
	add ax, bx
	push ax
	pop sum_x
	movsx eax, sum_x
	push eax
	call OutNumberLn
	push 0
	jmp local0
local0:
	pop eax
	ret
sum ENDP

asdf PROC asdf_a : SWORD, asdf_b : SWORD
	push offset Lit4
	pop asdf_sumbol
	push Lit5
	pop asdf_assfaf
	push asdf_a
	push asdf_b
	pop ax
	pop bx
	add ax, bx
	push ax
	pop asdf_x
	push asdf_sumbol
	call OutSymbLn
	push asdf_sumbol
	jmp local1
local1:
	pop eax
	ret
asdf ENDP

qwerty PROC 
	push 1
	jmp local2
local2:
	pop eax
	ret
qwerty ENDP

program PROC
	push Lit7
	pop main_first
	movsx eax, main_first
	push eax
	call OutNumberLn
	push Lit8
	pop main_second
	push main_first
	push main_second
	pop dx
	pop dx
	movsx eax, main_second
	push eax
	movsx eax, main_first
	push eax
	call sum
	push eax
	pop main_s
	push Lit9
	pop ax
	cmp ax, 0
	je l0
	jne l1
l0:
	mov ax, 0
	push ax
	jmp endofexpr0
l1:
	mov ax, 1
	push ax

endofexpr0:
	pop main_flag1
	push Lit10
	pop ax
	cmp ax, 0
	je l2
	jne l3
l2:
	mov ax, 0
	push ax
	jmp endofexpr1
l3:
	mov ax, 1
	push ax

endofexpr1:
	pop main_flag2
	push Lit11
	push Lit12
	pop dx
	pop dx
	movsx eax, Lit12
	push eax
	movsx eax, Lit11
	push eax
	call RANDOM
	push ax
	push Lit13
	pop ax
	pop bx
	add ax, bx
	push ax
	push Lit14
	pop ax
	pop bx
	add ax, bx
	push ax
	pop main_num
	movsx eax, main_num
	push eax
	call OutNumberLn
	mov ax, main_flag1
	cmp ax, main_flag2
	jne m0
	je m1
m0:
	push offset Lit15
	call OutLineLn
	jmp e0
m1:
	push offset Lit16
	call OutLineLn
e0:
	push offset Lit17
	pop main_word1
	push offset Lit18
	pop main_word2
	push main_word1
	push main_word2
	pop dx
	pop dx
	push main_word2
	push main_word1
	push offset buffer
	call CONCAT
	push eax
	pop main_word3
	mov ax, Lit19
	cmp ax, 0
	jnz m2
	jz m3
m2:
	push main_word3
	call OutLineLn
m3:
	push main_word1
	pop dx
	push main_word1
	call LENLINE
	push ax
	pop main_length1
	push main_word2
	pop dx
	push main_word2
	call LENLINE
	push ax
	pop main_length2
	pop ax
	cmp ax, 0
	je l4
	jne l5
l4:
	mov ax, 0
	push ax
	jmp endofexpr2
l5:
	mov ax, 1
	push ax

endofexpr2:
	pop main_res
	movsx eax, main_res
	push eax
	call OutBoolLn
	push Lit20
	push Lit21
	pop dx
	pop dx
	movsx eax, Lit21
	push eax
	movsx eax, Lit20
	push eax
	call asdf
	push eax
	pop main_as
	push Lit22
	pop main_zxc
	push main_zxc
	pop main_jopa
	push main_jopa
	pop ax
	neg ax
	push ax
	pop main_pisun
	movsx eax, main_pisun
	push eax
	call OutNumberLn
	push main_zxc
	pop ax
	neg ax
	push ax
	push Lit23
	push Lit24
	pop bx
	pop ax
	sub ax, bx
	push ax
	push Lit25
	pop ax
	pop bx
	mul bx
	push ax
	pop ax
	pop bx
	add ax, bx
	push ax
	pop main_kav
	movsx eax, main_kav
	push eax
	call OutNumberLn
	mov ax, main_length1
	cmp ax, main_length2
	jg m4
	jl m5
m4:
	push main_word1
	call OutLine
m5:
	push 0
		jmp theend
theend:
	push 0
	call ExitProcess
nulldiv:
	push offset null_division
	call OutLineLn
	push -1
	call ExitProcess
program ENDP
end program