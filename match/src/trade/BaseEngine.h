#ifndef BASEENGINE_H
#define BASEENGINE_H

#include "errorDefine.h"
#include "mdb.h"
#include "mdbLog.h"
#include "monitorIndex.h"

extern const char *INI_FILE_NAME;

class CBaseEngine
{
public:
	CBaseEngine();
	virtual ~CBaseEngine(void);
protected:
	CMDB *m_pMDB;
	CMDBLog *m_pMDBLog;
	CErrorEngine *m_pErrorEngine;
	CTransaction m_Transaction;
};

#endif
