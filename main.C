#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xil_exception.h"
#include "string.h"
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
//#define MIO_PIN_17 0xF8000744
//#define MIO_PIN_18 0xF8000748
//#define MIO_PIN_50 0xF80007C8
//#define MIO_PIN_51 0xF80007CC
#define UART_INT_DIS (0xE0001000 + 0x0C)
#define UART_RxFIFO_Trigger 0xE0001020
#define UART_Priority_Reg 0xF8F01450
#define UART_Processor_Target_Reg 0xF8F01850
#define UART_Set_En 0xF8F01108
#define UART_Config_Reg 0xF8F01C14
#define UART_ISR 0xE0001014
#define MIO_PIN_17 0xF8000744
#define MIO_PIN_18 0xF8000748
#define MIO_PIN_50 0xF80007C8
#define MIO_PIN_51 0xF80007CC
#define GPIO_DIRM_0 0xE000A204 // Direction mode bank 0
#define GPIO_OUTE_0 0xE000A208 // Output enable bank 0
#define GPIO_DIRM_1 0xE000A244 // Direction mode bank 1
#define GPIO_INT_STAT_1 0xE000A258 // Interrupt status bank 1
#define LED_Base_Address 0x4BB00000
# define BTN_Base_Address 0x4BB02004
#define SVN_SEG_CTRL 0X4BB03000
#define DIG1_ADDRESS (0X4BB03000 + 4)
#define DIG2_ADDRESS (0X4BB03000 + 8)
#define DIG3_ADDRESS (0X4BB03000 + 12)
#define DIG4_ADDRESS (0X4BB03000 + 16)
#define SVN_SEG_DP (0X4BB03000 + 20)
//lab specific registers
#define UART1_CON_Addr 0xE0001000
#define UART_INT_EN (0xE0001000 + 0x08)
#define UART1_Mode_Addr 0xE0001004
#define UART1_INT_EN_Addr 0xE0001008
#define UART1_INT_DIS_Addr 0xE000100C
#define UART1_INT_MASK_Addr 0xE0001010
#define UART1_ISR_Addr 0xE0001014
#define UART1_Buad_Gen_Addr 0xE0001018
#define UART1_RT_Addr 0xE000101C
#define UART1_RFIFO_LEV_Addr 0xE0001020
#define UART1_Modem_CNT_Addr 0xE0001024
#define UART1_Modem_Stat_Addr 0xE0001028
#define UART1_C_Stat_Addr 0xE000102C
#define UART1_FIFO_Addr 0xE0001030
#define UART1_Baud_DIV_Addr 0xE0001034
#define UART1_FCD_Addr 0xE0001038
#define UART1_TFIFO_LEV_Addr 0xE0001044


void Initialize_UART1();
void Initialize_SVD();
void SendChar(uint8_t C);
void delay(int j);
void configure_GIC();
char array[35];
int i = 0;
void Initialize_GPIO_Interrupts();
int turnOnLED1();
void Configure_IO();
int turnOnLED2();
int turnOnLED4();
int turnOnLED3();
void MyGPIOIRQHandler(void *data);
void turnOffLED1();
//int turnOnLED();
void enable_interrupts();
void UnInitialize_SVD();
void disable_interrupts();
void ButtonHandler(uint32_t button_press);
void IRQ_Handler(void *data);
void turnOffLED();
void checkLEDON();
void checkLEDOFF();
uint8_t D1 =0;
uint8_t D2 =0;
uint8_t D3 =0;
uint8_t D4 =0;

int main()
{

	init_platform(); //initializes the platform, in the "platform.h" file, configures the UART
	Initialize_UART1();
	disable_interrupts();
	Configure_IO();
	//Initialize_SVD();
	configure_GIC();
	Initialize_GPIO_Interrupts();
	Xil_ExceptionRegisterHandler(5, IRQ_Handler, NULL);
	enable_interrupts();
	while (1)
	{
		;
	}
	// build-in
	return 0;
}

