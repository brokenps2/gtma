#include <iostream>
#include <string>
#include <libconfig.h++>

using namespace std;
using namespace libconfig;

string path;

int resX;
int resY;
string vtShaderPath;
string frShaderPath;
string title;

string os;

void cfgSetPath(string newPath) {
    path = newPath;
}

void cfgGetOS() {
    #ifdef _WIN32
    os = "Windows 32-bit";
    #elif _WIN64
    os = "Windows 64-bit";
    #elif __APPLE__ || __MACH__
    os = "macOS/OSX";
    #elif __linux__
    os = "Linux";
    #elif __FreeBSD__
    os = "FreeBSD";
    #elif __unix || __unix__
    os = "Unix";
    #else
    os = "Other";
    #endif
}

void cfgSetResX() {
  
    Config cfg;

    try {
        cfg.readFile(path);
    }
    catch(const FileIOException &fioex) {
        std::cerr << "I/O Error Reading configuration file" << std::endl;
        exit(1);
    }
    catch(const ParseException &pex) {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                << " - " << pex.getError() << std::endl;
        exit(1);
    }

    try {
        int sx = cfg.lookup("resX");
        resX = sx;
    }
    catch(const SettingNotFoundException &nfex) {
        cerr << "No 'resX' setting in configuration file. Defaulting to 800" << endl;
        resX = 800;
    }
}

int cfgGetResX() {
    return resX;
}

int cfgSetResY() {
  
    Config cfg;

    try {
        cfg.readFile(path);
    }
    catch(const FileIOException &fioex) {
        std::cerr << "I/O error while reading configuration file." << std::endl;
        exit(1);
    }
    catch(const ParseException &pex) {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                << " - " << pex.getError() << std::endl;
        exit(1);
    }

    try {
        int sy = cfg.lookup("resY");
        resY = sy;
    }
    catch(const SettingNotFoundException &nfex) {
        cerr << "No 'resY' setting in configuration file." << endl;
        resY = 600;
    }

    return 1;

}

int cfgGetResY() {
    return resY;
}

void cfgSetTitle() {
  
    Config cfg;

    try {
        cfg.readFile(path);
    }
    catch(const FileIOException &fioex) {
        std::cerr << "I/O error while reading configuration file." << std::endl;
        exit(1);
    }
    catch(const ParseException &pex) {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                << " - " << pex.getError() << std::endl;
        exit(1);
    }

    try {
        string st = cfg.lookup("title");
        title = st;
    }
    catch(const SettingNotFoundException &nfex) {
        cerr << "No 'name' setting in configuration file." << endl;
        title = "gtma";
    }

}

string cfgGetTitle() {
    return title;
}

void cfgSetVertexShaderPath() {
  
    Config cfg;

    try {
        cfg.readFile(path);
    }
    catch(const FileIOException &fioex) {
        std::cout << path << std::endl;
        std::cerr << "I/O error while reading configuration file." << std::endl;
        exit(1);
    }
    catch(const ParseException &pex) {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                << " - " << pex.getError() << std::endl;
        exit(1);
    }

    try {
        string st = cfg.lookup("vtShaderPath");
        vtShaderPath = st;
    }
    catch(const SettingNotFoundException &nfex) {
        cerr << "No 'vtShaderPath' setting in configuration file." << endl;
        exit(1);
    }

}

string cfgGetVertexShaderPath() {
    return vtShaderPath;
}

void cfgSetFragmentShaderPath() {
  
    Config cfg;

    try {
        cfg.readFile(path);
    }
    catch(const FileIOException &fioex) {
        std::cerr << "I/O error while reading configuration file." << std::endl;
        exit(1);
    }
    catch(const ParseException &pex) {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                << " - " << pex.getError() << std::endl;
        exit(1);
    }

    try {
        string st = cfg.lookup("frShaderPath");
        frShaderPath = st;
    }
    catch(const SettingNotFoundException &nfex) {
        cerr << "No 'frShaderPath' setting in configuration file." << endl;
        exit(1);
    }
}

string cfgGetFragmentShaderPath() {
    return frShaderPath;
}


void cfgInitValues(string path) {
    cfgSetPath(path);
    cfgSetResX();
    cfgSetResY();
    cfgSetTitle();
    cfgSetVertexShaderPath();
    cfgSetFragmentShaderPath();
    cfgGetOS();
}
