/*
   LZ4 - Fast LZ compression algorithm
   Streamlined implementation for RAN Community
   Optimized for game packet compression (64-2048 bytes)
   
   Copyright (C) 2011-2020, Yann Collet.
   BSD 2-Clause License
*/

#include "lz4.h"
#include <string.h>
#include <stdlib.h>

/*-************************************
*  Constants
**************************************/
#define LZ4_ACCELERATION_DEFAULT 1
#define MINMATCH 4
#define MFLIMIT 12
#define LASTLITERALS 5
#define WILDCOPYLENGTH 8
#define ML_BITS  4
#define ML_MASK  ((1U<<ML_BITS)-1)
#define RUN_BITS (8-ML_BITS)
#define RUN_MASK ((1U<<RUN_BITS)-1)

/*-************************************
*  Types
**************************************/
typedef unsigned char BYTE;
typedef unsigned short U16;
typedef unsigned int U32;

/*-************************************
*  Memory operations
**************************************/
static U32 LZ4_read32(const void* ptr) { 
    return *(const U32*)ptr; 
}

static void LZ4_write32(void* ptr, U32 value) { 
    *(U32*)ptr = value; 
}

static U16 LZ4_read16(const void* ptr) { 
    return *(const U16*)ptr; 
}

static void LZ4_write16(void* ptr, U16 value) { 
    *(U16*)ptr = value; 
}

/*-************************************
*  Hash function for small data
**************************************/
static U32 LZ4_hash4(U32 sequence) {
    return ((sequence * 2654435761U) >> (32-12));
}

/*-************************************
*  Simple memory copy functions
**************************************/
static void LZ4_copy8(BYTE* dst, const BYTE* src) {
    memcpy(dst, src, 8);
}

static void LZ4_wildCopy(BYTE* dst, const BYTE* src, BYTE* dstEnd) {
    do {
        LZ4_copy8(dst, src);
        dst += 8;
        src += 8;
    } while (dst < dstEnd);
}

/*-************************************
*  Compression bound
**************************************/
int LZ4_compressBound(int inputSize) {
    return (inputSize > LZ4_MAX_INPUT_SIZE) ? 0 : (inputSize + ((inputSize)/255) + 16);
}

/*-************************************
*  Simple compression - optimized for game packets
**************************************/
int LZ4_compress_default(const char* src, char* dst, int srcSize, int dstCapacity) {
    return LZ4_compress_fast(src, dst, srcSize, dstCapacity, LZ4_ACCELERATION_DEFAULT);
}

int LZ4_compress_fast(const char* src, char* dst, int srcSize, int dstCapacity, int acceleration) {
    const BYTE* ip = (const BYTE*)src;
    const BYTE* anchor = ip;
    const BYTE* const iend = ip + srcSize;
    const BYTE* const mflimit = iend - MFLIMIT;
    
    BYTE* op = (BYTE*)dst;
    BYTE* const oend = op + dstCapacity;
    
    U32 hashTable[4096] = {0}; // Simple hash table for small packets
    
    // Input validation
    if (srcSize < 0) return 0;
    if (dstCapacity < 0) return 0;
    if (srcSize == 0) {
        if (dstCapacity >= 1) {
            *op++ = 0; // Empty block marker
            return 1;
        }
        return 0;
    }
    if (dstCapacity < LZ4_compressBound(srcSize)) {
        if (dstCapacity < 1) return 0;
    }
    
    // Too small to compress effectively
    if (srcSize < 13) goto _last_literals;
    
    // First byte
    ip++;
    
    // Main compression loop - simplified for game packets
    while (ip < mflimit) {
        const BYTE* match;
        BYTE* token;
        
                 // Find match using simple hash
         U32 h = LZ4_hash4(LZ4_read32(ip));
         U32 matchIndex = hashTable[h & 4095];
         match = (const BYTE*)src + matchIndex;
         hashTable[h & 4095] = (U32)(ip - (const BYTE*)src);
         
         // Check if match is valid and within distance
         if ((matchIndex != 0) && 
             (ip - match < 65536) && 
             (match >= (const BYTE*)src) &&
             (ip + 4 <= iend) && (match + 4 <= iend) &&
             (LZ4_read32(match) == LZ4_read32(ip))) {
            
            // Found a match - encode literals first
            U32 litLength = (U32)(ip - anchor);
            token = op++;
            
            if (op + litLength + (2 + 1 + LASTLITERALS) + (litLength >> 8) > oend) {
                return 0; // Not enough space
            }
            
            if (litLength >= RUN_MASK) {
                int len = (int)litLength - RUN_MASK;
                *token = (RUN_MASK << ML_BITS);
                for (; len >= 255; len -= 255) *op++ = 255;
                *op++ = (BYTE)len;
            } else {
                *token = (BYTE)(litLength << ML_BITS);
            }
            
            // Copy literals
            if (litLength > 0) {
                memcpy(op, anchor, litLength);
                op += litLength;
            }
            
            // Encode match
            {
                U32 offset = (U32)(ip - match);
                U32 matchLength = MINMATCH;
                
                // Find match length
                while ((ip + matchLength < iend - LASTLITERALS) && 
                       (match[matchLength] == ip[matchLength])) {
                    matchLength++;
                }
                
                // Write offset
                LZ4_write16(op, (U16)offset);
                op += 2;
                
                // Write match length
                matchLength -= MINMATCH;
                if (matchLength >= ML_MASK) {
                    *token += ML_MASK;
                    matchLength -= ML_MASK;
                    for (; matchLength >= 255; matchLength -= 255) *op++ = 255;
                    *op++ = (BYTE)matchLength;
                } else {
                    *token += (BYTE)matchLength;
                }
                
                // Move forward
                ip += matchLength + MINMATCH;
                anchor = ip;
            }
        } else {
            // No match found, move forward
            ip++;
        }
    }
    
_last_literals:
    // Encode remaining literals
    {
        U32 lastRun = (U32)(iend - anchor);
        if (op + lastRun + 1 + (lastRun >> 8) > oend) {
            return 0; // Not enough space
        }
        
        if (lastRun >= RUN_MASK) {
            *op++ = (RUN_MASK << ML_BITS);
            lastRun -= RUN_MASK;
            for (; lastRun >= 255; lastRun -= 255) *op++ = 255;
            *op++ = (BYTE)lastRun;
        } else {
            *op++ = (BYTE)(lastRun << ML_BITS);
        }
        
        if (anchor < iend) {
            memcpy(op, anchor, iend - anchor);
            op += iend - anchor;
        }
    }
    
    return (int)(op - (BYTE*)dst);
}

