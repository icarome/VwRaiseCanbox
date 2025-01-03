#include <VwRaiseCanbox.h>
#include <SoftwareSerial.h>

#define RXD2 13
#define TXD2 15

VwRaiseCanboxRemote* remote;
SoftwareSerial SoftSerial;
DoorStatus ds;
CarStatus cs;
CarAcStatus ac_state;

void setup()
{
    Serial.begin(115200);
    SoftSerial.begin(38400, SWSERIAL_8N1, RXD2, TXD2);

    remote = new VwRaiseCanboxRemote(SoftSerial);
    ds.status.hand_brake = 1;
}

void loop()
{
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

    if (inChar == 'w') {
        Serial.println("Close all doors");

        ds.status.front_left = 0;
        ds.status.front_right = 0;
        ds.status.rear_right = 0;
        ds.status.rear_left = 0;
        ds.status.clean_fluid_error = 0;
        ds.status.seat_belts_sensor = 0;
        ds.status.hand_brake = 1;
        ds.status.trunk = 0;


        remote->SendCarInfo(&ds);
    }

    if (inChar == '[') {
        Serial.println("Toggle AC State");
        ac_state.powerfull = !ac_state.powerfull;
        remote->SendAcInfo(&ac_state);
    }

    if (inChar == 'f') {
        Serial.println("Toggle AC Wind State");
        ac_state.wind = !ac_state.wind;
        remote->SendAcInfo(&ac_state);
    }
    if (inChar == 'g') {
        Serial.println("Toggle AC Middle State");
        ac_state.middle = !ac_state.middle;
        remote->SendAcInfo(&ac_state);
    }
    if (inChar == 'h') {
        Serial.println("Toggle AC Floor State");
        ac_state.floor = !ac_state.floor;
        remote->SendAcInfo(&ac_state);
    }
    if (inChar == ']') {
        Serial.println("Set AC Temp State");
        ac_state.l_temp = 16;
        ac_state.r_temp = 15;
        remote->SendAcInfo(&ac_state);
    }

    if (inChar == 'p') {
        Serial.println("Reset AC State");

        ac_state.fanspeed = 0x00;
        ac_state.powerfull = 0;
        ac_state.ac = 0;
        ac_state.recycling = 0;
        ac_state.rear = 0;
        ac_state.wind = 0;
        ac_state.middle = 0;
        ac_state.floor = 0;
        ac_state.l_temp = 0;
        ac_state.r_temp = 0;
        ac_state.l_seat = 0;
        ac_state.r_seat = 0;
        ac_state.ac_max = 0;
        ac_state.rear_lock = 0;
        ac_state.aqs = 0;
        ac_state.recycling_max = 0;
        ac_state.recycling_min = 0;



        remote->SendAcInfo(&ac_state);
    }
    if (inChar == 'b') {
        Serial.println("Parking true");
        remote->SendParkStatus(true);
    }
    if (inChar == 'v') {
        Serial.println("Parking false");
        remote->SendParkStatus(false);
    }
    if (inChar == 's') {
        Serial.println("Wheel Angle -50");
        remote->SendWheelAngle(-50); // values Should be between -100 and 100
    }
    if (inChar == '+') {
        Serial.println("Wheel Angle 50");
        remote->SendWheelAngle(50); // values Should be between -100 and 100
    }
    if (inChar == '/') {
        Serial.println("Send Radar");
        // values Should be between 0 and 10 for each area
        // params are: left, left middle, right middle and right
        remote->SendFrontRadar(3,2,2,3);
        remote->SendRearRadar(5,7,7,5);
    }
}
