
#ifndef THREAD_POOL_H_INCLUDED
#define THREAD_POOL_H_INCLUDED

#include <map>
#include <string>
#include "ConnectionEventListener.h"

using namespace std;

class TWorker;

class ThreadPool : public ConnectionEventListener {
public:
   static ThreadPool& Instance();
   void acceptConnection(int connFd);

   // ConnectionEventListener interface
   virtual void onDisconnect(int /*fd*/);
   virtual void onMessage(const string& /*message*/);

private:
   typedef map<int /*fd*/, TWorker*> ThreadMap;

   ThreadPool() { /*empty private implementation*/ }
   ThreadPool(const ThreadPool&); // n/a
   ThreadPool& operator=(const ThreadPool&); // n/a

   ThreadMap threads;
};

#endif // THREAD_POOL_H_INCLUDED
