

const int NUM_PORTS = 1;
const int NUM_LEDS = 1;
const int IN_BUFF_LEN = 100;

void setup() {
	Serial.begin(9600);
        calibrate_voltage();
        setup_relay_output();
        setup_led_output();
}

void loop() {
	char in_buff[IN_BUFF_LEN] = {0};
	int buff_index = 0;
	int overflow = 0;
	char c;

	for(;;) {       

        // Poll the current sense resistor of each socket in the down time
        // Also poll for the switch button 
        for(int i=0;i<NUM_PORTS;i++){
            poll_current_sense(i);
            poll_relay_button(i);
        }

		// Add new characters to a buffer
		if(Serial.available()) {
			c = Serial.read();

			// If the buffer is full
			if(buff_index == IN_BUFF_LEN) {
				buff_index = 0;
				overflow = 1;
			}

			// Add new character to buffer, if it has not overflowed
			if(overflow == 0) {
				in_buff[buff_index] = c;
				buff_index++;
			}

			// Detect newline character
			if(c == '\r' || c == '\n') {
				if(overflow) {
					// Ignore this overflowed line.
					// Now that this overflowed line has ended, 
					// reset the overflow flag.
					overflow = 0;
				} else if(buff_index-1 < IN_BUFF_LEN) {
					// Replace the '\n' with null terminator
					in_buff[buff_index-1] = 0;
					// Parse this line as a command
					handle_input_line(in_buff);
				}	
				buff_index = 0;
			}
		}
	}
}
