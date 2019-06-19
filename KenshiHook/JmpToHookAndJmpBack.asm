includelib legacy_stdio_definitions.lib

.model flat, c

.data
extern presentAddr : dword
extern jmpBackAddr : dword

; This jmps to our hook, then performs instructions 
; originally performed by dxgi.dll in the memory that 
; we've replaced, and then jmps back

.code
	JmpToHookAndJmpBack PROC 
		jmp [presentAddr]

		; Previously overwritten instructions
		mov edi,edi
		push ebp
		mov ebp,esp

		jmp [jmpBackAddr]
	JmpToHookAndJmpBack ENDP
end