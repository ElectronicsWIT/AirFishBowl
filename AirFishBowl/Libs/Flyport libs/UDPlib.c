/** \file UDPlib.c
 *  \brief UDP wrapper for FreeRTOS
 *
 * \addtogroup net
 * @{
*/

/**
\defgroup UDPlib UDPlib stack
@{

UDP provides the commands to manage UDP connections. Flyport supports the creation of two different UDP socket, with their own RX buffer.

\section UDPlib UDP library
*/

/* **************************************************************************																					
 *                                OpenPicus                 www.openpicus.com
 *                                                            italian concept
 * 
 *            openSource wireless Platform for sensors and Internet of Things	
 * **************************************************************************
 *  FileName:        UDPlib.c
 *  Dependencies:    Microchip configs files
 *  Module:          FlyPort WI-FI
 *  Compiler:        Microchip C30 v3.12 or higher
 *
 *  Author               Rev.    Date              Comment
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Andrea Seraghiti	 1.0	 1/20/2011         First release (core team)
 *  Andrea Seraghiti	 1.1	 2/12/2011         Fixed UPDRead, reuse of
 *                                                 a closed socket, changed
 *												   ARPResolve control in UDPclient
 *  Simone Marra		 1.2     3/09/2012         Added UDPRxOver, UDPpRead 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  Software License Agreement
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  This is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License (version 2) as published by 
 *  the Free Software Foundation AND MODIFIED BY OpenPicus team.
 *  
 *  ***NOTE*** The exception to the GPL is included to allow you to distribute
 *  a combined work that includes OpenPicus code without being obliged to 
 *  provide the source code for proprietary components outside of the OpenPicus
 *  code. 
 *  OpenPicus software is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details. 
 * 
 * 
 * Warranty
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * WE ARE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 **************************************************************************/

#include "UDPlib.h"

#if MAX_UDP_SOCKETS_FREERTOS>0

static int ToSend = 0; 
/// @cond debug
//*****************************************************************************************
// Only internal use:
// 35 - UDPGenericOpen and callback function: opens a generic UDP socket
//*****************************************************************************************
BYTE UDPGenericOpen(char localport[], NODE_INFO* remhost, char remoteport[] )
{
	BYTE retsock;
	while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);		//	xSemFrontEnd TAKE
	xErr = 0;
	if (xFrontEndStat == 0)
	{	
		xFrontEndStatRet = 2;	
		ToSend=35;
		xUDPLocalPort = atoi(localport);
		xUDPRemotePort = atoi(remoteport);
		udpIPAddress = remhost;
		xQueueSendToBack(xQueue,&ToSend,0);					//	Send COMMAND to the stack
		xFrontEndStat = 1;
		xSemaphoreGive(xSemFrontEnd);						//	xSemFrontEnd GIVE
	}
	else
	{
		xErr = 35; //ToSend;
		xSemaphoreGive(xSemFrontEnd);
		taskYIELD();
		return 0;//Socket creation error
	}
	while (xFrontEndStat != 2);								//	Waits for stack answer
	while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);		//	xSemFrontEnd TAKE
	retsock = callbackUdpSocket;
	xFrontEndStat = 0;
	xSemaphoreGive(xSemFrontEnd);							//	xSemFrontEnd GIVE
	taskYIELD();
	return retsock;
}

//	UDPGenericOpen callback function
int cUDPGenericOpen()
{
	BYTE count = 0;
	UDP_SOCKET tmp_sock = INVALID_UDP_SOCKET;

	udpInt = 0;
	tmp_sock = UDPOpen(xUDPLocalPort, udpIPAddress, xUDPRemotePort);
	if (tmp_sock == INVALID_UDP_SOCKET)
	{
		callbackUdpSocket = 0;
		return 1; // Error;
	}
	else
	{
		numUdpSocket++;
		while (count < MAX_UDP_SOCKETS_FREERTOS)
		{
			if (udpSocket[count] == INVALID_UDP_SOCKET)
			{
				udpSocket[count] = tmp_sock;
				xUDPPort[count] = UDPSocketInfo[tmp_sock].localPort;
				count++;
				callbackUdpSocket = count;
				return 0; // Open succes
			}
			count++;
		}
		callbackUdpSocket = 0;
		return 1; //Error
	}
	
}
/// @endcond


/**
 * UDPLocalPort - Returns the local port of the specified UDP socket. It can be useful when using a socket as client.
 * \param udplocalsocket the number of the UDP socket.
 * \return The number of current socket, or 0 if an error occured during the opening of the socket.
 */
