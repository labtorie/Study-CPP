#include "pch.h"
#include <iostream>
#include <string>
#include "huffman_coder.h"
#include <conio.h>

using namespace std; 

int main()
{
	while (true) {
		string _string;
		cout << "[INPUT]: ";
		getline(cin, _string);

		HuffmanCoder(_string).encode();

		_getch();
		system("cls");
	}
}