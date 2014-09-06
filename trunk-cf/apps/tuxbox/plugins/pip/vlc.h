/*
 * vlc.h
 * Copyright (C) 2000-2003 Michel Lespinasse <walken@zoy.org>
 * Copyright (C) 1999-2000 Aaron Holtzman <aholtzma@ess.engr.uvic.ca>
 *
 * This file is part of mpeg2dec, a free MPEG-2 video stream decoder.
 * See http://libmpeg2.sourceforge.net/ for updates.
 *
 * mpeg2dec is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * mpeg2dec is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#define GETWORD(bit_buf,shift,bit_ptr)				\
do {								\
    bit_buf |= ((bit_ptr[0] << 8) | bit_ptr[1]) << (shift);	\
    bit_ptr += 2;						\
} while (0)

static inline void bitstream_init (mpeg2_decoder_t * decoder,
				   const uint8_t * start)
{
    decoder->bitstream_buf =
	(start[0] << 24) | (start[1] << 16) | (start[2] << 8) | start[3];
    decoder->bitstream_ptr = start + 4;
    decoder->bitstream_bits = -16;
}

/* make sure that there are at least 16 valid bits in bit_buf */
#define NEEDBITS(bit_buf,bits,bit_ptr)		\
do {						\
    if (unlikely (bits > 0)) {			\
	GETWORD (bit_buf, bits, bit_ptr);	\
	bits -= 16;				\
    }						\
} while (0)

/* remove num valid bits from bit_buf */
#define DUMPBITS(bit_buf,bits,num)	\
do {					\
    bit_buf <<= (num);			\
    bits += (num);			\
} while (0)

/* take num bits from the high part of bit_buf and zero extend them */
#define UBITS(bit_buf,num) (((uint32_t)(bit_buf)) >> (32 - (num)))

/* take num bits from the high part of bit_buf and sign extend them */
#define SBITS(bit_buf,num) (((int32_t)(bit_buf)) >> (32 - (num)))

typedef struct {
    uint8_t modes;
    uint8_t len;
} MBtab;

typedef struct {
    uint8_t delta;
    uint8_t len;
} MVtab;

typedef struct {
    int8_t dmv;
    uint8_t len;
} DMVtab;

typedef struct {
    uint8_t cbp;
    uint8_t len;
} CBPtab;

typedef struct {
    uint8_t size;
    uint8_t len;
} DCtab;

typedef struct {
    uint8_t run;
    uint8_t level;
    uint8_t len;
} DCTtab;

typedef struct {
    uint8_t mba;
    uint8_t len;
} MBAtab;


#define INTRA MACROBLOCK_INTRA
#define QUANT MACROBLOCK_QUANT

static const MBtab MB_I [] = {
    {INTRA|QUANT, 2}, {INTRA, 1}
};

#define MC MACROBLOCK_MOTION_FORWARD
#define CODED MACROBLOCK_PATTERN

static const MBtab MB_P [] = {
    {INTRA|QUANT, 6}, {CODED|QUANT, 5}, {MC|CODED|QUANT, 5}, {INTRA,    5},
    {MC,          3}, {MC,          3}, {MC,             3}, {MC,       3},
    {CODED,       2}, {CODED,       2}, {CODED,          2}, {CODED,    2},
    {CODED,       2}, {CODED,       2}, {CODED,          2}, {CODED,    2},
    {MC|CODED,    1}, {MC|CODED,    1}, {MC|CODED,       1}, {MC|CODED, 1},
    {MC|CODED,    1}, {MC|CODED,    1}, {MC|CODED,       1}, {MC|CODED, 1},
    {MC|CODED,    1}, {MC|CODED,    1}, {MC|CODED,       1}, {MC|CODED, 1},
    {MC|CODED,    1}, {MC|CODED,    1}, {MC|CODED,       1}, {MC|CODED, 1}
};

