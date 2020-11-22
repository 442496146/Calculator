//��򵥱��ʽ��ֵ
#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MaxSize 100

class ErrorControl{
public:
	bool judge(char* exp) {
		int numOfSymbol = 0;//�����˫Ŀ�������
		int numOfNum = 0;//��������ָ����������ַ�����
		int numOfOpenparen = 0;//�������������
		int numOfCloseparen = 0;//�������������
		for (int i = 0; exp[i] != '\0'; i++) {
			if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/' || exp[i] == '^')
			{	//ͳ�������˫Ŀ�������
				numOfSymbol++;
			}
			else if ((exp[i] >= '0' && exp[i] <= '9'|| exp[i] == 'P')&&!(exp[i-1] >= '0' && exp[i-1] <= '9' || exp[i-1] == 'P' ||exp[i-1] == '.'))
			{	//ͳ����������ָ����������ַ�����
				numOfNum++;
			}
			else if (exp[i] == '(')
			{	//ͳ���������������
				numOfOpenparen++;
			}
			else if (exp[i] == ')')
			{	//ͳ���������������
				numOfCloseparen++;
			}
		}
		if (numOfSymbol + 1 != numOfNum|| numOfOpenparen != numOfCloseparen)
			return false;//�������Ĳ����Ϲ淶���򷵻�false����֪�˱��ʽ�����޷�����
		else
			return true;//�������ķ��Ϲ淶���򷵻�ture����֪�˱��ʽ��ȷ���ɼ���
	}
};

//---------------------------------------------------------
//--�����ջ��������---------------------------------------
//---------------------------------------------------------
typedef struct
{
	char data[MaxSize];			//��������
	int top;					//ջ��ָ��
} SqStack;
class Postexp {
public:
	void InitStack(SqStack*& s)		//��ʼ��ջ
	{
		s = (SqStack*)malloc(sizeof(SqStack));
		s->top = -1;
	}
	void DestroyStack(SqStack*& s)	//����ջ
	{
		free(s);
	}
	bool StackEmpty(SqStack* s)		//�ж�ջ�Ƿ�Ϊ��
	{
		return(s->top == -1);
	}
	bool Push(SqStack*& s, char e)	//��ջԪ��e
	{
		if (s->top == MaxSize - 1)
			return false;
		s->top++;
		s->data[s->top] = e;
		return true;
	}
	bool Pop(SqStack*& s, char& e)	//��ջԪ��e
	{
		if (s->top == -1)
			return false;
		e = s->data[s->top];
		s->top--;
		return true;
	}
	bool GetTop(SqStack* s, char& e)	//ȡջ��Ԫ��e
	{
		if (s->top == -1)
			return false;
		e = s->data[s->top];
		return true;
	}
	//---------------------------------------------------------

