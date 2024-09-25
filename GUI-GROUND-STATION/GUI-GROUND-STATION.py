# IMPORTING LIBRARIES
import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque
import threading

# SERIAL PORT CONFIGURATION (ARDUINO NANO EVERY)
ser = serial.Serial('COM3', 115200)

# PLOT INITIALIZE |time|acx|acy|acz|gyx|gyy|gyz|alt|vel|tmp|aoa|
fig, axes = plt.subplots(2, 3, figsize=(8, 4))  

# INDIVIDUAL PLOT AXIS
ACC, GYR, ALT, VEL, TMP, AOA = axes.flatten()

# DEQUES FOR EFFICIENT DATA MANAGEMENT
time = deque(maxlen=200)
acx = deque(maxlen=200)
gyx = deque(maxlen=200)
alt = deque(maxlen=200)
vel = deque(maxlen=200)
tmp = deque(maxlen=200)
aoa = deque(maxlen=200)

# LOCK FOR SHARED RESOURCES
data_lock = threading.Lock()

def read_serial_data():
    while True:
        try:
            line = ser.readline().decode('utf-8').strip()
            data = line.split(',')

            if len(data) >= 7:
                with data_lock:
                    time.append(float(data[0])/1000)
                    acx.append(float(data[1]))
                    gyx.append(float(data[2]))
                    alt.append(float(data[3]))
                    vel.append(float(data[4]))
                    tmp.append(float(data[5]))
                    aoa.append(float(data[6]))
        except Exception as e:
            print(f"Error reading serial data: {e}")

# READING SERIAL THREAD
serial_thread = threading.Thread(target=read_serial_data)
serial_thread.daemon = True
serial_thread.start()

# Initializing line objects for efficient updates
acc_line, = ACC.plot([], [], label='acx', color='yellow')
gyr_line, = GYR.plot([], [], label='gyx', color='red')
alt_line, = ALT.plot([], [], label='alt', color='lightblue')
vel_line, = VEL.plot([], [], label='vel', color='blue')
tmp_line, = TMP.plot([], [], label='tmp', color='orange')
aoa_line, = AOA.plot([], [], label='aoa', color='green')

# ANIMATION
def animate(i):
    with data_lock:
        # Update the data of the plot lines, instead of clearing and redrawing
        acc_line.set_data(time, acx)
        gyr_line.set_data(time, gyx)
        alt_line.set_data(time, alt)
        vel_line.set_data(time, vel)
        tmp_line.set_data(time, tmp)
        aoa_line.set_data(time, aoa)

        # Dynamically rescale the axes
        ACC.relim()
        ACC.autoscale_view()
        GYR.relim()
        GYR.autoscale_view()
        ALT.relim()
        ALT.autoscale_view()
        VEL.relim()
        VEL.autoscale_view()
        TMP.relim()
        TMP.autoscale_view()
        AOA.relim()
        AOA.autoscale_view()

# SETUP ANIMATION with a longer interval for better performance (e.g., 200ms)
ani = animation.FuncAnimation(fig, animate, interval=200)

# PLOT
plt.tight_layout()
plt.show()

# END TRANSMISSION
ser.close()


