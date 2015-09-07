//=============================================================================
//  mkrandom, fast generator of incompressible random files.
//=============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

//=============================================================================
// *Really* minimal PCG32 code / (c) 2014 M.E. O'Neill / pcg-random.org
// Licensed under Apache License 2.0 (NO WARRANTY, etc. see website)
//=============================================================================
typedef struct { uint64_t state;  uint64_t inc; } pcg32_random_t;
uint32_t pcg32_random_r(pcg32_random_t* rng)
{
    uint64_t oldstate = rng->state;
    // Advance internal state
    rng->state = oldstate * 6364136223846793005ULL + (rng->inc|1);
    // Calculate output function (XSH RR), uses old state for max ILP
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}
pcg32_random_t pcg32_random;

//=============================================================================
// fills buffer with PCG randoms
//=============================================================================
void fill_buffer( uint32_t* buff, int size )
{
    for ( int i = 0; i < size; i++ ) {
        *buff = pcg32_random_r( &pcg32_random );
        buff++;
    }
}

//=============================================================================
int main( int argc, char* argv[] )
{
    pcg32_random.state = (uint64_t)time(NULL) ^ (uint64_t)&main;     
    FILE* pFile;
    uint32_t* buff;
    int chunk_size;
    int count;
    if ( argc != 4 ) {
        printf( "usage: mkrandom <FileName> <ChunkSize> <ChunkCount>\n" );
        return 1;
    }
    chunk_size = atoi( argv[ 2 ] );
    count = atoi( argv[ 3 ] );
    buff = malloc( chunk_size );
    pFile = fopen( argv[ 1 ], "wb" );
    for ( int i = 0; i < count; ++i ) {
        fill_buffer( buff, chunk_size / sizeof( uint32_t ) );
        fwrite( buff, chunk_size, 1, pFile );
    }
    fclose( pFile );
    return 0;
}
