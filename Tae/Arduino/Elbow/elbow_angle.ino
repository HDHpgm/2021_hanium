#include <MPU9250.h>

MPU9250 mpu;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    delay(2000);

    if (!mpu.setup(0x68)) {  // change to your own address
        while (1) {
            Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
            delay(5000);
        }
    }
}

void loop() {
    if (mpu.update()) {
        static uint32_t prev_ms = millis();
        if (millis() > prev_ms + 25) {
            print_roll_pitch_yaw();
            prev_ms = millis();
        }
    }
}

void print_roll_pitch_yaw() {
    Serial.print("Pitch: ");
    Serial.println((int)mpu.getPitch());
    Serial.print("us: ");
    int ra = constrain(map((int)mpu.getPitch(), 0, 90, 596, 1369), 596, 1369);
    Serial.println(ra);


}