#define FWD MACROBLOCK_MOTION_FORWARD
#define BWD MACROBLOCK_MOTION_BACKWARD
#define INTER MACROBLOCK_MOTION_FORWARD|MACROBLOCK_MOTION_BACKWARD

static const MBtab MB_B [] = {
    {0,                 0}, {INTRA|QUANT,       6},
    {BWD|CODED|QUANT,   6}, {FWD|CODED|QUANT,   6},
    {INTER|CODED|QUANT, 5}, {INTER|CODED|QUANT, 5},
					{INTRA,       5}, {INTRA,       5},
    {FWD,         4}, {FWD,         4}, {FWD,         4}, {FWD,         4},
    {FWD|CODED,   4}, {FWD|CODED,   4}, {FWD|CODED,   4}, {FWD|CODED,   4},
    {BWD,         3}, {BWD,         3}, {BWD,         3}, {BWD,         3},
    {BWD,         3}, {BWD,         3}, {BWD,         3}, {BWD,         3},
    {BWD|CODED,   3}, {BWD|CODED,   3}, {BWD|CODED,   3}, {BWD|CODED,   3},
    {BWD|CODED,   3}, {BWD|CODED,   3}, {BWD|CODED,   3}, {BWD|CODED,   3},
    {INTER,       2}, {INTER,       2}, {INTER,       2}, {INTER,       2},
    {INTER,       2}, {INTER,       2}, {INTER,       2}, {INTER,       2},
    {INTER,       2}, {INTER,       2}, {INTER,       2}, {INTER,       2},
    {INTER,       2}, {INTER,       2}, {INTER,       2}, {INTER,       2},
    {INTER|CODED, 2}, {INTER|CODED, 2}, {INTER|CODED, 2}, {INTER|CODED, 2},
    {INTER|CODED, 2}, {INTER|CODED, 2}, {INTER|CODED, 2}, {INTER|CODED, 2},
    {INTER|CODED, 2}, {INTER|CODED, 2}, {INTER|CODED, 2}, {INTER|CODED, 2},
    {INTER|CODED, 2}, {INTER|CODED, 2}, {INTER|CODED, 2}, {INTER|CODED, 2}
};

#undef INTRA
#undef QUANT
#undef MC
#undef CODED
#undef FWD
#undef BWD
#undef INTER


static const MVtab MV_4 [] = {
    { 3, 6}, { 2, 4}, { 1, 3}, { 1, 3}, { 0, 2}, { 0, 2}, { 0, 2}, { 0, 2}
};

static const MVtab MV_10 [] = {
    { 0,10}, { 0,10}, { 0,10}, { 0,10}, { 0,10}, { 0,10}, { 0,10}, { 0,10},
    { 0,10}, { 0,10}, { 0,10}, { 0,10}, {15,10}, {14,10}, {13,10}, {12,10},
    {11,10}, {10,10}, { 9, 9}, { 9, 9}, { 8, 9}, { 8, 9}, { 7, 9}, { 7, 9},
    { 6, 7}, { 6, 7}, { 6, 7}, { 6, 7}, { 6, 7}, { 6, 7}, { 6, 7}, { 6, 7},
    { 5, 7}, { 5, 7}, { 5, 7}, { 5, 7}, { 5, 7}, { 5, 7}, { 5, 7}, { 5, 7},
    { 4, 7}, { 4, 7}, { 4, 7}, { 4, 7}, { 4, 7}, { 4, 7}, { 4, 7}, { 4, 7}
};


static const DMVtab DMV_2 [] = {
    { 0, 1}, { 0, 1}, { 1, 2}, {-1, 2}
};


