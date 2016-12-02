// TurnDll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "TurnDll.h"



TURNDLL_API char strTobin( char a )
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

TURNDLL_API UINT CalculateKey(UINT a)
{
	return a*a + a + 1;
}
