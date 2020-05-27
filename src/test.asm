main:
addi $t0,$zero,10
sw $t0,24($sp)
addi $t0,$zero,24
sw $t0,20($sp)
addi $t0,$zero,2
sw $t0,16($sp)
lw $t1,20($sp)
lw $t2,16($sp)
mult $t1,$t2
mflo $t0
sw $t0,12($sp)
lw $t1,24($sp)
lw $t2,12($sp)
add $t0,$t1,$t2
sw $t0,4($sp)
lw $t1,4($sp)
add $t0,$zero,$t1
sw $t0,8($sp)
addi $t0,$zero,0
sw $t0,4($sp)
lw $t1,44($sp)
lw $t2,4($sp)
slt $t0,$t2,$t1
sw $t0,4($sp)
lw $t0,4($sp)
beq $t0,$zero label0
addi $t0,$zero,10
sw $t0,4($sp)
lw $t1,4($sp)
add $t0,$zero,$t1
sw $t0,16($sp)
j label1
label0:
addi $t0,$zero,9
sw $t0,4($sp)
lw $t0,4($sp)
move $a0,$t0
jal fact
add $t0,$v0,$zero
lw $t1,4($sp)
add $t0,$zero,$t1
sw $t0,24($sp)
label1:
label2:
addi $t0,$zero,0
sw $t0,4($sp)
lw $t1,64($sp)
lw $t2,4($sp)
slt $t0,$t2,$t1
sw $t0,4($sp)
lw $t0,4($sp)
beq $t0,$zero label3
addi $t0,$zero,1
sw $t0,4($sp)
lw $t1,72($sp)
lw $t2,4($sp)
add $t0,$t1,$t2
sw $t0,4($sp)
lw $t1,4($sp)
add $t0,$zero,$t1
sw $t0,76($sp)
j label2
label3:
fact:
addi $t0,$zero,0
sw $t0,4($sp)
lw $t1,4($sp)
add $t0,$zero,$t1
sw $t0,8($sp)
addi $t0,$zero,1
sw $t0,4($sp)
lw $t1,4($sp)
add $t0,$a0,$t1
sw $t0,4($sp)
lw $t1,4($sp)
add $t0,$zero,$t1
sw $t0,16($sp)
addi $t0,$zero,1
sw $t0,4($sp)
lw $t0,4($sp)
move $v0,$t0
jr $ra
