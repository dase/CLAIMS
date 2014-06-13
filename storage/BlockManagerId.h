/*
 * BlockManagerId.h
 *
 *  Created on: 2013-10-20
 *      Author: casa
 */

#ifndef BLOCKMANAGERID_H_
#define BLOCKMANAGERID_H_

/*
 * 在spark中是一个类，在我们系统中BlockmanagerId也是一个类（在一个单独的文件中），
 * 这样有利于多个文件引用而避免include环，这个类以后会被覆盖掉
 * */
#include <string>
using namespace std;

#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#ifdef DMALLOC
#include "dmalloc.h"
#endif

static string getIpFromLocal(){
	char * ipLocal=0;
    char hname[128];
    struct hostent *hent;
    int i=0;
    gethostname(hname, sizeof(hname));

    hent = gethostbyname(hname);

    for(i = 0; hent->h_addr_list[i]; i++) {
        ipLocal=inet_ntoa(*(struct in_addr*)(hent->h_addr_list[i]));
    }
	return ipLocal;
}

class BlockManagerId {
public:
	BlockManagerId();
	BlockManagerId(string bmi);
	virtual ~BlockManagerId();

	string blockManagerId;
};

#endif /* BLOCKMANAGERID_H_ */
