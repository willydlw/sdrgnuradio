''' convert gnu radio binary file sink data to readable text file

    gnuradio data stored in output.bin file

    The gnuradio data type is byte.

    Use scipy.uint8 to convert binary input to unsigned 8 bit byte
'''
import scipy
import numpy

f = scipy.fromfile(open("output.bin"), dtype=scipy.uint8)
numpy.savetxt("readable.txt", f)    
'''%x hexadecimal format
   %c character format
   %u unsigned int format
'''
f.tofile("readable.txt",sep="\n",format="%u")