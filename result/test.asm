.data
.text
j main
cal_bmi:
subi $sp,$sp,80z
addi $at,$sp,0
sw $a0,0($at)
addi $at,$sp,4
sw $a1,0($at)
addi $at,$sp,8
sw $a2,0($at)
mult $a1,$a1
mflo $t0
addi $at,$sp,12
sw $t0,0($at)
addi $at,$sp,12
lw $t1,0($at)
div $a0,$t1
mflo $t0
addi $at,$sp,16
sw $t0,0($at)
addi $at,$sp,16
lw $t0,0($at)
move $v0,$t0
jr $ra
main:
subi $sp,$sp,80
addi $at,$sp,0
sw $a0,0($at)
addi $at,$sp,4
sw $a1,0($at)
addi $at,$sp,8
sw $a2,0($at)
li $s0,10
addi $at,$sp,12
sw $s0,0($at)
addi $at,$sp,12
lw $t0,0($at)
addi $at,$sp,20
sw $t0,0($at)
li $s0,40
addi $at,$sp,16
sw $s0,0($at)
addi $at,$sp,16
lw $t0,0($at)
addi $at,$sp,24
sw $t0,0($at)
addi $at,$sp,20
lw $t0,0($at)
move $a0,$t0
addi $at,$sp,24
lw $t0,0($at)
move $a1,$t0
addi $at,$sp,32
sw $ra,0($at)
jal cal_bmi
move $t0,$v0
addi $sp,$sp,80
addi $at,$sp,36
sw $t0,0($at)
addi $at,$sp,0
lw $t1,0($at)
move $a0,$t1
addi $at,$sp,4
lw $t2,0($at)
move $a1,$t2
addi $at,$sp,8
lw $t3,0($at)
move $a2,$t3
addi $at,$sp,32
lw $t4,0($at)
move $ra,$t4
addi $at,$sp,36
lw $t0,0($at)
addi $at,$sp,28
sw $t0,0($at)
