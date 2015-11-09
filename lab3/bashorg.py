#!/usr/bin/env python

import urllib2, time
from HTMLParser import HTMLParser

class MyHTMLParser(HTMLParser):
   capture = False
   story = ''

   def handle_starttag(self, tag, attrs):
      if tag == 'div':
         for attr in attrs:
            if attr[0] == 'class' and attr[1] == 'text':
               self.capture = True
      elif self.capture and tag == 'br':
         self.story += "\n"
   def handle_endtag(self, tag):
      if tag == 'div' and self.capture:
         self.capture = False
   def handle_data(self, data):
      if self.capture:
         self.story = self.story + data

output = open('stories.txt', 'w')

for story in xrange(431052, 431100):
   url = 'http://bash.im/quote/' + str(story)
   request = urllib2.Request(url,  headers={
      "Host" : "bash.im",
      "User-Agent" : "Mozilla/5.0 (X11; Linux x86_64; rv:32.0) Gecko/20100101 Firefox/32.0 Iceweasel/32.0",
      "Accept" : "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8",
      "Accept-Language" : "en-US,en;q=0.5",
      "Accept-Encoding" : "text/html",
      "Connection" : "keep-alive",
      "Pragma" : "no-cache",
      "Cache-Control" : "no-cache"
   })
   response = urllib2.urlopen(request)
   html = response.read()
   response.close()

   parser = MyHTMLParser()
   parser.feed(html.decode('cp1251'))

   output.write(parser.story.encode('utf8'))
   output.write("\n======================\n")

   print "got story #", story

   time.sleep(1)
