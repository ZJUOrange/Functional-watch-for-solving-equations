	AREA	My_Function2,CODE,READONLY	; 这一行必要的除了My_Function可以自己取名以外，其他的都是模板
	EXPORT 	weekget ; 与在c文件中定义的weekget函数关联起来
	PRESERVE8
weekget
	STMFD SP!,{R4-R11,R14}		;保护寄存器的数据
	CMP R1,#1					;month == 1
	BEQ mon12
	CMP R1,#2					;month == 2
	BEQ mon12
	;MOV R3,#0					;R3是c
	;MOV R4,#0					;R4是y
	B calcweek
	
mon12
	ADD R1,R1,#12
	SUB R0,R0,#1

calcweek
	MOV R10,#100
	UDIV R3,R0,R10				;R3是c
	MUL	R5,R3,R10				;R5是temp,中间计算值
	;SUB R4,R0,R5				;R4是y
	SUB R9,R0,R5				;R4是y
	ADD R1,R1,#1				;R1=m+1
	MOV R10,#26
	MUL	R1,R1,R10				;R1=26*(m+1)
	MOV R10,#10
	UDIV R1,R1,R10				;R1=26*(m+1)/10
	ADD R1,R1,R2				;R1=26*(m+1)/10+d
	SUB R1,R1,#1				;R1=26*(m+1)/10+d-1
	;ADD R1,R1,R4,LSR #2			;R1=y/4+26*(m+1)/10+d-1
	ADD R1,R1,R9,LSR #2			;R1=y/4+26*(m+1)/10+d-1
	;ADD R1,R1,R4				;R1=y+y/4+26*(m+1)/10+d-1
	ADD R1,R1,R9				;R1=y+y/4+26*(m+1)/10+d-1
	SUB R1,R1,R3,LSL #1			;R1=-2*c+y/4+26*(m+1)/10+d-1
	ADD R1,R1,R3,LSR #2			;R1=c/4-2*c+y+y/4+26*(m+1)/10+d-1
								;R1是W
	CMP R1,#0
	BGE week
	MOV R10,#0
	SUB R0,R10,R1				;R0=-W
	MOV R10,#7
	UDIV R0,R0,R10				;R0=-W/7
	ADD R0,R0,#1				;R0=-W/7+1
	MUL R0,R0,R10				;R0=(-W/7+1)*7
	SUB R0,R0,R1				;R0=W+(-W/7+1)*7
	UDIV R5,R0,R10				;R5是temp
	MUL	R5,R5,R10
	SUB R0,R0,R5
	B week_end
week
	MOV R10,#7
	UDIV R5,R1,R10				;R5是temp
	MUL	R5,R5,R10
	SUB R0,R1,R5
	B week_end

week_end
	LDMFD SP!,{R4-R11,R14}
	MOV PC,LR
	
	END
		