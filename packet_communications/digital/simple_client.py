import socket
import sys
import time

messages = ['abcd'] #, 'ef', 'ghi']

server_address = ('localhost', 8000)

# Create a TCP/IP socket
clsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
print >>sys.stderr, 'connecting to %s port %s' % server_address
clsocket.connect(server_address)

for message in messages:
    print >> sys.stderr, '%s: sending "%s"' % (clsocket.getsockname(), message)
    clsocket.send(message)
    time.sleep(1)

print >> sys.stderr, 'closing socket', clsocket.getsockname()
clsocket.close()