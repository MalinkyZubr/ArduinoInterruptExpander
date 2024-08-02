.include "../include/tn85def.inc"


.def mask = r16 ; holds toggle 1 for xor the plex clock value
.def scanCounter = r17 ; maximum 64

.def temp1 = r18 ; misc general register
.def temp2 = r19 ; misc general register

.equ maxInputAddress = 64 ; maximum read address for interrupt expander
.equ plexClockPort = PORTB0 ; define multiplexer clock port
.equ plexReadPort = PORTB1 ; define multiplexer read port

.cseg
.org 0x00

ldi mask, 1 ; load 1 into the mask bit for toggling the plex clock

; toggle the specified port for clock purposes
; @0: general purpose register for loading io register for xor
; @1: constant representing bit position in PORTB register

.macro 

; main macro for running the multiplexing operations
; @0 gregister for operations
; @1 const representing bit position of clock pin in PORTB register
; @2 const representing bit position of read pin in PORTB register
.macro plexScan
    ldi @0, 0
    gpioToggle @0, @1
    gpioToggle @0, @1

.endmacro

start: ldi scanCounter, 0 ; start the scan loop, reset the value of the scanCounter register to 0