//configure button 4/5 as inputs and LED8 as the output
void configure_GIC()
{
*((uint32_t*) ICDIPTR_BASEADDR+13) = 0x00000000;
*((uint32_t*) ICDICER_BASEADDR+1) = 0x00000000;
*((uint32_t*) ICDDCR_BASEADDR) = 0x0;
*((uint32_t*) ICDIPR_BASEADDR+13) = 0x000000A0;
*((uint32_t*) ICDIPTR_BASEADDR+13) = 0x00000001;
*((uint32_t*) ICDICFR_BASEADDR+3) = 0x55555555; //GPIO
*((uint32_t *) ICDISER_BASEADDR+1) = 0x00100000;
*((uint32_t*) ICCPMR_BASEADDR) = 0xFF;
*((uint32_t*) ICCICR_BASEADDR) = 0x3;
*((uint32_t*) ICDDCR_BASEADDR) = 0x1;

*((uint32_t*) UART_Priority_Reg) = 0x00B00000; //sets priority for ID #82
*((uint32_t*) UART_Config_Reg) = 0x0000000C; //Configures interrupt for ID #82, rising edge active
*((uint32_t*) UART_Processor_Target_Reg) = 0x0010000; //this sets the target as CPU0
*((uint32_t*) UART_Set_En) = 0xFFFFFFFF;//writing a set enable for ID: 95-64, ICDICER2
return;
}

void Initialize_UART1(){
*((uint32_t*) UART1_CON_Addr) = 0x0000000; // disable transmitter and receiver
*((uint32_t*) UART1_Buad_Gen_Addr) = 0x7C; // Baud Gen
*((uint32_t*) UART1_Baud_DIV_Addr) = 0x6; // Baud Divider
*((uint32_t*) UART1_CON_Addr) = 0x0000157; // disable transmitter and receiver
// Normal mode, single stop bit, no parity 8 bits data
*((uint32_t*)UART_RxFIFO_Trigger) = 0x1; //shown in Reference document,
// (in line above) trigger set when receiver fifo fills to the # of bytes specified
*((uint32_t*) UART1_Mode_Addr) = 0x0000030;
//*((uint32_t*) UART1_INT_DIS_Addr) = 0x000FFFF; // Disable interrupt
*((uint32_t*) UART1_FIFO_Addr) = 0x1;
*((uint32_t*) UART1_RT_Addr) = 0x1; // Reciever Time out
*((uint32_t*) UART_INT_DIS) = 0xFEFE; //this enablese the receiver overflow interrupt
*((uint32_t*) UART_INT_EN) = 0x101;

}


void SendChar(uint8_t C){
*((uint32_t*) UART1_FIFO_Addr) = C; // Disable interrupt
}

void IRQ_Handler(void *data)
{

	 //buys time for FIFO to fill

//bit wise and it with two
uint32_t interrupt_ID = *((uint32_t*)ICCIAR_BASEADDR);
	if (interrupt_ID == 82) //checking if the interrupt is from the UART
	{

		//int z =0;
//

			// while((*((uint32_t*) UART1_C_Stat_Addr)) != 8){
			 uint32_t R= *((uint32_t*) UART1_C_Stat_Addr);
			 if (((R & 0x0002) != 0x2)) //if the FIFO is not empty >> //prolly a while
			 {
				 //delay(10000); //because the clock cycle is too fast so it goes through the FIFo twice even though only one interrupt was done
				 uint8_t C = *((uint32_t*) UART1_FIFO_Addr);
								 if (C >= 32)
								 {
									//SendChar(C);
									array[i] = C;
									SendChar(array[i]); //proves that it's storing in the array correctly
									i++;
								 }
								 if (C == ';')
								 {
									 checkLEDON();
								 }


								  //D1++;
								 // (*((uint32_t*)DIG1_ADDRESS)) = D1;


			 }


			 }

*((uint32_t*)UART_ISR) = 0xFFFFFF; //resetting the Interrupt Status Register so it clears interrupts
//*((uint32_t*)UART1_RT_Addr) = 0xFFFFFF; //resetting the Reset Timeout for UART
*((uint32_t*)ICCEOIR_BASEADDR) = interrupt_ID; // Clears the GIC flag bit.

}

