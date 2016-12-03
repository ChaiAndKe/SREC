/************************************************************************/
/*
1.�ṩ����д�ļ�Ӧ��֤��ַ�� 8 �ֽڶ��뼴ĩλ�� 0 ���� 8����λ���ڵ���д��ȡ�ļ�
ʱ����ȫ�ļ����һ�飬�����ֲ�����Ҫ����������ʾ�����ļ�������Ҫ����ļ���
���� ������������������д���̣�
2.��Ч���ݵĳ��� Length ӦΪ 4 �ı��������� 4 �ı���ʱ���� 4 ����С������ 0xff ���룻
3.֡��ʽ 2 �ĳ��ȹ̶�Ϊ 24 �ֽڣ���Ч���ݳ��Ȳ��� 16 �ֽ�ʱ������֡������ 0xff ����
Ϊ 16 �ֽ�
4.Data������Ч���ݳ��ȣ�Length = Data length����ֵֻ��Ϊ4�ı����� ��4/8/12/16.
5.д����λ���ĵ�ַ���ĸ��ֽڡ���ֵ����8�ֽڶ��룬��16���Ʊ�ʾʱ ĩλֻ��Ϊ0��8
������0x01000010,0x01000018����
6.��ַ����С��ģʽ������������srec��˳����,����0x11223344�����͵�ʱ���ַ����һ���ֽ���44���ڶ�����33
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
	Ҫ���͵����ݵ���ʼλ�úͽ���λ�ã�
	�ڶ�ȡһ�е�ʱ���ж�sendAllData=TRUE?
	TRUE��������ȫ������
	����Ǽ٣����ж������Ƿ��ڷ�Χ��
	*/
	BOOL sendAllData;//ȫ����Ч���Ƿ���ȫ������
	UINT dataToSendStartAddr;//ȫ�֣�д���ݵ���ʼ��ַ
	UINT dataToSendStopAddr;//ȫ�֣�д���ݵĽ�����ַ

	int dataToSendStartLineNum;//ȫ�֣�д���ݵ���ʼ��
	int dataToSendStopLineNum;//ȫ�֣�д���ݵĽ�����
	int curLineNum;//��ǰ�к�

	UCHAR sendLength;//�����ж��Ժ�ǰ��Ӧ�����͵����ݳ��ȣ��ڷ�Χ�ڣ�
	UCHAR sendStartPosition;//�����ж��Ժ�ǰ��Ӧ�����͵�������lineDataָ���е���ʼλ��
	UINT sendStartAddr;//�����ж��Ժ�ǰ�з��ͳ�ȥ�����ݵ��׵�ַ

	CString fileName;//�ļ���
	CString strLine;//ReadString ����
	UCHAR *lineData;//������ת����char��Ĵ�Ż�����
	UCHAR lineType;//��ǰ�е����ͣ�s1,s2,s3...
	UCHAR lineLength;//��ǰ����Ч���ݳ��ȣ�������ַ�����ݺ�У��=lineTotalLength-3
	UCHAR lineTotalLength;//��ǰ���ܳ��ȣ��ᾭ���ֽںϲ�
	UINT lineStartAddr;//��ǰ�е���ʼ��ַ
	UINT lineStopAddr;//��ǰ�еĽ�����ַ
	UCHAR addrLength;//��ǰ�е�ַλ�ĳ���s1-2;s2-3;s3-4.
	UCHAR dataLength;//��ǰ�����ݳ���=lineLength-addrLength-1;
	ULONGLONG fileLength;//�ļ��ܳ���
	UINT mainStartAddr;//main�����ĵ�ַ

	UCHAR *dataSend;//���ⲿ�ķ�������

	UINT sendedLength;//�Ѿ����͵��ֽ�����writedataʱ��
	UCHAR sendedPercent;//�Ѿ����͵����ݵı���
	//��ȫ������ʱ������������ݵ�ǰָ���λ�����ļ��еı�������
	//�����Ͳ���ʱ��������ݸ����ѷ��͵��ֽ�������Ҫ���͵��ֽ�����ռ��������
	//���ļ���ȡ���ʱ���жϸñ�������=0������������ķ�Χ������

private:
	BOOL IsEnd();//�ļ��Ƿ����
	int ReadAndTurn();//��ȡ��ת����16����
	UCHAR GenerateLineType();//��ȡ����������
	UCHAR GenerateLineLength();//��ȡ�г���
	UCHAR GenerateAddrLength();//��ȡ��ַλ����
	UINT GenerateLineStartAddress();//��ȡ��������ʼ��ַ
	int IsDataInside();//�����Ƿ��ڷ�Χ��
	void GenerateSendData();//����Ҫ���͵�����

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
	//�ⲿ�ӿ�
	//ʹ��ǰ������ô˺���
	//ʹ��ǰ����ļ�
	int CheckSrecFile();
	BOOL SetArrange(BOOL sendAllData,UINT leftSide = 0, UINT rightSide = 0);//sendAllDataΪFALSE��ֻд���ݣ��������ݷ�Χ
	int ReadNextLine();//��ȡ����

	const UCHAR *GetDataSend();
	UCHAR GetDataSendLength();
	UINT GetDataSendAddr();
	UINT GetMainStartAddr();
	UCHAR GetSendedPercent();
};


#endif