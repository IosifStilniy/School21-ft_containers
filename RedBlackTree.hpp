#ifndef REDBLACKTREE_HPP
# define REDBLACKTREE_HPP

# include "pair.hpp"
# include "tree_iterator.hpp"
# include "tree_node.hpp"

namespace ft
{
	template <typename T, typename Comparator, typename Alloc>
	class RedBlackTree
	{
		private:
			typedef typename	ft::TreeNode<T>										Node;

		public:
			typedef				Alloc												allocator_type;
			typedef typename	allocator_type::size_type							size_type;
			typedef typename	ft::tree_iterator<T, Node >							iterator;
			typedef typename	ft::tree_iterator<const T, Node >					const_iterator;
			typedef typename	ft::iterator_traits<iterator>::difference_type		difference_type;
			typedef typename	ft::reverse_iterator<iterator>						reverse_iterator;
			typedef typename	ft::reverse_iterator<const_iterator>				const_reverse_iterator;

		private:

			Node			*	_root;
			allocator_type		_allocator;
			Comparator			_comparator;

			void	_updateRoot(void)
			{
				while (this->_root->parent)
					this->_root = this->_root->parent;				
			};

			void	_insertionRebalance(Node * start)
			{
				Node *	uncle = start->getUncle();

				if (!start->parent)
					start->red = false;
				else if (start->parent->red && !start->parent->parent)
					start->parent->red = false;
				else if (start->parent->red && (!uncle || !uncle->red))
				{
					if (!start->isOuterGrandchild())
					{
						Node *	buf = start->parent;

						start->getOnSurface();
						start = buf;
					}

					start->parent->getOnSurface();

					start->parent->red = false;

					if (start->parent->left)
						start->parent->left->red = true;
					if (start->parent->right)
						start->parent->right->red = true;
				}
				else if (start->parent->red && uncle->red)
				{
					start->parent->red = false;
					uncle->red = false;
					start->parent->parent->red = true;
					this->_insertionRebalance(start->parent->parent);
				}

				this->_updateRoot();
			};

		public:
			explicit RedBlackTree(allocator_type const & alloc = allocator_type(), Comparator const & comparator = Comparator())
				: _root(nullptr), _allocator(alloc), _comparator(comparator) {};

			explicit RedBlackTree(const T & val, allocator_type const & alloc = allocator_type(), Comparator const & comparator = Comparator())
				: _root(this->_allocator.allocate(1)), _allocator(alloc), _comparator(comparator)
			{
				this->_allocator.construct(this->_root, Node(val, false));
			};
			~RedBlackTree()
			{
				this->clear();
			};

			ft::pair<iterator, bool>	insert(const T & val)
			{
				Node *	parent = this->_root;
				Node **	insertion_side = &this->_root;

				while (*insertion_side)
				{
					if (this->_comparator(val, parent->value))
						insertion_side = &parent->left;
					else if (this->_comparator(parent->value, val))
						insertion_side = &parent->right;
					else
						return (ft::make_pair(iterator(parent), false));

					if (*insertion_side)
						parent = *insertion_side;
				}
				
				*insertion_side = this->_allocator.allocate(1);
				this->_allocator.construct(*insertion_side, Node(val, parent));

				this->_insertionRebalance(*insertion_side);

				return (ft::make_pair(iterator(*insertion_side), true));
			};

			void	erase(Node * node)
			{

			};

			void	clear(Node * start = this->_root)
			{
				if (start->left)
					this->_clearTree(start->left);
				if (start->right)
					this->_clearTree(start->right);
				
				this->_allocator.destroy(start);
				this->_allocator.deallocate(start, 1);
			};
	};
}

#endif
