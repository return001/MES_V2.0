#pragma once

class CSemaphoreClass
{
public:
	CSemaphoreClass();
	~CSemaphoreClass();

	void Lock();
	void Unlock();

private:
	CRITICAL_SECTION m_hSem;
};

class CAutoLock
{
public:
	CAutoLock(CSemaphoreClass *pCSem);
	~CAutoLock();

private:
	CSemaphoreClass *m_pAutoSem;
};

