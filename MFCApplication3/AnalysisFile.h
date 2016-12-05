/************************************************************************/
/*
1.提供的烧写文件应保证地址是 8 字节对齐即末位是 0 或者 8，上位机在点烧写读取文件
时，先全文件检查一遍，若出现不符合要求的则给出提示（“文件不符合要求或文件损
坏” ），不继续后续的烧写流程；
2.有效数据的长度 Length 应为 4 的倍数，不足 4 的倍数时按照 4 的最小倍数填 0xff 补齐；
3.帧格式 2 的长度固定为 24 字节，有效数据长度不足 16 字节时，数据帧区域填 0xff 补齐
为 16 字节
4.Data域中有效数据长度，Length = Data length，其值只可为4的倍数， 即4/8/12/16.
5.写入下位机的地址，四个字节。其值必须8字节对齐，即16进制表示时 末位只可为0或8
（比如0x01000010,0x01000018）。
6.地址按照小端模式，数据区按照srec的顺序来,比如0x11223344，发送的时候地址区第一个字节填44，第二个填33
/************************************************************************/
#ifndef _ANALYSISFILE_H
#define _ANALYSISFILE_H

#include "afx.h"
#include "header.h"
class CAnalysisFile : public CStdioFile
{
public:
	CAnalysisFile(LPCTSTR lpszFileName);
	virtual ~CAnalysisFile(void);

private:
	/*
	要发送的数据的起始位置和结束位置，
	在读取一行的时候都判断sendAllData=TRUE?
	TRUE表明发送全部数据
	如果是假，则判断数据是否在范围内
	*/
	BOOL sendAllData;//全局有效，是否发送全部数据
	UINT dataToSendStartAddr;//全局，写数据的起始地址
	UINT dataToSendStopAddr;//全局，写数据的结束地址

	int dataToSendStartLineNum;//全局，写数据的起始行
	int dataToSendStopLineNum;//全局，写数据的结束行
	int curLineNum;//当前行号

	UCHAR sendLength;//经过判断以后当前行应当发送的数据长度（在范围内）
	UCHAR sendStartPosition;//经过判断以后当前行应当发送的数据在lineData指针中的起始位置
	UINT sendStartAddr;//经过判断以后当前行发送出去的数据的首地址

	CString fileName;//文件名
	CString strLine;//ReadString 参数
	UCHAR *lineData;//行数据转换成char后的存放缓冲区
	UCHAR lineType;//当前行的类型，s1,s2,s3...
	UCHAR lineLength;//当前行有效数据长度，包含地址，数据和校验=lineTotalLength-3
	UCHAR lineTotalLength;//当前行总长度，会经过字节合并
	UINT lineStartAddr;//当前行的起始地址
	UINT lineStopAddr;//当前行的结束地址
	UCHAR addrLength;//当前行地址位的长度s1-2;s2-3;s3-4.
	UCHAR dataLength;//当前行数据长度=lineLength-addrLength-1;
	ULONGLONG fileLength;//文件总长度
	UINT mainStartAddr;//main函数的地址

	UCHAR *dataSend;//给外部的发送数据

	UINT sendedLength;//已经发送的字节数，writedata时用
	UCHAR sendedPercent;//已经发送的数据的比例
	//当全部发送时，这个比例根据当前指针的位置在文件中的比例决定
	//当发送部分时，这个数据根据已发送的字节数在需要发送的字节数的占比来决定
	//当文件读取完成时，判断该比例，若=0，则表明所给的范围不存在

private:
	BOOL IsEnd();//文件是否结束
	int ReadAndTurn();//读取并转换成16进制
	UCHAR GenerateLineType();//获取行数据类型
	UCHAR GenerateLineLength();//获取行长度
	UCHAR GenerateAddrLength();//获取地址位长度
	UINT GenerateLineStartAddress();//获取行数据起始地址
	int IsDataInside();//数据是否在范围内
	void GenerateSendData();//生成要发送的数据

	char strTobin( char a )
	{
		if (a>='0' && a<='9')
		{
			a -= 0x30;
		}
		else if (a>='A' && a<='F')
		{
			a -= 0x37;
		}else if (a>='a' && a<='f')
		{
			a -= 0x57;
		}
		return a;
	}

public:
	//外部接口
	//使用前必须调用此函数
	//使用前检查文件
	int CheckSrecFile();
	BOOL SetArrange(BOOL sendAllData,UINT leftSide = 0, UINT rightSide = 0);//sendAllData为FALSE：只写数据，设置数据范围
	int ReadNextLine();//读取数据

	const UCHAR *GetDataSend();
	UCHAR GetDataSendLength();
	UINT GetDataSendAddr();
	UINT GetMainStartAddr();
	UCHAR GetSendedPercent();
};


#endif