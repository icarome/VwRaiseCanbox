#include "VwRaiseCanbox.h"

VwRaiseCanboxRemote::VwRaiseCanboxRemote(Stream& serial)
{
    serialPort = &serial;
}

void VwRaiseCanboxRemote::SendButtonCode(VwRaiseCanboxButton button)
{
    uint8_t data[] = { (uint8_t) button, 0x01 };
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
    uint8_t buf[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };

    if (acStatus->powerfull)
      buf[0] |= 0x80;
    if (acStatus->ac)
      buf[0] |= 0x40;
    if (acStatus->recycling)
      buf[0] |= 0x20;
    if (acStatus->recycling_max)
      buf[0] |= 0x10;
    if (acStatus->recycling_min)
      buf[0] |= 0x08;
    if (acStatus->dual)
      buf[0] |= 0x04;
    if (acStatus->ac_max)
      buf[0] |= 0x02;
    if (acStatus->rear)
      buf[0] |= 0x01;

    if (acStatus->wind)
      buf[1] |= 0x80;
    if (acStatus->middle)
      buf[1] |= 0x40;
    if (acStatus->floor)
      buf[1] |= 0x20;

    uint8_t speed = ((acStatus->fanspeed  * 0x07) / 0x0F);
    buf[1] |= speed & 0x07;

    if ((acStatus->l_temp % 10) == 0x05)
      buf[2] |= 0x01;
    buf[2] |= ((int)acStatus->l_temp) << 1;

    if ((acStatus->r_temp % 10) == 0x05)
      buf[3] |= 0x01;
    buf[3] |= ((int)acStatus->r_temp) << 1;

    if (acStatus->aqs)
      buf[4] |= 0x80;
    if (acStatus->rear_lock)
      buf[4] |= 0x08;
    if (acStatus->ac_max)
      buf[4] |= 0x04;

    if (acStatus->l_seat)
      buf[4] |= (acStatus->l_seat << 4) & 0x30;
    if (acStatus->r_seat)
      buf[4] |= (acStatus->r_seat) & 0x03;

    SendData(AcStatusCode, buf, sizeof(buf));
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



void VwRaiseCanboxRemote::SendData(uint8_t type, uint8_t * msg, uint8_t size) {
  uint8_t buf[4 + size];
  buf[0] = 0x2e;
  buf[1] = type;
  buf[2] = size;
  memcpy(buf + 3, msg, size);
  buf[3 + size] = canbox_checksum(buf + 1, size + 2);
  serialPort->write(buf, sizeof(buf));
}

uint8_t VwRaiseCanboxRemote::canbox_checksum(uint8_t * buf, uint8_t len) {
  uint8_t sum = 0;
  for (uint8_t i = 0; i < len; i++)
    sum += buf[i];

  sum = sum ^ 0xff;

  return sum;
}
