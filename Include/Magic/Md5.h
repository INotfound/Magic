#pragma once
// Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
// rights reserved.

// License to copy and use this software is granted provided that it
// is identified as the "RSA Data Security, Inc. Md5 Message-Digest
// Algorithm" in all material mentioning or referencing this software
// or this function.
//
// License is also granted to make and use derivative works provided
// that such works are identified as "derived from the RSA Data
// Security, Inc. Md5 Message-Digest Algorithm" in all material
// mentioning or referencing the derived work.
//
// RSA Data Security, Inc. makes no representations concerning either
// the merchantability of this software or the suitability of this
// software for any particular purpose. It is provided "as is"
// without express or implied warranty of any kind.
//
// These notices must be retained in any copies of any part of this
// documentation and/or software.



// The original md5 implementation avoids external libraries.
// This version has dependency on stdio.h for file input and
// string.h for memcpy.
#include <cstdio>
#include <cstring>
#include <iostream>

namespace Magic {
    // convenient object that wraps
    // the C-functions for use in C++ only
    class Md5 {
    public:
        // MAIN FUNCTIONS
        Md5();
        /// Load a file from disk and digest it
        // Digests a file and returns the result.
        const char* getFile( const char *filename );

        // Digests a string and prints the result.
        const char* getString(const char *string );
    private:
        //#pragma region static helper functions
        // The core of the Md5 algorithm is here.
        // Md5 basic transformation. Transforms state based on block.
        static void MD5Transform( uint32_t state[4], unsigned char block[64] );
        // Encodes input (uint32_t) into output (unsigned char). Assumes len is
        // a multiple of 4.
        static void Encode( unsigned char *output, uint32_t *input, unsigned int len );

        // Decodes input (unsigned char) into output (uint32_t). Assumes len is
        // a multiple of 4.
        static void Decode( uint32_t *output, unsigned char *input, unsigned int len );
        //#pragma endregion

        // Md5 initialization. Begins an Md5 operation, writing a new context.
        void Init();

        // Md5 block update operation. Continues an Md5 message-digest
        // operation, processing another message block, and updating the
        // context.
        void Update(unsigned char *input,unsigned int inputLen);

        // Md5 finalization. Ends an Md5 message-digest operation, writing the
        // the message digest and zeroizing the context.
        // Writes to Raw
        void Final();

        /// Buffer must be 32+1 (nul) = 33 chars long at least
        void writeToString();
    private:
        struct context_t {
            uint32_t state[4];                                   /* state (ABCD) */
            uint32_t count[2];        /* number of bits, modulo 2^64 (lsb first) */
            unsigned char buffer[64];                         /* input buffer */
        } context ;

        // an Md5 digest is a 16-byte number (32 hex digits)
        unsigned char Raw[ 16 ] ;

        // This version of the digest is actually
        // a "printf'd" version of the digest.
        char Chars[ 33 ] ;
    };
}