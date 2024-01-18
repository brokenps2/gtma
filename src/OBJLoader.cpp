#include <string>
#include <fstream>
#include <iostream>

using namespace std;

string getModelSrc(string path) {
  
  ifstream in(path);
  string contents((istreambuf_iterator<char>(in)),
    istreambuf_iterator<char>());

  if(contents == "") {
    char ans;
    std::cout << "\n";
    std::cerr << "Specified OBJ File: " << path << " couldn't be read! (or the file is empty)" << std::endl;
    std::cout << "continue running program with empty file? y/N" << std::endl;
    std::cin >> ans;
    if(ans == 'y' || ans == 'Y') {

    } else {
      exit(0);
    }
  }

  return contents;

}




