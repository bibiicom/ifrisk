#include "BigDigit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef _WIN32
#define stoi64(x) _atoi64(x)
#else
#define stoi64(x) strtoll(x,NULL,10)
#endif

const LONGINT _power = (LONGINT)pow((double)10, PREC_DEPTH);

void tostr(char *s, LONGINT num, LONGINT frac)
{
#ifdef _WIN32
	snprintf(s, 100, "%I64d%0*I64d", num, PREC_DEPTH, frac);
#else
	snprintf(s, 100, "%lld%0*lld", num, PREC_DEPTH, frac);
#endif
}

void multistr(char *a, char *b, char *c)
{
	int i, j, m, n;
	long sum, carry;
	m = (int)strlen(a) - 1;
	n = (int)strlen(b) - 1;
	for (i = m; i >= 0; i--)
		a[i] -= '0';
	for (i = n; i >= 0; i--)
		b[i] -= '0';
	c[m + n + 2] = '\0';
	carry = 0;
	for (i = m + n; i >= 0; i--)
	{
		sum = carry;
		if ((j = (i - m)) < 0)
			j = 0;
		for (; j <= i && j <= n; j++)
			sum += a[i - j] * b[j];
		c[i + 1] = sum % 10 + '0';
		carry = sum / 10;
	}
	c[0] = (char)carry + '0';
}

bool divcomp(char *a, int la, char *b, int lb)
{
	if (la > lb) return true;
	if (la < lb) return false;
	for (int i = 0; i < la; i++)
	{
		if (a[i] > b[i]) return true;
		if (a[i] < b[i]) return false;
	}
	return true;
}

void divminus(char *a, int la, char *b, int lb)
{
	int i = la - 1;
	int j = lb - 1;
	int c = 0;
	for (; j >= 0; i--, j--)
	{
		char v = a[i] - b[j] - c;
		if (v < 0)
		{
			v += 10;
			c = 1;
		}
		else c = 0;

		a[i] = v + '0';
	}
	if (c) a[i]--;
}

void divstr(char *a, char *b, char *c)
{
	while (*a == '0') a++;
	while (*b == '0') b++;
	int la = (int)strlen(a);
	int lb = (int)strlen(b);
	if (la > lb)
		la = lb;

	for (; ; la++)
	{
		int r = 0;
		while (divcomp(a, la, b, lb))
		{
			r++;
			divminus(a, la, b, lb);
			while (la && *a == '0')
			{
				a++;
				la--;
			}
		}
		*c++ = r + '0';
		while (*a == '0')
		{
			a++;
			*c++ = '0';
		}
		if (a[la] == 0) break;
	}
	*c = '\0';
	return;
}

CFloat::CFloat()
{
	clear();
}

CFloat::CFloat(const CFloat &f)
{
	assign(f);
}
CFloat::CFloat(int v)
{
	assign((LONGINT)v);
}
CFloat::CFloat(long v)
{
	assign((LONGINT)v);
}
CFloat::CFloat(LONGINT v)
{
	assign(v);
}
CFloat::CFloat(double v)
{
	assign(v);
}
CFloat::CFloat(const char *s)
{
	assign(s);
}

void CFloat::clear()
{
	m_sign = 0;
	m_integer = 0;
	m_fraction = 0;
}

int CFloat::sign() const
{
	return m_sign;
}

CFloat CFloat::power()
{
	CFloat r(_power);
	return r;
}

char *CFloat::toString(char *buf) const
{
	if (buf == NULL)
		return NULL;

	if (m_sign == 0)
	{
		strcpy(buf, "0");
		return buf;
	}

	char ss[2] = { 0 };
	if (m_sign < 0) {
		ss[0] = '-';
		ss[1] = 0;
	}
	if (m_fraction == 0)
	{
#ifdef _WIN32
		snprintf(buf, 32, "%s%I64d", ss, m_integer);
#else
		snprintf(buf, 32, "%s%lld", ss, m_integer);
#endif
	}
	else
	{
#ifdef _WIN32
		snprintf(buf, 36, "%s%I64d.%0*I64d", ss, m_integer, PREC_DEPTH, m_fraction);
#else
		snprintf(buf, 36, "%s%lld.%0*lld", ss, m_integer, PREC_DEPTH, m_fraction);
#endif
		for (int i = (int)strlen(buf) - 1; buf[i] == '0'; buf[i] = 0, i--);
	}
	return buf;
}

