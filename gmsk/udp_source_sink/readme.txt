There are multiple directories and programs within folder. 

Directory: gnuradio
    udp_tx_rx_sim.grc  - Simulation
    
        program data source: udp_source that receives data from an external udp client program 
        simulation uses packet encoding, gmsk modulation/demodulation, packet decoding, and 
        a udp sink that sends data to an external udp server program.

    
    To build executable python program, open the file udp_tx_rx_sim.grc with gnuradio-companion
    and press the generate flowgraph button. This creates the file udp_tx_rx_sim.py


Directory: clientserver

    txclient.c - client program that provides gnuradio UDP source with payload data 

        usage: etxclient hostname port packetLength totalPackets
    
        program flow: 
            opens socket connection to same IP address (hostname) and port number as UDP source
            creates packets of length packetLength
                The first two bytes of the packet are a unique packet number, starting a 0 and
                incrementing by 1 as each packet is created. The remaining packet bytes are filled
                with random values in the range [0,255]

            Each packet is written to the file: packetsTransmitted.bin 

            The total number of packets created is specified by the command line value for totalPackets.

        Note: A dummy packet with packet number 0xFFFF and bytes filled with zeros is sent before 
        the program terminates. This is due to the gnuradio program not processing the last packet sent.
        The last packet sent is received by the gnuradio program and recorded in the file udpsource.bin,
        but the packetDecoded.bin and udprecived.bin files never contain the last value. The reason is
        not known at this time. (11/8/2018)

    txrxhelper.c, txhelper.h - programs contain functions that record data, close connections, and
        create packets.


    rxserver.c - server program that receives udp packets from the gnruadio UDP Sink 

        usage: erxserver

        At this time, the rxserver program is hard coded to the same IP address and port as the 
        gnuradio UDP sink, 127.0.0.1, 8080.

        The program must be terminated with the signal interrupt ctrl+c. This will cause the receive
        loop to stop running and close all connections before terminating. 

    To build the executables in this directory, type make. Two executables: etxclient, erxserver will
    be created.


There are three programs that must be running at the same time to run the simulation:
    erxserver, udp_tx_rx_sim.py, etxclient 

    The programs must be run in this order, with the following commands
    erxserver
    python udp_tx_rx_sim.py
    etxclient localhost 8000 16 5 

    Note that you may use numbers other than 16 for packet length, and 5 for total number of packets.

    When the txclient program stops transmitting packets, it will terminate and print a message that
    all packets have been transmitted. After that, you can press any key to terminate the python program,
    and press ctrl c to terminate the rxserver program.

    The binary data files produced are stored in the directory recorded_data.


Directory: recorded_data

    This directory will not exist after cloning the repository files, as the data files generated
    are not stored in the repository. You will need to create it the first time as this is the 
    hard-coded default path for data files.

    After running the simulation, there will be four binary files in this directory: 
        packetsTransmitted.bin, udpsource.bin, packetsDecoded.bin, packetsReceived.bin 


Directory: check_packet_count

    The programs in this directory are designed to report missing packet numbers from the files
    in the recorded data directory.

    Use the makefile to build the executable.

    usage echeck filename packetLength totalPackets

    The program writes the overall stats to standard error stream and records more detailed 
    data in files written to the reports directory.

    Before running the executable for the first time, create a reports subdirectory.


TODO:
    Add code to send a SIGTERM to rxserver program when txserver program is finished 
    sending packets.