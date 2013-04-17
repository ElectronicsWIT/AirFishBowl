/** \file NETlib.c
 *  \brief Function to manage Wifi and Ethernet modules and to change the network settings at runtime.
 */

/**
\addtogroup net
@{
*/

/**
\defgroup WiFi and Ethernet

@{
<H3>Introduction</H3>
The <B>NET library </B>contains all the functions to manage the Ethernet and WiFi stack. 
It's possible to connect or disconnect from a network, change the settings, and save them inside the falsh memory of the device, to recall at the startup.

<H3>Connection profiles</H3>
A <VAR>connection profile</VAR> contains all the information to connect to or to create a network. 
The Flyport modules have two possible profile to use: a DEFAULT one (fixed in firmware), and a CUSTOM (this profile can be modified at runtime).
<B>WiFi Profiles:</B><UL>
	<LI><B>WF_DEFAULT</B>: contains all the values set in the IDE, with the <B>TCP/IP Setup</B>.</LI>
	<LI><B>WF_CUSTOM</B>: it's the customizable profile. The user can change any parameter and save them in the flash memory of the Flyport. At the device startup it is the same of WF_DEFAULT</LI>
</UL>
<B>Ethernet Profiles:</B><UL>
	<LI><B>ETH_DEFAULT</B>: contains all the values set in the IDE, with the <B>TCP/IP Setup</B>.</LI>
	<LI><B>ETH_CUSTOM</B>: it's the customizable profile. The user can change any parameter and save them in the flash memory of the Flyport. At the device startup it is the same of ETH_DEFAULT</LI>
</UL>

*/
/* **************************************************************************																					
 *                                OpenPicus                 www.openpicus.com
 *                                                            italian concept
 * 
 *            openSource wireless Platform for sensors and Internet of Things	
 * **************************************************************************
 *  FileName:        NETlib.c
 *  Dependencies:    Microchip configs files
 *  Module:          FlyPort WI-FI
 *  Compiler:        Microchip C30 v3.12 or higher
 *
  *  Author               Rev.    Date              Comment
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Gabriele Allegria    1.0     1/20/2011         First release  (core team)
 *  Andrea Seraghiti     1.1     3/25/2011         added WiFi Power Save
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
#include "TCPIP Stack/TCPIP.h"
#include "NETlib.h"
#include "taskTCPIP.h"

#include "libpic30.h"

extern APP_CONFIG NETConf[];

/// @cond debug
//	Frontend variables
extern BYTE xIPAddress[];
extern WORD xTCPPort;
extern TCP_SOCKET xSocket;
extern int xFrontEndStat;
extern int xFrontEndStatRet;
extern int xErr;
extern BOOL xBool;
extern WORD xWord;
extern char *xChar;
extern BYTE *xByte;
extern BYTE xByte2;
extern BYTE xByte3;
extern int xInt;
extern NODE_INFO xNode;

//	RTOS components - Semaphore and queues
extern xQueueHandle xQueue;
extern xSemaphoreHandle xSemFrontEnd;
extern xTaskHandle hTCPIPTask;

extern UINT8 ConnectionProfileID;
/// @endcond

/**
 * NETCustomSave - When the Flyport is powered down, all the changes on the CUSTOM profile will be lost, because are stored in RAM. To prevent the loosing of all the data, you can use the 
 command WFCustomSave. It saves all the network parameters for the CUSTOM connection profile. No need to set anything, the data will be saved in a reserved part of the flash memory, so it will
 be available also if you power off the Flyport.
 * \param None
 * \return None
 */
void NETCustomSave()
{
	_erase_flash(0x2A000);
	int in1=0;
	int *wmem = (int*)NETConf;
	unsigned int offset;

	NVMCON = 0x4003; // Initialize NVMCON
	TBLPAG = 0x2A000>>16; 						// Initialize PM Page Boundary SFR
	offset = 0x2A000 & 0xFFFF; 				// Initialize lower word of address
	__builtin_tblwtl(offset, 0x0A0B);     	// Write to address low word
	
	asm("DISI #5"); 	
	__builtin_write_NVM(); 	

	#ifdef FLYPORTETH
	while (in1 < 47)
	#endif
	#ifdef FLYPORT
	while (in1 < 153)
	#endif
	{

		unsigned long progAddr = 0x2A002+in1; 			// Address of word to program
		unsigned int progDataL = (unsigned int) *(wmem);			// Data to program lower word

		NVMCON = 0x4003; // Initialize NVMCON
		TBLPAG = progAddr>>16; 						// Initialize PM Page Boundary SFR
		offset = progAddr & 0xFFFF; 				// Initialize lower word of address
		__builtin_tblwtl(offset, progDataL);     	// Write to address low word
		
		asm("DISI #5"); 	
		__builtin_write_NVM(); 	

		in1=in1+2;
		wmem++;
	}
}

