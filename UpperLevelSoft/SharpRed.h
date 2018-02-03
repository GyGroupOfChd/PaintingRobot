#include "./Cncomm/CnComm.h"
#include "ADIO16.h"


#define ONERANG			2.5     //0-20
#define TWORANK         2       //20-30
#define THREERNK		1.55    //30-40
#define FOURRANK		1.25    //40-50
#define FIVERANK		1.1     //50-60
#define SIXRANK			0.9     //60-70
#define SEVENRANK		0.8     //70-80
#define EIGHTRANK		0.75    //80-90
#define NINERANK		0.7     //90-100
#define TENRANK			0.6     //100-110
#define ELEVENRANK		0.55    //110-120
#define TWENTHRANK		0.5     //120-130
#define THIRTEENRANK	0.48    //130-140
#define FOURTEENRANK	0.45    //140-150

#define TWENTY				20
#define THIRTY				30
#define FORTY				40
#define FIFTY				50
#define SIXTY				60
#define SEVENTY				70
#define EIGHTY				80
#define NINETY				90
#define ONEHUNDRED			100
#define ONEHUNDREDANDTEN	110
#define ONEHUNDREDANDTWENTY 120
#define ONEHUNDREDANDTHIRTY 130
#define ONEHUNDREDANDFORTY  140
#define ONEHUNDREDANDFIFTY  150

#define MODULUS 4096          //模数转换等级
#define VOLTAGERANGE  9.83    //电压范围
typedef unsigned __int32 uint32_t;
typedef unsigned __int16 uint16_t; 
typedef unsigned __int8  uint8_t;
typedef uint32_t (*FindDeviceLL)();
typedef	uint32_t (*OpenDeviceLL)(uint32_t  ID);
typedef uint32_t (*m_ReadDeviceHID2BUFLL)( uint32_t ID, uint32_t *ADBuf, uint16_t Len, uint32_t *DAbuf, uint32_t *CNbuf, uint32_t *DIO);
typedef uint32_t (*WriteDeviceHIDEXLL)(uint32_t  ID, uint8_t  CMD, uint8_t  D0, uint8_t  D1, uint8_t  D2, uint8_t  D3);

class SharpRedMeasure
{
public:
	friend DWORD __stdcall GPSImprovement(void* pParam);
	SharpRedMeasure();
	~SharpRedMeasure();
private:
	bool OpenSharpUsb(int portnum, int Baud);
	bool Stop();
	int VoltageTranslateDistance(float Voltage);
	int VoltageRank(float Voltage);
	float ThresholdRank[14];
	int   ThresholdDistance[14];
	void ThresholdRankAndDistanceInit();
	int RankToDistance(int Rank,float Voltage);
public:
	bool OpenDeviceCaiJiKa();
	void GetNumberStream();
	void Init();
	void GetCapyture();
	LARGE_INTEGER m_Freq;
	HANDLE m_SharpPollingThread;
	DWORD m_SharpPollingThreadID;
public:
	CnComm m_SharpRed;              //GPS串口
	BOOL JudgeValue;
	unsigned int CaiJiKa;
public:
	unsigned int ADBuf[1024]; 
	unsigned int DAbuf[4];
	unsigned int CNbuf[16];
	unsigned int DIO[2];
	m_ReadDeviceHID2BUFLL m_m;
	FindDeviceLL m_FindDeviceLL;
	OpenDeviceLL m_OpenDeviceLL;
	WriteDeviceHIDEXLL m_WriteDeviceHID;
	uint32_t m_OpenOrNot; //是否打开设备
	HINSTANCE hDLL;
	int m_SharpDistance;
	int m_SharpDistance1;
	int m_SharpDistance2;
};