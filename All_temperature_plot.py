
import time
import csv
import matplotlib.pyplot as plt
import numpy as np

def main():
    
    while True:

        with open("tent_measurement.txt", 'r') as file:
            csvReader = csv.reader(file)
            
        
        values = np.genfromtxt("tent_measurement.txt", delimiter=',', names=["time","Infrared_light","visible_light", "temperature", "humidity","water_temp", "Ph", "conductivity","Co2"])
        fig, axs = plt.subplots(2)
        axs[0].plot(values["time"], values["temperature"])
        axs[0].set(xlabel="time(s)", ylabel="Temperature in °C")
        axs[1].plot(values["time"], values["water_temp"], color="orange")
        axs[1].set(xlabel="time(h)", ylabel="Water Temperature in °C")
        fig.suptitle('Temperature sensor output')
        plt.show()
        
        time.sleep(1800)
        
            
#call main():            
if __name__ == "__main__":
    main()