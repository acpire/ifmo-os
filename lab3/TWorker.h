
#ifndef TWORKER_H_INCLUDED
#define TWORKER_H_INCLUDED

#include <pthread.h>

class TWorker {
private:
  pthread_t handle;

public:
  void Start();
  void Wait();
  virtual void Run() = 0;
  virtual ~TWorker() {}
};

#endif // TWORKER_THREAD_INCLUDED
