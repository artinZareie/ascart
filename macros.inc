%include "consts.inc"

EXTERN strlen

%macro printstr 2
	mov rax, SYS_WRITE
	mov rdi, STDOUT
	mov rsi, %1
	mov rdx, %2
	syscall
%endmacro

%macro exit 1
	mov rax, SYS_EXIT
	mov rdi, %1
	syscall
%endmacro

%macro strlenm 1
	mov rdi, %1
	call strlen
%endmacro
