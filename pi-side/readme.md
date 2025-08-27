Dependencies for the Raspberry Pi side of the robot:
 - AVRDUDE
 - Python 3.x (Tkinter, Serial)
 - Raspberry Pi OS Bookworm (Debian 12)

The file main.py handles the main gui interface for the robot, obstacle mode selection and programming of the Arduino Nano through avrdude.
Compiled HEX files for AVRDUDE are located in the "arduhex" folder.
