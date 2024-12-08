#pragma once

class ObjectPoolable
{
	virtual bool IsNeedToReturn() = 0;
	virtual void InitForObjectPool() = 0;
	virtual void ResetForObjectPool() = 0;
};