#include "logger.h"
#include <memory.h>
#include <stdarg.h>
#include <string>

const char *levelStrings[6] = {
    "[FATAL]", "[ERROR]", "[WARN]", "[INFO]", "[DEBUG]", "[TRACE]",
};

const char *levelColorCodes[6] = {
    "\033[0;31m", // Red
    "\033[0;35m", // Purple
    "\033[0;33m", // yellow
    "\033[0;37m", // white
    "\033[0;36m", // Cyan
    "\033[0;34m", // blue
};

void log(LogLevel level, const char *message, ...);

void log(LogLevel level, const char *message, ...) {
  bool is_error = level < LOG_LEVEL_WARN;

  const int msg_length = 32000;
  char out_message[msg_length];
  memset(out_message, 0, sizeof(out_message));

  // Format message
  __builtin_va_list arg_ptr;
  va_start(arg_ptr, message);
  vsnprintf(out_message, msg_length, message, arg_ptr);
  va_end(arg_ptr);

  char out_message2[msg_length];
  sprintf(out_message2, "%s%s %s\033[0;37m\n", levelColorCodes[level], levelStrings[level], out_message);

  printf("%s", out_message2);
};