/**
 * NETCustomDelete - Deletes the custom settings for the network profile CUSTOM.
 * \param None
 * \return None
 */
void NETCustomDelete()
{
	_erase_flash(0x2A000);
}


/**
 * NETCustomExist - Verifies if in memory is present some data for the CUSTOM profile. It can be useful at the startup of the device, because it's possible to control if in a previous session (before any power off)has been saved some configuration data.
 * \param None
 * \return FALSE: no data present.
 * \return TRUE: valid data present.
 */
BOOL NETCustomExist()
{
	int rdoffset;
	int vRead,vRead1;
    TBLPAG = ((0x2A000 & 0x7F0000)>>16);
    rdoffset = (0x2A000 & 0x00FFFF);
	asm("tblrdh.w [%1], %0" : "=r"(vRead1)     : "r"(rdoffset));
    asm("tblrdl.w [%1], %0" : "=r"(vRead)     : "r"(rdoffset));
	if (vRead == 2571)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/**
 * NETCustomLoad - Loads from the flash memory the previously set parameters for the CUSTOM profile. 
 * \param None
 * \return None
 */
void NETCustomLoad()
{
	int in1 = 0;
	int *rmem = (int*) &NETConf[0];
	int rdoffset;
	int vRead,vRead1;
	long int addr1 = 0x2A002;

	#ifdef FLYPORTETH
	while (in1 < 47)
	#endif
	#ifdef FLYPORT
	while (in1 < 153)
	#endif
	{
		TBLPAG = (((addr1+in1) & 0x7F0000)>>16);
		rdoffset = ((addr1+in1) & 0x00FFFF);
		asm("tblrdh.w [%1], %0" : "=r"(vRead1)     : "r"(rdoffset));
		asm("tblrdl.w [%1], %0" : "=r"(vRead)     : "r"(rdoffset));	

		*rmem = vRead;
		in1=in1+2;
		rmem = rmem + 1;
	}
}


/**
 * NETSetParam - With this command is possible to change any network parameter of the CUSTOM profile.
 * \param paramtoset - the parameter to change. 
 * \param paramstring - value of the parameter. <B>Note:</B> Some parameters are available only for Flyport WiFi module.
 * \return None.
 * \paragraph usage Usage
 <B>Sintax of the command to set the parameters: </B>
 <UL>
	<LI><B>IP address of the device:</B> NETSetParam(MY_IP_ADDR , string with the IP address, for example "192.168.1.100")</LI><BR>
	<LI><B>Primary DNS server:</B> NETSetParam(PRIMARY_DNS , string with the IP address, for example "192.168.1.1")</LI><BR>
	<LI><B>Secondary DNS server:</B> NETSetParam(SECONDARY_DNS , string with the IP address, for example "192.168.1.1")</LI><BR>
	<LI><B>Default gateway:</B> NETSetParam(MY_GATEWAY , string with the IP address, for example "192.168.1.1")</LI><BR>
	<LI><B>Subnet mask:</B> NETSetParam(SUBNET_MASK , string with the subnet mask, for example "255.255.255.0")</LI><BR>
	<LI><B>Netbios name:</B> NETSetParam(NETBIOS_NAME , string with the Netbios name, for example "Flyport")</LI><BR>
	<LI><B>DHCP client enabled or not:</B> NETSetParam(DHCP_ENABLE , ENABLED or DISABLED)</LI><BR>
	<LI><B>SSID:</B> NETSetParam(SSID_NAME , string with the SSID name, for example "FlyportNet") <B>Note:</B> Only for Flyport WiFi!</LI><BR>
	<LI><B>Network type (adhoc or infrastructure):</B> NETSetParam(NETWORK_TYPE , ADHOC or INFRASTRUCTURE) <B>Note:</B> Only for Flyport WiFi!</LI> <BR>
 </ul>
 */
void NETSetParam(int paramtoset , char paramstring[])
{
	int ind1;
	BOOL nbns_flag = FALSE;
	switch (paramtoset)
	{
	//	Changing Flyport IP address
	case MY_IP_ADDR:
		StringToIPAddress((BYTE*) paramstring, &(NETConf[0].DefaultIPAddr));
		StringToIPAddress((BYTE*) paramstring, &(NETConf[0].MyIPAddr));
		break;
	//	Changing primary DNS server
	case PRIMARY_DNS:
		StringToIPAddress((BYTE*) paramstring, &(NETConf[0].PrimaryDNSServer));
		break;
	//	Changing secondary DNS server
	case SECONDARY_DNS:
		StringToIPAddress((BYTE*) paramstring, &(NETConf[0].SecondaryDNSServer));	
		break;		
	//	Changing Gateway
	case MY_GATEWAY:
		StringToIPAddress((BYTE*) paramstring, &(NETConf[0].MyGateway));
		break;
	//	Subnet mask
	case SUBNET_MASK:
		StringToIPAddress((BYTE*) paramstring, &(NETConf[0].MyMask));	
		StringToIPAddress((BYTE*) paramstring, &(NETConf[0].DefaultMask));
		break;
	//	Netbios name
	case NETBIOS_NAME:
		for (ind1 = 0; ind1 < 15; ind1++)
		{
			if ( (paramstring[ind1] != '\0') && (!nbns_flag) )
			{
				// UPPERCASING NETBIOS NAME
				if ( (paramstring[ind1] > 96) && (paramstring[ind1] < 123) )
					NETConf[0].NetBIOSName[ind1] = paramstring[ind1]-32;
				else
					NETConf[0].NetBIOSName[ind1] = paramstring[ind1];
			}
			else if (paramstring[ind1] == '\0')
				nbns_flag = TRUE;
	
			if (nbns_flag)
				NETConf[0].NetBIOSName[ind1] = 0x0020;
		}
		NETConf[0].NetBIOSName[15] = '\0';
		break;
	//	DHCP enabling
	case DHCP_ENABLE:
		if (!strcmp(paramstring,"On"))
			NETConf[0].Flags.bIsDHCPEnabled = TRUE;
		else if (!strcmp(paramstring,"Off"))
			NETConf[0].Flags.bIsDHCPEnabled = FALSE;
		break;
	#ifdef FLYPORT
	//	SSID name
	case SSID_NAME:
		strcpy( (char *) NETConf[0].MySSID,paramstring);
		NETConf[0].SsidLength = strlen( (char *) NETConf[0].MySSID);
		break;	
	case NETWORK_TYPE:
		if (!strcmp(paramstring,"infra"))
			NETConf[0].networkType = WF_INFRASTRUCTURE;
		else if (!strcmp(paramstring,"adhoc"))
			NETConf[0].networkType = WF_ADHOC;
	#endif
	}
}

/// #cond debug
static int ToSend = 0; 
/****************************************************
 *	ETHERNET MANAGEMENT
 ****************************************************/
#if defined (FLYPORTETH)
/// @endcond
/**
 * ETHRestart - Connects the Flyport Ethernet Module to the network with the specified profle.
 * \param pconn - Specifies the profile used to connect to the network. The following profile are available:
				  ETH_DEFAULT : uses the settings choosen in the IDE TCP/IP setup. This profile cannot be changed.
				  ETH_CUSTOM : this profile can be customized by the user and even saved in the flash memory. At the startup is identical to the default, but you can change it anytime you want.
 * \return None
 */
void ETHRestart(int ethprofile)
{
	BOOL opok = FALSE;
	while (!opok)
	{
		while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);			//	xSemFrontEnd TAKE
		if (xFrontEndStat == 0)
		{	
			ToSend = 1;
			xFrontEndStatRet = 2;
			xInt = ethprofile;
			xQueueSendToBack(xQueue,&ToSend,0);						//	Send function code command to the stack
			xFrontEndStat = 1;
			xSemaphoreGive(xSemFrontEnd);							//	xSemFrontEnd GIVE		
			opok = TRUE;
		}
		else
		{
			xSemaphoreGive(xSemFrontEnd);
			taskYIELD();
		}
	}

	while (xFrontEndStat != 2);								//	Waits for stack answer
	while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);		//	xSemFrontEnd TAKE

	xFrontEndStat = 0;
	xSemaphoreGive(xSemFrontEnd);							//	xSemFrontEnd GIVE
	taskYIELD();
}

