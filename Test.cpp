#define _CRT_SECURE_NO_WARNINGS 1
#include"BigData.h"

void Test1()
{//构造大数函数
	////将字符串转换成一个整数并返回结果int atoi(const char*)
	//cout << atoi("12345") << endl;
	//cout << atoi("+123456") << endl;
	//cout << atoi("00001234567") << endl;
	//cout << atoi("    ") << endl;
	//cout << atoi("12345678xyz") << endl;
	BigData p0 = 12345;
	BigData p01 = "       ";//产生随机值
	BigData p11 = "   56789";
	BigData p12 = "lyf1931256";
	BigData p1 = "    ";
	BigData p2 = "+12345";
	BigData p3 = "-12345";
	BigData p4 = "00000000000123456";
	BigData p5 = "1234567xygdu";
	BigData p6 = "12345678";
	BigData p7 = "-9223372036854775819";
	BigData p8 = "+9223372036854775818";
	cout << p0 << p01 << p11 << p12 << p1 << p2 << endl;
	cout << p3 << p4 << p5 << p6 << p7 << p8 << endl;
}

void Test2()
{
	//BigData left = 12345;
	//BigData right = "-21";

	//BigData left = "9223372036854775806";
	//BigData right = 10;

	//BigData left = "-9223372036854775818";//BigData left = -922337203685477581;//此写法不能越界
	//BigData right = "-2";
	
	//BigData left = "9223372036854775818";
	//BigData right = 11111111111;//此写法不能越界

	//调用减法运算
	//BigData left =  "9223372036854775818";
	//BigData right = "-1111111111111111111";

	//BigData left = "-9223372036854775818";
	//BigData right = "1111111111111111111";

	//BigData left = "-1111111111111111111";
	//BigData right = "9223372036854775818";

	////BigData left = "1111111111111111111";
	////BigData right = "-9223372036854775818";
	//BigData ret = left + right;
	//cout << ret << endl;


	BigData left = "11111111111111111111111111";
	BigData right = "111111111";
	BigData ret = left + right;
	cout << ret << endl;
}

void Test3()
{
	//BigData left = "12345";
	//BigData right = "-123";

	//BigData left = "-9223372036854775807";
	//BigData right = "123";

	//BigData left = "9223372036854775807";
	//BigData right = "-123";
	////至少一个溢出
	//BigData left = "9223372036854775817";
	//BigData right = "-123";

	BigData left = "9223372036854775817123456";
	BigData right = "79";

	//BigData left = "-9223372036854775817";
	//BigData right = "9223372036854775818";
	BigData ret = left - right;
	cout << ret << endl;
}

void Test4()
{
	//BigData left = "666666";
	//BigData right = "333";

	//BigData left = "66666666";
	//BigData right = "-333";
	//两数都没溢出，乘积溢出
	BigData left = "111111111";
	BigData right = "-1111111111111111111";
	//两数至少一个溢出
	//BigData left = "660000000000000000666";
	//BigData right = "333";

	//BigData left = "-6666666666666666666666";
	//BigData right = "-333";

	//BigData left = "-6666666666666666666666";
	//BigData right = "333";
	BigData ret = left*right;
	cout << ret << endl;
}

void Test5()
{
	//BigData left = "-9999999999999999999999";
	//BigData right = "9999999999999999999999";

	//BigData left = "-222222222222222222222222222222";
	//BigData right = "33";

	//BigData left = "-222222000222222222222222222222222";
	//BigData right = "33";

	BigData left = "-123456789100126772353276";
	BigData right = "123456";

	BigData ret = left / right;
	cout << ret << endl;
}

void Test6()
{
	string p1 = "1111111111111111111";
	string p2 = "-9223372036854775818";
	char* Left = (char*)p1.data() + 1;//data()返回内容的字符数组形式
	char* Right = (char*)p2.data() + 1;
	//bool ret = p1 < p2;//‘-’比‘+’大
	bool ret = strcmp(p1.c_str(), p2.c_str()) > 0;
	bool Ret = Left < Right;
	cout << ret << endl << Ret << endl;
}

int main()
{
	//Test1();
	//Test2();
	//Test3();
	Test4();
	//Test5();

	//Test6();
	system("pause");
	return 0;
}
