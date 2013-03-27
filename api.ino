

const int SET_MODE_RELAY = 0;
const int SET_MODE_LED = 1;


void handle_input_line(char * line) {
	if(String(line) == String("report")) {
		_handle_report();

	} else if(String(line).startsWith("set ")) {
		_handle_set(line + 4, SET_MODE_RELAY);

	} else if(String(line).startsWith("led ")) {
		_handle_set(line + 4, SET_MODE_LED);
	}
}

void _handle_report() {
	Serial.print("{");
	Serial.print("\"success\":true,");
	for(int i = 0; i < NUM_PORTS; i++) {
                Serial.print("\"");
		Serial.print(i);
		Serial.print("\":{");

		Serial.print("\"state\":");
		Serial.print(get_relay(i));
		Serial.print(",");

		Serial.print("\"power\":");
		Serial.print(measure_power(i));
		Serial.print(",");

		Serial.print("\"led\":");
		Serial.print(get_led(i));

		Serial.print("}");
		if(i < NUM_PORTS-1) {
			Serial.print(",");
		}
	}

	Serial.println("}");
}

int _handle_set(char * pairs, int mode) {
	int tok_end = 0;
	int status = 0;
	
	for(int i = 0; i < IN_BUFF_LEN; i++) {
		if(pairs[tok_end] == 0) {
			status += _handle_pair(pairs, mode);
			break;
		} else if(pairs[tok_end] == ' ') {
			pairs[tok_end] = 0;
			status += _handle_pair(pairs, mode);
			pairs = pairs + tok_end+1;
			tok_end = 0;
		} else {
			tok_end++;
		}
	}
	
	if(status != 0) {
		Serial.print("{\"success\":false,\"message\":null}\n");
		return 1;
	}

	Serial.print("{\"success\":true,\"message\":null}\n");
	return 0;
}

int _handle_pair(char * pair, int mode) {
	int socket;
	int state;
	int status = 0;
	
	for(int i = 0; i < strlen(pair); i++) {
		if(pair[i] == ':') {
			pair[i] = 0;
			socket = atoi(pair);
			state = atoi(pair + i+1);

			switch(mode) {
			case SET_MODE_RELAY:
				status += set_relay(socket, state);
				break;
			case SET_MODE_LED:
				status += set_led(socket, state);
				break;
			}
		}
	}
	
	if(status != 0) {
		return 1;
	}
	
	return 0;
}