static const CBPtab CBP_7 [] = {
    {0x11, 7}, {0x12, 7}, {0x14, 7}, {0x18, 7},
    {0x21, 7}, {0x22, 7}, {0x24, 7}, {0x28, 7},
    {0x3f, 6}, {0x3f, 6}, {0x30, 6}, {0x30, 6},
    {0x09, 6}, {0x09, 6}, {0x06, 6}, {0x06, 6},
    {0x1f, 5}, {0x1f, 5}, {0x1f, 5}, {0x1f, 5},
    {0x10, 5}, {0x10, 5}, {0x10, 5}, {0x10, 5},
    {0x2f, 5}, {0x2f, 5}, {0x2f, 5}, {0x2f, 5},
    {0x20, 5}, {0x20, 5}, {0x20, 5}, {0x20, 5},
    {0x07, 5}, {0x07, 5}, {0x07, 5}, {0x07, 5},
    {0x0b, 5}, {0x0b, 5}, {0x0b, 5}, {0x0b, 5},
    {0x0d, 5}, {0x0d, 5}, {0x0d, 5}, {0x0d, 5},
    {0x0e, 5}, {0x0e, 5}, {0x0e, 5}, {0x0e, 5},
    {0x05, 5}, {0x05, 5}, {0x05, 5}, {0x05, 5},
    {0x0a, 5}, {0x0a, 5}, {0x0a, 5}, {0x0a, 5},
    {0x03, 5}, {0x03, 5}, {0x03, 5}, {0x03, 5},
    {0x0c, 5}, {0x0c, 5}, {0x0c, 5}, {0x0c, 5},
    {0x01, 4}, {0x01, 4}, {0x01, 4}, {0x01, 4},
    {0x01, 4}, {0x01, 4}, {0x01, 4}, {0x01, 4},
    {0x02, 4}, {0x02, 4}, {0x02, 4}, {0x02, 4},
    {0x02, 4}, {0x02, 4}, {0x02, 4}, {0x02, 4},
    {0x04, 4}, {0x04, 4}, {0x04, 4}, {0x04, 4},
    {0x04, 4}, {0x04, 4}, {0x04, 4}, {0x04, 4},
    {0x08, 4}, {0x08, 4}, {0x08, 4}, {0x08, 4},
    {0x08, 4}, {0x08, 4}, {0x08, 4}, {0x08, 4},
    {0x0f, 3}, {0x0f, 3}, {0x0f, 3}, {0x0f, 3},
    {0x0f, 3}, {0x0f, 3}, {0x0f, 3}, {0x0f, 3},
    {0x0f, 3}, {0x0f, 3}, {0x0f, 3}, {0x0f, 3},
    {0x0f, 3}, {0x0f, 3}, {0x0f, 3}, {0x0f, 3}
};

static const CBPtab CBP_9 [] = {
    {0,    0}, {0x00, 9}, {0x39, 9}, {0x36, 9},
    {0x37, 9}, {0x3b, 9}, {0x3d, 9}, {0x3e, 9},
    {0x17, 8}, {0x17, 8}, {0x1b, 8}, {0x1b, 8},
    {0x1d, 8}, {0x1d, 8}, {0x1e, 8}, {0x1e, 8},
    {0x27, 8}, {0x27, 8}, {0x2b, 8}, {0x2b, 8},
    {0x2d, 8}, {0x2d, 8}, {0x2e, 8}, {0x2e, 8},
    {0x19, 8}, {0x19, 8}, {0x16, 8}, {0x16, 8},
    {0x29, 8}, {0x29, 8}, {0x26, 8}, {0x26, 8},
    {0x35, 8}, {0x35, 8}, {0x3a, 8}, {0x3a, 8},
    {0x33, 8}, {0x33, 8}, {0x3c, 8}, {0x3c, 8},
    {0x15, 8}, {0x15, 8}, {0x1a, 8}, {0x1a, 8},
    {0x13, 8}, {0x13, 8}, {0x1c, 8}, {0x1c, 8},
    {0x25, 8}, {0x25, 8}, {0x2a, 8}, {0x2a, 8},
    {0x23, 8}, {0x23, 8}, {0x2c, 8}, {0x2c, 8},
    {0x31, 8}, {0x31, 8}, {0x32, 8}, {0x32, 8},
    {0x34, 8}, {0x34, 8}, {0x38, 8}, {0x38, 8}
};


