#ifndef __UART_H__
#define __UART_H__

void uart_init(unsigned baud);
void uart_putc(char c);
void uart_puts(char *s);
char uart_getc(void);

#endif
