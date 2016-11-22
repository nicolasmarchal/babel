//
// Created by armeyon on 13/11/16.
//

#ifndef IFILE_HH
#define IFILE_HH

#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <cstring>
#include "../../protocol/protocol.hh"

#ifdef __linux
# define SEP "/"
# define BDD_PATH "./Bdd/"
#endif

#ifdef __APPLE__
# define SEP "/"
# define BDD_PATH "./Bdd/"
#endif

#if defined(WIN32) || defined(WIN64)
# define SEP "\\"
# define BDD_PATH ".\\Bdd\\"
#endif

class IFile {
public:
    virtual ~IFile() {};
    virtual const std::string getPassword() const = 0;
    virtual const std::list<std::string> getRequests() const = 0;
    virtual const std::list<std::string> getUsers() const = 0;
    virtual void setPassword(std::string pass) = 0;
    virtual void addRequest(std::string request) = 0;
    virtual void delRequest(int nb) = 0;
    virtual void addUser(std::string user) = 0;
    virtual void delUser(std::string user) = 0;
    virtual void addMessage(std::string from, std::string to, std::string message) = 0;
    virtual const std::vector<t_usrtxt> getMessage(std::string who) const = 0;
};

#endif //IFILE_HH
