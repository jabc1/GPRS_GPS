//#include "MainDemo.h"
//#include "MainDemoStrings.h"

//#include "GenericTypeDefs.h"
//#include "Compiler.h"
#include "MainDemo.h"
#include "GOL.h"

#include "Gprs.h"

#include <stdio.h>

#include <string.h>

void  UART3PutChar(uchar Ch);

extern RTCC _time;
extern volatile unsigned char switches;
extern XCHAR 		dateTimeStr[];

SIMREADY simReady = SIM_ADD;
volatile unsigned char simReadyFlag = 0;
volatile CHECK_ERROR checkError = ERROR_E;
volatile unsigned char errorFlag = 0;

#if 1

volatile RECEIVE_BUF Gprs_recBuf;

extern void InitUart3(void);
volatile SEND_BUF Gprs_sendBuf;


unsigned char GPS_Latitude[15];
unsigned char GPS_Longitude[15];

volatile char GprsStatus = CONTROL_PIN_H;

volatile unsigned int Gprs_timeTick = 0;


volatile unsigned int Gprs_timeInterval;
char *pBuffer = &Gprs_sendBuf.Buffer[0];
//static unsigned int curRecordIndex;
unsigned char uartFlag;
volatile unsigned int simTick = 0;
unsigned char singalStrength = 0;
unsigned int heartBeatTick = 0;
volatile SUCC succ = CHAR_ERR;
volatile unsigned char succ_flag;
volatile char Gps_needSend = 0;
unsigned int flashAdd;
//unsigned char tempBuf[75];
volatile XCHAR	tempBuf[RecordSizeMax];
extern unsigned char DeviceID[];
 extern XCHAR Ip[] ;
 extern XCHAR Port[];

//extern SCREEN_STATES  screenState;

extern unsigned int GpsTick;


/*AT命令*/
const char  AT_AT[] = "AT\r\n";
const char ATE000[] = "ATE0\r\n";
const char AT_PIN[] = "AT+CPIN?\r\n";
const char CHAR_WAIT_PIN_READY[] = "+CPIN: READY";
const char AT_GET_COPS[] = "AT+COPS?\r\n";
const char CHAR_WAIT_GET_COPS[] = "MOBILE";
const char AT_SET_COPS[] = "AT+COPS=0\r\n";
const char CHAR_OK[] = "OK";
const char CHAR_ERROR[] = "ERROR";
const char CHAR_DOT[] = ".";

const char AT_CSQ[] = "AT+CSQ\r\n";
const char CHAR_WAIT_CSQ[] = "+CSQ:";
const char AT_GET_GREG[] = "AT+CGREG?\r\n";
const char WAIT_GET_GREG[] = "+CGREG: 0,1";
const char AT_GET_MIPCALL[] = "AT+MIPCALL?\r\n";
const char CHAR_WAIT_GET_MIPCALL[] = "+MIPCALL: 1";
const char AT_SET_MIPCALL[] = "AT+MIPCALL=1,\"CMNET\"\r\n";
const char CHAR_WAIT_SET_MIPCALL[] = "+MIPCALL:";
const char AT_CLOSE_MIPCALL[] = "AT+MIPCALL=0\r\n";
const char AT_GET_SOCKET_STATUS[] = "AT+MIPOPEN?\r\n";
const char CHAR_WAIT_GET_SOCKET_STATUS[] = "+MIPOPEN: 2,3,4";
//#pragma idata AT_BUILD_SOCKET
//char AT_BUILD_SOCKET[] = "AT+MIPOPEN=1,,\"119.145.001.066\",60000,0\r\n";
char AT_BUILD_SOCKET[] = "AT+MIPOPEN=1,,\"202.105.129.162\",60000,0\r\n"; //add by lili 20150906
const char CHAR_WAIT_BUILD_SOCKET[] = "+MIPOPEN: 1,1";
const char AT_CLOSE_SOCKET[] = "AT+MIPCLOSE=1\r\n";
const char CHAR_MORE_THAN[] = ">";
const char CHAR_HEARTBEAT[] = "HEARTBEAT\r\n";
const char AT_RESPONSE[] = "AT+GTSET=\"IPRFMT\",2\r\n";
const char AT_CHECK_CHANNEL[] = "AT+MIPPUSH=1\r\n";
const char CHAR_CHECK_CHANNEL[]= "+MIPPUSH: 1,2";
const char CHAR_CHECK_CHANNEL1[]= "+MIPPUSH: 1,0";

const char CHAR_QUOTA[] = "\"";
const char AT_GETSET[] = "AT+GTSET=\"IPRFMT\",2\r\n";

const char CHAR_SUCC[] = "succ";
const char CHAR_FAIL[] = "fail";

const char AT_HEARTBEAT[] = "AT+MIPSEND=1,9\r\n";
const char CHAR_WAIT_HEARTBEAT[] = "+MIPPUSH: 1,0";

const char AT_SET_UNION_MIPCALL[] = "AT+MIPCALL=1,\"uninet\"\r\n";
const char CHAR_UNION_COPS[] = "UNICOM";

const char CHAR_READY[] = "+SIM";
const char CHAR_CHINA_STR[] = "CHINA";



//const const char const const char 
//const const char const const char 
//const const char const const char 
//const const char const const char 
//const const char const const char 
//const const char const const char 
//const const char const const char 
//const const char const const char 
//const const char const const char 
//const const char const const char 


volatile FAIL_STATUS  failStatus = GPRS_FAIL_ERR;
volatile unsigned  char failFlag = 0;

//校时
volatile TM_STATUS tmStatus = TM_ERROR;
volatile TM_STRUCT tmStruct;    //缓存接受的时间
volatile unsigned char tmFlag = 0;   //接受时间有效的标志
unsigned int tmTick = 0;       //校时节拍
unsigned char tmNeedSend = 1; //需要校时的标志
unsigned char sendTmCounter = 0;
unsigned char simType = 0;




/*AT结束*/

const char   reqHead[]="POST /alky_gps_server/alky/send8 HTTP/1.1\r\n"   //此为要发送的数据包
		
        //"User-Agent: TeamSoft WinInet Component\r\n" 
        //"Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: 58\r\n"
        "Host: ";//219.133.254.027:08081\r\n\r\n" ;
       // "Connection: Keep-Alive\r\n\r\n";
        //"Cache-Control: no-cache\r\n\r\n";
		// "Cookie: ************** \r\n\r\n"
const char	reqHeadRecord[]="POST /alky_gps_server/alky/send8 HTTP/1.1\r\n"	//此为要发送的数据包
			   
			   //"User-Agent: TeamSoft WinInet Component\r\n" 
			   //"Content-Type: application/x-www-form-urlencoded\r\n"
			   "Content-Length: 130\r\n"
			   "Host: ";//219.133.254.027:08081\r\n\r\n" ;
			  // "Connection: Keep-Alive\r\n\r\n";
			   //"Cache-Control: no-cache\r\n\r\n";
			   // "Cookie: ************** \r\n\r\n"
const char reqHeadTm[] = "POST /alky_gps_server/alky/send8 HTTP/1.1\r\n"	
                         "Content-Length: 31\r\n"
                         "Host: ";

        


//GPRS_STATUS preStatus = WAIT_PIN;
static unsigned int Gprs_sendQueueFront, Gprs_sendQueueRear;





void Gprs_Init(void)
{
	static unsigned char i;
	//GPRS_POWER_ON(); //开机
	//memset(&Gprs_sendQueue, 0, sizeof(Gprs_sendQueue));
	//memset(&Gprs_recBuf, 0, sizeof(Gprs_recBuf));
	//memset(&Gprs_sendBuf, 0, sizeof(Gprs_sendBuf));
/*
	Gprs_sendQueue.front = 0;
	Gprs_sendQueue.rear = 0;
    for ( i=0; i<SEND_QUEUE_NUM; i++ )
    {
        Gprs_sendQueue.sendQueue[i] = 0;
	}
*/
	
	Gprs_recBuf.length = 0;
	for ( i=0; i<MAX_RECEIVE_BUFFER; i++ )
	{
        Gprs_recBuf.Buffer[i] = 0;
	}

	Gprs_sendBuf.length = 0;
	for ( i=0; i<MAX_SEND_BUFFER; i++)
	{
        Gprs_sendBuf.Buffer[i] = 0;
	}
	Gprs_sendBuf.status = 0;

	
	
    
  

	
	//Gprs_bRecData = 0;
	Gprs_timeTick = 0;
	simTick = 0;
	singalStrength = 0;
	heartBeatTick = 0;
	//Gprs_needSend = 0;
    //delayms(5000);
	//Gprs_SimpleSendCmd( ATE000);


	

}


void CheckFailString( unsigned char ch )
{
    switch ( failStatus )
	{
	    case GPRS_FAIL_ERR:
			if ( ch == 'f' )
			{
			    failStatus = GPRS_FAIL_F;
			}
			break;
        case GPRS_FAIL_F:
			if ( ch == 'a' )
			{
			    failStatus = GPRS_FAIL_A;
			}
			else if ( ch == 'f' )
			{
			    failStatus = GPRS_FAIL_F;
			}
			else 
			{
			    failStatus = GPRS_FAIL_ERR;
			}
			break;
		case GPRS_FAIL_A:
			if ( ch == 'i' )
			{
				failStatus = GPRS_FAIL_I;
			}
			else if ( ch == 'f' )
			{
				failStatus = GPRS_FAIL_F;
			}
			else 
			{
				failStatus = GPRS_FAIL_ERR;
			}
			break;
		case GPRS_FAIL_I:
			if ( ch == 'l' )
			{
				failStatus = GPRS_FAIL_ERR;
				failFlag = 1;
			}
			else if ( ch == 'f' )
			{
				failStatus = GPRS_FAIL_F;
			}
			else 
			{
				failStatus = GPRS_FAIL_ERR;
			}
			break;
		default:break;

	}
}




