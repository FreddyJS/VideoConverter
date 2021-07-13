#include <logs.hpp>
#include <stdarg.h>

void vclog(int log_type, const char *fmt, ...)
{
    va_list args;

    switch (log_type)
    {
    case VCLOGINFO:
        fprintf( stdout, "[inf] " );
        break;
    
    case VCLOGWARNING:
        fprintf( stdout, "[war] " );
        break;

    case VCLOGERROR:
        fprintf( stdout, "[err] " );
        break;

    case VCLOGLIBAV:
        fprintf( stdout, "[lav] " );
        break;

    default:
        break;
    }

    va_start( args, fmt );
    vfprintf( stderr, fmt, args );
    va_end( args );
    fprintf( stderr, "\n" );
}