''' convert gnu radio binary file sink data to readable text file

    gnuradio data stored in output.bin file

    The gnuradio data type is float

    Use scipy.floa32to convert binary input to 32 bit floating point
'''
import scipy
import numpy

f = scipy.fromfile(open("output.bin"), dtype=scipy.float32)
numpy.savetxt("readable.txt", f) 
   
'''%f float or %lf long float
'''
f.tofile("readable.txt",sep="\n",format="%f")