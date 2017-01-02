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

	lineData = new UCHAR[50];//每行数据最多46个
	dataSend = new UCHAR[25];//每行有效数据21个
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


BOOL CAnalysisFile::SetArrange(UINT leftSide, UINT rightSide)//sendAllData为FALSE：只写数据，设置数据范围
{
	//this->sendAllData = sendAllData;

	//if (this->sendAllData == FALSE && leftSide>rightSide)
	if (leftSide>rightSide)
	{
		//左边不能比右边大
		return FALSE;
	}
	dataToSendStartAddr = leftSide;
	dataToSendStopAddr = rightSide;
	IsSetArrange = TRUE;

	curLineNum = 0;
	if(ReadNextLine()==FILE_READ_END)
	{
		throw "设置的地址不在文件范围内，请检查！";
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

//读取下一行数据
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
		//读取错误
		return FILE_READ_ERROR;
		break;
	case FILE_READ_NORMAL:
		//返回正常
		break;
	case FILE_READ_END:
		//文件结尾
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
		//判断数据是否在范围内
		int a = IsDataInside();
		switch(a)
		{
		case FILE_JUDGE_OUT://范围外，重读
			return ReadNextLine();
			break;
		case FILE_JUDGE_ALLIN://全在范围内
			sendLength = dataLength;
			sendStartPosition = 3+addrLength;
			sendStartAddr = lineStartAddr;
			break;
		case FILE_JUDGE_LEFTOUT://左边超出左边界
			sendLength = dataLength-(dataToSendStartAddr-lineStartAddr);
			sendStartPosition = 3+addrLength+(dataToSendStartAddr-lineStartAddr);
			sendStartAddr = dataToSendStartAddr;
			break;
		case FILE_JUDGE_RIGHTOUT://右边超出
			sendLength = dataLength-(lineStopAddr-dataToSendStopAddr);
			sendStartPosition = 3+addrLength;
			sendStartAddr = lineStartAddr;
			break;
		case FILE_JUDGE_BOTHOUT://两边超出，需要的数据量太小...一种理论情况
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

//读取数据并转换字节
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
	//不是要发的数据，则自动过滤了，再读取下一行
	if (lineData[1]=='0' || lineData[1]=='5')
	{
		return ReadAndTurn();
	}
	for(int i=2;i<lineTotalLength;i++)
	{
		lineData[i] = strTobin(lineData[i]);
	}
	//字节合并
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
	case '1'://2个字节的地址
		len = 2;
		break;
	case '2'://3个字节的地址
		len = 3;
		break;
	case '3'://4个字节的地址
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
	case '1'://2个字节的地址
	case '9':
		add = lineData[3]<<8|lineData[4];
		break;

	case '2'://3个字节的地址
	case '8':
		add = lineData[3]<<16|lineData[4]<<8|lineData[5];
		break;

	case '3'://4个字节的地址
	case '7':
		add = lineData[3]<<24|lineData[4]<<16|lineData[5]<<8|lineData[6];
		break;
	}
	return add;
}

//返回值：0,1,2,3,4
int CAnalysisFile::IsDataInside()
{
	if (lineStartAddr>dataToSendStopAddr	|| lineStopAddr<dataToSendStartAddr)
	{
		//不在范围内
		return FILE_JUDGE_OUT;
	}
	if (lineStartAddr>=dataToSendStartAddr	&& lineStopAddr<=dataToSendStopAddr)
	{
		//全部在范围内
		return FILE_JUDGE_ALLIN;
	}
	if (lineStartAddr<dataToSendStartAddr	
		&& lineStopAddr>=dataToSendStartAddr
		&& lineStopAddr<=dataToSendStopAddr)
	{
		//后一部分在
		return FILE_JUDGE_LEFTOUT;
	}
	if (lineStopAddr>dataToSendStopAddr	
		&& lineStartAddr>=dataToSendStartAddr
		&& lineStartAddr<=dataToSendStopAddr)
	{
		//前一部分在
		return FILE_JUDGE_RIGHTOUT;
	}
	if (lineStartAddr<dataToSendStartAddr	
		&& lineStopAddr>dataToSendStopAddr)
	{
		//范围超出了，可能么？不过不能留bug
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
			//读取错误
			return FILE_READ_ERROR;
			break;
		case FILE_READ_NORMAL:
			//返回正常
			break;
		case FILE_READ_END:
			//文件结尾
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

	return -1;//文件结束，正常情况下执行不到这里，因为到结束符后会在上方return;
}
