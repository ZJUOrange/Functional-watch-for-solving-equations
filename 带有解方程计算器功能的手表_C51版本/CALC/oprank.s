	AREA	My_Function,CODE,READONLY	; ��һ�б�Ҫ�ĳ���My_Function�����Լ�ȡ�����⣬�����Ķ���ģ��
	EXPORT 	oprank  ; ����c�ļ��ж����oprank������������
	IMPORT myprintf
oprank
	;BL myprintf
	CMP r0,#0x2b
	BEQ ret4
	CMP r0,#0x2D
	BEQ ret4
	CMP r0,#0x2A
	BEQ ret3
	CMP r0,#0x2F
	BEQ ret3
	CMP r0,#0x25
	BEQ ret3
	CMP r0,#0x63
	BEQ ret2
	CMP r0,#0x73
	BEQ ret2
	CMP r0,#0x74
	BEQ ret2 
	CMP r0,#0x70
    BEQ ret2 
	CMP r0,#0x6c
	BEQ ret2
	CMP r0,#0x78
	BEQ ret1
	MOV r0,#0xFFFFFFFF
	MOV PC,LR
ret4
	MOVS r0,#4
	MOV PC,LR
ret3
	MOVS r0,#3
	MOV PC,LR
ret2
	MOVS r0,#2
	MOV PC,LR
ret1
	MOVS r0,#1
	MOV PC,LR

	END