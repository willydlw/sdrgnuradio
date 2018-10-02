1. Run the gnuradio flow graph complex.grc to generate the file output.iq, that contains complex data
2. Run the python program read_complex_data.py that creates two files: sameline.txt and differentline.txt
    sameline.txt has the I and Q data on the same line
    differentline.txt has the I data on the first line and Q data on the second line
3. load_complex_data.py is an example program that loads the .txt files as complex numbers.