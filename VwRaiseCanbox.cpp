#include "VwRaiseCanbox.h"

VwRaiseCanboxRemote::VwRaiseCanboxRemote(Stream& serial)
{
    serialPort = &serial;
}

void VwRaiseCanboxRemote::SendVersion(uint8_t *ver, uint8_t len)
{
    SendData(VersionCode, ver, len);
}

void VwRaiseCanboxRemote::ReleaseButton(VwRaiseCanboxButton button)
{
    uint8_t data[] = { (uint8_t) button, 0x00 };
    SendData(ButtonCode, data, sizeof(data));
}

void VwRaiseCanboxRemote::SendButtonCode(VwRaiseCanboxButton button)
{
    uint8_t data[] = { (uint8_t) button, 0x01 };
    SendData(ButtonCode, data, sizeof(data));
}

void VwRaiseCanboxRemote::SendCarInfo(CarStatus* carStatus)
{
    uint8_t t1 = (carStatus->current_rpm >> 8) & 0xff;
    uint8_t t2 = carStatus->current_rpm & 0xff;
    uint16_t car_speed = carStatus->current_speed * 100;
    uint8_t t3 = (car_speed >> 8) & 0xff;
    uint8_t t4 = car_speed & 0xff;
    uint16_t voltage = carStatus->current_voltage * 100;
    uint8_t t5 = (voltage >> 8) & 0xff;
    uint8_t t6 = voltage & 0xff;
    uint16_t temp = carStatus->current_temperature * 10;
    uint8_t t7 = (temp >> 8) & 0xff;
    uint8_t t8 = temp & 0xff;
    uint8_t t9 = (carStatus->current_mileage >> 16) & 0xff;
    uint8_t t10 = (carStatus->current_mileage >> 8) & 0xff;
    uint8_t t11 = carStatus->current_mileage & 0xff;
    uint8_t t12 = carStatus->current_fuel;

    uint8_t buf[13] = { 0x02, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12 };

    SendData(CarStatusCode, buf, sizeof(buf));
}

void VwRaiseCanboxRemote::SendAcInfo(CarAcStatus* acStatus)
{
    SendData(AcStatusCode, acStatus->bytes, sizeof(acStatus->bytes));
}

void VwRaiseCanboxRemote::SendCarInfo(DoorStatus* doorStatus) {
    uint8_t buf[] = { 0x01, doorStatus->asByte };

    SendData(CarStatusCode, buf, sizeof(buf));
}

void VwRaiseCanboxRemote::SendCarInfo(CarStatus* carStatus, DoorStatus* DoorStatus) {
    SendCarInfo(carStatus);
    SendCarInfo(DoorStatus);
}

void VwRaiseCanboxRemote::SendCarInfo(DoorStatus* DoorStatus, CarStatus* carStatus) {
    SendCarInfo(carStatus);
    SendCarInfo(DoorStatus);
}

void SendWheelAngle(float angle) {
    uint8_t buf[] = { (uint16_t) angle, (uint16_t) angle >> 8 };

    SendData(WheelAngleCode, buf, sizeof(buf));
}

void SendParkStatus(bool parked) {
    uint8_t buf = parked ? 0x02 : 0x00;

    SendData(ParkingStatusCode, &buf, sizeof(buf));
}

void SendRearRadar(uint8_t rl, uint8_t rlm, uint8_t rrm, uint8_t rr) {
    uint8_t buf[] = { 0x00, 0x00, 0x00, 0x00 };
	buf[0] = 11 - rl;
	buf[1] = 11 - rlm;
	buf[2] = 11 - rrm;
	buf[3] = 11 - rr;

    SendData(RearRadarCode, buf, sizeof(buf));
}

void SendFrontRadar(uint8_t fl, uint8_t flm, uint8_t frm, uint8_t fr) {
    uint8_t buf[] = { 0x00, 0x00, 0x00, 0x00 };
	buf[0] = 11 - fl;
	buf[1] = 11 - flm;
	buf[2] = 11 - frm;
	buf[3] = 11 - fr;

    SendData(FrontRadarCode, buf, sizeof(buf));
}


void VwRaiseCanboxRemote::SendData(uint8_t type, uint8_t * msg, uint8_t size) {
  uint8_t buf[4 + size];
  buf[0] = 0x2e;
  buf[1] = type;
  buf[2] = size;
  memcpy(buf + 3, msg, size);
  buf[3 + size] = CanboxChecksum(buf + 1, size + 2);
  serialPort->write(buf, sizeof(buf));
}

uint8_t VwRaiseCanboxRemote::CanboxChecksum(uint8_t * buf, uint8_t len) {
  uint8_t sum = 0;
  for (uint8_t i = 0; i < len; i++)
    sum += buf[i];

  sum = sum ^ 0xff;

  return sum;
}
