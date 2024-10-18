#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <map>
#include <iomanip>

#include "PicoSHA2/picosha2.h"

using namespace std;

size_t min_size = 0;
bool search_recursively = false;

void group_by_hash(string folder, map<string, vector<filesystem::path>>& res) {
  filesystem::path pt(folder);
  filesystem::directory_iterator start(pt);
  filesystem::directory_iterator end;

  for_each(start, end, [&res](const auto& entry){
      if (entry.is_directory() and search_recursively) {
        group_by_hash(entry.path().string(), res);
      } else if (entry.is_regular_file() and (not entry.is_symlink())) {
        if (filesystem::file_size(entry.path()) < min_size)
          return; // We are returning because this is a lambda function running
                  // in an iteration.

        string hash_str;
        hash_str.resize(picosha2::k_digest_size);

        ifstream reader(entry.path().string(), ios::binary);
        picosha2::hash256(reader, hash_str.begin(), hash_str.end());

        res[hash_str].push_back(entry.path());
      }
    });
}

void delete_duplicates(vector<filesystem::path> paths, filesystem::path original) {
  for (filesystem::path file : paths) {
    if (file == original)
      continue;

    if (remove(file))
      create_symlink(original, file);
    else
      cerr << "Cannot find " << file << "." << endl;
  }
}

int main(int argc, char** argv) {
  string folder = ".";

  if (argc >= 2) {
    for (int idx = 1; idx < argc; idx++) {
      string argstr = argv[idx];
      if (argstr.find("--minsize=") != string::npos) {
        string min_size_str = argstr.substr(string("--minsize=").length());
        min_size = (size_t)(stoi(min_size_str));
      }
      else if (argstr == "-r")
        search_recursively = true;
      else
        folder = argstr;
    }
  }

  map<string, vector<filesystem::path>> entries;
  group_by_hash(folder, entries);

  for (auto entry : entries) {
    if (entry.second.size() <= 1)
      continue;

    cout << "Duplicate files found:\n";
    int x = 1;
    for (auto name : entry.second) 
      cout << x++ << ") " << name.filename().string() << endl;
    cout << "Do you want to delete them (y/n)? ";
    char ch;
    cin >> ch;
    if (ch == 'y' or ch == 'Y') {
      cout << "Choose the original file number (1 - " << --x <<"): ";
      cin >> x;
      delete_duplicates(entry.second, entry.second[x - 1]);
    }
  }

  return 0;
}

