/** \file HWlib.c
 *  \brief Hardware library to manage the analog and digital IOs and UART
 */

/**
\addtogroup Hardware
@{
*/

/* **************************************************************************																					
 *                                OpenPicus                 www.openpicus.com
 *                                                            italian concept
 * 
 *            openSource wireless Platform for sensors and Internet of Things	
 * **************************************************************************
 *  FileName:        HWlib.c
 *  Dependencies:    Microchip configs files
 *  Module:          FlyPort WI-FI
 *  Compiler:        Microchip C30 v3.12 or higher
 *
 *  Author               Rev.    Date              Comment
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Gabriele Allegria    1.0     1/20/2011		   First release  (core team)
 *  Stefano Cappello 							   Added I2C + support in PWM
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
/// @cond debug
#ifndef __SOFTLIB_C
#define __SOFTLIB_C

#include "HWlib.h"
#include "p24FJ256GA106.h"
#include "TCPIP Stack/tick.h"


/*****************************************************************************
*									SECTION:								 *
*	Hardware defines and mapping.											 *
*****************************************************************************/
/*****************************************************************************
 *						--- PORT REGISTER MAPPING ---					 	 *
 ****************************************************************************/	
#define IOPINS	(10)

extern int *LATs[];	
extern int *TRISs[];	
extern int *PORTs[];
extern int *CNPUs[];
extern int *OCCON1s[];
extern int *OCCON2s[];
extern int *OCRs[];
extern int *OCRSs[];
extern int *TCONs[];
extern int *RPORs[];
extern int *RPIRs[];

extern BOOL RPIRPos[];
extern int RPIORPin[];
extern int RPFunc[];
extern int OCM[];
						
extern int *CNPDs[];
extern int IOMode[26];

extern int CNPos[];

extern int IOPos[];

extern int an[];

extern int *UMODEs[];
extern int *USTAs[];
extern int *UBRGs[];
extern int *UIFSs[];
extern int *UIECs[];
extern int *UTXREGs[];
extern int *URXREGs[];

extern int UTXIPos[];
extern int URXIPos[];
				
extern int *AD1CFGL;
extern int *AD1CFGH;
extern int *AD1CONF1;
extern int *AD1CONF2;
extern int *AD1CONF3;
extern int *AD1CH;
extern int *AD1CSL;

static int bufind_w[4];
static int bufind_r[4];
static int Status[26];
static char Buffer[UART_PORTS][UART_BUFFER_SIZE];
static char buffover [4];
static BYTE last_op[4];

static int OCTimer[9];
static char OCTSel[9];
BOOL TimerOn[5];

static unsigned int period;
/// @endcond



/**
\defgroup ADC
@{
The ADC library contains the command to manage the ADC, so it's possible to easy convert an analog voltage value. 
The precision of the ADC is 10 bit.

*/

/**
 * ADCInit - initializes the ADC module with default values. <B>NOTE:</B> this function is already called at Flyport startup.
 * \param None
 * \return None
 */
void ADCInit()
{
	*AD1CFGL = ADCCHANNELSL;		//	ADC channels enabled
	*AD1CFGH = 0x3;					//	Internal band gap disabled for scan
	
	//	AD1CON1 settings:
	//	ADC OFF, continue operation in idle mode, output like integer, autoconv.
	*AD1CONF1 = 0x00E0;	
	//AD1CON1bits.ASAM = 0;

	//	AD1CHS settings:
	//	MUX A inputs Vref+ and Vref-.
	*AD1CH = 0x0; 
	*AD1CSL = 0x0;	
	//	AD1CON3 settings:
	//	Clock derived from system clock, auto sample: 1Tad, conv. clock = 5 * Tcy
	*AD1CONF3 = 0x0F10;	
	
	//	AD1CON2 settings:
	//	External Vref, input scan, 2 sequences per interrupt, buffer is 16bit wide
	//	only MUX A used.
	*AD1CONF2 = 0x6000;
	
	//ADCVref = Vref2V;
	IPC3bits.AD1IP = 7;
	IFS0bits.AD1IF  = 0;
	IEC0bits.AD1IE  = 1;
	AD1CON1bits.ADON = 1;
}

static int AD_val = 0;
static BOOL AD_flag = FALSE;

