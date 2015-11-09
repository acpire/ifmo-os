#!/usr/bin/env python

import socket, sys, re

def getMsg(story):
   msg = 'imsg'
   size = str(len(story))
   while len(size) < 4:
      size = '0' + size
   msg += size
   msg += story
   return msg

HOST = 'localhost'    # The remote host
PORT = 5000           # The same port as used by the server
s = None
for res in socket.getaddrinfo(HOST, PORT, socket.AF_UNSPEC, socket.SOCK_STREAM):
    af, socktype, proto, canonname, sa = res
    try:
        s = socket.socket(af, socktype, proto)
    except socket.error as msg:
        s = None
        continue
    try:
        s.connect(sa)
    except socket.error as msg:
        s.close()
        s = None
        continue
    break

if s is None:
    print 'could not open socket'
    sys.exit(1)


story = ''
separator = re.compile('^=+$')
with open('./stories/stories', 'r') as stories:
   for line in stories:
      if separator.match(line):
         if len(story) > 0:
            s.sendall(getMsg(story))
         story = ''
      else:
         story += line

   if len(story) > 0:
      s.sendall(getMsg(story))

s.close()

