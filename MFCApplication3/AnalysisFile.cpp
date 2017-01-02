#include "StdAfx.h"
#include "AnalysisFile.h"


BOOL IsSetArrange = FALSE;

CAnalysisFile::CAnalysisFile(LPCTSTR lpszFileName)
	:CStdioFile(lpszFileName, CFile::modeRead)
{
	fileName = lpszFileName;
	strLine = _T("");
	fileLength = GetLength();

	sendAllData = TRUE;
	dataToSendStartAddr = 0;
	dataToSendStopAddr = 0;
	curLineNum = 0;
	dataToSendStartLineNum = 0;
	dataToSendStopLineNum = 0;
	sendLength = 0;
	sendStartPosition = 0;
	sendStartAddr = 0;
	lineType = '0';
	lineLength = 0;
	lineTotalLength = 0;
	lineStartAddr = 0;
	lineStopAddr = 0;
	addrLength = 0;
	dataLength = 0;
	mainStartAddr = 0;
	sendedLength = 0;
	sendedPercent = 0;

	lineData = new UCHAR[50];//ÿ���������46��
	dataSend = new UCHAR[25];//ÿ����Ч����21��
	memset(lineData,0,50*sizeof(UCHAR));
	memset(dataSend,0,25*sizeof(UCHAR));
}


CAnalysisFile::~CAnalysisFile(void)
{
	if (lineData!=NULL)
	{
		delete lineData;
		lineData = NULL;
	}
	if (dataSend!=NULL)
	{
		delete dataSend;
		dataSend = NULL;
	}
}


BOOL CAnalysisFile::SetArrange(UINT leftSide, UINT rightSide)//sendAllDataΪFALSE��ֻд���ݣ��������ݷ�Χ
{
	//this->sendAllData = sendAllData;

	//if (this->sendAllData == FALSE && leftSide>rightSide)
	if (leftSide>rightSide)
	{
		//��߲��ܱ��ұߴ�
		return FALSE;
	}
	dataToSendStartAddr = leftSide;
	dataToSendStopAddr = rightSide;
	IsSetArrange = TRUE;

	curLineNum = 0;
	if(ReadNextLine()==FILE_READ_END)
	{
		throw "���õĵ�ַ�����ļ���Χ�ڣ����飡";
		return FALSE;
	}
	dataToSendStartLineNum = curLineNum;
	while (FILE_READ_NORMAL == ReadNextLine())
		dataToSendStopLineNum = curLineNum;
	if(dataToSendStopLineNum<dataToSendStartLineNum)
		dataToSendStopLineNum=dataToSendStartLineNum;
	SeekToBegin();
	curLineNum = 0;

	return TRUE;
}

