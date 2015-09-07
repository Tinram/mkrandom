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

#define KILO (1024)
#define MEGA (1024 * 1024)
#define GIGA (1024 * 1024 * 1024)
#define TERA (1024 * 1024 * 1024 * 1024)

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
long long size_to_bytes( char *file_size ) {
    size_t len = strlen(file_size);
    long long ret = 0;
    long long part = 0;
    for (size_t i = 0; i < len; i++) {
        if (isdigit(file_size[i])) {
            part = part * 10 + (file_size[i] - '0');
        } else {
            int modifier = tolower(file_size[i]);
            switch (modifier) {
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
    assert(size_to_bytes("177g") == 177L * GIGA);
    assert(size_to_bytes("5t") == 5LL * 1024 * TERA);
    assert(size_to_bytes("3t25g217m1011k273") == 3LL * TERA + 25LL * GIGA + 217LL * MEGA + 1011LL * KILO + 273);
}

//=============================================================================
long long parse_arguments( int argc, char* argv[] ) {
    if ( argc != 3 ) {
        return -1;
    }
    return size_to_bytes( argv[ 1 ] );
}

//=============================================================================
int mkrandom( long long file_size, char *file_name ) {
    long page_size = sysconf( _SC_PAGESIZE );
    uint32_t *buff = malloc( page_size );
    FILE* pFile = fopen( file_name, "wb" );
    setvbuf(pFile, NULL, _IOFBF, BUFSIZ);
    long long num_pages = file_size / page_size;
    long long tail_size = file_size % page_size;
    for ( long long i = 0; i < num_pages; i++ ) {
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
int print_help() {
    printf( "usage: mkrandom size[k|m|g|t] filename\n" );
    return 1;
}

//=============================================================================
int main( int argc, char* argv[] )
{    
    run_tests();
    seed_pcg_random();
    long long file_size = parse_arguments( argc, argv );
    if ( file_size <= 0) {
        return print_help();
    }
    if (mkrandom( file_size, argv[ 2 ] ) != 0) {
        return print_help();
    }
    return 0;
}
