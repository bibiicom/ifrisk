////////////////////////////////////////////////////////////////////////////////////////////////////
//实验一：表达式求值
//文件名:ExpEvaluate.h
//文件功能：实现表达式的中缀转换成后缀,并且计算出结果

#ifndef EXPEVALUATE_H_INCLUDED
#define EXPEVALUATE_H_INCLUDED
#include <iostream>
#include "stack.h"
#pragma   warning(disable:4996)		//去掉编译警告：“warning C4996: 'strcpy' was declared deprecated”
#define     EXPRESION_SIZE    1000
class expr
{
private:
	char   Exp[EXPRESION_SIZE];
	char   PostExp[EXPRESION_SIZE];
	double _value;

public:
	expr()
	{
		_value = 0;
		for (int i=0; i<EXPRESION_SIZE; i++)
		{
			Exp[i] = '\0';
			PostExp[i]= '\0';
		}
	}

	void init(void)
	{
		for (int i=0; i<EXPRESION_SIZE; i++)
		{
			Exp[i] = '\0';
			PostExp[i]= '\0';
		}
	}
	////////////////////////////////////////////////////////////////////////////////
	//
	///得到该表达式计算后的结果
	//
	double GetValue(void)
	{
		return _value;
	}
	////////////////////////////////////////////////////////////////////////////////
	//
	///判断用户输入的表达式是否为合法的
	//
	bool IsLegal(char *a)
	{
		char temp[EXPRESION_SIZE];
		char test;
		int   length = strlen(a);
		int   left=0,right=0;
		int   isnumber=0, ischar=0;

		if(length == 0)
		{
			//cout<<"输入为空,请输入表达式！"<<endl;
			return false;
		}
		int i = 0,k = 0,j=0;
		for(i=0; i<EXPRESION_SIZE; i++)
			temp[i] = '\0';
		///////////////////////////////////////////////////////////////////
		///过滤掉空格
		for(i=0,k=0; i<length; i++,k++)
		{
			test = a[i];
			if(test == ' ')
			{
				k--;
				continue;
			}
			temp[k] = test;
		}
		strcpy(a,temp);
		length = strlen(a);
		///////////////////////////////////////////////////////////////////
		///长度不合法
		for(i=0; i<EXPRESION_SIZE; i++)
			temp[i] = '\0';
		if(length > EXPRESION_SIZE)
		{
			//cout<<"存储空间不够！"<<endl;
			exit(0);
		}
		///////////////////////////////////////////////////////////////////
		///处理数字与括号间省略括号的情况
		for(j=0,k=0; j<length; j++,k++)
		{
			test = a[j];
			if(test == '(')
			{
				if(j!=0)
				{
					if((a[j-1]>='0' && a[j-1]<='9') || a[j-1] == ')')
					{
						temp[k] = '*';    ///添上省略的括号
						temp[k+1] = test;
						k++;
						continue;
					}
				}
			}
			temp[k] = test;
		}
		strcpy(a,temp);
		length = strlen(a);
		for(i=0; i<EXPRESION_SIZE; i++)
			temp[i] = '\0';
		///////////////////////////////////////////////////////////////////
		///处理负数问题
		for(j=0,k=0; j<length; j++,k++)
		{
			test = a[j];
			if(j==0 && test == '-')
			{
				temp[k] = '0';        ///第一个数是负数的时候,在最前面加一个0
				temp[k++]='-';
				continue;
			}
			if(test == '(' && a[j+1] == '-')
			{
				temp[k]   = '(';     ///遇到加括号的负数时,在括号和负号之间添上一个0
				temp[k+1] = '0';
				temp[k+2] = '-';
				k+=2;
				j++;
				continue;
			}
			temp[k] = test;
		}
		strcpy(a,temp);
		length = strlen(a);
		///////////////////////////////////////////////////////////////////
		///判断有没有非法的字符
		int m = 0;
		for( m=0; m<length; m++)
		{
			test = a[m];
			if(!(test=='+'||test=='-'||test=='*'||test=='/'||test=='^'||test=='('||test==')'||(test<='Z'&&test>='A')||(test<='9'&&test>='0')||test=='.'))
			{
				//cout<<"含有非法的字符！"<<endl;
				return false;
			}
		}
		///////////////////////////////////////////////////////////////////
		///检测括号是否匹配
		for(m=0; m<length; m++)
		{
			test = a[m];
			if(test == '(')
				left++;
			if(test == ')')
				right++;
		}
		if(left != right)
		{
			//cout<<"括号不匹配！"<<endl;
			return false;
		}
		///////////////////////////////////////////////////////////////////
		///判断表达式的一些数字与符号的错误
		for(int p=0; p<length; p++)
		{
			test = a[p];
			///////////////////////////////////////////////////////////////////
			///判断小数的非法
			if(test=='.')
			{
				//第一位与最后一位不能是小数点
				if(p==0 || p==length-1)
				{
					//cout<<"小数点有错误！"<<endl;
					return false;
				}
				//小数点前与后不能是非数字
				if((a[p+1]>'9' && a[p+1]<'0') || (a[p-1]>'9' && a[p-1]<'0'))
				{
					//cout<<"小数点有错误！"<<endl;
					return false;
				}
			}
			///////////////////////////////////////////////////////////////////
			///判断运算符和数字的数量是否合法
			if((test<='9'&&test>='0') || test=='.'|| (test<='Z'&&test>='A'))
			{
				while(test<='9'&&test>='0' || test== '.' || (test<='Z'&&test>='A'))
				{
					test = a[++p];
				}
				isnumber++;
				p--;
				continue;
			}
			if(test=='+'||test=='-'||test=='*'||test=='/'||test=='^')
			{
				ischar++;
				continue;
			}
		}
		///符号数量少
		if(isnumber > ischar+1)
		{
			////cout<<"缺少符号！"<<endl;
			return false;
		}
		///数字数量少
		if(isnumber < ischar+1)
		{
			////cout<<"缺少数字！"<<endl;
			return false;
		}
		return true;
	}
	////////////////////////////////////////////////////////////////////////////////
	//
	///使用提供的字符串来设置表达式前缀的值
	//
	bool SetExp(char* a)
	{
		if (IsLegal(a))
		{
			strcpy(Exp, a);
			return true;
		}
		else
			//cout<<"输入的表达式不合法！请重新输入！"<<endl;
		return false;
	}
	////////////////////////////////////////////////////////////////////////////////
	//
	///将表达式前缀转换为后缀,这里优先级已经体现在程序中,没有另外建立判断函数
	//
	void translate(void)
	{
		stackchar oprate;
		char temp = *Exp;
		char Top;
		char elem;
		int   j=0;

		for (int i=0; temp != '\0'; i++)
		{
			temp = *(Exp+i);
			switch (temp)
			{
			case '(':        ///遇左括号进栈
				oprate.push(temp);
				break;
			case ')':
				if (!oprate.IsEmpty())    ///遇右括号一直出栈直到栈顶为左括号,并且将左括号删除
				{
					oprate.GetTop(Top);
					while (Top != '(')
					{
						oprate.pop(elem);
						PostExp[j++] = elem;
						if (!oprate.IsEmpty())
							oprate.GetTop(Top);
						else
							break;
					}

					oprate.pop(elem);
				}
				break;
			case '+':
			case '-':        ///遇加减号优先级相同,可以一同处理
				if (!oprate.IsEmpty())
				{
					oprate.GetTop(Top);
					while (Top != '(')
					{
						oprate.pop(elem);
						PostExp[j++] = elem;
						if (!oprate.IsEmpty())
							oprate.GetTop(Top);
						else
							break;
					}
				}
				oprate.push(temp);
				break;
			case '^':
				if (!oprate.IsEmpty())
				{
					oprate.GetTop(Top);
					while (Top=='^')      ///优先级最高
					{
						oprate.pop(elem);
						PostExp[j++] = elem;
						if (!oprate.IsEmpty())
							oprate.GetTop(Top);
						else
							break;
					}
				}
				oprate.push(temp);
				break;
			case '*':
			case '/':        ///乘除法优先级也是相同的
				if (!oprate.IsEmpty())
				{
					oprate.GetTop(Top);
					while (Top=='*' || Top=='/' || Top=='^')
					{
						oprate.pop(elem);
						PostExp[j++] = elem;
						if (!oprate.IsEmpty())
							oprate.GetTop(Top);
						else
							break;
					}
				}
				oprate.push(temp);
				break;
			case ' ':          ///过滤空格
				break;
			default:          ///处理数字,并以#号作为数字结束符号
				while ((temp>='0' && temp<='9') || temp=='.')
				{
					PostExp[j++] = temp;
					i++;
					temp = *(Exp+i);
				}
				i--;
				PostExp[j++] = '#';
				break;
			}
		}

		while (!oprate.IsEmpty())
		{
			oprate.pop(elem);
			PostExp[j++] = elem;
		}
		PostExp[j] = '\0';
	}
	////////////////////////////////////////////////////////////////////////////////
	//
	///计算后缀表达式的值
	//
	bool ComputeValue(void)
	{
		stackdouble number;
		char   temp = *PostExp;         ///用来暂存字符
		double one=0, two=0, value=0;     ///运算的数和结果
		double date = 0, ispoint = 0;
		int    count=0;
		int    length = strlen(PostExp);    ///后缀表达式的长度

		for (int i=0; i<length; i++)
		{
			temp = *(PostExp+i);

			switch (temp)
			{
			case '+':          ///遇到加号将数字出栈,进行运算
				one = 0;
				two = 0;
				value = 0;
				number.pop(one);
				number.pop(two);
				value = one + two;
				number.push(value);
				break;
			case '-':          ///遇到号将数字出栈,进行运算
				one = 0;
				two = 0;
				value = 0;
				number.pop(one);
				number.pop(two);
				value = two - one;
				number.push(value);
				break;
			case '*':                               ///计算乘号
				one = 0;
				two = 0;
				value = 0;
				number.pop(one);
				number.pop(two);
				value = two * one;
				number.push(value);
				break;
			case '/':                               ///计算除号
				one = 0;
				two = 0;
				value = 0;
				number.pop(one);
				number.pop(two);
				if (one != 0)
					value = two / one;
				else
				{
					//cout<<"除零错误！退出"<<endl; /// 除数为零计算失败
					return false;
				}
				number.push(value);
				break;
			case '^':           ///计算指数
				one = 0;
				two = 1;
				value = 0;
				number.pop(one);
				number.pop(two);
				if (two > 0)
					value = pow(two, one);
				else
				{
					//cout<<"错误！退出"<<endl;
					exit(0);
				}
				number.push(value);
				break;
			default:
				if (temp != '#')
				{
					date    = 0;
					ispoint = 0;
					count   = 0;
					///////////////////////////////////////////////////////////////////////////////
					///将字符串的数字转换为数可以用库函数,这里自己编写的
					while ((temp>='0' && temp <='9') || temp=='.')
					{
						if(temp == '.')    ///遇到小数点然后后面的数字计算方式会改变
						{
							ispoint = 1;
							i++;
							temp = *(PostExp+i);
							continue;
						}
						if (ispoint==0)
						{
							date=date*10+temp-'0';
						}
						else
						{
							count++;
							date += (temp-'0')*pow(0.1,count);
						}
						i++;
						temp = *(PostExp+i);
					}
					i--;
					number.push(date);
				}
			}
		}
		number.pop(_value);
		return true;
	}
	////////////////////////////////////////////////////////////////////////////////
	//
	///将表达式的中缀式和后缀式以及最终的结果打印输出
	//
	void Display(void)
	{
		//cout<<"中缀表达式为："<<Exp<<endl;
		//cout<<"后缀表达式为："<<PostExp<<endl;
	}
};

#endif