void Gprs_SimpleSendCmd(  char* pCmd  )
{
	unsigned char ch;
	/*
	PIE3bits.TX2IE = 0;
	TXSTA2bits.TXEN = 1;

	RCSTA2bits.CREN = 0;
    PIE3bits.RC2IE  = 0;
    */
	ClearReceiveBuf();
    while ( ch = *pCmd++  )
	{
		//pMainDly->SendByte(ch);
		UART3PutChar( ch );
	}
	/*
    RCSTA2bits.CREN = 1;
    PIE3bits.RC2IE  = 1;
    */
	//pMainDly->SendByte( 0x0d );  //换行符
	//DelayMs(500);
    
	
	
}



/* 
功能：清除接收缓存，仅为GPRS模块内部使用
*/
void ClearReceiveBuf()
{
	 unsigned char i;
    // memset(&Gprs_recBuf, 0, sizeof(Gprs_recBuf));
    //SetZero( (char*)&Gprs_recBuf, sizeof(Gprs_recBuf) );
	//Gprs_recBuf.Buffer[0] = 0;
//	Gprs_recBuf.length = 0;

	Gprs_recBuf.length = 0;
	for ( i=0; i<MAX_RECEIVE_BUFFER; i++ )
	{
        Gprs_recBuf.Buffer[i] = 0;
	}
}



/* 
功能：发送命令 
参数：pCmd 发发送的命令
      nextStatus发送完这个命令之后GPRS的状态
	  timeDly 发送pCmd这条命令超时时间

*/
 void Gprs_SendCmd(char  *pCmd, char nextStatus, unsigned int timeDly, unsigned  int timeInterval  )
{
	//unsigned char i;
	unsigned char ch;
    //char *pString = (char*)&Gprs_sendBuf.Buffer[0];
    /*
    PIR3bits.TX2IF  = 0;
	PIE3bits.TX2IE = 0;
	
    RCSTA2bits.CREN = 0;
    PIE3bits.RC2IE  = 0;
    */
	ClearReceiveBuf();
   // Gprs_bRecData = 0;
	Gprs_sendBuf.status = 0;
	Gprs_timeTick = timeDly;
	Gprs_timeInterval = timeInterval;
	GprsStatus = nextStatus;
	//TXSTA2bits.TXEN = 1;

	
	while ( (ch = *pCmd++) != '\0' )
	{
        UART3PutChar( ch );
	}
    
	
    
    
	//RCSTA2bits.CREN = 1;
    //PIE3bits.RC2IE  = 1;


	
}




/* 
功能：接收处理接收到的数据
参数：pCorrectCode 返回结果要包含pCorrectCode这个串才能认为是成功的命令
      sucessStatus 命令成功时GPRS的下一个状态
      failStatus   命令失败时GPRS下一个状态
 */
void Gprs_receiveCmd(char*pCorrectCode, char sucessStatus, char failStatus)
{
	static char *pChar = (char*)&Gprs_recBuf.Buffer[0];
	
	//static unsigned char i;
	pChar = (char*)&Gprs_recBuf.Buffer[0];
    if ( !Gprs_sendBuf.status )
	{
		if ( Gprs_timeTick != 0 )
		{
			Gprs_timeTick--;
			if ( Gprs_timeTick == 0 )
			{
				simTick = 0;
				//RCSTA2bits.CREN = 0;
                //PIE3bits.RC2IE  = 0;
				
				if ( strstr( pChar, (char *)pCorrectCode ) )
				{
					GprsStatus = sucessStatus;
					
				}
				else
				{
					
					GprsStatus = failStatus;
					
				}
			}
		}
	}
}



void ChangeLatORLon( char *pData,  char *pDest )
{
    unsigned char i;
	unsigned char count = 0;
	char *pTempBuf, *pTempDest;
    char ch;
	unsigned long temp;
	unsigned char f = 0;
	unsigned long denominator = 10000;
	double fTemp;
    pTempDest = pDest;
	pTempBuf = pData;

	for ( i=0; i<15; i++ ) pDest[i] = '0';
	while ( (ch = *pTempBuf++) != 0 )
	{
	    if ( (ch == 0x7b) || (ch == 0x27) ) count++;
	}
	if ( count != 2 )
	{
		//for ( i=0; i<15; i++ ) pDest[i] = 0;
        return;
	}
    
	pTempBuf = pData + 1;
   
    while ( (ch = *pTempBuf++) != 0 )
	{
	    
		
        if ( ch == 0x7b ) break;

		*pDest++ = ch;
		*pDest = 0;
	}
    *pDest++ = '.';
	*pDest = 0;
	fTemp = 0;
	count = 0;
	while( (ch = *pTempBuf++) != 0 )
	{
        if ( ch == 0x27 ) break;
        
		if ( ch == 0x2e ) 
		{
			f = 1;
			continue;
		}
        if ( f ) count++;
          
		fTemp = fTemp * 10 + ch - 0x30;

	}
    
	//temp = (unsigned long)(fTemp / 70.0);
	//fTemp = fTemp / 70;

	if ( count < 5 )
	{
         for ( i=0; i<5-count; i++ ) fTemp = fTemp * 10;
	}
	else if ( count > 5 )
	{
         for ( i=0; i<count-5; i++ ) fTemp = fTemp / 10;
	}
	
    temp = (unsigned long)(fTemp / 60);

    
	for( i=0; i<5; i++ )
	{
		*pDest++ = temp / denominator + 0x30;
		*pDest = 0;
		temp = temp % denominator;
		denominator = denominator / 10;
	}

	*pDest++ = ',';
	*pDest++ = pData[0];
	*pDest++ = 0x30;



}




/*
    更新GPS Buf
*/

unsigned int Gprs_UpdateGpsBuf( unsigned char mode )
{
     GPS_RECORD *pRecord;
     unsigned int i;
	 unsigned char ckSum = 0;
	 unsigned int recordLen;
	 unsigned int reqHeadLen;
	 unsigned char temp[15];
	

    ckSum = 0;
	reqHeadLen = sizeof( reqHead ) - 1;
	if ( mode == 1 )
	{
		return (reqHeadLen + sizeof(GPS_RECORD) + 4 + 25);
	}
	
	

    //StrCpy_Rom2Ram(( char*)&Gprs_sendBuf.Buffer[0], (rom far char *)reqHead);
    strcpy( ( char*)&Gprs_sendBuf.Buffer[0], ( char *)reqHead );


	for ( i=0; i<15; i++ )
	{
        Gprs_sendBuf.Buffer[i+reqHeadLen] = Ip[i];
	}
	reqHeadLen += 15;
	Gprs_sendBuf.Buffer[reqHeadLen] = ':';
	reqHeadLen++;
	for ( i=0; i<5; i++ )
	{
        Gprs_sendBuf.Buffer[reqHeadLen+i] = Port[i];
	}
	reqHeadLen += 5;
	Gprs_sendBuf.Buffer[reqHeadLen++] = '\r';
    Gprs_sendBuf.Buffer[reqHeadLen++] = '\n';
	Gprs_sendBuf.Buffer[reqHeadLen++] = '\r';
    Gprs_sendBuf.Buffer[reqHeadLen++] = '\n';
	pRecord = (GPS_RECORD*)&Gprs_sendBuf.Buffer[reqHeadLen];
	
	
	pRecord->startChar1 = 0x68;
	//Device_ID
	pRecord->deviceSerialNum[0] = DeviceID[0];
	pRecord->deviceSerialNum[1] = DeviceID[1];
	pRecord->deviceSerialNum[2] = DeviceID[2];
	pRecord->deviceSerialNum[3] = 0;
	pRecord->deviceSerialNum[4] = LANGUAGE_VERSION;
	pRecord->deviceSerialNum[5] = MACHINE_TYPE;



	pRecord->startChar2 = 0x68;
	pRecord->controlCode = 0x84;
	pRecord->dataLengthMSB = 00;
	pRecord->dataLengthLSB = 45;
	pRecord->diMSB = 0x90;
	pRecord->diLSB = 0x00;


	pRecord->year = _time.yr;
	pRecord->month = _time.mth;
	pRecord->day = _time.day;
	pRecord->hour = _time.hr;
	pRecord->minute = _time.min;

	for ( i=0; i<15; i++ )
	{
        pRecord->latitude[i] = GPS_Latitude[i];
		pRecord->longitude[i] = GPS_Longitude[i];
	}


	pBuffer = ( char*)&Gprs_sendBuf.Buffer[reqHeadLen];

	for ( i=0; i<sizeof(GPS_RECORD)-2; i++ )
	{
        ckSum += pBuffer[i];
	}
	pRecord->controlCode1 = ckSum;
	pRecord->endCode = 0x16;
    recordLen = sizeof( GPS_RECORD );
	Gprs_sendBuf.Buffer[recordLen+reqHeadLen] = '\r';
	Gprs_sendBuf.Buffer[recordLen+reqHeadLen+1] = '\n';
	Gprs_sendBuf.Buffer[recordLen+reqHeadLen+2] = '\r';
	Gprs_sendBuf.Buffer[recordLen+reqHeadLen+3] = '\n';

	//Gprs_sendBuf.Buffer[recordLen+reqHeadLen+4] = 0X55;
	
    //Gprs_sendBuf.Buffer[recordLen+reqHeadLen+5] = 0X66;

	for ( i=4; i<9; i++ )
	{
        Gprs_sendBuf.Buffer[recordLen+reqHeadLen+i] = 0X55 + i;
	}
	

	Gprs_sendBuf.length = recordLen+reqHeadLen + i;

	
	 
    return recordLen+reqHeadLen+i;
#endif

}