//��ȡ��һ������
int CAnalysisFile::ReadNextLine()
{
	if (!IsSetArrange)
	{
		throw "ERROR: Function SetArrage must be called successly before Read";
	}

	if (IsEnd())
	{
		return FILE_READ_END;
	}
	
	int flag = ReadAndTurn();
	switch(flag)
	{
	case FILE_READ_ERROR:
		//��ȡ����
		return FILE_READ_ERROR;
		break;
	case FILE_READ_NORMAL:
		//��������
		break;
	case FILE_READ_END:
		//�ļ���β
		return FILE_READ_END;
		break;
	}

	lineType = GenerateLineType();
	lineLength = GenerateLineLength();
	addrLength = GenerateAddrLength();
	dataLength = lineLength-addrLength-1;
	lineStartAddr = GenerateLineStartAddress();
	lineStopAddr = lineStartAddr+dataLength-1;
	
	if (lineType=='7' || lineType=='8' || lineType=='9')
	{
		mainStartAddr = GenerateLineStartAddress();
		switch(lineData[1])
		{
		case '7':
			mainStartAddr = lineData[3]<<24|lineData[4]<<16|lineData[5]<<8|lineData[6];
			break;
		case '8':
			mainStartAddr = lineData[3]<<16|lineData[4]<<8|lineData[5];
			break;
		case '9':
			mainStartAddr = lineData[3]<<8|lineData[4];
			break;
		}

		return ReadNextLine();
	}

	//if (!sendAllData)
	{
		//�ж������Ƿ��ڷ�Χ��
		int a = IsDataInside();
		switch(a)
		{
		case FILE_JUDGE_OUT://��Χ�⣬�ض�
			return ReadNextLine();
			break;
		case FILE_JUDGE_ALLIN://ȫ�ڷ�Χ��
			sendLength = dataLength;
			sendStartPosition = 3+addrLength;
			sendStartAddr = lineStartAddr;
			break;
		case FILE_JUDGE_LEFTOUT://��߳�����߽�
			sendLength = dataLength-(dataToSendStartAddr-lineStartAddr);
			sendStartPosition = 3+addrLength+(dataToSendStartAddr-lineStartAddr);
			sendStartAddr = dataToSendStartAddr;
			break;
		case FILE_JUDGE_RIGHTOUT://�ұ߳���
			sendLength = dataLength-(lineStopAddr-dataToSendStopAddr);
			sendStartPosition = 3+addrLength;
			sendStartAddr = lineStartAddr;
			break;
		case FILE_JUDGE_BOTHOUT://���߳�������Ҫ��������̫С...һ���������
			sendLength = dataToSendStopAddr-dataToSendStartAddr+1;
			sendStartPosition = 3+addrLength+(dataToSendStartAddr-lineStartAddr);
			sendStartAddr = dataToSendStartAddr;
			break;

		default:
			break;
		}
		sendedLength +=sendLength;
	}
	/*
	else
	{
		sendLength = dataLength;
		sendStartPosition = 3+addrLength;
		sendStartAddr = lineStartAddr;
	}*/

	GenerateSendData();
	return FILE_READ_NORMAL;
}

//��ȡ���ݲ�ת���ֽ�
int CAnalysisFile::ReadAndTurn()
{
	if (IsEnd())
	{
		return FILE_READ_END;
	}
	if(!ReadString(strLine))
	{
		return FILE_READ_ERROR;
	}
	lineTotalLength = strLine.GetLength();
	curLineNum++;

	if (lineTotalLength==0)
	{
		return ReadAndTurn();
	}

	memset(lineData,0,50*sizeof(UCHAR));
	for (int i=0;i<lineTotalLength;i++)
	{
		lineData[i] = (UCHAR)strLine.GetAt(i);
	}
	//����Ҫ�������ݣ����Զ������ˣ��ٶ�ȡ��һ��
	if (lineData[1]=='0' || lineData[1]=='5')
	{
		return ReadAndTurn();
	}
	for(int i=2;i<lineTotalLength;i++)
	{
		lineData[i] = strTobin(lineData[i]);
	}
	//�ֽںϲ�
	for(int i=2;i<2+(lineTotalLength-2)/2;i++)
	{
		lineData[i] = lineData[2*i-2]<<4|lineData[2*i-1];
	}
	
	lineTotalLength = 2+(lineTotalLength-2)/2;
	return FILE_READ_NORMAL;
}
BOOL CAnalysisFile::IsEnd()
{
	if (GetPosition()>=fileLength)
		return TRUE;
	else
		return FALSE;
}

UCHAR CAnalysisFile::GenerateLineType()
{
	return lineData[1];
}

UCHAR CAnalysisFile::GenerateLineLength()
{
	return lineData[2];
}

UCHAR CAnalysisFile::GenerateAddrLength()
{
	UCHAR len = 0;
	switch(lineType)
	{
	case '1'://2���ֽڵĵ�ַ
		len = 2;
		break;
	case '2'://3���ֽڵĵ�ַ
		len = 3;
		break;
	case '3'://4���ֽڵĵ�ַ
		len = 4;
		break;
	}
	return len;
}