void turnOffLED1(){
*((uint32_t*) LED_Base_Address) = 0x00000000;
*((uint32_t*) LED_Base_Address+1) = 0x00000000;
return;
}

int turnOnLED1(){
*((uint32_t*) LED_Base_Address) = 0x00000001;
*((uint32_t*) LED_Base_Address+1) = 0x00000001;
return 1;
}

int turnOnLED2()
{
	*((uint32_t*) LED_Base_Address) = 0x00000002;
	*((uint32_t*) LED_Base_Address+1) = 0x00000002;
	return 1;
}

int turnOnLED3()
{
	*((uint32_t*) LED_Base_Address) = 0x00000004;
	*((uint32_t*) LED_Base_Address+1) = 0x00000004;
	return 1;
}

int turnOnLED4()
{
	*((uint32_t*) LED_Base_Address) = 0x00000008;
	*((uint32_t*) LED_Base_Address+1) = 0x00000008;
	return 1;
}

void checkLEDON()
{
	//turnOnLED();
	int v =0;
	if ((strncmp(array, ">>LED1 ON;", strlen(">>LED1 ON;"))) == 0)
	{
//		D1++;
//		(*((uint32_t*)DIG1_ADDRESS)) = D1;
		turnOnLED1();

	}
	else if((strncmp(array, ">>LED2 ON;", strlen(">>LED2 ON;"))) == 0)
	{
		turnOnLED2();
	}
	else if((strncmp(array, ">>LED3 ON;", strlen(">>LED3 ON;"))) == 0)
	{
		turnOnLED3();
	}
	else if((strncmp(array, ">>LED4 ON;", strlen(">>LED4 ON;"))) == 0)
	{
		turnOnLED4();
	}
	else if(((strncmp(array, ">>LED1 OFF;", strlen(">>LED1 OFF;"))) == 0) && (turnOnLED1() == 1))
	{
		turnOffLED1();
	}
	else if(((strncmp(array, ">>LED2 OFF;", strlen(">>LED2 OFF;"))) == 0) && (turnOnLED2() == 1))
	{
		turnOffLED1();
	}
	else if(((strncmp(array, ">>LED3 OFF;", strlen(">>LED3 OFF;"))) == 0) && (turnOnLED3() == 1))
	{
		turnOffLED1();
	}
	else if(((strncmp(array, ">>LED4 OFF;", strlen(">>LED4 OFF;"))) == 0) && (turnOnLED4() == 1))
	{
		turnOffLED1(); //need to edit this one
	}
	else if((strncmp(array, ">>SVD ON;", strlen(">>SVD ON;"))) == 0)
	{
		Initialize_SVD();
	}
	else if((strncmp(array, ">>SVD OFF;", strlen(">>SVD OFF;"))) == 0)
	{
		UnInitialize_SVD();
	}
	else if((strncmp(array, " >> INCREMENT DIGIT1;", strlen( " >> INCREMENT DIGIT1;"))) == 0)
	{
		D1++;
		(*((uint32_t*)DIG1_ADDRESS)) = D1;

	}
	else if((strncmp(array, " >> INCREMENT DIGIT2;", strlen( " >> INCREMENT DIGIT2;"))) == 0)
	{
		D2++;
		(*((uint32_t*)DIG2_ADDRESS)) = D2;
	}
	else if((strncmp(array, " >> INCREMENT DIGIT3;", strlen( " >> INCREMENT DIGIT3;"))) == 0)
	{
		D3++;
		(*((uint32_t*)DIG3_ADDRESS)) = D3;
	}
	else if((strncmp(array, " >> INCREMENT DIGIT4;", strlen( " >> INCREMENT DIGIT4;"))) == 0)
	{
		D4++;
		(*((uint32_t*)DIG4_ADDRESS)) = D4;
	}
	else if((strncmp(array, ">> RESET DIGIT1;", strlen(">> RESET DIGIT1;"))) == 0)
	{
		D1 = 0;
		(*((uint32_t*)DIG1_ADDRESS)) = D1;
	}
	else if((strncmp(array, ">> RESET DIGIT2;", strlen(">> RESET DIGIT2;"))) == 0)
	{
		D2 = 0;
		(*((uint32_t*)DIG2_ADDRESS)) = D2;
	}
	else if((strncmp(array, ">> RESET DIGIT3;", strlen(">> RESET DIGIT3;"))) == 0)
	{
		D3 = 0;
		(*((uint32_t*)DIG3_ADDRESS)) = D3;
	}
	else if((strncmp(array, ">> RESET DIGIT4;", strlen(">> RESET DIGIT4;"))) == 0)
	{
		D4 = 0;
		(*((uint32_t*)DIG4_ADDRESS)) = D4;
	}
	else if((strncmp(array, ">> INCREMENT DIGIT1 ON BTN4;", strlen(">> INCREMENT DIGIT1 ON BTN4;"))) == 0)
	{
		//insert handler here
		Initialize_SVD();
		Xil_ExceptionRegisterHandler(5, MyGPIOIRQHandler, NULL);
	}




		//send echo command that this is not valid

	 i = 0;
	 return;
}

