.data
array: .word 3, 3, 3, 3, 3, 3
count: .word 6
mensaje1: .asciiz "Los datos en memoria han sido borrados "
mensaje2: .asciiz "Gracias a la pila, s0 tiene el dato siguiente"

.text
.globl main
main:

la $a0,array
lw $s0, 0($a0)
la $a1,count
lw $a1, 0($a1)

jal clear_array

la $a0, mensaje1
addi $v0, $0, 4
syscall

la $a0, mensaje2
addi $v0, $0, 4
syscall

addi $a0,$s0, 0
addi $v0, $0, 1
syscall

addi $v0, $0, 10
syscall

clear_array:
addi $sp, $sp, -8 

#La subrutina se encarga de guardar los valores apuntados a los registros guardados en los $s
sw $s0, 0($sp)	
sw $s1, 4($sp)

add $s0, $zero, $zero
add $s1, $zero, $a0

Loop:
beq $s0, $a1, exit
sw $zero, 0($s1)
addi $s1, $s1, 4
addi $s0, $s0, 1
j Loop

exit:
lw $s0, 0($sp)
lw $s1, 4($sp)
addi $sp, $sp, 8
jr $31