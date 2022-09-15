#include <iostream>
// #include <vector>
// #include <map>
// #include <list>
// #include <forward_list>
// #include <deque>
// #include <memory>
// #include <thread>
// #include <iterator>
// #include <string>
// #include <type_traits>
// #include <stdexcept>
// #include <typeinfo>
// #include "type_traits.hpp"
#include "vector.hpp"

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
		Popa(T k) : k(k) {};
		~Popa() { std::cout << "destr" << std::endl; };

		void puk()
		{
			std::cout << "puk" << std::endl;
		};

		void puk(Popa & src)
		{
			this->k = src.k;
			std::cout << "puk " << this->k << std::endl;
		};
};

// template <typename T>
// typename ft::enable_if<std::is_integral<T>::value, bool>::type	is_odd(T i)
// {
// 	return (i % 2);
// };

// template <typename T, typename = typename ft::enable_if<std::is_integral<T>::value>::type >
// bool	is_even(T i)
// {
// 	return !(i % 2);
// }

int main(void)
{
	// const int		*i;
	// const int		*j;
	// std::vector<int>	a(5);
	// std::vector<int>	b(3);
	ft::vector<Popa<int> >	a(2);
	// ft::vector<int>	a(2);
	// Popa<int> ** 	p;
	// Popa<int>		ref(2);
	// Popa<int>		reff(3);


	// std::cout << "tuta" << std::endl;
	// a.insert(a.begin() + 1, a[0]);
	// std::cout << "tama" << std::endl;

	// ref.puk(reff);
	// for (int i = 0; i < 5; i++)
	// 	a[i] = i + 1;

	// for (int i = 0; i < 3; i++)
	// 	b[i] = (i + 1) * 10;

	// std::cout << &a[0] << std::endl;
	// a.swap(b);
	// std::cout << &b[0] << std::endl;

	// std::cout << a.capacity() << " " << b.capacity() << std::endl;
	// std::cout << "do" << std::endl;
	// b.resize(2);
	// std::cout << "posle" << std::endl;
	// a = b;
	// b[0] = 1;
	// b[1] = 2;
	// std::vector<int>::iterator	c = b.begin() + 1;
	// std::cout << c.base() << std::endl;
	// std::cout << std::boolalpha << (typeid(std::vector<int>::iterator::pointer) == typeid(int *)) << std::endl;
	// std::cout << &(*c) << std::endl;
	// std::vector<int>::iterator	d = b.insert(c, 1);
	// std::cout << *c << std::endl;
	// std::cout << *++c << std::endl;
	// std::cout << *d << std::endl;
	// p = std::allocator<Popa<int> * >().allocate(1);
	// std::allocator<Popa<int> * >().construct(p);
	// std::allocator<Popa<int> >().construct(p, ref);
	// std::allocator<Popa<int> >().construct(p, ref);
	// p->puk();
	// std::allocator<Popa<int> * >().deallocate(p, -1);
	// std::cout << "asdasd" << std::endl;
	// a.resize(1);
	// std::vector<int>::iterator b = a.end();
	// std::cout << &(*a.begin()) << std::endl;
	// std::cout << &(*b) << std::endl;
	// a.push_back(2);
	// std::cout << &(*b) << std::endl;
	// std::cout << *b << std::endl;
	// std::cout << a.size() << std::endl;
	// a.assign(b.begin(), b.end());
	// std::cout << a.size() << std::endl;
	// a[0] = 5;
	// a[4] = 5;
	// a.resize(1, 100);
	// std::cout << a.at(4) << std::endl;
	
	// b = a;
	// std::cout << b[4] << std::endl;
	// b.resize(5);
	// std::cout << b[4] << std::endl;
	// std::is_integral
	// std::cout << ft::integral_constant<int, 10>::value << std::endl;
	// std::cout << ft::integral_constant<int, 10>() << std::endl;

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