static const DCtab DC_lum_5 [] = {
    {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2},
    {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2},
    {0, 3}, {0, 3}, {0, 3}, {0, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3},
    {4, 3}, {4, 3}, {4, 3}, {4, 3}, {5, 4}, {5, 4}, {6, 5}
};

static const DCtab DC_chrom_5 [] = {
    {0, 2}, {0, 2}, {0, 2}, {0, 2}, {0, 2}, {0, 2}, {0, 2}, {0, 2},
    {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2},
    {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2},
    {3, 3}, {3, 3}, {3, 3}, {3, 3}, {4, 4}, {4, 4}, {5, 5}
};

static const DCtab DC_long [] = {
    {6, 5}, {6, 5}, {6, 5}, {6, 5}, {6, 5}, {6, 5}, { 6, 5}, { 6, 5},
    {6, 5}, {6, 5}, {6, 5}, {6, 5}, {6, 5}, {6, 5}, { 6, 5}, { 6, 5},
    {7, 6}, {7, 6}, {7, 6}, {7, 6}, {7, 6}, {7, 6}, { 7, 6}, { 7, 6},
    {8, 7}, {8, 7}, {8, 7}, {8, 7}, {9, 8}, {9, 8}, {10, 9}, {11, 9}
};


static const DCTtab DCT_16 [] = {
    {129, 0, 0}, {129, 0, 0}, {129, 0, 0}, {129, 0, 0},
    {129, 0, 0}, {129, 0, 0}, {129, 0, 0}, {129, 0, 0},
    {129, 0, 0}, {129, 0, 0}, {129, 0, 0}, {129, 0, 0},
    {129, 0, 0}, {129, 0, 0}, {129, 0, 0}, {129, 0, 0},
    {  2,18, 0}, {  2,17, 0}, {  2,16, 0}, {  2,15, 0},
    {  7, 3, 0}, { 17, 2, 0}, { 16, 2, 0}, { 15, 2, 0},
    { 14, 2, 0}, { 13, 2, 0}, { 12, 2, 0}, { 32, 1, 0},
    { 31, 1, 0}, { 30, 1, 0}, { 29, 1, 0}, { 28, 1, 0}
};

static const DCTtab DCT_15 [] = {
    {  1,40,15}, {  1,39,15}, {  1,38,15}, {  1,37,15},
    {  1,36,15}, {  1,35,15}, {  1,34,15}, {  1,33,15},
    {  1,32,15}, {  2,14,15}, {  2,13,15}, {  2,12,15},
    {  2,11,15}, {  2,10,15}, {  2, 9,15}, {  2, 8,15},
    {  1,31,14}, {  1,31,14}, {  1,30,14}, {  1,30,14},
    {  1,29,14}, {  1,29,14}, {  1,28,14}, {  1,28,14},
    {  1,27,14}, {  1,27,14}, {  1,26,14}, {  1,26,14},
    {  1,25,14}, {  1,25,14}, {  1,24,14}, {  1,24,14},
    {  1,23,14}, {  1,23,14}, {  1,22,14}, {  1,22,14},
    {  1,21,14}, {  1,21,14}, {  1,20,14}, {  1,20,14},
    {  1,19,14}, {  1,19,14}, {  1,18,14}, {  1,18,14},
    {  1,17,14}, {  1,17,14}, {  1,16,14}, {  1,16,14}
};

