	.text
	addi	r2,r0,#2	
	addi	r3,r0,#3
	add	r4,r2,r3
	sw	0(r0),r0
	sw	4(r0),r2
	sw	8(r0),r3
	sw	12(r0),r4
	lhi	r2,#0xdead
	addui	r2,r2,#0xbeef
	addi	r3,r0,#4
loop:
	sw	0(r2),r3
	subi	r3,r3,#1
	bnez	r3,loop
	addi	r2,r2,#4

	
