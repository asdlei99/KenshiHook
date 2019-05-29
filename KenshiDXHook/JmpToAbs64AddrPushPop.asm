includelib legacy_stdio_definitions.lib

.data
extern absAddr : qword

.code
	JmpToAbs64AddrPushPop PROC
		push rax
		mov rax, [absAddr]
		jmp rax
		pop rax
		ret
	JmpToAbs64AddrPushPop ENDP
end
