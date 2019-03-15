#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdint.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<signal.h>
#include <stdbool.h>
#include <signal.h>
#include "RGBLed_1.h"

int gpio_export(char *gpio)  
{
	int fd;

	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);     //export gpio
	if (fd < 0) {
		perror("gpio/export");
		return fd;
	}

	write(fd, gpio, strlen(gpio));
	close(fd);

	return 0;
}

int gpio_unexport(unsigned int gpio)
{
	int fd, len;
	char buf[MAX_BUF];

	fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);    //unexport gpio 
	if (fd < 0) {
		perror("gpio/export");
		return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
	return 0;
}

int gpio_set_dir(char *gpio, char *out_dir)                               //Sets the gpio direction as out or in
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR  "/gpio%s/direction", gpio);
	printf("Gpio Set direction - %s %s\n", buf, out_dir);
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/direction");
		return fd;
	}

	if (strcmp(out_dir, "out") == 0)
		write(fd, "out", 3);
	else
		write(fd, "in", 2);

	close(fd);
	return 0;
}


int gpio_set_value(char *gpio, char *value)                             //Sets the value of the gpio pins  as either 1 or 0
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%s/value", gpio);
	printf("Gpio Set value - %s %s\n", buf, value);
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-value");
		return fd;
	}

	if (strcmp(value,"1") == 0)
		write(fd, "1", 1);
	else
		write(fd, "0", 1);

	close(fd);
	return 0;
}

int mux_gpio_set(char *gpio, char *out_dir, char *value, bool isPin)   
{
	gpio_export(gpio);
	gpio_set_dir(gpio, out_dir);

	if(!isPin)
		gpio_set_value(gpio, value);

	return 0;
}
                                                                     
void* mouse_thread_body(void *arguments)                             
{
    int fd, bytes;
    unsigned char data[4];

    const char *mDevice = MOUSE_DEV;                                 


      
    fd = open(mDevice, O_RDWR);                                           //open mouse device in read write mode
printf("Mouse file opened%s\n", mDevice);
    if(fd == -1) {
        printf("ERROR Opening %s\n", mDevice);
		return NULL;
    }

    int left, right;

    while(exit_flag == 0) {
        bytes = read(fd, data, sizeof(data));
        if(bytes > 0)                                                      
        {
        left = data[0] & 0x01;                                       
        right = data[0] & 0x02;    

        if(left == 1 || right == 2)                                         //Identifies left or right mouse click 
		{
			printf("Mouse is clicked\n");
			sigHandler(left);                                                      //sends signal to signal handler on occurence of left or right mouse click
	    }
     }
}
     printf("Mouse thread exiting\n");
     pthread_exit(NULL);                                                //exits mouse thread
     return NULL;
}

int create_mouse_thread() {
	//Initialize mouse semaphore in a locked state
//	sem_init(&mouse, 0, 0);

	int err = pthread_create(&mouse_thread, NULL, &mouse_thread_body, NULL);    //Creates a mouse thread to continuously poll for an event
	if (err != 0) {
		printf("\nCan't create mouse thread :[%s]", strerror(err));
	}
	else {
		printf("\n Mouse Thread created successfully\n");
    }
	return 0;
}


void* lightup_sequence(void *arguments)                                         //lightup_sequence lits the RGB leds as per the specified pattern
{
    struct task_params_struct *args = arguments;
    int i, row,column = 0;
    int rows_of_pattern_array = ROW_SIZE(lightup_pattern_array);
    printf("\nLightup Thread created successfully. Click the left mouse button %d\n", rows_of_pattern_array);
    printf("\nPattern array %s\n", gpio_mapping_array[args -> red_led][PIN_COLUMN]);

    while(exit_flag == 0) {
        for(row = 0; row < rows_of_pattern_array; row++){           //Reads the pattern array declared in header file
	
            char* r = lightup_pattern_array[row][column];           //Reads each element of character array and assigns 1st element to r , 2nd to g and 3rd to b 
            char* g = lightup_pattern_array[row][column+1];
            char* b = lightup_pattern_array[row][column+2];

	        char* red_led_pin = gpio_mapping_array[args -> red_led][PIN_COLUMN];        //Reads each element from the gpio_mapping_array and maps the gpio pins for the input IO  
	        char* green_led_pin = gpio_mapping_array[args -> green_led][PIN_COLUMN];
	        char* blue_led_pin = gpio_mapping_array[args -> blue_led][PIN_COLUMN];

            printf("\nPattern: %s %s %s\n", r, g, b);                       

			for (i = 0; i<25; i++)                                            //The loop executes for 25 cycles for each pattern set
			{
            gpio_set_value(red_led_pin, r);                                   //sets the value for the mapped gpio pin for red_led_pin
            printf("\nPin: %s\n", red_led_pin);

            gpio_set_value(green_led_pin, g);                                 //sets the value for the mapped gpio pin for green_led_pin
            printf("\nPin: %s\n", green_led_pin);

            gpio_set_value(blue_led_pin, b);                                  //sets the value for the mapped gpio pin for blue_led_pin
            printf("\nPin: %s\n", blue_led_pin);

			usleep(calculate_on_time() * (int) SLEEP_PERIOD_MULTIPLIER_US);   //The on time of respective pattern
			
            gpio_set_value(red_led_pin, "0");                                 //sets the value for the mapped gpio pin for red_led_pin                         
            printf("\nPin: %s\n", red_led_pin);

            gpio_set_value(green_led_pin, "0");                               //sets the value for the mapped gpio pin for green_led_pin
            printf("\nPin: %s\n", green_led_pin);

            gpio_set_value(blue_led_pin, "0");                                //sets the value for the mapped gpio pin for blue_led_pin
            printf("\nPin: %s\n", blue_led_pin);
				
			usleep(calculate_on_time() * (int) SLEEP_PERIOD_MULTIPLIER_US);  //The off time of respective pattern
			
			}
        }
    }

    printf("Sequence thread exiting\n");
    pthread_exit(NULL);                                          
}

