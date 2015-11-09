
#include "TWorker.h"

namespace {
   void* runThread(void* obj)
   {
      TWorker* worker = static_cast<TWorker*>(obj);
      worker->Run();
      return NULL;
   }
} // namespace 

void TWorker::Wait()
{
   pthread_join(handle, NULL);
}

void TWorker::Start()
{
   pthread_create(&handle, NULL, runThread, static_cast<void*>(this));
}