void __attribute__ ((__interrupt__, no_auto_psv)) _ADC1Interrupt(void)
{
	IFS0bits.AD1IF  = 0;
	AD1CON1bits.ASAM = 0;
	AD_val = ADC1BUF0;
	AD_flag = TRUE;
}
 /**
 * ADCVal - Reads the value of the analog channel specified. 
 * \param ch - the number of the analog channel to read. For the number of the channel, refer to the Flyport pinout.
 * \return the value read by the function.
 */
int ADCVal(int ch)
{
	*AD1CH = an[ch];
	// Added to stability correction 
	// on using ADCVal with webserver active

 	IFS0bits.AD1IF = 0;	
	AD_flag = FALSE;
	AD1CON1bits.ASAM = 1;
	while (!AD_flag);
	return AD_val;
}
/*! @} */


/**
\defgroup GPIOs
@{

The GPIOs commands can be used the manage the IO pins, to configure, to change or to read their values.
*/

/**
 * IOPut - Puts the putval value on the specified IO output pin. 
 * \param io - specifies the ouput pin.
 * \param putval - the value to assign to the pin:
 <UL>
	<LI><B>on (or ON, or 1)</B> high level (about 3.3V).</LI>
	<LI><B>off (or OFF, or 0)</B> low level (0V).</LI>	
 * \return None
 */
void IOPut(int io, int putval)
{	
	io--;
	int addval = 0;
	switch(putval)
	{
	//	Output clear
	case 0:
		addval = 1 << IOPos[io];
		addval = ~addval;
		*LATs[io] = *LATs[io] & addval;
		Status[io] = 0; 
		break;
		
	//	Output set
	case 1:
		addval = 1 << IOPos[io];
		*LATs[io] = *LATs[io] | addval;
		Status[io] = 1;
		break;
		
	//	Output toggle
	case 2:
		if (Status[io] == 1)
		{
			addval = 1 << IOPos[io];
			addval = ~addval;
			*LATs[io] = *LATs[io] & addval;	
			Status[io] = 0;
		}
		else
		{
			addval = 1 << IOPos[io];
			*LATs[io] = *LATs[io] | addval;	
			Status[io] = 1;
		}
		break;
	}
}


/**
 * IOInit - Initializes the specified pin like output, input, input with pull-up or pull-down resistor.
 * \param io - specifies the pin.
 * \param putval - specifies how the pin must be initialized. The valid parameters are the following:
 <UL>
	<LI><B>in (or IN)</B> input pin.</LI>
	<LI><B>inup (or INUP)</B> input pin with pullup resistor (about 5 KOhm).</LI>	
	<LI><B>indown (or INDOWN)</B> input pin with pulldown resistor (about 5 Kohm).</LI>
	<LI><B>out (or OUT)</B> output pin.</LI>
	<LI><B>UART1RX</B> UART1 RX input pin.</LI>
	<LI><B>UART1CTS</B> UART1 CTS input pin.</LI>
	<LI><B>UART2RX</B> UART2 RX input pin.</LI>
	<LI><B>UART2CTS</B> UART2 CTS input pin.</LI>
	<LI><B>UART3RX</B> UART3 RX input pin.</LI>
	<LI><B>UART3CTS</B> UART3 CTS input pin.</LI>
	<LI><B>UART4RX</B> UART4 RX input pin.</LI>
	<LI><B>UART4CTS</B> UART4 CTS input pin.</LI>
	<LI><B>EXT_INT2</B> External Interrupt 2 input pin.</LI>
	<LI><B>EXT_INT3</B> External Interrupt 3 input pin.</LI>
	<LI><B>EXT_INT4</B> External Interrupt 4 input pin.</LI>
	<LI><B>SPICLKIN</B> SPI clock input pin (only in slave mode).</LI>
	<LI><B>SPI_IN</B> SPI data input pin.</LI>
	<LI><B>SPI_SS</B> SPI slave select input pin (only in slave mode).</LI>
	<LI><B>TIM_4_CLK</B> External Timer 4 input pin.</LI>
	<LI><B>UART1TX</B> UART1 TX output pin.</LI>
	<LI><B>UART1RTS</B> UART1 RTS output pin.</LI>
	<LI><B>UART2TX</B> UART2 TX output pin.</LI>
	<LI><B>UART2RTS</B> UART2 RTS output pin.</LI>
	<LI><B>UART3TX</B> UART3 TX output pin.</LI>
	<LI><B>UART3RTS</B> UART3 RTS output pin.</LI>
	<LI><B>UART4TX</B> UART4 TX output pin.</LI>
	<LI><B>UART4RTS</B> UART4 RTS output pin.</LI>
	<LI><B>SPICLKOUT</B> SPI clock output pin (only in master mode).</LI>
	<LI><B>SPI_OUT</B> SPI data output pin.</LI>
	<LI><B>SPI_SS_OUT</B> SPI slave select output pin (only in master mode).</LI>
</UL>
 * \return None
 */ 