unsigned int Gprs_UpdateTmBuf( unsigned char mode )
{
     TM_SEND *pRecord;
     unsigned int i;
	 unsigned char ckSum = 0;
	 unsigned int recordLen;
	 unsigned int reqHeadLen;
	 unsigned char temp[15];
	

    ckSum = 0;
	reqHeadLen = sizeof( reqHeadTm ) - 1;
	if ( mode == 1 )
	{
		return (reqHeadLen + sizeof(TM_SEND) + 4 + 25);
	}
	
	

    //StrCpy_Rom2Ram(( char*)&Gprs_sendBuf.Buffer[0], (rom far char *)reqHead);
    strcpy( ( char*)&Gprs_sendBuf.Buffer[0], ( char *)reqHeadTm );


	for ( i=0; i<15; i++ )
	{
        Gprs_sendBuf.Buffer[i+reqHeadLen] = Ip[i];
	}
	reqHeadLen += 15;
	Gprs_sendBuf.Buffer[reqHeadLen] = ':';
	reqHeadLen++;
	for ( i=0; i<5; i++ )
	{
        Gprs_sendBuf.Buffer[reqHeadLen+i] = Port[i];
	}
	reqHeadLen += 5;
	Gprs_sendBuf.Buffer[reqHeadLen++] = '\r';
    Gprs_sendBuf.Buffer[reqHeadLen++] = '\n';
	Gprs_sendBuf.Buffer[reqHeadLen++] = '\r';
    Gprs_sendBuf.Buffer[reqHeadLen++] = '\n';
	pRecord = (TM_SEND*)&Gprs_sendBuf.Buffer[reqHeadLen];
	
	
	pRecord->startChar1 = 0x68;
	//Device_ID
	pRecord->deviceSerialNum[0] = DeviceID[0];
	pRecord->deviceSerialNum[1] = DeviceID[1];
	pRecord->deviceSerialNum[2] = DeviceID[2];
	pRecord->deviceSerialNum[3] = 0;
	pRecord->deviceSerialNum[4] = LANGUAGE_VERSION;
	pRecord->deviceSerialNum[5] = MACHINE_TYPE;



	pRecord->startChar2 = 0x68;
	pRecord->controlCode = 0x84;
	pRecord->dataLengthMSB = 00;
	pRecord->dataLengthLSB = 18;
	pRecord->diMSB = 0x80;
	pRecord->diLSB = 0x02;


	pRecord->year = _time.yr;
	pRecord->month = _time.mth;
	pRecord->day = _time.day;
	pRecord->hour = _time.hr;
	pRecord->minute = _time.min;

	
	pBuffer = ( char*)&Gprs_sendBuf.Buffer[reqHeadLen];

	for ( i=0; i<sizeof(TM_SEND)-2; i++ )
	{
        ckSum += pBuffer[i];
	}
	pRecord->controlCode1 = ckSum;
	pRecord->endCode = 0x16;
    recordLen = sizeof( TM_SEND );
	Gprs_sendBuf.Buffer[recordLen+reqHeadLen] = '\r';
	Gprs_sendBuf.Buffer[recordLen+reqHeadLen+1] = '\n';
	Gprs_sendBuf.Buffer[recordLen+reqHeadLen+2] = '\r';
	Gprs_sendBuf.Buffer[recordLen+reqHeadLen+3] = '\n';

	//Gprs_sendBuf.Buffer[recordLen+reqHeadLen+4] = 0X55;
	
    //Gprs_sendBuf.Buffer[recordLen+reqHeadLen+5] = 0X66;

	for ( i=4; i<9; i++ )
	{
        Gprs_sendBuf.Buffer[recordLen+reqHeadLen+i] = 0X55 + i;
	}
	

	Gprs_sendBuf.length = recordLen+reqHeadLen + i;

	
	 
    return recordLen+reqHeadLen+i;


}





const char Gprs_BCD_ASC_MAP[] = "0123456789.NSEW";




/*  
mode =1时只是返回长度，数据不拷贝
*/



 unsigned int Gprs_UpdateRecordBuf(unsigned int serialNum, unsigned char mode)
{
	 RECORD *pRecord;
     //unsigned int i;
	 unsigned char ckSum = 0;
     int recordLen;
	 unsigned int reqHeadLen = 0;
     unsigned char cs=0;
	 unsigned int page,addr,i;
     unsigned char tempSerialNum[5], temp;
	 unsigned char f = 0;
	 unsigned char tempBuffer[15];
	
    


    //x = serialNum;
    flashAdd = serialNum;
    reqHeadLen = sizeof( reqHeadRecord ) - 1;
    	
	ckSum = 0;

	if ( mode == 1 )
	{
		return (reqHeadLen + sizeof(RECORD) + 4 + 25);
	}

	
    strcpy( (char*)&Gprs_sendBuf.Buffer[0], (char*)reqHeadRecord );
    


    for ( i=0; i<15; i++ )
	{
        Gprs_sendBuf.Buffer[i+reqHeadLen] = Ip[i];
	}
	reqHeadLen += 15;

	Gprs_sendBuf.Buffer[reqHeadLen] = ':';
	reqHeadLen++;
	for ( i=0; i<5; i++ )
	{
        Gprs_sendBuf.Buffer[reqHeadLen+i] = Port[i];
	}
	reqHeadLen += 5;
	Gprs_sendBuf.Buffer[reqHeadLen++] = '\r';
    Gprs_sendBuf.Buffer[reqHeadLen++] = '\n';
	Gprs_sendBuf.Buffer[reqHeadLen++] = '\r';
    Gprs_sendBuf.Buffer[reqHeadLen++] = '\n';
	
	pRecord = (RECORD*)&Gprs_sendBuf.Buffer[reqHeadLen];
	
	
	pRecord->startChar1 = 0x68;
#if 1

	#if 1
	pRecord->deviceSerialNum[0] = DeviceID[0];
	pRecord->deviceSerialNum[1] = DeviceID[1];
	pRecord->deviceSerialNum[2] = DeviceID[2];
	pRecord->deviceSerialNum[3] = 0;
	pRecord->deviceSerialNum[4] = LANGUAGE_VERSION;
	pRecord->deviceSerialNum[5] = MACHINE_TYPE;

	pRecord->startChar2 = 0x68;
	pRecord->controlCode = 0x84;
//	pRecord->dataLength = 88;
	pRecord->dataLengthMSB = 00;
	pRecord->dataLengthLSB = 117;
	pRecord->diMSB = 0x80;
	pRecord->diLSB = 0x00;

	pRecord->n = 1;


	EEPROMInit();
	page = (flashAdd-1)/2;
	addr = ((flashAdd-1)%2)*256;
	FLASH_RD(page,addr,RecordSizeMax,&tempBuf[0],cs);

    EEPROMInit();
	// 测试模式与单位
	pRecord->unitAndTestMode = 0;	
	//pRecord->unitAndTestMode = 0x80;
	if ( tempBuf[9] != 0 )  pRecord->unitAndTestMode |= 0x80;
	if ( tempBuf[16] & ST_REFUSE) pRecord->unitAndTestMode |= ST_REFUSE;
	else if ( tempBuf[16] & ST_INITIATIVE ) pRecord->unitAndTestMode |= ST_INITIATIVE;
	else pRecord->unitAndTestMode |= 0x0001;


	
	pRecord->reserve1[0] = 0;
    pRecord->reserve1[1] = 0;
	pRecord->reserve1[2] = 0;
	pRecord->reserve1[3] = 0;
	pRecord->reserve1[4] = 0;


	tempSerialNum[0] = (flashAdd /10000) ;// 万位
    tempSerialNum[1] = (flashAdd /1000 %10) ;// 千位
	tempSerialNum[2] = (flashAdd /100 %10) ;// 百位
	tempSerialNum[3] = (flashAdd /10 %100%10) ;// 十位
	tempSerialNum[4] = (flashAdd %10) ;// 个位


	pRecord->recordSerialNum[0] = tempSerialNum[0];
	pRecord->recordSerialNum[1] = (tempSerialNum[1] << 4) | tempSerialNum[2];
	pRecord->recordSerialNum[2] = (tempSerialNum[3] << 4) | tempSerialNum[4];


    pRecord->year = tempBuf[3];
	pRecord->month = tempBuf[4];
	pRecord->day = tempBuf[5];
	pRecord->hour = tempBuf[6];
	pRecord->minute = tempBuf[7];
	pRecord->second = tempBuf[8];

	
    pRecord->reserve2[0] = 0x30;
	pRecord->reserve2[1] = 0x30;
	pRecord->reserve2[2] = 0x30;

	
	pRecord->alcoholMSB = tempBuf[10];
	pRecord->alcoholLSB = tempBuf[11] & 0xF0;
    
    
    
	//pRecord->alcoholMSB = 0x10;
	//pRecord->alcoholLSB = 0x00;
		
    f = 0;
   //车牌号
    for ( i=1; i<18; i++ )
    {   
		temp = tempBuf[49+i];
		if ( (temp < 0x2e) || (temp > 0x80))
		{
             temp = '&';
			 f = 1;
		}
		if ( f ) temp = '&';
        pRecord->carLicenseNum [i] = temp;
	}

	temp = 0xff;
	for ( i=0; i<45; i++ )
	{
        if ( Pro_Char[i][0] == tempBuf[49] ) 
        {
            temp = Pro_Char[i][1]&0xff;
			break;
		}
	}
	pRecord->carLicenseNum[0] = temp;

f = 0;
//警员号
    for ( i=0; i<19; i++ )
    {
		temp = tempBuf[i+87]; 
		if ( (temp < 0x2e) || (temp > 0x80))
		{
             temp = '&';
			 f = 1;
		}
		if ( f ) temp = '&';
        pRecord->policeNum[i] = temp;
	}


f = 0;
//驾驶证号
    for ( i=0; i<19; i++ )
    {
		temp = tempBuf[68+i]; 
		if ( (temp < 0x2e) || (temp > 0x80))
		{
             temp = '&';
			 f = 1;
		}
		if ( f ) temp = '&';
        pRecord->driveNum[i] = temp;
	}

	//for ( i=17; i<=31; i++ ) pRecord->latitude[i-17] = tempBuf[i]; //复制伟度

	//for ( i=32; i<=45; i++ ) pRecord->longitude[i-32] = tempBuf[i];//复制经度

//	ChangeLatORLon( &tempBuf[17], &pRecord->latitude[0] );
//	ChangeLatORLon( &tempBuf[32], &pRecord->longitude[0]);

	for ( i=0; i<15; i++ ) tempBuffer[i] = tempBuf[i+17];
	//tempBuffer[14] = 0;
	ChangeLatORLon((char*)&tempBuffer[0], (char*)&pRecord->latitude[0]);
	for ( i=0;i<14; i++ ) tempBuffer[i] = tempBuf[i+32];
	tempBuffer[14] = 0;
	ChangeLatORLon((char*)&tempBuffer[0], (char*)&pRecord->longitude[0]);

     
#endif  
    pBuffer = (char*)&Gprs_sendBuf.Buffer[reqHeadLen];
	for ( i=0; i<sizeof(RECORD)-2; i++ )
	{
        ckSum += pBuffer[i];
	}
	pRecord->controlCode1 = ckSum;
	pRecord->endCode = 0x16;
    recordLen = sizeof( RECORD );
	Gprs_sendBuf.Buffer[recordLen+reqHeadLen] = '\r';
	Gprs_sendBuf.Buffer[recordLen+reqHeadLen+1] = '\n';
	Gprs_sendBuf.Buffer[recordLen+reqHeadLen+2] = '\r';
	Gprs_sendBuf.Buffer[recordLen+reqHeadLen+3] = '\n';
    //Gprs_sendBuf.Buffer[recordLen+reqHeadLen+4] = '\r';


    for ( i=4; i<8; i++ )
	{
        Gprs_sendBuf.Buffer[recordLen+reqHeadLen+i] = 0X55 + i;
	}
	
	Gprs_sendBuf.length = recordLen+reqHeadLen+i;

	

    return recordLen+reqHeadLen+i;

#endif

}


