#pragma once

typedef enum {
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR
} LogLevel;

void gtmaLog(LogLevel level, const char* fmt, ...);
void gtmaPrintSystemInfo();
