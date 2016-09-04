#define _CRT_SECURE_NO_WARNINGS 1
#include"BigData.h"

BigData::BigData(INT64 value)
      :_value(value)
{
	INT64ToString();//在构造函数时将数字转化成字符串
}

BigData::BigData(const char* pData)//对大数进行处理，优化
{
	//几种情况："     ","   56789","lyf1931256","+123456","00001234","12345xyz","123456789";
    assert(pData);
	char* pStr = (char*)pData;
	//"    ","   56789","lyf1931256"
	while (isspace(*pStr) || !isdigit(*pStr) && *pStr != '+'&&*pStr != '-')//isspace()消除空格,isdigit()判断是否为数字字符
	{
		if (*pStr == NULL)
			return;
		pStr++;
	}
	//"+123456"
	char cSymbol = *pStr;//标志符号位
	if ('+' == cSymbol || '-' == cSymbol)
	{
		pStr++;
	}
	else if (cSymbol >= '0' && cSymbol <= '9')
	{
		cSymbol = '+';
	}
	else 
		return;
	//"00001234"
	while ('0' == *pStr)
	{
		pStr++;
	}
	//"12345xyz"
	_strData.resize(strlen(pData) + 1);//string中resize()函数改变本字符串的大小
	_strData[0] = cSymbol;//第一位存放符号位
	_value = 0;
	int icount = 1;
	while (*pStr >= '0' && *pStr <= '9')//可使用isdigit，isdigit()判断是否为数字字符
	{
		_value = 10 * _value + *pStr - '0';
		_strData[icount++] = *pStr;
		pStr++;
	}
	_strData.resize(icount);//将本字符串的大小调到icount
	if ('-' == cSymbol)
	{
		_value = 0 - _value;//负值
	}
}

bool BigData::IsINT64OverFlow()const//判断大数是否溢出
{//64位中数字范围为：[-Ox8FFFFFFF FFFFFFFF，Ox7FFFFFFF FFFFFFFF]
	std::string temp = "+9223372036854775807";
	if ('-' == _strData[0])
	{
		temp = "-9223372036854775808";
	}
	//比较该大数与边界的size，相等时进行字符串直接比较
	if (_strData.size() < temp.size() || _strData.size() == temp.size() && _strData <= temp)
		return true;
	else
		return false;
}

void BigData::INT64ToString()//将_value转化成string类型
{//append()在字符串的末尾添加num个字符ch-----basic_string &(append(size_t num,char ch))
	char cSymbol = '+';
	if (_value < 0)
	{
		cSymbol = '-';
	}
	//12345->"54321"
	INT64 temp = _value;
	_strData.append(1, cSymbol);
	if (cSymbol == '-')//负数转变成正数再模除
	{
		temp = 0 - temp;
	}
	while (temp)
	{
		_strData.append(1, temp % 10 + '0');
		temp /= 10;
	}
	//"54321"->"12345"
	char* pLeft = (char*)_strData.c_str() + 1;
	char* pRight = pLeft + _strData.size() - 2;//包含符号位，故减2
	while (pLeft < pRight)
	{
		char tmp = *pLeft;
		*pLeft = *pRight;
		*pRight = tmp;
		pLeft++;
		pRight--;
	}
}

std::ostream& (operator<<(std::ostream& _cout, const BigData& bigdata))//输出大数
{//判断是否溢出，'+'不需要输出
	if (bigdata.IsINT64OverFlow())//没有溢出
	{
		_cout << bigdata._value << std::endl;
	}
	else
	{//c_str()函数返回一个指向正规C字符串的指针, 内容与本字符串相同
		char* pData = (char*)bigdata._strData.c_str();
		if ('+' == pData[0])
			pData++;
		_cout << pData << std::endl;
	}
	return _cout;
}

