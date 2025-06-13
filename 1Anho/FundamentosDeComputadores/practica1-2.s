.data
a: .word 305419896
b: .word -19088744
c: .word 0

.text
.globl main

main:
la $t0, a 
lw $a1, 0($t0)
la $t0, b
lw $a2, 0($t0)
la $t0, c
lw $a0, 0($t0)

add $a0, $a1, $a2
sw $a0, 0($t0)

addi $v0, $zero, 1
syscall

addi $v0, $zero, 0
syscall