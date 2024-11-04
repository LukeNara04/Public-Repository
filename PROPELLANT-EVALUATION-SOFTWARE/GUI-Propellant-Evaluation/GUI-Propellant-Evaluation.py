# IMPORTING LIBRARIES
import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque
import threading

# SERIAL PORT CONFIGURATION
ser = serial.Serial('COM4', 115200)  

# DEQUES TO STORE TIME AND PRESSURE DATA
time = deque(maxlen=500)
pressure = deque(maxlen=500)

# LOCK FOR SHARED RESOURCES
data_lock = threading.Lock()

# OPEN FILE TO SAVE DATA (CSV-like format with .txt extension)
output_file = open('time_pressure_data.txt', 'w')
output_file.write("Time [s], Pressure [PSI]\n")  # Header for the file

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
                    # Append data to deques
                    time.append(float(data[0]) / 1000)  # Time in seconds (or any unit)
                    pressure.append(float(data[1]))     # Pressure in psi
                    
                    # Write the data to the output file
                    output_file.write(f"{float(data[0])/1000}, {float(data[1])}\n")
                    output_file.flush()  # Ensure data is written immediately to the file
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
ax.set_xlim(0, 5)   # Initial x-axis limit (time)
ax.set_ylim(0, 20)  # Initial y-axis limit (pressure)
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
            ax.set_xlim(min(time), max(time) + 0.5)
        if pressure:
            ax.set_ylim(min(pressure), max(pressure) + 10)

    return line,

# SETUP ANIMATION
ani = animation.FuncAnimation(fig, animate, interval=100)

# SHOW PLOT
plt.tight_layout()
plt.show()

# END SERIAL TRANSMISSION AND CLOSE FILE WHEN PLOT WINDOW CLOSES
ser.close()
output_file.close()
