#pragma once

void gtmaInitConfig();

void cfgSetPath(const char* newPath);

int cfgGetResX();
int cfgGetResY();
const char* cfgGetTitle();
const char* cfgGetResLoc();
const char* cfgLookupString(const char* key);
int cfgLookupInt(const char* key);
int cfgLookupBool(const char* key);
