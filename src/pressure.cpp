#include <Arduino.h>
#include <secrets.h>
#include <telnet.h>
#include <config.h>
#include <net.h>

#define IN_PIN A0
#define PING_TIME 1000
#define LOG_TIME 5000

namespace Pressure {
	void init() {
		pinMode(IN_PIN, INPUT);
	}

	void send_pressure(int pressure) {
		char* path = (char*) malloc(sizeof(char) * 40);
		sprintf(path, "/pressure/%s/%d", KEY, pressure);
		Net::req_auth(BASE_NAME, 80, path);
		free(path);
	}

	unsigned int last_run = millis();
	unsigned int last_log = millis();
	void loop() {
		if (millis() - last_run >= PING_TIME) {
			last_run = millis();

			int pressure_read = analogRead(IN_PIN);
			send_pressure(pressure_read);

			if (millis() - last_log >= LOG_TIME) {
				LOGF("Pressure is %d\n", pressure_read);
				last_log = millis();
			}
		}
	}
}