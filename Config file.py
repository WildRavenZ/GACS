'''
*Title: Config file.py
Author: Fernando Franco
Version: 1.0
Initial date: 09/05/2023
Version date: 09/5/2023
Description: Python program that creates a configuration
file named "GACS_Config.txt" for use it in "GACS.ino".
'''
#Define a limit for the temperature.
ilt = input("Select the inferior limit range for the temperature of the plant: ");
#Define a limit for the light intensity.
ill = input("Select the inferior limit range for the light intensity of the plant: ");
#Create or open the "GACS_Config.txt" configuration file.
GACS = "GACS_Config.txt"
with open(GACS, "w") as gacs:
    gacs.write(ilt + '\n')
    gacs.write(ill)
