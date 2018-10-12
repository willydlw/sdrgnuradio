Flowgraph: view_fm_band.grc

Description: Use the HackRF One to view the FM band.

The tuning frequency variable is set to 98 MHz, with a
bandwidth of 20 MHz. This will show the entire FM radio
band in the frequency plot.

Stronger signals will have higher peaks. These are the FM
radio stations in your area. Move the mouse cursor overflow
the higher peaks to see the frequency.

You will not be able to hear
the radio stations as this flowgraph does not have an 
audio sink to play the sound. The flowgraph listen_fm.grc
builds upon this example to add audio output.


Experiments:
1. Change the bandwidth to 10 MHz. Notice that with the
   center tuning frequency at 98 MHz, you now only see
   the range 93 MHz to 103 MHz in the frequency plot.

2. Keep the bandwidth at 10 MHz. Change the tuning frequency
   to 100 MHz. You will only see a range of 95 MHz to
   105 MHz in the frequency plot.


Notes: 

1. The United States FM radio band is 88 MHz to 108 MHz.

2. With the sample rate variable, samp_rate, at 20 MHz, 
   a steady string of O's printed in the gnuradio console 
   output. This indicates overflow. You may or may not 
   experience this. I had to lower the sample rate to 
   10 MHz to stop the overflow.

   " "O" means there has been an overflow, some part of your 
    system is not fast enough to keep up with the incoming data, 
    probably your hard drive, or you may not have a fast enough 
    CPU to process as the sample rate you have chosen."

    https://lists.gnu.org/archive/html/discuss-gnuradio/2012-02/msg00490.html 

3. The HackRF One sample rate is 2 MHz - 20 MHz.