//大数四则运算
BigData BigData::operator+(const BigData& bigdata)
{
	//两个数都没溢出，结果也没溢出，直接进行相加
	if (IsINT64OverFlow() && bigdata.IsINT64OverFlow())
	{
		//两个数一正一负
		if (_strData[0] != bigdata._strData[0])
		{
			return _value + bigdata._value;
		}
		else
		{//两个数同号，没溢出的情况：如果边界是10，则10-3=7>=6 8,-10-(-3)=-7=<-6 -8
			if ((_value >= 0 && (MAX_INT64 - _value >= bigdata._value)) ||
				(_value < 0 && (MIN_INT64 - _value <= bigdata._value)))
			{
				return _value + bigdata._value;
			}
			else
			{
				return BigData(Add(_strData, bigdata._strData).c_str());
			}
		}
	}
	//两个数至少一个溢出，结果溢出
	//同号
	if (_strData[0] == bigdata._strData[0])
	{
		return BigData(Add(_strData, bigdata._strData).c_str());//c._str(),size()
	}
	//异号
	else
	{//两个数异号a,b；b为正数需要换负号，b为负数需要换正号
		string _StrData = bigdata._strData;//注意在此处定义字符串，不是BigData
		if (_StrData[0] == '+')
		{
			_StrData[0] = '-';
		}
		else
		{
			_StrData[0] = '+';
		}
		return BigData(Sub(_strData, _StrData).c_str());
	}
	return BigData(INT64(0));
}

BigData BigData::operator-(const BigData& bigdata)
{
	//两个数都没溢出，结果也没溢出，直接进行相加
	if (IsINT64OverFlow() && bigdata.IsINT64OverFlow())
	{
		if (_strData[0] == bigdata._strData[0])
		{
			return _value + bigdata._value;
		}
		else
		{
			//两个数异号，相减没溢出
			//-10 + 3 = -7 <= -6(减式：3-(-6))；10+（-3）= 7 >= 6(减式：-3-(6))
			if (_value >= 0 && (MIN_INT64 + _value <= bigdata._value) ||
				_value < 0 && (MAX_INT64 + _value >= bigdata._value))
			{
				return _value + bigdata._value;
			}
			else
			{//不用使bigdata._strData[0]设为'-',Add符号随左边数字即被减数(-9999-1 = -10000)
				BigData(Add(_strData, bigdata._strData).c_str());
			}
		}
	}
	//两个数至少一个溢出，
	//同号调用减法
	if (_strData[0] == bigdata._strData[0])
	{
		return BigData(Sub(_strData, bigdata._strData).c_str());
	}
	else
	{
		return BigData(Add(_strData, bigdata._strData).c_str());
	}
	return BigData(INT64(0));
}

BigData BigData::operator*(const BigData& bigdata)
{
	if (IsINT64OverFlow() && bigdata.IsINT64OverFlow())
	{
		if (_strData[0] == bigdata._strData[0])
		{
		    //例如:边界是10，10 / 2 = 5 > 4; 10 / -2 = -5 < -4;
			if (_value > 0 && MAX_INT64 / _value >= bigdata._value ||
				_value < 0 && MAX_INT64 / _value <= bigdata._value)
			{
				return _value*bigdata._value;
			}
		}
		else
		{
			//例如:边界是10，10 / 2 = 5 > -(-4);10 / -2 = -5 < -(4);(不能用MIN_INT64计算会出错)
			//if (_value>0 && MIN_INT64 / _value <= bigdata._value ||
			//	_value < 0 && MIN_INT64 / _value >= bigdata._value)
			if (_value>0 && MAX_INT64 / _value >= -bigdata._value ||
				_value < 0 && MAX_INT64 / _value <= -bigdata._value)
			{
				return _value*bigdata._value;
			}
		}
	}
	//两数至少有一个溢出
	if (_value != 0 && bigdata._value != 0)
	{
		return BigData(Mul(_strData, bigdata._strData).c_str());
	}
	return BigData(INT64(0));
}

