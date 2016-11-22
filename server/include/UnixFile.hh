//
// Created by armeyon on 13/11/16.
//

#ifndef UNIXFILE_HH
#define UNIXFILE_HH

#ifdef __linux
#include <dirent.h>
#include <unistd.h>
# define SEARCH(x) UnixFile::search(x)
# define GETUSER UnixFile::getUserList()
#endif

#ifdef __APPLE__
#include <dirent.h>
#include <unistd.h>
# define SEARCH(x) UnixFile::search(x)
# define GETUSER UnixFile::getUserList()
#endif

#include "AFile.hh"

class UnixFile : public AFile {
public:
    UnixFile();
    UnixFile(std::string name);
    ~UnixFile();
    static std::list<std::string> getUserList();
    static bool search(std::string filename);
    void addMessage(std::string from, std::string to, std::string message);
};

#endif //UNIXFILE_HH