WORD UDPLocalPort (BYTE udplocalsocket)
{			
	WORD dummyPort;
	dummyPort = xUDPPort[udplocalsocket - 1];
	return dummyPort;
}


/**
 * UDPServerOpen - Create a UDP server on specified port
 * \param udpport Local Port for UDP server
 * \return The number of current socket, or 0 if an error occured during the opening of the socket.
 */
BYTE UDPServerOpen (char udpport[])
{			
	return UDPGenericOpen(udpport, NULL , udpport);
}

/**
 * UDPClientOpen - Create a UDP client on specified port, try more time for arp request.
 * \param udpaddr IP address of server
 * \param udpport Remote Port of UDP server
 * \return The number of current socket or 0 if an error occured during the opening of the socket.
 */
BYTE UDPClientOpen (char *udpaddr, char udpport[])
{
    #if defined (FLYPORT)
	if (WFStatus != TURNED_OFF)
    #endif
	{
		static NODE_INFO Server;
		StringToIPAddress((BYTE*) udpaddr, &Server.IPAddr);
		ARPResolveMAC((char*) &(Server.IPAddr));
		vTaskDelay(30);
		if(ARPIsResolved(&Server.IPAddr, &Server.MACAddr))
			return UDPGenericOpen(0, &Server, udpport);
		else 
			return 0;
	}
	return 0;
}

/**
 * UDPBroadcastOpen - Create a UDP broadcast on specified port
 * \param udpport Remote Port for UDP
 * \return The number of current socket or 0 if an error occured during the opening of the socket.
 */
BYTE UDPBroadcastOpen (char udpport[])
{			
	return UDPGenericOpen(0, NULL, udpport);
}


/// @cond debug
//*****************************************************************************************
// Only internal use
// 37 - UDPGenericClose and callback function: close an open UDP socket
//*****************************************************************************************
BYTE UDPGenericClose(BYTE sock)
{
	while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);		//	xSemFrontEnd TAKE
	xErr = 0;
	if (xFrontEndStat == 0)
	{	
		xFrontEndStatRet = 2;
		ToSend=37;
		callbackUdpSocket = sock;
		xQueueSendToBack(xQueue,&ToSend,0);					//	Send COMMAND to the stack
		xFrontEndStat = 1;
		xSemaphoreGive(xSemFrontEnd);						//	xSemFrontEnd GIVE	
		while (xFrontEndStat != 2);							//	Waits for stack answer
		while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);	//	xSemFrontEnd TAKE
		xFrontEndStat = 0;
		xSemaphoreGive(xSemFrontEnd);	
		taskYIELD();
	}
	else
	{
		xErr = 37;
		xSemaphoreGive(xSemFrontEnd);
		taskYIELD();
	}
	return 0;
}

//	Callback function
int cUDPGenericClose()
{
	UDPClose(udpSocket[callbackUdpSocket-1]);
	udpSocket[callbackUdpSocket-1] = INVALID_UDP_SOCKET;
	numUdpSocket--;
	return 0;
}
/// @endcond


/**
 * UDPServerClose - Closes UDP Server socket
 * \param sock UDP Socket number
 */
BYTE UDPServerClose(BYTE sock) 
{
	return UDPGenericClose(sock);
}

/**
 * UDPClientClose - Closes UDP Client socket
 * \param sock UDP Socket number
 */
BYTE UDPClientClose(BYTE sock) 
{
	return UDPGenericClose(sock);
}

/**
 * UDPRxLen - Reads the length of the RX buffer
 * \param sock UDP socket number
 * \return The number of char that can be read from the UDP buffer.
 */
WORD UDPRxLen(BYTE sock)
{
	return udpRxLenGlobal[sock-1];
}

/**
 * UDPRxFlush - Empty the RX buffer
 * \param sock UDP socket number
 * \return none
 */
void UDPRxFlush(BYTE sock)
{
	udpRxLenGlobal[sock-1] = 0;
}

/**
 * UDPRxOver - Checks if a overflow was reached in UDP RX buffer, and clear the flag. 
 * \param sock UDP socket number
 * \return 0 = no overflow, 1 = overflow reached
 */
BOOL UDPRxOver(BYTE sock)
{
	if(UDPoverflowFlag[sock-1] == 1)
	{
		UDPoverflowFlag[sock-1] = 0;
		return 1;
	}
	else
		return 0;
}

/**
 * UDPRead - Reads lstr bytes from the RX buffer
 * \param sock UDP socket number
 * \param str2rd Buffer for data
 * \param lstr lenght of string
 * \return The number of read characters from the specified UDP socket.
 */
