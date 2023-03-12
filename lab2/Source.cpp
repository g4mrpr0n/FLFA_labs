#include "Grammar.h"
#include "FiniteAutomaton.h"

using namespace std;

int main(int argc, int* argv[])
{
	srand(time(NULL));
	//Grammar a; //b, c, d, e;
	FiniteAutomaton f;
	//cout << f.stringBelongToLanguage("bab") << "\n";
	//string A = a.generateString();
	//string B = b.generateString();
	//string C = c.generateString();
	//string D = d.generateString();
	//string E = e.generateString();
	//cout << A << " " << f.stringBelongToLanguage(A) << "\n";
	//cout << B << " " << f.stringBelongToLanguage(B) << "\n";
	//cout << C << " " << f.stringBelongToLanguage(C) << "\n";
	//cout << D << " " << f.stringBelongToLanguage(D) << "\n";
	//cout << E << " " << f.stringBelongToLanguage(E) << "\n";
	//Grammar g(f,6,3,3,'2');
	//for (int i = 0; i < 7; i++)
	//{
	//	cout << g.getP()[i] << "\n\n";
	//	if (i < 3)
	//	{
	//		cout << "Vn " << g.getVn()[i] << "\n\n";
	//	}
	//	if (i < 3)
	//	{
	//		cout << "Vt " << g.getVt()[i] << "\n\n";
	//	}
	//}
	f.NFADFA();
	//a.classification();
}