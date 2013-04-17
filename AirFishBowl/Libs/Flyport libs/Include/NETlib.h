#include "TCPIP Stack/TCPIP.h"

void NETCustomSave();
void NETCustomDelete();
void NETCustomLoad();
BOOL NETCustomExist();
void NETSetParam(int , char []);

#define MY_IP_ADDR		0
#define PRIMARY_DNS		1
#define SECONDARY_DNS	2
#define MY_GATEWAY		3
#define SUBNET_MASK		4
#define NETBIOS_NAME	5
#define DHCP_ENABLE		6

#define ENABLED		"On"
#define	DISABLED	"Off"


#if defined (FLYPORTETH)
void ETHRestart(int);
int cETHRestart();
#endif
#if defined (FLYPORT)
// TCPIP stack includes
#include "WF_Config.h"

#define SSID_NAME		7
#define NETWORK_TYPE	8

#define INFRASTRUCTURE 	"infra"
#define ADHOC			"adhoc"

#define WFCustomSave 		NETCustomSave
#define WFCustomDelete 		NETCustomDelete
#define WFCustomLoad		NETCustomLoad
#define WFCustomExist		NETCustomExist
#define WFSetParam			NETSetParam


extern int WFNetworkFound;

//	Struct that will contain the characteristic of the scanned networks
typedef struct
{
	BYTE bssid[WF_BSSID_LENGTH];
	char ssid[WF_MAX_SSID_LENGTH+1];
	UINT8 channel;
	UINT8 signal;
	BYTE security;
	BYTE type;
	UINT8 beacon;
	UINT8 preamble;
	
}tWFNetwork;

void WFGeneric(int); 

void WFConnect(int);
int cWFConnect();

void WFScan();
int cWFScan();

void WFDisconnect();
int cWFDisconnect();


void WFSetSecurity(BYTE , char* , BYTE , BYTE);

void WFStopConnecting();
int cWFStopConnecting();

tWFNetwork WFScanList(int);
int cWFScanList();

void WFHibernate();
void WFSleep();
void WFOn();


void WFPsPollEnable(BOOL ps_active);
int cWFPsPollDisable();
int cWFPsPollEnable();

#endif
