;  ADD     RD, RS  add registers RD+RS, store the result in RD
;  SUB     RD, RS  subtract registers RD-RS, store the result in RD
;  LOAD    RD,[RS] load 1 byte from memory at address RS into RD
;  STORE   RD,[RS] store 1 byte from RD into memory at address RS
;  JLEZ    RD,RS   if RS<=0, jump to address in RD
;  JALR    RD,RS   save PC+1 into RS, jump to RD
;  LUI     RT, IMM load 4-bit IMM into the upper four bits of RT
;  LLI     RT, IMM load 4-bit IMM into the lower four bits of RT

; init start here
LUI d,f    ; regd will be the address we're using for store & load 
LLI d,0
LUI b,0     
LLI b,0
STORE b,d  ; Put a 0 in addr 'a'  (this is 'a' @ 0xf0)
LLI d,f
STORE b,d  ; Put a 0 in addr 'c'  (this is 'c' @ 0xff)
LLI d,1
LLI b,1
STORE b,d  ; put a 1 in addr 'b'  (this is 'b' @ 0xf1)

; loop is here
LLI d,0
LOAD a,d   ; Load 'a' into rega
LLI d,1
LOAD b,d   ; Load 'b' into regb and...
LOAD c,d   ; Load 'b' into regc
ADD c,a    ; Add rega onto regc
LLI d,f
STORE c,d  ; Store the sum in 'c'

LLI d,0
STORE b,d  ; 'a' = 'b'
LOAD a,d
LLI d,f
LOAD b,d
LLI d,1
STORE b,d  ; 'b' = 'c'

LUI c,0
LLI c,1    ; put a 1 in c
LLI d,e    ; load in the counter
LOAD b,[d] ; put the index/counter in b
SUB b,c    ; subtract c from b (b-1 eseentially)
STORE b,[d]; put the index back
SUB b,c    ; subtract c from b (b-1 eseentially)

LUI c,2
LLI c,6    
JLEZ c,b
LLI C,a
LUI c,0
JALR C,c

LUI c,2
LLI c,8
JALR c,c   ; Endless loop
