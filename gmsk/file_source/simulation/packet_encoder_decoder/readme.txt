Simulation example based on these two examples:

https://youtu.be/kplMaL7e2l4 

https://advancedpersistentjest.com/2017/04/17/simple-packets-via-gnuradio/ 


Questions?

1. What is the purpose of the multiply const block with a value of 1?
2. What is the purpose of the low pass filter? Does it make sense that
   the filter is about 1/2 the sample rate, which is also the bandwidth?
   Likely there is noise outside this band, that cannot be seen in the FFT
   plot.