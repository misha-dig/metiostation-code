/* Transmit/Receive buffer */
#define UDR0 ((*(volatile unsigned char*)0xC6))
// Can only write as transmit buffer when UDRE0 flag set

/* USART Control and Status Register 0 A */
#define UCSR0A ((*(volatile unsigned char*)0xC0))
// bit 7 - USART Recv Complete [RXC0]
// bit 6 - USART transmit complete [TXC0]
// bit 5 - Data Register Empty [UDRE0]
// bit 4 - Frame error [FE0]
// bit 3 - Data OverRun [DOR0]
// bit 2 - USART Prity Error [UPE0]
// bit 1 - Double USART TX speed [U2X0]
// bit 0 - Multi-processor Comms [MPCM0]

#define UDRE 5

/* USART Control and Status Register 0 B */
#define UCSR0B ((*(volatile unsigned char*)0xC1))
// bit 7 - RX Complete Interrupt Enable 0 [RXCIE0]
// bit 6 - TX Complete Interrupt Enable 0 [TXCIE0]
// bit 5 - USART Data Register Empty IR enable 0 [UDRIE0]
// bit 4 - Receiver Enable 0 [RXEN0]
// bit 3 - Transmitter Enable 0 [TXEN0]
// bit 2 - Char size 0 [UCSZ02]
// bit 1 - Receive Data Bit 8 0 [RXB80]
// bit 0 - Transmit Data bit 8 0 [TCB80]

#define TXEN0 3

/* USART Control and Status Register 0 C */
#define UCSR0C ((*(volatile unsigned char*)0xC2))
// Bit 7/6 - USART Mode Select 0 n [UMSEL0n]
//    00 for asynch
// bit 5/4 USART Partiy mode 0 n [UPM0n]
//    00 for disabled
// bit 3 - USART Stop bit Select 0 [USBS0]
// bit 2 - USART character size/data order [UCSZ01/UDORD0]
// bit 1 - USART character size/clock phase [UCSZ00/UCPHA0]
// bit 0 - Clock Polarity [UCPOL0]

/* USART Baud Rate 0 Register Low */
#define UBRR0L ((*(volatile unsigned char*)0xC4))
/* USART Baud Rate 0 Register High */
#define UBRR0H ((*(volatile unsigned char*)0xC5))

/*
 * Baud rate calculation
 * (cpu_frequency / (desired_baudrate * 16UL)) - 1
 */
#define F_CPU 16000000UL
#define USART_BAUD_RATE 9600

#define BAUD_PRESCALAR (((F_CPU / (USART_BAUD_RATE * 16UL))) - 1)


// NOTE: UBBR is a 12 bit register using 8 bits low and 4 bits high

// file scope variable that acts as a 'private data member'
static char* message;
// can only be changed via 'public method' usart_set_message()

// Sets the private member data message
void usart_send_message(char* msg) {
  message = msg;
    // clear transmit buffer
  UDR0 = 0x00;
  // enable transmit mode
  UCSR0B = (1<<TXEN0);
  for (int idx = 0; message[idx] != 0; idx++) {
    while ((UCSR0A & (1<<UDRE))== 0);
    UDR0 = (char) message[idx];
  }
  while ((UCSR0A & (1<<UDRE))== 0);
  UDR0 = '\r';
  while ((UCSR0A & (1<<UDRE))== 0);
  UDR0 = '\n';
  // disable transmit mode
  UCSR0B = (0 << TXEN0);
}

// initializes the UART for use
void usart_init() {
  // reset USDR0 to empty, emptying RX/TX buffer
  // set baud rate
  UBRR0H = BAUD_PRESCALAR >> 8;
  UBRR0L = BAUD_PRESCALAR;
  // set frame format
  UCSR0C = (0b11<<1); // set to 8 bit format
}



