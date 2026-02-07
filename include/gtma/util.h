#pragma once

typedef enum {
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR
} LogLevel;

void gtmaInitConfig(const char* path);
void gtmaDestroyConfig();

const char* cfgGetResLoc();
const char* cfgLookupString(const char* key);
int cfgGetArraySize(const char* key);
int cfgLookupInt(const char* key);
float cfgLookupFloat(const char* key);
int cfgLookupBool(const char* key);

void gtmaLog(LogLevel level, const char* fmt, ...);
void gtmaPrintSystemInfo();

const char* res(const char* path);
char* getFileSrc(const char* path);
long getFileSize(const char* path);

int randBetween(int a, int b);
