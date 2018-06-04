/*
Christopher Robertson
Algorithms Project 2
2/14/17
*/
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

int fExp(int, int, int);
int gcd(int, int);
int gcdExtended(int, int, int *, int *);
int modInverse(int, int);
void genPrime(int &, int &);
void genKeys(int, int &, int &);
void encrypt(int, int);
void decrypt(int, int);

int main()
{
	bool repeat = true;
	char input;
	int prime1 = 0,
		prime2 = 0,
		key1 = 0,
		key2 = 0,
		m = 0,
		n = 0;

	cout << "Run module A and B in that order first.";

	do {
		cout << "\n\n\nMain menu\nA: Generate pseudo-prime numbers\nB: Get public and private keys\nC: Encrypt\nD: Decrypt\nE: Exit\nEnter your choice: ";
		cin >> input;
		cin.ignore(256, '\n');
		input = toupper(input);
		switch (input)
		{
			case 'A':
			{
				genPrime(prime1, prime2);
				cout << "Generated prime 1: " << prime1 << endl << "Generated prime 2: " << prime2 << endl << endl;
				m = (prime1 - 1) * (prime2 - 1);
				n = prime1 * prime2;
				break;
			}
			case 'B':
			{
				if (m == 0)
				{
					cout << "You must generate prime numbers first!\n\n";
				}
				genKeys(m, key1, key2);
				cout << "Generated key 1: " << key1 << endl <<  "Generated key 2: " << key2 << endl << endl;
				break;
			}
			case 'C':
			{
				encrypt(key1, n);
				break;
			}
			case 'D':
			{
				decrypt(key2, n);
				break;
			}
			case 'E':
			{
				repeat = false;
				break;
			}
			default:
			{
				cout << "Invalid response. Please try again.\n\n";
			}
		}
	} while (repeat);
	return 0;
}

int fExp(int base, int exp, int mod)
{
	unsigned long long int result = 1; //Used unsigned long long to store intermediate values without further calculations to avoid them
	unsigned long long int b = base;
	unsigned long long int e = exp;
	unsigned long long int m = mod;
	while (e > 0)
	{
		if (e % 2 == 0)
		{
			b = (b * b) % m;
			e = e / 2;
		}
		else
		{
			result = (b * result) % m;
			e--;
		}
	}
	return result;
}

int gcd(int a, int b)
{
	if (a < b)
	{
		int temp = a;
		a = b;
		b = temp;
	}
	if (b == 0)
	{
		return a;
	}
	else
	{
		return gcd(b, a % b);
	}
}

/***********************************************************************\
| gcdExtended and modInverse functions sourced from						|
| http://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/	|
\***********************************************************************/
int gcdExtended(int a, int b, int *x, int *y)
{
	if (a == 0)
	{
		*x = 0, *y = 1;
		return b;
	}

	int x1, y1;
	int gcd = gcdExtended(b % a, a, &x1, &y1);

	*x = y1 - (b / a) * x1;
	*y = x1;

	return gcd;
}

int modInverse(int a, int m)
{
	int x, y;
	int g = gcdExtended(a, m, &x, &y);
	if (g != 1)
	{
		cout << "Inverse doesn't exist.\n\n";
	}
	else
	{
		return (x % m + m) % m;
	}
}
/***************************\
| End of sourced functions	|
\***************************/

void genPrime(int &p1, int &p2)
{
	srand(time(NULL));
	int j = rand() % (1000 - 500 + 1) + 500;
	
	while (p1 == 0)
	{ //They are relatively prime if they aren't divisible by the first few prime numbers
		if (!(j % 2) | !(j % 3) | !(j % 5) | !(j % 7) | !(j % 11) | !(j % 13) | !(j % 17))
		{
			j++;
		}
		else
		{
			p1 = j;
		}
	}
	j = rand() % (1000 - j + 1) + j; //Generate prime 2 in between the max (1000) and the j that was chosen
	while (p2 == 0)
	{
		if (!(j % 2) | !(j % 3) | !(j % 5) | !(j % 7) | !(j % 11) | !(j % 13) | !(j % 17))
		{
			j++;
		}
		else
		{
			p2 = j;
		}
	}
}

void genKeys(int m, int &k1, int &k2)
{
	srand(time(NULL));
	int j = rand() % (2000 - 1000 + 1) + 1000;

	while (k1 == 0)
	{ //Look for a public key that is relatively prime to (p1 - 1) * (p2 - 1)
		if (gcd(j, m) == 1)
		{
			k1 = j;
		}
		else
		{
			j++;
		}
	}
	k2 = modInverse(k1, m); // Calculate the multiplicative inverse of the previously acquired public key, making this our private key
}

void encrypt(int k1, int n)
{
	string str;
	int SIZE;
	cout << "\n\nEnter a string to encrypt:\n";
	getline(cin, str);
	SIZE = str.length();
	int * encrypted = new int[SIZE];
	cout << "Encrypted message using public key: " << endl;
	for (int i = 0; i < SIZE; i++)
	{ // Send the letter to the fast exponential to encrypt it with our public key
		encrypted[i] = fExp(static_cast<int>(str[i]), k1, n);
		cout << encrypted[i] << " ";
	}
	cout << endl;
	delete[] encrypted; //We don't need the dynamic array anymore
}

void decrypt(int k2, int n)
{
	string en; // Accepts the string of numbers from before copied and pasted into the line where it asks for it
	int SIZE;
	cout << "\n\nEnter a string to decrypt:\n";
	getline(cin, en);
	SIZE = en.length();
	char de[100]; //Could've used a vector, but it works well enough
	int j = 0;
	int temp = 0;
	for (int i = 0; i <= SIZE; i++)
	{
		if (en[i] == ' ' || en[i] == '\0')
		{
			de[j] = (char)fExp(temp, k2, n);
			j++;
			temp = 0;
		}
		else
		{
			temp = temp * 10;
			temp += static_cast<int>(en[i]) - 48;
		}
	}
	cout << endl << "Decrypted message using private key: " << endl;
	for (int i = 0; i < j; i++)
	{
		cout << de[i];
	}
	cout << endl;
}