BigData BigData::operator/(const BigData& bigdata)
{
	//1、除数为0
	if (bigdata._strData[1] == '0')
	{
		assert(false);
	}
	//2、两个数没溢出
	if (IsINT64OverFlow() && bigdata.IsINT64OverFlow())
	{
		return _value / bigdata._value;
	}
	//3、除数为1或-1
	if (bigdata._value == 1 || bigdata._value == -1)
	{
		if (bigdata._strData[0] != _strData[0])
		{
			return -_value;
		}
		return _value;
	}
	//4、除数和被除数相等
	//if (strcmp(_strData.data() + 1, bigdata._strData.data() + 1) == 0)
	//data()返回内容的字符数组形式
	if (strcmp(_strData.c_str() + 1, bigdata._strData.c_str() + 1) == 0)
	{
		if (_strData[0] != bigdata._strData[0])
		{
			return BigData(INT64(-1));
		}
		return BigData(INT64(1));
	}
	if (_strData.size() < bigdata._strData.size() ||
		_strData.size() == bigdata._strData.size() &&
		strcmp(_strData.c_str() + 1, bigdata._strData.c_str() + 1) < 0)
	{
		return BigData(INT64(0));
	}
	return BigData(Div(_strData, bigdata._strData).c_str());
}

std::string BigData::Add(std::string left, std::string right)
{
	int iLsize = left.size();
	int iRsize = right.size();
	if (iLsize < iRsize)//只需要左边为长度长的即可
	{
		std::swap(left, right);
		std::swap(iLsize, iRsize);
	}
	std::string sRet;
	sRet.resize(iLsize + 1);//相加不会超过较大数的size+1
	sRet[0] = left[0];
	char Step = 0;//进位
	//通过模除得到每位的数字及进位Step
	for (int iIdx = 1; iIdx < iLsize;iIdx++)
	{
		int cRet = left[iLsize - iIdx] + Step - '0';
		if (iIdx < iRsize)
		{
			cRet += right[iRsize - iIdx] - '0';//cRet转为数字，-‘0’
		}
		sRet[iLsize - iIdx + 1] = cRet % 10 + '0';//sRet比iLsize多一位,存放字符，故+‘0’
		Step = cRet / 10;
	}
	sRet[1] = Step + '0';
	return sRet;
}

std::string BigData::Sub(std::string left, std::string right)
{
	int iLsize = left.size();
	int iRsize = right.size();
	char cSymbol = left[0];//保存所得差的符号位
	if (iLsize < iRsize || (iLsize==iRsize && left < right))//左边小于右边都进行交换
	{//-12-(-21)=9,21-34=-13发现两数的差与减数的相反
		std::swap(left,right);
		std::swap(iLsize, iRsize);
		if (cSymbol == '-')
		{
			cSymbol = '+';
		}
		else
		{
			cSymbol = '-';
		}
	}
	std::string sRet;
	sRet.resize(iLsize);
	sRet[0] = cSymbol;//保存符号位
	for (int iIdx = 1; iIdx < iLsize; iIdx++)//结束标志为iLsize-1,不是iLsize
	{
		//从低到高，取left每一位；
		char cRet = left[iLsize - iIdx] - '0';
		//在right范围内从低到高，取right每一位，然后相减；
		if (iIdx < iRsize)
		{
			cRet -= right[iRsize - iIdx] - '0';
		}
		//判断是否借位
		if (cRet < 0)
		{
			left[iLsize - iIdx - 1] -= 1;
			cRet += 10;
		}
		sRet[iLsize - iIdx] = cRet + '0';
	}
	return sRet;
}

