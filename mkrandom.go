package main

import (
	"encoding/binary"
	"fmt"
	"os"
	"time"
	"unicode"
	"unsafe"
)

const (
	KILO = uint64(0x400)
	MEGA = uint64(0x100000)
	GIGA = uint64(0x40000000)
	TERA = uint64(0x10000000000)
)

//=============================================================================
// *Really* minimal PCG32 code / (c) 2014 M.E. O'Neill / pcg-random.org
// Licensed under Apache License 2.0 (NO WARRANTY, etc. see website)
//=============================================================================
type pcg32_random_t struct {
	state uint64
	inc   uint64
}

func (rng *pcg32_random_t) pcg32_random_r() uint32 {
	oldstate := rng.state
	// Advance internal state
	rng.state = oldstate*6364136223846793005 + (rng.inc | 1)
	// Calculate output function (XSH RR), uses old state for max ILP
	xorshifted := uint32(((oldstate >> 18) ^ oldstate) >> 27)
	rot := uint32(oldstate >> 59)
	return (xorshifted >> rot) | (xorshifted << ((-rot) & 31))
}

var pcg32_random pcg32_random_t

func seed_pcg_random() {
	// randomness from timer and memory address
	pcg32_random.state ^= uint64(time.Now().UnixNano()) ^ uint64(uintptr(unsafe.Pointer(&pcg32_random)))
	// generate a dozen of integers for initial state to diverge
	for i := 0; i < 12; i++ {
		pcg32_random.pcg32_random_r()
	}
}

//=============================================================================
// fills buffer with PCG randoms
//=============================================================================
func fill_buffer(buff []byte) {
	for i := 0; i < len(buff)/4; i++ {
		binary.LittleEndian.PutUint32(buff[(i*4):], pcg32_random.pcg32_random_r())
	}
}

//=============================================================================
// Convert file size with k|m|g|t modifiers into size in bytes.
//=============================================================================
func size_to_bytes(file_size string) uint64 {
	ret := uint64(0)
	part := uint64(0)
	for _, r := range file_size {
		if unicode.IsDigit(r) {
			part = part*10 + uint64(r-'0')
		} else {
			modifier := unicode.ToLower(r)
			switch modifier {
			case 'k':
				ret += part * KILO
				part = 0
			case 'm':
				ret += part * MEGA
				part = 0
			case 'g':
				ret += part * GIGA
				part = 0
			case 't':
				ret += part * TERA
				part = 0
			default:
				ret += part
				part = 0
			}
		}
	}
	return ret + part
}

func assert(condition bool) {
	if !condition {
		os.Exit(1)
	}
}

//=============================================================================
func run_tests() {
	assert(size_to_bytes("123456") == 123456)
	assert(size_to_bytes("300k") == 300*KILO)
	assert(size_to_bytes("20m") == 20*MEGA)
	assert(size_to_bytes("177g") == 177*GIGA)
	assert(size_to_bytes("5t") == 5*TERA)
	assert(size_to_bytes("3t25g217m1011k273") == 3*TERA+25*GIGA+217*MEGA+1011*KILO+273)
}

//=============================================================================
func mkrandom(file_size uint64, file_name string, buffer_size uint64) {
	buff := make([]byte, buffer_size)
	// open output file
	pFile, err := os.Create(file_name)
	if err != nil {
		panic(err)
	}
	// close pFile on exit and check for its returned error
	defer func() {
		if err := pFile.Close(); err != nil {
			panic(err)
		}
	}()
	num_pages := file_size / buffer_size
	tail_size := file_size % buffer_size
	for i := uint64(0); i < num_pages; i++ {
		fill_buffer(buff)
		// write a chunk
		if _, err := pFile.Write(buff); err != nil {
			panic(err)
		}
	}
	if tail_size > 0 {
		fill_buffer(buff)
		// write a chunk
		if _, err := pFile.Write(buff[:tail_size]); err != nil {
			panic(err)
		}
	}
}

func main() {
	run_tests()
	seed_pcg_random()
	if len(os.Args) != 3 {
		fmt.Println("usage: mkrandom size[k|m|g|t] filename")
		return
	}
	block_size := 4 * KILO
	buffer_size := 16 * block_size * 4
	mkrandom(size_to_bytes(os.Args[1]), os.Args[2], buffer_size)
}
