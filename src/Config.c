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

cJSON* cfgGetNestedItem(const char* key) {
    char keyCopy[256];
    strncpy(keyCopy, key, sizeof(keyCopy) - 1);
    keyCopy[sizeof(keyCopy) - 1] = '\0';
    
    char* token = strtok(keyCopy, ".");
    cJSON* item = json;
    
    while (token && item) {
        char* bracket = strchr(token, '[');
        if (bracket) {
            *bracket = '\0';
            cJSON* array = cJSON_GetObjectItem(item, token);
            if (cJSON_IsArray(array)) {
                int index = atoi(bracket + 1);
                item = cJSON_GetArrayItem(array, index);
            } else {
                return NULL;
            }
        } else {
            item = cJSON_GetObjectItem(item, token);
        }
        token = strtok(NULL, ".");
    }
    
    return item;
}

int cfgGetArraySize(const char* key) {
    cJSON* value = cfgGetNestedItem(key);
    if(cJSON_IsArray(value)) {
        return cJSON_GetArraySize(value);
    }
    printf("error reading config file value size \"%s\"\n", key);
    exit(1);
}

const char* cfgLookupString(const char* key) {
    cJSON* value = cfgGetNestedItem(key);
    if (cJSON_IsString(value)) {
        return value->valuestring;
    }
    printf("error reading config file value \"%s\"\n", key);
    exit(1);
}

int cfgLookupInt(const char* key) {
    cJSON* value = cfgGetNestedItem(key);
    if (cJSON_IsNumber(value)) {
        return value->valueint;
    }
    printf("error reading config file value \"%s\"\n", key);
    exit(1);
}

float cfgLookupFloat(const char* key) {
    cJSON* value = cfgGetNestedItem(key);
    if (cJSON_IsNumber(value)) {
        return (float)value->valuedouble;
    }
    printf("error reading config file value \"%s\"\n", key);
    exit(1);
}

int cfgLookupBool(const char* key) {
    cJSON* value = cfgGetNestedItem(key);
    if (cJSON_IsBool(value)) {
        return value->valueint;
    }
    printf("error reading config file value \"%s\"\n", key);
    exit(1);
}


void gtmaDestroyConfig() {
    free(contents);
    cJSON_Delete(json);
}
