#include <iostream>
#include <vector>
#include <list>
#include <forward_list>
#include <deque>
#include <memory>
#include <thread>
#include <iterator>

class Kaka
{
	public:
		Kaka() { std::cout << "Kaka" << std::endl; };
};

class Popa
{
	private:
		Kaka	k;
	public:
		Popa() { std::cout << "cnstr" << std::endl; };
		Popa(const Popa & src) { std::cout << "copy cnstr" << std::endl; };
		~Popa() { std::cout << "destr" << std::endl; };
};

int main(void)
{
	// Popa	a;
	// std::vector<Popa>(10);
	std::vector<int>			a;
	std::list<int>		b;

	for (int i = 0; i < 10; i++)
	{
		b.push_back(i);
		a.push_back(i);
	}

	// std::reverse_iterator<std::list<int>::iterator>	aa(b.end());
	// std::reverse_iterator<std::list<int>::iterator>	bb(b.begin());
	std::reverse_iterator<std::vector<int>::iterator>	aa(a.end());
	std::reverse_iterator<std::vector<int>::iterator>	bb(a.begin());
	std::cout << (aa > bb) << std::endl;
	// std::reverse_iterator<std::vector<int>::iterator>	cc;
	
	// std::cout << *aa << std::endl;
	// std::cout << *aa.base() << std::endl;
	// std::cout << *bb << std::endl;
	// std::cout << *bb.base() << std::endl;
	// aa++;
	// aa++;
	// std::cout << *aa << std::endl;
	// std::cout << *aa.base() << std::endl;

	// while (aa != bb)
	// {
	// 	std::cout << &(*aa) << std::endl;
	// 	std::cout << &(*aa.base()) << std::endl << std::endl;
	// 	aa++;
	// }
	// std::cout << &(*a.end()) << std::endl;
	// std::cout << &(*aa) << std::endl;
	// std::cout << &(*aa.base()) << std::endl << std::endl;

	// std::cout << &(*a.begin()) << std::endl;
	// std::cout << &(*bb) << std::endl;
	// std::cout << &(*bb.base()) << std::endl;
	// std::cout << "asd" << std::endl;
}