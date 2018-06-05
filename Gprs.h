//#if 0
#ifndef __GPRS_H__
#define __GPRS_H__

/*
typedef enum GPRS_STATUS_T
{
	CONTROL_PIN_H=0,
	CONTROL_PIN_L,
	GPRSREADY,
	PIN,
	WAIT_PIN,
	NOSIM,
	COPS,
	WAIT_COPS,
    COPSSTATUS,
	WAIT_COPSSTATUS,
	CSQ,
	WAIT_CSQ,
	CREG,
	WAIT_CREG,
	PPP_DIAL_STATUS,
	WAIT_PPP_DIAL_STATUS,
	BUILD_SOCKET,
	WAIT_BUILD_SOCKET,
	PPP_DIAL,
	WAIT_PPP_DIAL,
	CONNECT_FAIL,
	WAIT_CONNECT_FAIL,
	REPPP_DIAL,
	REWAIT_PPP_DIAL,
	RECONNECT_FAIL,
	REWAIT_CONNECT_FAIL,
	SOCKETSTATUS,
	WAIT_SOCKETSTATUS,
	SEND_RECORD,
	WAIT_SEND_RECORD,
	SOCKETFAIL,
	WAIT_SOCKETFAIL,
	REBUILD_SOCKET,
	WAIT_REBUILD_SOCKET,
	IDLE,
	HEARTBEAT,
	WAIT_HEARTBEAT,
	SEND_HEARTBEAT_CHAR,
	WAIT_SEND_HEARTBEAT_CHAR,
	CLOSE_SOCKET,
	WAIT_CLOSE_SOCKET,
	CLOSE_CONNECT,
	WAIT_CLOSE_CONNECT,
	SENDINGRECORD,
	WAIT_SENDINGRECORD,
	RESPONSE,
	WAIT_RESPONSE,
	SEND_GPS,
	WAIT_SEND_GPS,
	SENDINGGPS,
	WAIT_SENDINGGPS,
	GPS_RESPONSE,
	WAIT_GPS_RESPONSE,
	GETCSQ,
	WAIT_GETCSQ,
	CHECK_CHANNEL,
	WAIT_CHECK_CHANNEL,
	SELECT_DATA,
	POWERDOWN


} GPRS_STATUS;

*/


typedef enum SUCC_T
{

    CHAR_S = 0,
	CHAR_U,
	CHAR_C_1,
	//CHAR_C_2,
	CHAR_ERR,
} SUCC;

#define LANGUAGE_VERSION      1
#define MACHINE_TYPE          8

#define SENDQUEUEHEAD_ADDRESS 0X4000
#define SENDQUEUEREAR_ADDRESS 0X4002

#define IPADDRESS 0x4004//-0x3012
#define PORTADDRESS 0x4013// 0x3017
#define SWITCHADDRESS 0x4018
#define SENDQUEUESTART   0x4200
#define SENDQUEUEEND     0x5200 



#define GPRS_START_SEND uartFlag=1
#define GPRS_STOP_SEND  uartFlag=0
#define GPRS_SEND_STATUS uartFlag

#define SEND_QUEUE_NUM 10
#define MAX_RECEIVE_BUFFER 200
#define MAX_SEND_BUFFER 250
#define NOREC_TIMEOUT_NUM 1000
#define RECEIVE_TIMEOUT_NUM 10

typedef unsigned char uint8;

typedef struct  
{
	volatile char Buffer[MAX_RECEIVE_BUFFER];
	volatile unsigned char length;
} RECEIVE_BUF;

typedef struct  
{
    char Buffer[MAX_SEND_BUFFER];
	unsigned char status;//1表示正在发送，0表示发送完了
	int length;
} SEND_BUF;




typedef enum SIMREADY_T
{
        SIM_ADD,
		SIM_S,
		SIM_I,
		SIM_M,
		SIM_SPACE,
		SIM_R,
		SIM_E,
		SIM_A,
		SIM_D,
		SIM_Y
} SIMREADY ;
void ReadyCheck( unsigned char ch );


