#include "cmysql/c_mysql_server.h"

int main(){
	CMysqlServer *server=new CMysqlServer();
	server->start();
	return 0;
}
