#ifndef D3V_ERROR_H
#define D3V_ERROR_H

#define D3V_ERRORS(ERROR)                       \
    ERROR(WAVEFRONT_INVALID_LINE)               \
    ERROR(WAVEFRONT_MODEL_UNEXPECTED_OBJECT)    \
    ERROR(FILE_BAD_ENCODING)                    \

#define ERROR_TO_ENUM(error)                    \
    D3V_ERR_##error,

enum d3v_error_type {
    D3V_ERRORS(ERROR_TO_ENUM)
    MAX_ERROR
};

void d3v_warning(char const *message, ...);
void d3v_error(char const *message, ...);

#endif //ERROR_H
