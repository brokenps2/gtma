#include <iostream>
#include <string>
#include <libconfig.h++>

using namespace std;
using namespace libconfig;

string path;

void cfgSetPath(string newPath) {
    path = newPath;
}

int cfgGetResX() {
  
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
        return sx;
    }
    catch(const SettingNotFoundException &nfex) {
        cerr << "No 'resX' setting in configuration file." << endl;
    }

    return 1;
}

int cfgGetResY() {
  
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
        return sy;
    }
    catch(const SettingNotFoundException &nfex) {
        cerr << "No 'resY' setting in configuration file." << endl;
    }

    return 1;

}

string cfgGetTitle() {
  
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
        return st;
    }
    catch(const SettingNotFoundException &nfex) {
        cerr << "No 'name' setting in configuration file." << endl;
    }

    return "gtma";

}


string cfgGetVertexShaderPath() {
  
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
        return st;
    }
    catch(const SettingNotFoundException &nfex) {
        cerr << "No 'vtShaderPath' setting in configuration file." << endl;
    }

    return "1";

}

string cfgGetFragmentShaderPath() {
  
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
        return st;
    }
    catch(const SettingNotFoundException &nfex) {
        cerr << "No 'frShaderPath' setting in configuration file." << endl;
    }

    return "1";

}


void cfgPrintAllValues() {

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
        int sx = cfg.lookup("resX");
        int sy = cfg.lookup("resY");
        string st = cfg.lookup("title");

        std::cout << "resX: " << sx << "\n";

        std::cout << "resY: " << sy << "\n";

        std::cout << "title: " << st << "\n";

    }
    catch(const SettingNotFoundException &nfex) {
        cerr << "can't find setting(s) in configuration file." << endl;
    }

}
