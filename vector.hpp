#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>

#include <vector>

namespace ft
{
	template <typename T, class Alloc = std::allocator<T> >
	class vector
	{
		private:
			T*	values;
		public:
			typedef				T									value_type;
			typedef typename	Alloc								allocator_type;
			typedef typename	allocator_type::reference			reference;
			typedef typename	allocator_type::const_reference		const_reference;
			typedef typename	allocator_type::pointer				pointer;
			typedef typename	allocator_type::const_pointer		const_pointer;
			// typedef typename										iterator;
			// typedef typename										const_iterator;
			// typedef typename										reverse_iterator;
			// typedef typename										const_reverse_iterator;
			// typedef typename										difference_type;
			typedef				size_t								size_type;


			explicit	vector(const allocator_type & alloc);
			explicit	vector(size_type n, const value_type & val, const allocator_type & alloc);

			template <class InputIterator>
			vector(InputIterator first, InputIterator last, const allocator_type & alloc);

			vector(const Vector & src);
			~vector();
	};
	
	#include "vector.tpp"
	
}
#endif
