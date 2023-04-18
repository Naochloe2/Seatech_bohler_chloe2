/* 
 * File:   UART_Protocol.h
 * Author: TP-EO-5
 *
 * Created on 18 avril 2023, 10:59
 */

#ifndef UART_PROTOCOL_H
#define	UART_PROTOCOL_H

void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload);
void UartDecodeMessage(unsigned char c);
void UartProcessDecodedMessage(int function, int payloadLength, unsigned char* payload);

#endif	/* UART_PROTOCOL_H */

