includelib legacy_stdio_definitions.lib

.data
extern presentAddr : qword
extern jmpBackAddr : qword

; This performs instructions originally performed by dxgi.dll in the
; memory that we've replaced, and then returns

.code
	FixAndJmpBack PROC 

		;push rsp
		push rbp
		mov rbp, rsp
		;sub rsp, 20h

		push rax
		;push rbx
		push rcx
		push rdx
		;push rdi
		;push rsi
		push r8
		push r9
		push r10
		push r11
		;push r12
		;push r13
		;push r14
		;push r15

		call [presentAddr]

		;pop r15
		;pop r14
		;pop r13
		;pop r12
		pop r11
		pop r10
		pop r9
		pop r8
		;pop rsi
		;pop rdi
		pop rdx
		pop rcx
		;pop rbx
		pop rax

		pop rbp
		;pop rsp

		; Previously overwritten instructions
		mov [rsp+10h],rbx
		mov [rsp+20h],rsi
		push rbp
		push rdi
		push r14
		jmp qword ptr [jmpBackAddr]
	FixAndJmpBack ENDP
end

