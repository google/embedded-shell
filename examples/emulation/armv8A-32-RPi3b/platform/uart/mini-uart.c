/**
 * Copyright 2022 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#include "mini-uart.h"

#define PERIPH_BASE		0x3f000000
#define AUX_BASE		(PERIPH_BASE + 0x215000)
#define GPIO_BASE		(PERIPH_BASE + 0x200000)

#define AUX_IRQ 		    *((unsigned int*)(AUX_BASE + 0x0)) 
#define AUX_ENABLES 		*((unsigned int*)(AUX_BASE + 0x4))
#define AUX_MU_IO_REG 		*((unsigned int*)(AUX_BASE + 0x40))
#define AUX_MU_IER_REG 		*((unsigned int*)(AUX_BASE + 0x44))
#define AUX_MU_IIR_REG 		*((unsigned int*)(AUX_BASE + 0x48))
#define AUX_MU_LCR_REG 		*((unsigned int*)(AUX_BASE + 0x4c))
#define AUX_MU_MCR_REG 		*((unsigned int*)(AUX_BASE + 0x50))
#define AUX_MU_LSR_REG 		*((unsigned int*)(AUX_BASE + 0x54))
#define AUX_MU_MSR_REG 		*((unsigned int*)(AUX_BASE + 0x58))
#define AUX_MU_SCRATCH 		*((unsigned int*)(AUX_BASE + 0x5c))
#define AUX_MU_CNTL_REG 	*((unsigned int*)(AUX_BASE + 0x60))
#define AUX_MU_STAT_REG 	*((unsigned int*)(AUX_BASE + 0x64))
#define AUX_MU_BAUD_REG 	*((unsigned int*)(AUX_BASE + 0x68))


#define GPFSEL0         	*((unsigned int*)(GPIO_BASE+0x00))
#define GPFSEL1         	*((unsigned int*)(GPIO_BASE+0x04))
#define GPFSEL2         	*((unsigned int*)(GPIO_BASE+0x08))
#define GPFSEL3         	*((unsigned int*)(GPIO_BASE+0x0C))
#define GPFSEL4         	*((unsigned int*)(GPIO_BASE+0x10))
#define GPFSEL5         	*((unsigned int*)(GPIO_BASE+0x14))
#define GPSET0          	*((unsigned int*)(GPIO_BASE+0x1C))
#define GPSET1          	*((unsigned int*)(GPIO_BASE+0x20))
#define GPCLR0          	*((unsigned int*)(GPIO_BASE+0x28))
#define GPLEV0         		*((unsigned int*)(GPIO_BASE+0x34))
#define GPLEV1          	*((unsigned int*)(GPIO_BASE+0x38))
#define GPEDS0          	*((unsigned int*)(GPIO_BASE+0x40))
#define GPEDS1          	*((unsigned int*)(GPIO_BASE+0x44))
#define GPHEN0          	*((unsigned int*)(GPIO_BASE+0x64))
#define GPHEN1          	*((unsigned int*)(GPIO_BASE+0x68))
#define GPPUD           	*((unsigned int*)(GPIO_BASE+0x94))
#define GPPUDCLK0       	*((unsigned int*)(GPIO_BASE+0x98))
#define GPPUDCLK1       	*((unsigned int*)(GPIO_BASE+0x9C))

void muputc(char c){
    do{
		asm volatile("nop");
	}while(!(AUX_MU_LSR_REG & 0x20)); // wait for UART tx

    AUX_MU_IO_REG = c;  // write byte to IO register.
}


int mugetc(void){

    int response;

    do{
		asm volatile("nop");
	}while(!(AUX_MU_LSR_REG & 0x01)); 
	
	response = AUX_MU_IO_REG & 0xFF; // read from the IO register.

	if(response == '\r') return '\n'; // conv carriage ret into newline

	return response;    
}


void mini_uart_init(void){

    AUX_ENABLES     |= 0x1;
    AUX_MU_IER_REG  = 0;
    AUX_MU_CNTL_REG = 0;
	AUX_MU_LCR_REG  = 3; // uart 8-bit mode
    AUX_MU_MCR_REG  = 0;
    AUX_MU_IER_REG  = 1;
    AUX_MU_IIR_REG  = 0xc6;
    AUX_MU_BAUD_REG = 270; // 115200 baudrate

    

	GPFSEL1 &= ~((0b11 << 12 | 0b11 << 15)); // reset GPIO Pins 14 and 15
	GPFSEL1 |= (0b10 << 14 | 0b10 << 15);

	GPPUD = 0;
	for(int i=0;i<150;i++); // wait for approx. 150 cycles
    GPPUDCLK0 = ((1u << 14) | (1u << 15));
	for(int i=0;i<150;i++); // wait for approx. 150 cycles
    GPPUDCLK0 = 0;

	AUX_MU_CNTL_REG = 3;  //enable tx/rx
}
