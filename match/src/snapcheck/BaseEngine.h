#ifndef BASEENGINE_H
#define BASEENGINE_H

#include "errorDefine.h"
#include "mdb.h"
#include "mdbLog.h"
#include "monitorIndex.h"
#include "Config.h"

class CBaseEngine
{
public:
	CBaseEngine();
	virtual ~CBaseEngine(void);

protected:
	CMDB *m_pMDB;
	CMDBLog *m_pMDBLog;
	CErrorEngine *m_pErrorEngine;
};

#endif
