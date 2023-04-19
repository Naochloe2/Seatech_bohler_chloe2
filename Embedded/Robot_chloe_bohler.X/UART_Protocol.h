/* 
 * File:   UART_Protocol.h
 * Author: TP-EO-5
 *
 * Created on 18 avril 2023, 10:59
 */

#ifndef UART_PROTOCOL_H
#define	UART_PROTOCOL_H
#define SET_ROBOT_STATE 0x0051
#define SET_ROBOT_MANUAL_CONTROL 0x0052

unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char* msgPayload);
void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload);
void UartDecodeMessage(unsigned char c);
void UartProcessDecodedMessage(int function, int payloadLength, unsigned char* payload);

#endif	/* UART_PROTOCOL_H */

