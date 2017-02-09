ADD B,C     ; = 0 0 0 0 0 1 1 0 = 06h
SUB D,D     ; = 0 0 0 1 1 1 1 1 = 1Fh
LOAD C,[B]  ; = 0 0 1 0 1 0 0 1 = 29h
STORE C,[B] ; = 0 0 1 1 1 0 0 1 = 39h
JLEZ  A,B    ; = 0 1 0 0 0 0 0 1 = 41h
JALR  C,D    ; = 0 1 0 1 1 0 1 1 = 5bh
LUI   C, 9    ; = 1 0 1 0 1 0 0 1 = A9h
LLI   B, 2    ; = 1 1 0 1 0 0 1 0 = D2h

