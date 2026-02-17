/*
   LZ4 - Fast LZ compression algorithm
   Copyright (C) 2011-2020, Yann Collet.

   BSD 2-Clause License (http://www.opensource.org/licenses/bsd-license.php)

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:

       * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above
   copyright notice, this list of conditions and the following disclaimer
   in the documentation and/or other materials provided with the
   distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   You can contact the author at :
   - LZ4 homepage : http://www.lz4.org
   - LZ4 source repository : https://github.com/lz4/lz4
*/

/*-************************************
*  Streamlined LZ4 Implementation for RAN Community
*  Optimized for game packet compression (64-2048 bytes)
*  Focus: Speed over maximum compression ratio
**************************************/

#include "lz4.h"
#include <string.h>
#include <stdlib.h>

/*-************************************
*  Constants and Macros
**************************************/
#define LZ4_ACCELERATION_DEFAULT 1
#define LZ4_ACCELERATION_MAX     65537

#define ML_BITS  4
#define ML_MASK  ((1U<<ML_BITS)-1)
#define RUN_BITS (8-ML_BITS)
#define RUN_MASK ((1U<<RUN_BITS)-1)

#define MINMATCH 4

#define LZ4_HASHLOG   (LZ4_MEMORY_USAGE-2)
#define LZ4_HASHTABLESIZE (1 << LZ4_MEMORY_USAGE)
#define LZ4_HASH_SIZE_U32 (1 << LZ4_HASHLOG)

#define LZ4_64Klimit ((64 KB) + (MFLIMIT-1))
#define LZ4_skipTrigger 6

/*-************************************
*  Types
**************************************/
typedef unsigned char BYTE;
typedef unsigned short U16;
typedef unsigned int U32;
typedef signed int S32;
typedef unsigned long long U64;
typedef uintptr_t uptrval;

