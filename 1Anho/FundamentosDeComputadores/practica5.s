.data
array: .word 1, 2, 3, 4, 5, 6
count: .word 6

.text
.globl main
main:

la $a0, array
la $a1, count
lw $a1, 0($a1)

jal suma #Cambia el PC al de la instruccion primera de suma y guarda en $ra la direccion de la onstruccion siguiente al jal suma

addi $a0, $v0, 0
addi $v0, $0, 1
syscall
addi $v0, $0, 10
syscall

suma:
add $t0, $a0, $0
add $t1, $a1, $0
add $t2, $0, $0

Loop:
beq $t1, $0, salir
lw $t3, 0($t0)
add $t2, $t3, $t2
addi $t1, $t1, -1
addi $t0, $t0, 4

j Loop

salir:
add $v0, $t2, $0
jr $ra