void IOInit(int io, int putval) 
{
	io--;
	int addval = 0;
	if (putval < 5)
	{
		switch (putval)
		{
		//	Pin set as OUPUT first disables pull-up and pull-down, then set pin 
		//	as output
		case 0:
			addval = 1 << CNPos[io];
			addval = ~addval;
			*CNPDs[io] = *CNPDs[io] & addval;
			*CNPUs[io] = *CNPUs[io] & addval;
			addval = 0;
			addval = 1 << IOPos[io];
			addval = ~addval;
			*TRISs[io] = *TRISs[io] & addval;
			IOMode[io] = 0;
			break;
			
		//	Pin set as INPUT - first sets the pin as input, then disables pull-up 
		//	and pull-down
		case 1:
			addval = 1 << IOPos[io];
			*TRISs[io] = *TRISs[io] | addval;	
			addval = 0;	
			addval = 1 << CNPos[io];
			addval = ~addval;
			*CNPDs[io] = *CNPDs[io] & addval;
			*CNPUs[io] = *CNPUs[io] & addval;	
			IOMode[io] = 1;
			break;
			
		//	Pin set as INPUT with Pull-up - First disables pull-down, then sets 
		//	pin as input and enables pull-up
		case 2:
			addval = 1 << CNPos[io];
			addval = ~addval;
			*CNPDs[io] = *CNPDs[io] & addval;
			addval = ~addval;
			*CNPUs[io] = *CNPUs[io] | addval;
			addval = 0;
			addval = 1 << IOPos[io];
			*TRISs[io] = *TRISs[io] | addval;
			IOMode[io] = 2;
			break;
			
		//	Pin set as INPUT with Pull-down - First disables pull-up, then sets
		//	pin as input and enables pull-down
		case 3:
			addval = 1 << CNPos[io];
			addval = ~addval;
			*CNPUs[io] = *CNPUs[io] & addval;
			addval = ~addval;
			*CNPDs[io] = *CNPDs[io] | addval;	
			addval = 0;
			addval = 1 << IOPos[io];
			*TRISs[io] = *TRISs[io] | addval;
			IOMode[io] = 3;		
		}
	}
	else if (putval < 31)
	{
		if (RPIORPin[io] != 0)
		{
			if ( (putval < 13) || (putval > 15) )
			{
				addval = 1 << IOPos[io];
				*TRISs[io] = *TRISs[io] | addval;	
				addval = 0;	
				addval = 1 << CNPos[io];
				addval = ~addval;
				*CNPDs[io] = *CNPDs[io] & addval;
				*CNPUs[io] = *CNPUs[io] & addval;	
				IOMode[io] = 1;
			}
			
			int rpdummy;
			if (RPIORPin[io] < 0)
				rpdummy = -RPIORPin[io];
			else 
				rpdummy = RPIORPin[io];
			int reg_val;
			if (RPIRPos[putval-5])
			{
				reg_val = 0x00FF & (*RPIRs[putval-5]);
				reg_val = reg_val | ( rpdummy << 8);
			}
			else
			{
				reg_val = 0xFF00 & (*RPIRs[putval-5]);
				reg_val = reg_val | rpdummy;
			}
			__builtin_write_OSCCONL(OSCCON & 0xBF);						// Unlock registers	
			(*RPIRs[putval-5]) = reg_val;
			__builtin_write_OSCCONL(OSCCON | 0x40);						// Lock register
		}
	}
	else if (putval > 30)
	{
		__builtin_write_OSCCONL(OSCCON & 0xBF);							// Unlock registers
		if (RPIORPin[io]%2==0)
		{					  											// Tests which pin is remapped
			*RPORs[io] =  (*RPORs[io]&0x3F00)|(RPFunc[putval-31]);		// Write on RPOR from 0 to 5th bit
		}
		else
		{
			*RPORs[io] = (*RPORs[io]&0x3F)|(RPFunc[putval-31]<<8);		// Write on RPOR from 8th to 13th bit
		}
		__builtin_write_OSCCONL(OSCCON | 0x40);	
	}

}		


 /**
 * IOGet - Reads the value of the specified pin. 
 * \param io - the pin to read.
 * \return the value read by the function.
 */