int cETHRestart()
{
	AppConfig = NETConf[xInt];
	MACInit();
	return 0;
}

/// @cond debug
#endif


/****************************************************
 *	WIFI MANAGEMENT
 ****************************************************/
#if defined (FLYPORT)

extern tWFNetwork xNet;
extern int WFStatusold;


extern void	WF_Connect(int);


void WFGeneric(int function)
{
	BOOL opok = FALSE;
	while (!opok)
	{
		while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);			//	xSemFrontEnd TAKE
		if (xFrontEndStat == 0)
		{	
			ToSend = function;
			xFrontEndStatRet = 2;

			xQueueSendToBack(xQueue,&ToSend,0);						//	Send function code command to the stack
			xFrontEndStat = 1;
			xSemaphoreGive(xSemFrontEnd);							//	xSemFrontEnd GIVE		
			opok = TRUE;
		}
		else
		{
			xSemaphoreGive(xSemFrontEnd);
			taskYIELD();
			//	If WiFi module if turned OFF, function doesn't do anything
			if (xFrontEndStat == -1)
				return;
		}
	}

	while (xFrontEndStat != 2);								//	Waits for stack answer
	while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);		//	xSemFrontEnd TAKE

	xFrontEndStat = 0;
	xSemaphoreGive(xSemFrontEnd);							//	xSemFrontEnd GIVE
	taskYIELD();
}
/// @endcond

