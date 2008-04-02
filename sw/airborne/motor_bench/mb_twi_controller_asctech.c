#include "mb_twi_controller.h"

#include "i2c.h"

#include "led.h"

bool_t  mb_twi_controller_asctech_command;
#define MB_TWI_CONTROLLER_COMMAND_NONE     0
#define MB_TWI_CONTROLLER_COMMAND_SET_ADDR 1
#define MB_TWI_CONTROLLER_COMMAND_TEST     2
#define MB_TWI_CONTROLLER_COMMAND_REVERSE  3
uint8_t mb_twi_controller_asctech_command_type;

#define MB_TWI_CONTROLLER_ASCTECH_ADDR_FRONT 0
#define MB_TWI_CONTROLLER_ASCTECH_ADDR_BACK  1
#define MB_TWI_CONTROLLER_ASCTECH_ADDR_LEFT  2
#define MB_TWI_CONTROLLER_ASCTECH_ADDR_RIGHT 3
uint8_t mb_twi_controller_asctech_addr;

uint8_t mb_twi_nb_overun;
uint8_t mb_twi_i2c_done;


#define MB_TWI_CONTROLLER_MAX_CMD 200
#define MB_TWI_CONTROLLER_ADDR 0x02

void mb_twi_controller_init(void) {
  mb_twi_nb_overun = 0;
  mb_twi_i2c_done = TRUE;
  mb_twi_controller_asctech_command = FALSE;
  mb_twi_controller_asctech_command_type = MB_TWI_CONTROLLER_COMMAND_NONE;
  mb_twi_controller_asctech_addr = MB_TWI_CONTROLLER_ASCTECH_ADDR_FRONT;
}

void mb_twi_controller_set( float throttle ) {
  if (mb_twi_i2c_done) {
    if (mb_twi_controller_asctech_command) {
      mb_twi_controller_asctech_command = FALSE;
      switch (mb_twi_controller_asctech_command_type) {
      case MB_TWI_CONTROLLER_COMMAND_REVERSE :
	i2c_buf[0] = 254;
	i2c_buf[1] = mb_twi_controller_asctech_addr;
	i2c_buf[2] = 0;
	i2c_buf[3] = 234 + mb_twi_controller_asctech_addr;
	i2c_transmit(MB_TWI_CONTROLLER_ADDR, 4, &mb_twi_i2c_done);
	break;
      }
    }
    else {
      uint8_t pitch = 100;
      uint8_t roll  = 100;
      uint8_t yaw   = 100;
      uint8_t power = throttle * MB_TWI_CONTROLLER_MAX_CMD;
      i2c_buf[0] = pitch;
      i2c_buf[1] = roll;
      i2c_buf[2] = yaw;
      i2c_buf[3] = power;
      i2c_transmit(MB_TWI_CONTROLLER_ADDR, 4, &mb_twi_i2c_done);
    }
  }
  else
    mb_twi_nb_overun++;
}
