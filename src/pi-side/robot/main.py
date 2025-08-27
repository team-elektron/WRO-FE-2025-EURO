import tkinter as tk
from tkinter import ttk
import subprocess
import threading

root = tk.Tk()
root.title("LCD Control")
root.geometry("320x212+0+28")  # account for top bar
root.resizable(False, False)

# === Variables for radio button groups ===
challenge_mode = tk.StringVar(value="Open Challenge")
run_side = tk.StringVar(value="Left")

# === Top row: radio buttons inline ===
radio_frame = ttk.Frame(root, padding=5)
radio_frame.pack(fill="x", pady=5)

# Challenge Mode (left)
challenge_frame = ttk.LabelFrame(radio_frame, text="Challenge Mode", padding=5)
challenge_frame.pack(side="left", expand=True, fill="both", padx=5)

ttk.Radiobutton(challenge_frame, text="Open Challenge", variable=challenge_mode, value="Open Challenge").pack(anchor="w")
ttk.Radiobutton(challenge_frame, text="Obstacle Challenge", variable=challenge_mode, value="Obstacle Challenge").pack(anchor="w")

# Run Side (right)
run_frame = ttk.LabelFrame(radio_frame, text="Run Side", padding=5)
run_frame.pack(side="right", expand=True, fill="both", padx=5)

ttk.Radiobutton(run_frame, text="Left", variable=run_side, value="Left").pack(anchor="w")
ttk.Radiobutton(run_frame, text="Right", variable=run_side, value="Right").pack(anchor="w")

# === Button actions ===
def run_program_command():
    cm = challenge_mode.get()
    side = run_side.get()
    print(f"Program pressed -> Challenge: {cm}, Side: {side}")

    # Run different avrdude commands depending on combo
    if cm == "Open Challenge" and side == "Left":
        subprocess.run([
            "avrdude", "-v",
            "-patmega328p",
            "-carduino",
            "-P/dev/ttyUSB0",
            "-b115200",
            "-D",
            "-Uflash:w:/home/ngx/Desktop/robot/arduhex/nano_open_left.hex"
        ])
    elif cm == "Open Challenge" and side == "Right":
        subprocess.run([
            "avrdude", "-v",
            "-patmega328p",
            "-carduino",
            "-P/dev/ttyUSB0",
            "-b115200",
            "-D",
            "-Uflash:w:/home/ngx/Desktop/robot/arduhex/nano_open_right.hex"
        ])
    elif cm == "Obstacle Challenge" and side == "Left":
        subprocess.run([
            "avrdude", "-v",
            "-patmega328p",
            "-carduino",
            "-P/dev/ttyUSB0",
            "-b115200",
            "-D",
            "-Uflash:w:/home/ngx/Desktop/robot/arduhex/nano_obstacle_left.hex"
        ])
    elif cm == "Obstacle Challenge" and side == "Right":
        subprocess.run([
            "avrdude", "-v",
            "-patmega328p",
            "-carduino",
            "-P/dev/ttyUSB0",
            "-b115200",
            "-D",
            "-Uflash:w:/home/ngx/Desktop/robot/arduhex/nano_obstacle_right.hex"
        ])

# Program button wrapper for threading
def program_action():
    threading.Thread(target=run_program_command).start()

def start_action():
    cm = challenge_mode.get()
    print(f"Start pressed -> Challenge: {cm}")

    if cm == "Open Challenge":
        # Run the Python command in a thread
        def run_start_command():
            import serial
            import time
            # Open the serial port
            ser = serial.Serial("/dev/ttyUSB0", 115200)
            # Wait for Arduino Nano to reset and bootloader to finish
            time.sleep(2)
            # Send the 'o' character
            ser.write(b'o')
            # Close the serial port
            ser.close()

        threading.Thread(target=run_start_command).start()


def stop_action():
    print("Stop pressed")  # placeholder

# === Buttons layout ===
button_frame = ttk.Frame(root, padding=5)
button_frame.pack(fill="both", expand=True)

button_frame.columnconfigure(0, weight=1)
button_frame.columnconfigure(1, weight=1)

# Left column (Program + Start stacked)
program_button = tk.Button(button_frame, text="Program", bg="blue", fg="white",
                           font=("Arial", 12, "bold"), command=program_action)
start_button = tk.Button(button_frame, text="Start", bg="green", fg="white",
                         font=("Arial", 12, "bold"), command=start_action)

program_button.grid(row=0, column=0, sticky="nsew", pady=2, ipadx=5, ipady=5)
start_button.grid(row=1, column=0, sticky="nsew", pady=2, ipadx=5, ipady=5)

# Stop button bigger, to side
stop_button = tk.Button(button_frame, text="Stop", bg="red", fg="white",
                        font=("Arial", 14, "bold"), command=stop_action)
stop_button.grid(row=0, column=1, rowspan=2, sticky="nsew", padx=5, pady=2, ipadx=10, ipady=20)

root.mainloop()
