
const float current_sense_coef = 1.0;
const unsigned int poll_length = 256;
const int analog_current_sense_pin = A0;

unsigned long accumulator[NUM_PORTS];
unsigned int step_num[NUM_PORTS];
float voltage_correction_factor = 1.0;

int get_relay(int socket) {
	// TODO
	Serial.print("get_relay not implemented. ");
	Serial.println(socket);
	return 0;
}

int set_relay(int socket, int status) {
	// TODO
	Serial.print("set_relay not implemented. ");
	Serial.print(socket);
	Serial.print(" ");
	Serial.println(status);
	return 0;
}

int get_led(int socket) {
	// TODO
	Serial.print("get_led not implemented. ");
	Serial.println(socket);
	return 0;
}

int set_led(int socket, int status) {
	// TODO
	Serial.print("set_led not implemented. ");
	Serial.print(socket);
	Serial.print(" ");
	Serial.println(status);
	return 0;
}

float measure_power(int socket) {
    float power;
    power = accumulator[socket]/step_num[socket];	
    accumulator[socket] = 0; //reset the accumulator
    step_num[socket] = 0;
	return power;
}

void calibrate_voltage() {
    //TODO: get voltage from analog pin 1 and set voltage correction gain
    voltage_correction_factor = 1.0;        
}

void poll_current_sense(int socket) {
    //TODO: set mux chip to correct socket (0-3)

    for (int i=0; i<poll_length; i++) //poll the socket for enough time to capture a few periods
    {
           if (step_num[socket] != 65536) {
               accumulator[socket] += analogRead(analog_current_sense_pin); //read new analog value
               step_num[socket] += 1;
           } else { //accumulator has reached step limit, reset
               step_num[socket] = 0;
               accumulator[socket]=0;
               return;
           }
    }           
}
