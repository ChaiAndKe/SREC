#ifndef HEADER_H
#define HEADER_H

#if 0
#define _TEST
#endif

#ifdef __cplusplus

//文件读取错误信息
const int FILE_READ_NORMAL = 0;
const int FILE_READ_ERROR = 1;
const int FILE_READ_END = 2;

const int FILE_JUDGE_OUT = 3;//范围外，重读
const int FILE_JUDGE_ALLIN = 4;//全在范围内
const int FILE_JUDGE_LEFTOUT = 5;//左边超出左边界
const int FILE_JUDGE_RIGHTOUT = 6;//右边超出右边界
const int FILE_JUDGE_BOTHOUT = 7;//两边超出，需要的数据量太小...一种理论情况?

const int FILE_STARTADDR_ERROR = 8;//设置的数据起始位置错误
const int FILE_ADDRESS_ERROR = 9;//行起始地址错误

//通讯命令定义
const UCHAR PASSWORD_OK = 0X01;
const UCHAR PASSRORD_NOTOK = 0X11;
const UCHAR KEY_OK = 0X02;
const UCHAR KEY_NOTOK = 0X22;
const UCHAR ERASE_OK = 0X03;
const UCHAR ERASE_NOTOK = 0X33;
const UCHAR PROGRAM_OK = 0X04;
const UCHAR PROGRAM_NOTOK = 0X44;
const UCHAR MAINSTART_OK = 0X05;
const UCHAR MAINSTART_NOTOK = 0X55;
const UCHAR DATA_ERR = 0X7F;
const UCHAR GETVERSION_OK = 0X06;
const UCHAR GETVERSION_NOTOK = 0X66;
const UCHAR BOOTEND_OK = 0X07;
const UCHAR BOOTEND_NOTOK = 0X77;

//上位机到下位机的帧命令定义
const UCHAR ORDER_BOOT = 'B';
const UCHAR ORDER_KEY = 'K';
const UCHAR ORDER_ERASE = 'E';
const UCHAR ORDER_PROGRAM = 'P';
const UCHAR ORDER_GETVERSION = 'G';
const UCHAR ORDER_MAINSTART = 'M';
const UCHAR ORDER_BOOTEND = 'o';

//CAN连接参数定义
const int USBCAN_II = 0;
const int USBCAN_2E_U = 1;

const int CHANNEL_0 = 0;
const int CHANNEL_1 = 1;

const int _1000KBPS = 0;
const int _500KBPS = 1;
const int _250KBPS = 2;
const int _125KBPS = 3;
const int _100KBPS = 4;

//CAN错误定义
const int CAN_CONNECT_OK = 0;
const int CAN_CONNECT_NOTOK = 1;
const int CAN_DISCONNECT_OK = 0;
const int CAN_DISCONNECT_NOTOK = 1;

#else

#define FILE_READ_NORMAL 0
#define FILE_READ_ERROR 1
#define FILE_READ_END 2

#define FILE_JUDGE_OUT 0
#define FILE_JUDGE_ALLIN 1
#define FILE_JUDGE_LEFTOUT 2
#define FILE_JUDGE_RIGHTOUT 3
#define FILE_JUDGE_BOTHOUT 4


//通讯命令定义
#define PASSWORD_OK 0X01
#define PASSRORD_NOTOK 0X11
#define KEY_OK 0X02
#define KEY_NOTOK 0X22
#define ERASE_OK 0X03
#define ERASE_NOTOK 0X33
#define PROGRAM_OK 0X04
#define PROGRAM_NOTOK 0X44
#define MAINSTART_OK 0X05
#define MAINSTART_NOTOK 0X55
#define DATA_ERR 0X7F
#define GETVERSION_OK 0X06
#define GETVERSION_NOTOK 0X66
#define BOOTEND_OK 0X07
#define BOOTEND_NOTOK 0X77

//上位机到下位机的帧命令定义
#define ORDER_BOOT 'B'
#define ORDER_KEY 'K'
#define ORDER_ERASE 'E'
#define ORDER_PROGRAM 'P'
#define ORDER_GETVERSION 'G'
#define ORDER_MAINSTART 'M'
#define ORDER_BOOTEND 'o'

//CAN连接参数定义
#define USBCAN_II 0
#define USBCAN_2E_U 1

#define CHANNEL_0 0
#define CHANNEL_1 1

#define _1000KBPS 0
#define _500KBPS 1
#define _250KBPS 2
#define _125KBPS 3
#define _100KBPS 4

//CAN错误定义
#define CAN_CONNECT_OK 0
#define CAN_CONNECT_NOTOK 1
#define CAN_DISCONNECT_OK 0
#define CAN_DISCONNECT_NOTOK 1



#endif

class BaseType{
public:
	UCHAR startSign;
	UCHAR command;
	UCHAR returnValue;
	UCHAR dataLength;
	UCHAR totalLength;
	UCHAR m_check;
	UCHAR* allData;

private:
	BaseType();

public:
	BaseType(UCHAR totalLength){
		startSign = '$';command = 0;returnValue = 0;m_check = 0;dataLength = 0;

		this->totalLength = totalLength;
		allData = new UCHAR[totalLength];
		memset(allData,0x00,totalLength*sizeof(UCHAR));
	}
public:
	UCHAR GetCheck()
	{
		UCHAR l_check = 0;
		for (UCHAR i=1;i<totalLength-1;i++)
		{
			l_check += allData[i];
		}
		return l_check;
	}
	int SetAllData(const char *d,int len)
	{
		if (totalLength!=len)
		{
			return -1;
		}
		for(int i=0;i<totalLength;i++)
		{
			allData[i]=d[i];
		}
		if (d[0]==0xA5)
		{
			returnValue = d[1];
			dataLength = d[2];
			m_check=d[7];
		}
	}

public:
	int SetData(UCHAR command,UCHAR length,const UINT data)
	{
		if (totalLength!=8)
		{
			return -1;//长度错误
		}

		this->command = command;
		this->dataLength = length;

		memset(allData,0,totalLength*sizeof(UCHAR));

		allData[0] = '$';
		allData[1] = command;
		allData[2] = length;
		allData[3] = data>>24;
		allData[4] = data>>16;
		allData[5] = data>>8;
		allData[6] = data;

		allData[7] = GetCheck();
		
		return TRUE;
	}

	int SetData(UCHAR command,UCHAR length,const UINT addr,const UCHAR *data)
	{
		if (totalLength!=24)
		{
			return -1;//长度错误
		}

		this->command = command;
		this->dataLength = length;

		memset(allData,0,totalLength*sizeof(UCHAR));

		allData[0] = '$';
		allData[1] = command;
		allData[2] = length;
		allData[3] = addr>>24;
		allData[4] = addr>>16;
		allData[5] = addr>>8;
		allData[6] = addr;

		for (UCHAR i=0;i<length;i++)
		{
			allData[7+i] = data[i];
		}
		allData[totalLength-1] = GetCheck();
		return TRUE;
	}
};

//S0 记录描述信息
//S1 数据，地址2byte
//S2 数据，地址3byte
//S3 数据，地址4byte
//S4
//S5
//S6
//S7 程序开始地址，4byte
//S8 程序开始地址，3byte
//S9 程序开始地址，2byte

#endif