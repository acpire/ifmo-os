#include <queue> 
#include <thread>
#include <mutex>
#include <memory>
#include <string>
#include <iostream>
#include <climits>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <sys/types.h>

using namespace std;

// Класс для блокировки.
// Реализуйте секции TODO, можете использовать std::mutex или pthread,
// или любую рабочую блокировку.
class TMutex {
private:
   std::mutex mtx;
public:
   void Lock();
   void Unlock();
};

void
TMutex::Lock()
{
  // TODO
}

void
TMutex::Unlock()
{
  // TODO
}

// Класс т.н. "локера"
// Нужен для того, чтобы мы не забывали разблокировать мьютекс.
// Захватите блокировку в конструкторе, освободите в деструкторе.
class TMutexLocker {
public:
   TMutexLocker(TMutex& mutex);
   ~TMutexLocker();

private:
   TMutex& mutex;
};

TMutexLocker::TMutexLocker(TMutex& mutex) :
   mutex(mutex)
{
  // TODO
}

TMutexLocker::~TMutexLocker() {
  // TODO
}


////////////////////////////////////////////////////////////////
// Код ниже не должен модифицироваться
////////////////////////////////////////////////////////////////
class Wallet {
public:
   static Wallet& Instance() {
      static Wallet instance;
      return instance;
   }

   bool stopped;
   queue<string> mybitcoins;
   TMutex mutex;

private:
   Wallet() :
      stopped(false) { }
};

// Родительский класс для поточных процедур.
// У нас это производитель и потребитель.
class TWorker {
private:
   std::unique_ptr<std::thread> worker;

public:
  void Start();
  void Wait();
  virtual void Run() = 0;
  virtual ~TWorker() {}
};

void TWorker::Wait()
{
   if (worker.get())
      worker->join();
}

static void* runThread(void* obj)
{
  TWorker* worker = static_cast<TWorker*>(obj);
  worker->Run();
}

void TWorker::Start()
{
   worker.reset(new std::thread(runThread, this));
}

// producer implementation
class Producer : public TWorker {
public:
   Producer() : generated(0) {
      cout << "Created producer #0x" << hex << this << endl << dec;
   }
   ~Producer() {
      cout << "Destroyed producer #0x" << hex << this << dec << ", "
        "generated " << generated << " permutations" << endl;
   }
private:
  int generated;

  void Run();
};

void Producer::Run()
{
   string hash("012345678abcdef");
   Wallet& wallet = Wallet::Instance();

   {
      TMutexLocker lock(Wallet::Instance().mutex);
      wallet.mybitcoins.push(hash);
   }

   ++generated;
   while (next_permutation(hash.begin(), hash.end())) {
      ++generated;
      TMutexLocker lock();
      wallet.mybitcoins.push(hash);
   }
   wallet.stopped = true;
}

// consumer implementation
class Consumer : public TWorker {
public:
   Consumer() : consumed(0) {
      cout << "Created consumer #0x" << hex << this << endl << dec;
   }
   ~Consumer() {
      cout << "Destroyed consumer #0x" << hex << this << dec << ", "
         "consumed " << consumed << " permutations" << endl;
   }
private:
   int consumed;

   void Run();
};

void Consumer::Run()
{
   Wallet& wallet = Wallet::Instance();

   while (!wallet.stopped || !wallet.mybitcoins.empty()) {
      if (wallet.mybitcoins.empty()) {
         continue;
      }
      
      cout << "Buy cool stuff using bitcoin " << wallet.mybitcoins.front() << endl;
      {
         TMutexLocker lock(wallet.mutex);
         if (!wallet.mybitcoins.empty()) {
            wallet.mybitcoins.pop();
         }
      }
      ++consumed;
   }
}

static void usage(const char* prog_name, const char* err=0)
{
   if (err) cerr << "error: " << err << endl;
   cout << "usage: " << prog_name << " <num producers> <num consumers>" << endl;
}

int main(int argc, char* argv[])
{
   if (argc < 3) {
      usage(argv[0]);
      return 1;
   }

   unsigned long input_val = strtoul(argv[1], NULL, 10);
   if (input_val < 1 || input_val > 10) {
      usage(argv[0], "producers number is out of range 0-10");
      return 1;
   }
   int producers = input_val;

   input_val = strtoul(argv[2], NULL, 10);
   if (input_val < 1 || input_val > 10) {
      usage(argv[0], "consumers number is out of range 0-10");
      return 1;
   }
   int consumers = input_val;

   vector<TWorker*> workers;
   while (producers || consumers) {
      if (producers) {
         workers.push_back(new Producer());
         --producers;
      }

      if (consumers) {
         workers.push_back(new Consumer());
         --consumers;
      }
   }

   for (int i=0; i<workers.size(); ++i) {
      workers[i]->Start();
   }

   for (int i=0; i<workers.size(); ++i) {
      workers[i]->Wait();
      delete workers[i];
   }
}

