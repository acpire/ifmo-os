#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <climits>
#include "ConnectionThread.h"
#include "ConnectionEventListener.h"

/*
message format:
4b 'imsg' signature
4b msg size
*/

using namespace std;

namespace {
bool is_number(const std::string& s)
{
   std::string::const_iterator it = s.begin();
   while (it != s.end() && std::isdigit(*it)) ++it;
   return !s.empty() && it == s.end();
}
} // namespace

ConnectionThread::ConnectionThread(int fd, ConnectionEventListener* listener) :
   connFd(fd),
   eventListener(listener),
   gotHeader(false),
   msgLen(0)
{
}

int
ConnectionThread::getMessageLen(const string& msg) const
{
//   cout << "NOTE: getMessageLen(" << msg << ")" << msg << endl;
   static const string signature = "imsg";
   const int msgSizeFieldLen = 4; // byte

   if (msg.size() < (signature.size() + msgSizeFieldLen)) {
      cerr << "ERR: no header in message" << endl;
      return -1;
   }

   if (msg.compare(0, signature.size(), signature)) {
      cerr << "ERR: message signature doesn't match: " <<
         msg.substr(0, signature.size()) << endl;
      return -1;
   }

   string msgSize = msg.substr(signature.size(), msgSizeFieldLen);
//   cout << "NOTE: msgSize=" << msgSize << endl;
   if (!is_number(msgSize)) {
      cerr << "ERR: non numeric message size" << endl;
      return -1;
   }

   long msgLen = strtol(msgSize.c_str(), NULL, 10);
   if (msgLen == LONG_MAX || msgLen == LONG_MIN) {
      cerr << "ERR: message size is out of range" << endl;
      return -1;
   }

   return msgLen;
}

bool
ConnectionThread::recv(const char* recvBuff, int size)
{
//   cout << "NOTE: recv " << recvBuff << endl;
   buffer.write(recvBuff, size);
   while (buffer.good()) {
      if (!gotHeader) {
         if (buffer.tellp() >= 8) {
//            cout << "NOTE: parse header" << endl;
            string msg = buffer.str();
            msgLen = getMessageLen(msg);
//            cout << "NOTE: got message len " << msgLen << endl;
            if (msgLen < 0) {
               return false;
            }
            gotHeader = true;
            buffer.str("");
            buffer.clear();
            buffer << msg.substr(8);
//            cout << "NOTE: reading message with len " << msgLen << " "
//               "buffer.tellp()=" << buffer.tellp() << endl;
         }
         else {
            // not enough data, read furher
//            cout << "NOTE: not enough data to parse header tellp=" <<
//               buffer.tellp() << endl;
            break;
         }
      }

      if (buffer.tellp() >= msgLen) {
         string bufferStr = buffer.str();
         string msg = bufferStr.substr(0, msgLen);
         gotHeader = false;
         buffer.str("");
         buffer.clear();
//         cout << "NOTE: got message: " <<  msg << " "
//            "buffer.tellp()=" << buffer.tellp() << " "
//            "rest of buffer=" << bufferStr.substr(msg.size()) << endl;
         buffer << bufferStr.substr(msg.size());
         if (eventListener) {
            eventListener->onMessage(msg);
         }
      }
      else {
         break;
      }
   }
   return true;
}

void
ConnectionThread::Run()
{
   char recvBuff[10];
   int n = 0;
   while ((n = read(connFd, recvBuff, sizeof(recvBuff)-1)) > 0)
   {
      if (!recv(recvBuff, n)) {
         cout << "ERR: cannot process buffer" << endl;
         break;
      }
   }

   if (eventListener) {
      eventListener->onDisconnect(connFd);
   }
}

