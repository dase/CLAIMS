#include <stdio.h>
/*
 * TheronTest.cpp
 *
 *  Created on: Mar 28, 2014
 *      Author: wangli
 */
#include <stdio.h>
#include <Theron/Theron.h>
#include <unistd.h>
int mainasdfasdf324(){
	int master;
	printf("Master or slave?");

#ifdef xs
	printf("xs is defined!\n");
#endif

	scanf("%d",&master);
	if(master==0){
		Theron::EndPoint endpoint("endpoint_master","tcp://127.0.0.1:10009");

	}
	else{
		Theron::EndPoint endpoint("endpoint_slave","tcp://127.0.0.1:10001");
		if(endpoint.Connect("tcp://127.0.0.1:10009")){
			printf("Connect successfully!\n");
		}
		else{
			printf("connect fails!\n");
		}
	}
	while(true){
		sleep(1);
	}
}
//#include <stdio.h>
//#include <string>
//
//#include <Theron/Theron.h>
//
//
//// Actor type that prints strings.
//// Derives from Theron::Actor.
//class Printer : public Theron::Actor
//{
//public:
//
//    // Constructor, passes the framework to the baseclass.
//    Printer(Theron::Framework &framework) : Theron::Actor(framework)
//    {
//        // Register the message handler.
//        RegisterHandler(this, &Printer::Print);
//    }
//
//private:
//
//    // Handler for messages of type std::string.
//    void Print(const std::string &message, const Theron::Address from)
//    {
//        // Print the string.
//        printf("%s\n", message.c_str());
//
//        // Send a dummy message back for synchronization.
//        Send(0, from);
//    }
//};
//
//
//int main()
//{
//    // Construct a framework and instantiate a Printer within it.
//    Theron::Framework framework;
//    Printer printer(framework);
//
//    // Construct a receiver to receive the reply message.
//    Theron::Receiver receiver;
//
//    // Send a string message to the Printer.
//    // We pass the address of the receiver as the 'from' address.
//    if (!framework.Send(
//        std::string("Hello world!"),
//        receiver.GetAddress(),
//        printer.GetAddress()))
//    {
//        printf("ERROR: Failed to send message\n");
//    }
//
//    // Synchronize with the dummy message sent in reply to make sure we're done.
//    receiver.Wait();
//}

