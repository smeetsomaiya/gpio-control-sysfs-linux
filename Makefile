CC_i586=/opt/iot-devkit/1.7.2/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux/i586-poky-linux-gcc
	
CC_DEFAULT=gcc

EXECUTABLE_GPIO = RGBLed_1.o
EXECUTABLE_PWM = RGBLed_2.o

## Make commands for the Galileo/ i586 arch

galileo_gpio:
	$(CC_i586) -pthread -Wall -g -o $(EXECUTABLE_GPIO) RGBLed_1.c

galileo_pwm:
	$(CC_i586) -pthread -Wall -g -o $(EXECUTABLE_PWM) RGBLed_2.c

## Make commands for the host machine

host_gpio:
	$(CC_DEFAULT) -pthread -Wall -g -o $(EXECUTABLE_GPIO) RGBLed_1.c

host_pwm:
	$(CC_DEFAULT) -pthread -Wall -g -o $(EXECUTABLE_PWM) RGBLed_2.c
	
clean:
	
	rm -f *.o
	rm -f $(EXECUTABLE_GPIO)
	rm -f $(EXECUTABLE_PWM) 