void checkLEDOFF(int z)
{
	if(z == 1 && ((strncmp(array, "LEDx OFF;", 9)) == 0))
	{
		turnOffLED1();
		return;
	}
	else
	{
		return;
	}
}



void enable_interrupts(){
uint32_t read_cpsr=0; // used to read previous CPSR value
uint32_t mode = 0x5F; // System mode [4:0] and IRQ enabled [7]
uint32_t bit_mask = 0xFF; // used to clear bottom 8 bits
__asm__ __volatile__("mrs %0, cpsr\n" : "=r" (read_cpsr) );
__asm__ __volatile__("msr cpsr,%0\n" : : "r" ((read_cpsr & (~bit_mask))| mode));
return;
}

void disable_interrupts()
{

	uint32_t mode = 0xDF; // System mode [4:0] and IRQ disabled [7], D == 1101, F == 1111
	//what does this mean???
	uint32_t read_cpsr=0; // used to read previous CPSR value, read status register values
	uint32_t bit_mask = 0xFF; // used to clear bottom 8 bits
	__asm__ __volatile__("mrs %0, cpsr\n" : "=r" (read_cpsr) ); // execute the assembly instruction MSR
	__asm__ __volatile__("msr cpsr,%0\n" : : "r" ((read_cpsr & (~bit_mask))| mode)); // only change the
	//lower 8 bits
	return;
}

void Initialize_SVD()
{
	*((uint32_t*)SVN_SEG_CTRL) = 0x9;
	*((uint32_t*)DIG1_ADDRESS) = 0x0;
	*((uint32_t*)DIG2_ADDRESS) = 0x0;
	*((uint32_t*)DIG3_ADDRESS) = 0x0;
	*((uint32_t*)DIG4_ADDRESS) = 0x0;
	//*((uint32_t*)SVN_SEG_DP) = 0x1;
	*((uint32_t*)D1) =0;
	*((uint32_t*)D2) =0;
	*((uint32_t*)D3) =0;
	*((uint32_t*)D4) =0;
	//I think I need to add some code here that
	return;
}

