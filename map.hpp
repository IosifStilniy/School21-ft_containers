#ifndef MAP_HPP
# define MAP_HPP

# include <functional>
# include <memory>
# include <stdexcept>
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
			typedef				Key															key_type;
			typedef				T															mapped_type;
			typedef typename	ft::pair<const Key, T>										value_type;
			typedef				Compare														key_compare;
			typedef				Alloc														allocator_type;
			typedef	typename	allocator_type::reference									reference;
			typedef	typename	allocator_type::const_reference								const_reference;
			typedef	typename	allocator_type::pointer										pointer;
			typedef	typename	allocator_type::const_pointer								const_pointer;
			typedef typename	allocator_type::size_type									size_type;

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
			typedef				RedBlackTree <value_type, value_compare, allocator_type>	Tree;
			typedef typename	Tree::Node													Node;

		public:
			typedef typename	Tree::iterator												iterator;
			typedef typename	Tree::const_iterator										const_iterator;
			typedef typename	ft::reverse_iterator<iterator>								reverse_iterator;
			typedef typename	ft::reverse_iterator<const_iterator>						const_reverse_iterator;
			typedef typename	ft::iterator_traits<iterator>::difference_type				difference_type;

		private:
			key_compare		_comparator;
			allocator_type	_allocator;
			Tree			_tree;

			Node *	_find(const key_type & key)
			{
				Node *	founded = this->_tree.find(value_type(key, mapped_type()));

				if (this->_comparator(key, founded->value.first))
					founded = founded->left;
				else
					founded = founded->right;

				if (founded
					&& !this->_comparator(key, founded->value.first)
					&& !this->_comparator(founded->value.first, key))
					return (founded);
				return (nullptr);
			};

		public:

			explicit map(const key_compare & comp = key_compare(), const allocator_type & alloc = allocator_type())
				: _comparator(key_compare(comp)), _allocator(allocator_type(alloc)), _tree(Tree(alloc, this->_comparator))
			{};

			template <typename InputIter>
			map(InputIter first, InputIter last, const key_compare & comp = key_compare(), const allocator_type & alloc = allocator_type())
				:	map(comp, alloc)
			{
				for (; first != last; first++)
					this->_tree.insert(*first);
			};

			map(const map & src)
			{
				*this = src;
			};

			~map() {};

			map &	operator=(const map & rhd)
			{
				this->_comparator = rhd._comparator;
				this->_allocator = rhd._allocator;
				this->_tree = rhd._tree;

				return (*this);
			};

			mapped_type &	operator[](const key_type & key)
			{
				iterator	founded = this->find(key);

				if (founded != this->end())
					return ((*founded).second);
				
				return ((*this->insert(value_type(key, mapped_type())).first).second);
			};

			inline iterator	begin(void)
			{
				return (this->_tree.begin());
			};

			inline const_iterator	begin(void)	const
			{
				return (this->_tree.cbegin());
			};

			inline iterator	end(void)
			{
				return (this->_tree.end());
			};

			inline const_iterator	end(void)	const
			{
				return (this->_tree.cend());
			};

			inline reverse_iterator	rbegin(void)
			{
				return (reverse_iterator(this->end()));
			};

			inline const_reverse_iterator	rbegin(void)	const
			{
				return (const_reverse_iterator(this->end()));
			};

			inline reverse_iterator	rend(void)
			{
				return (reverse_iterator(this->begin()));
			};

			inline const_reverse_iterator	rend(void)	const
			{
				return (const_reverse_iterator(this->begin()));
			};

			inline const_iterator	cbegin(void)	const
			{
				return (this->_tree.cbegin());
			};

			inline const_iterator	cend(void)	const
			{
				return (this->_tree.cend());
			};

			inline const_reverse_iterator	crbegin(void)	const
			{
				return (const_reverse_iterator(this->end()));
			};

			inline const_reverse_iterator	crend(void)	const
			{
				return (const_reverse_iterator(this->begin()));
			};

			inline bool	empty(void)	const
			{
				return (!this->_tree.size);
			};

			inline size_type	size(void)	const
			{
				return (this->_tree.size);
			};

			inline size_type	max_size(void)	const
			{
				return (this->_allocator.max_size());
			};

			mapped_type &	at(const key_type & key)
			{
				iterator	founded = this->find(key);

				if (founded == this->end())
					throw std::out_of_range("map");
				
				return ((*founded).second);
			};

			const mapped_type &	at(const key_type & key)	const
			{
				const_iterator	founded = this->find(key);

				if (founded == this->end())
					throw std::out_of_range("map");
				
				return ((*founded).second);
			};

			inline ft::pair<iterator, bool>	insert(const value_type & val)
			{
				return (this->_tree.insert(val));
			};

			inline iterator	insert(iterator position, const value_type & val)
			{
				return (this->_tree.insert(val, position).first);
			};

			template <typename InpIter>
			inline void	insert(InpIter first, InpIter last)
			{
				for (; first != last; first++)
					this->insert(*first);
			};

			inline void	erase(iterator position)
			{
				this->_tree.erase(position.current);
			};

			size_type	erase(const key_type & key)
			{
				iterator	founded = this->find(key);

				if (founded == this->end())
					return (0);
				
				this->_tree.erase(founded.current);
				return (1);
			};

			inline void	erase(iterator first, iterator last)
			{
				for (; first != last; first++)
					this->erase(first);
			};

			void	swap(map & ref)
			{
				Node *	buf = this->_tree.root;

				this->_tree.root = ref._tree.root;
				ref._tree.root = buf;
			};

			inline void	clear(void)
			{
				this->_tree.clear();
			};

			inline key_compare	key_comp(void)	const
			{
				return (this->_comparator);
			};

			inline value_compare	value_comp(void)	const
			{
				return (this->_tree.comparator);
			};

			iterator	find(const key_type & key)
			{
				Node *	founded = this->_find(key);

				if (founded)
					return (iterator(founded));
				return (this->end());
			};

			const_iterator	find(const key_type & key)	const
			{
				Node *	founded = this->_find(key);

				if (founded)
					return (const_iterator(founded));
				return (this->end());
			};

			inline size_type	count(const key_type & key)	const
			{
				return (this->find(key) != this->end());
			};

			inline iterator	lower_bound(const key_type & key)
			{
				(++iterator(this->_tree.find(value_type(key, mapped_type()))));
			};

			inline const_iterator	lower_bound(const key_type & key)	const
			{
				(++const_iterator(this->_tree.find(value_type(key, mapped_type()))));
			};

			iterator	upper_bound(const key_type & key)
			{
				iterator	it = this->lower_bound(key);

				if (!this->_comparator((*it).first, key) && !this->_comparator(key, (*it).first))
					return (++it);
				return (it);
			};

			const_iterator	upper_bound(const key_type & key)	const
			{
				const_iterator	it = this->lower_bound(key);

				if (!this->_comparator((*it).first, key) && !this->_comparator(key, (*it).first))
					return (++it);
				return (it);
			};

			inline ft::pair<iterator, iterator>	equal_range(const key_type & key)
			{
				return (ft::make_pair(this->lower_bound(key), this->upper_bound(key)));
			};

			inline ft::pair<const_iterator, const_iterator>	equal_range(const key_type & key)	const
			{
				return (ft::make_pair(this->lower_bound(key), this->upper_bound(key)));
			};

			inline allocator_type	get_allocator(void)	const
			{
				return (this->_allocator);
			};
	};
};

#endif
