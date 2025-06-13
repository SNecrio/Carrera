.data
cadena: .asciiz "Hola"
cadena2: .space 10

.text
.globl main
main:

la $a0, cadena2
addi $v0, $0, 8
syscall

lw $a1, 0($a0)

la $a0, cadena
addi $v0, $0, 4	# el printf hola
syscall

lw $a0, 0($a1)
addi $v0, $0, 4	# el printf hola
syscall

addi $v0, $0, 10
syscall