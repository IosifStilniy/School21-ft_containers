#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>
# include "iterator.hpp"
# include "iterator_traits.hpp"
#include <vector>
std::vector<int> a;
namespace ft
{
	template <typename T, class Alloc = std::allocator<T> >
	class vector
	{
		private:
			T *	_values;

			class _iterator : public ft::iterator<ft::random_access_iterator_tag, T>
			{
				private:
					/* data */
				public:
					_iterator(/* args */);
					~_iterator();
			};

		public:
			typedef				T															value_type;
			typedef typename	Alloc														allocator_type;
			typedef typename	allocator_type::reference									reference;
			typedef typename	allocator_type::const_reference								const_reference;
			typedef typename	allocator_type::pointer										pointer;
			typedef typename	allocator_type::const_pointer								const_pointer;
			typedef typename	ft::iterator_traits<>::difference_type						difference_type;
			typedef typename 	_iterator													iterator;
			typedef typename 	ft::iterator<ft::random_access_iterator_tag, const T>		const_iterator;
			// typedef typename										reverse_iterator;
			// typedef typename										const_reverse_iterator;
			typedef				size_t								size_type;


			explicit	vector(void) : _values(nullptr)
			{
			};

			explicit	vector(size_type n, const value_type & val = value_type(),
				const allocator_type & alloc = allocator_type())
			{
				this->_values = alloc.allocate(n);
				for (size_type i = 0; i < n; i++)
					alloc.construct(this->_values[i]);
			};

			template <class InputIterator>
			vector(InputIterator first, InputIterator last, const allocator_type & alloc = allocator_type())
			{
				this->_values = alloc.allocate(std::distance(first, last));
				std::copy(first, last, this->begin());
			}

			vector(const Vector & src);
			~vector();
	};
}
#endif
