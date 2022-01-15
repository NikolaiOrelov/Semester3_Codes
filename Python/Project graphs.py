 # -*- coding: utf-8 -*-
"""
Created on Fri Nov  5 12:15:38 2021

@author: CFS
"""
from datetime import datetime
import time
import csv
import numpy as np

def main():
    datetime_out = datetime.now().strftime("%d/%m/%y %H:%M")
    
    while True:
            
    
            # Opens the CSV file to extract data and import it 
            try:
    
                with open("tent_measurement.txt", 'r') as file: 
                      csvReader = csv.reader(file)
                   
                values = np.genfromtxt("tent_measurement.txt", delimiter=',', names=["time","Infrared_light","visible_light", "temperature", "humidity","water_temp", "Ph", "conductivity","Co2"])
                
            
            except:
                print("Unable to read data from CSV file try again!!")
            
            
            try:
                
                Visiblelight = values["visible_light"][-1]
                Infra = values["Infrared_light"][-1]
                temp = values["temperature"][-1]
                Humidity = values["humidity"][-1]
                Watertemperature = values["water_temp"][-1]
                My_ph = values["Ph"][-1]
                MY_EC = values["conductivity"][-1]
                My_co2 = values["Co2"][-1]
                print("--------------------------------------------------------------------")
                print(f" The current date and time is: {datetime_out}")
                print(f" The current amount of visible light is: {Visiblelight} cd")
                print(f" The current amount of Infrared light is: {Infra} cd")
                print(f" The current tent temperature is: {temp} °C")
                print(f" The current tent humidity is: {Humidity} %")
                print(f" The current water temperature is: {Watertemperature} °C")
                print(f" The current water Ph is: {My_ph} PH(0-14)")
                print(f" The current Conductivity of the water is : { MY_EC} µS/cm")
                print(f" The current amount of Co2 in the tent is: {My_co2} ppm")
                print("--------------------------------------------------------------------")
            
            except:
               
                print("Unable to print the data!! please ty again")
                break
         
            time.sleep(1800)
 
            
#call main():            
if __name__ == "__main__":
    main()
