#include <iostream>
#include <vector>
#include <memory>
#include <thread>

class Popa
{
	public:
		Popa() { std::cout << "cnstr" << std::endl; };
		Popa(const Popa & src) { std::cout << "copy cnstr" << std::endl; };
};

int main(void)
{
	int*	a;

	for (int i = 0; i < 10; i++)
		std::cout << a[i] << std::endl;
	std::cout << std::endl;
	a = std::allocator<int>().allocate(10);
	for (int i = 0; i < 10; i++)
		std::cout << a[i] << std::endl;
	std::allocator<int>().deallocate(a + 2, 1);
}