#if 1

 
char *mystrstr(volatile char *s1, uchar	*s2)
 {
	 unsigned int n;
	 
	 if (*s2)					   //两种情况考虑
	 {
		 while(*s1) 			  
		 {
			 for (n=0;*(s1+n)==*(s2+n);n++)
			 {
				 if (!*(s2+n+1))			//查找的下一个字符是否为''
				 {
					 
					 return (char*)s1;
				 }
			 }
			 s1++;
		 }
		 
		 return NULL;
	 }
	 else
	 {
		 
		 return (char*)s1;
	 }
 }


      


 char cmdBuf[20] = {"AT+MIPSEND=1,"};
  char *pTempBuffer;
  char *pTempBuffer1;
 volatile unsigned int resetTick = 0;
 volatile unsigned int csqTick = 0;
 volatile unsigned char Gprs_powerDownCounter = 0;
 volatile unsigned char sendRecordCounter = 0;
 volatile unsigned char sendGPSCounter = 0;
 volatile unsigned char GPRS_Counter = 0;
 volatile unsigned char noSimCardCounter = 0;

const  char CHAR_GPRS[] = "G\r\n";



 void GprsRun()
{
    //time_t rawtime;
    //char *pChar;
     unsigned int page,addr;
     ///unsigned char charTemp[4];
	 unsigned int tempFlashAdd;
     unsigned  char i;
    
	 unsigned int len;

	unsigned char cs = 0;
	static unsigned char buff[20];
	unsigned char temp[100];
	XCHAR wTemp[100];

    //static const XCHAR char_succ[] = { 's', 'u', 'c', 'c', 0 };
	//static const XCHAR fail[] = { 'f', 'a', 'i', 'l', 0 };

    // 每6.25ms到这里一次
	
	if ( GPRS_Counter++ < 3 ) return;
	GPRS_Counter = 0; 
	//每25ms到这里一次
    //EEPROMInit();
    
    switch( GprsStatus )
	{
		case POWERDOWN:
			
            if ( resetTick == 0)
            {
                resetTick++;
				//TRISFbits.TRISF7 = 0;
				///PORTFbits.RF7 = 1;

				_TRISD10 = 0;
				PORTDbits.RD10 = 1;
			}
			else if ( resetTick > 250 )
			{
				resetTick = 0;
				simTick = 0;
                //TRISFbits.TRISF7 = 0;
				//PORTFbits.RF7 = 0;
				_TRISD10 = 0;
				PORTDbits.RD10 = 0;
				GprsStatus = CONTROL_PIN_H;
				
			}
			else
			{
                 resetTick++;
			}
			break;
		case CONTROL_PIN_H:
			    
                 
				 if ( simTick >= 40 )  // 1s
				 {
                     GprsStatus = CONTROL_PIN_L;
					 simTick = 0;
					 
					 
				 }
				 else
				 {
                     simTick++;
				 }
				 
				 	
			     break;

        case CONTROL_PIN_L:
			     
				 
				 
                 
				 if ( simTick == 0 )
				 {
                     //TRISFbits.TRISF7=0;
					 //PORTFbits.RF7=1;   //拉低
					 _TRISD10 = 0;
				     PORTDbits.RD10 = 1;
					 simTick++;
					 //ClearReceiveBuf();
				 }
				 else if ( simTick > 40 ) //40 * 25 = 1000ms
				 {

                     resetTick++;
					 //TRISFbits.TRISF7=0;
					 //PORTFbits.RF7=0;
					 _TRISD10 = 0;
				     PORTDbits.RD10 = 0;
					 simTick = 0;
					 
			         if ( resetTick > 5 )
			         {    
				 	      resetTick = 0;
					      GprsStatus = POWERDOWN;
                      break;
				     }
					 InitUart3();
                     GprsStatus = GPRSREADY;
					 
					 
				 }
				 else
				 {
                     simTick++;
				 }
			     break;
#define DELAY_TIME 500
		case GPRSREADY:
                
			    if ( simTick < DELAY_TIME )
			    {
                     if ( simReadyFlag )
                     {
                         simTick = DELAY_TIME;
						 
					 }
				}

                if ( simTick == DELAY_TIME ) //320*25 = 8000ms
                {
                     Gprs_SimpleSendCmd( AT_AT);
					 simTick++;
				}
				else if ( simTick >= DELAY_TIME+4 ) // 240ms
				{
				     pTempBuffer = (char*)&Gprs_recBuf.Buffer[0];
					 simTick = 0;
					 Gprs_powerDownCounter++;
						  if ( Gprs_powerDownCounter > 15 )
						  {    
						  	   Gprs_powerDownCounter = 0;
                               resetTick = 0;
					           GprsStatus = POWERDOWN;
						  }

					 
						  
                     if ( strstr(pTempBuffer, (char *)CHAR_OK) )
                     {
					 	  resetTick = 0;
						  GprsStatus = NOSIM;
						  //simTick = 0;
						  //SetColor(0);
				          //SetFont((void*)&FONTDEFAULT);// FONTMEDIUM
					      //MoveTo( 160, 270 );
					 
					      //while (!OutText( (XCHAR*)char_succ ));
					      
						  
					 }
					 else
					 {    
					 	  
					 	  //SetColor(0);
				          //SetFont((void*)&FONTDEFAULT);// FONTMEDIUM
					      //MoveTo( 160, 270 );
					 
					      //while (!OutText( (XCHAR*)fail ));
					      
						  //simTick = 0;
                          GprsStatus = CONTROL_PIN_L;
					 }
				}
				else
				{
                    simTick++;
				}
			    break;
		
	    case NOSIM:
			
			           //UART1PutChar( 'D' );
                       if ( simTick == 0)
                       {
                            Gprs_SimpleSendCmd( (char*)ATE000);
						    simTick++;
				       }
					   else if ( simTick == 5 )   // 125ms
					   {
                             Gprs_SimpleSendCmd((char*)AT_GETSET);
							 simTick++;
					   }
					   else if ( simTick == 10 )//125ms
					   {
					   	    Gprs_SimpleSendCmd( (char*)AT_CLOSE_SOCKET );
                            simTick++;
					   }
					   else if ( simTick == 50 ) //1s
					   {
					   	    Gprs_SimpleSendCmd( (char*)AT_CLOSE_MIPCALL );
                            simTick++;
					   }
				       else if ( simTick > 90 )//1s
				       {
                            simTick = 0;
						    GprsStatus = PIN;
						
				     
				       }
				       else 
				       {
					       simTick++;

				       }
				   
			       

			       break;
	    case PIN:     
						
			
			
			if ( noSimCardCounter++ > 20 )
			{
                  simTick = 0;
				  GprsStatus = GPRSREADY;
				  noSimCardCounter = 0;
			}
			else
			{
				Gprs_SendCmd( (char*)AT_PIN, WAIT_PIN, 50, 2 );//1.2s 48ms
				
			}
			
			break;
		
		case WAIT_PIN: 
			
			Gprs_receiveCmd( (char*)CHAR_WAIT_PIN_READY, COPSSTATUS, GPRSREADY);break;
				
		
		case COPSSTATUS:  

			    
			    
			    noSimCardCounter = 0;
			    Gprs_SendCmd( (char*)AT_GET_COPS, WAIT_COPSSTATUS, 50, 2 );//1.2s 48ms
				break;
		
		case WAIT_COPSSTATUS: 
			 
			
		//Gprs_receiveCmd((char*)CHAR_WAIT_GET_COPS, CSQ, COPS);break;
		
		if ( !Gprs_sendBuf.status )
			{
				if ( Gprs_timeTick != 0 )
				{
					Gprs_timeTick--;
					if ( Gprs_timeTick == 0 )
					{
						simTick = 0;
						//RCSTA2bits.CREN = 0;
						//PIE3bits.RC2IE  = 0;
                       
                        pTempBuffer = (char *)strstr((char*)&Gprs_recBuf.Buffer[0], (char *)CHAR_CHINA_STR );
						if ( pTempBuffer != NULL )
						{
						
							pTempBuffer1 = (char *)strstr((char*)&Gprs_recBuf.Buffer[0], (char *)CHAR_WAIT_GET_COPS );
							if ( pTempBuffer1 != NULL)
							{
								simType = 1;
								GprsStatus = IDLE;
								break;
							}
			
							pTempBuffer1 = (char *)strstr((char*)&Gprs_recBuf.Buffer[0], (char *)CHAR_UNION_COPS );
							if ( pTempBuffer1 != NULL)
							{
								simType = 2;
								GprsStatus = IDLE;
								break;
							}
						}
						
							
					GprsStatus = COPS;
					break;
							
						
					}
				}
			}
		break;

		case COPS: 
			
			Gprs_SendCmd( (char*)AT_SET_COPS, WAIT_COPS, 250, 4 );break; // 6s, 2.4s
		
	    case WAIT_COPS: 
			
			//Gprs_receiveCmd( (char*)CHAR_OK, CSQ, GPRSREADY); simTick = 0;break;
			Gprs_receiveCmd( (char*)CHAR_OK, COPSSTATUS, GPRSREADY); simTick = 0;break;
			
#if 0
        case CSQ:
			
			Gprs_SendCmd( (char*)AT_CSQ, WAIT_CSQ, 50, 2 );break;//1.2s 48ms
		
		case WAIT_CSQ:
			         
			         if ( !Gprs_sendBuf.status )
					 {
						 if ( Gprs_timeTick != 0 )
						 {
							 Gprs_timeTick--;
							 if ( Gprs_timeTick == 0 )
							 {
								 if ( (pTempBuffer = ( char*)strstr((char*)&Gprs_recBuf.Buffer[0], ( char *)CHAR_WAIT_CSQ )) != 0 )
								 {
									  GprsStatus = PPP_DIAL_STATUS;
									//GprsStatus = CSQ;
									 
									 
									 //simTick = 0;
									 if ( pTempBuffer[7] == ',' )
									 {
										 singalStrength = pTempBuffer[6] - 0x30;
									 }
									 else
									 {
										 singalStrength = (pTempBuffer[6] - 0x30) * 10 + pTempBuffer[7] - 0x30;
									 }
									 /*
                                     UART1PutChar( '+' );
									 UART1PutChar( singalStrength);
									 UART1PutChar( '+' );
									 */ 									
								 }
								 else
								 {
									 GprsStatus = GPRSREADY;
									 //GprsStatus = CSQ;
									 simTick = 0;
								 }
							 }
						 }
					 }
			         break;
		#endif
		
		case PPP_DIAL_STATUS:  
			
			Gprs_SendCmd( (char*)AT_GET_MIPCALL, WAIT_PPP_DIAL_STATUS, 50, 4 );break;// 1.2s 240ms

		case WAIT_PPP_DIAL_STATUS: 
			
			Gprs_receiveCmd( (char*)CHAR_WAIT_GET_MIPCALL, SOCKETSTATUS, PPP_DIAL);break;
		

		case PPP_DIAL: 
			
			//Gprs_SendCmd( (char*)AT_SET_MIPCALL, WAIT_PPP_DIAL, 250, 100 );break;//6s,2.4s
			
			if ( simType == 1)
						{
							Gprs_SendCmd( (char*)AT_SET_MIPCALL, WAIT_PPP_DIAL, 250, 100 );
							break;//6s,2.4s
						}
						else if ( simType == 2)
						{
							Gprs_SendCmd( (char*)AT_SET_UNION_MIPCALL, WAIT_PPP_DIAL, 250, 100 );
							break;
						}
						else
						{
							 GprsStatus = COPSSTATUS;
							 break;
						}
						break;

		case WAIT_PPP_DIAL: 
			
			if ( !Gprs_sendBuf.status )
			{
				if ( Gprs_timeTick != 0 )
				{
					Gprs_timeTick--;
					if ( Gprs_timeTick == 0 )
					{


                        pTempBuffer = (char*)strstr((char*)&Gprs_recBuf.Buffer[0], ( char *)CHAR_OK);
						pTempBuffer1 = (char*)strstr((char*)&Gprs_recBuf.Buffer[0], ( char *)CHAR_WAIT_SET_MIPCALL);
						
						//if ( mystrstr((char*)&Gprs_recBuf.Buffer[0], CHAR_OK) && mystrstr((char*)&Gprs_recBuf.Buffer[0], CHAR_WAIT_SET_MIPCALL ) )
						if ( (pTempBuffer != 0) && (pTempBuffer1 != 0 ))
						{
							GprsStatus = SOCKETSTATUS;
							//simTick = 0;
							//TRACE("%s OK\n", "MIPCALL");
						}
						else
						{
							//TRACE("%s fail\n",  "MIPCALL");
							GprsStatus = CONNECT_FAIL;
							//simTick = 0;
						}
					}
					else
					{
                        pTempBuffer = (char*)strstr((char*)&Gprs_recBuf.Buffer[0], ( char *)CHAR_OK);
						pTempBuffer1 = (char*)strstr((char*)&Gprs_recBuf.Buffer[0], ( char *)CHAR_WAIT_SET_MIPCALL);
						
						//if ( mystrstr((char*)&Gprs_recBuf.Buffer[0], CHAR_OK) && mystrstr((char*)&Gprs_recBuf.Buffer[0], CHAR_WAIT_SET_MIPCALL ) )
						if ( (pTempBuffer != 0) && (pTempBuffer1 != 0 ))
						{
							GprsStatus = SOCKETSTATUS;
							break;
						}
						
					}
				}
			}
			               break;
		case CONNECT_FAIL:
			
			Gprs_SendCmd( (char*)AT_CLOSE_MIPCALL, WAIT_CONNECT_FAIL, 25, 4 );break;

		case WAIT_CONNECT_FAIL:
			
			if ( !Gprs_sendBuf.status )
			{
				if ( Gprs_timeTick != 0 )
				{
					Gprs_timeTick--;
					if ( Gprs_timeTick == 0 )
					{
						    
                            GprsStatus = GPRSREADY;
					}
				}
			}
		
			                   break;
		
		case SOCKETSTATUS: 
			
			Gprs_SendCmd( (char*)AT_GET_SOCKET_STATUS, WAIT_SOCKETSTATUS, 50, 4 );break;

		case WAIT_SOCKETSTATUS: 
			
		//Gprs_receiveCmd( (char*)CHAR_WAIT_GET_SOCKET_STATUS, IDLE, BUILD_SOCKET);break;
		Gprs_receiveCmd( (char*)CHAR_WAIT_GET_SOCKET_STATUS, SELECT_DATA, BUILD_SOCKET);break;

		case BUILD_SOCKET: 

			/*
		PIE3bits.TX2IE = 0;
	    TXSTA2bits.TXEN = 1;
    //PIE3bits.TX2IE = 1;
    RCSTA2bits.CREN = 0;
    PIE3bits.RC2IE  = 0;
    */
	ClearReceiveBuf();

	pTempBuffer = &AT_BUILD_SOCKET[15];
	for ( i=0; i<15; i++ )
	{
		
        pTempBuffer[i] = Ip[i];
	}
	pTempBuffer = pTempBuffer + 17;
	for ( i=0; i<5; i++ )
	{
		
        pTempBuffer[i] = Port[i];
	}
    pTempBuffer = &AT_BUILD_SOCKET[0];
	while ( (i = *pTempBuffer++ ) != '\0')
	{
           UART3PutChar( i );
	}
	
	    
//	Gprs_bRecData = 0;
	Gprs_sendBuf.status = 0;
	Gprs_timeTick = 250;
	Gprs_timeInterval = 100;
	GprsStatus = WAIT_BUILD_SOCKET;

	//TXSTA2.TXEN = 1;
	//RCSTA2bits.CREN = 1;
    //PIE3bits.RC2IE  = 1;
	//PIE3bits.TX2IE = 1;
	//TXSTA2bits.TXEN = 1;
	break;

		case WAIT_BUILD_SOCKET: 
			
			if ( !Gprs_sendBuf.status )
			{
				if ( Gprs_timeTick != 0 )
				{
					Gprs_timeTick--;
					if ( Gprs_timeTick == 0 )
					{
						pTempBuffer = (char*)strstr((char*)&Gprs_recBuf.Buffer[0], (char *)CHAR_OK);
						pTempBuffer1 = (char*)strstr((char*)&Gprs_recBuf.Buffer[0], ( char *)CHAR_WAIT_BUILD_SOCKET);
						//if ( mystrstr((char*)&Gprs_recBuf.Buffer[0], CHAR_OK) && mystrstr((char*)&Gprs_recBuf.Buffer[0], CHAR_WAIT_BUILD_SOCKET) )
                        if ( ( pTempBuffer != NULL) && ( pTempBuffer1 != NULL) )
						{
							//GprsStatus = IDLE;
							GprsStatus = SELECT_DATA;
							csqTick = 0;
							simTick = 0;
							//TRACE("%s OK 1\n", "socket");
							
						}
						else
						{
							//TRACE("%s fail 1\n",  "socket");
							GprsStatus = SOCKETFAIL;
							simTick = 0;
						}
					}
					else
					{
                        pTempBuffer = (char*)strstr((char*)&Gprs_recBuf.Buffer[0], (char *)CHAR_OK);
						pTempBuffer1 = (char*)strstr((char*)&Gprs_recBuf.Buffer[0], ( char *)CHAR_WAIT_BUILD_SOCKET);
						//if ( mystrstr((char*)&Gprs_recBuf.Buffer[0], CHAR_OK) && mystrstr((char*)&Gprs_recBuf.Buffer[0], CHAR_WAIT_BUILD_SOCKET) )
                        if ( ( pTempBuffer != NULL) && ( pTempBuffer1 != NULL) )
						{
							//GprsStatus = IDLE;
							GprsStatus = SELECT_DATA;
							csqTick = 0;
							simTick = 0;
							//TRACE("%s OK 1\n", "socket");
							
						}
					}
				}
			} 
			                   break;

		case SOCKETFAIL: 
			
		Gprs_SendCmd( (char*)AT_CLOSE_SOCKET, WAIT_SOCKETFAIL, 250, 10); break;

		case WAIT_SOCKETFAIL:
			
			if ( !Gprs_sendBuf.status )
			{
				if ( Gprs_timeTick != 0 )
				{
					Gprs_timeTick--;
					if ( Gprs_timeTick == 0 )
					{
						//TRACE( "socket fail\n" );
						simTick = 0;
						GprsStatus = PPP_DIAL_STATUS;
					}
				}
			}
			                 break;
		
        

		case SEND_RECORD:
			    

					  len = Gprs_UpdateRecordBuf( 0, 1);
                      cmdBuf[16] =  len % 10 + 0x30;
					  len = len /10;
					  
					  cmdBuf[15] =  len % 10 + 0x30;
					  len = len /10;
					  
					  cmdBuf[14] =  len % 10 + 0x30;
					  len = len /10;
					  
					  cmdBuf[13] =  len % 10+ 0x30;
					  
					  cmdBuf[17] = '\r';
					  cmdBuf[18] = '\n';
					  cmdBuf[19] = '\0';
					  
                      //Gprs_SendCmd((unsigned char*)cmdBuf, WAIT_SEND_GPS, 500, 40); break; //

					  //pTempBuffer= (char*)&Gprs_sendBuf.Buffer[0];
                      //PIR3bits.TX2IF  = 0;
    
					  ClearReceiveBuf();
	
	                  //StrCpy_Ram2Ram( pTempBuffer, (char*)&cmdBuf[0] );
                      //pBuffer = (char*)&Gprs_sendBuf.Buffer[0];
 
	
	                  //Gprs_sendBuf.length = strlen_Ram( (char*)&cmdBuf[0] );

	
    
						//Gprs_bRecData = 0;
						Gprs_sendBuf.status = 0;
						Gprs_timeTick = 100;
						Gprs_timeInterval = 75;
						GprsStatus = WAIT_SEND_RECORD;

						for ( i=0; i<19; i++ )
						{
                             UART3PutChar( cmdBuf[i]);
						}
						
                         //RCSTA2bits.CREN = 1;
                         //PIE3bits.RC2IE  = 1;     
			             break;
		case WAIT_SEND_RECORD: 
			//printString(CHAR_WAIT_SEND_RECORD);
			Gprs_receiveCmd( (char*)CHAR_MORE_THAN, SENDINGRECORD, SOCKETSTATUS);break;

		case SENDINGRECORD: 
			              
		                    //read_ee( Gprs_sendQueueFront, &charTemp[0], 4);
							//tempFlashAdd = *((unsigned long*)&charTemp[0]);
							len = EEPROMReadWord( SENDQUEUEHEAD_ADDRESS );
							delayms( 10 );
							tempFlashAdd = EEPROMReadWord(len);
			                len = Gprs_UpdateRecordBuf( tempFlashAdd , 0);
			                Gprs_sendBuf.length = len;
			                ClearReceiveBuf();
			                pBuffer = (char*)Gprs_sendBuf.Buffer;
			
			                //Gprs_bRecData = 0;
			                Gprs_sendBuf.status = 0;
			                Gprs_timeTick = 2;
			                Gprs_timeInterval = 2;
			                GprsStatus = WAIT_SENDINGRECORD;
							succ_flag = 0;
							failFlag = 0;
							succ = CHAR_ERR;

                            for ( i=0; i<len; i++ )
                            {
                                UART3PutChar(Gprs_sendBuf.Buffer[i]);
							}
							
                            //RCSTA2bits.CREN = 1;
                            //PIE3bits.RC2IE  = 1;
							//sendRecordCounter++;

			               break;


		case WAIT_SENDINGRECORD:
			//printString(CHAR_WAIT_SENDINGRECORD);

			if ( !Gprs_sendBuf.status )
			{
				if ( Gprs_timeTick != 0 )
				{
					Gprs_timeTick--;
					if ( Gprs_timeTick == 0 )
					{
							GprsStatus = RESPONSE;
							
					}
				}
			}
			                    break;

        case RESPONSE: //Gprs_SendCmd((unsigned char*)"AT+GTSET=\"IPRFMT\",2\r", WAIT_RESPONSE, 1000, 1000); break;

					 //Gprs_bRecData = 0;
					 //printString( CHAR_RESPONSE );
					 Gprs_sendBuf.status = 0;
					 Gprs_timeTick = 600;
					 Gprs_timeInterval = 600;
					 GprsStatus = WAIT_RESPONSE;
					 
					 
                     //GPRS_START_SEND; 
			         break;

		case WAIT_RESPONSE:
			//printString(CHAR_WAIT_RESPONSE);
			if ( !Gprs_sendBuf.status )
			{
				if ( Gprs_timeTick != 0 )
				{
					Gprs_timeTick--;

						if ( succ_flag )
						{

							 if ( !(tempBuf[110] & 0x01) )
							 {
							 EEPROMInit();
							 
							 tempBuf[110] = 0xffff;
							 page = (flashAdd-1)/2;	
	                         addr = ((flashAdd-1)%2)*256;//in fact,only write 240bytes 

	                         flash_buffer(0x53,page,cs);
	                         write_bufferstart(0x84,addr,cs); //p1 is flash1,0x84 is buff1,93 is buff1 addr
	                         write_buffer(&tempBuf[0],RecordSizeMax);//ptra is buffer address,33 is write length
	                         write_bufferend(cs);
	                         solid_data(0x83,page,cs);//0 is flash1,0x83 is buff1,yy is page no.
	                         Nop();
	                         Nop();
	                         FLASH_RD(page,addr,RecordSizeMax,&tempBuf[0],cs);
							 //delayms( 20 );
							 EEPROMInit();
							 delayms( 10 );
							 }
							 
                             Gprs_sendQueueFront += 2;
                             
							 if ( Gprs_sendQueueFront > SENDQUEUEEND )
							 {   
							 	 
                                 Gprs_sendQueueFront = SENDQUEUESTART;
							 }
							 delayms( 5 );
							 //更新发送队列
							 EEPROMWriteWord( Gprs_sendQueueFront, SENDQUEUEHEAD_ADDRESS);
							 delayms( 10 );
							 len = EEPROMReadWord( SENDQUEUEHEAD_ADDRESS );
                             //更新记录的发送状态

                    /*
							  sprintf((char*)temp, "%x %x", Gprs_sendQueueFront, len);

					  for ( i=0; i<100;i ++ ) wTemp[i] = temp[i];
                      
                      SetColor(WHITE);
                      Bar( 120, 270 , 230, 300);
                      
					  SetColor(0);
				      SetFont((void*)&FONTDEFAULT);// FONTMEDIUM
					  MoveTo( 120, 270 );
					 
					 while (!OutText( (XCHAR*)wTemp ));
                      
                       */      
							 //Flashbuff

                            //delayms( 100 );
							 
							 succ_flag = 0;
							 failFlag = 0;
							 
							sendRecordCounter = 0;
                            GprsStatus = IDLE; 
							break;
						}

						

						if ( (Gprs_timeTick == 0) || ( failFlag == 1 ) )
						{
                            if ( sendRecordCounter++ > 2 )
							{ 
								 sendRecordCounter = 0; 
                                 Gprs_sendQueueFront += 2;
                                 
                            
							     if ( Gprs_sendQueueFront > SENDQUEUEEND )
							     {   
							 	 
                                     Gprs_sendQueueFront = SENDQUEUESTART;
							     }
							 //更新发送队列
							     EEPROMWriteWord( Gprs_sendQueueFront, SENDQUEUEHEAD_ADDRESS);
							}
                            succ_flag = 0;
							failFlag = 0;
							GprsStatus = IDLE;
							break;
						}
						
					//}
				}
	}
			               break;
		case CHECK_CHANNEL:
			errorFlag = 0;
			Gprs_SendCmd( (char*)AT_CHECK_CHANNEL, WAIT_CHECK_CHANNEL, 100, 10); 
			break;
		case WAIT_CHECK_CHANNEL:
                          //Gprs_receiveCmd( CHAR_CHECK_CHANNEL, SELECT_DATA, IDLE);break;
                          //printString(CHAR_WAIT_CHECK_CHANNEL);
                          if ( !Gprs_sendBuf.status )
						  {
							if ( Gprs_timeTick != 0 )
							{
								Gprs_timeTick--;
								if ( Gprs_timeTick == 0 )
								{
									
									
									simTick = 0;
									//pTempBuffer= (char*)&Gprs_recBuf.Buffer[0];

									pTempBuffer1 = (char*)strstr( (char*)&Gprs_recBuf.Buffer[0], (char *)CHAR_CHECK_CHANNEL );
									if ( pTempBuffer1 != NULL )
									{
										GprsStatus = SELECT_DATA;
										//GprsStatus = IDLE;
									    return;
										
									}

																		
									
									//pTempBuffer1 = (char*)strstr( (char*)&Gprs_recBuf.Buffer[0], (char *)CHAR_ERROR );
									//if ( pTempBuffer1 != NULL )
                                 /*
									if ( errorFlag )
									{
										errorFlag = 0;
										GprsStatus = SOCKETSTATUS;
										
										return;
										
									}
                                     
                                     GprsStatus = IDLE;
                                     */
                                     GprsStatus = SOCKETSTATUS;
								     break;
									
								}
							}
						}
			              break;
		case SELECT_DATA:
			               if (tmNeedSend )
			               {
						   	  GprsStatus = SEND_TM;
                              break;
						   }
			   
                           if ( Gps_needSend == 1 )
                           {
                               GprsStatus = SEND_GPS;
						       break;
					       }
						   
				            
	                       Gprs_sendQueueFront = EEPROMReadWord( SENDQUEUEHEAD_ADDRESS );
                      
					      Gprs_sendQueueRear =  EEPROMReadWord( SENDQUEUEREAR_ADDRESS );
                           if ( Gprs_sendQueueFront != Gprs_sendQueueRear )

                          {
                              GprsStatus = SEND_RECORD;
							  break;
					      }
					      
						   
					      
						   
                           GprsStatus = IDLE;
						   
				      
			              break;
		case SEND_TM:
			          tmFlag = 0;
			          len = Gprs_UpdateTmBuf( 1 );
					  
					  cmdBuf[16] =  len % 10 + 0x30;
					  len = len /10;
					  
					  cmdBuf[15] =  len % 10 + 0x30;
					  len = len /10;
					  
					  cmdBuf[14] =  len % 10 + 0x30;
					  len = len /10;
					  
					  cmdBuf[13] =  len % 10+ 0x30;
					  
					  cmdBuf[17] = '\r';
					  cmdBuf[18] = '\n';
					  cmdBuf[19] = '\0';
					  
      
    
					  ClearReceiveBuf();
	
	                 	
    
						//Gprs_bRecData = 0;
						Gprs_sendBuf.status = 0;
						Gprs_timeTick = 100;
						Gprs_timeInterval = 50;
						GprsStatus = WAIT_SEND_TM;

						for ( i=0; i<19; i++ )
						{
                             UART3PutChar( cmdBuf[i]);

							 
						}

						//RCSTA2bits.CREN = 1;
                        //PIE3bits.RC2IE  = 1;
					
						

			          break;
			break;
		case WAIT_SEND_TM:
			Gprs_receiveCmd( (char*)CHAR_MORE_THAN, SENDINGTM, SOCKETSTATUS);
			break;
		case SENDINGTM:
			len = Gprs_UpdateTmBuf( 0 );
			          Gprs_sendBuf.length = len;
			          ClearReceiveBuf();
			          pBuffer = (char*)&Gprs_sendBuf.Buffer[0];
			
			          //Gprs_bRecData = 0;
			          Gprs_sendBuf.status = 0;
			          Gprs_timeTick = 2;
			          Gprs_timeInterval = 2;
			          GprsStatus = WAIT_SENDINGTM;
					  succ_flag = 0;
					  failFlag = 0;

					  for ( i=0; i<len; i++ )
					  {
                          UART3PutChar( Gprs_sendBuf.Buffer[i] );
					  }
			break;
			
		case WAIT_SENDINGTM:
			if ( !Gprs_sendBuf.status )
			{
				if ( Gprs_timeTick != 0 )
				{
					Gprs_timeTick--;
					if ( Gprs_timeTick == 0 )
					{
						GprsStatus = TM_RESPONSE;
						
					}
				}
			}
			                 
			break;
		case TM_RESPONSE:
			Gprs_sendBuf.status = 0;
			Gprs_timeTick = 600;
			Gprs_timeInterval = 600;
			GprsStatus = WAIT_TM_RESPONSE;
			break;

		case WAIT_TM_RESPONSE:
			if ( !Gprs_sendBuf.status )
			{
				if ( Gprs_timeTick != 0 )
				{
					Gprs_timeTick--;

					if ( succ_flag == 1 )
					{
						 //RCSTA2bits.CREN = 0;
                         //PIE3bits.RC2IE  = 0;
                         GprsStatus = IDLE;
						 //Gps_needSend = 0;
						 
						 succ_flag = 0;
						 failFlag = 0;

						 if ( tmFlag )
						 {
						 	 tmNeedSend = 0;
                             if ( (tmStruct.year> 2020) || ( tmStruct.year <2000) ) break;

							 if ( (tmStruct.month > 12 ) || ( tmStruct.month == 0 ) ) break;
							 
							 if ( ( tmStruct.day >31) || (tmStruct.day== 0) ) break; 

							 if ( tmStruct.hour > 23  ) break;

							 if ( tmStruct.minute >= 60 ) break;

							 if ( tmStruct.second >= 60) break;

							 if ( tmStruct.week > 7  ) break;

							 if ( tmStruct.week == 7 ) tmStruct.week = 0;


							 buff[0] = 0x00;
							 buff[1] = 0x20;
							 buff[2] = 0x00;

							 buff[3] = ( ( (tmStruct.second / 10) & 0x0f )<< 4) | ( (tmStruct.second % 10) & 0x0f );
							 buff[4] = ( ( (tmStruct.minute/ 10) & 0x0f )<< 4)  | ( (tmStruct.minute % 10) & 0x0f );
							 buff[5] = ( ( (tmStruct.hour/ 10) & 0x0f )<< 4)  | ( (tmStruct.hour % 10) & 0x0f );

							 buff[6] = ( ( (tmStruct.day/ 10) & 0x0f )<< 4)  | ( (tmStruct.day % 10) & 0x0f );
							 buff[7] = tmStruct.week;
							 buff[8] = ( ( (tmStruct.month/ 10) & 0x0f )<< 4)  | ( (tmStruct.month % 10) & 0x0f );
							 buff[9] = tmStruct.year % 10;
							 tmStruct.year /= 10;
							 
							 buff[9] |= (tmStruct.year % 10) << 4;

							 for(i = 10; i < 14; i++)
		                    {
			                   buff[i] = 0x80;
		                    }
		                    buff[14] = 0x80;
		                    buff[15] = 0;
		                    buff[16] = 0x00;

							while(1)
		                   {
			                  if(I2cWrite(0xa2, &buff[0], 17) != 0) {error_i2();}
			                  else break;
		                   }
		                   buff[0] = 0;
		                   buff[1] = 0;
		                   buff[2] = 0;

		                  while(1)
		                  {
			                if(I2cWrite(0xa2, &buff[0], 3) != 0) {error_i2();}
			                else break;
		                  }
            
		                  RTCCProcessEvents();	
						  i = 0;	
		while (i < 12) {
			dateTimeStr[i] = _time_str[i];
			dateTimeStr[i+13] = _date_str[i];
			i++;
		}
		dateTimeStr[12] = 0x0000;	// Spring.chen 0x000A; 	// (XCHAR)'\n';
		dateTimeStr[17] = 0x00F0;// 年
		dateTimeStr[20] = 0x0119;// 月
		dateTimeStr[23] = 0x0114;//日?
		dateTimeStr[24] = 0x0000;	// (XCHAR)'\0';
		
		SetScreenMain();
							 
							 
						 }
						 else
						 {
                             if ( sendTmCounter++ > 2 )
						{
                             tmNeedSend = 0;
							 //sendGPSCounter = 0;
							 sendTmCounter = 0;
						}
						//RCSTA2bits.CREN = 0;
                        //PIE3bits.RC2IE  = 0;
						succ_flag = 0;
						failFlag = 0;
						GprsStatus = IDLE;
						break;
						 }
						 break;
					}
					
					
					if ( (Gprs_timeTick == 0) || ( failFlag == 1) )
					{
						
                      		
						//GpsTick = 0;
						if ( sendTmCounter++ > 2 )
						{
                             tmNeedSend = 0;
							 //sendGPSCounter = 0;
							 sendTmCounter = 0;
						}
						//RCSTA2bits.CREN = 0;
                        //PIE3bits.RC2IE  = 0;
						succ_flag = 0;
						failFlag = 0;
						GprsStatus = IDLE;
						break;
					}
				}
			}
			break;
			

		case IDLE:
			     
			      
			      //printString(CHAR_IDLE);
			      csqTick++;
                  Gprs_powerDownCounter = 0;
				  if ( tmTick == 0 ) tmNeedSend = 1;
				   tmTick++;
				   if ( tmTick >= 20000 ) tmTick = 0;
/*
				  if ( csqTick == 100 )
				  {
				      
				      GprsStatus = GETCSQ;
				      return;
				  }
*/

				  //if ( csqTick == 51)
				  if ( csqTick >= 100 )
				  {
				  	  csqTick = 0;
				  	  if ( tmNeedSend )
				  	  {
                          GprsStatus = CHECK_CHANNEL;
						  return;
					  }

					  if ( switches & 0x01 ) 
                      if ( Gps_needSend == 1 )
                      {
                           GprsStatus = CHECK_CHANNEL;
					       return;
					  }

					  if ( switches & 0x02 )

					  {
					  Gprs_sendQueueFront = EEPROMReadWord( SENDQUEUEHEAD_ADDRESS );
                      //delayms( 10 );
					  Gprs_sendQueueRear =  EEPROMReadWord( SENDQUEUEREAR_ADDRESS );
                     /*
					  EEPROMReadArray( SENDQUEUEHEAD_ADDRESS, charTemp, 4);
					  
                      Gprs_sendQueueFront = *( (WORD*)charTemp );
					  Gprs_sendQueueRear  = *( (WORD*)&charTemp[2] );
                      */
                      //if ( screenState == DISPLAY_MAIN )
                      /*
                      sprintf(temp, "%x %x", Gprs_sendQueueFront, Gprs_sendQueueRear);

					  for ( i=0; i<100;i ++ ) wTemp[i] = temp[i];
                      
                      SetColor(WHITE);
                      Bar( 0, 270 , 120, 300);
                      
					  SetColor(0);
				      SetFont((void*)&FONTDEFAULT);// FONTMEDIUM
					  MoveTo( 0, 270 );
					 
					 while (!OutText( (XCHAR*)wTemp ));
                      
					 */
				      if (  Gprs_sendQueueFront != Gprs_sendQueueRear )
				      {
                           GprsStatus = CHECK_CHANNEL;
					       return;
				      }
					  }
				  }
			      
			      
			      
				  
				  
				  if ( csqTick == 51 )
				  {
				      
				      GprsStatus = GETCSQ;
				      return;
				  }
				  
				  
			      break;
		case GETCSQ:
            //printString(CHAR_GETCSQ);
            Gprs_SendCmd( (char*)AT_CSQ, WAIT_GETCSQ, 50, 4 );
			break;

		case WAIT_GETCSQ:
            //printString(CHAR_WAIT_GETCSQ);
            if ( !Gprs_sendBuf.status )
					 {
						 if ( Gprs_timeTick != 0 )
						 {
							 Gprs_timeTick--;
							 if ( Gprs_timeTick == 0 )
							 {
							 	pTempBuffer = (char*)strstr((char*)&Gprs_recBuf.Buffer[0], ( char *)CHAR_WAIT_CSQ );
								 if ( pTempBuffer != 0 )
								 {
									 GprsStatus = IDLE;
									 if ( pTempBuffer[7] == ',' )
									 {
										 singalStrength = pTempBuffer[6] - 0x30;
									 }
									 else
									 {
										 singalStrength = (pTempBuffer[6] - 0x30) * 10 + pTempBuffer[7] - 0x30;
									 }
                                     
									 									
								 }
								 else
								 {
									 GprsStatus = IDLE;
									 simTick = 0;
								 }
							 }
						 }
					 }
			break;

        case SEND_GPS:
	
					  /*
					  RCSTA2bits.CREN = 0;
                      PIE3bits.RC2IE  = 0;
					  PIE3bits.TX2IE = 0;
	                  TXSTA2bits.TXEN = 1;
*/
					  len = Gprs_UpdateGpsBuf( 1 );
					  
					  cmdBuf[16] =  len % 10 + 0x30;
					  len = len /10;
					  
					  cmdBuf[15] =  len % 10 + 0x30;
					  len = len /10;
					  
					  cmdBuf[14] =  len % 10 + 0x30;
					  len = len /10;
					  
					  cmdBuf[13] =  len % 10+ 0x30;
					  
					  cmdBuf[17] = '\r';
					  cmdBuf[18] = '\n';
					  cmdBuf[19] = '\0';
					  
      
    
					  ClearReceiveBuf();
	
	                 	
    
						//Gprs_bRecData = 0;
						Gprs_sendBuf.status = 0;
						Gprs_timeTick = 100;
						Gprs_timeInterval = 50;
						GprsStatus = WAIT_SEND_GPS;

						for ( i=0; i<19; i++ )
						{
                             UART3PutChar( cmdBuf[i]);

							 
						}

						//RCSTA2bits.CREN = 1;
                        //PIE3bits.RC2IE  = 1;
					
						

			          break;
		
		case WAIT_SEND_GPS:

			               Gprs_receiveCmd( (char*)CHAR_MORE_THAN, SENDINGGPS, SOCKETSTATUS);break;
			               break;

		case SENDINGGPS:

			          
/*
                      RCSTA2bits.CREN = 0;
                      PIE3bits.RC2IE  = 0;
					  PIE3bits.TX2IE = 0;
	                  TXSTA2bits.TXEN = 1;
	                  */
			          len = Gprs_UpdateGpsBuf( 0 );
			          Gprs_sendBuf.length = len;
			          ClearReceiveBuf();
			          pBuffer = (char*)&Gprs_sendBuf.Buffer[0];
			
			          //Gprs_bRecData = 0;
			          Gprs_sendBuf.status = 0;
			          Gprs_timeTick = 2;
			          Gprs_timeInterval = 2;
			          GprsStatus = WAIT_SENDINGGPS;
					  succ_flag = 0;
					  failFlag = 0;

					  for ( i=0; i<len; i++ )
					  {
                          UART3PutChar( Gprs_sendBuf.Buffer[i] );
					  }

					  //RCSTA2bits.CREN = 1;
                      //PIE3bits.RC2IE  = 1;

			            break;
		case WAIT_SENDINGGPS:
			
			if ( !Gprs_sendBuf.status )
			{
				if ( Gprs_timeTick != 0 )
				{
					Gprs_timeTick--;
					if ( Gprs_timeTick == 0 )
					{
						GprsStatus = GPS_RESPONSE;
						
					}
				}
			}
			                 break;

		case GPS_RESPONSE:
			
            //Gprs_bRecData = 0;
			Gprs_sendBuf.status = 0;
			Gprs_timeTick = 600;
			Gprs_timeInterval = 600;
			GprsStatus = WAIT_GPS_RESPONSE;
			
			break;

		case WAIT_GPS_RESPONSE:
			
			
			if ( !Gprs_sendBuf.status )
			{
				if ( Gprs_timeTick != 0 )
				{
					Gprs_timeTick--;

					if ( succ_flag == 1 )
					{
						 //RCSTA2bits.CREN = 0;
                         //PIE3bits.RC2IE  = 0;
                         GprsStatus = IDLE;
						 Gps_needSend = 0;
						 succ_flag = 0;
						 failFlag = 0;
						 Gps_needSend = 0;
						 GpsTick = 0;
						 break;
					}
					
					
					if ( (Gprs_timeTick == 0) || ( failFlag == 1) )
					{
						
                      		
						GpsTick = 0;
						if ( sendGPSCounter++ > 2 )
						{
                             Gps_needSend = 0;
							 sendGPSCounter = 0;
						}
						//RCSTA2bits.CREN = 0;
                        //PIE3bits.RC2IE  = 0;
						succ_flag = 0;
						failFlag = 0;
						GprsStatus = IDLE;
						break;
					}
				}
			}
			break;

		
		

		default:GprsStatus = POWERDOWN;
		
		break;
	}

	
		
	

}



