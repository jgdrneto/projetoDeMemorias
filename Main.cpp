#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>

#include "LIMS.cpp"

using namespace std;

int main(int argc, char const *argv[]){	
	
	LIMS lims;

	while(1){

		cout << "\n ========================================================== \n " << endl;

		lims.update();

		cout << "Memória total: "	<< lims.getMemoria()	<<  
				" SWAP : " 			<< lims.getSwap() 		<<
				" Cache: "			<< lims.getCache()		<<
				" Minor faults: "	<< lims.getMifaults()	<<
				" Major faults: "	<< lims.getMjfaults()	<< endl;

		cout << "\n" << endl;		

		cout << lims.getInformacoesRelevantes() << endl;

		cout << "\n ========================================================== \n " << endl;

		usleep(2000000);
	
	}
	
}	