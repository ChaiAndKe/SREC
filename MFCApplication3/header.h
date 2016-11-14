#ifndef HEADER_H
#define HEADER_H

#if 0
#define _TEST
#endif

#ifdef __cplusplus

//�ļ���ȡ������Ϣ
const int FILE_READ_NORMAL = 0;
const int FILE_READ_ERROR = 1;
const int FILE_READ_END = 2;

const int FILE_JUDGE_OUT = 3;//��Χ�⣬�ض�
const int FILE_JUDGE_ALLIN = 4;//ȫ�ڷ�Χ��
const int FILE_JUDGE_LEFTOUT = 5;//��߳�����߽�
const int FILE_JUDGE_RIGHTOUT = 6;//�ұ߳����ұ߽�
const int FILE_JUDGE_BOTHOUT = 7;//���߳�������Ҫ��������̫С...һ���������?

const int FILE_STARTADDR_ERROR = 8;//���õ�������ʼλ�ô���
const int FILE_ADDRESS_ERROR = 9;//����ʼ��ַ����

//ͨѶ�����
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

//��λ������λ����֡�����
const UCHAR ORDER_BOOT = 'B';
const UCHAR ORDER_KEY = 'K';
const UCHAR ORDER_ERASE = 'E';
const UCHAR ORDER_PROGRAM = 'P';
const UCHAR ORDER_GETVERSION = 'G';
const UCHAR ORDER_MAINSTART = 'M';
const UCHAR ORDER_BOOTEND = 'o';

//CAN���Ӳ�������
const int USBCAN_II = 0;
const int USBCAN_2E_U = 1;

const int CHANNEL_0 = 0;
const int CHANNEL_1 = 1;

const int _1000KBPS = 0;
const int _500KBPS = 1;
const int _250KBPS = 2;
const int _125KBPS = 3;
const int _100KBPS = 4;

//CAN������
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


//ͨѶ�����
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

//��λ������λ����֡�����
#define ORDER_BOOT 'B'
#define ORDER_KEY 'K'
#define ORDER_ERASE 'E'
#define ORDER_PROGRAM 'P'
#define ORDER_GETVERSION 'G'
#define ORDER_MAINSTART 'M'
#define ORDER_BOOTEND 'o'

//CAN���Ӳ�������
#define USBCAN_II 0
#define USBCAN_2E_U 1

#define CHANNEL_0 0
#define CHANNEL_1 1

#define _1000KBPS 0
#define _500KBPS 1
#define _250KBPS 2
#define _125KBPS 3
#define _100KBPS 4

//CAN������
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
			return -1;//���ȴ���
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
			return -1;//���ȴ���
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

//S0 ��¼������Ϣ
//S1 ���ݣ���ַ2byte
//S2 ���ݣ���ַ3byte
//S3 ���ݣ���ַ4byte
//S4
//S5
//S6
//S7 ����ʼ��ַ��4byte
//S8 ����ʼ��ַ��3byte
//S9 ����ʼ��ַ��2byte

#endif