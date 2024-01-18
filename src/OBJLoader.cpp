#include <string>
#include <fstream>

using namespace std;

string getModelSrc(string path) {
  
  ifstream in(path);
  string contents((istreambuf_iterator<char>(in)),
    istreambuf_iterator<char>());

  return contents;

}




