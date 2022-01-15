
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
        axs[0].plot(values["time"], values["visible_light"])
        axs[0].set(xlabel="time(s)", ylabel="Visible light in cd")
        axs[1].plot(values["time"], values["Infrared_light"],color="red")
        axs[1].set(xlabel="time(s)", ylabel="Infrared light in cd")
        fig.suptitle('Light sensor output')
        plt.show()
        
        time.sleep(1800)
        
            
#call main():            
if __name__ == "__main__":
    main()
