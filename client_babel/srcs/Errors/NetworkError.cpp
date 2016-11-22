#include "NetworkError.hh"

NetworkError::NetworkError(std::string const &e)
  : err(e)
{
}

NetworkError::NetworkError(const char *e)
  : err(std::string(e))
{
}

const char *
NetworkError::what() const throw()
{
  return (err.c_str());
}