	void trans(char* exp, char postexp[])	//���������ʽexpת���ɺ�׺���ʽpostexp
	{
		char e;
		SqStack* Optr;						//���������ջ
		InitStack(Optr);					//��ʼ�������ջ
		int i = 0;							//i��Ϊpostexp���±�
		while (*exp != '\0')					//exp���ʽδɨ����ʱѭ��
		{
			switch (*exp)
			{
			case '(':						//�ж�Ϊ������
				Push(Optr, '(');				//�����Ž�ջ
				exp++;						//����ɨ�������ַ�
				break;
			case ')':						//�ж�Ϊ������
				Pop(Optr, e);				//��ջԪ��e
				while (e != '(')				//��Ϊ'('ʱѭ��
				{
					postexp[i++] = e;			//��e��ŵ�postexp��
					Pop(Optr, e);			//������ջԪ��e
				}
				exp++;						//����ɨ�������ַ�
				break;
			case '+':						//�ж�Ϊ�ӻ����
			case '-':
				while (!StackEmpty(Optr))	//ջ����ѭ��
				{
					GetTop(Optr, e);			//ȡջ��Ԫ��e
					if (e != '(')				//e����'('
					{
						postexp[i++] = e;		//��e��ŵ�postexp��
						Pop(Optr, e);		//��ջԪ��e
					}
					else					//e��'(ʱ�˳�ѭ��
						break;
				}
				Push(Optr, *exp);			//��'+'��'-'��ջ
				exp++;						//����ɨ�������ַ�
				break;
			case '*':						//�ж�Ϊ'*'��'/'��
			case '/':
				while (!StackEmpty(Optr))	//ջ����ѭ��
				{
					GetTop(Optr, e);			//ȡջ��Ԫ��e
					if(!(e=='(' || e=='+' || e=='-'))//��ջ��'*'��'/'�������ջ����ŵ�postexp��
					{
						postexp[i++] = e;		//��e��ŵ�postexp��
						Pop(Optr, e);		//��ջԪ��e
					}
					else					//eΪ��'*'��'/'�����ʱ�˳�ѭ��
						break;
				}
				Push(Optr, *exp);			//��'*'��'/'��ջ
				exp++;						//����ɨ�������ַ�
				break;
			case '^':
			case '!':
			case 'S':                        //�����š���S���
				while (!StackEmpty(Optr))	//ջ����ѭ��
				{
					GetTop(Optr, e);			//ȡջ��Ԫ��e
					if (e == '^' || e == '!' || e == 'S' )	//��ջ��'^'��'!'��'��'�������ջ����ŵ�postexp��
					{
						postexp[i++] = e;		//��e��ŵ�postexp��
						Pop(Optr, e);		//��ջԪ��e
					}
					else					//eΪ��'^'��'!'��'��'�����ʱ�˳�ѭ��
						break;
				}
				Push(Optr, *exp);			//��'^'��'!'��'��'��ջ
				exp++;						//����ɨ�������ַ�
				break;
			default:				//���������ַ�
				while (*exp >= '0' && *exp <= '9' || *exp == 'P'||*exp=='.') //�ж�Ϊ����
				{

					postexp[i++] = *exp;
					exp++;
				}
				postexp[i++] = '#';	//��#��ʶһ����ֵ������
			}
		}
		while (!StackEmpty(Optr))	//��ʱexpɨ�����,ջ����ʱѭ��
		{
			Pop(Optr, e);			//��ջԪ��e
			postexp[i++] = e;			//��e��ŵ�postexp��
		}
		postexp[i] = '\0';			//��postexp���ʽ��ӽ�����ʶ
		DestroyStack(Optr);			//����ջ		
	}
};

//---------------------------------------------------------
//--������ջ��������---------------------------------------
//---------------------------------------------------------
typedef struct
{
	double data[MaxSize];			//�����ֵ
	int top;						//ջ��ָ��
} SqStack1;
class Compvalue {
public:
	void InitStack1(SqStack1*& s)		//��ʼ��ջ
	{
		s = (SqStack1*)malloc(sizeof(SqStack1));
		s->top = -1;
	}
	void DestroyStack1(SqStack1*& s)	//����ջ
	{
		free(s);
	}
	bool StackEmpty1(SqStack1* s)		//�ж�ջ�Ƿ�Ϊ��
	{
		return(s->top == -1);
	}
	bool Push1(SqStack1*& s, double e)	//��ջԪ��e
	{
		if (s->top == MaxSize - 1)
			return false;
		s->top++;
		s->data[s->top] = e;
		return true;
	}
	bool Pop1(SqStack1*& s, double& e)	//��ջԪ��e
	{
		if (s->top == -1)
			return false;
		e = s->data[s->top];
		s->top--;
		return true;
	}
	bool GetTop1(SqStack1* s, double& e)	//ȡջ��Ԫ��e
	{
		if (s->top == -1)
			return false;
		e = s->data[s->top];
		return true;
	}
	//---------------------------------------------------------

