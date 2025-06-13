.data
s1: .space 5
s2: .asciiz  "Hola"

.text
.globl main
main:

la $t0, s2
la $a0, s1

lb $s0, 3($t0)
sb $s0, 0($a0)

lb $s0, 2($t0)
sb $s0, 1($a0)

lb $s0, 1($t0)
sb $s0, 2($a0)

lb $s0, 0($t0)
sb $s0, 3($a0)

addi $v0, $0, 4
syscall