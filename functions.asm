;==============quad STRLEN(*str)==============
; IN: *str(rdi)
; OUT: size(rax)
section .text
global strlen

strlen:
	push rdi
	xor rax, rax ; rax = 0

.L1:
	cmp byte [rdi], 0
	je .L2

	inc rax
	inc rdi
	jmp .L1

.L2:
	dec rax

	pop rdi
	ret
