#include <iostream>
#include <unistd.h>

using namespace std;

int main(){
	for (int i = 0; i < 10; i++){
		cout << i;
		sleep(2);
		system("clear");
	}
}
