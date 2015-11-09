
#include <string>

#include "MessageLog.h"

using namespace std;

MessageLog&
MessageLog::Instance()
{
    static MessageLog messageLogInstance;
    return messageLogInstance;
}

void
MessageLog::writeMessage(const string& message)
{
   ++messageCount;
}
