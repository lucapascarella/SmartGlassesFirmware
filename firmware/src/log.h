
#define LOG_FATAL       0 /* Errors that have the potential to cause a system crash. */
#define LOG_ERROR       1 /* Errors that have the potential to cause incorrect behavior. */
#define LOG_WARNING     2 /* Warnings about potentially unexpected behavior or side effects. */
#define LOG_INFO        3 /* Information helpful to understanding potential errors and warnings. */
#define LOG_DEBUG       4 /* Verbose information helpful during debugging and testing. */
#define LOG_TRACE       5 /* Deep verbose information helpful during debugging and testing. */

// Allow inclusion in header files
#undef logPrint
#undef logTrace
#undef logDebug
#undef logInfo
#undef logWarning
#undef logError
#undef logFatal


#define LOG_PRINT(level, color, src, line, fmt, ...)        do{SYS_DEBUG_PRINT_EXT(level, color, src, line, fmt, ##__VA_ARGS__);}while(0);


// Expose a simple always log
#define logPrint(fmt, ...)                                  do{SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, fmt, ##__VA_ARGS__);}while(0);



#if (LOG_LEVEL >= LOG_TRACE)
    #define logTrace(fmt, ...)              do{LOG_PRINT(SYS_ERROR_DEBUG, CONSOLE_COLOR_BLUE, __FILE__, __LINE__, fmt, ##__VA_ARGS__);}while(0);
    #undef LOG_TRACE_DISABLED
    #define LOG_TRACE_ENABLED
#else
    #define logTrace(fmt, ...)        
    #undef LOG_TRACE_ENABLED
    #define LOG_TRACE_DISABLED
#endif

#if (LOG_LEVEL >= LOG_DEBUG)
    #define logDebug(fmt, ...)              do{LOG_PRINT(SYS_ERROR_DEBUG, CONSOLE_COLOR_MAGENTA, __FILE__, __LINE__, fmt, ##__VA_ARGS__);}while(0);
    #undef LOG_DEBUG_DISABLED
    #define LOG_DEBUG_ENABLED
#else
    #define logDebug(fmt, ...)
    #undef LOG_DEBUG_ENABLED
    #define LOG_DEBUG_DISABLED
#endif

#if (LOG_LEVEL >= LOG_INFO)
    #define logInfo(fmt, ...)               do{LOG_PRINT(SYS_ERROR_INFO, CONSOLE_COLOR_GREEN, __FILE__, __LINE__, fmt, ##__VA_ARGS__);}while(0);
    #undef LOG_INFO_DISABLED
    #define LOG_INFO_ENABLED
#else
    #define logInfo(fmt, ...)
    #undef LOG_INFO_ENABLED
    #define LOG_INFO_DISABLED
#endif

#if (LOG_LEVEL >= LOG_WARNING)
    #define logWarning(fmt, ...)            do{LOG_PRINT(SYS_ERROR_WARNING, CONSOLE_COLOR_YELLOW, __FILE__, __LINE__, fmt, ##__VA_ARGS__);}while(0);
    #undef LOG_WARNING_DISABLED
    #define LOG_WARNING_ENABLED
#else
    #define logWarning(fmt, ...)
    #undef LOG_WARNING_ENABLED
    #define LOG_WARNING_DISABLED
#endif

#if (LOG_LEVEL >= LOG_ERROR)
    #define logError(fmt, ...)              do{LOG_PRINT(SYS_ERROR_ERROR, CONSOLE_COLOR_RED, __FILE__, __LINE__, fmt, ##__VA_ARGS__);}while(0);
    #undef LOG_ERROR_DISABLED
    #define LOG_ERROR_ENABLED
#else
    #define logError(fmt, ...)
    #undef LOG_ERROR_ENABLED
    #define LOG_ERROR_DISABLED
#endif

#if (LOG_LEVEL >= LOG_FATAL)
    #define logFatal(fmt, ...)              do{LOG_PRINT(SYS_ERROR_FATAL, CONSOLE_COLOR_RED, __FILE__, __LINE__, fmt, ##__VA_ARGS__);}while(0);
    #undef LOG_FATAL_DISABLED
    #define LOG_FATAL_ENABLED
#else
    #define logFatal(fmt, ...)
    #undef LOG_FATAL_ENABLED
    #define LOG_FATAL_DISABLED
#endif

