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
	add  A
	addi B test_define
	addc L
	sub  U
	subi A 99
	subc A
	not  A
	and  A
	andi A 34
	or   A
	ori  A 4
	xor  A 
	xori A 54
	shl  A
	shlc A
	ldi  A 45
	ld   A
	lda  A 123
	st   A
	sta  A 244
	sti  A 88
	stia 23 9321
	cmp  
	test_macro_1 A 3
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
	mov  A A
	mov  A B
	mov  A L
	mov  A U
	mov  B A
	mov  B B
	mov  B L
	mov  B U
	nop
	setc
	clc