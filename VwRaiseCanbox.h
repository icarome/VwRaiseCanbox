#ifndef _VWRAISECANBOX_h
#define _VWRAISECANBOX_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

enum VwRaiseCanboxButton
{
    VolumeUp        = 0x01,
    VolumeDown      = 0x02,
    NextTrack       = 0x03,
    PreviousTrack   = 0x04,
    Source          = 0x07,
    Mute            = 0x06,
    Mode            = 0x20,
    Tel             = 0x05,
    Mic             = 0x08,
};

enum VwRaiseCanboxFunctionId
{
    CarStatusCode       = 0x41,
    AcStatusCode        = 0x21,
    ButtonCode          = 0x20,
    WheelAngleCode      = 0x26,
    ParkingStatusCode   = 0x25,
    FrontRadarCode      = 0x23,
    RearRadarCode       = 0x22,
    VersionCode         = 0x30,
};

typedef struct {
    // Bit 0
    uint8_t rear_defrost                :1;
    uint8_t front_defrost               :1;
    uint8_t ac_cool_mode_dual           :1;
    uint8_t ac_cool_mode_auto           :1;
    uint8_t ac_cool_mode_auto_2         :1;
    uint8_t cycle_mode                  :1;
    uint8_t ac_cool_mode_on             :1;
    uint8_t ac_status                   :1;
    // Bit 1
    uint8_t unused8                     :4;
    uint8_t show_ac_bar                 :1;
    uint8_t fan_direction_down          :1;
    uint8_t fan_direction_center        :1;
    uint8_t fan_direction_up            :1;
    // Bit 2
    uint8_t interior_temp_driver          ;
    // Bit 3
    uint8_t interior_temp_passenger       ;
    // Bit 4
    uint8_t heater_passenger_seat       :2;
    uint8_t ac_cool_mode_max            :1;
    uint8_t unused35                    :1;
    uint8_t heater_driver_seat          :2;
    uint8_t unused38                    :1;
    uint8_t ac_auto_in_out_cycle        :1;

} CarAcStatusStruct;

typedef struct {
    uint16_t current_rpm;
    uint16_t current_speed;
    uint8_t current_voltage;
    uint16_t current_temperature;
    uint32_t current_mileage;
    uint8_t current_fuel;
} CarStatus;

typedef struct {
    uint8_t front_left          : 1; // bit 0
    uint8_t front_right         : 1; // bit 1
    uint8_t rear_left           : 1; // bit 2
    uint8_t rear_right          : 1; // bit 3
    uint8_t trunk               : 1; // bit 4
    uint8_t hand_brake          : 1; // bit 5
    uint8_t clean_fluid_error   : 1; // bit 6
    uint8_t seat_belts_sensor   : 1; // bit 7
} DoorStatusStruct;

typedef union {
    DoorStatusStruct status;
    uint8_t asByte;
} DoorStatus;

typedef union {
    CarAcStatusStruct status;
    uint8_t bytes[5];
} CarAcStatus;


constexpr uint8_t MAX_VWRAISECANBOX_MESSAGE_LENGTH = 17;

/*
 * This is an implementation for remote controlling 8227L based Chinese Android players.
 * In order to make it work, you need to select Raise VolksWagen from the CAN Type settings.
 * This library is tied to the Android unit and not to the car's model. So you don't have to have a Volkswagen car to make this library work.
 * You can interface any car as long as you implement whatever protocol it uses.
 */
class VwRaiseCanboxRemote
{
private:
    Stream* serialPort;
    void SendData(uint8_t type, uint8_t * msg, uint8_t size);
    uint8_t CanboxChecksum(uint8_t * buf, uint8_t len);

public:
    /*
     * Serial should be an initialized Serial port with the following setup:
     * Baud      : 38400
     * Start bits: 1
     * Data bits : 8
     * Stop bits : 1
     */
    VwRaiseCanboxRemote(Stream &serial);
    ~VwRaiseCanboxRemote();

    void SendButtonCode(VwRaiseCanboxButton button);
    void ReleaseButton(VwRaiseCanboxButton button);
    void SendCarInfo(CarStatus *carStatus);
    void SendCarInfo(DoorStatus *doorStatus);
    void SendCarInfo(CarStatus *carStatus, DoorStatus *doorStatus);
    void SendCarInfo(DoorStatus *doorStatus, CarStatus *carStatus);
    void SendAcInfo(CarAcStatus *acStatus);
    void SendWheelAngle(float angle);
    void SendParkStatus(bool parked);
    void SendFrontRadar(uint8_t fl, uint8_t flm, uint8_t frm, uint8_t fr);
    void SendRearRadar(uint8_t fl, uint8_t flm, uint8_t frm, uint8_t fr);
    void SendVersion(uint8_t *ver, uint8_t len);
};

#endif
