#include "Files.h"
#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include <string.h>

const char* configPath;
char* contents;

cJSON* json;

void gtmaInitConfig(const char* path) {
    
    configPath = path;
    contents = malloc((getFileSize(configPath) * sizeof(char)));
    contents = getFileSrc(configPath);

    if(!contents) {
        printf("config file of path %s contents is NULL\n", path);
        exit(1);
    }

    json = cJSON_Parse(contents);

    if(!json) {
        printf("error reading config file %s\n", path);
        exit(1);
    }

}

const char* cfgGetResLoc() {

    cJSON* resLocPath = cJSON_GetObjectItem(json, "resLoc");

    if(cJSON_IsString(resLocPath)) {
        return resLocPath->valuestring;
    } else {
        printf("error reading config file value \"resLoc\"\n");
        exit(1);
    }
}

const char* cfgLookupString(const char* key) {

    cJSON* value = cJSON_GetObjectItem(json, key);
 
    if(cJSON_IsString(value)) {
        return value->valuestring;
    } else {
        printf("error reading config file value \"%s\"\n", key);
        exit(1);
    }

}

int cfgLookupInt(const char* key) {
    cJSON* value = cJSON_GetObjectItem(json, key);
    
    if(cJSON_IsNumber(value)) {
        return value->valueint;
    } else {
        printf("error reading config file value \"%s\"\n", key);
        exit(1);
    }

}

float cfgLookupFloat(const char* key) {

    cJSON* value = cJSON_GetObjectItem(json, key);
 
    if(cJSON_IsNumber(value)) {
        return value->valuedouble;
    } else {
        printf("error reading config file value \"%s\"\n", key);
        exit(1);
    }

}

int cfgLookupBool(const char* key) {
    cJSON* value = cJSON_GetObjectItem(json, key);
 
    if(cJSON_IsBool(value)) {
        return value->valueint;
    } else {
        printf("error reading config file value \"%s\"\n", key);
        exit(1);
    }

}

void gtmaDestroyConfig() {
    free(contents);
    cJSON_Delete(json);
}
