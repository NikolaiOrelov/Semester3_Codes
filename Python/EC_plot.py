import time
import csv
import matplotlib.pyplot as plt
import numpy as np

def main():
    
    while True:
        
        with open("tent_measurement.txt", 'r') as file:
            csvReader = csv.reader(file)
            
        
        values = np.genfromtxt("tent_measurement.txt", delimiter=',', names=["time","Infrared_light","visible_light", "temperature", "humidity","water_temp", "Ph", "conductivity","Co2"])
        fig = plt.figure()
        x1 = values["time"]
        y1 = values["conductivity"]
        fig.suptitle('Conductivity sensor output')
        plt.xlabel('time(h)')
        plt.ylabel('Electric conductivity of Water in µS/cm')
        plt.plot(x1, y1)
        plt.show()
        
        time.sleep(1800)
        
            
#call main():            
if __name__ == "__main__":
    main()

