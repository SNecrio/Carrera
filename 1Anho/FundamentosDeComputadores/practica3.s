.data
vector: .word 8, 4, 3, 5, 9
tamano: .word 5
suma: .word 0
vari: .word 0

.text
.globl main
main:

la $t0, vector
la $a1, tamano
la $a2, vari
la $a0, suma


lw $s1, 0($a1)
lw $s2, 0($a2)
lw $s0, 0($a0)

bne $s2, $s1, JUMP

JUMP:
	lw $t1, 0($t0)
	add $s0, $s0, $t1
	addi $s2, $s2, 1
	addi $t0, $t0, 4


bne $s2, $s1, JUMP

add $a0, $0, $s0
addi $v0, $0, 1
syscall

addi $v0, $0, 10
syscall