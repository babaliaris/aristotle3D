#ifndef ARS3D_DEBUG_H
#define ARS3D_DEBUG_H
#include <aristotle3D/core/ars3d_stdio.h>


/**
 * @file
 * @brief Debug functionalities for Aristotle3D
 */


// --------------------|ANSI Color Codes|-------------------- //
// Reset
#define ARS3D_COLOR_RESET   "\033[0m"

// Regular Colors
#define ARS3D_COLOR_R_BLACK   "\033[0;30m"
#define ARS3D_COLOR_R_RED     "\033[0;31m"
#define ARS3D_COLOR_R_GREEN   "\033[0;32m"
#define ARS3D_COLOR_R_YELLOW  "\033[0;33m"
#define ARS3D_COLOR_R_BLUE    "\033[0;34m"
#define ARS3D_COLOR_R_MAGENTA "\033[0;35m"
#define ARS3D_COLOR_R_CYAN    "\033[0;36m"
#define ARS3D_COLOR_R_WHITE   "\033[0;37m"

// Bold / Bright Colors
#define ARS3D_COLOR_B_BLACK   "\033[1;30m"
#define ARS3D_COLOR_B_RED     "\033[1;31m"
#define ARS3D_COLOR_B_GREEN   "\033[1;32m"
#define ARS3D_COLOR_B_YELLOW  "\033[1;33m"
#define ARS3D_COLOR_B_BLUE    "\033[1;34m"
#define ARS3D_COLOR_B_MAGENTA "\033[1;35m"
#define ARS3D_COLOR_B_CYAN    "\033[1;36m"
#define ARS3D_COLOR_B_WHITE   "\033[1;37m"

// Background Colors
#define ARS3D_BG_RED          "\033[41m"
#define ARS3D_BG_YELLOW       "\033[43m"
// --------------------|ANSI Color Codes|-------------------- //


/**
 * @brief Initializes the Debug Console
 */
ARS3D_API ars3d_void ars3dInitDebugConsole(ars3d_void);


/**
 * @brief A helper function to print debug messages.
 * 
 * @param p_file The file name __FILE__
 * @param p_line The line in the file __LINE__
 * @param p_color The ansi color code string
 * @param p_fmt The printf formatted string.
 */
ARS3D_API ars3d_void __ars3dColorfulDebugHelper__(
    const ars3d_char*   p_agent,
    const ars3d_char*   p_reason,
    const ars3d_char*   p_file,
    ars3d_int           p_line,
    const ars3d_char*   p_color,
    const ars3d_char*   p_fmt,
    ...
);


/**
 * @brief Test the debug logging macros by visual inspection.
 * 
 * The user can call this to test if all the macros work correctly
 * at runtime for both debug and release builds. If not debug, then
 * this funcion should do nothing.
 */
ARS3D_API ars3d_void ars3dDebugTestMacros(ars3d_void);


#if defined(ARS3D_DEBUG)

    #define ARS3D_TRACE(fmt, ...)\
    __ars3dColorfulDebugHelper__("Aristotle3D", "TRACE", __FILE__, __LINE__, ARS3D_COLOR_R_WHITE, fmt, ##__VA_ARGS__)

    #define ARS3D_INFO(fmt, ...)\
    __ars3dColorfulDebugHelper__("Aristotle3D", "INFO", __FILE__, __LINE__, ARS3D_COLOR_R_GREEN, fmt, ##__VA_ARGS__)

    #define ARS3D_WARN(fmt, ...)\
    __ars3dColorfulDebugHelper__("Aristotle3D", "WARN", __FILE__, __LINE__, ARS3D_COLOR_R_YELLOW, fmt, ##__VA_ARGS__)

    #define ARS3D_ERROR(fmt, ...)\
    __ars3dColorfulDebugHelper__("Aristotle3D", "ERROR", __FILE__, __LINE__, ARS3D_COLOR_R_RED, fmt, ##__VA_ARGS__)

    #define ARS3D_FATAL(fmt, ...)\
    __ars3dColorfulDebugHelper__("Aristotle3D", "FATAL", __FILE__, __LINE__, ARS3D_BG_RED, fmt, ##__VA_ARGS__)


    #define ARS3D_USER_TRACE(fmt, ...)\
    __ars3dColorfulDebugHelper__("User", "TRACE", __FILE__, __LINE__, ARS3D_COLOR_R_WHITE, fmt, ##__VA_ARGS__)

    #define ARS3D_USER_INFO(fmt, ...)\
    __ars3dColorfulDebugHelper__("User", "INFO", __FILE__, __LINE__, ARS3D_COLOR_R_GREEN, fmt, ##__VA_ARGS__)

    #define ARS3D_USER_WARN(fmt, ...)\
    __ars3dColorfulDebugHelper__("User", "WARN", __FILE__, __LINE__, ARS3D_COLOR_R_YELLOW, fmt, ##__VA_ARGS__)

    #define ARS3D_USER_ERROR(fmt, ...)\
    __ars3dColorfulDebugHelper__("User", "ERROR", __FILE__, __LINE__, ARS3D_COLOR_R_RED, fmt, ##__VA_ARGS__)

    #define ARS3D_USER_FATAL(fmt, ...)\
    __ars3dColorfulDebugHelper__("User", "FATAL", __FILE__, __LINE__, ARS3D_BG_RED, fmt, ##__VA_ARGS__)


    #define ARS3D_ASSERT_SOFT(cond, ...)\
    do\
    {\
        if (!(cond))\
        {\
            __ars3dColorfulDebugHelper__(\
                "Aristotle3D", "ASSERT", __FILE__, __LINE__, ARS3D_COLOR_B_RED, \
                "Condition failed: [" #cond "] " __VA_ARGS__\
            );\
        }\
    } while (0)

    #define ARS3D_USER_ASSERT_SOFT(cond, ...)\
    do\
    {\
        if (!(cond))\
        {\
            __ars3dColorfulDebugHelper__(\
                "Aristotle3D", "ASSERT", __FILE__, __LINE__, ARS3D_COLOR_B_RED, \
                "Condition failed: [" #cond "] " __VA_ARGS__\
            );\
        }\
    } while (0)


#else
    #define ARS3D_TRACE(fmt, ...)
    #define ARS3D_INFO(fmt, ...)
    #define ARS3D_WARN(fmt, ...)
    #define ARS3D_ERROR(fmt, ...)
    #define ARS3D_FATAL(fmt, ...)


    #define ARS3D_USER_TRACE(fmt, ...)
    #define ARS3D_USER_INFO(fmt, ...)
    #define ARS3D_USER_WARN(fmt, ...)
    #define ARS3D_USER_ERROR(fmt, ...)
    #define ARS3D_USER_FATAL(fmt, ...)


    #define ARS3D_ASSERT_SOFT(cond, ...)
    #define ARS3D_USER_ASSERT_SOFT(cond, ...)
#endif


#endif