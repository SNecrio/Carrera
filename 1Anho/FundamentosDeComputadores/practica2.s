.data
mensaje: .asciiz "La suma de 5 + 10 + 15 + 20 es "
vector: .word 5, 10, 15, 20
resultado: .word 0

.text
.globl main
main:

la $t0, vector
lw $s0, 0($t0)
add $s1, $0, $s0

lw $s0, 4($t0)
add $s1, $s1, $s0

lw $s0, 8($t0)
add $s1, $s1, $s0

lw $s0, 12($t0)
add $s1, $s1, $s0

la $t0, resultado
sw $s1,0($t0)

la $a0, mensaje
addi $v0, $0, 4
syscall

add $a0, $s1, $0
addi $v0, $0, 1
syscall
addi $v0, $0, 10
syscall