
ATTENTTOIN!!!!!!!!!!!!!!!!!

It is needed user input.

Use the pyocd to flash:

	First do a mass erase:
		$ pyocd erase --mass
		- power cycle the board (or press on/off button)
		- At the same time push Reset Button

	Load/flash binary
		$ pyocd flash ./tfm.hex

		