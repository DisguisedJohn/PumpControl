# PumpControl
Pump control realised with relay module. This project does not solve detecting water level in tank, it is designed to work with system that detects water level and is build to turn pump it self. Addon designed in this projects is for well in which the water level is rising too fast so the pump is turned on and off in small intervals, which is not good for electric motor in the pump. 

Input for this module is one binary signal which 0V turns the pump on and 5V indicates low water level and turns the pump off. The program realized on arduino board increases the interval between turning on and off to defined time. 

Note: While the water level raises the pressure reduces, so there is no risk of overflow. Therefore we can increase the interval between pumping. 

System is designed for running 24/7 therefore WatchDog timer is applied to reduce downtime and increase reliability. 
