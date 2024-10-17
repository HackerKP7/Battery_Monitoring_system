import serial
from matplotlib import pyplot as plt
import matplotlib.dates as mdates
import csv 
from datetime import datetime
import time
from time import sleep
prev_discrete_battery_percent = 0
once = 0
offset = 5
trigger_status = 0
prev_status = 0


port = '/dev/ttyUSB1'
baud_rate = 9600

ser = serial.Serial(port,baud_rate)

time = []
graph_time = []
battery_percent = []
battery_volts = []
figure,axis = plt.subplots(3)
line1, = axis[0].plot([],[],linestyle='solid',label = 'Battery Percent all Time')
axis[0].set_xlabel('Time')
axis[0].set_ylabel('Battery Volt')
line2, = axis[1].plot([],[],linestyle = 'solid',label = 'Battery Percent all Time')
axis[1].set_xlabel('Time')
axis[1].set_ylabel('Battery Percent')
line3, = axis[2].plot([],[],linestyle = 'solid',label = 'Battery Percent')
axis[2].set_xlabel('Time')
axis[2].set_ylabel('Battery Percent')

for ax in axis:
    ax.xaxis.set_major_formatter(mdates.DateFormatter('%H:%M'))


plt.ion()

def process():
    global discrete_battery_percent,prev_discrete_battery_percent,once,num,prev_status,trigger_status,s

    line = ser.readline().decode('utf-8').strip()
    battery_reading = line.split(', ')
    current_time = datetime.now()
    discrete_battery_percent = int(battery_reading[1])
    status = int(battery_reading[2])

    if (prev_status != status):
        trigger_status = 1

    if(status == 1 and trigger_status == 1):
        print("IN")
        s = "Discharging"
        num = 100  
        trigger_status = 0

    if(status == 1 and trigger_status == 1):
        print("IN")
        s = "Charging"
        num = 0
        trigger_status = 0
    
    prev_status = status

    print(f'Time: {current_time}, battery voltage: {battery_reading[0]} V, battery percentage: {battery_reading[1]} %, Current: {battery_reading[3]} mA, Status: {s}, prev_battery_voltage: {prev_discrete_battery_percent} %, num: {num}')
#when charging replace with lowest_num and sign change, plus chage to -10
    # h = str(datetime.now().time().hour) + ":" + str(datetime.now().time().minute)

    time.append(current_time)
    battery_volts.append(float(battery_reading[0]))
    battery_percent.append(int(battery_reading[1]))

    line1.set_data(time, battery_volts)
    line2.set_data(time, battery_percent)

  

    if (s == "Discharging"):
        if (float(battery_reading[3]) < 5):
            print("----------------------------CUTOFF----------------------------")
        if ((num > discrete_battery_percent and prev_discrete_battery_percent == discrete_battery_percent+offset) or (once == 0)):    
        # print("IN")

            datalogging()
            plotting()
            num = discrete_battery_percent
            once = 1
    if (s == "Charging"):
        if (float(battery_reading[3]) > -5):
            print("----------------------------CUTOFF----------------------------")
    
        if ((num < discrete_battery_percent and prev_discrete_battery_percent == discrete_battery_percent-offset) or (once == 0)):    
        # print("IN")
            datalogging()
            plotting()
            num = discrete_battery_percent
            once = 1

    for ax in axis:
        ax.relim()
        ax.autoscale_view()

    prev_discrete_battery_percent = discrete_battery_percent
    figure.canvas.draw()
    figure.canvas.flush_events()
    plt.show(block = False)
    plt.pause(0.1)


def plotting():
    line3.set_data(time,battery_percent)
   
    

def datalogging():
    with open('data.csv','w',newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(['Time','Battery Volt','Battery percent'])
        for x,bv,bp in zip(time,battery_volts,battery_percent):
            writer.writerow([x,bv,bp])


while True:
    process()
    print(" ")
    sleep(1)
    plt.savefig("plot.png")



