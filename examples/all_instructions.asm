%macro
test_macro_1 $r $0
	nop
	addi $r $0
test_label:
	nop
	be           test_label
%endm

%macro
test_macro_2
	nop
	be  test_label
%endm

%define test_define 99

start:
	add  r0
	addi r1 test_define
	addc r2
	sub  r3
	subi r0 99
	subc r0
	not  r0
	and  r0
	andi r0 34
	or   r0
	ori  r0 4
	xor  r0 
	xori r0 54
	shl  r0
	shlc r0
	ldi  r0 45
	ld   r0
	lda  r0 123
	st   r0
	sta  r0 244
	sti  r0 88
	stia 23 9321
	cmp  
	test_macro_1 r0 3
	b 
	b    start
	bc 
	bc   start
	blt 
	blt  start
	blte 
	blte start
	be 
	be   start
	bne
	bne  start
	bgt 
	bgt  start
	bgte
	bgte start
	test_macro_2
	mov  r0 r0
	mov  r0 r1
	mov  r0 r2
	mov  r0 r3
	mov  r1 r0
	mov  r1 r1
	mov  r1 r2
	mov  r1 r3
	nop
	setc
	clc