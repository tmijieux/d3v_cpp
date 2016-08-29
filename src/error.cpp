#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <glib/gi18n.h>

#include "d3v/error.hpp"

void d3v_warning(char const *message, ...)
{
    fprintf(stderr, _("D3V-WARNING: "));

    va_list ap;
    va_start(ap, message);
    vfprintf(stderr, message, ap);
    va_end(ap);
}

void d3v_error(char const *message, ...)
{
    fprintf(stderr, _("D3V-ERROR: "));
    
    va_list ap;
    va_start(ap, message);
    vfprintf(stderr, message, ap);
    va_end(ap);
}
