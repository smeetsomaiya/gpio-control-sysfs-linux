Controlling GPIO pins through the linux sysfs interface.
                              
To compile the program, check the different recipies available in the Makefile. As an example, I describe one below

Open the Linux terminal and navigate to the directory using cd/path r 
To compile for Galileo:
	make galileo_pwm 

To compile for other x86 architectures:
	make host_pwm

make command will create object file
	RGBLed_1.o - For GPIO control
	RGBLed_2.o - For PWM control

Run the object file using 
	./<filename>.o

The duty cycle and RGBLed IO pins (which map to the Arduino shield pins on Intel Galileo Gen2) can be changed via input.txt