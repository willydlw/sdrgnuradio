''' convert gnu radio binary file sink data to readable text file

    gnuradio data stored in output.bin file

    The gnuradio data type is complex

    Use scipy.complex64 to convert binary input to complex values
'''
import scipy
import numpy

f = scipy.fromfile(open("output.bin"), dtype=scipy.complex64)

# view writes real and imaginary part on the same line
numpy.savetxt("sameline.txt", f.view(float).reshape(-1,2))

# writes one value per line. writes the real first and complex next
numpy.savetxt("differentline.txt", f.view(float))
