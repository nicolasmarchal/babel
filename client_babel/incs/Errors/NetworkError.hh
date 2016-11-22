#ifndef NETWORKERROR_HH_
# define NETWORKERROR_HH_
# include <exception>
# include <string>

class NetworkError : public std::exception
{
public:
  NetworkError(std::string const &e = "An error happens during the socket's creation");
  NetworkError(const char *e = "An error happens during the socket's creation");
  virtual const char *what() const throw();
private:
  std::string err;
};

#endif
