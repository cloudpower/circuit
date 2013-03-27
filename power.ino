
const float current_sense_coef = 1.0;
const unsigned int poll_length = 5;
const int analog_current_sense_pin = A0;

unsigned long accumulator[NUM_PORTS];
unsigned int step_num[NUM_PORTS];
float power_correction_factor = -20;

const int relay_control_pin[NUM_PORTS] = {5};
const int relay_led_pin[NUM_PORTS] = {3};
const int led_control_pin[NUM_LEDS] = {10};
const int socket_button_pin[NUM_PORTS] = {4};

byte relay_status[NUM_PORTS];
byte led_status[NUM_PORTS];

long last_debounce_time[NUM_PORTS] = {0};  // the last time the output pin was toggled
int last_button_state[NUM_PORTS] = {LOW};
const long debounceDelay = 50; 

void setup_relay_output() {
       for (int i=0; i<NUM_PORTS; i++) {
              pinMode(relay_control_pin[i], OUTPUT);
              pinMode(relay_led_pin[i],OUTPUT);
              pinMode(socket_button_pin[i], INPUT);
              digitalWrite(relay_control_pin[i], LOW);
              digitalWrite(relay_led_pin[i],LOW);
              relay_status[i] = 0; 
       } 
}
void setup_led_output() {
       for (int i=0; i<NUM_PORTS; i++) {
              pinMode(led_control_pin[i], OUTPUT);
              digitalWrite(led_control_pin[i], LOW);
              led_status[i] = 0; 
       }  
}

int get_relay(int socket) {
        if (socket >= NUM_PORTS) {
              return -1;
        } 
	return relay_status[socket];
}

int set_relay(int socket, int status) {
        if (socket >= NUM_PORTS) {
              return -1;
        }
        if (status == 0) {
             digitalWrite(relay_control_pin[socket],LOW);
             digitalWrite(relay_led_pin[socket],LOW);
             relay_status[socket] = 0; 
        } else {
             digitalWrite(relay_control_pin[socket],HIGH);
             digitalWrite(relay_led_pin[socket],HIGH);
             relay_status[socket] = 1;
        }
	return 0;
}

int get_led(int led) {
	if (led >= NUM_LEDS) {
              return -1;
        } 
	return led_status[led];
}

int set_led(int led, int status) {
	if (led >= NUM_LEDS) {
              return -1;
        }
        if (status == 0) {
             digitalWrite(led_control_pin[led],LOW);
             led_status[led] = 0; 
        } else {
             digitalWrite(led_control_pin[led],HIGH);
             led_status[led] = 1;
        }
	return 0;
}

float measure_power(int socket) {
    float power;
    power = accumulator[socket]/step_num[socket];
    power = 512 - power; //move down half
    power = power*power_correction_factor;	
    accumulator[socket] = 0; //reset the accumulator
    step_num[socket] = 0;
    return power;
}

void calibrate_voltage() {
    //TODO: get voltage from analog pin 1 and set voltage correction gain        
}

void poll_current_sense(int socket) {
    //TODO: set mux chip to correct socket (0-3)

    for (int i=0; i<poll_length; i++) //poll the socket for enough time to capture a few periods
    {
           if (step_num[socket] != 65536) {
               accumulator[socket] += analogRead(analog_current_sense_pin); //read new analog value
               step_num[socket] += 1;
               delay(2);
           } else { //accumulator has reached step limit, reset
               step_num[socket] = 0;
               accumulator[socket]=0;
               return;
           }
    }           
}

void poll_relay_button(int socket) {
      int state = digitalRead(socket_button_pin[socket]);
      if (state == HIGH && last_button_state[socket] == LOW && millis() - last_debounce_time[socket] > debounceDelay) {
        if (relay_status[socket] == 1) {
              set_relay(socket,0);
        } else {
              set_relay(socket,1);
        }   
        last_debounce_time[socket] = millis();  
      }
      last_button_state[socket] = state;
}
