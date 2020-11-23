#include "thread.h"
#include "socketserver.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <cctype>
#include <cstring>
#include <vector>

using namespace Sync;
using namespace std;
bool checker = false;
int counter = 0;

void reverseString(string& str) { // String reversal function for incoming inputs from client
    int n = str.length();

    for (int i = 0; i < n / 2; i++) {
        swap(str[i], str[n - i - 1]);
    }
};

vector <Thread*> clientthreads;

// This thread handles the server operations
class ServerThread : public Thread
{
private:
    SocketServer& server;
    
public:
    bool check;    
           
    ServerThread(SocketServer& server)
    : server(server)
    {
                       
    }

    ~ServerThread() {

    }      

    virtual long ThreadMain()
    {
        while(true) {
        // Wait for a client socket connection
        Socket* newConnection = new Socket(server.Accept());
        counter += 1;
        clientthreads.push_back(new ServerThread(server));
        
        string message = ""; 
        string flag = "";          

        // A reference to this pointer 
        Socket& socketReference = *newConnection;

	    //You can use this to read data from socket and write data to socket. You may want to put this read/write somewhere else. You may use ByteArray
	    // Wait for data
        ByteArray data;

        do {       
            // Read Data
            socketReference.Read(data);
            
            // Convert for readability
            message = data.ToString();
            flag = message; // set condition for client closing       
            
            reverseString(message); // reverse the string

            cout << "Message Received: The reverse string is " << message << " and will now be sent back!" << endl;

            // Send it back
            ByteArray response(message);
            socketReference.Write(response);                
            
            

        } while (flag != "done");        
        
        cout << "Client ended connection on their side" << endl;
        return 1;
    }
}
};

int main(void)
{
    std::cout << "I am a server." << std::endl;
    string input;
    
    // Create our server
    SocketServer server(3000);    
    
    // Need a thread to perform server operations
    ServerThread serverthread(server);
    
	// This will wait for input to shutdown the server
    FlexWait cinWaiter(1, stdin);
    cinWaiter.Wait();

    for (int i = 0; i < clientthreads.size(); i++) {
        cout << "The size of the vector is " << clientthreads.size() << endl; 
        cout << clientthreads[i] << endl;
        delete clientthreads[i];
    }   
    
    // Shut down and clean up the server
    server.Shutdown();
}
 
