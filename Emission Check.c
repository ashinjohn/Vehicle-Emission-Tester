#include<avr/io.h>
#define F_CPU 16000000UL
#include<avr/interrupt.h>
#include<util/delay.h>
volatile unsigned char value;
volatile unsigned int O_flag=0,K_flag=0,send_flag=0;

int horse=0;
int cat=0;

int read_adc (char chn1)
{

int val , val1;


ADMUX = 0x40 ; 
ADCSRA = 0x86 ; 
			  
			   

ADMUX = (0x40|(chn1&0x07)); 
							

	

_delay_ms(1); 

ADCSRA |= 0x40; 

while ((ADCSRA&0x40)); 
					   
val=ADCL;
val1=ADCH;

val= val|(val1<<8); 
return (val);


}




ISR(USART_RXC_vect)
{

value=UDR;

if(value=='O')
O_flag=1;

else if((value=='K')&&(O_flag==1))
{
K_flag=1;
cat=1;
send_flag=1;
}


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



prints("ATE0\r\n");
prints("AT+CMGF=1\r\n");
prints("AT+CNMI=2,1,0,0,0\r\n");
prints("AT+CMGS=\"+919400927068\"\r\n");

sei();


char buf[6];
int cnt=0;
int adc_val=0;


int sens1,sens2;
int problem;

int pre1,pre2;

while(1)
{

	adc_val=read_adc(0);   // reading data from sensor 1 
		sens1=adc_val;
		
		if (horse==0)
		{ pre1=sens1-50; }
		
		
		
		buf[4]=0;
		for (cnt=3;cnt>=0;cnt--)
		{
		 buf[cnt]=(adc_val%10)+'0';
		 adc_val=adc_val/10;
		}

			for (cnt=1;cnt<=3;cnt++)
		{
		 	send_char(buf[cnt]);
		}

		send_char(' ');
	
	
	adc_val=read_adc(1);   // reading data from sensor 2
		sens2=adc_val;
		
			if (horse==0)
			{ pre2=sens2-30; }
		
		
		buf[4]=0;
		for (cnt=3;cnt>=0;cnt--)
		{
		 buf[cnt]=(adc_val%10)+'0';
		 adc_val=adc_val/10;
		}
	for (cnt=1;cnt<=3;cnt++)
		{
		 	send_char(buf[cnt]);
		}

    
	
	
	
	horse=1;



	if (sens1<pre1)
	problem=1;
	else 
	problem=0;



send_char('\r');
send_char('\n');


if (problem==1)
{
prints(" White Maruthi Suzuki Swift KL 40 A 1717 ");
send_char(0x1A);
send_flag=0;
while(1);
}
}
}




