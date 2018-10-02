import binascii
import socket
import sys
import time
import struct 


server_address = ('localhost', 8000)

# Create a TCP/IP socket
clsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
print >>sys.stderr, 'connecting to %s port %s' % server_address
clsocket.connect(server_address)


for b in range(0,10, 2):
    # https://docs.python.org/2/library/struct.html 
    # B is the format for unsigned char
    packed_byte = struct.pack("BB", b, b+1)
    print >> sys.stderr, '%s: sending "%s"' % (clsocket.getsockname(),  binascii.hexlify(packed_byte))
    # note function sendall is used as send requires a string
    clsocket.sendall(packed_byte)
    time.sleep(1)

print >> sys.stderr, 'closing socket', clsocket.getsockname()
clsocket.close()