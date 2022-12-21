void InitADC()
{
    ADMUX |= (1<<REFS0);
    ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);     
}
uint16_t ReadADC(uint8_t ADCchannel)
{
    ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F); 
    ADCSRA |= (1<<ADSC);
    while( ADCSRA & (1<<ADSC) );
   return ADC;
}
