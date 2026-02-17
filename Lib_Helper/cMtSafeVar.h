#pragma once

//#include <Windows.h>

struct cMtSafeVar
{
private:
    char buffer[8];
    UINT64 key;

    void GenerateNewKey();

public:
	__int64 GetInt64(bool encrypted = false);
	double GetDouble(bool encrypted = false);
	UINT64 GetUINT64(bool encrypted = false);
	UINT32 GetUINT32(bool encrypted = false);
	UINT16 GetUINT16(bool encrypted = false);
	UINT8 GetUINT8(bool encrypted = false);
	float GetFloat(bool encrypted = false);
	short GetShort(bool encrypted = false);
	char GetChar(bool encrypted = false);
	int GetInt(bool encrypted = false);
	__int64 Set(__int64 value);
	double Set(double value);
	UINT64 Set(UINT64 value);
	UINT32 Set(UINT32 value);
	UINT16 Set(UINT16 value);
	UINT8 Set(UINT8 value);
	float Set(float value);
	short Set(short value);
	char Set(char value);
	int Set(int value);
	cMtSafeVar(__int64 value);
	cMtSafeVar(double value);
	cMtSafeVar(UINT64 value);
	cMtSafeVar(UINT32 value);
	cMtSafeVar(UINT16 value);
	cMtSafeVar(UINT8 value);
	cMtSafeVar(float value);
	cMtSafeVar(short value);
	cMtSafeVar(char value);
	cMtSafeVar(int value);
	cMtSafeVar();
};