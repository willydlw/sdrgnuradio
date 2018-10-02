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


for i in range(0,9):
    # https://docs.python.org/2/library/struct.html 
    # I is unsigned integer  
    # The character ! defines byte order as network(=big-endian)
    packed_data = struct.pack("!I", i)
    print >> sys.stderr, '%s: sending "%s"' % (clsocket.getsockname(), binascii.hexlify(packed_data))
    clsocket.sendall(packed_data)
    time.sleep(1)

print >> sys.stderr, 'closing socket', clsocket.getsockname()
clsocket.close()