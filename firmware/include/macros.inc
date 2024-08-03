.include "tn85def.inc"

; @0: end of ram address. RAMEND
.macro setupStack @0
    push r16

    ldi r16,LOW(@0)
    out SPL, r16
    ldi r16,HIGH(@0)
    out SPH,r16

    pop r16
.endmacro