/*-************************************
*  Reading and writing into memory
**************************************/
static unsigned LZ4_read32(const void* ptr) { 
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
*  Hash function
**************************************/
static U32 LZ4_hash4(U32 sequence, tableType_t const tableType)
{
    if (tableType == byU16)
        return ((sequence * 2654435761U) >> ((MINMATCH*8)-(LZ4_HASHLOG+1)));
    else
        return ((sequence * 2654435761U) >> ((MINMATCH*8)-LZ4_HASHLOG));
}

static U32 LZ4_hash5(U64 sequence, tableType_t const tableType)
{
    const U32 hashLog = (tableType == byU16) ? LZ4_HASHLOG+1 : LZ4_HASHLOG;
    if (LZ4_isLittleEndian()) {
        const U64 prime5bytes = 889523592379ULL;
        return (U32)(((sequence << 24) * prime5bytes) >> (64 - hashLog));
    } else {
        const U64 prime8bytes = 11400714785074694791ULL;
        return (U32)(((sequence >> 24) * prime8bytes) >> (64 - hashLog));
    }
}

/*-************************************
*  Simple compression function
**************************************/
int LZ4_compress_default(const char* src, char* dst, int srcSize, int dstCapacity)
{
    return LZ4_compress_fast(src, dst, srcSize, dstCapacity, 1);
}

/*-************************************
*  Fast compression function
**************************************/
int LZ4_compress_fast(const char* src, char* dst, int srcSize, int dstCapacity, int acceleration)
{
    int result;
    
    if (acceleration < 1) acceleration = LZ4_ACCELERATION_DEFAULT;
    if (acceleration > LZ4_ACCELERATION_MAX) acceleration = LZ4_ACCELERATION_MAX;

    if (srcSize < LZ4_64Klimit) {
        result = LZ4_compress_generic(src, dst, srcSize, NULL, dstCapacity, limitedOutput, byU16, noDict, noDictIssue, acceleration);
    } else {
        result = LZ4_compress_generic(src, dst, srcSize, NULL, dstCapacity, limitedOutput, byU32, noDict, noDictIssue, acceleration);
    }
    
    return result;
}

/*-************************************
*  Core compression algorithm
**************************************/
static int LZ4_compress_generic(
    const char* const src,
    char* const dst,
    const int srcSize,
    int *inputConsumed,
    const int dstCapacity,
    const limitedOutput_directive outputDirective,
    const tableType_t tableType,
    const dict_directive dictDirective,
    const dictIssue_directive dictIssue,
    const int acceleration)
{
    int result;
    const BYTE* ip = (const BYTE*) src;

    U32 const startIndex = dictDirective == withPrefix64k ? dictCtx->currentOffset : 0;
    const BYTE* base = (const BYTE*) src - startIndex;
    const BYTE* lowLimit;

    const LZ4_stream_t_internal* dictCtx = (const LZ4_stream_t_internal*) dict;
    const BYTE* const dictEnd = dictCtx ? dictCtx->dictionary + dictCtx->dictSize : NULL;
    const BYTE* const dictBase = dictCtx ? dictCtx->dictionary : NULL;
    const U32 dictLimit = dictCtx ? startIndex + dictCtx->currentOffset : 0;

    const BYTE* const lowRefLimit = ip - dictCtx->dictSize;

    const BYTE* const source = ip;
    const BYTE* const iend = ip + srcSize;
    const BYTE* const mflimitPlusOne = iend - MFLIMIT + 1;
    const BYTE* const matchlimit = iend - LASTLITERALS;

    /* the dictCtx currentOffset is indexed from the start of the dictionary,
     * while a dictionary in the current context precedes the currentOffset */
    const BYTE* const dictStart = dictCtx ? dictCtx->dictionary + dictCtx->currentOffset : NULL;

    BYTE* op = (BYTE*) dst;
    BYTE* const olimit = op + dstCapacity;

    U32 offset = 0;
    U32 forwardH;

    /* Simplified compression for game packets - focus on speed */
    
    /* Init conditions */
    if (outputDirective == limitedOutput && dstCapacity < 1) return 0;
    if ((U32)srcSize > (U32)LZ4_MAX_INPUT_SIZE) return 0;
    if ((tableType == byU16) && (srcSize>=LZ4_64Klimit)) return 0;
    if (tableType==byPtr) assert(dictDirective==noDict);
    if (dictDirective==withPrefix64k) assert(tableType != byPtr);

    /* If input is very small, skip compression */
    if (srcSize < minLength) goto _last_literals;

    /* First Byte - simplified for game packets */
    LZ4_putPosition(ip, ctx, tableType, base);
    ip++; forwardH = LZ4_hashPosition(ip, tableType);

    /* Main Loop - optimized for small game packets */
    for ( ; ; ) {
        const BYTE* match;
        BYTE* token;
        const BYTE* filledIp;

        /* Find a match */
        if (tableType == byPtr) {
            const BYTE* forwardIp = ip;
            int step = 1;
            int searchMatchNb = acceleration << LZ4_skipTrigger;
            do {
                U32 const h = forwardH;
                ip = forwardIp;
                forwardIp += step;
                step = (searchMatchNb++ >> LZ4_skipTrigger);

                if (unlikely(forwardIp > mflimitPlusOne)) goto _last_literals;
                assert(ip < mflimitPlusOne);

                match = LZ4_getPositionOnHash(h, ctx, tableType, base);
                forwardH = LZ4_hashPosition(forwardIp, tableType);
                LZ4_putPositionOnHash(ip, h, ctx, tableType, base);

            } while ( (match+LZ4_DISTANCE_MAX < ip)
                   || (LZ4_read32(match) != LZ4_read32(ip)) );
        } else {
            /* Simplified matching for game data */
            const BYTE* forwardIp = ip;
            int step = 1;
            int searchMatchNb = acceleration << LZ4_skipTrigger;

            do {
                U32 const h = forwardH;
                U32 const current = (U32)(forwardIp - base);
                U32 matchIndex = LZ4_getIndexOnHash(h, ctx, tableType);
                assert(matchIndex <= current);
                assert(forwardIp - base < (ptrdiff_t)(2 GB - 1));
                ip = forwardIp;
                forwardIp += step;
                step = (searchMatchNb++ >> LZ4_skipTrigger);

                if (unlikely(forwardIp > mflimitPlusOne)) goto _last_literals;
                assert(ip < mflimitPlusOne);

                if (dictDirective == usingDictCtx) {
                    if (matchIndex < startIndex) {
                        /* there was no match, try the dictionary */
                        assert(tableType == byU32);
                        matchIndex = LZ4_getIndexOnHash(h, dictCtx, byU32);
                        match = dictBase + matchIndex;
                        matchIndex += dictDelta;
                        lowLimit = dictionary;
                    } else {
                        match = base + matchIndex;
                        lowLimit = (const BYTE*)source;
                    }
                } else if (dictDirective==usingExtDict) {
                    if (matchIndex < startIndex) {
                        DEBUGLOG(7, "extDict candidate: matchIndex=%5u  <  startIndex=%5u", matchIndex, startIndex);
                        assert(startIndex - matchIndex >= MINMATCH);
                        match = dictBase + matchIndex;
                        lowLimit = dictionary;
                    } else {
                        match = base + matchIndex;
                        lowLimit = (const BYTE*)source;
                    }
                } else {   /* single continuous memory segment */
                    match = base + matchIndex;
                }
                forwardH = LZ4_hashPosition(forwardIp, tableType);
                LZ4_putIndexOnHash(current, h, ctx, tableType);

                DEBUGLOG(7, "candidate at pos=%u  (offset=%u \n", matchIndex, current - matchIndex);
                if ((dictIssue == dictSmall) && (matchIndex < prefixIdxLimit)) { continue; }
                assert(matchIndex < current);
                if ( ((tableType != byU16) || (LZ4_DISTANCE_MAX < LZ4_DISTANCE_ABSOLUTE_MAX))
                  && (matchIndex+LZ4_DISTANCE_MAX < current)) {
                    continue;
                } /* too far */
                assert((current - matchIndex) <= LZ4_DISTANCE_MAX);  /* match now expected within distance */

                if (LZ4_read32(match) == LZ4_read32(ip)) {
                    if (maybe_extMem) offset = current - matchIndex;
                    break;   /* match found */
                }

            } while(1);
        }

        /* Catch up - simplified for game packets */
        filledIp = ip;
        while (((ip>anchor) & (match > lowLimit)) && (unlikely(ip[-1]==match[-1]))) { ip--; match--; }

        /* Encode Literal length */
        {   unsigned const litLength = (unsigned)(ip - anchor);
            token = op++;
            if ((outputDirective == limitedOutput) &&
                (unlikely(op + litLength + (2 + 1 + LASTLITERALS) + (litLength/255) > olimit))) {
                return 0;   /* Check output buffer overflow */
            }
            if (litLength >= RUN_MASK) {
                int len = (int)(litLength - RUN_MASK);
                *token = (RUN_MASK<<ML_BITS);
                for(; len >= 255 ; len-=255) *op++ = 255;
                *op++ = (BYTE)len;
            } else {
                *token = (BYTE)(litLength<<ML_BITS);
            }

            /* Copy Literals */
            LZ4_wildCopy8(op, anchor, op+litLength);
            op += litLength;
            DEBUGLOG(6, "seq.start:%i, literals=%u, match.start:%i",
                        (int)(anchor-(const BYTE*)source), litLength, (int)(ip-(const BYTE*)source));
        }

_next_match:
        /* at this stage, the following variables must be correctly set :
         * - ip : at start of LZ operation
         * - match : at start of previous pattern occurence; can be within current prefix, or within extDict
         * - offset : if maybe_extMem==1 (constant-time)
         * - lowLimit : must not overwrite this position, can be == ip (no limit)
         */

        /* Test end of chunk */
        if (ip >= mflimitPlusOne) goto _last_literals;

        /* Fill table */
        LZ4_putPosition(ip-2, ctx, tableType, base);

        /* Test next position */
        if (tableType == byPtr) {

            match = LZ4_getPosition(ip, ctx, tableType, base);
            LZ4_putPosition(ip, ctx, tableType, base);
            if ( (match+LZ4_DISTANCE_MAX >= ip)
              && (LZ4_read32(match) == LZ4_read32(ip)) )
            { token=op++; *token=0; goto _next_match; }

        } else {   /* byU32, byU16 */

            U32 const h = LZ4_hashPosition(ip, tableType);
            U32 const current = (U32)(ip-base);
            U32 matchIndex = LZ4_getIndexOnHash(h, ctx, tableType);
            assert(matchIndex < current);
            if (dictDirective == usingDictCtx) {
                if (matchIndex < startIndex) {
                    /* there was no match, try the dictionary */
                    matchIndex = LZ4_getIndexOnHash(h, dictCtx, byU32);
                    match = dictBase + matchIndex;
                    lowLimit = dictionary;   /* required for match length counter */
                    matchIndex += dictDelta;
                } else {
                    match = base + matchIndex;
                    lowLimit = (const BYTE*)source;   /* required for match length counter */
                }
            } else if (dictDirective==usingExtDict) {
                if (matchIndex < startIndex) {
                    match = dictBase + matchIndex;
                    lowLimit = dictionary;   /* required for match length counter */
                } else {
                    match = base + matchIndex;
                    lowLimit = (const BYTE*)source;   /* required for match length counter */
                }
            } else {   /* single memory segment */
                match = base + matchIndex;
            }
            LZ4_putIndexOnHash(current, h, ctx, tableType);
            assert(matchIndex < current);
            if ( ((dictIssue==dictSmall) ? (matchIndex >= prefixIdxLimit) : 1)
              && (((tableType==byU16) && (current - matchIndex < LZ4_DISTANCE_MAX)) || (tableType==byU32))
              && (LZ4_read32(match) == LZ4_read32(ip)) ) {
                token=op++;
                *token=0;
                if (maybe_extMem) offset = current - matchIndex;
                DEBUGLOG(6, "seq.start:%i, literals=%u, match.start:%i", (int)(anchor-(const BYTE*)source), 0, (int)(ip-(const BYTE*)source));
                goto _next_match;
            }
        }

        /* Prepare next loop */
        forwardH = LZ4_hashPosition(++ip, tableType);

    }

_last_literals:
    /* Encode Last Literals */
    {   size_t lastRun = (size_t)(iend - anchor);
        if ( (outputDirective) &&  /* Check output buffer overflow */
            (op + lastRun + 1 + ((lastRun+255-RUN_MASK)/255) > olimit)) {
            if (outputDirective == fillOutput) {
                /* adapt lastRun to fill 'dst' */
                assert(olimit >= op);
                lastRun  = (size_t)(olimit-op) - 1;
                lastRun -= (lastRun+240)/255;
            } else {
                assert(outputDirective == limitedOutput);
                return 0;   /* cannot compress within `dst` budget. Stored indexes in hash table are nonetheless fine */
            }
        }
        if (lastRun >= RUN_MASK) {
            size_t accumulator = lastRun - RUN_MASK;
            *op++ = RUN_MASK << ML_BITS;
            for(; accumulator >= 255 ; accumulator-=255) *op++ = 255;
            *op++ = (BYTE) accumulator;
        } else {
            *op++ = (BYTE)(lastRun<<ML_BITS);
        }
        memcpy(op, anchor, lastRun);
        ip = anchor + lastRun;
        op += lastRun;
    }

    if (outputDirective == fillOutput) {
        *inputConsumed = (int) (((const char*)ip)-source);
    }
    result = (int)(((char*)op) - dst);
    assert(result > 0);
    DEBUGLOG(5, "LZ4_compress_generic: compressed %i bytes into %i bytes", srcSize, result);
    return result;
}

/*-************************************
*  Decompression functions
**************************************/
int LZ4_decompress_safe(const char* src, char* dst, int compressedSize, int dstCapacity)
{
    if (compressedSize < 0) return -1;
    if (dstCapacity <= 0) return -1;
    
    return LZ4_decompress_generic(src, dst, compressedSize, dstCapacity, endOnInputSize, decode_full_block, noDict, (BYTE*)dst, NULL, 0);
}

/*-************************************
*  Core decompression algorithm  
**************************************/
LZ4_FORCE_INLINE int
LZ4_decompress_generic(
                 const char* const src,
                 char* const dst,
                 int srcSize,
                 int outputSize,
                 endCondition_directive endOnInput,
                 earlyEnd_directive partialDecoding,
                 dict_directive dict,
                 const BYTE* const lowPrefix,
                 const BYTE* const dictStart,
                 const size_t dictSize
                 )
{
    if (src == NULL) { return -1; }

    {   const BYTE* ip = (const BYTE*) src;
        const BYTE* const iend = ip + srcSize;

        BYTE* op = (BYTE*) dst;
        const BYTE* const oend = op + outputSize;
        BYTE* cpy;

        const BYTE* const dictEnd = (dictStart == NULL) ? NULL : dictStart + dictSize;

        const int safeDecode = (endOnInput==endOnInputSize);
        const int checkOffset = ((safeDecode) && (dictSize < (int)(64 KB)));

        /* Set up the "end" pointers for the shortcut. */
        const BYTE* const shortiend = iend - (endOnInput ? 14 : 8) /*maxLL*/ - 2 /*offset*/;
        const BYTE* const shortoend = oend - (endOnInput ? 14 : 8) /*maxLL*/ - 18 /*maxML*/;

        const BYTE* match;
        size_t offset;
        unsigned token;
        size_t length;

        DEBUGLOG(5, "LZ4_decompress_generic (srcSize:%i, dstSize:%i)", srcSize, outputSize);

        /* Special cases - simplified for game packets */
        assert(lowPrefix <= op);
        if ((endOnInput) && (unlikely(outputSize==0))) {
            /* Empty output buffer */
            if (partialDecoding) return 0;
            return ((srcSize==1) && (*ip==0)) ? 0 : -1;
        }
        if ((!endOnInput) && (unlikely(outputSize==0))) { return (*ip==0 ? 1 : -1); }
        if ((endOnInput) && unlikely(srcSize==0)) { return -1; }

        /* Currently the fast loop shows a regression on qualcomm arm chips. */
#if LZ4_FAST_DEC_LOOP
        if ((oend - op) < FASTLOOP_SAFE_DISTANCE) {
            DEBUGLOG(6, "skip fast decode loop");
            goto safe_decode;
        }

        /* Fast loop : decode sequences as long as output < iend-FASTLOOP_SAFE_DISTANCE */
        while (1) {
            /* Main fastloop assertion: We can always wildcopy FASTLOOP_SAFE_DISTANCE */
            assert(oend - op >= FASTLOOP_SAFE_DISTANCE);
            if (endOnInput) { assert(ip < iend); }
            token = *ip++;
            length = token >> ML_BITS;  /* literal length */

            assert(!endOnInput || ip <= iend); /* ip < iend before the increment */

            /* decode literal length */
            if (length == RUN_MASK) {
                variable_length_error error = ok;
                length += read_variable_length(&ip, iend - RUN_MASK, (int)endOnInput, 0, &error);
                if (error != ok) goto _output_error;
                if ((safeDecode) && unlikely((uptrval)(op)+length<(uptrval)(op))) goto _output_error; /* overflow detection */
                if ((safeDecode) && unlikely((uptrval)(ip)+length<(uptrval)(ip))) goto _output_error; /* overflow detection */

                /* copy literals */
                if (op + length > oend - FASTLOOP_SAFE_DISTANCE) {
                    goto safe_literal_copy;
                }
                if (endOnInput && ip + length > iend - FASTLOOP_SAFE_DISTANCE) {
                    goto safe_literal_copy;
                }
                LZ4_wildCopy32(op, ip, op + length);
            } else {
                if (endOnInput && ip + length > iend - FASTLOOP_SAFE_DISTANCE) {
                    goto safe_literal_copy;
                }
                if (op + length > oend - FASTLOOP_SAFE_DISTANCE) {
                    goto safe_literal_copy;
                }

                /* Literals */
                LZ4_memcpy_using_offset(op, ip, op + length, length);
            }
            ip += length; op += length;

            /* get offset */
            offset = LZ4_readLE16(ip); ip+=2;
            match = op - offset;

            /* get matchlength */
            length = token & ML_MASK;

            if (length == ML_MASK) {
                variable_length_error error = ok;
                if ((checkOffset) && (unlikely(match + dictSize < lowPrefix))) goto _output_error; /* Error : offset outside buffers */
                length += read_variable_length(&ip, iend - LASTLITERALS + 1, (int)endOnInput, 0, &error);
                if (error != ok) goto _output_error;
                if ((safeDecode) && unlikely((uptrval)(op)+length<(uptrval)op)) goto _output_error; /* overflow detection */
                length += MINMATCH;
                if (op + length > oend - FASTLOOP_SAFE_DISTANCE) {
                    goto safe_match_copy;
                }
            } else {
                length += MINMATCH;
                if (op + length > oend - FASTLOOP_SAFE_DISTANCE) {
                    goto safe_match_copy;
                }

                /* Fastpath check: Avoids a branch in LZ4_wildCopy32 if true */
                if ((dict == withPrefix64k) || (match >= lowPrefix)) {
                    if (offset >= 8) {
                        assert(match >= lowPrefix);
                        assert(match <= op);
                        assert(op + length <= oend);

                        LZ4_memcpy_using_offset(op, match, op + length, length);
                        op += length;
                        continue;
            }   }   }

            if (checkOffset && (unlikely(match + dictSize < lowPrefix))) goto _output_error; /* Error : offset outside buffers */
            /* match starting within external dictionary */
            if ((dict==usingExtDict) && (match < lowPrefix)) {
                if (unlikely(op+length > oend-LASTLITERALS)) {
                    if (partialDecoding) {
                        DEBUGLOG(7, "partialDecoding: dictionary match, close to dstEnd");
                        length = MIN(length, (size_t)(oend-op));
                    } else {
                        goto _output_error; /* end-of-block condition violated */
                    }
                }

                if (length <= (size_t)(lowPrefix-match)) {
                    /* match fits entirely within external dictionary : just copy */
                    memmove(op, dictEnd - (lowPrefix-match), length);
                    op += length;
                } else {
                    /* match stretches into both external dictionary and current block */
                    size_t const copySize = (size_t)(lowPrefix - match);
                    size_t const restSize = length - copySize;
                    LZ4_memcpy_using_offset(op, dictEnd - copySize, op + copySize, copySize);
                    op += copySize;
                    if (restSize > (size_t)(op - lowPrefix)) { /* overlap copy */
                        BYTE* const endOfMatch = op + restSize;
                        const BYTE* copyFrom = lowPrefix;
                        while (op < endOfMatch) *op++ = *copyFrom++;
                    } else {
                        LZ4_memcpy_using_offset(op, lowPrefix, op + restSize, restSize);
                        op += restSize;
                    }
                }
                continue;
            }

            /* copy match within block */
            cpy = op + length;

            assert((op <= oend) && (oend-op >= 32));
            if (unlikely(offset<16)) {
                LZ4_memcpy_using_offset_base(op, match, cpy, offset);
            } else {
                LZ4_wildCopy32(op, match, cpy);
            }

            op = cpy;   /* wildcopy correction */
        }
        safe_decode:
#endif

        /* Main Loop : decode remaining sequences where output < FASTLOOP_SAFE_DISTANCE */
        while (1) {
            token = *ip++;
            length = token >> ML_BITS;  /* literal length */

            /* A two-stage shortcut for the most common case:
             * 1) If the literal length is 0..14, and there is enough space,
             * enter the shortcut and copy 16 bytes on behalf of the literals
             * (in the fast mode, only 8 bytes can be safely copied this way).
             * 2) Further if the match length is 4..18, copy 18 bytes in a similar
             * manner; but we ensure that there's enough space in the output for
             * those 18 bytes earlier, upon entering the shortcut (in other words,
             * there is a combined check for both stages).
             */
            if ( (endOnInput ? length != RUN_MASK : length <= 8)
                /* strictly "less than" on input, to re-enter the loop with at least one byte */
              && likely((endOnInput ? ip < shortiend : 1) & (op <= shortoend)) ) {
                /* Copy the literals */
                LZ4_memcpy_using_offset(op, ip, op + 16, 16);
                op += length; ip += length;

                /* The second stage: prepare for match copying, decode full info.
                 * If it doesn't work out, the info won't be wasted. */
                length = token & ML_MASK; /* match length */
                offset = LZ4_readLE16(ip); ip += 2;
                match = op - offset;
                assert(match <= op); /* check overflow */

                /* Do not deal with overlapping matches. */
                if ( (length != ML_MASK)
                  && (offset >= 8)
                  && (dict==withPrefix64k || match >= lowPrefix) ) {
                    /* Copy the match. */
                    LZ4_memcpy_using_offset(op, match, op + length + MINMATCH, length + MINMATCH);
                    op += length + MINMATCH;
                    /* Both stages worked, load the next token. */
                    continue;
                }

                /* The second stage didn't work out, but the info is ready.
                 * Propel it right to the point of match copying. */
                goto _copy_match;
            }

            /* decode literal length */
            if (length == RUN_MASK) {
                variable_length_error error = ok;
                length += read_variable_length(&ip, iend-RUN_MASK, (int)endOnInput, 0, &error);
                if (error == initial_error) { goto _output_error; }
                if ((safeDecode) && unlikely((uptrval)(op)+length<(uptrval)(op))) { goto _output_error; } /* overflow detection */
                if ((safeDecode) && unlikely((uptrval)(ip)+length<(uptrval)(ip))) { goto _output_error; } /* overflow detection */
            }

            /* copy literals */
            if (endOnInput) {  /* LZ4_decompress_safe() */
                if ((cpy>oend-MFLIMIT) || (ip+length>iend-(2+1+LASTLITERALS))) {
                    /* We've either hit the input parsing restriction or the output parsing restriction.
                     * In the normal scenario, decoding a full block, it must be the last sequence,
                     * otherwise it's an error (invalid input or dimensions).
                     * In partialDecoding scenario, it's OK to multiple attempts, as long as there is no issue with the byte boundaries.
                     */
                    if (partialDecoding) {
                        /* Since we are partial decoding we may be in this block because of the output parsing
                         * restriction, which is not valid since the output buffer is allowed to be undersized.
                         */
                        DEBUGLOG(7, "partialDecoding: copying literals, close to input or output end")
                        DEBUGLOG(7, "partialDecoding: literal length = %u", (unsigned)length);
                        DEBUGLOG(7, "partialDecoding: remaining space in dstBuffer : %i", (int)(oend - op));
                        DEBUGLOG(7, "partialDecoding: remaining space in srcBuffer : %i", (int)(iend - ip));
                        /* Finishing in the middle of a literals segment,
                         * due to lack of input.
                         */
                        if (ip+length > iend) {
                            length = (size_t)(iend-ip);
                            cpy = op + length;
                        }
                        /* Finishing in the middle of a literals segment,
                         * due to lack of output space.
                         */
                        if (cpy > oend) {
                            cpy = oend;
                            assert(op<=oend);
                            length = (size_t)(oend-op);
                        }
                    } else {
                        if ((!endOnInput) && (cpy != oend)) { goto _output_error; } /* Error : block decoding must stop exactly there */
                        if ((endOnInput) && ((ip+length != iend) || (cpy > oend))) { goto _output_error; } /* Error : input must be consumed */
                    }
                    LZ4_memmove(op, ip, length);  /*< supports overlapping memory regions, for in-place decompression scenarios */
                    ip += length;
                    op += length;
                    /* Necessarily EOF when !partialDecoding.
                     * When partialDecoding, it is EOF if we've either
                     * filled the output buffer or
                     * can't proceed with reading an offset for following match.
                     */
                    if (!partialDecoding || (cpy == oend) || (ip >= (iend-2))) {
                        break;
                    }
                } else {
                    LZ4_wildCopy8(op, ip, cpy);   /* may overwrite up to 8 bytes beyond cpy */
                    ip += length; op = cpy;
                }
            } else {  /* !endOnInput : LZ4_decompress_fast() */
                LZ4_wildCopy8(op, ip, cpy);
                ip += length; op = cpy;
            }

            /* get offset */
            offset = LZ4_readLE16(ip); ip+=2;
            match = op - offset;

            /* get matchlength */
            length = token & ML_MASK;

    _copy_match:
            if (length == ML_MASK) {
              variable_length_error error = ok;
              length += read_variable_length(&ip, iend - LASTLITERALS + 1, (int)endOnInput, 0, &error);
              if (error != ok) goto _output_error;
                if ((safeDecode) && unlikely((uptrval)(op)+length<(uptrval)op)) goto _output_error; /* overflow detection */
            }
            length += MINMATCH;

#if LZ4_FAST_DEC_LOOP
        safe_match_copy:
#endif
            if (checkOffset && (unlikely(match + dictSize < lowPrefix))) goto _output_error; /* Error : offset outside buffers */
            /* match starting within external dictionary */
            if ((dict==usingExtDict) && (match < lowPrefix)) {
                if (unlikely(op+length > oend-LASTLITERALS)) {
                    if (partialDecoding) length = MIN(length, (size_t)(oend-op));
                    else goto _output_error; /* doesn't respect parsing restriction */
                }

                if (length <= (size_t)(lowPrefix-match)) {
                    /* match fits entirely within external dictionary : just copy */
                    memmove(op, dictEnd - (lowPrefix-match), length);
                    op += length;
                } else {
                    /* match stretches into both external dictionary and current block */
                    size_t const copySize = (size_t)(lowPrefix - match);
                    size_t const restSize = length - copySize;
                    LZ4_memcpy_using_offset(op, dictEnd - copySize, op + copySize, copySize);
                    op += copySize;
                    if (restSize > (size_t)(op - lowPrefix)) { /* overlap copy */
                        BYTE* const endOfMatch = op + restSize;
                        const BYTE* copyFrom = lowPrefix;
                        while (op < endOfMatch) *op++ = *copyFrom++;
                    } else {
                        LZ4_memcpy_using_offset(op, lowPrefix, op + restSize, restSize);
                        op += restSize;
                } }
                continue;
            }
            assert(match >= lowPrefix);

            /* copy match within block */
            cpy = op + length;

            /* partialDecoding : may end anywhere within the block */
            assert(op<=oend);
            if (partialDecoding && (cpy > oend-MATCH_SAFEGUARD_DISTANCE)) {
                size_t const mlen = MIN(length, (size_t)(oend-op));
                const BYTE* const matchEnd = match + mlen;
                BYTE* const copyEnd = op + mlen;
                if (matchEnd > op) {   /* overlap copy */
                    while (op < copyEnd) { *op++ = *match++; }
                } else {
                    LZ4_memcpy_using_offset(op, match, copyEnd, mlen);
                }
                op = copyEnd;
                if (op == oend) { break; }
                continue;
            }

            if (unlikely(offset<8)) {
                LZ4_write32(op, 0);   /* silence msan warning when offset==0 */
                op[0] = match[0];
                op[1] = match[1];
                op[2] = match[2];
                op[3] = match[3];
                match += inc32table[offset];
                LZ4_memcpy_using_offset(op+4, match, op+4, 4);
                match -= dec64table[offset];
            } else {
                LZ4_memcpy_using_offset(op, match, op, 8);
                match += 8;
            }
            op += 8;

            if (unlikely(cpy > oend-MATCH_SAFEGUARD_DISTANCE)) {
                BYTE* const oCopyLimit = oend - (WILDCOPYLENGTH-1);
                if (cpy > oend-LASTLITERALS) { goto _output_error; } /* Error : last LASTLITERALS bytes must be literals (uncompressed) */
                if (op < oCopyLimit) {
                    LZ4_wildCopy8(op, match, oCopyLimit);
                    match += oCopyLimit - op;
                    op = oCopyLimit;
                }
                while (op < cpy) { *op++ = *match++; }
            } else {
                LZ4_memcpy_using_offset(op, match, cpy, 8);
            }
            op = cpy;   /* wildcopy correction */
        }

        /* end of decoding */
        if (endOnInput) {
            DEBUGLOG(5, "decoded %i bytes", (int) (((char*)op)-dst));
            return (int) (((char*)op)-dst);     /* Nb of output bytes decoded */
        } else {
            DEBUGLOG(5, "decoded %i bytes", (int) (((char*)ip)-src));
            return (int) (((char*)ip)-src);     /* Nb of input bytes read */
        }

        /* Overflow error detected */
    _output_error:
        DEBUGLOG(5, "Overflow error detected");
        return (int) (-(((const char*)ip)-src))-1;
    }
}

/*-************************************
*  Utility functions
**************************************/
int LZ4_compressBound(int inputSize)
{
    return LZ4_COMPRESSBOUND(inputSize);
}

int LZ4_sizeofState(void)
{
    return LZ4_STREAMSIZE;
}

int LZ4_compress_fast_extState(void* state, const char* src, char* dst, int srcSize, int dstCapacity, int acceleration)
{
    LZ4_stream_t_internal* const ctx = &((LZ4_stream_t*)state)->internal_donotuse;
    if (acceleration < 1) acceleration = LZ4_ACCELERATION_DEFAULT;
    if (acceleration > LZ4_ACCELERATION_MAX) acceleration = LZ4_ACCELERATION_MAX;
    
    LZ4_resetStream((LZ4_stream_t*)state);
    if (srcSize < LZ4_64Klimit) {
        return LZ4_compress_generic(src, dst, srcSize, NULL, dstCapacity, limitedOutput, byU16, noDict, noDictIssue, acceleration);
    } else {
        return LZ4_compress_generic(src, dst, srcSize, NULL, dstCapacity, limitedOutput, byU32, noDict, noDictIssue, acceleration);
    }
}

int LZ4_decompress_safe_partial(const char* src, char* dst, int srcSize, int targetOutputSize, int dstCapacity)
{
    dstCapacity = MIN(targetOutputSize, dstCapacity);
    return LZ4_decompress_generic(src, dst, srcSize, dstCapacity, endOnInputSize, partial, noDict, (BYTE*)dst, NULL, 0);
} 