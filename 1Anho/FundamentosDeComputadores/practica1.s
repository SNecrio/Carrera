.text
.globl main
main:
addi $a0, $zero, 4660
addi $a1, $zero, 65244
add $a0, $a0, $a1
addi $v0, $zero, 1
syscall
addi $v0, $zero, 10
syscall