/*
Experiment 13: 
Implement delay generic functions using 16MHZ SysTick timer with interrupt method. 
Void KM_Delayms(int );
Void KM_Delayus(int ); 

Board						:	Raayanmini Board V4.0/V4.1
Microcontroller	:	STM32F401RBT6
*/

//RCC registers
#define		RCC_AHB1ENR								(*(volatile int *)0x40023830)						//RCC system clock
//GPIOB registers
#define		GPIO_PORTB_MODE						(*(volatile int *)0x40020400)						//GPIOB MODE register
#define		GPIO_PORTB_IDR						(*(volatile int *)0x40020410)						//GPIOB input data register
#define		GPIO_PORTB_ODR						(*(volatile int *)0x40020414)						//GPIOB output data register
#define		STK_CTRL            			(*(volatile int *)0xE000E010) 
#define		STK_LOAD            			(*(volatile int *)0xE000E014)
#define		STK_VAL             			(*(volatile int *)0xE000E018)

void Systick_Init(void);
void GPIO_PORT_B_Init(void);
void KM_Delay_us(int Delay);
void KM_Delay_ms(int Delay);
void SysTick_Handler(void);

void GPIO_PORT_B_Init(void)
{
  	RCC_AHB1ENR         |= 0x02;              		//Enable clock for GPIO_PORT_B
		while(!(RCC_AHB1ENR & 0x02));             		//Wait untill Clock Enable 
	  GPIO_PORTB_MODE     &= ~(0x03<<26);       		//Clear 26 and 27 bits for setting Output mode 
	  GPIO_PORTB_MODE     |= (0x01<<26);        		//Set 26 bit for PB13 Output mode for RED LED
}

void Systick_Init(void)
{
		STK_CTRL |= 0x07;                     			//Select Clock source as 16mhz( Processor clock (AHB))  |  SysTick exception request enable   |   Systick Counter enable   
		STK_VAL   = 0x123;                     		 	//write of any value clears the field to 0, and also clears the COUNTFLAG bit in the STK_CTRL register to 0.
}

void KM_Delay_ms(int Delay)
{
		STK_VAL  = 0x123;
		STK_LOAD = 16000 * Delay;             			//16000 for 1ms delay
}	

void KM_Delay_us(int Delay)
{
	  STK_VAL  = 0x123;
		STK_LOAD = 16 * Delay ;                			//16 for 1us delay
}

int main(void)
{
		GPIO_PORT_B_Init();
		Systick_Init();
		KM_Delay_ms(100);
		while(1)
		{
		  ; 
		}
}

void SysTick_Handler(void)
{
	  GPIO_PORTB_ODR ^= (0x01<<13);          			//Toggle PB13 LED at Every 100ms Delay
}
