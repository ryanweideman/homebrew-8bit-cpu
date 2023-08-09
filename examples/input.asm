	# this is a comment
start:
	ldi r0 0

sum:
	ldi r1 1
	add r0
	mov r0 r2
	ldi r3 0
	st r0
	ldi r0 255
	ld r0

	ldi r1 255
	cmp
	blt  sum

end:
	b start