static const DCTtab DCT_13 [] = {
    { 11, 2,13}, { 10, 2,13}, {  6, 3,13}, {  4, 4,13},
    {  3, 5,13}, {  2, 7,13}, {  2, 6,13}, {  1,15,13},
    {  1,14,13}, {  1,13,13}, {  1,12,13}, { 27, 1,13},
    { 26, 1,13}, { 25, 1,13}, { 24, 1,13}, { 23, 1,13},
    {  1,11,12}, {  1,11,12}, {  9, 2,12}, {  9, 2,12},
    {  5, 3,12}, {  5, 3,12}, {  1,10,12}, {  1,10,12},
    {  3, 4,12}, {  3, 4,12}, {  8, 2,12}, {  8, 2,12},
    { 22, 1,12}, { 22, 1,12}, { 21, 1,12}, { 21, 1,12},
    {  1, 9,12}, {  1, 9,12}, { 20, 1,12}, { 20, 1,12},
    { 19, 1,12}, { 19, 1,12}, {  2, 5,12}, {  2, 5,12},
    {  4, 3,12}, {  4, 3,12}, {  1, 8,12}, {  1, 8,12},
    {  7, 2,12}, {  7, 2,12}, { 18, 1,12}, { 18, 1,12}
};

static const DCTtab DCT_B14_10 [] = {
    { 17, 1,10}, {  6, 2,10}, {  1, 7,10}, {  3, 3,10},
    {  2, 4,10}, { 16, 1,10}, { 15, 1,10}, {  5, 2,10}
};

static const DCTtab DCT_B14_8 [] = {
    { 65, 0, 6}, { 65, 0, 6}, { 65, 0, 6}, { 65, 0, 6},
    {  3, 2, 7}, {  3, 2, 7}, { 10, 1, 7}, { 10, 1, 7},
    {  1, 4, 7}, {  1, 4, 7}, {  9, 1, 7}, {  9, 1, 7},
    {  8, 1, 6}, {  8, 1, 6}, {  8, 1, 6}, {  8, 1, 6},
    {  7, 1, 6}, {  7, 1, 6}, {  7, 1, 6}, {  7, 1, 6},
    {  2, 2, 6}, {  2, 2, 6}, {  2, 2, 6}, {  2, 2, 6},
    {  6, 1, 6}, {  6, 1, 6}, {  6, 1, 6}, {  6, 1, 6},
    { 14, 1, 8}, {  1, 6, 8}, { 13, 1, 8}, { 12, 1, 8},
    {  4, 2, 8}, {  2, 3, 8}, {  1, 5, 8}, { 11, 1, 8}
};

static const DCTtab DCT_B14AC_5 [] = {
		 {  1, 3, 5}, {  5, 1, 5}, {  4, 1, 5},
    {  1, 2, 4}, {  1, 2, 4}, {  3, 1, 4}, {  3, 1, 4},
    {  2, 1, 3}, {  2, 1, 3}, {  2, 1, 3}, {  2, 1, 3},
    {129, 0, 2}, {129, 0, 2}, {129, 0, 2}, {129, 0, 2},
    {129, 0, 2}, {129, 0, 2}, {129, 0, 2}, {129, 0, 2},
    {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2},
    {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2}
};

static const DCTtab DCT_B14DC_5 [] = {
		 {  1, 3, 5}, {  5, 1, 5}, {  4, 1, 5},
    {  1, 2, 4}, {  1, 2, 4}, {  3, 1, 4}, {  3, 1, 4},
    {  2, 1, 3}, {  2, 1, 3}, {  2, 1, 3}, {  2, 1, 3},
    {  1, 1, 1}, {  1, 1, 1}, {  1, 1, 1}, {  1, 1, 1},
    {  1, 1, 1}, {  1, 1, 1}, {  1, 1, 1}, {  1, 1, 1},
    {  1, 1, 1}, {  1, 1, 1}, {  1, 1, 1}, {  1, 1, 1},
    {  1, 1, 1}, {  1, 1, 1}, {  1, 1, 1}, {  1, 1, 1}
};

static const DCTtab DCT_B15_10 [] = {
    {  6, 2, 9}, {  6, 2, 9}, { 15, 1, 9}, { 15, 1, 9},
    {  3, 4,10}, { 17, 1,10}, { 16, 1, 9}, { 16, 1, 9}
};

