.include "../include/tn85def.inc"
.include "../include/constants.inc"


.def clockMask = r16 ; holds toggle 1 for xor the plex clock value
.def readMask = r17 ; bitmask for read port
.def scanCounter = r18 ; maximum 64
.def readValue = r19 ; the value read from the read pin

.def temp1 = r20 ; misc general register
.def temp2 = r21 ; misc general register


start:
    ; unless there is a condition that breaks out of the main scan loop, stack need not be used for subroutine call
    ; push clockMask
    ; push readMask
    ; push scanCounter
    
    ; push temp1
    ; push temp2

    ldi clockMask, plexClockPort
    ldi readMask, plexReadPort
    ldi readValue, 0

toggleClockPort:
    push temp1
    in temp1, PORTB
    eor temp1, clockMask
    out PORTB, temp1
    pop temp1

    ret

readPlexReadPort:
    in temp1, PINB
    and temp1, readMask
    
    ret

handlePlexInput:
    cpi temp1, 0
    breq skipSPI; call something here

    ; send byte via spi here

    skipSPI: 

    ret

scanMainLoop:
    start_loop:
        ldi scanCounter, 0
    next_iteration:
        rcall toggleClockPort
        rcall toggleClockPort

        rcall readPlexReadPort
        rcall handlePlexInput

        inc scanCounter
        cpi scanCounter, maxInputAddress

        brne next_iteration
        rjmp start_loop

    ret