	CString compvalue(char* postexp)	//�����׺���ʽ��ֵ
	{
		CString s;
		bool arrivePoint;            //�ж�������ֵʱ�Ƿ�ΪС�������Ƿ�����С����
		int numOfPointRight;         //�ۼ���������С�����ұ��м�λ
		double pointLeft,pointRight,a, b, c, e;//pointLeft����������������ֵ,pointRight������С��������ֵ
		SqStack1* Opnd;				//���������ջ
		InitStack1(Opnd);			//��ʼ��������ջ
		while (*postexp != '\0')		//postexp�ַ���δɨ����ʱѭ��
		{
			switch (*postexp)
			{
			case '+':				//�ж�Ϊ'+'��
				Pop1(Opnd, a);		//��ջԪ��a
				Pop1(Opnd, b);		//��ջԪ��b
				c = b + a;				//����c
				Push1(Opnd, c);		//��������c��ջ
				break;
			case '-':				//�ж�Ϊ'-'��
				Pop1(Opnd, a);		//��ջԪ��a
				Pop1(Opnd, b);		//��ջԪ��b
				c = b - a;				//����c
				Push1(Opnd, c);		//��������c��ջ
				break;
			case '*':				//�ж�Ϊ'*'��
				Pop1(Opnd, a);		//��ջԪ��a
				Pop1(Opnd, b);		//��ջԪ��b
				c = b * a;				//����c
				Push1(Opnd, c);		//��������c��ջ
				break;
			case '^':				//�ж�Ϊ'^'��
				Pop1(Opnd, a);		//��ջԪ��a
				Pop1(Opnd, b);		//��ջԪ��b
				c = pow(b, a);			//����c
				Push1(Opnd, c);	//��������c��ջ
				break;
			case 'S':				//�ж�Ϊ'��'��
				Pop1(Opnd, a);		//��ջԪ��a
				c = sqrt(a);			//����c
				Push1(Opnd, c);	//��������c��ջ
				break;
			case '!':				//�ж�Ϊ'!'��
				Pop1(Opnd, a);		//��ջԪ��a
				c = 1;
				if (a != 0) {
					if (a - (int)a == 0)
						for (int i = 1; i <= a; i++)
							c = c * i;      //����c
					else                   //���׳���ֵ��Ϊ����ʱ��ʾ����
					{
						s = "����";      //��ʾ����
						DestroyStack1(Opnd);		//����ջ
						return s;
					}
				}
				Push1(Opnd, c);	//��������c��ջ
				break;
			case '/':				//�ж�Ϊ'/'��
				Pop1(Opnd, a);		//��ջԪ��a
				Pop1(Opnd, b);		//��ջԪ��b
				if (a != 0)
				{
					c = b / a;			//����c
					Push1(Opnd, c);	//��������c��ջ
					break;
				}
				else
				{
					s = "����";      //��ʾ����
					DestroyStack1(Opnd);		//����ջ
					return s;
				}
				break;
			default:				//���������ַ�
				pointLeft = 0;				//�������������ַ�ת���ɶ�Ӧ����ֵ��ŵ�d��
				pointRight = 0;
				numOfPointRight = 0;
				arrivePoint = false;
				while (*postexp >= '0' && *postexp <= '9' || *postexp == 'P'||*postexp=='.')   //�ж�Ϊ�����ַ�
				{
					if (*postexp == 'P')   //�ж��Ƿ������Ϊ��
					{
						pointLeft = 3.1415926535897;
						postexp++;
					}
					else
					{
						if (*postexp != '.'&& !arrivePoint) {           //����С������ߵ���ֵ��С
							pointLeft = 10 * pointLeft + *postexp - '0';
							postexp++;
						}
						else if (*postexp == '.')      //��⵽С����
						{
							arrivePoint = true;
							postexp++;
						}
						else                          //����С�����ұ���ֵ��С
						{
							numOfPointRight++;        //����С�����ұ�λ��
							pointRight = pointRight + (*postexp - '0')*pow(0.1,numOfPointRight);
							postexp++;
						}
					}
					c = pointLeft + pointRight;     //С����������ֵ��ӣ���������ֵ
				}
				Push1(Opnd, c);		//����ֵc��ջ
				break;
			}
			postexp++;				//�������������ַ�
		}
		GetTop1(Opnd, e);			//ȡջ��Ԫ��e
		DestroyStack1(Opnd);		//����ջ
		s.Format(_T("%lf"), e);		//��double��eת��CString��s
		return s;					//����s
	}
};