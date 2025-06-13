.data
vector: .word 8, 4, 3, 5, 9
n1: .word 0
n2: .word 0
n3: .word 0

.text
.globl main
main:

la $t0, vector
la $a1, n1
la $a2, n2
la $a0, n3

lw $t1, 0($t0)
lw $t2, 16($t0)
add $a1, $t1, $t2

lw $t1, 12($t0)
lw $t2, 4($t0)
add $a2, $t1, $t2

add $a0, $a2, $a1
lw $t1, 8($t0)
add $a0, $a0, $t1

addi $v0, $0, 1
syscall

addi $v0, $0, 10
syscall