.data 
zahln: .asciiz "Zahl n groesser oder gleich 0: \n"
zahlk: .asciiz "Zahl k groesser oder gleich 0 und kleiner oder gleich n: \n"
bino: .asciiz "Binominalkoeffizient: \n"

.text
main:
	sub $sp, $sp, 4
	sw $ra, ($sp)
	la $a0,zahln
	li $v0, 4
	syscall
	li $v0,5
	syscall
	move $t0,$v0  #store n in t0
	la $a0,zahlk
	li $v0, 4
	syscall
	li $v0,5
	syscall
	move $t1,$v0	#store k in t1
	la $a0,bino
	li $v0, 4
	syscall
	jal reku
	move $a0,$v0
	li $v0,1
	syscall
	li $a0,10
	li $v0,12
	syscall

	lw $ra, ($sp)
	add $sp, $sp, 4

reku:
	sub $sp, $sp, 16
	sw $ra, ($sp)
	sw $t0, 4($sp)
	sw $t1, 8($sp)
	sw $s0, 12($sp)
	li $s0,0
	beqz $t1,return1		#if k==0 then reuturn1
	beq $t0, $t1, return1	#if k==n then reuturn1
	sub $t0, $t0, 1 		#n-1
	jal reku
	add $s0, $s0, $v0
	
	sub $t1, $t1, 1			#k-1
	jal reku
	add $s0, $s0, $v0
	move $v0,$s0	
	lw $ra, ($sp)
	lw $t0, 4($sp)
	lw $t1, 8($sp)
	lw $s0, 12($sp)
	add $sp, $sp, 16
	jr $ra

return1:
	li $v0,1	
	lw $ra, ($sp)
	lw $t0, 4($sp)	
	lw $t1, 8($sp)
	lw $s0, 12($sp)
	add $sp, $sp, 16
	jr $ra

