.include "tn85def.inc"


.def scanCounter = r18 ; maximum 64

.equ maxInputAddress = 65 ; maximum read address for interrupt expander
.equ SPIDataInMask = (1<<PORTB0)
.equ SPIDataOutMask = (1<<PORTB1)
.equ SPIClockMask = (1<<PORTB2)
.equ plexPortMask = (1<<PORTB3) ;handles clock and reading
.equ SPIChipSelectMask = (1<<PORTB4) ; doubles as interrupt port to tell master to initiate communications
