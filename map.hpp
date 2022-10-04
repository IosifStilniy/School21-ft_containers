#ifndef MAP_HPP
# define MAP_HPP

# include <functional>
# include <memory>
# include "reverse_iterator.hpp"
# include "pair.hpp"
# include "iterator_traits.hpp"
# include "RedBlackTree.hpp"

namespace ft
{
	template <typename Key, typename T, class Compare = std::less<Key>, class Alloc = std::allocator<ft::pair<const Key, T> > >
	class map
	{
		public:
			typedef				Key									key_type;
			typedef				T									mapped_type;
			typedef typename	ft::pair<const Key, T>				value_type;
			typedef				Compare								key_compare;
			typedef				Alloc								allocator_type;
			typedef	typename	allocator_type::reference			reference;
			typedef	typename	allocator_type::const_reference		const_reference;
			typedef	typename	allocator_type::pointer				pointer;
			typedef	typename	allocator_type::const_pointer		const_pointer;
			// typedef 											iterator;
			// typedef 											const_iterator;
			// typedef typename			ft::reverse_iterator<iterator>		reverse_iterator;
			// typedef typename			ft::reverse_iterator<const_iterator>		const_reverse_iterator;
			// typedef typename			ft::iterator_traits<iterator>::difference_type				difference_type;
			// typedef typename			ft::iterator_traits<iterator>::difference_type				difference_type;
			typedef typename	allocator_type::size_type			size_type;

			class value_compare : public std::binary_function<value_type, value_type, bool>
			{
				friend class map;

				private:
					key_compare	_comparator;

					value_compare(key_compare comparator) : _comparator(comparator) {};

					value_compare &	operator=(const value_compare & rhd)
					{
						this->_comparator = rhd._comparator;
						return (*this);
					};

				public:

					bool	operator()(const value_type & lhd, const value_type & rhd)	const
					{
						return (this->_comparator(lhd.first, rhd.first));
					};
			};

		private:
			allocator_type													_allocator;
			value_compare													_comparator;
			RedBlackTree <value_type, value_compare, allocator_type>		_tree;

		public:

			explicit map(const key_compare & comp = key_compare(), const allocator_type & alloc = allocator_type())
			{
				this->_comparator = value_compare(comp);
				this->_allocator = allocator_type(alloc);
			};

			template <typename InputIter>
			map(InputIter first, InputIter last, const key_compare & comp = key_compare(), const allocator_type & alloc = allocator_type())
			{
				this->_comparator = value_compare(comp);
				this->_allocator = allocator_type(alloc);
			};

			map(const map & src)
			{
				*this = src;
			};

			~map() {};

			map &	operator=(const map & rhd)
			{
				this->_allocator = rhd._allocator;
				this->_comparator = rhd._comparator;
				this->_tree = rhd._tree;

				return (*this);
			};

			key_compare	key_comp(void)	const
			{
				return (key_compare(this->_comparator));
			};
	};
};

#endif
