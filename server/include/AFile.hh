//
// Created by armeyon on 13/11/16.
//

#ifndef AFILE_HH
#define AFILE_HH

#include "IFile.hh"

class AFile : public IFile {
public:
    AFile() {};
    AFile(std::string name, std::string path, std::string origin, int nbR, int nbU);
    virtual ~AFile() {};
    const std::string getPassword() const;
    const std::list<std::string> getRequests() const;
    const std::list<std::string> getUsers() const;
    void setPassword(std::string pass);
    void addRequest(std::string request);
    void delRequest(int nb);
    void addUser(std::string user);
    void delUser(std::string user);
    const std::vector<t_usrtxt> getMessage(std::string who) const;
    virtual void addMessage(std::string from, std::string to, std::string message) = 0;
protected:
    std::string _name;
    std::string _path;
    std::string _origin;
    int nbRequest;
    int nbUser;
};

#endif //AFILE_HH
