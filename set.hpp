#ifndef SET_HPP
# define SET_HPP

# include <functional>
# include <memory>
# include <limits>
# include <algorithm>
# include "algorithm.hpp"
# include "red_black_tree.hpp"
# include "iterator_traits.hpp"
# include "pair.hpp"

namespace ft
{
	template <typename T, typename Compare = std::less<T>, typename Alloc = std::allocator<T> >
	class set
	{
		public:
			typedef				T									key_type;
			typedef				T									value_type;
			typedef				Compare								key_compare;
			typedef				Compare								value_compare;
			typedef				Alloc								allocator_type;
			typedef typename	allocator_type::reference			reference;
			typedef typename	allocator_type::const_reference		const_reference;
			typedef typename	allocator_type::pointer				pointer;
			typedef typename	allocator_type::const_pointer		const_pointer;
			typedef typename	allocator_type::size_type			size_type;

		private:
			typedef				red_black_tree<value_type, value_compare, allocator_type>		tree;

		public:
			typedef typename	tree::iterator										iterator;
			typedef typename	tree::const_iterator								const_iterator;
			typedef typename	tree::reverse_iterator								reverse_iterator;
			typedef typename	tree::const_reverse_iterator						const_reverse_iterator;
			typedef typename	ft::iterator_traits<iterator>::difference_type		difference_type;

		private:
			key_compare		m_comparator;
			tree			m_tree;

		public:
			explicit set(const key_compare & comp = key_compare(), const allocator_type & alloc = allocator_type())
				: m_comparator(key_compare(comp)), m_tree(tree(alloc, comp))
			{};

			template <typename InpIter>
			set (InpIter first, InpIter last, const key_compare & comp = key_compare(), const allocator_type & alloc = allocator_type())
				: m_comparator(key_compare(comp)), m_tree(tree(alloc, comp))
			{
				this->insert(first, last);
			};

			set(const set & src)
				: m_comparator(src.m_comparator), m_tree(src.m_tree)
			{};

			~set() {};

			set &	operator=(const set & rhd)
			{
				this->m_comparator = rhd.m_comparator;
				this->m_tree = rhd.m_tree;
				return (*this);
			};

			iterator	begin(void)
			{
				return (this->m_tree.begin());
			};

			const_iterator	begin(void)	const
			{
				return (this->m_tree.cbegin());
			};

			iterator	end(void)
			{
				return (this->m_tree.end());
			};

			const_iterator	end(void)	const
			{
				return (this->m_tree.cend());
			};

			reverse_iterator	rbegin(void)
			{
				return (this->m_tree.rbegin());
			};

			const_reverse_iterator	rbegin(void)	const
			{
				return (this->m_tree.crbegin());
			};

			reverse_iterator	rend(void)
			{
				return (this->m_tree.rend());
			};

			const_reverse_iterator	rend(void)	const
			{
				return (this->m_tree.crend());
			};

			const_iterator	cbegin(void)	const
			{
				return (this->m_tree.cbegin());
			};

			const_iterator	cend(void)	const
			{
				return (this->m_tree.cend());
			};

			const_reverse_iterator	crbegin(void)	const
			{
				return (this->m_tree.crbegin());
			};

			const_reverse_iterator	crend(void)	const
			{
				return (this->m_tree.crend());
			};

			bool	empty(void)	const
			{
				return (!this->m_tree.size());
			};

			size_type	size(void)	const
			{
				return (this->m_tree.size());
			};

			size_type	max_size(void)	const
			{
				return (std::min(static_cast<size_type>(std::numeric_limits<difference_type>::max()), this->_allocator.max_size()));
			};

			ft::pair<iterator, bool>	insert(const value_type & val)
			{
				return (this->m_tree.insert(val));
			};

			iterator	insert(iterator position, const value_type & val)
			{
				return (this->m_tree.insert(val, position.current).first);
			};

