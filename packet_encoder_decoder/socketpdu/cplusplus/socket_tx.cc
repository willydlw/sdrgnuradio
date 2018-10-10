/*
 * Copyright 2011,2013 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */


/*
 * GNU Radio C++ example creating a socket tcp server
 * that receives data from a client and prints the 
 * received data to standard output.
 * 
 * The protocol data unit must be converted to a stream
 * for processing, via the PDU to Tagged Stream function.
 * 
 * The stream data is written to a file.
 *
 * Input
 *  Socket Protocol Data Unit
 *    Type: TCP Server
 *    Host   localhost
 *    Port   8000
 * 
 * Output
 *  Message Debug - prints pdu information to stdout
 * 
 *
 */

// Include header files for each block used in flowgraph
#include <gnuradio/top_block.h>
#include <gnuradio/blocks/message_debug.h>
#include <gnuradio/blocks/socket_pdu.h>
#include <gnuradio/blocks/pdu_to_tagged_stream.h>
#include <gnuradio/blocks/file_sink.h>
#include <gnuradio/blocks/tag_debug.h>

using namespace gr;

int main(int argc, char **argv)
{
  // Construct a top block that will contain flowgraph blocks.  Alternatively,
  // one may create a derived class from top_block and hold instantiated blocks
  // as member data for later manipulation.
  top_block_sptr tb = make_top_block("send_to_socket");

  // Construct a socket_pdu source
  blocks::socket_pdu::sptr socket_pdu_0 = blocks::socket_pdu::make("TCP_SERVER", "127.0.0.1", "8000");

  // Construct a message debug block
  blocks::message_debug::sptr message_debug_0 = blocks::message_debug::make();

  // Construct a tag debug block
  blocks::tag_debug::sptr tag_debug_0 = blocks::tag_debug::make(sizeof(char), "", "");
  tag_debug_0->set_display(true);

  // Construct a pdu_to_tagged_stream block, stream data type is byte,
  // name of the tag that specified packet length
  blocks::pdu_to_tagged_stream::sptr pdu_to_tagged_stream_0 = 
                blocks::pdu_to_tagged_stream::make(blocks::pdu::byte_t,"packet_len");

  // Construct a file sink to store data stream
  blocks::file_sink::sptr file_sink_0 = blocks::file_sink::make(sizeof(char), "tag_stream.bin");
  /*##################################################
  # Connections
  ################################################## */
  
  // Connect socket pdu output to message debug print pdu input
  tb->msg_connect(socket_pdu_0, "pdus", message_debug_0, "print_pdu");

  // Connect socket pdu output to pdu to tagged stream input
  tb->msg_connect(socket_pdu_0, "pdus", pdu_to_tagged_stream_0, "pdus");

  tb->connect(pdu_to_tagged_stream_0, 0, tag_debug_0, 0);
  tb->connect(pdu_to_tagged_stream_0, 0, file_sink_0, 0);
  

  // Tell GNU Radio runtime to start flowgraph threads; the foreground thread
  // will block until either flowgraph exits (this example doesn't) or the
  // application receives SIGINT (e.g., user hits CTRL-C).
  //
  // Real applications may use tb->start() which returns, allowing the foreground
  // thread to proceed, then later use tb->stop(), followed by tb->wait(), to cleanup
  // GNU Radio before exiting.
  //tb->run();

  tb->start();
  
  // equivalent of python's raw_input function
  // program terminates normally when user presses enter key
  // If program does not terminate normally (Ctrl + C), data will not be
  // written to the output file.
  try {
    std::string quitString;
    std::cout << "Press enter to quit: ";
    getline(std::cin, quitString);
  }
  catch(int e){
    std::cout << "An exception occurred. Exception number: " << e << '\n';
  }

  tb->stop();
  tb->wait();

  


  // Exit normally.
  return 0;
}