includelib legacy_stdio_definitions.lib

.data
extern presentAddr : qword
extern jmpBackAddr : qword

; This performs instructions originally performed by dxgi.dll in the
; memory that we've replaced, and then returns

.code
	FixAndReturn PROC 
		call [presentAddr]
		mov [rsp+10h],rbx
		mov [rsp+20h],rsi
		push rbp
		push rdi
		push r14
		jmp qword ptr [jmpBackAddr]
	FixAndReturn ENDP
end
