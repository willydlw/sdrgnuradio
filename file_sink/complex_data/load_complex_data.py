''' load complex data from a file to a numpy array
'''
import numpy

# load data that was saved with real on first line, complex on second line
array1 = numpy.loadtxt('differentline.txt').view(complex)


# load data that was saved with real and complex on the same line
array2 = numpy.loadtxt('sameline.txt').view(complex)

print "array1"
print array1

print "\narray2"
print array2
