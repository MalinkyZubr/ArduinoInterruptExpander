.include "tn85def.inc"


.equ maxInputAddress = 64 ; maximum read address for interrupt expander
.equ plexClockPort = PORTB0 ; define multiplexer clock port
.equ plexReadPort = PORTB1 ; define multiplexer read port