CFloat& CFloat::abs()
{
	if (m_sign < 0)
		m_sign = 1;
	return (*this);
}

CFloat& CFloat::trunc()
{
	m_fraction = 0;
	if (m_integer == 0)
		m_sign = 0;
	return (*this);
}

CFloat& CFloat::tail()
{
	m_integer = 0;
	if (m_fraction == 0)
		m_sign = 0;
	return (*this);
}

CFloat& CFloat::operator= (const CFloat &f)
{
	assign(f);
	return (*this);
}
CFloat& CFloat::operator= (int v)
{
	assign((LONGINT)v);
	return (*this);
}
CFloat& CFloat::operator= (long v)
{
	assign((LONGINT)v);
	return (*this);
}
CFloat& CFloat::operator= (LONGINT v)
{
	assign(v);
	return (*this);
}
CFloat& CFloat::operator= (double v)
{
	assign(v);
	return (*this);
}
CFloat& CFloat::operator= (const char *s)
{
	assign(s);
	return (*this);
}

CFloat CFloat::operator+ (const CFloat &f) const
{
	CFloat r(*this);
	r += f;
	return r;
}
void CFloat::operator+= (const CFloat &f)
{
	if (m_sign == 0) {
		assign(f);
		return;
	}
	if (f.m_sign == 0)
		return;

	//符号相同
	if (m_sign == f.m_sign)
	{
		m_fraction += f.m_fraction;
		int c = 0;
		if (m_fraction >= _power)
		{
			m_fraction -= _power;
			c = 1;
		}
		m_integer += f.m_integer + c;
		return;
	}
	//符号不同
	if (m_integer == f.m_integer)
	{
		if (m_fraction == f.m_fraction)
		{
			m_sign = 0;
			m_fraction = 0;
		}
		else if (m_fraction > f.m_fraction)
		{
			m_fraction -= f.m_fraction;
		}
		else
		{
			m_fraction = f.m_fraction - m_fraction;
			m_sign = -m_sign;
		}
		m_integer = 0;
	}
	else if (m_integer > f.m_integer)
	{
		int c = 0;
		m_fraction -= f.m_fraction;
		if (m_fraction < 0)
		{
			m_fraction += _power;
			c = 1;
		}
		m_integer -= f.m_integer + c;
	}
	else
	{
		int c = 0;
		m_fraction = f.m_fraction - m_fraction;
		if (m_fraction < 0)
		{
			m_fraction += _power;
			c = -1;
		}
		m_integer = f.m_integer - m_integer + c;
		m_sign = -m_sign;
	}
}

CFloat CFloat::operator- () const
{
	CFloat r(*this);
	r.m_sign = -r.m_sign;
	return r;
}

CFloat CFloat::operator- (const CFloat &f) const
{
	CFloat r(*this);
	r += (-f);
	return r;
}
void CFloat::operator-= (const CFloat &f)
{
	*this += (-f);
}

CFloat CFloat::operator* (const CFloat &f) const
{
	CFloat r(*this);
	r.multiply(f, false);
	return r;
}
void CFloat::operator*= (const CFloat &f)
{
	multiply(f, false);
}
CFloat& CFloat::Multiply(const CFloat &f)
{
	multiply(f, true);
	return (*this);
}
void CFloat::multiply(const CFloat &f, bool carry)
{
	if (m_sign == 0)
		return;
	if (f.m_sign == 0) {
		clear();
		return;
	}
	bool flag = (m_sign == f.m_sign);

	char a[100], b[100], c[200];
	tostr(a, m_integer, m_fraction);
	tostr(b, f.m_integer, f.m_fraction);
	multistr(a, b, c + 1);
	c[0] = (flag ? '+' : '-');
	char *p = c + strlen(c) - PREC_DEPTH * 2;
	memmove(p + 1, p, PREC_DEPTH * 2 + 1);
	*p = '.';
	assign(c);

	if (!carry) return;

	int k = 0;
	for (p += PREC_DEPTH + 1; *p; p++)
	{
		if (*p > '0') {
			k = 1;
			break;
		}
	}
	if (k > 0)
	{
		m_fraction += k;
		if (m_fraction > _power) {
			m_fraction -= _power;
			m_integer += k;
		}
		m_sign = (flag ? 1 : -1);
	}
}

