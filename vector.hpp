#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>
# include <stdexcept>
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
		public:

			typedef	typename	T													value_type;
			typedef				Alloc												allocator_type;
			typedef typename	allocator_type::reference							reference;
			typedef typename	allocator_type::const_reference						const_reference;
			typedef typename	allocator_type::pointer								pointer;
			typedef typename	allocator_type::const_pointer						const_pointer;
			typedef typename	allocator_type::size_type							size_type;

		private:

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

					bool	operator==(_iterator const & rhd)
					{
						return (this->_current == rhd._current);
					};

					bool	operator!=(_iterator const & rhd)
					{
						return (this->_current != rhd._current);
					};

					bool	operator>(_iterator const & rhd)
					{
						return (this->_current > rhd._current);
					};

					bool	operator>=(_iterator const & rhd)
					{
						return (this->_current >= rhd._current);
					};

					bool	operator<(_iterator const & rhd)
					{
						return (this->_current < rhd._current);
					};

					bool	operator<=(_iterator const & rhd)
					{
						return (this->_current <= rhd._current);
					};

					const pointer &	base(void)	const
					{
						return (this->_current);
					};
			};

		public:

			typedef  			_iterator<pointer>									iterator;
			typedef  			_iterator<const_pointer>							const_iterator;
			typedef typename	ft::iterator_traits<iterator>::difference_type		difference_type;
			typedef 			reverse_iterator<iterator>							reverse_iterator;
			typedef				reverse_iterator<const_iterator>					const_reverse_iterator;

		private:

			allocator_type	_allocator;

			pointer		_values;
			size_type	_size;
			size_type	_capacity;

			void	_reallocWithCapacity(size_type new_capacity)
			{
				pointer			new_values = _allocator.allocate(new_capacity);

				for (size_type i = 0; i < this->_size; i++)
				{
					_allocator.construct(new_values + i, this->_values[i]);
					_allocator.destroy(this->_values + i);
				}
				
				for (size_type i = this->_size; i < this->_capacity; i++)
					_allocator.destroy(this->_values + i);

				_allocator.deallocate(this->_values);
				this->_values = new_values;
				this->_capacity = new_capacity;
			};

		public:

			explicit	vector(void) : _values(nullptr) {};

			explicit	vector(size_type n, const value_type & val = value_type(),
				const allocator_type & alloc = allocator_type()) : _size(n), _capacity(n)
			{
				this->_values = alloc.allocate(n);
				for (size_type i = 0; i < n; i++)
					alloc.construct(this->_values + i, val);
			};

			template <class InputIterator>
			vector(InputIterator first, InputIterator last, const allocator_type & alloc = allocator_type())
			{
				this->_values = alloc.allocate(std::distance(first, last));
				std::copy(first, last, this->begin());
			};

			vector(const Vector & src)
			{
				*this = src;
			};

			~vector()
			{
				for (size_type i = 0; i < this->capacity(); i++)
					_allocator.destroy(this->_values[i]);

				_allocator.deallocate(this->_values, this->_capacity);
			};

			vector &	operator=(vector const & rhd)
			{
				if (this->_capacity != rhd._capacity)
				{

					for (size_type i = 0; i < this->_capacity; i++)
						_allocator.destroy(this->_values[i]);
					
					_allocator.deallocate(this->_values, this->_capacity);
					this->_values = _allocator.allocate(rhd._capacity);
					this->_capacity = rhd._capacity;
				}

				this->_size = rhd._size;

				for (size_type i = 0; i < this->_size; i++)
					_allocator.construct(this->_values + i, rhd._values[i]);
				
				for (size_type i = this->_size; i < this->_capacity; i++)
					_allocator.construct(this->_values + i, value_type());
			};

			reference	operator[](size_type n)
			{
				return (this->_values[n]);
			};

			const_reference	operator[](size_type n)	const
			{
				return (this->_values[n]);
			};

			iterator	begin(void)
			{
				return (iterator(this->_values));
			};

			const_iterator	begin(void)	const
			{
				return (const_iterator(this->_values));
			};

			iterator	end(void)
			{
				if (this->empty())
					return (this->begin());

				return (iterator(this->_values + this->_size));
			};

			const_iterator	end(void)	const
			{
				if (this->empty())
					return (this->begin());

				return (const_iterator(this->_values + this->_size));
			};

			reverse_iterator	rbegin(void)
			{
				return (reverse_iterator(this->end()));
			};

			const_reverse_iterator	rbegin(void)	const
			{
				return (const_reverse_iterator(this->end()));
			};

			reverse_iterator	rend(void)
			{
				return (reverse_iterator(this->begin()));
			};

			const_reverse_iterator	rend(void)	const
			{
				return (const_reverse_iterator(this->begin()));
			};

			const_iterator	cbegin(void)	const
			{
				return (const_iterator(this->_values));
			};

			const_iterator	cend(void)	const
			{
				if (this->empty())
					return (this->cbegin());

				return (const_iterator(this->_values + this->_size));
			};

			const_reverse_iterator	crbegin(void)	const
			{
				return (const_reverse_iterator(this->end()));
			};

			const_reverse_iterator	crend(void)	const
			{
				return (const_reverse_iterator(this->begin()));
			};

			size_type	size(void)	const
			{
				return (this->_size);
			};

			size_type	max_size(void)	const
			{
				return (allocator_type().max_size());
			};

			void	resize(size_type n, value_type val = value_type())
			{
				if (n <= this->_size)
				{
					this->_size = n;
					for (size_type i = this->_size; i < this->_capacity; i++)
						_allocator.destroy(this->_values + i);
					return ;
				}

				this->reserve(n);

				while (this->_size < n)
					_allocator.construct(this->_values + this->_size++, val);
			};

			size_type	capacity(void)	const
			{
				return (this->_capacity);
			};

			bool	empty(void)	const
			{
				return (!this->_size);
			};

			void	reserve(size_type n)
			{
				if (n <= this->_capacity)
					return ;
				this->_reallocWithCapacity(n);
			};

			void	shrink_to_fit(void)
			{
				if (this->_size == this->_capacity)
					return ;
				this->_reallocWithCapacity(this->_size);
			};

			reference	at(size_type n)
			{
				if (!(n < this->_size))
					throw std::out_of_range("vector");
				
				return (this->_values[n]);
			};

			const_reference	at(size_type n)	const
			{
				if (!(n < this->_size))
					throw std::out_of_range("vector");
				
				return (this->_values[n]);
			};

			reference	front(void)
			{
				return (*this->_values);
			};

			const_reference	front(void)	const
			{
				return (*this->_values);
			};

			reference	back(void)
			{
				return (this->_values[this->_size - 1]);
			};

			const_reference	back(void)	const
			{
				return (this->_values[this->_size - 1]);
			};

			value_type *	data(void)
			{
				return (this->_values);
			};

			const value_type *	data(void)	const
			{
				return (this->_values);
			};

			template <typename InputIterator>
			void	assign(InputIterator first, InputIterator last)
			{
				this->reserve(std::distance(first, last));
				this->_size = std::distance(first, last);
				for (size_type i = 0; first != last; i++)
				{
					_allocator.destroy(this->_values + i);
					_allocator.construct(this->_values + i, *first);
					first++;
				}
			};

			void	assign(size_type n, const value_type & val)
			{
				this->reserve(n);
				for (this->_size = 0; this->_size < n; this->_size++)
				{
					_allocator.destroy(this->_values + this->_size);
					_allocator.construct(this->_values + this->_size, val);
				}
			};

			void	push_back(const_reference val)
			{
				this->reserve(this->_size + 1);
				this->_size++;
				allocator_type().construct(this->_values + this->_size - 1, val);
			};

			void	pop_back(void)
			{
				if (!this->_size)
					return ;
				_allocator.destroy(this->_values + --this->_size);
			};

			iterator	insert(iterator position, const_reference val)
			{
				size_type		indx = position.base() - this->_values;
	
				this->reserve(this->_size + 1);

				++this->_size;
				
				for (size_type i = indx + 1; i < this->_size; i++)
				{
					_allocator.destroy(this->_values + i);
					_allocator.construct(this->_values + i, this->_values[i - 1]);
				}

				_allocator.destroy(this->_values + indx);
				_allocator.construct(this->_values + indx, val);

				return (iterator(this->_values + indx));
			};

			void	insert(iterator position, size_type n, const_reference val)
			{
				size_type		indx = position.base() - this->_values;

				this->reserve(this->_size + n);

				this->_size += n;

				for (size_type i = indx + n; i < this->_size; i++)
				{
					_allocator.destroy(this->_values + i);
					_allocator.construct(this->_values + i, this->_values[i - 1]);
				}
				
				for (; n >= 0; --n)
				{
					_allocator.destroy(this->_values + indx + n);
					_allocator.construct(this->_values + indx + n, val);
				}

				return (iterator(this->_values + indx));
			};

			template <typename InputIterator>
			void	insert(iterator position, InputIterator first, InputIterator last)
			{
				size_type		indx = position.base() - this->_values;
				size_type		n = std::distance(first, last);

				this->reserve(this->_size + n);

				this->_size += n;

				for (size_type i = indx + n; i < this->_size; i++)
				{
					_allocator.destroy(this->_values + i);
					_allocator.construct(this->_values + i, this->_values[i - 1]);
				}
				
				for (size_type i = indx; first != last; i++)
				{
					_allocator.destroy(this->_values + i);
					_allocator.construct(this->_values + i, *first);
					first++;
				}

				return (iterator(this->_values + indx));
			};
	};
};

#endif
