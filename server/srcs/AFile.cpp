//
// Created by armeyon on 13/11/16.
//

#include "../include/AFile.hh"

AFile::AFile(std::string name, std::string path, std::string origin, int nbR, int nbU):
_name(name), _path(path), _origin(origin), nbRequest(nbR), nbUser(nbU)
{
}


void printList(std::list<std::string> list)
{
    std::list<std::string>::iterator it;

    for (it = list.begin(); it != list.end(); ++it)
        std::cout << *it << std::endl;
}

void printVector(std::vector<t_usrtxt> list)
{
    unsigned int i;

    for (i = 0; i != list.size(); i++)
        std::cout << list[i].user << " " << list[i].txt << std::endl;
}

const std::vector<t_usrtxt> AFile::getMessage(std::string who) const {
    std::fstream file(_path + SEP + who);
    std::string line;
    std::vector<t_usrtxt> list;
    t_usrtxt txt;

    while (getline(file, line)) {
        memset(txt.user, 0, sizeof(txt.user));
        memset(txt.txt, 0, sizeof(txt.txt));
        strcpy(txt.user, line.substr(0, line.find_first_of(":", 0)).c_str());
        strcpy(txt.txt, line.substr(line.find_first_not_of(" ", line.find_first_of(" ")), line.size()).c_str());
        list.push_back(txt);
    }
    return (list);
}

void insert(std::string filename, std::string in, std::string linecheck, int nb, std::string type)
{
    std::ifstream filebuf(filename);
    std::string line;
    std::string backpath;
    std::ofstream backfile;
    bool check = false;

    backpath = filename + "._backup";
    backfile.open(backpath, std::ios::out);
    while (getline(filebuf, line))
    {
        if (line.size() != 0)
            backfile << line << std::endl;
        if (line.find(type, 0) !=line.npos)
            check = true;
        if (line.find(linecheck, 0) != line.npos && check == true)
            backfile << nb << ". " << in << std::endl;
    }
    filebuf.close();
    backfile.close();
    remove(filename.c_str());
    rename(backpath.c_str(), filename.c_str());
}

void writeIn(std::string filename, std::string in, int nb, std::string type)
{

    bool check = false;
    std::string line;
    std::string checkline;
    std::fstream file(filename);

    if (file)
    {
        while (getline(file, line)) {
            if (line.find(type, 0) != line.npos)
                check = true;
            else if (line.find("::", 0) != line.npos && check == true) {
                file.close();
                insert(filename, in, checkline, nb, type);
                return ;
            }
            if (line[0] != 0)
                checkline = line;
        }
        if (check == true || line.find("::", 0) == line.npos) {
            file.close();
            insert(filename, in, checkline, nb, type);
        }
    }
    else
        std::cerr << "Le fichier n'existe pas" << std::endl;
    file.close();
}

std::string sortLine(std::string line, int nb, int nbSort)
{
    std::string del;
    std::stringstream ret;

    del = line;
    del = del.substr(0, del.find(". "));
    line.replace(line.find(del, 0), del.size(), "");
    ret << nb + nbSort << line;
    if (line.size() == 0)
        return ("");
    return (ret.str());
}

std::string clearLine(std::string line)
{
    std::string del;

    del = line;
    del = del.substr(0, del.find_first_not_of(" ", del.find(" ", 0)));
    line.replace(line.find(del, 0), del.size(), "");
    return (line);
}

const std::string AFile::getPassword() const
{
    std::string line;

    std::fstream file(_name, std::ios::in);
    if (file)
    {
        while ((getline(file, line))) {
            if (line.find("::Password::", 0) != line.npos)
            {
                getline(file, line);
                return (line);
            }
        }
        file.close();
    }
    else
        std::cerr << "Le fichier n'existe pas" << std::endl;
    return NULL;
}

