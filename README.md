# 8227L Android Head unit canbox protocol

### What is it ?

A library to interface bewtween arduino and android head units based on MTK 8227L

### Why ?

Because it is fun. But other than that if you forgot to order the CAN bus adapter, or you are looking for a DIY project I am here to help. You don't need to have a Volkswagen car for this library, as it is written for the headunit. You can interface any car as long as you implement whatever protocol it uses.  For example you may have a Ford car where you implement the CAN bus parsing, but you use this library telling the headunit that you have a Volksvagen.

### The protocol

When you select CAN Type **Volkswagen Raise** from the settings the headunit will use a **38400** baud serial port with a 8 databits 1 stop bit and no parity to communicate with the "CAN box". It uses a simple protocol to send commands.

    0x2E 0x01 0x02 0x01 0x01 0xDB
     │    │    │   └───┼───┘  └────── checksum (0xFF - sum of the previous bytes excluding the header, can overflow)
     │    │    │       └───────────── data bytes
     │    │    └───────────────────── length of the payload
     │    └────────────────────────── function id
     └─────────────────────────────── header

A basic sketch with VwRaiseCanboxRemote is like this:

    #include <VwRaiseCanbox.h>

    #define RXD2 13
    #define TXD2 15

    VwRaiseCanboxRemote* remote;
    SoftwareSerial SoftSerial;
    DoorStatus ds;
    CarStatus cs;
    CarAcStatus ac_state;

    void setup() {
        Serial.begin(115200);
        SoftSerial.begin(38400, SWSERIAL_8N1, RXD2, TXD2);

        remote = new VwRaiseCanboxRemote(SoftSerial);
        ds.status.hand_brake = 1;
    }

    void loop() {
        uint8_t inChar = (uint8_t)Serial.read();
            if (inChar == '0') {
                Serial.println("Source");
                remote->SendButtonCode(Source);
            }
            if (inChar == '1') {
                Serial.println("Next track");
                remote->SendButtonCode(NextTrack);
            }
            if (inChar == '2') {
                Serial.println("Previous track");
                remote->SendButtonCode(PreviousTrack);
            }
            if (inChar == '3') {
                Serial.println("Volume up");
                remote->SendButtonCode(VolumeUp);
            }
            if (inChar == '4') {
                Serial.println("Volume down");
                remote->SendButtonCode(VolumeDown);
            }
            if (inChar == '5') {
                Serial.println("Mute");
                remote->SendButtonCode(Mute);
            }
            if (inChar == '6') {
                Serial.println("TripButton");
                remote->SendButtonCode(TripComputer);
            }
            if (inChar == 'q') {
                Serial.println("Front right door open");
                ds.status.front_right = 1;
                remote->SendCarInfo(&ds);
            }
            if (inChar == 'e') {
                Serial.println("Front left door open");
                ds.status.front_left = 1;
                remote->SendCarInfo(&ds);
            }
            if (inChar == 'r') {
                Serial.println("Rear right door open");

                ds.status.rear_right = 1;
                remote->SendCarInfo(&ds);
            }
            if (inChar == 't') {
                Serial.println("Rear Left door open");

                ds.status.rear_left = 1;
                remote->SendCarInfo(&ds);
            }
            if (inChar == 'k') {
                Serial.println("Trunk door open");

                ds.status.trunk = 1;
                remote->SendCarInfo(&ds);
            }
            if (inChar == 'l') {
                Serial.println("Toggle Handbrake");

                ds.status.hand_brake = !ds.status.hand_brake;
                remote->SendCarInfo(&ds);
            }
            if (inChar == 'z') {
                Serial.println("Toggle Clean Fluid Notification");

                ds.status.clean_fluid_error = !ds.status.clean_fluid_error;
                remote->SendCarInfo(&ds);
            }
            if (inChar == 'c') {
                Serial.println("Toggle Seat Belts Sensor");

                ds.status.seat_belts_sensor = !ds.status.seat_belts_sensor;
                remote->SendCarInfo(&ds);
            }
            if (inChar == 'n') {
                Serial.println("Sendcar Info");
                cs.current_rpm = 1500; // Rotations per minute
                cs.current_speed = 120; // Km/h
                cs.current_voltage = 14; // Volts
                cs.current_temperature = 18; // External Temperature in Celsius
                cs.current_mileage = 115467; // Car Mileage in Km
                cs.current_fuel = 40; // Fuel level in Liters

                remote->SendCarInfo(&cs);
            }
    }

### Schema

The schema and the example uses an ESP32 devboard, but the library should work with other Arduino compatible boards as well.

![schema](https://github.com/morcibacsi/JunsunPSARemote/raw/main/extras/schema.jpg)

### Components

The components needed for the project to work in your car is the following:
- The Android head unit
- An ESP8266/ESP8266 dev board

### Liability

I cannot take any responsibility if something goes wrong if you build it and install it in your car. So use it at your own risk.

### Based On

This work was built on top of existing projects such as:
- https://github.com/morcibacsi/JunsunPSARemote
- https://github.com/smartgauges/canbox
