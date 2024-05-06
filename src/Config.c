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


const char* cfgGetVertexShaderPath() {
  
    const char* vs;
    
    if(config_lookup_string(&cfg, "vtShaderPath", &vs)) {
        return vs;
    } else {
        printf("vtShaderPath setting not found, please set vertex shader path in configuration file.\n");
        exit(1);
    }
}

const char* cfgGetFragmentShaderPath() {

    const char* fs;
  
    if(config_lookup_string(&cfg, "frShaderPath", &fs)) {
        return fs;
    } else {
        printf("frShaderPath setting not found, please set fragment shader path in configuration file.\n");
        exit(1);
    }

}
