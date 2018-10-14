Filename: fm_transmit_frequency_slider.grc

Description: transmits a recorded wav file in the FM band.

Input: .wav file

Due to its large size, the wav file has not been
uploaded to the repository. Any wav file may be
used.

The wav file used for testing had a sample rate of
22050 Hz. Right click on the file properties, audio
tab to see sample rate.

If your file is different, change the flowgraph
variable wav_file_sample_rate, as well as the 
interpolation and decimation values in the rational
resampler block.

The rational resampler block's purpose is to boost
the lower input sample rate to an output sample rate
that matches the osmocom Sink sample rate.