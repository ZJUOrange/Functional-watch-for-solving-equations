	AREA	MY_FUNCTION1,CODE,READONLY	; ��һ�б�Ҫ�ĳ���My_Function�����Լ�ȡ�����⣬�����Ķ���ģ��
	EXPORT findop  ; ����c�ļ��ж����findop������������
	IMPORT strlen
	IMPORT myprintf
	IMPORT oprank
;	ENTRY
	PRESERVE8
findop
	;MOV R1,R0		   ;������1д��R1����
	STMFD SP!,{R4-R11,R14}
	;PUSH {R14}
	MOV R6,R0		   ;���ַ������׵�ַ����R6��
	MOV r4,#0xFFFFFFFF ;r4 �� ret
	MOV r5,#0xFFFFFFFF ;r5 �� lowestoprank
	MOV r7,#0		   ;r7 �� is
	MOV r8,#0		   ;r8 �� was
	;����strlen,����ֵ��R0��ͬʱ��ı�R14
	BL strlen
	MOV R11,R0		   ;��ʱR11��i
LOOP
	
	SUB R11,R11,#1		;R11����i����ʱΪstrlen(str)-1
	LDRB R9,[R6,R11]		;R9��str[i]��ascii��ֵ
	CMP R9,#0x29
	BNE N29
	ADD R7,R7,#1
	MOV R8,#1
	B IS0
N29
	CMP R9,#0x28
	BNE N28
	SUB R7,R7,#1  ;R7��is
	B IS0
N28 
	CMP R7,#0
	BNE IS0    ;hh
	CMP R9,#0x2D
	BNE N2D
	MOV R10,R11
	SUB R10,R10,#1
	LDRB R10,[R6,R10]  ;R10��str[i-1]��ascii��ֵ
	CMP R10,#0x29
	BEQ N2D
	CMP R10,#0X30
	BCC CONTINUE
	CMP R10,#0X39
	BLS N2D
CONTINUE			  ;������ǰѭ����
	B IS0
N2D	
	MOV R0,R9
	BL oprank  ;R0����ֵ��RANK
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
	