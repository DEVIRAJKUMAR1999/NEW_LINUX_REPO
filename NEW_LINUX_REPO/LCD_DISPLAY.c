
#define RCC_AHB1ENR (*(int*)0x40023830)
#define GPIOB_MODER (*(int *)0x40020400)
#define GPIOB_ODR (*(int *)0x40020414)
	
	void delay();
void portb_init()
{
	RCC_AHB1ENR |=(0x01<<1);//enble port b clock
	while(!( RCC_AHB1ENR&0x2)); //check enable
	GPIOB_MODER &=~(0xffffffff);//port B select
	GPIOB_MODER |=(0x10555);//port B select
}

void write_high_nibble(unsigned char ch)
{  
	GPIOB_ODR &=~(0xf);//odr pb0-pb3 clear bits 
	GPIOB_ODR |=(ch>>4)&0xf; 
	GPIOB_ODR |=0x0100;//PB8  odr8 enable
  delay(20); 
	GPIOB_ODR &=~0x0100;//PB8 disenable
  delay(1);
}

void write_low_nibble(unsigned char ch)
{  
	GPIOB_ODR &=~0xf;//PB clear bits 
	GPIOB_ODR |=((ch)&0xf); 
	GPIOB_ODR |=0x0100;//PB8 enable
  delay(20); 
	GPIOB_ODR &=~0x0100;//PB8 disenable
  delay(1);

}

void KM_LCD_Write_Cmd(unsigned char ch)
	{
GPIOB_ODR &=~(0x1<<4);										//for RS=0 command mode
	write_high_nibble(ch);
	write_low_nibble(ch);
	}

	void KM_LCD_Write_Data(unsigned char ch)
	{
  GPIOB_ODR |=(0x1<<4);										//for RS=1 datamode 
	write_high_nibble(ch);
	write_low_nibble(ch);
	}
	
	void lcd_Init(void)
{  
	delay(20);
	KM_LCD_Write_Cmd(0x33);		
	delay(1)		;										//delay 1msec
	KM_LCD_Write_Cmd(0x32);        //clear display
	KM_LCD_Write_Cmd (0x0C);											//Display ON, Cursor OFF
	KM_LCD_Write_Cmd (0x01);         //next line
}

void KM_LCD_Write_Str(char *p)
{
		int i;
		for(i=0;p[i]!=0;i++)
		{
			KM_LCD_Write_Data(p[i]);									//writing data into display character by character
		}
}

void next_window ()
{
		KM_LCD_Write_Cmd (0x01);
}
void next_line ()
{
		KM_LCD_Write_Cmd (0xc0);
}

void cursor_blinking ()
	{
		KM_LCD_Write_Cmd (0x0f);
	}
	
	void dis_off()
	{
		KM_LCD_Write_Cmd (0x08);
	}
void delay(int n)
{
int i;
for(i=2500*n;i>0;)
	{
		i--;
  }
}

int main()
{ 
	char p[]="first";
	portb_init();
	lcd_Init();
	KM_LCD_Write_Data('d');
	KM_LCD_Write_Data('e');
	KM_LCD_Write_Data('v');
	KM_LCD_Write_Data('i');
	KM_LCD_Write_Data(' ');
	delay(20);
	KM_LCD_Write_Str(p);
	next_line();  
	KM_LCD_Write_Str(" program");
	delay(1500);
	next_line();   
	delay(20);
  KM_LCD_Write_Str("its");
	//cursor_blinking ();
	KM_LCD_Write_Str(" good");
	delay(1500);
	next_line();  
	next_window ();
	KM_LCD_Write_Str("welcome");
	next_line(); 
	KM_LCD_Write_Str("ram");
	delay(1500);
	next_line();
	KM_LCD_Write_Str("uday");
	delay(1500);
	next_line();
	KM_LCD_Write_Str("manoj");
	 cursor_blinking ();
	//dis_off();
	while(1)
		{
			;
	}
	
}
