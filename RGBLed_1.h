//
// Created by Smeet Somaiya on 9/30/18.
//

/*
Structures to read input from the file
*/
struct task_params_struct
{
	int duty_cycle;
	int red_led;
	int green_led;
	int blue_led;
};
struct task_params_struct input_data;

//Macros related to lightup sequence
#define STEP_PERIOD_MS 500 
#define INTENSITY_CONTROL_PERIOD_MS 20
#define SLEEP_PERIOD_MULTIPLIER_US 1000


void* lightup_sequence(void *arguments);
void execute_sequence(struct task_params_struct *args);
static void sigHandler(int signum);
int calculate_on_time();

// Array containing the gpio pin configuration
char *gpio_mapping_array[20][4] = {
                            {"11","32","X","X"},
							{"12","28","45","X"},
							{"61","X","77","X"},
 							{"62","16","76","64"},
							{"6","36","X","X"},
							{"0","18","66","X"},
							{"1","20","68","X"},
							{"38","X","X","X"},
							{"40","X","X","X"},
							{"4","22","70","X"},
							{"10","26","74","X"},
							{"5","24","44","72"},
							{"15","42","X","X"},
							{"7","30","46","X"},
							{"48","X","X","X"},
							{"50","X","X","X"},
							{"52","X","X","X"},
							{"54","X","X","X"},
							{"56","X","60","78"},
							{"7","30","60","79"}
};

// Array containing the gpio pin configuration
char *gpio_values_array[20][4] = {
                            {"11","out","X","X"},
							{"12","out","0","X"},
							{"61","out","0","X"},
 							{"62","out","0","0"},
							{"6","out","X","X"},
							{"0","out","0","X"},
							{"1","out","0","X"},
							{"38","X","X","X"},
							{"40","X","X","X"},
							{"4","out","0","X"},
							{"10","out","0","X"},
							{"5","out","0","0"},
							{"15","out","X","X"},
							{"7","out","0","X"},
							{"48","X","X","X"},
							{"50","X","X","X"},
							{"52","X","X","X"},
							{"54","X","X","X"},
							{"56","X","1","1"},
							{"7","X","1","1"}
};

//Array containing the sequence
char *lightup_pattern_array[8][3] = {
        {"0", "0", "0"},
        {"1", "0", "0"},
        {"0", "1", "0"},
        {"0", "0", "1"},
        {"1", "1", "0"},
        {"1", "0", "1"},
        {"0", "1", "1"},
        {"1", "1", "1"},
};

int MAX_PIN_NUMBER = 19;

#define PIN_COLUMN 0
#define LEVEL_SHIFTER_COLUMN 1
#define MUX_1_COLUMN 2
#define MUX_2_COLUMN 3

#define ROW_SIZE(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))
#define COLUMN_SIZE(arr) ((int) sizeof ((arr)[0]) / sizeof (char*))

#define SYSFS_GPIO_DIR "/sys/class/gpio"

#define MAX_BUF 64

int exit_flag = 0;

#define MOUSE_DEV "/dev/input/mice" //File to read mouse values

pthread_t mouse_thread, lightup_thread;