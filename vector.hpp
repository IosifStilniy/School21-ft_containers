#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <algorithm>
# include <memory>
# include <stdexcept>
# include <type_traits>
# include <limits>
# include "iterator_traits.hpp"
# include "type_traits.hpp"
# include "reverse_iterator.hpp"
# include "algorithm.hpp"
# include "vector_iterator.hpp"

# include <iostream>

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

			typedef  			ft::vector_iterator<pointer, pointer>				iterator;
			typedef  			ft::vector_iterator<const_pointer, pointer>			const_iterator;
			typedef typename	ft::iterator_traits<iterator>::difference_type		difference_type;
			typedef				reverse_iterator<const_iterator>					const_reverse_iterator;
			typedef 			reverse_iterator<iterator>							reverse_iterator;

		private:

			typedef typename	ft::enable_if<
												ft::is_same<value_type, typename allocator_type::value_type>::value,
												value_type
											>::type							_value_type;

			pointer		_values;
			size_type	_size;
			size_type	_capacity;

			allocator_type	_allocator;

			void	_reallocWithCapacity(size_type new_capacity)
			{
				pointer		new_values = _allocator.allocate(new_capacity);
				size_type	i = 0;

				for (; i < this->_size; i++)
				{
					_allocator.construct(new_values + i, this->_values[i]);
					_allocator.destroy(this->_values + i);
				}
				
				if (this->_capacity)
					_allocator.deallocate(this->_values, this->_capacity);

				this->_values = new_values;
				this->_capacity = new_capacity;
			};

			iterator	_insertion_routine(iterator position, size_type val_num)
			{
				size_type	indx = position.base() - this->_values;

				if (this->_size + val_num > this->_capacity * 2)
					this->reserve(this->_size + val_num);
				else if (this->_capacity < this->_size + val_num)
					this->reserve(this->_capacity * 2);

				position = this->begin() + indx;

				iterator	last = this->end() + val_num;

				while (--last > this->end())
					this->_allocator.construct(last.base(), *(last - val_num));
				this->_allocator.construct(last.base(), *(last - val_num));

				while (--last > position + val_num - 1)
					*last = *(last - val_num);

				this->_size += val_num;

				return (position);
			}

			void	_assing_by_n(size_type n, const_reference val)
			{
				this->reserve(n);

				size_type	i = 0;
				size_type	ass = this->_size;

				if (n < this->_size)
					ass = n;

				for (; i < ass; i++)
					this->_values[i] = val;
				
				for (; i < n; i++)
					this->_allocator.construct(this->_values + i, val);

				for (; i < this->_size; i++)
					this->_allocator.destroy(this->_values + i);

				this->_size = n;
			}

			template <typename Integer>
			void	_assing_distributor(Integer n, Integer val, ft::true_type)
			{
				this->_assing_by_n(n, val);
			}

			template <typename InpIter>
			void	_assing_distributor(InpIter first, InpIter last, ft::false_type)
			{
				typedef typename	ft::is_same<std::input_iterator_tag,typename ft::iterator_traits<InpIter>::iterator_category>	_is_same;

				if (_is_same::value)
				{
					this->clear();

					for (; first != last; first++)
						this->push_back(*first);

					return ;
				}

				size_type	n = std::distance(first, last);

				this->reserve(n);

				size_type	i = 0;
				size_type	ass = this->_size;

				if (n < this->_size)
					ass = n;

				for (; i < ass; i++)
				{
					this->_values[i] = *first;
					++first;
				}
				
				for (; i < n; i++)
				{
					this->_allocator.construct(this->_values + i, *first);
					++first;
				}

				for (; i < this->_size; i++)
					this->_allocator.destroy(this->_values + i);

				this->_size = n;
			};

			void	_insert_by_n(iterator position, size_type n, const_reference val)
			{
				if (!n)
					return ;

				position = this->_insertion_routine(position, n);

				while (n--)
				{
					*position = val;
					++position;
				}
			}

			template <typename Integer>
			void	_insertion_distributor(iterator position, Integer n, Integer val, ft::true_type)
			{
				this->_insert_by_n(position, n, val);
			};

			template <typename InpIter>
			void	_insertion_distributor(iterator position, InpIter first, InpIter last, ft::false_type)
			{
				typedef typename	ft::is_same<std::input_iterator_tag,typename ft::iterator_traits<InpIter>::iterator_category>	_is_same;

				if (_is_same::value)
				{
					vector	buf(first, last);

					this->_insertion_distributor(position, buf.begin(), buf.end(), ft::false_type());
					return ;
				}

				position = this->_insertion_routine(position, std::distance(first, last));

				while (first != last)
				{
					*position = *first;
					++first;
					++position;
				}
			};

		public:

			explicit	vector(const allocator_type & alloc = allocator_type())
				: _values(nullptr), _size(0), _capacity(0), _allocator(alloc) {};

			explicit	vector(size_type n, const value_type & val = value_type(),
				const allocator_type & alloc = allocator_type())
					: _values(nullptr), _size(0), _capacity(0), _allocator(alloc)
			{
				this->assign(n, val);
			};

			template <class InputIterator>
			vector(InputIterator first, InputIterator last, const allocator_type & alloc = allocator_type())
				: _values(nullptr), _size(0), _capacity(0), _allocator(alloc)
			{
				this->assign(first, last);
			};

			vector(const vector & src)
				: _values(nullptr), _size(0), _capacity(0), _allocator(src._allocator)
			{
				*this = src;
			};

			~vector()
			{
				this->clear();

				if (this->_capacity)
					this->_allocator.deallocate(this->_values, this->_capacity);
			};

			vector &	operator=(vector const & rhd)
			{
				this->assign(rhd.begin(), rhd.end());
				return (*this);
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
				return (std::min(static_cast<size_type>(std::numeric_limits<difference_type>::max()), this->_allocator.max_size()));
			};

			void	resize(size_type n, value_type val = value_type())
			{
				if (n == this->_size)
					return ;

				if (n < this->_size)
				{
					while (this->_size > n)
						_allocator.destroy(this->_values + --this->_size);
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
				typedef typename	ft::is_integral<InputIterator>::type	_Integral;

				this->_assing_distributor(first, last, _Integral());
			};

			void	assign(size_type n, const_reference val)
			{
				this->_assing_by_n(n, val);
			};

			void	push_back(const_reference val)
			{
				if (this->_size == this->_capacity)
					this->reserve(this->_size * 2 + !this->_size);
				this->_allocator.construct(this->_values + this->_size++, val);
			};

			void	pop_back(void)
			{
				if (!this->_size)
					return ;
				this->_allocator.destroy(this->_values + --this->_size);
			};

			iterator	insert(iterator position, const_reference val)
			{
				position = this->_insertion_routine(position, 1);

				*position = val;

				return (position);
			};

			void	insert(iterator position, size_type n, const_reference val)
			{
				if (!this->_size)
				{
					this->assign(n, val);
					return ;
				}

				this->_insert_by_n(position, n, val);
			};

			template <typename InputIterator>
			void	insert(iterator position, InputIterator first, InputIterator last)
			{
				if (!this->_size)
				{
					this->assign(first, last);
					return ;
				}

				typedef typename	ft::is_integral<InputIterator>::type	_Integral;

				this->_insertion_distributor(position, first, last, _Integral());
			};

			iterator	erase(iterator position)
			{
				return(this->erase(position, position + 1));
			};

			iterator	erase(iterator first, iterator last)
			{
				iterator	edge = first;
				size_type	n = last - first;

				while (last != this->end())
					*(first++) = *(last++);
				
				for (; first != this->end(); first++)
					this->_allocator.destroy(first.base());

				this->_size -= n;

				return (edge);
			};

			void	swap(vector & src)
			{
				value_type *	buf = src._values;
				size_type		size_buf = src._size;
				size_type		capacity_buf = src._capacity;

				src._values = this->_values;
				src._size = this->_size;
				src._capacity = this->_capacity;
				this->_values = buf;
				this->_size = size_buf;
				this->_capacity = capacity_buf;
			};

			void	clear(void)
			{
				for (iterator start = this->begin(); start != this->end(); start++)
					_allocator.destroy(start.base());
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
	bool	operator==(const vector<T, Alloc> & lhd, const vector<T, Alloc> & rhd)
	{
		if (lhd.size() != rhd.size())
			return (false);
		return (ft::equal(lhd.begin(), lhd.end(), rhd.begin()));
	};

	template <typename T, typename Alloc>
	bool	operator!=(const vector<T, Alloc> & lhd, const vector<T, Alloc> & rhd)
	{
		return !(lhd == rhd);
	};

	template <typename T, typename Alloc>
	bool	operator<(const vector<T, Alloc> & lhd, const vector<T, Alloc> & rhd)
	{
		return (ft::lexicographical_compare(lhd.begin(), lhd.end(), rhd.begin(), rhd.end()));
	};

	template <typename T, typename Alloc>
	bool	operator>(const vector<T, Alloc> & lhd, const vector<T, Alloc> & rhd)
	{
		return (rhd < lhd);
	};

	template <typename T, typename Alloc>
	bool	operator<=(const vector<T, Alloc> & lhd, const vector<T, Alloc> & rhd)
	{
		return !(rhd < lhd);
	};

	template <typename T, typename Alloc>
	bool	operator>=(const vector<T, Alloc> & lhd, const vector<T, Alloc> & rhd)
	{
		return !(lhd < rhd);
	};
};

#endif
