#ifndef _CSVPARSER_H__
#define _CSVPARSER_H__

#if _MSC_VER > 1000
#pragma once
#endif

const int TOKEN_MAX_LEN = 512;

enum CSV_PARSER_ERROR
{
	CPE_HAS_NEXT,
	CPE_END,
	CPE_MARK_NOT_MATCH,
	CPE_TOKEN_TOO_LONG,
};

class CCSVParser  
{
public:
	CCSVParser();
	CCSVParser(const char *pszData);
	virtual ~CCSVParser();
	char *GetNextToken();
	void Parse(const char *pszData);
	inline CSV_PARSER_ERROR GetErrorCode();
private:
	void NextChar();
	void MakeWord(const char *pszEnd);
private:
	CSV_PARSER_ERROR m_nErrorCode;
	const char *m_pszData;
	char m_szCurrWord[TOKEN_MAX_LEN+1];
	const char *m_pCurr;
	char m_chC;
	char m_chNC;
};

inline CSV_PARSER_ERROR CCSVParser::GetErrorCode()
{
	return m_nErrorCode;
}

#endif
