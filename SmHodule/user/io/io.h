#ifndef USER_IO_H_
#define USER_IO_H_

static int keyDown = 0;
static int keyUp = 0;

uint16_t getGpio13Counter(void);                        // gpio13 counter
void resetGpio13Counter(void);                        // gpio13 counter
void ioInit(void);
void ioGPIO16(int ena);

#endif /* USER_IO_H_ */

