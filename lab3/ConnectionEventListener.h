
#ifndef CONNECTION_EVENT_LISTENER_INCLUDED
#define CONNECTION_EVENT_LISTENER_INCLUDED

#include <string>

class ConnectionEventListener {
public:
   virtual void onDisconnect(int /*fd*/) = 0;
   virtual void onMessage(const std::string& /*message*/) = 0;
};

#endif
