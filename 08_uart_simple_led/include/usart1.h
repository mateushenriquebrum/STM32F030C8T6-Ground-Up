#ifndef USART1_H
#define USART1_H

void usart1_init(void);
char usart1_read_character(void);
void usart1_write_character(char c);
void usart1_write_string(const char *text);

#endif