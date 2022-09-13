#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>
# include "iterator.hpp"
# include "iterator_traits.hpp"
# include "reverse_iterator.hpp"

#include <vector>
std::vector<int> a;

namespace ft
{
	template <typename T, class Alloc = std::allocator<T> >
	class vector
	{
		private:

			T *	_values;

			template <typename pointer>
			class _iterator : public ft::iterator<ft::random_access_iterator_tag, pointer>
			{
				private:
					pointer	_current;

				public:
					_iterator(void) {};

					_iterator(const _iterator & src)
					{
						*this = src;
					};

					_iterator(const pointer & src) : _current(src) {};

					~_iterator();

					_iterator &	operator=(const _iterator & rhd)
					{
						this->_current = rhd._current;
						return (*this);
					};

					reference	operator*(void)	const
					{
						return (*_current);
					};

					pointer	operator->(void)	const
					{
						return (_current);
					};

					reference	operator[](difference_type n)	const
					{
						return (this->_current[n]);
					};

					_iterator	operator+(difference_type n)	const
					{
						return (_iterator(this->_current + n));
					};

					_iterator &	operator+=(difference_type n)
					{
						this->_current += n;
						return (*this);
					};

					_iterator &	operator++(void)
					{
						this->_current++;
						return (*this);
					};

					_iterator	operator++(int)
					{
						return (_iterator(this->_current++));
					};

					_iterator	operator-(difference_type n)	const
					{
						return (_iterator(this->_current - n));
					};

					_iterator &	operator-=(difference_type n)
					{
						this->_current -= n;
						return (*this);
					};

					_iterator &	operator--(void)
					{
						this->_current--;
						return (*this);
					};

					_iterator	operator--(int)
					{
						return (_iterator(this->_current--));
					};

					const pointer &	base(void)	const
					{
						return (this->_current);
					};
			};

		public:

			typedef				T													value_type;
			typedef				Alloc												allocator_type;
			typedef typename	allocator_type::reference							reference;
			typedef typename	allocator_type::const_reference						const_reference;
			typedef typename	allocator_type::pointer								pointer;
			typedef typename	allocator_type::const_pointer						const_pointer;
			typedef  			_iterator<pointer>									iterator;
			typedef  			_iterator<const_pointer>							const_iterator;
			typedef typename	ft::iterator_traits<iterator>::difference_type		difference_type;
			typedef 			reverse_iterator<iterator>							reverse_iterator;
			typedef				reverse_iterator<const_iterator>					const_reverse_iterator;
			typedef				size_t												size_type;


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
};

#endif
