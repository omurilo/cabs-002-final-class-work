/* Minimal PNG writer (public domain) - supports 8-bit RGBA only.
 * This replaces the previous stub and provides a functional encoder
 * without external dependencies. It writes non-compressed deflate blocks
 * inside a zlib stream (can be larger than optimal but acceptable for
 * moderate frame counts). Segments data in 64KB chunks. */
#ifndef STB_IMAGE_WRITE_MIN_PNG
#define STB_IMAGE_WRITE_MIN_PNG
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static uint32_t crc32_table[256];
static int crc32_initialized = 0;
static void crc32_init() {
    if (crc32_initialized) return;
    for (uint32_t i=0;i<256;++i){
        uint32_t c=i;
        for (int k=0;k<8;++k){ c = (c & 1)?(0xEDB88320U ^ (c>>1)):(c>>1); }
        crc32_table[i]=c;
    }
    crc32_initialized=1;
}
static uint32_t crc32_calc(const unsigned char* buf, size_t len){
    crc32_init();
    uint32_t c=0xFFFFFFFFU;
    for (size_t i=0;i<len;++i) c = crc32_table[(c ^ buf[i]) & 0xFF] ^ (c >> 8);
    return c ^ 0xFFFFFFFFU;
}

static uint32_t adler32_calc(const unsigned char* data, size_t len){
    uint32_t a=1, b=0;
    const uint32_t MOD=65521U;
    for (size_t i=0;i<len;++i){ a=(a+data[i])%MOD; b=(b+a)%MOD; }
    return (b<<16)|a;
}

static void write_u32_be(FILE* f, uint32_t v){
    unsigned char b[4]={(unsigned char)(v>>24),(unsigned char)(v>>16),(unsigned char)(v>>8),(unsigned char)v};
    fwrite(b,1,4,f);
}

static int write_chunk(FILE* f, const char* type, const unsigned char* data, size_t len){
    write_u32_be(f,(uint32_t)len);
    unsigned char header[4];
    memcpy(header,type,4);
    fwrite(header,1,4,f);
    if (len) fwrite(data,1,len,f);
    uint32_t crc_init_len = 4 + (uint32_t)len;
    unsigned char* tmp = (unsigned char*)malloc(crc_init_len);
    if (!tmp) return 0;
    memcpy(tmp,header,4);
    if (len) memcpy(tmp+4,data,len);
    uint32_t crc = crc32_calc(tmp, crc_init_len);
    free(tmp);
    write_u32_be(f,crc);
    return 1;
}

// Main function
static int stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes){
    if (comp != 4 || w<=0 || h<=0 || !data) return 0; // only RGBA supported
    FILE* f = fopen(filename, "wb");
    if (!f) return 0;
    // PNG signature
    static const unsigned char sig[8] = {0x89,'P','N','G','\r','\n',0x1A,'\n'};
    fwrite(sig,1,8,f);
    // IHDR
    unsigned char ihdr[13];
    ihdr[0]=(unsigned char)(w>>24); ihdr[1]=(unsigned char)(w>>16); ihdr[2]=(unsigned char)(w>>8); ihdr[3]=(unsigned char)w;
    ihdr[4]=(unsigned char)(h>>24); ihdr[5]=(unsigned char)(h>>16); ihdr[6]=(unsigned char)(h>>8); ihdr[7]=(unsigned char)h;
    ihdr[8]=8; // bit depth
    ihdr[9]=6; // color type RGBA
    ihdr[10]=0; ihdr[11]=0; ihdr[12]=0; // compression, filter, interlace
    if (!write_chunk(f,"IHDR",ihdr,13)){ fclose(f); return 0; }
    // Prepare raw scanlines with filter byte 0
    size_t row_bytes = (size_t)w * 4;
    size_t scanline_bytes = row_bytes + 1; // + filter byte
    size_t raw_size = scanline_bytes * (size_t)h;
    unsigned char* raw = (unsigned char*)malloc(raw_size);
    if (!raw){ fclose(f); return 0; }
    const unsigned char* src = (const unsigned char*)data;
    for (int y=0;y<h;++y){
        raw[y*scanline_bytes] = 0; // filter type 0
        memcpy(&raw[y*scanline_bytes + 1], src + y*stride_in_bytes, row_bytes);
    }
    // Build zlib stream with uncompressed blocks
    // Worst-case size: 2 (header) + raw_size + raw_size/65535*5 + 4 (adler)
    size_t est_blocks = raw_size / 65535 + 1;
    size_t zcap = 2 + raw_size + est_blocks*5 + 4;
    unsigned char* zdata = (unsigned char*)malloc(zcap);
    if (!zdata){ free(raw); fclose(f); return 0; }
    size_t zlen=0;
    // zlib header (CMF/FLG) -> 0x78 0x01 (no compression, fastest algo, valid FCHECK)
    zdata[zlen++] = 0x78; zdata[zlen++] = 0x01;
    size_t remaining = raw_size;
    size_t offset = 0;
    while (remaining > 0){
        unsigned block_len = remaining > 65535 ? 65535 : (unsigned)remaining;
        int is_final = (remaining <= 65535);
        // BFINAL (1 bit) + BTYPE=00 (uncompressed) => low 3 bits: (is_final ? 1 : 0)
        unsigned char bheader = (unsigned char)(is_final ? 0x01 : 0x00);
        zdata[zlen++] = bheader;
        // LEN and NLEN little-endian
        zdata[zlen++] = (unsigned char)(block_len & 0xFF);
        zdata[zlen++] = (unsigned char)((block_len >> 8) & 0xFF);
        unsigned nlen = ~block_len;
        zdata[zlen++] = (unsigned char)(nlen & 0xFF);
        zdata[zlen++] = (unsigned char)((nlen >> 8) & 0xFF);
        memcpy(zdata + zlen, raw + offset, block_len);
        zlen += block_len;
        offset += block_len;
        remaining -= block_len;
    }
    uint32_t adler = adler32_calc(raw, raw_size);
    zdata[zlen++] = (unsigned char)(adler >> 24);
    zdata[zlen++] = (unsigned char)(adler >> 16);
    zdata[zlen++] = (unsigned char)(adler >> 8);
    zdata[zlen++] = (unsigned char)(adler);
    free(raw);
    if (!write_chunk(f,"IDAT", zdata, zlen)){ free(zdata); fclose(f); return 0; }
    free(zdata);
    // IEND
    if (!write_chunk(f,"IEND", NULL, 0)){ fclose(f); return 0; }
    fclose(f);
    return 1;
}

// Public prototype
extern int stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes);

#ifdef __cplusplus
}
#endif

#endif // STB_IMAGE_WRITE_MIN_PNG
