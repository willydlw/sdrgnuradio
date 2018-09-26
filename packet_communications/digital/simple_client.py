"""
sockets example: simple client

Flow:
    Create a TCP/IP socket
    connect to server 
    send messages to server
    close connection

"""

import socket
import sys
import time

messages = ['abcd']

server_address = ('localhost', 8000)

# Create a TCP/IP socket
socks = [ socket.socket(socket.AF_INET, socket.SOCK_STREAM)]

# Connect the socket to the port where the server is listening
print >>sys.stderr, 'connecting to %s port %s' % server_address
for s in socks:
    s.connect(server_address)

# send one message at a time 
for message in messages:
    for s in socks:
        print >>sys.stderr, '%s: sending "%s"' % (s.getsockname(), message)
        s.send(message)
        time.sleep(1)

# close socket connection
for s in socks:
    print >>sys.stderr, 'closing socket', s.getsockname()
    s.close()
