
#ifndef CONNECTION_THREAD_INCLUDED
#define CONNECTION_THREAD_INCLUDED

#include <string>
#include <sstream>
#include "TWorker.h"

class ConnectionEventListener;

class ConnectionThread : public TWorker {
public:
   explicit ConnectionThread(int fd, ConnectionEventListener* listener=0);
   virtual void Run();

protected:
   int getMessageLen(const std::string&) const;
   bool recv(const char*, int size);

private:
   int connFd;
   ConnectionEventListener* eventListener;
   std::ostringstream buffer;
   bool gotHeader;
   int msgLen;
};

#endif // CONNECTION_THREAD_INCLUDED

