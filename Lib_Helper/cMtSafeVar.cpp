#include "stdafx.h"
#include "./cMtSafeVar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

__int64 cMtSafeVar::GetInt64(bool encrypted) { return (*(__int64*)&this->buffer[0] ^ (encrypted ? 0 : (__int64)this->key)); }
UINT64 cMtSafeVar::GetUINT64(bool encrypted) { return (*(UINT64*)&this->buffer[0] ^ (encrypted ? 0 : (UINT64)this->key)); }
UINT32 cMtSafeVar::GetUINT32(bool encrypted) { return (*(UINT32*)&this->buffer[0] ^ (encrypted ? 0 : (UINT32)this->key)); }
UINT16 cMtSafeVar::GetUINT16(bool encrypted) { return (*(UINT16*)&this->buffer[0] ^ (encrypted ? 0 : (UINT16)this->key)); }
UINT8 cMtSafeVar::GetUINT8(bool encrypted) { return (*(UINT8*)&this->buffer[0] ^ (encrypted ? 0 : (UINT8)this->key)); }
short cMtSafeVar::GetShort(bool encrypted) { return (*(short*)&this->buffer[0] ^ (encrypted ? 0 : (short)this->key)); }
char cMtSafeVar::GetChar(bool encrypted) { return (*(char*)&this->buffer[0] ^ (encrypted ? 0 : (char)this->key)); }
int cMtSafeVar::GetInt(bool encrypted) { return (*(int*)&this->buffer[0] ^ (encrypted ? 0 : (int)this->key)); }

cMtSafeVar::cMtSafeVar(__int64 value) { this->Set(value); }
cMtSafeVar::cMtSafeVar(double value) { this->Set(value); }
cMtSafeVar::cMtSafeVar(UINT64 value) { this->Set(value); }
cMtSafeVar::cMtSafeVar(UINT32 value) { this->Set(value); }
cMtSafeVar::cMtSafeVar(UINT16 value) { this->Set(value); }
cMtSafeVar::cMtSafeVar(UINT8 value) { this->Set(value); }
cMtSafeVar::cMtSafeVar(float value) { this->Set(value); }
cMtSafeVar::cMtSafeVar(short value) { this->Set(value); }
cMtSafeVar::cMtSafeVar(char value) { this->Set(value); }
cMtSafeVar::cMtSafeVar(int value) { this->Set(value); }
cMtSafeVar::cMtSafeVar() { this->Set((UINT64)0); };

double cMtSafeVar::GetDouble(bool encrypted)
{
  double value;

  *(UINT64*)&value = (*(UINT64*)&this->buffer[0] ^ (encrypted ? 0 : (UINT64)this->key));

  return value;
}

float cMtSafeVar::GetFloat(bool encrypted)
{
  float value;

  *(UINT*)&value = (*(UINT*)&this->buffer[0] ^ (encrypted ? 0 : (UINT)this->key));

  return value;
}

__int64 cMtSafeVar::Set(__int64 value)
{
  this->GenerateNewKey();

  *(__int64*)&this->buffer[0] = (*(__int64*)&value ^ (__int64)this->key);

  return this->GetInt64();
}

UINT64 cMtSafeVar::Set(UINT64 value)
{
  this->GenerateNewKey();

  *(UINT64*)&this->buffer[0] = (*(UINT64*)&value ^ (UINT64)this->key);

  return this->GetUINT64();
}

UINT32 cMtSafeVar::Set(UINT32 value)
{
  this->GenerateNewKey();

  *(UINT32*)&this->buffer[0] = (*(UINT32*)&value ^ (UINT32)this->key);

  return this->GetUINT32();
}

UINT16 cMtSafeVar::Set(UINT16 value)
{
  this->GenerateNewKey();

  *(UINT16*)&this->buffer[0] = (*(UINT16*)&value ^ (UINT16)this->key);

  return this->GetUINT16();
}

UINT8 cMtSafeVar::Set(UINT8 value)
{
  this->GenerateNewKey();

  *(UINT8*)&this->buffer[0] = (*(UINT8*)&value ^ (UINT8)this->key);

  return this->GetUINT8();
}

double cMtSafeVar::Set(double value)
{
  this->GenerateNewKey();

  *(UINT64*)&this->buffer[0] = (*(UINT64*)&value ^ (UINT64)this->key);

  return this->GetDouble();
}

float cMtSafeVar::Set(float value)
{
  this->GenerateNewKey();

  *(UINT*)&this->buffer[0] = (*(UINT*)&value ^ (UINT)this->key);

  return this->GetFloat();
}

short cMtSafeVar::Set(short value)
{
  this->GenerateNewKey();

  *(short*)&this->buffer[0] = (*(short*)&value ^ (short)this->key);

  return this->GetShort();
}

char cMtSafeVar::Set(char value)
{
  this->GenerateNewKey();

  *(char*)&this->buffer[0] = (*(char*)&value ^ (char)this->key);

  return this->GetChar();
}

int cMtSafeVar::Set(int value)
{
  this->GenerateNewKey();

  *(int*)&this->buffer[0] = (*(int*)&value ^ (int)this->key);

  return this->GetInt();
}

void cMtSafeVar::GenerateNewKey()
{
  DWORD tick = GetTickCount();

  *(UINT*)&this->key = tick;
  *(UINT*)((UINT_PTR)&this->key + 4) = tick;
}