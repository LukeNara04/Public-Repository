# IMPORTING LIBRARIES
import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque
import threading

# SERIAL PORT CONFIGURATION
ser = serial.Serial('COM3', 115200)  # Replace 'COM3' with your actual port

# DEQUES TO STORE TIME AND PRESSURE DATA
time = deque(maxlen=200)
pressure = deque(maxlen=200)

# LOCK FOR SHARED RESOURCES
data_lock = threading.Lock()

# FUNCTION TO READ DATA FROM SERIAL PORT
def read_serial_data():
    while True:
        try:
            # Read a line from the serial port
            line = ser.readline().decode('utf-8').strip()
            data = line.split(',')

            # Ensure the data is valid (time and pressure)
            if len(data) == 2:
                with data_lock:
                    time.append(float(data[0])/1000)     # Time in seconds (or any unit)
                    pressure.append(float(data[1]))      # Pressure in psi 
        except Exception as e:
            print(f"Error reading serial data: {e}")

# START A THREAD FOR READING SERIAL DATA
serial_thread = threading.Thread(target=read_serial_data)
serial_thread.daemon = True
serial_thread.start()

# PLOT INITIALIZATION
fig, ax = plt.subplots()
line, = ax.plot([], [], lw=2, color='blue')

# SETUP AXES
ax.set_xlim(0, 10)   # Initial x-axis limit (time)
ax.set_ylim(0, 250)  # Initial y-axis limit (pressure)
ax.set_xlabel('Time [s]')
ax.set_ylabel('Pressure [PSI]')
ax.set_title('Time vs Pressure')

# ANIMATION FUNCTION TO UPDATE PLOT
def animate(i):
    with data_lock:
        # Update line with new data
        line.set_data(time, pressure)
        # Dynamically adjust the x and y limits
        if time:
            ax.set_xlim(min(time), max(time) + 1)
        if pressure:
            ax.set_ylim(min(pressure), max(pressure) + 10)

    return line,

# SETUP ANIMATION
ani = animation.FuncAnimation(fig, animate, interval=100)

# SHOW PLOT
plt.tight_layout()
plt.show()

# END SERIAL TRANSMISSION (will only trigger when plot window is closed)
ser.close()