/**
 * WFConnect - Connects the Flyport WiFi Module to the network with the specified profle.
 * \param pconn - Specifies the profile used to connect to the network. The following profile are available:
				  WF_DEFAULT : uses the settings choosen in the IDE TCP/IP setup. This profile cannot be changed.
				  WF_CUSTOM : this profile can be customized by the user and even saved in the flash memory. At the startup is identical to the default, but you can change it anytime you want.
 * \return None
 */
void WFConnect(int pconn)
{
	while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);		//	xSemFrontEnd TAKE
	if (WFStatus == NOT_CONNECTED)
	{
		BOOL opok = FALSE;
		while (!opok)
		{
			WFStatus = CONNECTING;
			if (xFrontEndStat == 0)
			{	
				ToSend = 1;
				xFrontEndStatRet = 2;
				xInt = pconn;
				
				xQueueSendToBack(xQueue,&ToSend,0);					//	Send TCPIsConnected command to the stack
				xFrontEndStat = 1;
				xSemaphoreGive(xSemFrontEnd);						//	xSemFrontEnd GIVE	

				while (xFrontEndStat != 2);								//	Waits for stack answer
				while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);		//	xSemFrontEnd TAKE

				xFrontEndStat = 0;
				xSemaphoreGive(xSemFrontEnd);							//	xSemFrontEnd GIVE
				taskYIELD();
				opok = TRUE;
			}
			else
			{
				xSemaphoreGive(xSemFrontEnd);
				taskYIELD();
				//	If WiFi module if turned OFF, function doesn't do anything
				if (xFrontEndStat == -1)
				{
					WFStatus = NOT_CONNECTED;
					return;
				}
			}
		}
	}
	else
	{
		xSemaphoreGive(xSemFrontEnd);
		taskYIELD();	
	}
}


/**
 * WFDisconnect - Disconnects the device from the network. No parameters required.
 * \param None
 * \return None
 */
void WFDisconnect()
{
	WFGeneric(2);
}



/**
 * WFScan - Starts a scan to detect all the WiFi networks available. The function doesn't return anything. When it has finished, it generates an event, you can catch in the WiFi events file.
 * \param None
 * \return None. The number of the retrieved WiFi networks found is passed in the function WF_events like an "event info". All the data for the retrieved networks can be accessed with the command WFScanList.
 * \attention The command must be issued when the device is not connected to any network, otherwise it won't give any error message, but it won't work.
 */
