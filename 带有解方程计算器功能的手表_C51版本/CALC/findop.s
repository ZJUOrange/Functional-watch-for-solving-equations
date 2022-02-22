	AREA	MY_FUNCTION1,CODE,READONLY	; 这一行必要的除了My_Function可以自己取名以外，其他的都是模板
	EXPORT findop  ; 与在c文件中定义的findop函数关联起来
	IMPORT strlen
	IMPORT myprintf
	IMPORT oprank
;	ENTRY
	PRESERVE8
findop
	;MOV R1,R0		   ;将参数1写入R1保护
	STMFD SP!,{R4-R11,R14}
	;PUSH {R14}
	MOV R6,R0		   ;将字符串的首地址放在R6中
	MOV r4,#0xFFFFFFFF ;r4 是 ret
	MOV r5,#0xFFFFFFFF ;r5 是 lowestoprank
	MOV r7,#0		   ;r7 是 is
	MOV r8,#0		   ;r8 是 was
	;调用strlen,返回值在R0，同时会改变R14
	BL strlen
	MOV R11,R0		   ;此时R11是i
LOOP
	
	SUB R11,R11,#1		;R11用作i，此时为strlen(str)-1
	LDRB R9,[R6,R11]		;R9是str[i]的ascii码值
	CMP R9,#0x29
	BNE N29
	ADD R7,R7,#1
	MOV R8,#1
	B IS0
N29
	CMP R9,#0x28
	BNE N28
	SUB R7,R7,#1  ;R7是is
	B IS0
N28 
	CMP R7,#0
	BNE IS0    ;hh
	CMP R9,#0x2D
	BNE N2D
	MOV R10,R11
	SUB R10,R10,#1
	LDRB R10,[R6,R10]  ;R10是str[i-1]的ascii码值
	CMP R10,#0x29
	BEQ N2D
	CMP R10,#0X30
	BCC CONTINUE
	CMP R10,#0X39
	BLS N2D
CONTINUE			  ;跳到当前循环底
	B IS0
N2D	
	MOV R0,R9
	BL oprank  ;R0的数值是RANK
	CMP R0,R5
	BLE IS0
	MOV R4,R11
	MOV R5,R0
IS0	
	CMP R11,#0
	BGT LOOP
	
	
	CMP R4,#0xFFFFFFFF
	BNE FINDOP_RETURN
	CMP R8,#1
	BNE FINDOP_RETURN
	MOV R4,#0xFFFFFFFE
FINDOP_RETURN
	MOVS R0,R4
	LDMFD SP!,{R4-R11,R14}
	MOV PC,LR
	
	END
	