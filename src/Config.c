#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>

const char* path;
config_t cfg;

void initConfig() {
    config_init(&cfg);
    if(!config_read_file(&cfg, path)) {
        printf("Unable to read config file\n");
        exit(1);
    }
}

void cfgSetPath(const char* spath) {
    path = spath;
}

int cfgGetResX() {

    int rx;

    if(config_lookup_int(&cfg, "resX", &rx)) {
        return rx;
    } else {
        printf("resX setting not found, defualting to 800.\n");
        return 800;
    }
}

int cfgGetResY() {

    int ry;

    if(config_lookup_int(&cfg, "resY", &ry)) {
        return ry;
    } else {
        printf("resY setting not found, defualting to 600.\n");
        return 600;
    }
}

const char* cfgGetTitle() {

    const char* tt;
    
    if(config_lookup_string(&cfg, "title", &tt)) {
        return tt;
    } else {
        printf("title setting not found, defualting.\n");
        return "gtma";
    }

}

const char* cfgGetResLoc() {

    char* pathToResLoc;

    if(config_lookup_string(&cfg, "resources", (const char**)&pathToResLoc)) {
        return pathToResLoc;
    } else {
        printf("No resource directory specified in config file, exiting.\n");
        exit(1);
    }
}