void UnInitialize_SVD()
{
	*((uint32_t*)SVN_SEG_CTRL) = 0x0;
	*((uint32_t*)DIG1_ADDRESS) = 0x0;
	*((uint32_t*)DIG2_ADDRESS) = 0x0;
	*((uint32_t*)DIG3_ADDRESS) = 0x0;
	*((uint32_t*)DIG4_ADDRESS) = 0x0;
		//*((uint32_t*)SVN_SEG_DP) = 0x1;
		*((uint32_t*)D1) =0;
		*((uint32_t*)D2) =0;
		*((uint32_t*)D3) =0;
		*((uint32_t*)D4) =0;
		//I think I need to add some code here that
		return;
}

void delay(int i)
{
int k = 0;
while (k<i)
k++;
}

void MyGPIOIRQHandler(void *data)
{
//	turnOnLED4();
	uint32_t interrupt_ID = *((uint32_t*)ICCIAR_BASEADDR);

	if(interrupt_ID == 52)
	{
		uint32_t GPIO_INT = *((uint32_t*)GPIO_INT_STAT_1);
		uint32_t button_press = 0xC0000 & GPIO_INT;
		ButtonHandler(button_press);
	}

	if(interrupt_ID == 82)
	{
	 uint32_t R= *((uint32_t*) UART1_C_Stat_Addr);
	 if (((R & 0x0002) != 0x2)) //if the FIFO is not empty >> //prolly a while
	 {
		 //delay(10000); //because the clock cycle is too fast so it goes through the FIFo twice even though only one interrupt was done
		 uint8_t C = *((uint32_t*) UART1_FIFO_Addr);
						 if (C >= 32)
						 {
							//SendChar(C);
							array[i] = C;
							SendChar(array[i]); //proves that it's storing in the array correctly
							i++;
						 }
						 if (C == ';')
						 {
							 checkLEDON();
						 }


						  //D1++;
						 // (*((uint32_t*)DIG1_ADDRESS)) = D1;


	 }
	}

	*((uint32_t*)ICCEOIR_BASEADDR) = interrupt_ID; // Clears the GIC flag bit.
}

void ButtonHandler(uint32_t button_press){
	uint32_t BTN5=0x80000;
	uint32_t BTN4=0x40000;
	delay(100000); // button rebounce
	if(button_press == BTN4)
	{
		D1++;
	}
	if(button_press == BTN5)
	{
		D1++;
	}
	(*((uint32_t*)DIG1_ADDRESS)) = D1;
	*((uint32_t*)GPIO_INT_STAT_1) = 0xFFFFFF; //clearing interrupt status reg
}

void Configure_IO()
{
	*((uint32_t *) 0xF8000000+0x8/4) = 0x0000DF0D; //Write unlock code to enable writing into system level
		//Control unlock Register
		*((uint32_t*) MIO_PIN_50) = 0x00000600; // BTN4 6th group of four is: 0110
		*((uint32_t*) MIO_PIN_51) = 0x00000600; //BTN5 6th group of four is: 0110
		*((uint32_t*) GPIO_DIRM_0) = 0x00070000; //direction mode for bank 0
		*((uint32_t*) GPIO_OUTE_0) = 0x00070000; //output enable for bank 0
		*((uint32_t*) GPIO_DIRM_1) = 0x00000000; //direction mode for bank 1
		return;

}

void Initialize_GPIO_Interrupts(){
*((uint32_t*) GPIO_INT_DIS_1) = 0xFFFFFFFF;
*((uint32_t*) GPIO_INT_DIS_0) = 0xFFFFFFFF;
*((uint32_t*) GPIO_INT_STAT_1) = 0xFFFFFFFF; // Clear Status register
*((uint32_t*) GPIO_INT_TYPE_1) = 0x0C0000; // Type of interrupt rising edge
*((uint32_t*) GPIO_INT_POL_1) = 0x0C0000; // Polarity of interrupt
*((uint32_t*) GPIO_INT_ANY_1) = 0x000000; // Interrupt any edge sensitivity
*((uint32_t*) GPIO_INT_EN_1) = 0x0C0000; // Enable interrupts in bank 0
return;
}



//i would be a star if Botched was about broken code




