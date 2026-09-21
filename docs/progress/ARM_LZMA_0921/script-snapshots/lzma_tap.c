#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <lzma.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

/* Independent diagnostic DSO. No modification of the liblzma stream or result.
 * One append write per call; no stdio file streams or per-call malloc.
 * errno after the real call is restored after every diagnostic operation.
 */
static lzma_ret (*real_code)(lzma_stream *, lzma_action);
static int trace_fd = -1;
static unsigned long long sequence;
static unsigned int error_snapshots;
static char output_base[256];
static char copy_buffer[16384];

static void write_all(int fd, const char *p, size_t n)
{
    while (n) {
        ssize_t done = write(fd, p, n);
        if (done < 0 && errno == EINTR) continue;
        if (done <= 0) return;
        p += done; n -= (size_t)done;
    }
}

static void snapshot_file(const char *input, const char *label, const char *part)
{
    char path[512];
    snprintf(path, sizeof(path), "%s.%s.%s", output_base, label, part);
    int source = open(input, O_RDONLY | O_CLOEXEC);
    int target = open(path, O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, 0600);
    if (source >= 0 && target >= 0) {
        ssize_t n;
        while ((n = read(source, copy_buffer, sizeof(copy_buffer))) > 0)
            write_all(target, copy_buffer, (size_t)n);
    }
    if (source >= 0) close(source);
    if (target >= 0) close(target);
}

static void snapshot(const char *label)
{
    snapshot_file("/proc/self/status", label, "status");
    snapshot_file("/proc/self/maps", label, "maps");
    snapshot_file("/proc/self/limits", label, "limits");
}

__attribute__((constructor)) static void initialize(void)
{
    int saved = errno;
    const char *base = getenv("LZMA_TAP_PREFIX");
    const char *va = getenv("QEMU_RESERVED_VA");
    char line[1024];
    if (!base) base = "/tmp/lzma-diag/rpmbuild-lzma";
    snprintf(output_base, sizeof(output_base), "%s", base);
    char path[512];
    snprintf(path, sizeof(path), "%s.calls.tsv", output_base);
    trace_fd = open(path, O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, 0600);
    if (trace_fd < 0) {
        const char msg[] = "LZMA_TAP_SETUP_FAILED: cannot create exclusive call log\n";
        write_all(2, msg, sizeof(msg)-1); _exit(125);
    }
    *(void **)(&real_code) = dlsym(RTLD_NEXT, "lzma_code");
    if (!real_code) {
        const char msg[] = "LZMA_TAP_SETUP_FAILED: RTLD_NEXT lzma_code unavailable\n";
        write_all(trace_fd, msg, sizeof(msg)-1); _exit(125);
    }
    struct rlimit as, data;
    getrlimit(RLIMIT_AS, &as); getrlimit(RLIMIT_DATA, &data);
    int n = snprintf(line, sizeof(line),
        "# pid=%ld QEMU_RESERVED_VA=%s pointer_bits=%zu tap=%p real=%p brk=%p rlimit_as=%llu rlimit_data=%llu\n",
        (long)getpid(), va ? va : "NOT_OBSERVED", 8*sizeof(void *),
        (void *)&lzma_code, (void *)real_code, sbrk(0),
        (unsigned long long)as.rlim_cur, (unsigned long long)data.rlim_cur);
    write_all(trace_fd, line, (size_t)n);
    const char header[] = "seq\tmonotonic_sec\tmonotonic_nsec\tstream\taction\tret\terrno_before\terrno_after\tin_before\tout_before\tin_after\tout_after\ttotal_in\ttotal_out\n";
    write_all(trace_fd, header, sizeof(header)-1);
    /* Scope preload to this process; do not instrument the build's children. */
    unsetenv("LD_PRELOAD"); unsetenv("QEMU_SET_ENV"); unsetenv("LZMA_TAP_PREFIX");
    snapshot("initial");
    errno = saved;
}

lzma_ret lzma_code(lzma_stream *stream, lzma_action action)
{
    int before_errno = errno;
    size_t input = stream->avail_in, output = stream->avail_out;
    /* No logging or allocation between the saved entry state and real call. */
    lzma_ret ret = real_code(stream, action);
    int after_errno = errno;
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    char line[512];
    unsigned long long seq = ++sequence;
    int n = snprintf(line, sizeof(line),
        "%llu\t%lld\t%ld\t%p\t%d\t%d\t%d\t%d\t%zu\t%zu\t%zu\t%zu\t%llu\t%llu\n",
        seq, (long long)now.tv_sec, now.tv_nsec, (void *)stream,
        (int)action, (int)ret, before_errno, after_errno,
        input, output, stream->avail_in, stream->avail_out,
        (unsigned long long)stream->total_in, (unsigned long long)stream->total_out);
    write_all(trace_fd, line, (size_t)n);
    /* STREAM_END is normal termination, not an encoder error. */
    if (ret != LZMA_OK && ret != LZMA_STREAM_END && error_snapshots++ == 0)
        snapshot("first-non-ok-non-end");
    errno = after_errno;
    return ret;
}
