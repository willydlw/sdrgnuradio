The programs tx_stage0.grc through tx_stage4.grc are derived from the gnuradio examples of
the same name. The Message strobe and RandomPDU source has been replaced with a SocketPDU 
source.

All programs are run in the nogui state. Any qtgui elements have been disabled, but left
in the flowgraph so they can be turned back on easily and run as qtgui applications.

***** Stage 0 Testing  *****

tx_stage0.grc generates the python file stage0.py

% python stage0.py

Run the test client programs one at a time. The stage0.py program must be started
before the test client programs. Do no press the enter key to quit before the client
program finishes running.


============ Test Client Programs ========================

% python string_client.py 

The Debug Message PDU output will show the pdu_length to be the same as the
string length. The contents are the hexadecimal values of the ASCII characters.
Notice there is one byte for each character.



% python send_unsigned_char.py

The Debug Message PDU will show the pdu length to be one as only one byte is 
transmitted at a time. The contents are the hexadecimal byte values.



% python send_two_unsigned_char.py

The Debug Message PDU will show the pdu length to be two as two bytes are 
transmitted at a time. The contents are the hexadecimal byte values.



% python send_unsigned_integer.py

The Debug Message PDU will show the pdu length to be four as four bytes are 
transmitted on my system for an unsigned integer, 32 bits. 
When the packed format is I: packed_data = struct.pack("I", i) and i is 8, the
least significant byte is transmitted first on my system.

PDU contents: 08 00 00 00     (little-endian)



Changing the packed format to: packed_data = struct.pack("!I", i)
The character ! defines byte order as network(=big-endian). The most significant
byte is transmitted first.

PDU contents: 00 00 00 08     (big endian)

