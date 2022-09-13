#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <forward_list>
#include <deque>
#include <memory>
#include <thread>
#include <iterator>
#include <string>
#include <type_traits>
#include "type_traits.hpp"

template <typename T>
class Popa
{
	private:
		T	k;


	public:

		template <typename U>
		class Kaka
		{
			public:
				T	a;
				U	b;
		};

		Popa() { std::cout << "cnstr" << std::endl; };
		Popa(const Popa & src) { std::cout << "copy cnstr" << std::endl; };
		~Popa() { std::cout << "destr" << std::endl; };
};

template <typename T>
typename ft::enable_if<std::is_integral<T>::value, bool>::type	is_odd(T i)
{
	return (i % 2);
};

// template <typename T, typename = typename ft::enable_if<std::is_integral<T>::value>::type >
// bool	is_even(T i)
// {
// 	return !(i % 2);
// }

int main(void)
{
	// const int		*i;
	// const int		*j;

	// std::is_integral
	std::cout << ft::integral_constant<int, 10>::value << std::endl;
	std::cout << ft::integral_constant<int, 10>() << std::endl;

	// i = &std::integral_constant<int, 5>::value;
	// std::cout << *i << std::endl;
	// j = &std::integral_constant<int, 10>::value;
	// std::cout << *j << std::endl;
	// std::cout << is_even(i) << std::endl;
	// std::cout << is_odd(a) << std::endl;
	// std::cout << is_even(a) << std::endl;
	// std::vector<int>	a;

	// a.push_back(1);
	// a.push_back(2);
	// a.push_back(3);
	// a.push_back(4);
	// Popa<int>::Kaka<char>	a;

	// a.a = 5;
	// a.b = 'a';
	// std::cout << a.a << " " << a.b << std::endl;
	// Popa	a;
	// std::vector<Popa>(10);
	// std::vector<int>			a;
	// std::list<int>		b;

	// for (int i = 0; i < 10; i++)
	// {
	// 	b.push_back(i);
	// 	a.push_back(i);
	// }

	// std::reverse_iterator<std::list<int>::iterator>	aa(b.end());
	// std::reverse_iterator<std::list<int>::iterator>	bb(b.begin());
	// std::reverse_iterator<std::vector<int>::iterator>	aa(a.end());
	// std::reverse_iterator<std::vector<int>::iterator>	bb(a.begin());
	// std::cout << (aa > bb) << std::endl;
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