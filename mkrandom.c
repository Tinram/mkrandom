//=============================================================================
// mkrandom, fast generator of incompressible random files
//     by cohadar, github.com/cohadar/mkrandom
//     minor revisions by Tinram
//     rv 23/04/18, add stdout
//     rv 30/04/18, add free malloc(bs) - fix valgrind memleak
//=============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include <assert.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>

#define KILO 0x400ULL
#define MEGA 0x100000ULL
#define GIGA 0x40000000ULL
#define TERA 0x10000000000ULL

//=============================================================================
// *Really* minimal PCG32 code / (c) 2014 Melissa E. O'Neill / pcg-random.org
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
    // randomness from timer and memory address
    pcg32_random.state ^= (uint64_t)time(NULL) ^ (uint64_t)&seed_pcg_random;
    // generate a dozen of integers for initial state to diverge
    for ( int i = 0; i < 12; i++ ) {
        pcg32_random_r( &pcg32_random );
    }
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
uintmax_t size_to_bytes( char *file_size ) {

    size_t len = strlen( file_size );
    uintmax_t ret = 0;
    uintmax_t part = 0;

    for ( size_t i = 0; i < len; i++ ) {
        if ( isdigit( file_size[ i ] ) ) {
            part = part * 10 + ( file_size[ i ] - '0' );
        }
        else {
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
/*void run_tests() {
    assert(size_to_bytes("123456") == 123456);
    assert(size_to_bytes("300k") == 300 * KILO);
    assert(size_to_bytes("20m") == 20 * MEGA);
    assert(size_to_bytes("177g") == 177 * GIGA);
    assert(size_to_bytes("5t") == 5 * TERA);
    assert(size_to_bytes("3t25g217m1011k273") == 3 * TERA + 25 * GIGA + 217 * MEGA + 1011 * KILO + 273);
}*/

//=============================================================================

int mkrandom( uintmax_t file_size, char *file_name, size_t bs, int argc ) {

    uint32_t *buff = malloc( bs );
    if (buff == NULL) {goto out_nomem;}

    FILE* pFile;
    uintmax_t num_pages = file_size / bs;
    size_t tail_size = file_size % bs;

    if (argc == 3) {

        pFile = fopen( file_name, "wb" );
        if (pFile == NULL) {goto out_fopen_failed;}

        for ( uintmax_t i = 0; i < num_pages; i++ ) {
            fill_buffer( buff, bs / sizeof( uint32_t ) );
            size_t bw = fwrite( buff, bs, 1, pFile );
            if ( bw != 1 ) {goto out_write_failed;}
        }

        if ( tail_size > 0 ) {
            fill_buffer( buff, bs / sizeof( uint32_t ) );
            size_t bw = fwrite( buff, tail_size, 1, pFile );
            if ( bw != 1 ) {goto out_write_failed;}
        }

        fclose( pFile );
    }
    else {

        for ( uintmax_t i = 0; i < num_pages; i++ ) {
            fill_buffer( buff, bs / sizeof( uint32_t ) );
            size_t bw = fwrite( buff, bs, 1, stdout );
            if ( bw != 1 ) {goto out_write_failed;}
        }

        if ( tail_size > 0 ) {
            fill_buffer( buff, bs / sizeof( uint32_t ) );
            size_t bw = fwrite( buff, tail_size, 1, stdout );
            if ( bw != 1 ) {goto out_write_failed;}
        }
    }

    free(buff);

    return 0;

    //////////////////////////////

    out_write_failed:
        if (argc == 3) {
            perror( file_name );
            fclose( pFile );
            free(buff);
            return -1;
        }

    out_nomem:
        perror( "buffer" );
        return errno;

    out_fopen_failed:
        perror( file_name );
        free(buff);
        return errno;
}

//=============================================================================
int main( int argc, char* argv[] )
{
    //run_tests();

    seed_pcg_random();

    if ( argc < 2 ) {
        printf( "usage: mkrandom <size><k|m|g|t> [filename]\n" );
        return 1;
    }

    size_t block_size = 4 * KILO;
    size_t buffer_size = 16 * block_size;

    return mkrandom( size_to_bytes( argv[ 1 ] ), argv[ 2 ], buffer_size, argc );
}
