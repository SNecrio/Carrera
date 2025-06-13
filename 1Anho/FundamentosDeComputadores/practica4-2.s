.data
array: .word 1, 5, 2, 8, 6, 3
count: .word 6
.text
.globl main
main:
la $t0,array
la $t1,count
lw $t1,0($t1)
lw $t2, 0($t0)
add $t3, $0, $0
addi $t1, $t1, -1
Loop:
slt $t4, $t3, $t2
beq $t4, $0, dato_siguiente
add $t3, $0, $t2
dato_siguiente:
beq $t1, $0, salir
addi $t0, $t0, 4
lw $t2,0($t0)
addi $t1, $t1, -1
j Loop
salir:
addi $a0,$t3,0
addi $v0, $0, 1
syscall
addi $v0, $0, 10
syscall
