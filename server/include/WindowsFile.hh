//
// Created by armeyon on 13/11/16.
//

#ifndef WINDOWSFILE_HH
#define WINDOWSFILE_HH

#if defined(WIN32) || defined(WIN64)
#include <direct.h>
#include <stdio.h>
#include <io.h>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

# define SEARCH(x) WindowsFile::search(x)
# define GETUSER Windows::getUserList()

#endif

#include "AFile.hh"

class WindowsFile : public AFile {
public:
    WindowsFile();
    WindowsFile(std::string name);
    ~WindowsFile();
    static std::list<std::string> getUserList();
    static bool search(std::string filename);
    void addMessage(std::string from, std::string to, std::string message);
};

#endif //WINDOWSFILE_HH
