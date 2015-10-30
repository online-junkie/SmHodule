
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */


#include "c_types.h"
#include "user_interface.h"
#include "espconn.h"
#include "mem.h"
#include "osapi.h"
#include "gpio.h"
#include "io/gpio16.h"
#include "wifi/wifi.h"
#include "espmissingincludes.h"
#include "smhodule.h"
#include "io/io.h"
#include "i2c/i2c.h"
#include "i2c/i2c_oled.h"
#include "i2c/i2c_bmp180.h"

#define BTNDOWNGPIO 15
#define BTNUPGPIO 13
#define BTNRIGHTGPIO 2
#define BTNLEFTGPIO 0

static volatile bool OLED;
static ETSTimer resetBtnDwntimer;
static ETSTimer resetBtnUptimer;
static ETSTimer resetBtnLefttimer;
static ETSTimer resetBtnRighttimer;
static int fontsize = 2;
uint32 stime0 = 0, stime1 = 0;
static uint16_t gpio13_counter = 0;                        // gpio13 counter

void ICACHE_FLASH_ATTR ioGPIO16(int ena) {
	if (ena) {
		gpio16_output_set(1);
		if ( atoi(Setup.OLED) == 1 )
			OLED_Print(1,(1*fontsize),"GPIO16:on ",fontsize); // x,y,text,fontsize -> y need to be 2*fontsize
	} else {
		gpio16_output_set(0);
		if ( atoi(Setup.OLED) == 1 )
			OLED_Print(1,(1*fontsize),"GPIO16:off",fontsize);
	}
}

static void ICACHE_FLASH_ATTR resetBtnUpTimerCb(void *arg) {
	static int resetCnt=0;
	if (!GPIO_INPUT_GET(BTNUPGPIO)) {
		resetCnt++;
		os_printf("UP Cnt: %d\n", resetCnt);
	} else {
		if (resetCnt>=3 ) { //>200msec pressed
			sendKeyData(2,resetCnt,1); // long
			os_printf("Key UP pressed long\n");
		} else if (resetCnt>=1 && resetCnt <=2 ) { //<200msec pressed
			sendKeyData(2,0,1); //short
			os_printf("Key UP pressed short\n");
		}
		resetCnt=0;
	}
}

static void ICACHE_FLASH_ATTR resetBtnLeftTimerCb(void *arg) {
	static int resetCnt=0;
	if (!GPIO_INPUT_GET(BTNLEFTGPIO)) {
		resetCnt++;
		os_printf("Left cnt: %d\n", resetCnt);
	} else {
		if (resetCnt > 40) { //>8 sec pressed and clear config
			setup_wifi_ap_mode();
			initSetupData();
		} else if (resetCnt>=3 && resetCnt<=40) { //<8 sec pressed
			sendKeyData(4,resetCnt,2); // long
			os_printf("Key Left pressed long\n");
		} else if (resetCnt>=1 && resetCnt <=2 ) { //<200msec pressed
			sendKeyData(4,0,2); //short
			os_printf("Key Left pressed short\n");
		}
		resetCnt=0;
	}
}

static void ICACHE_FLASH_ATTR resetBtnRightTimerCb(void *arg) {
	static int resetCnt=0;
	if (!GPIO_INPUT_GET(BTNRIGHTGPIO)) {
		resetCnt++;
		os_printf("Right cnt: %d\n", resetCnt);
	} else {
		if (resetCnt>=3) { //>200msec pressed
			sendKeyData(3,resetCnt,3); // long
			os_printf("Key RIGHT pressed long\n");
		} else if (resetCnt>=1 && resetCnt <=2 ) { //<200msec pressed
			sendKeyData(3,0,3); //short
			os_printf("Key RIGHT pressed short\n");
		}
		resetCnt=0;
	}
}

static void ICACHE_FLASH_ATTR resetBtnDwnTimerCb(void *arg) {
	static int resetCnt=0;
	if (!GPIO_INPUT_GET(BTNDOWNGPIO)) {
		resetCnt++;
		os_printf("DOWN resetCnt: %d\n", resetCnt);
	} else {
		if (resetCnt>=3) { //>=200msec pressed
			sendKeyData(1,resetCnt,0); // long
			os_printf("Key DOWN pressed long\n");
		} else if (resetCnt>=1 && resetCnt <=2 ) { //<200 msec pressed
			sendKeyData(1,0,0); //short
			os_printf("Key DOWN pressed short\n");
		}
		resetCnt=0;
	}
}

