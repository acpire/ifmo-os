
#include "ThreadPool.h"
#include "ConnectionThread.h"
#include "MessageLog.h"
#include <iostream>

using std::cout;

ThreadPool&
ThreadPool::Instance()
{
    static ThreadPool threadPool;
    return threadPool;
}

void
ThreadPool::onDisconnect(int fd)
{
   // TODO:
   // signal thread pool
   cout << "ThreadPool::onDisconnect(), messageCount=" <<
      MessageLog::Instance().getMessageCount() << endl;
}

void
ThreadPool::onMessage(const string& message)
{
   // TODO: synchronize access to message log
//   cout << "ThreadPool::onMessage(), \"" << message << "\"" << endl;
   MessageLog::Instance().writeMessage(message); 
}

void
ThreadPool::acceptConnection(int fd)
{
   ThreadMap::iterator it = threads.lower_bound(fd);
   if (it != threads.end() && it->first != fd) {
      // connection already accepted
      return;
   }

   it = threads.insert(it, make_pair(fd, new ConnectionThread(fd, this)));

   it->second->Start();
}