int UDPRead(BYTE sock, char str2rd[] , int lstr)
{
    #if defined (FLYPORT)
	if (WFStatus != TURNED_OFF)
    #endif
	{
		BYTE socktmp;
		int tmpread = 0;
		
		socktmp = sock-1;
		if (lstr > BUFFER_UDP_LEN[socktmp])  //overflow
			lstr = BUFFER_UDP_LEN[socktmp];
		
		//doppiaggio dei dati da testare con pulsante
		if (udpRxLenGlobal[socktmp] > BUFFER_UDP_LEN[socktmp])
		{
			udpRxLenGlobal[socktmp] = BUFFER_UDP_LEN[socktmp];
			p_udp_data[socktmp] = p_udp_wifiram[socktmp];
		}

		while(tmpread < lstr)
		{
			str2rd[tmpread] = *p_udp_data[socktmp];
			if ( p_udp_data[socktmp] < (udpBuffer[socktmp]+(BUFFER_UDP_LEN[socktmp]-1)) )
			{
				p_udp_data[socktmp]++;
			}
			else
			{
				p_udp_data[socktmp] = udpBuffer[socktmp];
			}
			tmpread++;
		}
		udpRxLenGlobal[socktmp] -= tmpread;
		return tmpread;
	}
	return 0;
}

/**
 * UDPpRead - Reads lstr bytes from the RX buffer without clear it
 * \param sock UDP socket number
 * \param str2rd Buffer for data
 * \param lstr lenght of string
 * \param start point of start reading (in byte)
 * \return The number of read characters from the specified UDP socket.
 */
int UDPpRead(BYTE sock, char str2rd[], int lstr, int start)
{
	#if defined (FLYPORT)
	if (WFStatus != TURNED_OFF)
    #endif
	{
		BYTE socktmp;
		socktmp = sock-1;
		int tmpread = 0;
		BYTE* peek;
		peek = p_udp_data[socktmp]+start;
		
		
		if (lstr > (BUFFER_UDP_LEN[socktmp]-start))  //overflow
			lstr = BUFFER_UDP_LEN[socktmp]-start;
		
		//doppiaggio dei dati da testare con pulsante
		if (udpRxLenGlobal[socktmp] > BUFFER_UDP_LEN[socktmp])
		{
			udpRxLenGlobal[socktmp] = BUFFER_UDP_LEN[socktmp];
			peek = p_udp_wifiram[socktmp];
		}

		while(tmpread < lstr)
		{
			str2rd[tmpread] = *peek;
			if ( peek < (udpBuffer[socktmp]+(BUFFER_UDP_LEN[socktmp]-1)) )
			{
				peek++;
			}
			else
			{
				peek = udpBuffer[socktmp];
			}
			tmpread++;
		}
		//udpRxLenGlobal[socktmp] -= tmpread;
		return tmpread;
	}
	return 0;
}	

/**
 * UDPWrite - Writes on the UDP socket
 * \param sockwr UDP socket number
 * \param str2wr String to write
 * \param lstr String lenght
 * \return The number of write characters to the specified UDP socket.
 */
WORD UDPWrite(BYTE sockwr, BYTE* str2wr , int lstr)
{
	while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);		//	xSemFrontEnd TAKE
	xErr = 0;
	if (xFrontEndStat == 0)
	{	
		ToSend = 36;
		xFrontEndStatRet = 2;
		callbackUdpSocket = sockwr;
		udpByte = str2wr;
		udpInt = lstr;
		xQueueSendToBack(xQueue,&ToSend,0);					//	Send UDPWrite command to the stack
		xFrontEndStat = 1;
		xSemaphoreGive(xSemFrontEnd);						//	xSemFrontEnd GIVE	
	}
	else
	{
		xErr = 36;
		xSemaphoreGive(xSemFrontEnd);
		taskYIELD();
		return FALSE;
	}
	
	while (xFrontEndStat != 2);								//	Waits for stack answer
	while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);		//	xSemFrontEnd TAKE
	WORD resconn;
	resconn = udpWord;
	xFrontEndStat = 0;
	xSemaphoreGive(xSemFrontEnd);							//	xSemFrontEnd GIVE
	taskYIELD();
	return resconn;
}

/// @cond debug
int cUDPWrite()
{
	//reads udp data and adds in ring buffer
	if ( UDPIsPutReady(udpSocket[callbackUdpSocket-1]) )
	{
		udpWord = UDPPutArray(udpByte,udpInt);
		UDPFlush();
		return 0;
	} else {
		udpWord = 0;
		return 1; //error
	}
}
/// @endcond

#endif
/** @} */
/** @} */
