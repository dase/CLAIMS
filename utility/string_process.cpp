/*
 * string_process.cpp
 *
 *  Created on: Apr 18, 2015
 *      Author: wangli
 */

#include "string_process.h"





std::string trimSpecialCharactor(string str) {
	int l=0,r=str.length()-1;
	for(int p=0;p<r;p++){
		if(str[p]=='\r' ||str[p]=='\n'||str[p]==' ' ){
			l=p+1;
			continue;
		}
		break;
	}
	for(int p=r;p>l;p--){
		if(str[p]=='\r' ||str[p]=='\n'||str[p]==' ' ){
			r=p-1;
			continue;
		}
		break;
	}
	return str.substr(l,r-l+1);
}

