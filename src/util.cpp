#include <string>
#include "d3v/util.hpp"
#include "d3v/error.hpp"

int d3v::readline(GIOChannel *chan, std::string &ret)
{
    int err = 0;
    char *str = NULL;
    size_t length, end;
    GError *error = NULL;
    ret.clear();
    
    GIOStatus status = g_io_channel_read_line(chan, &str, &length, &end, &error);
    if (status == G_IO_STATUS_EOF)
        return 1;
    else if (status != G_IO_STATUS_NORMAL) {
        printf ("getline: %s\n", error?error->message:"no error");
        g_free(str);
        g_clear_error(&error);
        err = -D3V_ERR_FILE_BAD_ENCODING;
    }
    str[end] = 0;
    g_strstrip(str);
    ret.assign(str);
    g_free(str);
    return err;
}
