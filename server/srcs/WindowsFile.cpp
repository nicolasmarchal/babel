//
// Created by armeyon on 13/11/16.
//

#include "WindowsFile.hh"

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

WindowsFile::WindowsFile()
{

}

WindowsFile::WindowsFile(std::string name) : AFile(BDD_PATH + name + SEP + name, BDD_PATH + name, name, 0, 0){
    if (getNb(_name, "::Friends::") > 0)
        nbUser = getNb(_name, "::Friends");
    if (getNb(_name, "::Requests::") > 0)
        nbRequest = getNb(_name, "::Requests::");
    _mkdir(BDD_PATH);
	_mkdir(_path.c_str());
}

WindowsFile::~WindowsFile() {};

void WindowsFile::addMessage(std::string from, std::string to, std::string message) {
    std::string path;
    std::ofstream file;

    _origin == from ? path = _path + SEP + to : path = _path + SEP + from;
    if (_access(path.c_str(), 0) != -1) {
		file.open(path, std::ios::app);
		file << std::endl;
	}
	else
		file.open(path, std::ios::app);
	file << from + ": " + message;
	file.close();
}

bool WindowsFile::search(std::string filename) {
    std::string path = BDD_PATH + filename + SEP + filename;

    if (_access(path.c_str(), 0) != -1)
	  return true;
  return false;
}

std::list<std::string> WindowsFile::getUserList() {

    std::list<std::string> list;
    std::string path = BDD_PATH;
    std::string folder = BDD_PATH;
    WIN32_FIND_DATA ffd;
    std::stringstream ss;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    hFind = FindFirstFile(".\\Bdd\\*", &ffd);
    if (INVALID_HANDLE_VALUE == hFind)
        return (list);
    else
    {
        do
        {
            if (ffd.cFileName[0] != '.')
            {
                path = folder + ffd.cFileName + SEP + ffd.cFileName;
                if (_access(path.c_str(), 0) != -1)
                    list.push_back(ffd.cFileName);
            }
        }
        while (FindNextFile(hFind, &ffd) != 0);
    }
    FindClose(hFind);
    return (list);
}