int calculate_on_time() {                                                //Calculates the Led on time
	float percent_value = ( (float) input_data.duty_cycle) / 100.0;      //Converts the input percent value of duty cycle
	printf("Percent value %f\n", percent_value);
	printf("Intensity control period %d\n", INTENSITY_CONTROL_PERIOD_MS);
	return (int) ((percent_value) * ((int) INTENSITY_CONTROL_PERIOD_MS)); //Returns the Calculated led on time 
}

int create_lightup_thread() {
	int err = pthread_create(&lightup_thread, NULL, &lightup_sequence, (void*) &input_data); //
	if (err != 0) {
		printf("\nCan't create lightup thread :[%s]", strerror(err));
		return err;
	}
	return 0;
}

struct task_params_struct read_input_file() {                //Structure to read the input file including duty_cycle and three IO pins connected to R,G,B Leds respectively
	struct task_params_struct params;
	char *c = NULL;
	ssize_t nread;
	size_t len = 0;
	FILE *input_file;

	input_file = fopen("input.txt", "r");                     //open the input text file
	while((nread = getline(&c,&len,input_file)) != -1) {
		params.duty_cycle = atoi(strsep(&c, ","));
		params.red_led = atoi(strsep(&c, ","));
		params.green_led = atoi(strsep(&c, ","));
		params.blue_led = atoi(strsep(&c, ","));
		printf("Duty cycle - %d\n", params.duty_cycle);
		printf("Red led is on IO%d\n", params.red_led);
		printf("Green led is on IO%d\n", params.green_led);
		printf("Blue led is on IO%d\n", params.blue_led);
	}
	fclose(input_file);                                       //close the input text file
	return params;
}


int check_valid_int(int digit) {
    return MAX_PIN_NUMBER - digit;                              //modify as per pinconfig
}


void configure_led(int gpio_array_row) {                         //Configuring R,G,B leds as per level shifter and Mux configuration
    int column;
    int columns_of_gpio_array = COLUMN_SIZE(gpio_mapping_array);

    for(column = 0; column < columns_of_gpio_array; column++) {
        char *read_value = gpio_mapping_array[gpio_array_row][column];
        if(strcmp(read_value, "X") != 0) {
            printf("\n\n Configure pins for IO %d - %s\n\n", gpio_array_row, gpio_values_array[gpio_array_row][column]);
            if(column == PIN_COLUMN)
                mux_gpio_set(gpio_mapping_array[gpio_array_row][column], "out", "-1", true);
            else if(column == LEVEL_SHIFTER_COLUMN)
                mux_gpio_set(gpio_mapping_array[gpio_array_row][column], gpio_values_array[gpio_array_row][column], "0", false);
            else if(column == MUX_1_COLUMN)
                mux_gpio_set(gpio_mapping_array[gpio_array_row][column], "out", gpio_values_array[gpio_array_row][column], false);
            else if(column == MUX_2_COLUMN)
                mux_gpio_set(gpio_mapping_array[gpio_array_row][column], "out", gpio_values_array[gpio_array_row][column], false);
        }
    }
}

void IOSetup(struct task_params_struct pin_data) { //Setting up IO to map each input IO pin to the internally connected gpio from the array
    configure_led(pin_data.red_led);
    configure_led(pin_data.green_led);
    configure_led(pin_data.blue_led);
}

void sigHandler(int signum) {                   //A signal handler that handles the signal received from the mouse event so as to immmediately stop the current running sequence and roll back to initial state
	printf("\n\nSig num - %d\n\n", signum);
	pthread_cancel(lightup_thread);             //terminates the execution of the current running sequence
    pthread_join(lightup_thread, NULL);
    create_lightup_thread();                    //creates a new thread to start the sequence from initial state
    printf("\nRestart sequence %d\n", signum);
}

int main() {
    input_data = read_input_file();
	printf("On time %d", calculate_on_time());
    if(check_valid_int(input_data.red_led) < 0 || check_valid_int(input_data.green_led) < 0 || check_valid_int(input_data.blue_led) < 0) //Checks for valid input pins
    {
        printf("Invalid pins in the input");
        goto exit;
    }

    IOSetup(input_data);

    create_mouse_thread();
    create_lightup_thread();

	sleep(60);                                   //Program will execute for 1 minute
    exit:
        exit_flag = 1;
        pthread_join(mouse_thread, NULL);        //waits for mouse thread and light_up thread to exit
        pthread_join(lightup_thread, NULL);
        printf("\n\nExiting\n\n");
		return 0;
}
