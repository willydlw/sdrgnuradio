1. packet_encoder_decoder.grc

This simulation tests the Packet Encoder/Decoder blocks, using
an image file source. It is easy to visually compare the output
image to the input.

The input and decoded output bytes are written to the binary files:
in.bin, decoded.bin to allow a byte by byte difference comparison.

The variables code1, code2 provide examples of different access codes
to test with the encoder/decoder. The default access code may be used
as well in testing.

There are no modulation/demodulation blocks in this simulation.