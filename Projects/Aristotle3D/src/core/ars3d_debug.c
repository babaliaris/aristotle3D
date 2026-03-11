#include <aristotle3D/core/ars3d_debug.h>
#include <aristotle3D/core/ars3d_application.h>

#if defined(ARS3D_WINDOWS)
    #include <windows.h>
#endif


static const ars3d_char* ars3dGetFileName(const ars3d_char* p_path)
{
    const ars3d_char* filename = p_path;

    for (const ars3d_char* p = p_path; *p != '\0'; p++)
    {
        if (*p == '/' || *p == '\\')
        {
            filename = p + 1;
        }
    }

    return filename;
}


ars3d_void ars3dInitDebugConsole(ars3d_void)
{
    #if defined(ARS3D_WINDOWS)
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE) return;

        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode)) return;

        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    #endif
}



ars3d_void __ars3dColorfulDebugHelper__(
    const ars3d_char*   p_agent,
    const ars3d_char*   p_reason,
    const ars3d_char*   p_file,
    ars3d_int           p_line,
    const ars3d_char*   p_color,
    const ars3d_char*   p_fmt,
    ...
)
{
    ars3dPrintf(
        ARS3D_COLOR_R_CYAN "[%s:%s:%d] {%s} " ARS3D_COLOR_RESET,
        p_agent, ars3dGetFileName(p_file), p_line, p_reason);


    ars3dPrintf("%s", p_color);
    
    va_list args;
    va_start(args, p_fmt);
    vprintf(p_fmt, args);
    va_end(args);


    ars3dPrintf(ARS3D_COLOR_RESET "\n");
}



ars3d_void __ars3dHardAssertDebugHelper__(
    const ars3d_char*   p_agent,
    const ars3d_char*   p_reason,
    const ars3d_char*   p_file,
    ars3d_int           p_line,
    const ars3d_char*   p_color,
    const ars3d_char*   p_fmt,
    ...
)
{
    ars3dPrintf(
        ARS3D_COLOR_R_CYAN "[%s:%s:%d] {%s} " ARS3D_COLOR_RESET,
        p_agent, ars3dGetFileName(p_file), p_line, p_reason);


    ars3dPrintf("%s", p_color);
    
    va_list args;
    va_start(args, p_fmt);
    vprintf(p_fmt, args);
    va_end(args);


    ars3dPrintf(ARS3D_COLOR_RESET "\n");

    __ars3dAppDestroy__(ars3dAppGet());

    ars3dExit(1);
}




ars3d_void ars3dDebugTestMacros(ars3d_void)
{
    // Aristotle3D Logging Macros
    ARS3D_TRACE("%s", "This is a Trace");
    ARS3D_INFO("%s", "This is an Info");
    ARS3D_WARN("%s", "This is a Warning");
    ARS3D_ERROR("%s", "This is an Error");
    ARS3D_FATAL("%s", "This is a Fatal Error\n");

    // User's Logging Macros
    ARS3D_USER_TRACE("%s", "This is a Trace");
    ARS3D_USER_INFO("%s", "This is an Info");
    ARS3D_USER_WARN("%s", "This is a Warning");
    ARS3D_USER_ERROR("%s", "This is an Error");
    ARS3D_USER_FATAL("%s", "This is a Fatal Error");
}
