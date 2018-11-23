/*
 *   C++ sockets on Unix and Windows
 *   Copyright (C) 2002
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "YASL.h"      // For Socket and SocketException
#include "checkArgs.h"
#include <iostream>    // For cerr and cout
#include <cstdlib>     // For atoi()
#include <fstream>

const uint64_t RCVBUFSIZE = 9216;    // Size of receive buffer

int main(int argc, char *argv[]) {

	checkArgs* argumentos = new checkArgs(argc, argv);

    std::string servAddress; 
	uint16_t    echoServPort;
    std::string peticion;
	std::string hostServ;
	std::string archive;

	servAddress   = argumentos->getArgs().SERVER;
	echoServPort  = argumentos->getArgs().PORT;
	peticion    = "GET / HTTP/1.1\nHost:"+servAddress+"\n\n";
	archive       = argumentos->getArgs().ARCHIVO;

	//delete argumentos;	
	uint32_t peticionLen = peticion.length();   // Determine input length
	std::ofstream fs(archive);

	try {
		// Establish connection with the echo server
		TCPSocket sock(servAddress, echoServPort);

		// Send the string to the server
		sock.send(peticion.c_str(), peticionLen);

		char echoBuffer[RCVBUFSIZE + 1];    // Buffer for echo string + \0
		uint32_t bytesReceived = 0;              // Bytes read on each recv()
		uint32_t totalBytesReceived = peticionLen+1 ;         // Total bytes read
		

		// Receive the same string back from the server
		//std::cout << "Received: ";               // Setup to print the echoed string
		while (peticionLen<totalBytesReceived) {
			//printf("%i", sock.recv(echoBuffer, RCVBUFSIZE));
			// Receive up to the buffer size bytes from the sender
			
			if ((bytesReceived = (sock.recv(echoBuffer, RCVBUFSIZE))) <= 0) {				
				std::cerr << "Unable to read\n";
				exit(EXIT_FAILURE);
			}
			totalBytesReceived += bytesReceived;     // Keep tally of total bytes
			echoBuffer[bytesReceived] = '\0';        // Terminate the string!
			std::cout << echoBuffer;                 // Print the echo buffer			
			fs << echoBuffer;
		}
		std::cout << std::endl;
		
		fs.close();
		// Destructor closes the socket

	} catch(SocketException &e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
