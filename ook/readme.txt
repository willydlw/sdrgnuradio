
OOK Examples

OOK stands for On-Off Keying. The system signals a 1 by transmitting a carrier with the maximum
possible amplitude. It signals a 0 by transmitting no carrier at all.

The following gnuradio flowgraph examples demonstrate how to create an OOK receiver. 


========= Program File: rx_iqfile_demodulation.grc ======================

Description: reads file containing on/off keying iq signal.

 Source file name: scan_00_c313M_s8M.iq 

 The actual on/off signal data is located at 310 MHz. 
 The center frequency of the captured spectrum is 313 MHz.


 
 Program variables:

 freq 			310 MHz
 center_freq 		313 MHz

 samp_rate		8M
 working_samp_rate	400k

 filter_cutoff		500k
 filter_transition  	1k



 Program flow:
 	1. Open source file
 	2. Throttle the data to avoid overwhelming the cpu. 
 	3. Apply low pass FIR filter.
 		The signal is tuned by downshifting the entire captured spectrum. 
 		The center frequency passed to the filter is (freq - center frequency) = (310e6 - 313e6) = -3e6.

 		The filter cutoff is 15 kHz, with a 1 kHz transition band.

 		The result is decimated by 20 to create the working sample rate of 400 k.  8M / 20 = 400 k

 	4. The magnitude of the complex filtered signal is calculated.
 	5. There are two time sink plots. One plots the baseband magnitude, and the other plots the complex signal.


 	The time sink will alternate between showning nothing where there was no data and then the square wave and sinusoidal data will scroll by at times.

 The next program then sets the time sink trigger level to better show the data.


================================================================

Program File: rx_iqfile_demodulation_trigger_output.grc

The following changes were made to the program file rx_iqfile_demodulation.grc 


Set the trigger on all of the gui time sink blocks
Trigger tab
	set trigger mode to AUTO
	set trigger level to 0.5


Once triggered, you will see the data. There are multiple samples of data in this file. Change the number of points in the time sink to a power of 2, such as 8192 to see more samples.

Now change it to 16384 and you will see all of the samples plus some points that have no data. This shows all of the transmission data.

Finally, change the trigger delay to 1e-3 and you see a clear beginning to end of transmission.


================================================================


Program: rx_clean_binary.grc


Description: convert floating point magnitude to binary waveform.

This program adds a variable and blocks to the rx_iqfile_demodulation_trigger_output.grc program.



Program Variables added:

threshold	0.3	



Program blocks added:

Binary slicer block takes a floating point input and outputs a byte with a value of 1 (hexadecimal notation 0x01) if the input value is greater than or equal to 0.0. In all other cases, the output byte is a 0 (ox00).

Examining the floating point data in this file, there are many data points close to 0.0, that could be converted to 1's instead of 0's. Any noise in the signal could increase the magnitude as well, causing these 0's to be converted to 1's.

This means we need additional processing before using the binary slicer block.

The signal amplitude will be shifted down by -0.3. Before shifting, we see that the magnitude ranges from 0 to 0.7 approximately. To center the shifted data around 0.0, we add -0.3, using the ADD CONST block and the threshold variable. Another time sink is added to display the shifted output.

The binary slicer block is connected to the add constant block to produce a byte output. We want to view this output, but the time sink block does not display bytes. Therefore, a uchar to float block is added to connect to another time sink. The output is shown as Clean Baseband.










Reference: The data file and source program examples are from the book Field Expedient SDR: Basic Digital Communications, Paul Clark and David Clark