typedef struct
{
   uint8 startChar1;
   uint8 deviceSerialNum[6];   
   uint8 startChar2;
   uint8 controlCode;

   uint8 dataLengthMSB;
   uint8 dataLengthLSB;
   uint8 diMSB;
   uint8 diLSB;
   uint8 encryptData[8];
   uint8 n;
   uint8 unitAndTestMode;
   //uint8 blowTime;
   uint8 reserve1[5];

   uint8 recordSerialNum[3];

   uint8 year;
   uint8 month;
   uint8 day;
   uint8 hour;
   uint8 minute;
   uint8 second;


   
   uint8 reserve2[3];
   uint8 alcoholMSB;
   uint8 alcoholLSB;
   
   uint8 carLicenseNum[18];
   uint8 policeNum[19];
   uint8 driveNum[19];

   uint8 latitude[15];
   uint8 longitude[15];

   uint8 controlCode1;
   uint8 endCode;

} RECORD;





typedef struct GPS_RECORD_T
{
	uint8 startChar1;
	uint8 deviceSerialNum[6];   
	uint8 startChar2;
	uint8 controlCode;
	uint8 dataLengthMSB;
	uint8 dataLengthLSB;
	uint8 diMSB;
    uint8 diLSB;
	uint8 encryptData[8];
	uint8 year;
	uint8 month;
	uint8 day;
	uint8 hour;
	uint8 minute;
	uint8 latitude[15];
	uint8 longitude[15];
	
	uint8 controlCode1;
	uint8 endCode;
} GPS_RECORD;

typedef enum FAIL_STATUS_T
{
    GPRS_FAIL_ERR = 0,
    GPRS_FAIL_F,
	GPRS_FAIL_A,
	GPRS_FAIL_I,
	GPRS_FAIL_L
} FAIL_STATUS;



typedef struct TM_STRUCT_T
{
    unsigned char week;
	unsigned char day;
	unsigned char month;
	unsigned int year;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;

} TM_STRUCT;


typedef enum TM_STATUS_T
{
	TM_ERROR,
    TM_CHAR_C,
	TM_CHAR_D,
	TM_CHAR_a,
	TM_CHAR_t,
	TM_CHAR_e,
	TM_CONLON_1,
	TM_SPACE_1,
    TM_WEEK,
	TM_COMMA,
	TM_SPACE_2,
	TM_DAY_H,
	TM_DAY_L,
	TM_SPACE_3,
	TM_MONTH_H,
	TM_MONTH_L,
	TM_SPACE_4,
	TM_YEAR_1,
	TM_YEAR_2,
	TM_YEAR_3,
	TM_YEAR_4,
	TM_SPACE_5,
	TM_HOUR_H,
    TM_HOUR_L,
	TM_CONLON_2,
	TM_MINUTE_H,
	TM_MINUTE_L,
	TM_CONLON_3,
	TM_SECOND_H,
	TM_SECOND_L,
	TM_SPACE_6,
	TM_CHAR_G,
	TM_CHAR_M,
	TM_CHAR_T,
	TM_SPACE_7,
	TM_CKSUM1,
	TM_CKSUM2
} TM_STATUS; 


typedef struct TM_SEND_T
{
	unsigned char startChar1;
	unsigned char deviceSerialNum[6];   
	unsigned char startChar2;
	unsigned char controlCode;
	unsigned char dataLengthMSB;
	unsigned char dataLengthLSB;
	unsigned char diMSB;
    unsigned char diLSB;
	unsigned char encryptData[8];
	unsigned char year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char res[3];
	
	unsigned char controlCode1;
	unsigned char endCode;
} TM_SEND;


typedef enum CHECK_ERROR_T
{
    ERROR_E,
	ERROR_R1,
	ERROR_R2,
	ERROR_O,
	ERROR_R3

} CHECK_ERROR;

extern volatile CHECK_ERROR checkError;
extern volatile unsigned char errorFlag;




