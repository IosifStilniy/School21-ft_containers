#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>
# include <stdexcept>
# include "iterator.hpp"
# include "iterator_traits.hpp"
# include "reverse_iterator.hpp"
# include "algorithm.hpp"
# include "vector_iterator.hpp"

namespace ft
{
	template <typename T, class Alloc = std::allocator<T> >
	class vector
	{
		public:

			typedef				T													value_type;
			typedef				Alloc												allocator_type;
			typedef typename	allocator_type::reference							reference;
			typedef typename	allocator_type::const_reference						const_reference;
			typedef typename	allocator_type::pointer								pointer;
			typedef typename	allocator_type::const_pointer						const_pointer;
			typedef typename	allocator_type::size_type							size_type;

			typedef  			ft::_iterator<pointer>								iterator;
			typedef  			ft::_iterator<const_pointer>						const_iterator;
			typedef typename	ft::iterator_traits<iterator>::difference_type		difference_type;
			typedef				reverse_iterator<const_iterator>					const_reverse_iterator;
			typedef 			reverse_iterator<iterator>							reverse_iterator;

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

				_allocator.deallocate(this->_values, this->_capacity);
				this->_values = new_values;
				this->_capacity = new_capacity;
			};

		public:

			explicit	vector(const allocator_type & alloc = allocator_type()) : _values(nullptr), _allocator(alloc) {};

			explicit	vector(size_type n, const value_type & val = value_type(),
				const allocator_type & alloc = allocator_type()) : _size(n), _capacity(n)
			{
				this->_allocator = allocator_type(alloc);

				this->_values = this->_allocator.allocate(n);
				for (size_type i = 0; i < n; i++)
					this->_allocator.construct(this->_values + i, val);
			};

			template <class InputIterator>
			vector(InputIterator first, InputIterator last, const allocator_type & alloc = allocator_type())
			{
				this->_allocator = allocator_type(alloc);

				this->_values = this->_allocator.allocate(std::distance(first, last));
				std::copy(first, last, this->begin());
			};

			vector(const vector & src)
			{
				*this = src;
			};

			~vector()
			{
				for (size_type i = 0; i < this->size(); i++)
					_allocator.destroy(this->_values + i);

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
				return (_allocator.max_size());
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
				if (this->_size == this->_capacity)
					this->reserve(this->_size * 2);
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
				size_type	indx = position.base() - this->_values;

				if (this->_size == this->_capacity)
					this->reserve(this->_size * 2);

				position = this->begin() + indx;

				iterator	last = this->end();
				
				_allocator.construct(last.base(), *(last - 1));
				while (--last > position)
					*last = *(last - 1);

				++this->_size;

				*position = val;

				return (position);
			};

			void	insert(iterator position, size_type n, const_reference val)
			{
				size_type		indx = position.base() - this->_values;

				if (this->_size + n > this->_capacity * 2)
					this->reserve(this->_size + n);
				else if (this->_capacity < this->_size + n)
					this->reserve(this->_capacity * 2);

				this->_size += n;

				for (size_type i = indx + n; i < this->_size; i++)
				{
					_allocator.destroy(this->_values + i);
					_allocator.construct(this->_values + i, this->_values[i - 1]);
				}

				for (; n > 0; --n)
				{
					_allocator.destroy(this->_values + indx + n);
					_allocator.construct(this->_values + indx + n, val);
				}
				_allocator.destroy(this->_values + indx + n);
				_allocator.construct(this->_values + indx + n, val);
			};

			template <typename InputIterator>
			void	insert(iterator position, InputIterator first, InputIterator last)
			{
				size_type	indx = position.base() - this->_values;
				size_type	n = std::distance(first, last);

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
			};

			iterator	erase(iterator position)
			{
				this->erase(position, position + 1);
			};

			iterator	erase(iterator first, iterator last)
			{
				size_type	src_indx = last.base() - this->_values;
				size_type	dst_indx = first.base() - this->_values;

				for (size_type i = src_indx; i < this->_size; i++)
				{
					_allocator.destroy(this->_values + dst_indx + i);
					_allocator.construct(this->_values + dst_indx + i, this->_values[src_indx + i]);
				}

				this->_size -= src_indx - dst_indx;
				for (size_type i = 0; i < src_indx - dst_indx; i++)
					_allocator.destroy(this->_values + this->_size + i);
				
				return (last);
			};

			void	swap(vector & src)
			{
				value_type *	buf = src->_values;
				size_type		size_buf = src->_size;
				size_type		capacity_buf = src->_capacity;

				src->_values = this->_values;
				src->_size = this->_size;
				src->_capacity = this->_capacity;
				this->_values = buf;
				this->_size = size_buf;
				this->_capacity = capacity_buf;
			};

			void	clear(void)
			{
				for (size_type i = 0; i < this->_size; i++)
					_allocator.destroy(this->_values + i);
				this->_size = 0;
			};

			allocator_type	get_allocator(void)	const
			{
				return (_allocator);
			};
	};

	template <typename T, typename Alloc>
	void	swap(vector<T, Alloc> & lhd, vector<T, Alloc> & rhd)
	{
		lhd.swap(rhd);
	};

	template <typename T, typename Alloc>
	bool	operator==(vector<T, Alloc> & lhd, vector<T, Alloc> & rhd)
	{
		if (lhd.size() != rhd.size())
			return (false);
		return (ft::equal(lhd.begin(), lhd.end(), rhd.begin()));
	};

	template <typename T, typename Alloc>
	bool	operator!=(vector<T, Alloc> & lhd, vector<T, Alloc> & rhd)
	{
		return !(lhd == rhd);
	};

	template <typename T, typename Alloc>
	bool	operator<(vector<T, Alloc> & lhd, vector<T, Alloc> & rhd)
	{
		return (ft::lexicographical_compare(lhd.begin(), lhd.end(), rhd.begin(), rhd.end()));
	};

	template <typename T, typename Alloc>
	bool	operator>(vector<T, Alloc> & lhd, vector<T, Alloc> & rhd)
	{
		return (rhd < lhd);
	};

	template <typename T, typename Alloc>
	bool	operator<=(vector<T, Alloc> & lhd, vector<T, Alloc> & rhd)
	{
		return !(rhd > lhd);
	};

	template <typename T, typename Alloc>
	bool	operator>=(vector<T, Alloc> & lhd, vector<T, Alloc> & rhd)
	{
		return !(lhd > rhd);
	};
};

#endif
