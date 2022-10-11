#ifndef TREE_ITERATOR_HPP
# define TREE_ITERATOR_HPP

# include <iterator>
# include "type_traits.hpp"

namespace ft
{
	template <typename T, typename Node>
	class tree_iterator
	{
		public:
			typedef				ptrdiff_t							difference_type;
			typedef				T									value_type;
			typedef				T *									pointer;
			typedef 			T &									reference;
			typedef typename	std::bidirectional_iterator_tag		iterator_category;

			Node *	current;
			Node *	root;

		private:
			void	_go(bool forward)
			{
				if (*this->current->dirs[forward])
				{
					this->current = *this->current->dirs[forward];

					while (*this->current->dirs[!forward])
						this->current = *this->current->dirs[!forward];
					
					return ;
				}

				while (this->current->getDir() == forward)
					this->current = this->current->parent;
				this->current = this->current->parent;
			};


			tree_iterator(Node * node, Node *root) : current(node), root(root) {};

		public:
			tree_iterator(void) {};

			tree_iterator(Node * node) : current(node)
			{
				while (node && node->parent)
					node = node->parent;
				this->root = node;
			};

			tree_iterator(const tree_iterator & it) : current(it.current), root(it.root) {};

			template <typename _T>
			tree_iterator(const tree_iterator<
												_T,
												typename ft::enable_if<(ft::is_same<const _T, T>::value), Node>::type
												> & it)
				: current(it.current), root(it.root)
			{};

			~tree_iterator() {};

			tree_iterator &	operator=(const tree_iterator & rhd)
			{
				this->current = rhd.current;
				this->root = rhd.root;

				return (*this);
			};

			template <typename _T>
			tree_iterator &	operator=(const tree_iterator<
															_T,
															typename ft::enable_if<(ft::is_same<const _T, T>::value), Node>::type
															> & rhd)
			{
				this->current = rhd.current;
				this->root = rhd.root;

				return (*this);
			};

			reference	operator*(void)	const
			{
				return (this->current->value);
			};

			pointer	operator->(void)	const
			{
				return (&this->current->value);
			};

			tree_iterator	operator++(void)
			{
				if (!this->root)
					return (*this);

				if (!this->current)
				{
					this->current = this->root;
					while (this->current->left)
						this->current = this->current->left;
					
					return (*this);
				}

				this->_go(true);

				return (*this);
			};

			tree_iterator	operator++(int)
			{
				if (!this->root)
					return (*this);

				tree_iterator	it = tree_iterator(this->current);

				if (!this->current)
				{
					this->current = this->root;

					while (this->current->left)
						this->current = this->current->left;
					
					return (*this);
				}

				this->_go(true);

				return (it);
			};

			tree_iterator	operator--(void)
			{
				if (!this->root)
					return (*this);

				if (!this->current)
				{
					this->current = this->root;
					while (this->current->right)
						this->current = this->current->right;
					
					return (*this);
				}

				this->_go(false);

				return (*this);
			};

			tree_iterator	operator--(int)
			{
				if (!this->root)
					return (*this);
				
				tree_iterator	it = tree_iterator(this->current);

				if (!this->current)
				{
					this->current = this->root;
					while (this->current->right)
						this->current = this->current->right;
					
					return (*this);
				}

				this->_go(false);

				return (it);
			};

			bool	operator==(const tree_iterator & rhd)	const
			{
				return (this->current == rhd.current);
			};

			bool	operator!=(const tree_iterator & rhd)	const
			{
				return (this->current != rhd.current);
			};
	};

	template <typename Iter1, typename Iter2>
	inline bool	operator==(const Iter1 & lhd, const Iter2 & rhd)
	{
		return (lhd.current == rhd.current);
	};

	template <typename Iter1, typename Iter2>
	inline bool	operator!=(const Iter1 & lhd, const Iter2 & rhd)
	{
		return (lhd.current != rhd.current);
	};
};

#endif
