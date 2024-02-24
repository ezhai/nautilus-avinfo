#include <glib.h>
#include <libavutil/avutil.h>

#include "format.h"

char *
format_string(char *buf, size_t n, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    g_vsnprintf(buf, n, format, args);
    return buf;
}


char *
format_bitrate(char *buf, size_t n, int64_t r)
{
    g_snprintf(buf, n, "%.2f kbps", r /(float)1000);
    return buf;
}

char *
format_duration(char *buf, size_t n, int64_t d)
{
    int64_t h, m, s;
    h = d / ((int64_t)60 * (int64_t)60 * AV_TIME_BASE);
    d = d % ((int64_t)60 * (int64_t)60 * AV_TIME_BASE);
    m = d / ((int64_t)60 * AV_TIME_BASE);
    d = d % ((int64_t)60 * AV_TIME_BASE);
    s = d / (AV_TIME_BASE);
    snprintf(buf, n, "%01ld:%02ld:%02ld", h, m, s);
    return buf;
}


char *
format_framerate(char *buf, size_t n, double r)
{
    snprintf(buf, n, "%.1f fps", r);
    return buf;
}

char *
format_basenumber(char *buf, size_t n, double r)
{
    snprintf(buf, n, "%.1fk tbn", r / 1000);
    return buf;
}

char *
format_dimensions(char *buf, size_t n, int w, int h) {
    snprintf(buf, n, "%dx%d", w, h);
    return buf;
}
