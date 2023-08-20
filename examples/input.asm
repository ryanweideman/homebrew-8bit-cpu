	# this is a comment
start:
	ldi A 0

sum:
	ldi B 1
	add A
	mov A L
	ldi U 0
	st  A
	ldi A 255
	ld  A

	ldi B 255
	cmp
	blt  sum

end:
	b start