#define  CONTROL_PIN_H   0 
#define	CONTROL_PIN_L   1 
#define	GPRSREADY   2 
#define	PIN   3 
#define	WAIT_PIN   4 
#define	NOSIM   5 
#define	COPS   6 
#define	WAIT_COPS   7 
#define  COPSSTATUS   8 
#define	WAIT_COPSSTATUS   9 
#define	CSQ   10 
#define	WAIT_CSQ   11 
#define	CREG   12 
#define	WAIT_CREG   13 
#define	PPP_DIAL_STATUS   14 
#define	WAIT_PPP_DIAL_STATUS   15 
#define	BUILD_SOCKET   16 
#define	WAIT_BUILD_SOCKET   17 
#define	PPP_DIAL   18 
#define	WAIT_PPP_DIAL   19 
#define	CONNECT_FAIL   20 
#define	WAIT_CONNECT_FAIL   21 
#define	REPPP_DIAL   22 
#define	REWAIT_PPP_DIAL   23 
#define	RECONNECT_FAIL   24 
#define	REWAIT_CONNECT_FAIL   25 
#define	SOCKETSTATUS   26 
#define	WAIT_SOCKETSTATUS   27 
#define	SEND_RECORD   28 
#define	WAIT_SEND_RECORD   29 
#define	SOCKETFAIL   30 
#define	WAIT_SOCKETFAIL   31 
#define	REBUILD_SOCKET   32 
#define	WAIT_REBUILD_SOCKET   33 
#define	IDLE   34 
#define	HEARTBEAT   35 
#define	WAIT_HEARTBEAT   36 
#define	SEND_HEARTBEAT_CHAR   37 
#define	WAIT_SEND_HEARTBEAT_CHAR   38 
#define	CLOSE_SOCKET   39 
#define	WAIT_CLOSE_SOCKET   40 
#define	CLOSE_CONNECT   41 
#define	WAIT_CLOSE_CONNECT   42 
#define	SENDINGRECORD   43 
#define	WAIT_SENDINGRECORD   44 
#define	RESPONSE   45 
#define	WAIT_RESPONSE   46 
#define	SEND_GPS   47 
#define	WAIT_SEND_GPS   48 
#define	SENDINGGPS   49 
#define	WAIT_SENDINGGPS   50 
#define	GPS_RESPONSE   51 
#define	WAIT_GPS_RESPONSE   52 
#define	GETCSQ   53 
#define	WAIT_GETCSQ   54 
#define	CHECK_CHANNEL   55 
#define	WAIT_CHECK_CHANNEL   56 
#define	SELECT_DATA   57 
#define	POWERDOWN  58

#define SEND_TM 60
#define WAIT_SEND_TM 61
#define SENDINGTM  62
#define WAIT_SENDINGTM 63
#define TM_RESPONSE 64
#define WAIT_TM_RESPONSE 65



extern SIMREADY simReady;
extern volatile unsigned char simReadyFlag;


extern void SetScreenMain();
extern unsigned char GPS_Latitude[];
extern unsigned char GPS_Longitude[];

extern volatile FAIL_STATUS failStatus;
extern volatile unsigned char failFlag;
extern void CheckFailString( unsigned char ch );
extern void TM_Receive( unsigned char ch );


extern volatile char Gps_needSend;
extern volatile SUCC succ;
//extern SEND_QUEUE Gprs_sendQueue;
extern  volatile RECEIVE_BUF Gprs_recBuf;
extern  volatile SEND_BUF Gprs_sendBuf;
extern volatile unsigned int Gprs_timeInterval;

//extern volatile  GPRS_STATUS GprsStatus;

extern  volatile  char GprsStatus;
extern const char Gprs_BCD_ASC_MAP[];

extern unsigned char Gprs_bRecData;
extern  char *pBuffer;
extern volatile unsigned int Gprs_timeTick;
extern volatile unsigned char succ_flag;
extern  void Gprs_Init(void);
extern  void Gprs_UpdateSendBuf( unsigned int index );

extern  void Gprs_SendCmd(  char  *pCmd, char nextStatus, unsigned int timeDly, unsigned  int timeInterval );
extern void ChangeLatORLon( char *pData, char *pDest );

//extern far rom void Gprs_SendCmd( static const far rom char  *pCmd, GPRS_STATUS nextStatus, unsigned int timeDly, unsigned  int timeInterval );
extern  void GprsRun(void);
//extern far rom void Gprs_receiveCmd(static const far rom char*pCorrectCode, GPRS_STATUS sucessStatus, GPRS_STATUS failStatus);
extern  void Gprs_receiveCmd( char*pCorrectCode, char sucessStatus, char failStatus);


//extern  char *mystrstr(static   char *s1, static rom far char *s2);
extern  void ClearReceiveBuf(void);
//far rom void Gprs_SimpleSendCmd( unsigned char *pCmd  );
extern  void Gprs_SimpleSendCmd(  char* pCmd  );
extern unsigned char uartFlag;

extern unsigned char singalStrength;
extern const char CHAR_DOT[];

extern volatile TM_STATUS tmStatus;
extern volatile TM_STRUCT tmStruct;
extern volatile unsigned char tmFlag;



#endif

