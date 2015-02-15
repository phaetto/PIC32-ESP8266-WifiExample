/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#ifdef __XC32
    #include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif

#include <math.h>
#include <plib.h>            /* Include to use PIC32 peripheral libraries     */
#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */

#include "user.h"           /* variables/params used by user.c               */

// Private
#define USER_AGENT "PIC32MX795+ESP8266"

AccessPointConnection routerConnection =
{
    "[SSDI]",
    "[PASSWORD]"
};

// Example request mapped to http://myip.dnsdynamic.org/
const char DnsDynamicHttpRequestString[] = "GET / HTTP/1.1" CMD_CRLF
    "Host: myip.dnsdynamic.org" CMD_CRLF
    "User-Agent: " USER_AGENT CMD_CRLF
    "Connection: close" CMD_CRLF
    "Cache-Control: no-cache" CMD_CRLF
    "Content-Type: text/html" CMD_CRLF
    CMD_CRLF "\0";

void OnDnsDynamicResponse(const char *string);

TcpRequest DnsDynamicTcpRequest =
{
    DnsDynamicHttpRequestString,
    sizeof(DnsDynamicHttpRequestString),
    "myip.dnsdynamic.org",
    80,
    OnDnsDynamicResponse
};

void OnDnsDynamicResponse(const char *string) {
    // This is the tcp response, so we will get all the http parts as well.

    // Do something with the response here

    WriteString(CMD_MAKEYELLOW);
    WriteString(CMD_MAKEBOLD);
    WriteString(string);
    WriteString(CMD_CLEARATTRIBUTES);
};

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

void InitApp(void)
{
    /* Initialize peripherals */

    // Configure UART modules
    UARTConfigure(UART_CMD_MODULE_ID, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART_CMD_MODULE_ID, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART_CMD_MODULE_ID, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART_CMD_MODULE_ID, GetPeripheralClock(), DESIRED_CMD_BAUDRATE);
    UARTEnable(UART_CMD_MODULE_ID, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

    UARTConfigure(UART_WIFI_MODULE_ID, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART_WIFI_MODULE_ID, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART_WIFI_MODULE_ID, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART_WIFI_MODULE_ID, GetPeripheralClock(), DESIRED_WIFI_BAUDRATE);
    UARTEnable(UART_WIFI_MODULE_ID, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

    // Configure UART Interrupts
    INTEnable(INT_SOURCE_UART_RX(UART_CMD_MODULE_ID), INT_ENABLED);
    INTSetVectorPriority(INT_VECTOR_UART(UART_CMD_MODULE_ID), INT_PRIORITY_LEVEL_2);
    INTSetVectorSubPriority(INT_VECTOR_UART(UART_CMD_MODULE_ID), INT_SUB_PRIORITY_LEVEL_0);

    INTEnable(INT_SOURCE_UART_RX(UART_WIFI_MODULE_ID), INT_ENABLED);
    INTSetVectorPriority(INT_VECTOR_UART(UART_WIFI_MODULE_ID), INT_PRIORITY_LEVEL_1);
    INTSetVectorSubPriority(INT_VECTOR_UART(UART_WIFI_MODULE_ID), INT_SUB_PRIORITY_LEVEL_0);

    // Make the requests to Wifi service - they will be picked up when the service needs them
    ConnectToAccessPoint(&routerConnection, &DefaultWifiService);
    SendRequest(&DnsDynamicTcpRequest, &DefaultWifiService);
}

// UART related methods

void DefaultWifiWriteString(const char *string)
{
    while(*string != '\0')
    {
        while(!UARTTransmitterIsReady(UART_WIFI_MODULE_ID));
        UARTSendDataByte(UART_WIFI_MODULE_ID, *string);
        string++;
        while(!UARTTransmissionHasCompleted(UART_WIFI_MODULE_ID));
    }
}

void WriteString(const char *string)
{
    while(*string != '\0')
    {
        while(!UARTTransmitterIsReady(UART_CMD_MODULE_ID));
        UARTSendDataByte(UART_CMD_MODULE_ID, *string);
        string++;
        while(!UARTTransmissionHasCompleted(UART_CMD_MODULE_ID));
    }
}

void DefaultErrorToOutput(const char *string) {
    WriteString(CMD_MAKEYELLOW);
    WriteString(CMD_MAKEBOLD);
    WriteString(string);
    WriteString(CMD_CLEARATTRIBUTES);
};

void PutCharacter(const char character)
{
    while(!UARTTransmitterIsReady(UART_CMD_MODULE_ID));
    UARTSendDataByte(UART_CMD_MODULE_ID, character);
    while(!UARTTransmissionHasCompleted(UART_CMD_MODULE_ID));
}