int IOGet(int io)
{
	io--;
	int addval = 0;
	if (IOMode[io] == 0)
	{
		int resget;
		resget = Status[io];

		return resget;
	}
	addval = 1 << IOPos[io];
	addval = addval & *PORTs[io];
	if (addval>0)
	{
		Status[io] = 1;
		return 1;		
	}
	else
	{
		Status[io] = 0;
		return 0;
	}

	return -1;
}


 /**
 * IOButtonState - Polls for the state of the button implemented on the specified pin. This command doesn't return the voltage level of the pin,
 * but if the button has been pressed or released. No problem with the "debounce" of the button. It doesn't matter if the button is implemented with a 
 * "low logic" or "high logic". You just have to initialize the pin like "inup" or "indown". 
 * \param io - the pin to read.
 * \return 
  <UL>
	<LI><B>pressed:</B> if the button has been pressed.</LI>
	<LI><B>released:</B> if the button has been released.</LI>
  </UL>
 */
static DWORD tdebounce1 = 0;
int IOButtonState(int io)
{
	DWORD tdebounce2;
	tdebounce2 = TickGetDiv256();
	if ((tdebounce2-tdebounce1)>5)
	{
		io = io - 1;
		int stat;
		stat = Status [io];
		
		if (IOGet(io+1) > stat)
		{
			if (IOMode[io] == 2)	
			{
				tdebounce1 = TickGetDiv256();
				return 1;	
			}
			else		
			{
				tdebounce1 = TickGetDiv256();
				return 2;	
			}
		}

		if (IOGet(io+1) < stat)
		{
			if (IOMode[io] == 2)	
			{
				tdebounce1 = TickGetDiv256();	
				return 2;	
			}
			else
			{
				tdebounce1 = TickGetDiv256();
				return 1;	
			}
		}
		if (stat == Status[io])
			return 0;	
		return -1;
	}
	return 0;	
}	
/*! @} */


/**
\defgroup UART
@{
The UART section provides serial communication. The flyport implements a buffer of 256 characters for the UART, to make serial communicate easier.
*/


 /**
 * UARTInit - Initializes the specified uart port with the specified baud rate.
 * \param port - the UART port to initialize. <B>Note:</B> at the moment the Flyport Framework supports just one UART, but the hardware allows to create up to four UARTs. Others will be added in next release, however is possible to create them with standard PIC commands.
 * \param baud - the desired baudrate.
 * \return None
 */
void UARTInit(int port,long int baud)
{
	port--;
	long int brg , baudcalc , clk , err;
	clk = GetInstructionClock();
	brg = (clk/(baud*16ul))-1;
	baudcalc = (clk/16ul/(brg+1));
	err = (abs(baudcalc-baud)*100ul)/baud;

	if (err<2)
	{
		*UMODEs[port] = 0;
		*UBRGs[port] = brg;
	}
	else
	{
		brg = (clk/(baud*4ul))-1;
		*UMODEs[port] = 0x8;
		*UBRGs[port] = brg;
	}
}


 /**
 * UARTOn - After the initialization, the UART must be turned on with this command.
 * \param port - the UART port to turn on. 
 * \return None
 */
void UARTOn(int port)
{
	port--;
	*UMODEs[port] = *UMODEs[port] | 0x8000;
	*USTAs[port] = *USTAs[port] | 0x400;
	
	*UIFSs[port] = *UIFSs[port] & (~URXIPos[port]);
	*UIFSs[port] = *UIFSs[port] & (~UTXIPos[port]);
	*UIECs[port] = *UIECs[port] | URXIPos[port];
	bufind_w[port] = 0;
	bufind_r[port] = 0;
	last_op[port] = 0;
}


 /**
 * UARTOff - Turns off the specified UART port.
 * \param port - the UART port to turn off. 
 * \return None
 */
