//
// Created by armeyon on 13/11/16.
//

#include "UnixFile.hh"

int getNb(std::string name, std::string type)
{
    std::ifstream filebuf(name);
    std::string line;
    bool check = false;
    int nb = 0;

    while (getline(filebuf, line))
    {
        if (line.size() != 0 && check == true)
            ++nb;
        if (line.find(type, 0) !=line.npos)
            check = true;
        else if (line.find("::", 0) != line.npos && check == true)
            check = false;
    }
    filebuf.close();
    return (nb);
}

UnixFile::UnixFile()
{

}

UnixFile::UnixFile(std::string name) : AFile(BDD_PATH + name + SEP + name, BDD_PATH + name, name, 0, 0)
{
    if (getNb(_name, "::Friends::") > 0)
        nbUser = getNb(_name, "::Friends");
    if (getNb(_name, "::Requests::") > 0)
        nbRequest = getNb(_name, "::Requests::");
    mkdir(_path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
}

UnixFile::~UnixFile() {}

std::list<std::string> UnixFile::getUserList() {
    std::list<std::string> list;
    std::string path = BDD_PATH;
    std::string folder = BDD_PATH;
    DIR *dir;
    struct dirent *name;

    if ((dir = opendir(path.c_str())) != NULL) {
        while ((name = readdir(dir)) != NULL) {
            if (name->d_name[0] != '.') {
                path = folder + name->d_name + SEP + name->d_name;
                if (access(path.c_str(), F_OK) != -1)
                    list.push_back(name->d_name);
            }
        }
        closedir(dir);
    }
    return (list);
}

bool UnixFile::search(std::string filename) {
    std::string path = BDD_PATH + filename + SEP + filename;
    if (access(path.c_str(), F_OK) != -1)
        return true;
    return false;
}

void UnixFile::addMessage(std::string from, std::string to, std::string message) {
    std::string path;
    std::ofstream file;

    _origin == from ? path = _path + SEP + to : path = _path + SEP + from;
    if (access(path.c_str(), F_OK) != -1) {
        file.open(path, std::ios::app);
        file << std::endl;
    }
    else
        file.open(path, std::ios::app);
    file << from + ": " + message;
    file.close();
}