void WFScan()
{
	if ( (WFStatus != CONNECTING) && (WFStatus != SCANNING) )
		WFGeneric(3);
}



/**
 * WFScanList - This command must be issued after a WFScan request has been completed (so the event is generated). The WFScan command returns in the event handler the number of the WiFi networks found,
 but all the data related to the networks, can be accessed using the function WFScanList. 
 * \param ntscn - number of the wifi network (1 to number of the found networks).
 * \return A tWFNetwork structure, which contains all the informations about the specified network.
 * \warning The function can't be called inside the WiFi Event handler. You always must call it from the FlyportTask.
 */
tWFNetwork WFScanList(int ntscn)
{
	if (WFNetworkFound != 0)
	{
		BOOL opok = FALSE;
		while (!opok)
		{
			while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);			//	xSemFrontEnd TAKE
			if (xFrontEndStat == 0)
			{	
				ToSend = 7;
				xInt = ntscn-1;
				xFrontEndStatRet = 2;
				xQueueSendToBack(xQueue,&ToSend,0);						//	Send WFScanList command to the stack
				xFrontEndStat = 1;
				xSemaphoreGive(xSemFrontEnd);							//	xSemFrontEnd GIVE	
				opok = TRUE;
			}
			else
			{
				xSemaphoreGive(xSemFrontEnd);
				taskYIELD();
				//	If WiFi module if turned OFF, function doesn't do anything
				if (xFrontEndStat == -1)
					return xNet;
			}
		}
		
		while (xFrontEndStat != 2);								//	Waits for stack answer
		while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);		//	xSemFrontEnd TAKE
		
		tWFNetwork netret;
		xFrontEndStat = 0;
		netret = xNet;
		xSemaphoreGive(xSemFrontEnd);							//	xSemFrontEnd GIVE
		taskYIELD();
		return netret;
	}
	return xNet;
}



/**
 * WFStopConnecting - When the command WFConnect is lauched, the device tries to connect to the selected WiFi network until it doesn't find it.
 *  If you want to stop the retries, you have to issu the command WFStopConnecting.
 * \param None
 * \return None
 */
void WFStopConnecting()
{
	while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);		//	xSemFrontEnd TAKE
	if (WFStatus == CONNECTING)
	{
		BOOL opok = FALSE;
		while (!opok)
		{
			if (xFrontEndStat == 0)
			{	
				ToSend = 10;
				xFrontEndStatRet = 2;
				
				xQueueSendToBack(xQueue,&ToSend,0);					//	Send TCPIsConnected command to the stack
				xFrontEndStat = 1;
				xSemaphoreGive(xSemFrontEnd);						//	xSemFrontEnd GIVE	
				opok = TRUE;
			}
			else
			{
				xSemaphoreGive(xSemFrontEnd);
				taskYIELD();
				//	If WiFi module if turned OFF, function doesn't do anything
				if (xFrontEndStat == -1)
					return;
			}
		}
	
		while (xFrontEndStat != 2);								//	Waits for stack answer
		while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);		//	xSemFrontEnd TAKE

		xFrontEndStat = 0;
		xSemaphoreGive(xSemFrontEnd);							//	xSemFrontEnd GIVE
		taskYIELD();
	}
	else
	{
		xSemaphoreGive(xSemFrontEnd);
		taskYIELD();	
	}
}



