#include "thread.h"
#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>

using namespace std;

using namespace Sync;

int main(void)
{
	// Welcome the user 
	std::cout << "SE3313 Lab 3 Client" << std::endl;

	string response;
	ByteArray message;
	string data;

	// Create our socket
	Socket socket("127.0.0.1", 3000);
	socket.Open();

	//To write to socket and read from socket. You may use ByteArray 
	do {

		cout << "Enter a message for the server: ";
		cin >> response;

		ByteArray packet(response);
		
		socket.Write(packet); // write the user input 
		socket.Read(message); // read server output

		data = message.ToString();
		cout << "Message Received from server: " << data << endl;
		cout << " " << endl;		

	} while (response != "done");

	cout << "Client will now exit" << endl;
	
	socket.Close();

	return 0;
}