			template <typename InpIter>
			void	insert(InpIter first, InpIter last)
			{
				while (first != last)
				{
					this->insert(*first);
					++first;
				}
			};

			void	erase(iterator position)
			{
				this->m_tree.erase(position.current);
			};

			size_type	erase(const key_type & key)
			{
				iterator	founded = this->find(key);

				if (founded == this->end())
					return (0);
				
				this->m_tree.erase(founded.current);
				return (1);
			};

			void	erase(iterator first, iterator last)
			{
				iterator	buf = first;

				while (first != last)
				{
					++buf;
					this->erase(first);
					first = buf;
				}
			};

			void	swap(set & ref)
			{
				this->m_tree.swap(ref.m_tree);
			};

			void	clear(void)
			{
				this->m_tree.clear();
			};

			key_compare	key_comp(void)	const
			{
				return (this->m_comparator);
			};

			value_compare	value_comp(void)	const
			{
				return (this->m_tree.value_comp());
			};

			iterator	find(const key_type & key)
			{
				return (this->m_tree.find(key));
			};

			const_iterator	find(const key_type & key)	const
			{
				return (this->m_tree.find(key));
			};

			size_type	count(const key_type & key)	const
			{
				return (this->find(key) != this->end());
			};

			iterator	lower_bound(const key_type & key)
			{
				return (this->m_tree.lower_bound(key));
			};

			const_iterator	lower_bound(const key_type & key)	const
			{
				return (this->m_tree.lower_bound(key));
			};

			iterator	upper_bound(const key_type & key)
			{
				return (this->m_tree.upper_bound(key));
			};

			const_iterator	upper_bound(const key_type & key)	const
			{
				return (this->m_tree.upper_bound(key));
			};

			ft::pair<iterator, iterator>	equal_range(const key_type & key)
			{
				return (ft::make_pair(this->lower_bound(key), this->upper_bound(key)));
			};

			ft::pair<const_iterator, const_iterator>	equal_range(const key_type & key)	const
			{
				return (ft::make_pair(this->lower_bound(key), this->upper_bound(key)));
			};

			allocator_type	get_allocator(void)	const
			{
				return (this->m_tree.get_allocator());
			};
	};

	template <typename T, class Compare, class Alloc>
	void	swap(set<T, Compare, Alloc> & lhd, set<T, Compare, Alloc> & rhd)
	{
		lhd.swap(rhd);
	};

	template <typename T, class Compare, class Alloc>
	bool	operator==(const set<T, Compare, Alloc> & lhd, const set<T, Compare, Alloc> & rhd)
	{
		if (lhd.size() != rhd.size())
			return (false);
		return (ft::equal(lhd.begin(), lhd.end(), rhd.begin()));
	};

	template <typename T, class Compare, class Alloc>
	bool	operator!=(const set<T, Compare, Alloc> & lhd, const set<T, Compare, Alloc> & rhd)
	{
		return !(lhd == rhd);
	};

	template <typename T, class Compare, class Alloc>
	bool	operator<(const set<T, Compare, Alloc> & lhd, const set<T, Compare, Alloc> & rhd)
	{
		return (ft::lexicographical_compare(lhd.begin(), lhd.end(), rhd.begin(), rhd.end()));
	};

	template <typename T, class Compare, class Alloc>
	bool	operator>(const set<T, Compare, Alloc> & lhd, const set<T, Compare, Alloc> & rhd)
	{
		return (rhd < lhd);
	};

	template <typename T, class Compare, class Alloc>
	bool	operator<=(const set<T, Compare, Alloc> & lhd, const set<T, Compare, Alloc> & rhd)
	{
		return !(rhd < lhd);
	};

	template <typename T, class Compare, class Alloc>
	bool	operator>=(const set<T, Compare, Alloc> & lhd, const set<T, Compare, Alloc> & rhd)
	{
		return !(lhd < rhd);
	};
};

#endif
