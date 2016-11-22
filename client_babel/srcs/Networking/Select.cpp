#include "Select.hh"

Select::Select()
  : maxFd(0)
{
  FD_ZERO(&readfds);
}

Select::~Select()
{}

void
Select::addSocket(ISocket *socket)
{
  sockets.push_back(socket);
  FD_SET(socket->getFd(), &readfds);
  if (socket->getFd() + 1 > (int)maxFd)
    maxFd = socket->getFd() + 1;
}

void
Select::removeSocket(ISocket *socket)
{
  bool recal = false;

  for (auto it = sockets.begin(); it != sockets.end(); ++it)
  {
    if (*it == socket)
      {
        sockets.erase(it);
        FD_CLR(socket->getFd(), &readfds);
        if (socket->getFd() + 1 == (int)maxFd)
          recal = true;
        break ;
      }
  }
  if (recal == true)
    setMaxFd();
}

void
Select::setMaxFd()
{
  maxFd = 0;
  for (auto it = sockets.begin(); it != sockets.end(); ++it)
  {
    if ((*it)->getFd() + 1 > (int)maxFd)
      maxFd = (*it)->getFd() + 1;
  }
}

Select::SelectIterator
Select::operator()(unsigned int delay)
{
  Select::SelectIterator si;
#if __APPLE__
  struct timeval tv;

  tv.tv_sec = 0;
  tv.tv_usec = delay;
#else
  struct timeval tv = {0, (long)delay};
#endif
  unsigned int nmaxFd;
#if defined(WIN32) || defined(WIN64)
	nmaxFd = 0;
#else
	nmaxFd = maxFd;
#endif

	if (sockets.size() > 0)
	{
		fd_set tmpfds = readfds;
		if (select(nmaxFd, &tmpfds, 0, 0, &tv) > 0)
		{
			for (auto it = sockets.begin(); it != sockets.end(); ++it)
			{
				if (FD_ISSET((*it)->getFd(), &tmpfds))
					si.addSocket(*it);
			}
		}
	}
	else
		sleep(delay);
  return (si);
}

void
Select::sleep(unsigned int delay)
{
#if defined(WIN32) || defined(WIN64)
		::Sleep(delay / 100);
#else
		::usleep(delay);
#endif
}

Select::SelectIterator::SelectIterator()
  : idx(0)
{}

Select::SelectIterator::~SelectIterator()
{}

void
Select::SelectIterator::addSocket(ISocket *socket)
{
  sockets.push_back(socket);
}

bool
Select::SelectIterator::hasNext() const
{
  return (idx < sockets.size());
}

ISocket *
Select::SelectIterator::getNext()
{
  return (sockets[idx++]);
}

void
Select::SelectIterator::reset()
{
  idx = 0;
}
