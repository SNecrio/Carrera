.data
cadena: .asciiz "Hola Mundo!"

.text
.globl main
main:
la $a0, cadena
addi $v0, $0, 4
syscall
addi $v0, $0, 10
syscall