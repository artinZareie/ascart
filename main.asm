%include "macros.inc"
%include "consts.inc"

BITS 64

section .data
	welcome_msg db "Hi sucker!", 0xA
	welcome_msg_len equ $ - welcome_msg

	scaled_width dd 0

section .bss
	input_path: resq 1
	output_path: resq 1
	argc: resd 1

section .text
	global _start

_start:
	printstr welcome_msg, welcome_msg_len

	; rsp + 0: argc
	; rsp + 8 + 8i: argv[i]

	; argc = [rsp]
	mov rax, [rsp]
	mov [argc], rax

	; input_path = [rsp + 16]
	mov rax, [rsp + 16]
	mov [input_path], rax

	; output_path = [rsp + 24]
	mov rax, [rsp + 24]
	mov [output_path], rax

	strlenm [output_path]

	printstr [input_path], rax

	exit 0

