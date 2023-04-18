#include <xc.h>
#include "UART_Protocol.h"

unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char* msgPayload)
{
    unsigned char checksum = 0;
    checksum ^= 0xFE;
    checksum ^= (unsigned char)(msgFunction >> 8);
    checksum ^= (unsigned char)(msgFunction >> 0);
    checksum ^= (unsigned char)(msgPayloadLength >> 8);
    checksum ^= (unsigned char)(msgPayloadLength >> 0);
    for (int i = 0; i < msgPayloadLength; i++)
    {
        checksum ^= msgPayload[i];
    }
    return checksum;
}

void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload)
{
unsigned char* message = unsigned char [msgPayloadLength + 6];
            int pos = 0;
            message[pos++] = 0xFE;
            message[pos++] = (unsigned char)(msgFunction >> 8); //MSB
            message[pos++] = (unsigned char)(msgFunction >> 0); //LSB
            message[pos++] = (unsigned char)(msgPayloadLength >> 8);
            message[pos++] = (unsigned char)(msgPayloadLength >> 0);
            for (int i = 0; i < msgPayloadLength; i++)
            {
                message[pos++] = msgPayload[i];
            }
            message[pos++] = CalculateChecksum(msgFunction, msgPayloadLength, msgPayload);
}

int msgDecodedFunction = 0;
int msgDecodedPayloadLength = 0;
unsigned char msgDecodedPayload[128];
int msgDecodedPayloadIndex = 0;


int rcvState = 1;

void UartDecodeMessage(unsigned char c)
{
//Fonction prenant en entree un octet et servant a reconstituer les trames
    switch (rcvState)
            {
                case 1:
                    if (c == 0xFE)
                        rcvState = 2;
                    break;
                case 2:
                    msgDecodedFunction = c << 8;
                    rcvState = 3;

                    break;
                case 3:
                    msgDecodedFunction += c << 0;
                    rcvState = 4;

                    break;
                case 4:
                    msgDecodedPayloadLength = c << 8;
                    rcvState = 5;

                    break;
                case 5:
                    msgDecodedPayloadLength += c << 0;
                    if (msgDecodedPayloadLength == 0)
                    {
                        rcvState = 7;
                    }
                    else
                    {
                        msgDecodedPayloadIndex = 0;
                        msgDecodedPayload = unsigned char [msgDecodedPayloadLength];
                        rcvState = 6;
                    }

                    break;
                case 6:
                    msgDecodedPayload[msgDecodedPayloadIndex++] = c;
                    if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
                        rcvState = 7;
                    break;
                case 7:

                    int calculatedChecksum = CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    int receivedChecksum = c;

                    if (calculatedChecksum == receivedChecksum)
                    {
                        Console.WriteLine("decodage Reussi");
                        //Success, on a un message valide
                    }
                    rcvState = 1;

                    break;
                default:
                    rcvState = 1;
                    break;
}

//void UartProcessDecodedMessage(int function, int payloadLength, unsigned char* payload)
{
//Fonction appelee apres le decodage pour executer l?action
//correspondant au message recu
...
}

//*************************************************************************/
//Fonctions correspondant aux messages
//*************************************************************************/