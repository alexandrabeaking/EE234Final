#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xil_exception.h"
#define ICCPMR_BASEADDR 0xF8F00104 // Interrupt Priority Mask Register
#define ICCICR_BASEADDR 0xF8F00100 // CPU Interface Control Register
#define ICDDCR_BASEADDR 0xF8F01000 // Distributor Control Register
#define ICDISER_BASEADDR 0xF8F01100 // Interrupt Set Enable Register
#define ICDICER_BASEADDR 0xF8F01180 // Interrupt Clear/Enable Register
#define ICDIPR_BASEADDR 0xF8F01400 // Interrupt Priority Register Base address
#define ICDIPTR_BASEADDR 0xF8F01800 // Interrupt Processor Targets Register
#define ICDICFR_BASEADDR 0xF8F01C00 // Interrupt Configuration Register
#define ICCIAR_BASEADDR 0xF8F0010C // Interrupt Acknowledge Register
#define ICCEOIR_BASEADDR 0xF8F00110 // End Of Interrupt Register
#define GPIO_MTDATA_OUT_0 0xE000A004 // Maskable data out in bank 0
#define GPIO_INT_DIS_0 0xE000A214 // Interrupt disable bank 0
#define GPIO_INT_EN_1 0xE000A250 // Interrupt enable bank 1
#define GPIO_INT_DIS_1 0xE000A254 // Interrupt disable bank 1
#define GPIO_INT_STAT_1 0xE000A258 // Interrupt status bank 1
#define GPIO_INT_TYPE_1 0xE000A25C // Interrupt type bank 1
#define GPIO_INT_POL_1 0xE000A260 // Interrupt polarity bank 1
#define GPIO_INT_ANY_1 0xE000A264 // Interrupt any edge sensitive bank 1
#define MIO_PIN_16 0xF8000740
#define UART_INT_DIS (0xE0001000 + 0x0C)
#define ICDIPR20 (0xF8F00000 + 0x1450)
#define MIO_PIN_17 0xF8000744
#define MIO_PIN_18 0xF8000748
#define MIO_PIN_50 0xF80007C8
#define MIO_PIN_51 0xF80007CC
#define GPIO_DIRM_0 0xE000A204 // Direction mode bank 0
#define GPIO_OUTE_0 0xE000A208 // Output enable bank 0
#define GPIO_DIRM_1 0xE000A244 // Direction mode bank 1

void Initialize_UART1();
void SendChar(uint8_t C);
void configure_GIC();

int main()
{
	char array[35]; 
	init_platform(); //initializes the platform, in the "platform.h" file, configures the UART
	configure_GIC();
	Initialize_UART1();
		 return 0;
}

//configure button 4/5 as inputs and LED8 as the outpu


void configure_GIC(){
*((uint32_t*) ICDIPTR_BASEADDR+13) = 0x00000000;
*((uint32_t*) ICDICER_BASEADDR+1) = 0x00000000;
*((uint32_t*)ICDIPR20) = 0x00000000; // this will do interrupt id#82 
*((uint32_t*) ICDDCR_BASEADDR) = 0x0;
*((uint32_t*) ICDIPR_BASEADDR+13) = 0x000000A0;
*((uint32_t*) ICDIPTR_BASEADDR+13) = 0x00000001;
*((uint32_t*) ICDICFR_BASEADDR+3) = 0x55555555;
*((uint32_t *) ICDISER_BASEADDR+1) = 0x00100000;
*((uint32_t*) ICCPMR_BASEADDR) = 0xFF;
*((uint32_t*) ICCICR_BASEADDR) = 0x3;
*((uint32_t*) ICDDCR_BASEADDR) = 0x1;
return;
}


void Initialize_UART1(){
*((uint32_t*) UART1_CON_Addr) = 0x0000000; // disable transmitter and receiver
*((uint32_t*) UART1_Buad_Gen_Addr) = 0x7C; // Baud Gen
*((uint32_t*) UART1_Baud_DIV_Addr) = 0x6; // Baud Divider
*((uint32_t*) UART1_CON_Addr) = 0x0000117; // disable transmitter and receiver
// Normal mode, single stop bit, no parity 8 bits data
*((uint32_t*) UART1_Mode_Addr) = 0x0000030;
*((uint32_t*) UART1_INT_DIS_Addr) = 0x000FFFF; // Disable interrupt
*((uint32_t*) UART1_RT_Addr) = 0xFF; // Reciever Time out
*((uint32_t*)ART_INT_DIS) = 0xFEFE; //this enablese the receiver overflow interrupt 

}

void test_UART()
{
	int i =0; 
	 while(1){
	 uint32_t R= *((uint32_t*) UART1_C_Stat_Addr);
	 if ((R && 0x0002)== 0x0){
	 uint8_t C = *((uint32_t*) UART1_FIFO_Addr);
	while (array[i]!= NULL)
	 {
		 array[i] = C;
		 i=i+1;
	 }
	 }
	 }
	 }
}

void SendChar(uint8_t C){
*((uint32_t*) UART1_FIFO_Addr) = C; // Disable interrupt
}