void UARTOff(int port)
{
	port--;
	*USTAs[port] = *USTAs[port] & 0xFBFF;
	*UMODEs[port] = *UMODEs[port] & 0x7FFF;
	
	*UIECs[port] = *UIECs[port] & (~URXIPos[port]);
	*UIECs[port] = *UIECs[port] & (~UTXIPos[port]);
}

/// @cond debug
/*---------------------------------------------------------------------------- 
  |	Function: 		UARTRxInt(int port)		 								 |
  | Description: 	Specific funtion to read the UART from inside the ISR.	 |
  |					It fills the static array Buffer[]						 |
  | Returns:		-														 |
  | Parameters:		int port - specifies the port (1 to 4)					 |
  --------------------------------------------------------------------------*/
void UARTRxInt(int port)
{
	port--;
	while ((*USTAs[port] & 1)!=0)
	{
		if (bufind_w[port] == bufind_r[port])
		{
			if (last_op[port] == 1)
			{
				buffover[port] = 1;	
				bufind_w[port] = 0;
				bufind_r[port] = 0;
				last_op[port] = 0;
			}
		}	
		
		Buffer[port][bufind_w[port]] = *URXREGs[port];
		if (bufind_w[port] == UART_BUFFER_SIZE - 1)
		{
			bufind_w[port] = 0;
		}
		else
			bufind_w[port]++;
	}
	last_op[port] = 1;	
	*UIFSs[port] = *UIFSs[port] & (~URXIPos[port]);
}
/// @endcond


 /**
 * UARTFlush - Flushes the buffer of the specified UART port.
 * \param port - the UART port to flush. 
 * \return None
 */
void UARTFlush(int port)
{
	port = port-1;
	bufind_w[port] = 0;
	bufind_r[port] = 0;
	last_op[port] = 0;
}


 /**
 * UARTBufferSize - Returns the RX buffer size of the specified UART port.
 * \param port - the UART port to read. 
 * \return the number of characters that can be read from the specified serial port.
 */
int UARTBufferSize(int port)
{
	port = port-1;
	BYTE loc_last_op = last_op[port];
	int conf_buff;
	int bsize=0;

	conf_buff = bufind_r[port] - bufind_w[port];
	if (conf_buff > 0)
		bsize = UART_BUFFER_SIZE - bufind_r[port] + bufind_w[port];
	else if (conf_buff < 0)

		bsize = bufind_w[port] - bufind_r[port];
	else if (conf_buff == 0)
		if (loc_last_op == 1)
			bsize = UART_BUFFER_SIZE;

	return bsize;
}


 /**
 * UARTRead - Reads characters from the UART RX buffer and put them in the char pointer "towrite" . Also returns the report for the operation.
 * \param port - the UART port to read. 
 * \param towrite - the char pointer to fill with the read characters.
 * \param count - the number of characters to read
 * \return the report for the operation:
  <UL>
	<LI><Bn>0:</B> N characters correctly read.</LI> 
	<LI><B>n<0:</B> N characters read, but buffer overflow detected.</LI> 
 </UL>
 */
int UARTRead (int port , char *towrite , int count)
{
	int rd,limit;
	limit = UARTBufferSize(port);
	if (count > limit)
		count=limit;
	port = port-1;
	int irx = 0;
	rd = 0;
	while (irx < count)
	{


		*(towrite+irx) = Buffer[port][bufind_r[port]];


		if (bufind_r[port] == (UART_BUFFER_SIZE-1))
			bufind_r[port] = 0;
		else
			bufind_r[port]++;		

		irx++;
	}
	
	if ( buffover [port] != 0 )
	{
		rd = -count;
		buffover[port] = 0;
	}
	else
		rd = count;



	last_op[port] = 2;
	return rd;
}



 /**
 * UARTWrite - writes the specified string on the UART port.
 * \param port - the UART port to write to.
 * \param buffer - the string to write (a NULL terminated char array).
 * \return None
 */
