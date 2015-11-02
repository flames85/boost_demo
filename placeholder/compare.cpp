#include <boost/bind.hpp> 
#include <vector> 
#include <algorithm> 
#include <iostream> 

bool compare(int i, int j) 
{ 
	std::cout << i << j << std::endl;
	return i > j; 
} 

int main() 
{ 
	std::vector<int> v; 
	v.push_back(1); 
	v.push_back(3); 
	v.push_back(2); 

	std::sort(v.begin(), v.end(), boost::bind(compare, _1, _2)); 
} 
