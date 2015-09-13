//=============================================================================
//  mkrandom, fast generator of incompressible random files.
//=============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>

#define KILO ((size_t)1024)
#define MEGA (KILO * 1024)
#define GIGA (MEGA * 1024)
#define TERA (GIGA * 1024)

//=============================================================================
// *Really* minimal PCG32 code / (c) 2014 M.E. O'Neill / pcg-random.org
// Licensed under Apache License 2.0 (NO WARRANTY, etc. see website)
//=============================================================================
typedef struct { uint64_t state;  uint64_t inc; } pcg32_random_t;
uint32_t pcg32_random_r(pcg32_random_t* rng)
{
    uint64_t oldstate = rng->state;
    // Advance internal state
    rng->state = oldstate * 6364136223846793005ULL + (rng->inc | 1);
    // Calculate output function (XSH RR), uses old state for max ILP
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}
pcg32_random_t pcg32_random;

void seed_pcg_random() {
    pcg32_random.state ^= (uint64_t)time(NULL) ^ (uint64_t)&seed_pcg_random;
}

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
// Convert file size with k|m|g|t modifiers into size in bytes.
//=============================================================================
size_t size_to_bytes( char *file_size ) {
    size_t len = strlen( file_size );
    size_t ret = 0;
    size_t part = 0;
    for ( size_t i = 0; i < len; i++ ) {
        if ( isdigit( file_size[ i ] ) ) {
            part = part * 10 + ( file_size[ i ] - '0' );
        } else {
            int modifier = tolower( file_size[ i ] );
            switch ( modifier ) {
            case 'k':
                ret += part * KILO;
                part = 0;
            case 'm':
                ret += part * MEGA;
                part = 0;
            case 'g':
                ret += part * GIGA;
                part = 0;
            case 't':
                ret += part * TERA;
                part = 0;
            default:
                ret += part;
                part = 0;
            }
        }
    }
    return ret + part;
}

//=============================================================================
void run_tests() {
    assert(size_to_bytes("123456") == 123456);
    assert(size_to_bytes("300k") == 300 * KILO);
    assert(size_to_bytes("20m") == 20 * MEGA);
    assert(size_to_bytes("177g") == 177 * GIGA);
    assert(size_to_bytes("5t") == 5 * TERA);
    assert(size_to_bytes("3t25g217m1011k273") == 3 * TERA + 25 * GIGA + 217 * MEGA + 1011 * KILO + 273);
}

//=============================================================================
int mkrandom( size_t file_size, char *file_name ) {
    long page_size = sysconf( _SC_PAGESIZE );
    uint32_t *buff = malloc( page_size );
    FILE* pFile = fopen( file_name, "wb" );
    if (pFile == NULL) {
        perror(file_name);
        return 1;
    }
    size_t num_pages = file_size / page_size;
    size_t tail_size = file_size % page_size;
    for ( size_t i = 0; i < num_pages; i++ ) {
        fill_buffer( buff, page_size / sizeof( uint32_t ) );
        fwrite( buff, page_size, 1, pFile );
    }
    if ( tail_size > 0 ) {
        fill_buffer( buff, page_size / sizeof( uint32_t ) );
        fwrite( buff, tail_size, 1, pFile );
    }
    fclose( pFile );
    return 0;
}

//=============================================================================
int main( int argc, char* argv[] )
{
    run_tests();
    seed_pcg_random();
    if ( argc != 3 ) {
        printf( "usage: mkrandom size[k|m|g|t] filename\n" );
        return 1;
    }
    return mkrandom( size_to_bytes( argv[ 1 ] ), argv[ 2 ] );
}