static const DCTtab DCT_B15_8 [] = {
    { 65, 0, 6}, { 65, 0, 6}, { 65, 0, 6}, { 65, 0, 6},
    {  8, 1, 7}, {  8, 1, 7}, {  9, 1, 7}, {  9, 1, 7},
    {  7, 1, 7}, {  7, 1, 7}, {  3, 2, 7}, {  3, 2, 7},
    {  1, 7, 6}, {  1, 7, 6}, {  1, 7, 6}, {  1, 7, 6},
    {  1, 6, 6}, {  1, 6, 6}, {  1, 6, 6}, {  1, 6, 6},
    {  5, 1, 6}, {  5, 1, 6}, {  5, 1, 6}, {  5, 1, 6},
    {  6, 1, 6}, {  6, 1, 6}, {  6, 1, 6}, {  6, 1, 6},
    {  2, 5, 8}, { 12, 1, 8}, {  1,11, 8}, {  1,10, 8},
    { 14, 1, 8}, { 13, 1, 8}, {  4, 2, 8}, {  2, 4, 8},
    {  3, 1, 5}, {  3, 1, 5}, {  3, 1, 5}, {  3, 1, 5},
    {  3, 1, 5}, {  3, 1, 5}, {  3, 1, 5}, {  3, 1, 5},
    {  2, 2, 5}, {  2, 2, 5}, {  2, 2, 5}, {  2, 2, 5},
    {  2, 2, 5}, {  2, 2, 5}, {  2, 2, 5}, {  2, 2, 5},
    {  4, 1, 5}, {  4, 1, 5}, {  4, 1, 5}, {  4, 1, 5},
    {  4, 1, 5}, {  4, 1, 5}, {  4, 1, 5}, {  4, 1, 5},
    {  2, 1, 3}, {  2, 1, 3}, {  2, 1, 3}, {  2, 1, 3},
    {  2, 1, 3}, {  2, 1, 3}, {  2, 1, 3}, {  2, 1, 3},
    {  2, 1, 3}, {  2, 1, 3}, {  2, 1, 3}, {  2, 1, 3},
    {  2, 1, 3}, {  2, 1, 3}, {  2, 1, 3}, {  2, 1, 3},
    {  2, 1, 3}, {  2, 1, 3}, {  2, 1, 3}, {  2, 1, 3},
    {  2, 1, 3}, {  2, 1, 3}, {  2, 1, 3}, {  2, 1, 3},
    {  2, 1, 3}, {  2, 1, 3}, {  2, 1, 3}, {  2, 1, 3},
    {  2, 1, 3}, {  2, 1, 3}, {  2, 1, 3}, {  2, 1, 3},
    {129, 0, 4}, {129, 0, 4}, {129, 0, 4}, {129, 0, 4},
    {129, 0, 4}, {129, 0, 4}, {129, 0, 4}, {129, 0, 4},
    {129, 0, 4}, {129, 0, 4}, {129, 0, 4}, {129, 0, 4},
    {129, 0, 4}, {129, 0, 4}, {129, 0, 4}, {129, 0, 4},
    {  1, 3, 4}, {  1, 3, 4}, {  1, 3, 4}, {  1, 3, 4},
    {  1, 3, 4}, {  1, 3, 4}, {  1, 3, 4}, {  1, 3, 4},
    {  1, 3, 4}, {  1, 3, 4}, {  1, 3, 4}, {  1, 3, 4},
    {  1, 3, 4}, {  1, 3, 4}, {  1, 3, 4}, {  1, 3, 4},
    {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2},
    {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2},
    {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2},
    {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2},
    {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2},
    {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2},
    {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2},
    {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2},
    {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2},
    {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2},
    {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2},
    {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2},
    {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2},
    {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2},
    {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2},
    {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2}, {  1, 1, 2},
    {  1, 2, 3}, {  1, 2, 3}, {  1, 2, 3}, {  1, 2, 3},
    {  1, 2, 3}, {  1, 2, 3}, {  1, 2, 3}, {  1, 2, 3},
    {  1, 2, 3}, {  1, 2, 3}, {  1, 2, 3}, {  1, 2, 3},
    {  1, 2, 3}, {  1, 2, 3}, {  1, 2, 3}, {  1, 2, 3},
    {  1, 2, 3}, {  1, 2, 3}, {  1, 2, 3}, {  1, 2, 3},
    {  1, 2, 3}, {  1, 2, 3}, {  1, 2, 3}, {  1, 2, 3},
    {  1, 2, 3}, {  1, 2, 3}, {  1, 2, 3}, {  1, 2, 3},
    {  1, 2, 3}, {  1, 2, 3}, {  1, 2, 3}, {  1, 2, 3},
    {  1, 4, 5}, {  1, 4, 5}, {  1, 4, 5}, {  1, 4, 5},
    {  1, 4, 5}, {  1, 4, 5}, {  1, 4, 5}, {  1, 4, 5},
    {  1, 5, 5}, {  1, 5, 5}, {  1, 5, 5}, {  1, 5, 5},
    {  1, 5, 5}, {  1, 5, 5}, {  1, 5, 5}, {  1, 5, 5},
    { 10, 1, 7}, { 10, 1, 7}, {  2, 3, 7}, {  2, 3, 7},
    { 11, 1, 7}, { 11, 1, 7}, {  1, 8, 7}, {  1, 8, 7},
    {  1, 9, 7}, {  1, 9, 7}, {  1,12, 8}, {  1,13, 8},
    {  3, 3, 8}, {  5, 2, 8}, {  1,14, 8}, {  1,15, 8}
};


