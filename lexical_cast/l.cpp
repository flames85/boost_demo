#include <iostream>
#include "boost/lexical_cast.hpp" 

int main()
{
		std::string str = "123456";
		//int a = boost::lexical_cast<int>(str);
		int a = boost::lexical_cast(str);
		std::cout << a;
		return 0;
}