std::string BigData::Mul(std::string left, std::string right)
{
	int iLsize = left.size();
	int iRsize = right.size();
	char cSymbol = '+';//确认符号位
	if (left[0] != right[0])
	{
		cSymbol = '-';
	}
	if (iLsize > iRsize)//使较小的数为left，提高效率。eg:99*12345678
	{
		swap(left, right);
		swap(iLsize, iRsize);
	}
	std::string sRet;
	//两个数相乘最大位数为两个数位数的和，left和right中有符号位故减1
	sRet.assign(iLsize + iRsize - 1, '0');//assign()为字符串赋新值'0'
	sRet[0] = cSymbol;
	int iDataLen = iLsize + iRsize - 1;
	int ioffset = 0;//移位
	//先取左边一位和右边相乘；再考虑移位可得到左边每位与右边相乘的结果
	for (int iLdx = 1; iLdx < iLsize; iLdx++)
	{
		char cLeft = left[iLsize - iLdx] - '0';
		if (cLeft == 0)//如果left中含有0，偏移量加1
		{
			ioffset++;
			continue;
		}
		char Step = 0;
		//99*999=89910+8991;
		for (int iRdx = 1; iRdx < iRsize; iRdx++)
		{
			char cRet = cLeft * (right[iRsize - iRdx] - '0') + Step;
			cRet += sRet[iDataLen - iRdx - ioffset] - '0';//cRet存放当前位最终乘加的结果
			sRet[iDataLen - iRdx - ioffset] = cRet % 10 + '0';//存放字符+'0'
			Step = cRet / 10;
		}
		sRet[iDataLen - iRsize - ioffset] += Step;
		ioffset++;
	}
	return sRet;
}

std::string BigData::Div(std::string left, std::string right)
{//此处用append()对字符串依次赋值
	std::string sRet;
	sRet.append(1, '+');
	if (left[0] != right[0])
	{
		sRet[0] = '-';
	}
	char* pLeft = (char*)left.c_str() + 1;
	char* pRight = (char*)right.c_str() + 1;
	int DataLen = right.size() - 1;//标记相除的除数位数
	int Lsize = left.size() - 1;
	int Rsize = right.size() - 1;
	//eg:222222/33首先取到22和33比较大小，如果大就直接相除，否则DataLen++；
	for (int iIdx = 0; iIdx < Lsize;)
	{
		if (!(IsLeftstrBig(pLeft, DataLen, pRight, Rsize)))//如果取到的数小于除数时，结果商0，向后再取一位
		{
			sRet.append(1, '0');
			DataLen++;
		}
		else
		{
			sRet.append(1, SubLoop(pLeft, DataLen, pRight, Rsize));//循环相减得到该位的商
			//判断pLeft中进行循环相减后依次去掉0，
			while (*pLeft == '0' && DataLen > 0)
			{
				pLeft++;
				DataLen--;
				iIdx++;
			}
			DataLen++;
		}
		if (DataLen > Rsize + 1)//pLeft比pRight大一位结果为0，则pLeft中含有0
		{
			pLeft++;
			DataLen--;
			iIdx++;
		}
		if (DataLen + iIdx > Lsize)//判断是否除法结束
			break;
	}
	return sRet;
} 

char BigData::SubLoop(char* pLeft, int Lsize, const char* pRight, int Rsize)
{
	assert(pLeft && pRight);
	char cRet = 0;
	while (IsLeftstrBig(pLeft, Lsize, pRight, Rsize))//直到被减数小于减数停止运算
	{
		for (int iIdx = 0; iIdx < Rsize; iIdx++)//进行减运算
		{
			char ret = pLeft[Lsize - iIdx - 1] - '0';
			ret -= pRight[Rsize - iIdx - 1] - '0';
			if (ret < 0)
			{
				pLeft[Lsize - iIdx - 2] -= 1;
				ret += 10;
			}
			pLeft[Lsize - iIdx - 1] = ret + '0';
		}
		while (*pLeft == '0'&&Lsize>0)
		{
			pLeft++;
			Lsize--;
		}
		cRet++;
	}
	return cRet + '0';
}

bool BigData::IsLeftstrBig(const char* pLeft, int Lsize, const char* pRight, int Rsize)//判断是否left大于right
{
	assert(pLeft && pRight);
	char* pleft = (char*)pLeft;
	char* pright = (char*)pRight;
	if (Lsize > Rsize && *pleft > '0')//eg:112和33
	{
		return true;
	}
	else if (Lsize == Rsize)//eg:57和33
	{
		while (pright)
		{
			if (*pleft > *pright)
			{
				return true;
			}
			else if (*pleft == *pright)
			{
				pleft++;
				pright++;
			}
			else
				return false;
		}
	}
	return false;
}