/*-************************************
*  Safe decompression
**************************************/
int LZ4_decompress_safe(const char* src, char* dst, int compressedSize, int dstCapacity) {
    const BYTE* ip = (const BYTE*)src;
    const BYTE* const iend = ip + compressedSize;
    
    BYTE* op = (BYTE*)dst;
    BYTE* const oend = op + dstCapacity;
    
    // Input validation
    if (compressedSize < 0) return -1;
    if (dstCapacity <= 0) return -1;
    if (compressedSize == 0) return 0;
    
    // Handle empty block
    if (compressedSize == 1 && *ip == 0) {
        return 0;
    }
    
    // Main decompression loop
    while (ip < iend) {
        BYTE token = *ip++;
        U32 literalLength = token >> ML_BITS;
        
        // Decode literal length
        if (literalLength == RUN_MASK) {
            BYTE s;
            do {
                if (ip >= iend) return -1;
                s = *ip++;
                literalLength += s;
            } while (s == 255);
        }
        
        // Copy literals
        if (literalLength > 0) {
            if (op + literalLength > oend) return -1;
            if (ip + literalLength > iend) return -1;
            
            memcpy(op, ip, literalLength);
            op += literalLength;
            ip += literalLength;
        }
        
        // Check if we're done
        if (ip >= iend) break;
        
        // Decode offset
        if (ip + 2 > iend) return -1;
        U32 offset = LZ4_read16(ip);
        ip += 2;
        
        if (offset == 0) return -1;
        
        // Decode match length
        U32 matchLength = (token & ML_MASK) + MINMATCH;
        if ((token & ML_MASK) == ML_MASK) {
            BYTE s;
            do {
                if (ip >= iend) return -1;
                s = *ip++;
                matchLength += s;
            } while (s == 255);
        }
        
        // Copy match
        {
            const BYTE* match = op - offset;
            if (match < (const BYTE*)dst) return -1;
            if (op + matchLength > oend) return -1;
            
            // Handle overlapping copy
            if (offset < matchLength) {
                // Overlapping - copy byte by byte
                for (U32 i = 0; i < matchLength; i++) {
                    op[i] = match[i];
                }
                op += matchLength;
            } else {
                // Non-overlapping - fast copy
                memcpy(op, match, matchLength);
                op += matchLength;
            }
        }
    }
    
    return (int)(op - (BYTE*)dst);
}

/*-************************************
*  Additional utility functions
**************************************/
int LZ4_sizeofState(void) {
    return 16384; // Simple state size for hash table
}

int LZ4_compress_fast_extState(void* state, const char* src, char* dst, int srcSize, int dstCapacity, int acceleration) {
    // For simplicity, ignore external state and use internal hash table
    return LZ4_compress_fast(src, dst, srcSize, dstCapacity, acceleration);
}

int LZ4_decompress_safe_partial(const char* src, char* dst, int srcSize, int targetOutputSize, int dstCapacity) {
    // Clamp target size to capacity
    int maxOutput = (targetOutputSize < dstCapacity) ? targetOutputSize : dstCapacity;
    return LZ4_decompress_safe(src, dst, srcSize, maxOutput);
} 