void UARTWrite(int port, char *buffer)
{
    
	port--;
	int pdsel;
    // transmits till NUL character is encountered 
	pdsel = (*UMODEs[port] & 6) >>1;
    if (pdsel == 3)        // checks if TX is 8bits or 9bits 
    {
        while(*buffer != '\0') 
        {
            while((*USTAs[port] & 512)>0);	// waits if the buffer is full 
            *UTXREGs[port] = *buffer++;    // sends char to TX reg
        }
    }
    else
    {
        while(*buffer != '\0')
        {
            while((*USTAs[port] & 512)>0);  // sends char to TX reg
            *UTXREGs[port] = *buffer++ & 0xFF;    // sends char to TX reg
        }
    }

}


 /**
 * UARTWriteCh - writes a single character on the UART port.
 * \param port - the UART port to write to.
 * \param char - the char to write.
 * \return None
 */
void UARTWriteCh(int port, char chr)
{
    
	port--;
	int pdsel;
	pdsel = (*UMODEs[port] & 6) >>1;
    if(pdsel == 3)        /* checks if TX is 8bits or 9bits */
    {
		while((*USTAs[port] & 512)>0);	/* waits if the buffer is full */
		*UTXREGs[port] = chr;    		/* transfer data to TX reg */
    }
    else
    {
		while((*USTAs[port] & 512)>0); /* waits if the buffer is full */
		*UTXREGs[port] = chr & 0xFF;   /* transfer data to TX reg */
    }

}

/*! @} */



/**
\defgroup PWM
@{
With the PWM library is possible to easily manage up to nine different PWM (different frequency and duty cycle). To use the PWM on a pin you first have to initialize the PWM, 
then assign it to the desired pin. At runtime it' possible to change the duty cycle of the PWM.
<BR><B>NOTE</B> :it possible to assign a PWM to any pin, also to the input pins.
*/


 /**
 * PWMInit - initializes the specified PWM with the desired frequency and duty cycle .
 * \param freq - frequency in hertz.
 * \param dutyc - ducty cycle for the PWM in percent (0-100).
 * \return None
 */
void PWMInit(BYTE pwm, float freq, float dutyc)
{
	float tpwm;
	float tcy= 0.0000000625;
	tpwm = 1 / freq;
	unsigned int d;
	float dcent = dutyc/100;

	pwm = pwm - 1;

	*OCCON1s[pwm] = 0;								// Reset OC1CON1 Register	
	*OCCON2s[pwm] = 0;								// Reset OC1CON2 Register

	if (freq > 244)
	{
		//	Timer 2 selected: prescaler 0
		period = (unsigned int)(tpwm/(tcy))-1;
		d = (unsigned)(dcent*(period +1));
		*TCONs[1] = *TCONs[1] & 0xFFCF;

		OCTSel[pwm] = 0;
		OCTimer[pwm] = 1;
	}
	else if (freq>30)
	{
		//	Timer 5 selected: prescaler 1
		period = (unsigned int)(tpwm/(tcy*8))-1;
		d = (unsigned)(dcent*(period +1));
		OCTSel[pwm] = 3;
		OCTimer[pwm] = 4;
	}
	else if (freq>3)
	{
		//	Timer 3 selected: prescaler 2
		period = (unsigned int)(tpwm/(tcy*64))-1;
		d = (unsigned)(dcent*(period +1));	
		*TCONs[2] = (*TCONs[2] & 0xFFCF) | 0x20;
		OCTSel[pwm] = 1;
		OCTimer[pwm] = 2;
	}
	else
	{
		//	Timer 1 selected: prescaler 3
		period = (unsigned int)(tpwm/(tcy*256))-1;
		d = (unsigned int)(dcent*(period +1));	
		OCTSel[pwm] = 4;
		OCTimer[pwm] = 0;
	}

	*OCRs[pwm] = d;						// Set duty-cycle
	*OCRSs[pwm] = period;

}


 /**
 * PWMOn - turns on the specified PWM on the specified pin.
 * \param io - pin to assign the PWM.
 * \param pwm - PWM number previously defined in PWMInit.
 * \return None
 */
