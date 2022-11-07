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

			pointer		m_values;
			size_type	m_size;
			size_type	m_capacity;

			allocator_type	m_allocator;

			void	_M_realloc_with_capacity(size_type new_capacity)
			{
				pointer		new_values = m_allocator.allocate(new_capacity);
				size_type	i = 0;

				for (; i < this->m_size; i++)
				{
					m_allocator.construct(new_values + i, this->m_values[i]);
					m_allocator.destroy(this->m_values + i);
				}
				
				if (this->m_capacity)
					m_allocator.deallocate(this->m_values, this->m_capacity);

				this->m_values = new_values;
				this->m_capacity = new_capacity;
			};

			iterator	_M_insertion_routine(iterator position, size_type val_num)
			{
				size_type	indx = position.base() - this->m_values;

				if (this->m_size + val_num > this->m_capacity * 2)
					this->reserve(this->m_size + val_num);
				else if (this->m_capacity < this->m_size + val_num)
					this->reserve(this->m_capacity * 2);

				position = this->begin() + indx;

				iterator	last = this->end() + val_num;

				while (--last >= this->end() && last - val_num >= this->begin())
					this->m_allocator.construct(last.base(), *(last - val_num));

				while (last >= this->end())
					this->m_allocator.construct((last--).base(), value_type());

				++last;

				while (--last > position + val_num - 1)
					*last = *(last - val_num);

				this->m_size += val_num;

				return (position);
			}

			void	_M_assign_by_n(size_type n, const_reference val)
			{
				this->reserve(n);

				size_type	i = 0;
				size_type	ass = this->m_size;

				if (n < this->m_size)
					ass = n;

				for (; i < ass; i++)
					this->m_values[i] = val;
				
				for (; i < n; i++)
					this->m_allocator.construct(this->m_values + i, val);

				for (; i < this->m_size; i++)
					this->m_allocator.destroy(this->m_values + i);

				this->m_size = n;
			}

			template <typename Integer>
			void	_M_assing_distributor(Integer n, Integer val, ft::true_type)
			{
				this->_M_assign_by_n(n, val);
			}

			template <typename InpIter>
			void	_M_assing_distributor(InpIter first, InpIter last, ft::false_type)
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
				size_type	ass = this->m_size;

				if (n < this->m_size)
					ass = n;

				for (; i < ass; i++)
				{
					this->m_values[i] = *first;
					++first;
				}
				
				for (; i < n; i++)
				{
					this->m_allocator.construct(this->m_values + i, *first);
					++first;
				}

				for (; i < this->m_size; i++)
					this->m_allocator.destroy(this->m_values + i);

				this->m_size = n;
			};

			void	_M_insert_by_n(iterator position, size_type n, const_reference val)
			{
				if (!n)
					return ;

				position = this->_M_insertion_routine(position, n);

				while (n--)
				{
					*position = val;
					++position;
				}
			}

			template <typename Integer>
			void	_M_insertion_distributor(iterator position, Integer n, Integer val, ft::true_type)
			{
				this->_M_insert_by_n(position, n, val);
			};

			template <typename InpIter>
			void	_M_insertion_distributor(iterator position, InpIter first, InpIter last, ft::false_type)
			{
				typedef typename	ft::is_same<std::input_iterator_tag,typename ft::iterator_traits<InpIter>::iterator_category>	_is_same;

				if (_is_same::value)
				{
					vector	buf(first, last);

					this->_M_insertion_distributor(position, buf.begin(), buf.end(), ft::false_type());
					return ;
				}

				position = this->_M_insertion_routine(position, std::distance(first, last));

				while (first != last)
				{
					*position = *first;
					++first;
					++position;
				}
			};

		public:

			explicit	vector(const allocator_type & alloc = allocator_type())
				: m_values(nullptr), m_size(0), m_capacity(0), m_allocator(alloc) {};

			explicit	vector(size_type n, const value_type & val = value_type(),
				const allocator_type & alloc = allocator_type())
					: m_values(nullptr), m_size(0), m_capacity(0), m_allocator(alloc)
			{
				this->assign(n, val);
			};

			template <class InputIterator>
			vector(InputIterator first, InputIterator last, const allocator_type & alloc = allocator_type())
				: m_values(nullptr), m_size(0), m_capacity(0), m_allocator(alloc)
			{
				this->assign(first, last);
			};

			vector(const vector & src)
				: m_values(nullptr), m_size(0), m_capacity(0), m_allocator(src.m_allocator)
			{
				*this = src;
			};

			~vector()
			{
				this->clear();

				if (this->m_capacity)
					this->m_allocator.deallocate(this->m_values, this->m_capacity);
			};

			vector &	operator=(vector const & rhd)
			{
				this->assign(rhd.begin(), rhd.end());
				return (*this);
			};

			reference	operator[](size_type n)
			{
				return (this->m_values[n]);
			};

			const_reference	operator[](size_type n)	const
			{
				return (this->m_values[n]);
			};

			iterator	begin(void)
			{
				return (iterator(this->m_values));
			};

			const_iterator	begin(void)	const
			{
				return (const_iterator(this->m_values));
			};

			iterator	end(void)
			{
				if (this->empty())
					return (this->begin());

				return (iterator(this->m_values + this->m_size));
			};

			const_iterator	end(void)	const
			{
				if (this->empty())
					return (this->begin());

				return (const_iterator(this->m_values + this->m_size));
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
				return (const_iterator(this->m_values));
			};

			const_iterator	cend(void)	const
			{
				if (this->empty())
					return (this->cbegin());

				return (const_iterator(this->m_values + this->m_size));
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
				return (this->m_size);
			};

			size_type	max_size(void)	const
			{
				return (std::min(static_cast<size_type>(std::numeric_limits<difference_type>::max()), this->m_allocator.max_size()));
			};

			void	resize(size_type n, value_type val = value_type())
			{
				if (n == this->m_size)
					return ;

				if (n < this->m_size)
				{
					while (this->m_size > n)
						m_allocator.destroy(this->m_values + --this->m_size);
					return ;
				}

				this->reserve(n);

				while (this->m_size < n)
					m_allocator.construct(this->m_values + this->m_size++, val);
			};

			size_type	capacity(void)	const
			{
				return (this->m_capacity);
			};

			bool	empty(void)	const
			{
				return (!this->m_size);
			};

			void	reserve(size_type n)
			{
				if (n <= this->m_capacity)
					return ;
				this->_M_realloc_with_capacity(n);
			};

			void	shrink_to_fit(void)
			{
				if (this->m_size == this->m_capacity)
					return ;
				this->_M_realloc_with_capacity(this->m_size);
			};

			reference	at(size_type n)
			{
				if (!(n < this->m_size))
					throw std::out_of_range("vector");
				
				return (this->m_values[n]);
			};

			const_reference	at(size_type n)	const
			{
				if (!(n < this->m_size))
					throw std::out_of_range("vector");
				
				return (this->m_values[n]);
			};

			reference	front(void)
			{
				return (*this->m_values);
			};

			const_reference	front(void)	const
			{
				return (*this->m_values);
			};

			reference	back(void)
			{
				return (this->m_values[this->m_size - 1]);
			};

			const_reference	back(void)	const
			{
				return (this->m_values[this->m_size - 1]);
			};

			value_type *	data(void)
			{
				return (this->m_values);
			};

			const value_type *	data(void)	const
			{
				return (this->m_values);
			};

			template <typename InputIterator>
			void	assign(InputIterator first, InputIterator last)
			{
				typedef typename	ft::is_integral<InputIterator>::type	_Integral;

				this->_M_assing_distributor(first, last, _Integral());
			};

			void	assign(size_type n, const_reference val)
			{
				this->_M_assign_by_n(n, val);
			};

			void	push_back(const_reference val)
			{
				if (this->m_size == this->m_capacity)
					this->reserve(this->m_size * 2 + !this->m_size);
				this->m_allocator.construct(this->m_values + this->m_size++, val);
			};

			void	pop_back(void)
			{
				if (!this->m_size)
					return ;
				this->m_allocator.destroy(this->m_values + --this->m_size);
			};

			iterator	insert(iterator position, const_reference val)
			{
				position = this->_M_insertion_routine(position, 1);

				*position = val;

				return (position);
			};

			void	insert(iterator position, size_type n, const_reference val)
			{
				if (!this->m_size)
				{
					this->assign(n, val);
					return ;
				}

				this->_M_insert_by_n(position, n, val);
			};

			template <typename InputIterator>
			void	insert(iterator position, InputIterator first, InputIterator last)
			{
				if (!this->m_size)
				{
					this->assign(first, last);
					return ;
				}

				typedef typename	ft::is_integral<InputIterator>::type	_Integral;

				this->_M_insertion_distributor(position, first, last, _Integral());
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
					this->m_allocator.destroy(first.base());

				this->m_size -= n;

				return (edge);
			};

			void	swap(vector & src)
			{
				value_type *	buf = src.m_values;
				size_type		size_buf = src.m_size;
				size_type		capacity_buf = src.m_capacity;

				src.m_values = this->m_values;
				src.m_size = this->m_size;
				src.m_capacity = this->m_capacity;
				this->m_values = buf;
				this->m_size = size_buf;
				this->m_capacity = capacity_buf;
			};

			void	clear(void)
			{
				for (iterator start = this->begin(); start != this->end(); start++)
					m_allocator.destroy(start.base());
				this->m_size = 0;
			};

			allocator_type	get_allocator(void)	const
			{
				return (m_allocator);
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
