#pragma once


char *format_string(char *buf, size_t n, const char *format, ...);

char *format_bitrate(char *buf, size_t n, int64_t r);

char *format_duration(char *buf, size_t n, int64_t d);

char *format_framerate(char *buf, size_t n, double r);

char *format_basenumber(char *buf, size_t n, double r);

char *format_dimensions(char *buf, size_t n, int w, int h);
