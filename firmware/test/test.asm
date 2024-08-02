	.include "../tn85def.inc"
	
	.cseg
	.org 0x00
	
	ldi r16, (1<<PINB0)          ; load 1 to register 16
	out DDRB, r16
	out PORTB, r16
	
	
loop: rjmp loop
	
	; avrdude -p t85 -e -P /dev/ttyACM0 -c avrisp -b 19200 -U flash:w:test.hex
