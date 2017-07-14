#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <string>
using namespace std;

int asdfasdfmain()
{
	ifstream result("result.txt");
	ofstream rgb("rgb.txt");
	ofstream depth("depth.txt");
	string item;
	while (result >> item)
	{
		rgb << item<<" ";
		result >> item;
		rgb << item<<endl;
		result >> item;
		depth << item << " ";
		result >> item;
		depth << item<<endl;
	}
}