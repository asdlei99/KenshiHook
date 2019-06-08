includelib legacy_stdio_definitions.lib

.data
extern presentAddr : qword
extern jmpBackAddr : qword

; This jmps to our hook, then performs instructions 
; originally performed by dxgi.dll in the memory that 
; we've replaced, and then jmps back

.code
	JmpToHookAndJmpBack PROC 
		jmp qword ptr [presentAddr]

		; Previously overwritten instructions
		mov [rsp+10h],rbx ; Our Present will jmp back here on its own
		mov [rsp+20h],rsi
		push rbp
		push rdi
		push r14

		jmp qword ptr [jmpBackAddr]
	JmpToHookAndJmpBack ENDP
end

