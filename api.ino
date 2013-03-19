

const int SET_MODE_RELAY = 0;
const int SET_MODE_LED = 1;


void handle_input_line(char * line) {
	Serial.println("handle_input_line");

	if(String(line) == String("report")) {
		_handle_report();

	} else if(String(line).startsWith("set ")) {
		_handle_set(line + 4, SET_MODE_RELAY);

	} else if(String(line).startsWith("led ")) {
		_handle_set(line + 4, SET_MODE_LED);
	}
}

void _handle_report() {
	// TODO
	Serial.println("_handle_report is not yet implemented");
}

int _handle_set(char * pairs, int mode) {
	Serial.println("_handle_set");
	//Serial.println(pairs);
	int tok_end = 0;

	for(int i = 0; i < IN_BUFF_LEN; i++) {
		if(pairs[tok_end] == 0) {
			_handle_pair(pairs, mode);
			break;
		} else if(pairs[tok_end] == ' ') {
			pairs[tok_end] = 0;
			_handle_pair(pairs, mode);
			pairs = pairs + tok_end+1;
			tok_end = 0;
		} else {
			tok_end++;
		}
	}

	return 0;
}

int _handle_pair(char * pair, int mode) {
	Serial.println("_handle_pair");
	//Serial.println(pair);
	int socket;
	int state;
	for(int i = 0; i < strlen(pair); i++) {
		if(pair[i] == ':') {
			pair[i] = 0;
			socket = atoi(pair);
			state = atoi(pair + i+1);

			switch(mode) {
			case SET_MODE_RELAY:
				set_relay(socket, state);
				break;
			case SET_MODE_LED:
				set_led(socket, state);
				break;
			}
		}
	}
	
	return 0;
}