std::list<std::string> getting(std::string filename, std::string type)
{
    std::fstream file(filename);
    std::string line;
    bool check = false;
    std::list<std::string> ret;

    if (file)
    {
        while (getline(file, line)) {
            if (line.find("::", 0) != line.npos && check == true)
                check = false;
            if (check == true) {
                if (line.size() != 0)
                    ret.push_back(clearLine(line));
            }
            if (line.find(type, 0) != line.npos)
                check = true;
        }
        file.close();
    }
    return (ret);
}

const std::list<std::string> AFile::getRequests() const {
    return (getting(_name, "::Requests::"));
}

const std::list<std::string> AFile::getUsers() const {
    return (getting(_name, "::Friends::"));
}

void AFile::setPassword(std::string pass)
{
    std::ofstream file;
    file.open(_name, std::ios::app);
    file << "::Password::" << std::endl;
    file << pass << std::endl;
    file.close();
}

void add(std::string in, std::string type, std::string filename, int nb)
{
    std::fstream getfile(filename);
    std::stringstream buf;

    if (!getfile) {
        std::cerr << "Le fichier n'existe pas" << std::endl;
        return ;
    }

    buf << getfile.rdbuf();
    std::string file = buf.str();

    if (file.find(type, 0) == file.npos)
        getfile << type << std::endl;
    writeIn(filename, in, nb + 1, type);
    getfile.close();
}

void AFile::addRequest(std::string request)
{
    add(request, "::Requests::", _name, nbRequest);
    ++nbRequest;
}

void AFile::addUser(std::string user)
{
    add(user, "::Friends::", _name, nbUser);
    ++nbUser;
}

void delLine(std::string filename, int nb, std::string type)
{
    int check = 0;
    int nbSort = -1;
    std::string line;
    std::fstream file(filename);
    std::stringstream snb;
    std::string backpath;
    std::ofstream backfile;
    backpath = filename + "._backup";
    backfile.open(backpath, std::ios::out);

    snb << nb << ". ";
    if (file)
    {
        while (getline(file, line)) {
            if (check == 2 && sortLine(line, nb, ++nbSort).size() != 0)
                backfile << sortLine(line, nb, nbSort) << std::endl;
            if (line.find(type, 0) != line.npos)
                check = 1;
            if (line.find("::", 0) != line.npos && check == 2)
                check = 0;
            if (line.find(snb.str(), 0) != line.npos && check == 1) {
                line.replace(line.find(snb.str(), 0), line.size(), "");
                check = 2;
            }
            if ((check == 0 || check == 1) && line.size() != 0)
                backfile << line << std::endl;
        }
        file.close();
        backfile.close();
        remove(filename.c_str());
        rename(backpath.c_str(), filename.c_str());
    }
    else
        std::cerr << "Le fichier n'existe pas" << std::endl;
}

void AFile::delRequest(int nb)
{
    if (nb < 0 && nb > nbRequest)
        return ;
    delLine(_name, nb, "::Requests::");
    --nbRequest;
}

bool checkUser(std::string user, std::string line)
{
    line = line.substr(line.find_first_not_of(" ", line.find_first_of(" ")), line.size());
    if (line == user)
        return (true);
    return (false);
}

int getUserId(std::string line)
{
    std::string del;
    int nb;

    del = line;
    del = del.substr(0, del.find_first_of(".", 0));
    nb = stoi(del, 0);
    return (nb);
}

int FindUser(std::string user, std::string filename)
{
    std::fstream file(filename);
    std::string line;
    bool check = false;
    std::list<std::string> ret;

    if (file)
    {
        while (getline(file, line)) {
            if (line.find("::", 0) != line.npos && check == true)
                check = false;
            if (check == true) {
                if (line.size() != 0 && checkUser(user, line) == true)
                    return (getUserId(line));
            }
            if (line.find("::Friends::", 0) != line.npos)
                check = true;
        }
        file.close();
    }
    return (-1);
}

void AFile::delUser(std::string user)
{
    int nb = FindUser(user, _name);

    if (nb < 0 && nb > nbUser)
        return ;
    delLine(_name, nb, "::Friends::");
    --nbUser;
}
