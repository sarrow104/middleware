

#include "readconfig.h"
#include <iostream>



int main()
{
	fkey_config abc;
	abc.push( "config.csv" , 0 );
	std::cout << abc[0][8][3] << std::endl;
	return 0;
}