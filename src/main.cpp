#include <Arduino.h>
#include "../lib/stemi-hexapod/src/PCA9685.h"

PCA9685 pwmPCA9685;
PCA9685_ServoEvaluator servo;

void gen_steps(uint16_t *ptr, int8_t angle) {
    uint16_t arr[16];
    for (uint8_t i = 0; i < 16; i++) {
        *(ptr + i) = servo.pwmForAngle(angle);
    }
}

void set_servo(int8_t angle) {
    uint16_t data[16];
    gen_steps(data, angle);

    pwmPCA9685.setChannelsPWM(0, 16, data);

    uint16_t pwm_value = (int) (((float)(angle + 90) / 180.0) * 5271.0) + 2029;

    ledcWrite(1, pwm_value);
    ledcWrite(2, pwm_value);
}

void setup() {
    Serial.begin(9600);

    pinMode(33, OUTPUT);
    digitalWrite(33, 1);

    ledcSetup(1, 50, 16);
    ledcSetup(2, 50, 16);
    ledcSetup(3, 50, 16);
    ledcAttachPin(18, 1);
    ledcAttachPin(21, 2);

    Wire.begin(23, 22);
    pwmPCA9685.resetDevices();
    pwmPCA9685.init(B000000);
    pwmPCA9685.setPWMFrequency(50);
}

void loop() {
    for (uint8_t i = 0; i < 2; i++) {
        for (int8_t angle = -90; angle < 90; angle += 2) {
            set_servo(angle);
            delay(25);
        }
    }

    set_servo(0);
    while (1) {}
}