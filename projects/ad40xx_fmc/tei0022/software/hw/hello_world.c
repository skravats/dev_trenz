/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include "system.h"
#include "altera_avalon_i2c.h"

int main()
{

	ALT_AVALON_I2C_DEV_t *i2c_dev;
	alt_u8 txbuffer[0x200];
	alt_u8 rxbuffer[0x200];
	int i;
	ALT_AVALON_I2C_STATUS_CODE status;

	//get a pointer to the avalon i2c instance
	i2c_dev = alt_avalon_i2c_open("/dev/hdmi_i2c");
	if (NULL==i2c_dev)
	{
		printf("Error: Cannot find /dev/i2c_0\n");
		return 1;
	}

	//set the address of the i2c slave device

	alt_avalon_i2c_master_target_set(i2c_dev,0x39);


	txbuffer[0]=0x41; txbuffer[1]=0x10;
	status = alt_avalon_i2c_master_tx(i2c_dev,txbuffer, 0x2, ALT_AVALON_I2C_NO_INTERRUPTS);
	if (status!=ALT_AVALON_I2C_SUCCESS)
		printf("write transaction failed\n");
	
	txbuffer[0]=0x98; txbuffer[1]=0x03;
	alt_avalon_i2c_master_tx(i2c_dev,txbuffer, 0x2, ALT_AVALON_I2C_NO_INTERRUPTS);
	
	txbuffer[0]=0x9A; txbuffer[1]=0xE0;
	alt_avalon_i2c_master_tx(i2c_dev,txbuffer, 0x2, ALT_AVALON_I2C_NO_INTERRUPTS);	
	
	txbuffer[0]=0x9C; txbuffer[1]=0x30;
	alt_avalon_i2c_master_tx(i2c_dev,txbuffer, 0x2, ALT_AVALON_I2C_NO_INTERRUPTS);
	
	txbuffer[0]=0x9D; txbuffer[1]=0x01;
	alt_avalon_i2c_master_tx(i2c_dev,txbuffer, 0x2, ALT_AVALON_I2C_NO_INTERRUPTS);	

	txbuffer[0]=0xA2; txbuffer[1]=0xA4;
	alt_avalon_i2c_master_tx(i2c_dev,txbuffer, 0x2, ALT_AVALON_I2C_NO_INTERRUPTS);
	
	txbuffer[0]=0xA3; txbuffer[1]=0xA4;
	alt_avalon_i2c_master_tx(i2c_dev,txbuffer, 0x2, ALT_AVALON_I2C_NO_INTERRUPTS);

	txbuffer[0]=0xE0; txbuffer[1]=0xD0;
	alt_avalon_i2c_master_tx(i2c_dev,txbuffer, 0x2, ALT_AVALON_I2C_NO_INTERRUPTS);
	
	txbuffer[0]=0xF9; txbuffer[1]=0x00; 
	alt_avalon_i2c_master_tx(i2c_dev,txbuffer, 0x2, ALT_AVALON_I2C_NO_INTERRUPTS);	

	txbuffer[0]=0x00; txbuffer[1]=0x15; txbuffer[2]=0x05;
	alt_avalon_i2c_master_tx(i2c_dev,txbuffer, 0x3, ALT_AVALON_I2C_NO_INTERRUPTS);
	
	txbuffer[0]=0x16; txbuffer[1]=0x34;
	alt_avalon_i2c_master_tx(i2c_dev,txbuffer, 0x2, ALT_AVALON_I2C_NO_INTERRUPTS);	
	
	txbuffer[0]=0x17; txbuffer[1]=0x02;
	alt_avalon_i2c_master_tx(i2c_dev,txbuffer, 0x2, ALT_AVALON_I2C_NO_INTERRUPTS);		

	txbuffer[0]=0x18; txbuffer[1]=0x00;
	alt_avalon_i2c_master_tx(i2c_dev,txbuffer, 0x2, ALT_AVALON_I2C_NO_INTERRUPTS);

	txbuffer[0]=0xAF; txbuffer[1]=0x02;
	alt_avalon_i2c_master_tx(i2c_dev,txbuffer, 0x2, ALT_AVALON_I2C_NO_INTERRUPTS);	

	txbuffer[0]=0x4C; txbuffer[1]=0x04;
	alt_avalon_i2c_master_tx(i2c_dev,txbuffer, 0x2, ALT_AVALON_I2C_NO_INTERRUPTS);

	txbuffer[0]=0x0;

	//read back the data into rxbuffer
	//This command sends the 2 byte eeprom data address required by the eeprom
	//Then does a restart and receives the data.
	status=alt_avalon_i2c_master_tx_rx(i2c_dev, txbuffer, 1, rxbuffer, 0xff, ALT_AVALON_I2C_NO_INTERRUPTS);
	//status=alt_avalon_i2c_master_rx(i2c_dev, rxbuffer, 0xff, ALT_AVALON_I2C_NO_INTERRUPTS);

	for (i=0;i<0xff;i++) {
		printf("RX[%x] = %x\n",i,rxbuffer[i]);
	}
	if (status!=ALT_AVALON_I2C_SUCCESS) return 1; //FAIL
	return 0;
}

