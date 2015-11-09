
#ifndef MESSAGE_LOG_H_INCLUDED
#define MESSAGE_LOG_H_INCLUDED

#include <string>

using namespace std;

class MessageLog {
public:
   static MessageLog& Instance();

   void writeMessage(const string& message);
   int getMessageCount() const { return messageCount; }
private:
   MessageLog(): messageCount(0) { /*empty private implementation*/ }
   MessageLog(const MessageLog&); // n/a
   MessageLog& operator=(const MessageLog&); // n/a

   int messageCount;
};

#endif // MESSAGE_LOG_H_INCLUDED
