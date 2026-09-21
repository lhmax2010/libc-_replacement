#include <errno.h>
#include <lzma.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct control { int fail; };
static void *allocate(void *opaque, size_t n, size_t s) {
    struct control *c = opaque;
    return c->fail ? NULL : calloc(n, s);
}
static void release(void *opaque, void *p) { (void)opaque; free(p); }
static unsigned char input[131072], output[128];
int main(int argc, char **argv) {
    if (argc != 3) return 2;
    struct control c = {0};
    lzma_allocator allocator = {allocate, release, &c};
    lzma_stream strm = LZMA_STREAM_INIT;
    strm.allocator = &allocator;
    lzma_mt options = {0};
    options.threads=1; options.preset=5; options.check=LZMA_CHECK_CRC64;
    lzma_ret init = lzma_stream_encoder_mt(&strm, &options);
    printf("init=%d mode=%s\n", init, argv[1]);
    if (init != LZMA_OK) return 3;
    for (size_t i=0;i<sizeof(input);++i) input[i]=(unsigned char)((i*17+i/113)%256);
    strm.next_in=input; strm.avail_in=sizeof(input);
    FILE *f=fopen(argv[2],"wb"); if (!f) return 4;
    if (!strcmp(argv[1],"memerror")) c.fail=1;
    unsigned seq=0;
    lzma_ret ret;
    do {
        strm.next_out=output; strm.avail_out=sizeof(output);
        errno=38;
        ret=lzma_code(&strm,!strcmp(argv[1],"progerror") ? (lzma_action)999 : LZMA_FINISH);
        int e=errno;
        printf("call=%u ret=%d errno=%d in=%zu out=%zu total_in=%llu total_out=%llu\n",
            ++seq,ret,e,strm.avail_in,strm.avail_out,
            (unsigned long long)strm.total_in,(unsigned long long)strm.total_out);
        fwrite(output,1,sizeof(output)-strm.avail_out,f);
    } while (ret==LZMA_OK && seq<10000);
    fclose(f); lzma_end(&strm);
    if (!strcmp(argv[1],"normal")) return ret==LZMA_STREAM_END ? 0 : 5;
    if (!strcmp(argv[1],"memerror")) return ret==LZMA_MEM_ERROR ? 0 : 6;
    return ret==LZMA_PROG_ERROR ? 0 : 7;
}