CFloat CFloat::operator/ (const CFloat &f) const
{
	CFloat r(*this);
	r.divide(f, false);
	return r;
}

CFloat CFloat::operator% (const CFloat &f) const
{
	CFloat r(*this);
	r.divide(f, false);
	return r.tail();
}

void CFloat::operator/= (const CFloat &f)
{
	divide(f, false);
}

CFloat& CFloat::Divide(const CFloat &f)
{
	divide(f, true);
	return (*this);
}

void CFloat::divide(const CFloat &f, bool carry)
{
	if (f.m_sign == 0)
		throw "divided by zero";
	if (m_sign == 0)
		return;
	bool flag = (m_sign == f.m_sign);

	char a[100], b[100], c[100];
	tostr(b, m_integer, m_fraction);
	snprintf(a, 100, "%s%0*d", b, PREC_DEPTH, 0);
	tostr(b, f.m_integer, f.m_fraction);
	char *q = c + PREC_DEPTH + 2;
	divstr(a, b, q);
	int lc = strlen(q);
	for (; lc <= PREC_DEPTH; lc++) {
		*(--q) = '0';
	}
	char *p = q + lc - PREC_DEPTH;
	memmove(p + 1, p, PREC_DEPTH + 1);
	*p = '.';
	*(--q) = (flag ? '+' : '-');
	assign(q);

	if (!carry) return;

	int k = 0;
	for (int i = 0; a[i]; i++)
	{
		if (a[i] > '0') {
			k = 1;
			break;
		}
	}
	if (k > 0)
	{
		m_fraction += k;
		if (m_fraction > _power) {
			m_fraction -= _power;
			m_integer += k;
		}
		m_sign = (flag ? 1 : -1);
	}
}

int CFloat::compare(const CFloat &f) const
{
	int res = 0;
	if (m_sign == f.m_sign)
	{
		if (m_integer == f.m_integer)
		{
			if (m_fraction > f.m_fraction)
			{
				res = m_sign;
			}
			else if (m_fraction < f.m_fraction)
			{
				res = -m_sign;
			}
		}
		else if (m_integer > f.m_integer)
		{
			res = m_sign;
		}
		else
		{
			res = -m_sign;
		}
	}
	else if (m_sign > f.m_sign)
	{
		res = 1;
	}
	else
	{
		res = -1;
	}
	return res;
}

bool CFloat::operator> (const CFloat &f) const
{
	return (compare(f) > 0);
}

bool CFloat::operator>= (const CFloat &f) const
{
	return (compare(f) >= 0);
}

bool CFloat::operator< (const CFloat &f) const
{
	return (compare(f) < 0);
}

bool CFloat::operator<= (const CFloat &f) const
{
	return (compare(f) <= 0);
}

bool CFloat::operator== (const CFloat &f) const
{
	return (compare(f) == 0);
}

bool CFloat::operator!= (const CFloat &f) const
{
	return (compare(f) != 0);
}

void CFloat::assign(const CFloat &f)
{
	m_sign = f.m_sign;
	m_integer = f.m_integer;
	m_fraction = f.m_fraction;
}

void CFloat::assign(LONGINT v)
{
	if (v == 0) {
		clear();
	}
	else if (v > 0) {
		m_sign = 1;
		m_integer = v;
		m_fraction = 0;
	}
	else {
		m_sign = -1;
		m_integer = -v;
		m_fraction = 0;
	}
}

void CFloat::assign(double v)
{
	char tmp[512];
	snprintf(tmp, 512, "%f", v);
	assign(tmp);
}

void CFloat::assign(const char *s)
{
	if (s == NULL) return;

	while (*s == ' ') s++;
	if (*s == '-') {
		m_sign = -1;
		s++;
	}
	else {
		m_sign = 1;
		if (*s == '+') s++;
	}

	m_integer = stoi64(s);

	const char *p = strchr(s, '.');
	if (p) {
		char tmp[100];
		strncpy(tmp, p + 1, PREC_DEPTH);
		tmp[PREC_DEPTH] = '\0';
		for (size_t i = strlen(tmp); i < PREC_DEPTH; i++)
		{
			tmp[i] = '0';
		}
		m_fraction = stoi64(tmp);
	}
	else {
		m_fraction = 0;
	}

	if (m_integer == 0 && m_fraction == 0)
	{
		m_sign = 0;
	}
}
