Flowgraph: fmradio_receive105.grc

Description: Use the HackRF One to listen to an FM
radio station.

This example builds upon the view_fm_band.grc flow graph.

The tuning frequency variable is set to 103.5 MHz, a 
local FM station.

The sample rate variable, samp_rate, is set to 4 MHz.
The HackRF One sample rate is 2 MHz - 20 MHz. If you
change the sample rate, the decimation and interpolation
values will also have to be changed.

Additional Components: 
    Low Pass Filter, WBFM Receive, Audio Sink 

Functional Operation:

    osmocom source -
        receives signal, tuned to CH0 frequency 
    
    DC blocker - 
        filters out direct current from the power supply

    Low Pass Filter - 
        reduces noise from the surrounding stations

    WBFM Receive - (Wideband FM)
        demodulates the received signal 

    Audio Sink - 
        enables computer's audio hardware


Variables:

hackrf_sample_rate:         4 MHz
tuning_frequency:           103.5 MHz

cutoff_frequency:           75 kHz
transition_frequency:       25 kHz 
low_pass_decimation:        20

resampler_decimation:       5
resampler_interpolation:    12

bandwidth:                  10 MHz


Notes: 

1. Ubuntu terminal command to get list of audio devices

   arecord --list-devices


   gives me this output, yours may be different

   **** List of CAPTURE Hardware Devices ****
card 0: PCH [HDA Intel PCH], device 0: ALC255 Analog [ALC255 Analog]
  Subdevices: 0/1
  Subdevice #0: subdevice #0



2. Ubuntu terminal command to find maximum audio sample rate:
    
    pactl list short sinks


    Gives me following output:

    alsa_output.pci-0000_00_1f.3.analog-stereo	module-alsa-card.c	s16le 2ch 44100Hz	SUSPENDED

    
    Says my maximum audio sample rate is 44100 Hz, but this example works on my computer
    at 48 kHz.


3. "The FM stations are assigned center frequencies at 200 kHz 
    separation starting at 88.1 MHz, for a maximum of 100 stations. 
    These FM stations have a 75 kHz maximum deviation from the center 
    frequency, which leaves 25 kHz upper and lower "guard bands" to 
    minimize interaction with the adjacent frequency band."

    http://hyperphysics.phy-astr.gsu.edu/hbase/Audio/radio.html


4. Flowgraph based on video tutorial: 
    https://www.youtube.com/watch?v=ye8wFVPF4wI&list=PLOIjAcRJHAoGS_LuAmM_10MSo9SANKZEK&index=2 
