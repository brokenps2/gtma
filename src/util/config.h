#pragma once

void gtmaInitConfig(const char* path);
void gtmaDestroyConfig();

const char* cfgGetResLoc();
const char* cfgLookupString(const char* key);
int cfgGetArraySize(const char* key);
int cfgLookupInt(const char* key);
float cfgLookupFloat(const char* key);
int cfgLookupBool(const char* key);
