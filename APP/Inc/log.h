#pragma once

typedef enum
{
    LOG_LEVEL_NONE    = 0,
    LOG_LEVEL_ERROR   = 1,
    LOG_LEVEL_WARNING = 2,
    LOG_LEVEL_INFO    = 3,
    LOG_LEVEL_DEBUG   = 4
}LOG_LEVEL;

void log_init(void);

// '__attribute__' give compiler specific extra attribute 
void log_print(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

#if LOG_LEVEL >= LOG_LEVEL_ERROR
//## invalidate the ',' if the variable parameters is null
#define LOG_ERROR(fmt, ...) log_print("ERROR: " fmt "\r\n", ##__VA_ARGS__)
#else
#define LOG_ERROR(fmt, ...) ((void)0)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define LOG_WARNING(fmt, ...) log_print("WARNING: " fmt "\r\n", ##__VA_ARGS__)
#else
#define LOG_WARNING(fmt, ...) ((void)0)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_INFO(fmt, ...) log_print("INFO: " fmt "\r\n", ##__VA_ARGS__)
#else
#define LOG_INFO(fmt, ...) ((void)0)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_DEBUG(fmt, ...) log_print("DEBUG: " fmt "\r\n", ##__VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...) ((void)0)
#endif
