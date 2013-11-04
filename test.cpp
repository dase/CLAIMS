/*
 * test.c
 *
 *  Created on: May 9, 2013
 *      Author: wangli
 */
#include <Theron/Theron.h>
#include <map>
#include <stdio.h>
#include <unistd.h>
using namespace std;

class Actor:public Theron::Actor
{
public:
	explicit Actor(Theron::Framework &framework): Theron::Actor(framework)
	{
		RegisterHandler(this,&Actor::Handler);
	}

private:
	void Handler(const int &message, const Theron::Address from)
	{
		sleep(3);
		printf(":)\n");
		Send(message, from);
	}
};




int main12()
{
	Theron::Receiver receiver;
	Theron::Framework framework;
	Actor actor(framework);

	framework.Send(int(0), receiver.GetAddress(), actor.GetAddress());


	receiver.Wait();


	framework.Send(int(0), receiver.GetAddress(), actor.GetAddress());


	receiver.Wait(1);




	map<int,int> hm;
	hm[10]=5;
	hm[3]=2;

	int input=0;;
	while(input>-1)
	{
		scanf("%d",&input);
		if(hm.find(input)!=hm.end())
		{
			printf("found!\n!");

		}
		else
		{
			printf("not found!");
		}
	}
	for(map<int,int>::iterator it=hm.begin();it!=hm.end();it++)
	{
		printf("%d,%d",it->first,it->second);
	}


}

