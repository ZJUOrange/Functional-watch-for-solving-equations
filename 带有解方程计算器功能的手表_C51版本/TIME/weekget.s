	AREA	My_Function2,CODE,READONLY	; ��һ�б�Ҫ�ĳ���My_Function�����Լ�ȡ�����⣬�����Ķ���ģ��
	EXPORT 	weekget ; ����c�ļ��ж����weekget������������
	PRESERVE8
weekget
	STMFD SP!,{R4-R11,R14}		;�����Ĵ���������
	CMP R1,#1					;month == 1
	BEQ mon12
	CMP R1,#2					;month == 2
	BEQ mon12
	;MOV R3,#0					;R3��c
	;MOV R4,#0					;R4��y
	B calcweek
	
mon12
	ADD R1,R1,#12
	SUB R0,R0,#1

calcweek
	MOV R10,#100
	UDIV R3,R0,R10				;R3��c
	MUL	R5,R3,R10				;R5��temp,�м����ֵ
	;SUB R4,R0,R5				;R4��y
	SUB R9,R0,R5				;R4��y
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
								;R1��W
	CMP R1,#0
	BGE week
	MOV R10,#0
	SUB R0,R10,R1				;R0=-W
	MOV R10,#7
	UDIV R0,R0,R10				;R0=-W/7
	ADD R0,R0,#1				;R0=-W/7+1
	MUL R0,R0,R10				;R0=(-W/7+1)*7
	SUB R0,R0,R1				;R0=W+(-W/7+1)*7
	UDIV R5,R0,R10				;R5��temp
	MUL	R5,R5,R10
	SUB R0,R0,R5
	B week_end
week
	MOV R10,#7
	UDIV R5,R1,R10				;R5��temp
	MUL	R5,R5,R10
	SUB R0,R1,R5
	B week_end

week_end
	LDMFD SP!,{R4-R11,R14}
	MOV PC,LR
	
	END
		