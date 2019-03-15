//
// Created by Smeet Somaiya on 9/30/18.
//

//Macros to read pwm ping config
#define PIN_COLUMN 0
#define LEVEL_SHIFTER_COLUMN 1
#define MUX_1_COLUMN 2
#define MUX_2_COLUMN 3

#define ROW_SIZE(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))
#define COLUMN_SIZE(arr) ((int) sizeof ((arr)[0]) / sizeof (char*))

// Sys fs directory paths
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define SYSFS_PWM_DIR "/sys/class/pwm/pwmchip0"

#define MAX_BUF 64

#define MOUSE_DEV "/dev/input/mice" //File to read mouse values

// Structures to read input from the file
struct task_params_struct
{
	float duty_cycle;
	int red_led;
	int green_led;
	int blue_led;
};
struct task_params_struct input_data;

/*
 GPIO functions
*/
int gpio_export(char *gpio);
int gpio_set_dir(char *gpio, char *out_dir);
int gpio_set_value(char *gpio, char *value);
int mux_gpio_set(char *gpio, char *out_dir, char *value, bool isPin);

/*
 PWM functions
*/
int pwm_set_duty_cycle(char pwm[MAX_BUF], char *value);
int pwm_export(char *pwm);
int pwm_set_polarity(char *pwm, char *out_dir);
int pwm_set_enable(char *pwm, char *value);
int pwm_set_period(char *value);
int mux_pwm_set(char *gpio, char *out_dir, char *value, bool isPin);
void configure_pwm(int gpio_array_row);
float calculate_duty_cycle(int left_or_right);
void modify_pwm_parameters(int left_or_right);
void IOSetup(struct task_params_struct pin_data);

pthread_t mouse_thread, lightup_thread;
/*
	Mouse thread functions
*/
void* mouse_thread_body(void *arguments);
int create_mouse_thread();
static void signal_handler_mouse(int signum);

/*
	Lightup sequence functions
*/
void* lightup_sequence();
int create_lightup_thread(struct task_params_struct input_data);

/*
 File read functions
*/
struct task_params_struct read_input_file();
static void signal_handler_lightup(int signum);
int check_valid_pin(int digit);


#define STEP_PERIOD_MS 500
#define INTENSITY_CONTROL_PERIOD_NS 20000000
#define SLEEP_PERIOD_MULTIPLIER_US 1000
#define SLEEP_PERIOD_MULTIPLIER_NS 1000000

#define DUTY_CYCLE_MAX 1
#define DUTY_CYCLE_MIN 0
#define DUTY_CYCLE_CHANGE_FACTOR 0.1
	
// Macros for identifying mouse events
#define LEFT_CLICK_VALUE 1
#define RIGHT_CLICK_VALUE 2


int step_period = 500;
float current_duty_cycle = 0;
float calculate_duty_cycle(int left_or_right);
void modify_pwm_parameters(int left_or_right);

// Array containing the pwm pin configuration
char *pwm_mapping_array[20][4] = {
	                        {"X","X","X","X"},
                            {"X","X","X","X"},
							{"X","X","X","X"},
							{"1","16","76","64"},
 							{"X","X","X","X"},
							{"3","18","66","X"},
							{"5","20","68","X"},
							{"X","X","X","X"},
							{"X","X","X","X"},
							{"7","22","70","X"},
							{"11","26","74","X"},
							{"9","24","X","72"},
							{"X","X","X","X"},
							{"X","X","X","X"},
                            {"X","X","X","X"},
                        	{"X","X","X","X"},
                            {"X","X","X","X"},
                            {"X","X","X","X"},
                            {"X","X","X","X"},
                            {"X","X","X","X"},
};

// Array containing the pwm pin configuration
char *pwm_values_array[20][4] = {
                            {"X","X","X","X"},
							{"X","X","X","X"},
							{"X","X","X","X"},
 							{"1","out","0","1"},
							{"X","X","X","X"},
							{"3","out","1","X"},
							{"5","out","1","X"},
							{"X","X","X","X"},
							{"X","X","X","X"},
							{"7","out","0","X"},
							{"11","out","1","X"},
							{"9","out","X","1"},
							{"X","X","X","X"},
                            {"X","X","X","X"},
                            {"X","X","X","X"},
                        	{"X","X","X","X"},
                            {"X","X","X","X"},
                            {"X","X","X","X"},
                            {"X","X","X","X"},
                            {"X","X","X","X"},
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