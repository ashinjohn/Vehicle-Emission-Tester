#include<avr/io.h>
#define F_CPU 16000000UL
#include<avr/interrupt.h>
#include<util/delay.h>
volatile unsigned char value;
volatile unsigned int O_flag=0,K_flag=0,send_flag=0;


void LCD_cmd(char cmd);
void LCD_dat(char dat);
void printlcd(const char *CPtr);



ISR(USART_RXC_vect)
{

	value=UDR;

	if(value=='O')
	O_flag=1;

	else if((value=='K')&&(O_flag==1))
	{
		K_flag=1;
		O_flag=0;
	}
	else if(value=='>')
	send_flag=1;


	UCSRA=0x40;
	UDR=value;
	while((UCSRA&0x40)==0);
}


void send_char(volatile unsigned char c)
{
UCSRA=0x40;
UDR=c;
while((UCSRA&0x40)==0);
}

void prints(char *ptr)
{
while(*ptr)
{
send_char(*ptr);
ptr++;
}
}

int main(void)
{
UBRRL=0x67;
UBRRH=0x00;
UCSRA=UCSRB=UCSRC=0x00;
UCSRB=0x98;
UCSRC=0x86;




	DDRC=0xFF;
	DDRD=0xE0;
	DDRA=0x00;


    LCD_cmd(0x30);
	LCD_cmd(0x30);
	LCD_cmd(0x30);

	LCD_cmd(0x38);

	LCD_cmd(0x38);
	LCD_cmd(0x06);
	LCD_cmd(0x0C);
 
    LCD_cmd(0x01);
	LCD_cmd(0x80);


	printlcd("SYSTEM READY");
	LCD_cmd(0xC0);
	printlcd("Monitoring Level");
	_delay_ms(1000);

	_delay_ms(1000);

sei();


int pro1 =0;
int pro2 =0;




while(1)
{

if((PINA & 0x01)==0)
{
pro1=1;
    LCD_cmd(0x01);
	LCD_cmd(0x80);


	printlcd("HIGH");
	LCD_cmd(0xC0);
	printlcd("Carbon Dioxide");
		_delay_ms(1000);
}

else
{
    pro1=0;
	LCD_cmd(0x01);
	LCD_cmd(0x80);


	printlcd("LOW");
	LCD_cmd(0xC0);
	printlcd("Carbon Monoxide");
		_delay_ms(1000);

}

_delay_ms(1000);

if((PINA & 0x02)==0)
{
pro2=1;

    LCD_cmd(0x01);
	LCD_cmd(0x80);


	printlcd("HIGH");
	LCD_cmd(0xC0);
	printlcd("Carbon Monoxide");
		_delay_ms(1000);
}
else
{
    pro2=0;
	LCD_cmd(0x01);
	LCD_cmd(0x80);


	printlcd("LOW");
	LCD_cmd(0xC0);
	printlcd("Carbon Dioxide");
		_delay_ms(1000);

}
_delay_ms(1000);

if (pro1==1||pro2==1)
{

    LCD_cmd(0x01);
	LCD_cmd(0x80);


	printlcd("Alerting");
	LCD_cmd(0xC0);
	printlcd("Authority");
	_delay_ms(1000);

   LCD_cmd(0x01);
	LCD_cmd(0x80);



		printlcd("Sending");
	LCD_cmd(0xC0);
	printlcd("Text Message");
	_delay_ms(1000);

 sei();    // global interrupt enable



			while(K_flag==0)
			{
				prints("AT\r\n");
				_delay_ms(2000);
			}

			prints("ATE0\r\n");
			_delay_ms(2000);
			prints("AT+CMGF=1\r\n");
			_delay_ms(2000);
			prints("AT+CNMI=2,1,0,0,0\r\n");
			_delay_ms(2000);
			prints("AT+CMGS=\"+919544907374\"\r\n");
			_delay_ms(2000);
			while(1)
			{
				if(send_flag==1)
				{
					send_flag==0;
					prints("Maruthi Suzuki Swift KL7 1717 White");
					
					_delay_ms(1000);
					send_char(0x1A);
					
				}
			}

   LCD_cmd(0x01);
	LCD_cmd(0x80);


	printlcd("MESSAGE SENT");

	_delay_ms(1000);

while(1);
} //probelm if 

}//while1  
} //main



void LCD_cmd(char cmd)
{
	PORTD&=(~0x80);
	PORTD&=(~0x40);
	PORTC=cmd;
	PORTD|=0x20;
	PORTD&=(~0x20);
	_delay_ms(10);
}

void LCD_dat(char dat)
{
	PORTD|=0x80;
	PORTD&=(~0x40);
	PORTC=dat;
	PORTD|=0x20;
	PORTD&=(~0x20);
	_delay_ms(10);
}

void printlcd(const char *CPtr)
{
	while(*CPtr != '\0'){
		LCD_dat(*CPtr);
		++CPtr;
	}
}