/**
 * WFSetSecurity - This command is used to set all the security parameters for the WF_CUSTOM connection profile
 * \param mode - the security mode. Valid security mode are the following:<BR>
 <UL>
	<LI><B>WF_SECURITY_OPEN:</B> no security.</LI>
	<LI><B>WF_SECURITY_WEP_40:</B> WEP security, with 40 bit key.</LI>
	<LI><B>WF_SECURITY_WEP_104:</B> WEP security, with 104 bit key.</LI>
	<LI><B>WF_SECURITY_WPA_WITH_KEY:</B> WPA-PSK personal security, the user specifies the hex key.</LI>
	<LI><B>WF_SECURITY_WPA_WITH_PASS_PHRASE:</B> WPA-PSK personal security, the user specifies only the passphrase.</LI>
	<LI><B>WF_SECURITY_WPA2_WITH_KEY:</B> WPA2-PSK personal security, the user specifies the hex key.</LI>
	<LI><B>WF_SECURITY_WPA2_WITH_PASS_PHRASE:</B>WPA2-PSK personal security, the user specifies only the passphrase.</LI>
	<LI><B>WF_SECURITY_WPA_AUTO_WITH_KEY:</B> WPA-PSK personal or WPA2-PSK personal (the Flyport will auto select the mode) with hex key.</LI>
	<LI><B>WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE:</B> WPA-PSK personal or WPA2-PSK personal (autoselect) with pass phrase.</LI>
 </UL>	
 * \param keypass - the key or passphrase for the network. A key must be specified also for open connections (you can put a blank string, like "").
 * \param keylen - length of the key/passphrase. Must be specified also for open connections (can be 0).
 * \param keyind - index of the key (used only for WEP security, but must be specified also for all others, in that case, can be 0).
 * \attention For WPA/WPA2 with passphrase, the Flyport must calculate the hex key. The calculation is long and difficult, so it will take
 * about 20 seconds to connect!
 * \return None.
 */	
void WFSetSecurity(BYTE mode , char * keypass , BYTE keylen , BYTE keyind)
{
	NETConf[0].SecurityMode = mode;
	NETConf[0].SecurityKeyLength = (BYTE) keylen;
	NETConf[0].WepKeyIndex = keyind;
	int isec=0;

	for (isec=0 ; isec<keylen ; isec++)
	{
		NETConf[0].SecurityKey[isec] = *(keypass + isec);
	}
}

/**
 * WFHibernate - Enables Hibernate mode on the MRF24WB0M, which effectively turns off the WiFi. The microcontroller is still completely running, so all the modules are working (UART, SPI, analog inputs...). To turn on again the module, use the command WFOn().
 * \param None
 * \return None
 */
void WFHibernate()
{
	WFStatus = TURNED_OFF;
	if (hTCPIPTask != NULL)
	{
		LATEbits.LATE6 = 1;				// Wi-Fi module hibernation
		vTaskDelete(hTCPIPTask);		// TCP task delete
		hTCPIPTask = NULL;
	}
	xFrontEndStat = -1;					// Frontend = -1 blocks all TCP/IP system calls
}


/**
 * WFSleep - Turns off the WiFi module and put the microcontroller in sleep mode, to obtain maximum power saving mode. 
 * \param none
 * \return None
  * \warning: After the command is issued, no the module is completely in sleep mode, no modules are active (UART, SPI, analog inputs...). The only way to awake the module is issue an external interupt. So before the WFSleep command, an interrupt must be set.
 */
void WFSleep()
{
	WFStatus = TURNED_OFF;
	if (hTCPIPTask != NULL)
	{
		LATEbits.LATE6 = 1;				// Wi-Fi module hibernation
		vTaskDelete(hTCPIPTask);		// TCP task delete
		hTCPIPTask = NULL;
	}
	xFrontEndStat = -1;					// Frontend = -1 blocks all TCP/IP system calls
	// PIC sleep mode
	RCONbits.VREGS = 0;
	vTaskDelay(50);
	asm("PWRSAV #0");
}


/**
 * WFOn - Turns on the WiFi module after a WFSleep() or WFHibernate() command.
 * \param none
 * \return None
 */
void WFOn()
{
	if (WFStatus == TURNED_OFF)
	{
		LATEbits.LATE6 = 0;					// WiFi module activation
		xFrontEndStat = 0;					// Frontend var = 0 to enable again all the TCP/IP system call
		
		// Creating again TCP/IP task
		if (hTCPIPTask == NULL)
		{
			xTaskCreate(TCPIPTask, (signed char*) "TCP", STACK_SIZE_TCPIP,
				NULL, tskIDLE_PRIORITY + 1, &hTCPIPTask);
		}
	}
}


/**
 * WFPsPollEnable - Enables or Disable Power-Save Pool at runtime
 * \param ps_active TRUE to enable Poll mode for longer battery life or FALSE to disable PS Poll mode, MRF24WB0M will stay active and not go sleep.
 * \return None
 */