void TM_Receive( unsigned char ch )
{
    switch( tmStatus )
	{
	    case TM_ERROR:
			if ( ch == 'C' ) tmStatus = TM_CHAR_C;
			break;

		case TM_CHAR_C:
			if ( ch == 'D' ) tmStatus = TM_CHAR_D;
			else if ( ch == 'C' ) tmStatus = TM_CHAR_C;
			else tmStatus = TM_ERROR;
			break;

		case TM_CHAR_D:
            if ( ch == 'a' ) tmStatus = TM_CHAR_a;
			else if ( ch == 'C' ) tmStatus = TM_CHAR_C;
			else tmStatus = TM_ERROR;
			break;
		
		case TM_CHAR_a:
			if ( ch == 't' ) tmStatus = TM_CHAR_t;
			else if ( ch == 'C' ) tmStatus = TM_CHAR_C;
			else tmStatus = TM_ERROR;
			break;
		
		case TM_CHAR_t:
			if ( ch == 'e' ) tmStatus = TM_CHAR_e;
			else if ( ch == 'C' ) tmStatus = TM_CHAR_C;
			else tmStatus = TM_ERROR;
			break;
		
		case TM_CHAR_e:
			if ( ch == ':' ) tmStatus = TM_CONLON_1;
			else if ( ch == 'C' ) tmStatus = TM_CHAR_C;
			else tmStatus = TM_ERROR;
			break;

		case TM_CONLON_1:
			if ( ch == ' ' ) tmStatus = TM_SPACE_1;
			else if ( ch == 'C' ) tmStatus = TM_CHAR_C;
			else tmStatus = TM_ERROR;
			break;
		case TM_SPACE_1:
			tmStruct.week = ch - '0';
			tmStatus = TM_WEEK;
			break;
		
		case TM_WEEK:
			if ( ch == ',' ) tmStatus = TM_COMMA;
			else if ( ch == 'C' ) tmStatus = TM_CHAR_C;
			else tmStatus = TM_ERROR;
			break;

		case TM_COMMA:
            if ( ch == ' ' ) tmStatus = TM_SPACE_2;
			else if ( ch == 'C' ) tmStatus = TM_CHAR_C;
			else tmStatus = TM_ERROR;
			break;
		
		case TM_SPACE_2:
			tmStruct.day = ( ch - '0' ) * 10;
            tmStatus = TM_DAY_H;
			break;
		case TM_DAY_H:
            tmStruct.day += ch - '0' ;
            tmStatus = TM_DAY_L;
			break;
		case TM_DAY_L:
            if ( ch == ' ' ) tmStatus = TM_SPACE_3;
			else if ( ch == 'C' ) tmStatus = TM_CHAR_C;
			else tmStatus = TM_ERROR;
			break;

		case TM_SPACE_3:
			tmStruct.month = ( ch - '0' ) * 10;
			tmStatus = TM_MONTH_H;
			break;
		case TM_MONTH_H:
			tmStruct.month += ch - '0';
			tmStatus = TM_MONTH_L;
			break;
		case TM_MONTH_L:
			if ( ch == ' ' ) tmStatus = TM_SPACE_4;
			else if ( ch == 'C' ) tmStatus = TM_CHAR_C;
			else tmStatus = TM_ERROR;
			break;
		case TM_SPACE_4:
			tmStruct.year = ( ch - '0' ) * 1000;
			tmStatus = TM_YEAR_1;
			break;
		case TM_YEAR_1:
			tmStruct.year += ( ch - '0' ) * 100;
			tmStatus = TM_YEAR_2;
			break;
		case TM_YEAR_2:
			tmStruct.year += ( ch - '0' ) * 10;
			tmStatus = TM_YEAR_3;
			break;
		case TM_YEAR_3:
			tmStruct.year +=  ch - '0' ;
			tmStatus = TM_YEAR_4;
			break;
		case TM_YEAR_4:
            if ( ch == ' ' ) tmStatus = TM_SPACE_5;
			else if ( ch == 'C' ) tmStatus = TM_CHAR_C;
			else tmStatus = TM_ERROR;
			break;
		case TM_SPACE_5:
            tmStruct.hour = ( ch - '0' ) * 10;
			tmStatus = TM_HOUR_H;
			break;
		case TM_HOUR_H:
			tmStruct.hour += ch - '0';
			tmStatus = TM_HOUR_L;
			break;
		case TM_HOUR_L:
			if ( ch == ':' ) tmStatus = TM_CONLON_2;
			else if ( ch == 'C' ) tmStatus = TM_CHAR_C;
			else tmStatus = TM_ERROR;
			break;
		case TM_CONLON_2:
            tmStruct.minute = ( ch - '0' ) * 10;
			tmStatus = TM_MINUTE_H;
			break;
		case TM_MINUTE_H:
			tmStruct.minute += ch - '0';
			tmStatus = TM_MINUTE_L;
			break;
		case TM_MINUTE_L:
			if ( ch == ':' ) tmStatus = TM_CONLON_3;
			else if ( ch == 'C' ) tmStatus = TM_CHAR_C;
			else tmStatus = TM_ERROR;
			break;
		case TM_CONLON_3:
			tmStruct.second = ( ch - '0' ) * 10;
			tmStatus = TM_SECOND_H;
			break;
		case TM_SECOND_H:
			tmStruct.second += ch - '0';
			tmStatus = TM_SECOND_L;
			break;
		case TM_SECOND_L:
			if ( ch == ' ' ) tmStatus = TM_SPACE_6;
			else if ( ch == 'C' ) tmStatus = TM_CHAR_C;
			else tmStatus = TM_ERROR;
			break;
		case TM_SPACE_6:
			if ( ch == 'G' ) tmStatus = TM_CHAR_G;
			else if ( ch == 'C' ) tmStatus = TM_CHAR_C;
			else tmStatus = TM_ERROR;
			break;
		case TM_CHAR_G:
			if ( ch == 'M' ) tmStatus = TM_CHAR_M;
			else if ( ch == 'C' ) tmStatus = TM_CHAR_C;
			else tmStatus = TM_ERROR;
			break;
		case TM_CHAR_M:
			if ( ch == 'T' ) 
			{
				tmStatus = TM_ERROR;
				tmFlag = 1;
			}
			else if ( ch == 'C' ) tmStatus = TM_CHAR_C;
			else tmStatus = TM_ERROR;
			break;
			break;
		
		default:break;

	}
}


#endif