static void ICACHE_FLASH_ATTR gpio13_int_handler(int8_t key) // key not right used
{
	ETS_GPIO_INTR_DISABLE();  										// Disable gpio interrupts
	// Increment counter
    gpio13_counter++;
    //Not that sure what this does yet and where the register is used for
    uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
    //clear interrupt status
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(key));
    ETS_GPIO_INTR_ENABLE();                                         // Enable gpio interrupts
}

void ICACHE_FLASH_ATTR gpio13_int_prepare(void)
{
	// prepare GPIO13 to handle interrupts
	ETS_GPIO_INTR_DISABLE();  										// Disable gpio interrupts
	ETS_GPIO_INTR_ATTACH(gpio13_int_handler, 13);                   // GPIO13 interrupt handler
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);            // Set GPIO13 function
	gpio_output_set(0, 0, 0, GPIO_ID_PIN(13));                       // Set GPIO13 as input
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(13));               // Clear GPIO13 status
	gpio_pin_intr_state_set(GPIO_ID_PIN(13), GPIO_PIN_INTR_POSEDGE); // Interrupt on GPIO13
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_MTCK_U);                          // Disabe pullup
	ETS_GPIO_INTR_ENABLE();                                         // Enable gpio interrupts
}

uint16_t ICACHE_FLASH_ATTR getGpio13Counter(void) {
	return gpio13_counter;
}

void ICACHE_FLASH_ATTR resetGpio13Counter(void) {
	gpio13_counter = 0;
}

void ioInit() {
	bool isInitialized = 0;
	gpio_init();
	// validate if possible because during boot GPIO0 and GPIO15 need to be low level

    // 12 and 14 should be used for I2C bus
	// this are the two pins we're going to use
    // gpio12 (sda), 14 (scl) are used as i2c
	if ( atoi(Setup.OLED) == 1 ) {
		i2c_init();
		isInitialized = 1;
		OLED = OLED_Init();
		OLED_Print(0, 0, getSmHoduleName(), fontsize);
	}
	if ( atoi(Setup.BMP180) == 1 ) {
		if ( !isInitialized)
			i2c_init();
		BMP180_Init();
	}
	// dangerous, use only if gpio16 is not bridged with rst
	// use this module as an actor
	if ( atoi((uint8_t *) Setup.Type) >= 3 ) {
		// SmHodule is used as an actor -> GPIO16
		gpio16_output_conf();
		gpio16_output_set(0);
	} else if ( atoi((uint8_t *) Setup.Type) == 2 ) {
		// SmHodule is used as a sender...
		//gpio16_output_set(1);

		// Key Right GPIO2
		PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
		os_timer_disarm(&resetBtnRighttimer);
		os_timer_setfn(&resetBtnRighttimer, resetBtnRightTimerCb, NULL);
		os_timer_arm(&resetBtnRighttimer, 100, 1);

		// Key Up GPIO13
		PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);
		os_timer_disarm(&resetBtnUptimer);
		os_timer_setfn(&resetBtnUptimer, resetBtnUpTimerCb, NULL);
		os_timer_arm(&resetBtnUptimer, 100, 1);

		// Key Left GPIO0
		PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
		os_timer_disarm(&resetBtnLefttimer);
		os_timer_setfn(&resetBtnLefttimer, resetBtnLeftTimerCb, NULL);
		os_timer_arm(&resetBtnLefttimer, 100, 1);
	} else {
		// SmHodule is used as a sensor
		// if used as a sensor, then gpio13 is used to count events
		// basically a 10sec measurement and then value is send to host
		gpio13_int_prepare();  // for interrupt
	}

	// By default GPIO 15 is used to reset module incl. wifi
	//key Dwn GPIO15
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, FUNC_GPIO15);
	os_timer_disarm(&resetBtnDwntimer);
	os_timer_setfn(&resetBtnDwntimer, resetBtnDwnTimerCb, NULL);
	os_timer_arm(&resetBtnDwntimer, 100, 1);
}