void PWMOn(BYTE io, BYTE pwm )
{	
	io--;
	pwm = pwm -1;
	if (RPIORPin[io] > 0)
	{
		__builtin_write_OSCCONL(OSCCON & 0xBF);					// Unlock registers
		if (RPIORPin[io]%2==0)
		{					  									// Tests which pin is remapped
			*RPORs[io] =  (*RPORs[io]&0x3F00)|(OCM[pwm]);		// Write on RPOR from 0 to 5th bit
		}
		else
		{
			*RPORs[io] = (*RPORs[io]&0x3F)|(OCM[pwm]<<8);		// Write on RPOR from 8th to 13th bit
		}
		__builtin_write_OSCCONL(OSCCON | 0x40);
		
		*OCCON2s[pwm] = 0x1F;									// SYNCSEL = Ox1F ( Source of Syncronism), OCTRIG=0
		*OCCON1s[pwm] = 0x0006 | (OCTSel[pwm]<<10);				// Start PWM
		if (TimerOn[OCTimer[pwm]] == FALSE)
		{
			*TCONs[OCTimer[pwm]] = *TCONs[OCTimer[pwm]] | 0x8000;
			TimerOn[OCTimer[pwm]] = TRUE;
		}	
	}
}


 /**
 * PWMDuty - changes the duty cycle of the PWM without turning it off. Useful for motors or dimmers.
 * \param duty - new duty cycle desired (0-100).
 * \param pwm - PWM number previously defined in PWMInit.
 * \return None
 */
void PWMDuty(float duty, BYTE pwm)
{
	pwm--;
	unsigned calcpwm, respwm;
	float calcpwm2;
	
	calcpwm = *OCRSs[pwm];
	calcpwm2 = (calcpwm + 1) / 100;
	respwm = calcpwm2 * duty;
	*OCRs[pwm] =(unsigned int) respwm; 
}

 /**
 * PWMOff - turns off the specified PWM.
 * \param pwm - PWM number previously defined in PWMInit.
 * \return None
 */
void PWMOff(BYTE pwm)
{
	pwm--;
	*OCCON1s[pwm] = 0x0000;					// Stop PWM	
}
/*! @} */


/**
\defgroup I2C
@{
The I2C library allows the user to communicate with external devices with I2C bus, like flash memories or sensors. The Flyport is initialized as I2C master.
*/


 /**
 * I2CInit - Initializes the I2C module.
 * \param None
 * \return None
 */
void I2CInit(BYTE I2CSpeed)
{
	TRISGbits.TRISG2 = 1;
	TRISGbits.TRISG3 = 1;

	I2C1TRN = 0x0000;
	I2C1RCV = 0x0000;
		
	I2C1BRG = I2CSpeed;			// Set I2C module at 100 KHz
	I2C1CON = 0x8200;			// Configuration of module
}


 /**
 * I2CStart - Sends a start sequence on the bus.
 * \param None
 * \return None
 */
void I2CStart()
{
	I2C1CONbits.SEN = 1;		 		// Sends a start sequence on I2C bus	
	while(I2C1CONbits.SEN);				// waits the end of start

}


 /**
 * I2CRestart - Sends a repeated start sequence on the bus	.
 * \param None
 * \return None
 */
void I2CRestart()
{
	
	I2C1CONbits.RSEN=1;				// Sends a repeated start sequence
	while(I2C1CONbits.RSEN);		// waits the end of restart

}


 /**
 * I2CStop - Stops the trasmissions on the bus.
 * \param None
 * \return None
 */
void I2CStop()
{
	I2C1CONbits.PEN=1;				// Initiate a Stop condition on the bus
	while(I2C1CONbits.PEN);		// waits the end of stop
}


 /**
 * I2CWrite - writes one byte on the bus.
 * \param None
 * \return None
 */
void I2CWrite(BYTE data)
{
	I2C1TRN = data;					// Sends a byte on the bus

	while(I2C1STATbits.TRSTAT || I2C1STATbits.TBF);		// waits the end of trasmissions
	// Idle	
	while(I2C1CONbits.SEN || I2C1CONbits.PEN || I2C1CONbits.RCEN || I2C1CONbits.RSEN || I2C1CONbits.ACKEN);

} 


 /**
 * I2CRead - Reads one byte from the data bus.
 * \param the value of the Acknowledge to send (0 or 1)
 * \return the value read by the function.
 */
BYTE I2CRead(BYTE ack)
{
	I2C1CONbits.RCEN=1;				// Reads one byte from the bus
	while(I2C1CONbits.RCEN);		// waits the end of the read
	I2C1STATbits.I2COV = 0;			// Resets the overflow flag
	
	I2C1CONbits.ACKDT = ack;		
	I2C1CONbits.ACKEN = 1;			// Initiate an aknowledge sequence
	return I2C1RCV;					// Returns the byte	
}


/*! @} */

#endif
/*! @} */
/*! @} */