void WFPsPollEnable(BOOL ps_active)
{
	BOOL opok = FALSE;
	while (!opok)
	{
		while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);			//	xSemFrontEnd TAKE
		xErr = 0;
		if (xFrontEndStat == 0)
		{	
			if (ps_active)
				ToSend = 6; //cWFPsPollEnable
			else
				ToSend = 5; //cWFPsPollDisable
			xFrontEndStatRet = 2;

			xQueueSendToBack(xQueue,&ToSend,0);						//	Send function code command to the stack
			xFrontEndStat = 1;
			xSemaphoreGive(xSemFrontEnd);							//	xSemFrontEnd GIVE	
			opok = TRUE;
		}
		else
		{
			if (ps_active)
				xErr = 6; //cWFPsPollEnable
			else
				xErr = 5; //cWFPsPollDisable
			xSemaphoreGive(xSemFrontEnd);
			taskYIELD();
			//	If WiFi module if turned OFF, function doesn't do anything
			if (xFrontEndStat == -1)
				return;
		}
	}
	
	while (xFrontEndStat != 2);								//	Waits for stack answer
	while (xSemaphoreTake(xSemFrontEnd,0) != pdTRUE);		//	xSemFrontEnd TAKE

	xFrontEndStat = 0;
	xSemaphoreGive(xSemFrontEnd);							//	xSemFrontEnd GIVE
	taskYIELD();
}

/// @cond debug
//****************************************************************************
//	Only internal use:
//	WFDisconnect callback function
//****************************************************************************
int cWFDisconnect()
{
	if (WFStatus == CONNECTED)
	{
		WF_CMDisconnect();
		WFStatus = NOT_CONNECTED;
	}
	if (WFStatus == CONNECTION_LOST)
		WFStatus = NOT_CONNECTED;
	return 0;
}


//****************************************************************************
//	Only internal use:
//	WFScan callback function
//****************************************************************************
int cWFScan()
{
	WFStatusold = WFStatus;
	WFStatus = SCANNING;
	WF_Scan(WF_SCAN_ALL);
	return 0;
}

//****************************************************************************
//	Only internal use:
//	WFScanList callback function
//****************************************************************************
int cWFScanList()
{

	int x1;
	tWFScanResult  Sc1out;	
	WF_ScanGetResult(xInt,&Sc1out);

	//	SSID of the network

	for (x1 = 0 ; x1 < Sc1out.ssidLen ; x1++)
		xNet.ssid[x1] = (char)Sc1out.ssid[x1];
	xNet.ssid[Sc1out.ssidLen] = '\0';
	
	// Type of the network (WF_INFRASTRUCTURE or WF_ADHOC)
	xNet.type = Sc1out.bssType;
	
	// Signal strength
	xNet.signal = Sc1out.rssi;
	
	// Channel
	xNet.channel = Sc1out.channel;
	
	// Beacon period
	xNet.beacon = Sc1out.beaconPeriod;
	
	// BSSID
	for (x1 = 0 ; x1 < WF_BSSID_LENGTH ; x1++)
		xNet.bssid[x1] = (char)Sc1out.bssid[x1];
		
	// Security
	if ( (Sc1out.apConfig & 16) == 0)
		xNet.security = WF_SECURITY_OPEN;
	else
	{
		if ( (Sc1out.apConfig & 128) != 0)
			xNet.security = WF_SECURITY_GENERIC_WPA2;	
		else 
			if ( (Sc1out.apConfig & 64) != 0)
				xNet.security = WF_SECURITY_GENERIC_WPA; 
			else
				xNet.security = WF_SECURITY_GENERIC_WEP; 	
	}

	return 0;
}




//****************************************************************************
//	Only internal use:
//	WFStopConnecting callback function
//****************************************************************************
int cWFStopConnecting()
{
	WFStatus = STOPPING;
	return 0;
}


//****************************************************************************
//	Only internal use:
//	WFConnect callback function
//****************************************************************************
int cWFConnect()
{
	WF_Connect(xInt);
	return 0;
}


//****************************************************************************
//	Only internal use:
//	WFPsPollDisable callback function
//****************************************************************************
int cWFPsPollDisable()
{
	WF_PsPollDisable();
	return 0;
}
//****************************************************************************
//	Only internal use:
//	WFPsPollEnable callback function
//****************************************************************************
int cWFPsPollEnable()
{
	WF_PsPollEnable(TRUE);
	return 0;
}
/// @endcond
#endif

