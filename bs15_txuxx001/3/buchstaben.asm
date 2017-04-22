.data
hint: .asciiz "String: \n"
grossanzahl: .asciiz "\nAnzahl Grossbuchstaben: \n"
kleinanzahl: .asciiz "\nAnzahl Kleinbuchstaben: \n"
einmal: .asciiz "veraendert: \n"
zweimal: .asciiz "\nzweimal veraendert: \n"
buff: .space 100
newBuff: .space 100
newBuff2: .space 100
.text
main:
	sub $sp, $sp, 24
	sw $ra, ($sp)			#prolog, return addrr in stack
	sw $s0, 4($sp)
	sw $s1, 8($sp)
	sw $s2, 12($sp)
	sw $s3, 16($sp)
	sw $s4, 20($sp)
	la $a0,hint
	li $v0,4	
	syscall
	la $a0,buff
	li $a1,100
	li $v0,8
	syscall
	la $s2,buff 		#store addr of buff in s2
	la $s3,newBuff 		#store addr of newbuff in s3
	la $s4,newBuff2
	move $t0,$s2 		#load addr of buff in t0 to read
	move $t1,$s3		#load addr of newbuff in t1 to write
	jal tausche
	move $t0,$s3 		#load addr of newbuff in t0 to count
	jal gross
	move $s0,$v0 		#store return value of gross in s0
	move $t0,$s3		#load addr of newbuff in t0 to count
	jal klein
	move $s1,$v0 		#store return value of klein in s1
	la $a0,einmal
	li $v0,4
	syscall
	la $a0,newBuff
	li $v0,4
	syscall	
	la $a0,grossanzahl			
	li $v0,4
	syscall
	move $a0,$s0
	li $v0,1
	syscall
	la $a0,kleinanzahl			
	li $v0,4
	syscall
	move $a0,$s1
	li $v0,1
	syscall
	
	move $t0,$s3 	#load addr of newbuff in t0 to read
	move $t1,$s4	#load addr of newbuff2 in t1 to write
	jal tausche	
	move $t0,$s2 	#load addr of buff in t0 to count
	jal gross
	move $s0,$v0 		#store return value of gross in s0
	move $t0,$s2 	#load addr of buff in t0 to count
	jal klein
	move $s1,$v0 
	la $a0,zweimal
	li $v0,4
	syscall
	la $a0,newBuff2
	li $v0,4
	syscall	
	la $a0,grossanzahl			
	li $v0,4
	syscall
	move $a0,$s0
	li $v0,1
	syscall
	la $a0,kleinanzahl			
	li $v0,4
	syscall
	move $a0,$s1
	li $v0,1
	syscall
	li $a0,10
	li $v0,11
	syscall

	lw $ra, ($sp)			
	lw $s0, 4($sp)
	lw $s1, 8($sp)
	lw $s2, 12($sp)
	lw $s3, 16($sp)
	lw $s4, 20($sp)
	addi $sp, $sp, 24
	jr $ra

tausche:
	sub $sp, $sp, 4
	sw $ra, ($sp)
	lb $t3, ($t0) 			
	li $t4,'a'
	li $t5,32 		#space
	li $t6,10		#new line
tauscheLoop:
	beqz $t3,tauscheLoopEnd 		#if(tmp == '\0') then loop end
	beq $t3, $t6 tauscheLoopEnd  	#if(newline) then loop end
	beq $t3,$t5 ifend       		#if(space) then if end not count
    blt $t3,$t4,Else  				#if(tmp < a) then else
   	sub $t3, $t3, 32
   	j ifend
Else:
	add $t3, $t3, 32
ifend:	
	sb $t3, ($t1) 			# newbuff[count] = tmp
	addi $t0, $t0, 1 		# buffcount ++
	addi $t1, $t1, 1		# newbuffcount ++
	lb $t3, ($t0)			# tmp = buff[count]
	j tauscheLoop
tauscheLoopEnd:
	lw $ra, ($sp)			
	addi $sp, $sp, 4
	jr $ra

gross:
	sub $sp, $sp, 4
	sw $ra, ($sp)
	#move $t0,$a0
	lb $t1, ($t0)  # char* tmp = buff[count]
	li $t2,0x41		# 'A'
	li $t3,0x5a		#'Z'
	li $v0,0 		#rslt of gross
grossLoop:
	beqz $t1,grossLoopEnd 		#while(tmp == '\0') then end
	bgt $t1,$t3,grossLoopIfEnd	#if(tmp > 'Z') end if
	blt $t1,$t2,grossLoopIfEnd	#if(tmp < 'A') end if
	addi $v0,$v0,1				# rslt++
grossLoopIfEnd:
	addi $t0,$t0,1				#count++
	lb $t1, ($t0)				#tmp = buffer[count]
	j grossLoop
grossLoopEnd:
	lw $ra, ($sp)			
	addi $sp, $sp, 4
	jr $ra

klein:
	sub $sp, $sp, 4
	sw $ra, ($sp)
	#move $t0,$a0
	lb $t1, ($t0)  # char* tmp = buff[count]
	li $t2,0x61		# 'a'
	li $t3,0x7a		#'z'
	li $v0,0 		#rslt of klein
kleinLoop:
	beqz $t1,kleinLoopEnd 		#while(tmp == '\0') then end
	blt $t1,$t2,kleinLoopIfEnd	#if(tmp < 'a') end if
	addi $v0,$v0,1				# rslt++
kleinLoopIfEnd:
	addi $t0,$t0,1				#count++
	lb $t1, ($t0)				#tmp = buffer[count]
	j kleinLoop
kleinLoopEnd:
	lw $ra, ($sp)			
	addi $sp, $sp, 4
	jr $ra