static const MBAtab MBA_5 [] = {
		    {6, 5}, {5, 5}, {4, 4}, {4, 4}, {3, 4}, {3, 4},
    {2, 3}, {2, 3}, {2, 3}, {2, 3}, {1, 3}, {1, 3}, {1, 3}, {1, 3},
    {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1},
    {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}
};

static const MBAtab MBA_11 [] = {
    {32, 11}, {31, 11}, {30, 11}, {29, 11},
    {28, 11}, {27, 11}, {26, 11}, {25, 11},
    {24, 11}, {23, 11}, {22, 11}, {21, 11},
    {20, 10}, {20, 10}, {19, 10}, {19, 10},
    {18, 10}, {18, 10}, {17, 10}, {17, 10},
    {16, 10}, {16, 10}, {15, 10}, {15, 10},
    {14,  8}, {14,  8}, {14,  8}, {14,  8},
    {14,  8}, {14,  8}, {14,  8}, {14,  8},
    {13,  8}, {13,  8}, {13,  8}, {13,  8},
    {13,  8}, {13,  8}, {13,  8}, {13,  8},
    {12,  8}, {12,  8}, {12,  8}, {12,  8},
    {12,  8}, {12,  8}, {12,  8}, {12,  8},
    {11,  8}, {11,  8}, {11,  8}, {11,  8},
    {11,  8}, {11,  8}, {11,  8}, {11,  8},
    {10,  8}, {10,  8}, {10,  8}, {10,  8},
    {10,  8}, {10,  8}, {10,  8}, {10,  8},
    { 9,  8}, { 9,  8}, { 9,  8}, { 9,  8},
    { 9,  8}, { 9,  8}, { 9,  8}, { 9,  8},
    { 8,  7}, { 8,  7}, { 8,  7}, { 8,  7},
    { 8,  7}, { 8,  7}, { 8,  7}, { 8,  7},
    { 8,  7}, { 8,  7}, { 8,  7}, { 8,  7},
    { 8,  7}, { 8,  7}, { 8,  7}, { 8,  7},
    { 7,  7}, { 7,  7}, { 7,  7}, { 7,  7},
    { 7,  7}, { 7,  7}, { 7,  7}, { 7,  7},
    { 7,  7}, { 7,  7}, { 7,  7}, { 7,  7},
    { 7,  7}, { 7,  7}, { 7,  7}, { 7,  7}
};
