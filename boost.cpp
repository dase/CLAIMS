/*
 * boost.cpp
 *
 *  Created on: May 11, 2013
 *      Author: wangli
 */



#include <Theron/Theron.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
//#include <boost/archive/binary_oarchive.hpp>
//#include <boost/archive/binary_iarchive.hpp>
#include <fstream>
#include <iostream>
#include <sstream>


// Simple message type that can be copied with memcpy so safely sent over the network.
struct TextMessage
{
    explicit TextMessage(const char *const text)
    {
        mText[0] = '\0';
        strcpy(mText, text);
    }

    char mText[256];
};

// In order to be sent over the network, message types must be registered.
THERON_REGISTER_MESSAGE(TextMessage);
THERON_REGISTER_MESSAGE(double);
class Printer : public Theron::Actor
{
public:

    Printer(int no,Theron::Framework &framework, const char *const name) :
      Theron::Actor(framework, name),no(no)
    {
        RegisterHandler(this, &Printer::Handler);
    }

private:

    void Handler(const TextMessage &message, const Theron::Address from)
    {
        printf("%s\n", message.mText);
        if (strcmp(message.mText, "exit") == 0)
        {
            // Signal we're done.
        	if(no==0)
        		;
        		//Send(0, Theron::Address("receiver"));
        	else
        		Send(0, Theron::Address("receiver1"));
        }
    }
    int no;
};

class Printer1 : public Theron::Actor
{
public:

    Printer1(int no,Theron::Framework &framework, const char *const name) :
      Theron::Actor(framework, name),no(no)
    {
        RegisterHandler(this, &Printer1::Handler);
    }

private:

    void Handler(const double &message, const Theron::Address from)
    {
        printf("Printter1:%f\n",message);
//        if (strcmp(message.mText, "exit") == 0)
//        {
//            // Signal we're done.
//        	if(no==0)
//        		Send(0, Theron::Address("receiver"));
//        	else
//        		Send(0, Theron::Address("receiver1"));
//        }
    }
    int no;
};

int server();
int client();

/**
 * The sender do not need to connect to the receiver, but it is not true vasa viser.
 */

int main234()
{





	/*
	printf("Begins!\n");
	const  A a(3);
	const int iii=3;


	std::ostringstream ss;

	boost::archive::binary_oarchive oa(ss);
	oa << a;

	std::string str=ss.str();

	printf("\nlength=%d",str.length());

	std::istringstream is(str+"                                             asf    ");
	boost::archive::binary_iarchive ia(is);
	int b=10;

	A bb(5);
	ia >> bb;
	bb.p();
	//printf("b=%d",b);
	//is.close();*/
	char switcher;
	scanf("%c",&switcher);

	if(switcher=='s')
	{
		server();
	}
	else
	{
		client();
	}
	return 1;
}

int server()
{
	printf("Give me the printer Number : 0 or 1");
	int no;
	scanf("%d",&no);

	char buffer[256] = { '\0' };

	    // Create a local endpoint.
	if(no==0)
	    sprintf(buffer, "tcp://58.198.176.121:5555");
	else
		sprintf(buffer, "tcp://58.198.176.121:5557");


	    Theron::EndPoint endPoint("server", buffer);

	    // Connect to the remote endpoint.

	    	sprintf(buffer, "tcp://58.198.176.121:5556");
	    	if (!endPoint.Connect(buffer))
	    	{
	    		printf("ERROR: Connection failed - check networking is enabled.\n");
	    		return 1;
	    	}



	    // The framework and receiver are tied to the endpoint.
	    if(no==0)
	    {
	    	//Theron::Receiver receiver(endPoint, "receiver");

	    	Theron::Framework framework(endPoint);

	    	Printer printer(no,framework, "printer");
	    	Printer1 printer1(no,framework, "printer1");
	    	//receiver.Wait();
	    	while(1);

	    }
	    else
	    {
	    	Theron::Receiver receiver(endPoint, "receiver1");
	    	Theron::Framework framework(endPoint);

	    	Printer printer1(no,framework,"printer");
	    	receiver.Wait();

	    }


}

int client()
{
	  char buffer[256] = { '\0' };

	    // Create a local endpoint.
	    sprintf(buffer, "tcp://58.198.176.121:5556");
	    Theron::EndPoint endPoint("client", buffer);

	    // Connect to the remote endpoint.
	    sprintf(buffer, "tcp://58.198.176.121:5555");
	    if (!endPoint.Connect(buffer))
	    {
	        printf("ERROR: Connection failed - check networking is enabled.\n");
	        return 1;
	    }
	    else
	    {
	    	printf("Connected to %s\n",buffer);
	    }
	    sprintf(buffer, "tcp://58.198.176.121:5557");
	    if (!endPoint.Connect(buffer))
	    {
	        printf("ERROR: Connection failed - check networking is enabled.\n");
	        return 1;
	    }
	    else
	    {
	    	printf("Connected to %s\n",buffer);
	    }

	    // The framework is tied to the endpoint.
	    Theron::Framework framework(endPoint);

	    // Send messages to the server in a loop until the user enters 'exit'.
	    // Note that if the server hasn't started listening yet it may miss the first messages!
	    printf("Enter lines of text (max 256 chars per line). Type 'exit' to end.\n");

	    while (strcmp(buffer, "exit") != 0)
	    {
	        // Send the text in a messages to the remote 'printer' actor using its unique name.
	        gets(buffer);
	        framework.Send(
	            TextMessage(buffer),
	            Theron::Address(),
	            Theron::Address("printer"));

	        framework.Send(
	  	            double(28.234),
	  	            Theron::Address(),
	  	            Theron::Address("printer1"));
	    }
}

