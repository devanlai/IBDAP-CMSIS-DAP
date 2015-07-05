/*
 * uart.c
 *
 *  Created on: Jun 26, 2015
 *      Author: yliu
 */

#include "LPC11Uxx.h"

void init_uart () {
	/*
	LPC_IOCON->PIO0_18 = 1; // PIO0_18 used for RXD
	LPC_IOCON->PIO0_19 = 1; // PIO0_19 used for TXD

	// Setup UART for 115.2K8N1
	Chip_UART_Init(LPC_USART);
	LPC_SYSCON->SYSAHBCLKCTRL |= 1 << 12;
	Chip_UART_SetBaud(LPC_USART, 115200);
	Chip_UART_ConfigData(LPC_USART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	Chip_UART_SetupFIFOS(LPC_USART, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
	Chip_UART_TXEnable(LPC_USART);
	*/
}
