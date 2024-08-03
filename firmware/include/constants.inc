.include "tn85def.inc"


.def scanCounter = r18 ; maximum 64

.equ maxInputAddress = 65 ; maximum read address for interrupt expander
.equ SPIDataInMask = (1<<PORTB0)
.equ SPIDataOutMask = (1<<PORTB1)
.equ SPIClockMask = (1<<PORTB2)
.equ plexClockPortMask = (1<<PORTB3) ; define multiplexer clock port
.equ plexReadPortMask = (1<<PORTB4) ; define multiplexer read port
.equ SPIChipSelectMask = (1<<PORTB5) ; doubles as interrupt port to tell master to initiate communications