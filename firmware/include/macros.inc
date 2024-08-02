.include "tn85def.inc"


.macro setupStack
    ldi @0,LOW(@1)
    out SPL,@0
    ldi @0,HIGH(@1)
    out SPH,@0

    clr @0
.endmacro

.macro setupStackRamend
    setupStack @0,RAMEND
.endmacro

.macro gpio_configure ; complete later
    ; set the clock port as an output
    ldi temp1, (1<<plexClockPort)
    out DDRB, temp1

    ; set the read port as an input
    in temp1, DDRB
    ldi temp2, ~(1<<plexReadPort)

    and temp1, temp2
    out DDRB, temp1

    clr temp2
.endmacro