UINT CAnalysisFile::GenerateLineStartAddress()
{
	UINT add = 0;
	switch(lineType)
	{
	case '1'://2���ֽڵĵ�ַ
	case '9':
		add = lineData[3]<<8|lineData[4];
		break;

	case '2'://3���ֽڵĵ�ַ
	case '8':
		add = lineData[3]<<16|lineData[4]<<8|lineData[5];
		break;

	case '3'://4���ֽڵĵ�ַ
	case '7':
		add = lineData[3]<<24|lineData[4]<<16|lineData[5]<<8|lineData[6];
		break;
	}
	return add;
}

//����ֵ��0,1,2,3,4
int CAnalysisFile::IsDataInside()
{
	if (lineStartAddr>dataToSendStopAddr	|| lineStopAddr<dataToSendStartAddr)
	{
		//���ڷ�Χ��
		return FILE_JUDGE_OUT;
	}
	if (lineStartAddr>=dataToSendStartAddr	&& lineStopAddr<=dataToSendStopAddr)
	{
		//ȫ���ڷ�Χ��
		return FILE_JUDGE_ALLIN;
	}
	if (lineStartAddr<dataToSendStartAddr	
		&& lineStopAddr>=dataToSendStartAddr
		&& lineStopAddr<=dataToSendStopAddr)
	{
		//��һ������
		return FILE_JUDGE_LEFTOUT;
	}
	if (lineStopAddr>dataToSendStopAddr	
		&& lineStartAddr>=dataToSendStartAddr
		&& lineStartAddr<=dataToSendStopAddr)
	{
		//ǰһ������
		return FILE_JUDGE_RIGHTOUT;
	}
	if (lineStartAddr<dataToSendStartAddr	
		&& lineStopAddr>dataToSendStopAddr)
	{
		//��Χ�����ˣ�����ô������������bug
		return FILE_JUDGE_BOTHOUT;
	}

	return -1;
}

void CAnalysisFile::GenerateSendData()
{
	memset(dataSend,0,25*sizeof(UCHAR));
	for (UCHAR i=0;i<sendLength;i++)
	{
		dataSend[i] = lineData[sendStartPosition+i];
	}
}



const UCHAR *CAnalysisFile::GetDataSend()
{
	return dataSend;
}
UCHAR CAnalysisFile::GetDataSendLength()
{
	return sendLength;
}
UINT CAnalysisFile::GetDataSendAddr()
{
	return sendStartAddr;
}
UINT CAnalysisFile::GetMainStartAddr()
{
	return mainStartAddr;
}
UCHAR CAnalysisFile::GetSendedPercent()
{
	if (curLineNum>dataToSendStopLineNum)
	{
		return 100;
	}
	int l_ = curLineNum-dataToSendStartLineNum+1;
	int m_ = dataToSendStopLineNum-dataToSendStartLineNum+1;
	l_ = l_<0?0:l_;
	m_ = m_<1?1:m_;
	sendedPercent = l_*100/m_;
	return sendedPercent;
}

int CAnalysisFile::CheckSrecFile()
{
	int flag = 0;
	do 
	{
		flag = ReadAndTurn();
		switch(flag)
		{
		case FILE_READ_ERROR:
			//��ȡ����
			return FILE_READ_ERROR;
			break;
		case FILE_READ_NORMAL:
			//��������
			break;
		case FILE_READ_END:
			//�ļ���β
			return FILE_READ_END;
			break;
		}

		lineType = GenerateLineType();
		lineLength = GenerateLineLength();
		addrLength = GenerateAddrLength();
		dataLength = lineLength-addrLength-1;
		lineStartAddr = GenerateLineStartAddress();
		lineStopAddr = lineStartAddr+dataLength-1;

		if (lineStartAddr%8!=0)
		{
			return FILE_ADDRESS_ERROR;
		}

	} while (flag == FILE_READ_NORMAL);

	return -1;//�ļ����������������ִ�в��������Ϊ��������������Ϸ�return;
}
