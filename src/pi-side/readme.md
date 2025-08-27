Dependencies for the Raspberry Pi side of the robot:

    AVRDUDE
    Python 3.x (Tkinter, Serial)
    Raspberry Pi OS Bookworm (Debian 12)

"run.sh" is used for starting the main GUI interface from the desktop and killing the start menu process to conserve screen space. The file "./robot/main.py" handles the main gui interface for the robot, obstacle mode selection and programming of the Arduino Nano through avrdude. Compiled HEX files for AVRDUDE are located in the "./robot/arduhex" folder.
