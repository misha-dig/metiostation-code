#define F_CPU 16000000UL

#define DHT11_PIN 4

#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "adc.h"
#include "calc_values.h"

void config_amdux(){
 ADMUX = (3 << 6) | (0 & 0x07);
}
void blink_on(){
  PORTB |= (1<<PORTB5);
	_delay_ms(1000);
}
void blink_off(){
  PORTB &= ~ (1<<PORTB5);
	_delay_ms(1000);
}

uint8_t c=0,I_RH,D_RH,I_Temp,D_Temp,CheckSum;

void Request()				/* Microcontroller send start pulse/request */
{
	DDRD |= (1<<DHT11_PIN);
	PORTD &= ~(1<<DHT11_PIN);	/* set to low pin */
	_delay_ms(20);			/* wait for 20ms */
	PORTD |= (1<<DHT11_PIN);	/* set to high pin */
}

void Response()				/* receive response from DHT11 */
{
	DDRD &= ~(1<<DHT11_PIN);
	while(PIND & (1<<DHT11_PIN));
	while((PIND & (1<<DHT11_PIN))==0);
	while(PIND & (1<<DHT11_PIN));
}

uint8_t Receive_data()			/* receive data */
{	
	for (int q=0; q<8; q++)
	{
		while((PIND & (1<<DHT11_PIN)) == 0);  /* check received bit 0 or 1 */
		_delay_us(30);
		if(PIND & (1<<DHT11_PIN))/* if high pulse is greater than 30ms */
		c = (c<<1)|(0x01);	/* then its logic HIGH */
		else			/* otherwise its logic LOW */
		c = (c<<1);
		while(PIND & (1<<DHT11_PIN));
	}
	return c;
}

int main(void)
{
  config_amdux();
  usart_init();
  InitADC();
  char data[5];                  // Буфер для дисплея
	DDRB |= (1<<DDB5);
    while (1) 
    {
    //blink_on();
	_delay_ms(15000);
    Request();		/* send start pulse */
		Response();		/* receive response */
    I_RH=Receive_data();	/* store first eight bit in I_RH */
		D_RH=Receive_data();	/* store next eight bit in D_RH */
		I_Temp=Receive_data();	/* store next eight bit in I_Temp */
		D_Temp=Receive_data();	/* store next eight bit in D_Temp */
		CheckSum=Receive_data();/* store next eight bit in CheckSum */
    char str[64];
    int solar=(ReadADC(1)*0.82421875);
    int temp=temp_in_degress(ReadADC(2));
    int photo=ReadADC(3);
    int battary=(int) (ReadADC(4)*0.60205078125);
    int water=ReadADC(7);
    sprintf(str,"solar:%d,analog_temp:%d,photo:%d,battary:%d,water:%d hum:%d,temp_d:%d",solar,temp,photo,battary,water,I_RH  ,I_Temp);
    usart_send_message(str);
	_delay_ms(15000);

    }

}







