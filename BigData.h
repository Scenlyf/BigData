#ifndef BIG_DATA_H
#define BIG_DATA_H

#include<iostream>
using namespace std;
#include<assert.h>
#include<string>
 
#define UN_INTT 0xCCCCCCCCCCCCCCCC
#define MAX_INT64 0x7FFFFFFFFFFFFFFF
#define MIN_INT64 0x8000000000000000

typedef long long INT64;

//内置类型
class BigData
{
public:
    BigData(INT64 value = 0xCCCCCCCCCCCCCCCC);//BigData(INT64 value);
	BigData(const char* pData);//对大数进行处理，优化
	friend std::ostream& (operator<<(std::ostream& _cout, const BigData& bigdata));//输出大数
public:
	//大数的四则运算
	BigData operator+(const BigData& bigdata);//返回值不能传引用
	BigData operator-(const BigData& bigdata);
	BigData operator*(const BigData& bigdata);
	BigData operator/(const BigData& bigdata);
	std::string Add(std::string left, std::string right);
	std::string Sub(std::string left, std::string right);
	std::string Mul(std::string left, std::string right);
	std::string Div(std::string left, std::string right);
private:
	bool IsINT64OverFlow()const;//判断大数是否溢出
	void INT64ToString();//由于值在_value中，需将_value转化成string类型
	char SubLoop(char* pLeft, int Lsize, const char* pRight, int Rsize);//循环相减
	bool IsLeftstrBig(const char* pLeft, int Lsize, const char* pRight, int Rsize);//判断是否left大于right
private:
	std::string _strData;
	INT64 _value;
};

#endif
