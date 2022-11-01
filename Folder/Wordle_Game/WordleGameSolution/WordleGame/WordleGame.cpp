// WordleGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <stdlib.h>
#include <string.h>

using namespace std;

void checker(char s)
{
	int ok = 1;
	if (strlen(sol) != 5)
		cout << "Your word doesn't have 5 letters!"
		for (int i = 0; i < 10; i++)
			if (strcmp(sol, cuv[i]) != 0)
				ok = 0;
	if (ok == 0)
		cout << "This is not a word from the dictionary! wtf bro";
	else
		cout << "Good!"
	
}

int main()
{
	char cuv[10][6];
	char guess,sol;
	int i, tries = 0;
	strcmp(cuv[0], "cizma");
	strcmp(cuv[1], "cadou");
	strcmp(cuv[2], "arena");
	strcmp(cuv[3], "aerat");
	strcmp(cuv[4], "musca");
	cout<<"The game starts!"
	run_game = 1;
		while (run_game)
		cout << "Number of tries: " << tries << '\n';
		cin >> sol;
		tries++;
	
		
	

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
