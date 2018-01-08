/*
**	g++ main.cpp -ljpeg -lm -fpermissive -o syncar.out
*/
#include "base/httplib.h"
#include <iostream>
using namespace std;

int main(void){
	httplib::Client cli("166.111.66.49", 8001);
	
	auto res = cli.post("/uploadinfor/", "name=john1&note=coder", "application/x-www-form-urlencoded");
    if (res && res->status == 200) {
        cout << res->body << endl;
    }else{
		cout << "failed" << endl;
	}
}