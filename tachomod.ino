#include<avr/io.h>
#include<avr/interrupt.h>
#include <stdlib.h>


int rpm=0;
int count=0;
int clock_ms=0;
float time0 = 0, time1 = 0;
int overf = 0;
int x = 100;  //maintained rpm

void usart_int(void)
{
  UCSR0A=0x00;
  UCSR0B=(1<< RXEN0)|(1<< TXEN0);//TRANSMIT AND RECEIVE ENABLE
  UCSR0C=(1<<UCSZ01)|(1<<UCSZ00);//ASYNCHRONOUS, 8 BIT TRANSFER
  UBRR0L= 0x67 ; //BAUD RATE 9600
  
}

void usart_send(int ch )

{
  
  while(UCSR0A!=(UCSR0A|(1<<UDRE0)));//waiting for UDRE to become high
  
  UDR0= ch;

}
void dis(char a[100])
{int i=0;
  while(a[i])
  {usart_send(a[i]);
  i++;
}
}


void disp(int a)
{
  int flag=0;
  int t=a;
  char dis[10];
  while(t>0)
  {
    flag++;
    t/=10;
    }
    t=a;
    int g=1,l=0;
    while(t>0)
    {
      l=t%10;
      dis[flag-g]=l+48;
      t/=10;
      g++;
      
      }
    for(int i=0;i<flag;i++)
    usart_send(dis[i]);
  }

int main(void)
{
  sei();
  
  usart_int();
 
  
  
  DDRC|=0b00000000;
  DDRD|=0b11000000;
  PORTD|=0b01000000;
  DDRB|=0b00000010;
  
  
  
  
  TCCR0A|=(1<<WGM01);//initialize timer 0 in CTC
  TCCR0B|=(1<<CS01)|(1<<CS00);//pre-scale 64
  
  TCCR1A |= (1<<COM1A1)|(1<<WGM10);
  TCCR1B |= (1<<CS10) | (1<<WGM12);
  
  OCR0A=250;
  
  OCR1A=255;
  sei();
  TIMSK0=(1<<OCIE0A);
  
  EIMSK|=(1<<INT0);
  EICRA|=(1<<ISC01);
  
 
  
  while(1)
  {
   
  }
  
  return 0;
}
  ISR(TIMER0_COMPA_vect)
  {
    clock_ms++;
   // if(clock_ms == 65535) 
   // {overf++; clock_ms=0;}
    
  }
  
  ISR(INT0_vect)
  {
    count++;
     _delay_ms(200);
    if(count==1)
    {
      
     time0 = clock_ms; 
     
    }
    
    else if(count==2)
    {
     time1 = clock_ms;
     float time = time1+200 - time0;
      rpm = 1000*(60 / time);
      count=0;
      //overf=0;
      clock_ms=0;
      dis("RPM is ");
      disp(rpm);
      usart_send('\n');
     // Serial.println(rpm);
     
     
     if(rpm>=x-3&&rpm<x)
     {
      //do nothing to OCR1A 
     }
      if(rpm>x)
     {
      OCR1A-=(rpm-x)*0.2;
      /* if(rpm>x+30)
       OCR1A-=10;
       else if(rpm>x+15)
       OCR1A-=7;
       else if(OCR1A>=5) 
       OCR1A-=5;
       */
     }
     else if(rpm<x-3)
     {
      OCR1A+=(rpm-x)*0.2;
      /*if(rpm<x-30)
      OCR1A+=10;
      else if(rpm<x-15)
      OCR1A+=7;
     else if(OCR1A<=250) 
      OCR1A+=5; 
     }
     
     */
   
    }
   
  }
  }
  
      
  
  
  
  
  
