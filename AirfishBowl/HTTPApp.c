/* **************************************************************************																					
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
 
 
/****************************************************************************
  SECTION 	Include
****************************************************************************/

#include "TCPIP Stack/TCPIP.h"
#if defined(STACK_USE_HTTP2_SERVER)
#define __HTTPAPP_C

/****************Global Variables*********************/

int myGlobal = 1;
int myGlobal2 = 1;

/*****************Global Variables end****************/

/****************************************************************************
  FUNCTION	HTTP_IO_RESULT HTTPExecuteGet(void)
	
  This function processes every GET request from the pages. In the example, 
  it processes only the leds.cgi function, but you can add code to process 
  other GET requests.
*****************************************************************************/

HTTP_IO_RESULT HTTPExecuteGet(void)
{
	BYTE *ptr;
	BYTE filename[20];
	
	// STEP #1:
	// The function MPFSGetFilename retrieves the name of the requested cgi,
	// in this case "leds.cgi" and puts it inside the filename variable.
	// Make sure BYTE filename[] above is large enough for your longest name
	MPFSGetFilename(curHTTP.file, filename, 20);

	// STEP #2:
	// Handling of the cgi requests, in this case we have only "leds.cgi" but
	// it would be possible to have any other cgi request, depending on the webpage
	
	if(!memcmp(filename, "leds.cgi", 8))		// Is the requested file name "leds.cgi"?
	{
		// STEP #3:
		// The complete request is contained inside the system variable curHTTP.data.
		// Using the function HTTPGetArg is possible to read the arguments
		// of the cgi request from curHTTP.data. In this case we are reading the 
		// argument "led" from the request "leds.cgi?led=x" and we assign it to ptr.
		
		ptr = HTTPGetArg(curHTTP.data, (BYTE *)"led");
		
		// The requested led is toggled
		switch(*ptr)
		{
			case '0':
			//Straight
			    myGlobal = 0;
				PWMOff(1);
				PWMOn(d1out, 1);
				break;
				
			case '1':
			//Stop
				myGlobal = 1;
				PWMOff(1);
				PWMOn(d1out, 1);
				break;
				
			case '2':
				myGlobal = 2;
				//Left
				PWMOff(1);
				PWMOn(d1out, 1);
				break;
				
			case '3':
				myGlobal = 3;
				// Right
				PWMOn(d1out, 1);
				break;
			
			case '4':
				myGlobal2 = 4;
				//Up
				//PWMOff(1);
				//PWMOff(3);
				PWMOn(d4out, 2);
				//PWMOn(d1out, 1);
				break;
				
			case '5':
				myGlobal2 = 5;
				//Down
				//PWMOff(1);
				//PWMOff(3);
				PWMOn(d4out, 2);
				//PWMOn(d1out, 1);
				break;
		}
		
	}
	
	return HTTP_IO_DONE;
}


/****************************************************************************
  Section:
	POST Form Handlers
  ***************************************************************************/
#if defined(HTTP_USE_POST)

/*****************************************************************************
  Function:
	HTTP_IO_RESULT HTTPExecutePost(void)

  	This function processes every GET request from the pages. 
  ***************************************************************************/
HTTP_IO_RESULT HTTPExecutePost(void)
{
	// Resolve which function to use and pass along
	BYTE filename[20];

	// Load the file name
	// Make sure BYTE filename[] above is large enough for your longest name
	MPFSGetFilename(curHTTP.file, filename, sizeof(filename));

	return HTTP_IO_DONE;
}

#endif 



/***************************************************************************
  SECTION	Dynamic Variable Callback Functions
  
  In this section are managed the "dynamic variables" of the webserver.
  Dynamic variables are contained in the status.xml file requested by the 
  webpage. 
  For each dynamic variable a callback function named HTTPPrint_varname 
  must be created.
****************************************************************************/

BYTE HTML_UP_ARROW[] = "up";
BYTE HTML_DOWN_ARROW[] = "dn";

// Callback function for the dynamic variable ~pot(num)~. Anytime the browser
// asks for ~pot(num)~, this function is executed.
void HTTPPrint_pot(WORD num)
{
	BYTE AN0String[8];
	WORD ADval;

	// Analog values reading according to the webpage request.
	switch(num)
	{
		case 0:
			ADval = ADCVal(1);
			uitoa(ADval, (BYTE*)AN0String);
			break;
		case 1:
			ADval = ADCVal(2);
			uitoa(ADval, (BYTE*)AN0String);
			break;	
	}

	// After the analog value is read, it is sent to the webpage
	// using the function TCPPutString. sktHTTP is the standard
	// socket for the HTTP communication.
   	TCPPutString(sktHTTP, AN0String);
}

// Callback function for the dynamic variable ~btn(num)~.
void HTTPPrint_btn(WORD num)
{
	// Determine the button
	switch(num)
	{
		case 0:
			num = IOGet(p5);
			break;
		case 1:
			num = IOGet(p7);
			break;
		case 2:
			num = IOGet(p9);
			break;
		case 3:
			num = IOGet(p11);
			break;
		case 4:
			num = IOGet(p2);
			break;
		default:
			num = 0;
	}

	// Sending back the button status to the webpage.
	TCPPutString(sktHTTP, (num?HTML_UP_ARROW:HTML_DOWN_ARROW));
	return;
}
	
// Callback function for the dynamic variable ~led(num)~.
void HTTPPrint_led(WORD num)
{
	// Determine which LED
	switch(num)
	{
		case 0:
			num = IOGet(p4);
			break;
		case 1:
			num = IOGet(p6);
			break;
		case 2:
			num = IOGet(p17);
			break;
		case 3:
			num = IOGet(p19);
			break;
		case 4:
			num = IOGet(p21);
			break;
		default:
			num = 0;
	}

	// Sending back the led status to the webpage.
	TCPPut(sktHTTP, (num?'1':'0'));
	return;
}


/****************************************************************************
  SECTION 	Authorization Handlers
****************************************************************************/
 

/*****************************************************************************
  FUNCTION	BYTE HTTPNeedsAuth(BYTE* cFile)

  This function is used by the stack to decide if a page is access protected.
  If the function returns 0x00, the page is protected, if returns 0x80, no 
  authentication is required
*****************************************************************************/
#if defined(HTTP_USE_AUTHENTICATION)
BYTE HTTPNeedsAuth(BYTE* cFile)
{
	//	If you want to restrict the access to some page, include it in the folder "protect"
	//	here you can change the folder, or add others
	if(memcmp(cFile, (void*)"protect", 7) == 0)
		return 0x00;		// Authentication will be needed later

	return 0x80;			// No authentication required
}
#endif

/*****************************************************************************
  FUNCTION	BYTE HTTPCheckAuth(BYTE* cUser, BYTE* cPass)
	
  This function checks if username and password inserted are acceptable

  ***************************************************************************/
#if defined(HTTP_USE_AUTHENTICATION)
BYTE HTTPCheckAuth(BYTE* cUser, BYTE* cPass)
{
	if(strcmp((char *)cUser,(char *)"admin") == 0
		&& strcmp((char *)cPass, (char *)"flyport") == 0)
		return 0x80;		// We accept this combination

	return 0x00;			// Provided user/pass is invalid
}
#endif

#endif
