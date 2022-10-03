#ifndef REDBLACKTREE_HPP
# define REDBLACKTREE_HPP

# include "pair.hpp"

namespace ft
{
	template <typename T, typename Comparator, typename Alloc>
	class RedBlackTree
	{
		public:
			typedef				Alloc						allocator_type;
			typedef typename	allocator_type::size_type	size_type;

		private:

			class TreeNode
			{
				friend RedBlackTree;

				private:

					T				value;
					bool			red;
					TreeNode	*	left;
					TreeNode	*	right;
					TreeNode	*	parent;

					TreeNode(const T & value, const bool red = false)
						: value(value), left(nullptr), right(nullptr), red(red) {};

				public:
					~TreeNode() {};
			};
			
			TreeNode		*	_root;
			allocator_type		_allocator;
			Comparator			_comparator;

		public:
			explicit RedBlackTree(allocator_type const & alloc = allocator_type(), Comparator const & comparator = Comparator())
				: _root(nullptr), _allocator(alloc), _comparator(comparator) {};

			explicit RedBlackTree(const T & val, allocator_type const & alloc = allocator_type(), Comparator const & comparator = Comparator())
				: _root(this->_allocator.allocate(1)), _allocator(alloc), _comparator(comparator)
			{
				this->_allocator.construct(this->_root, TreeNode(val));
			};

			ft::pair<TreeNode *, bool>	insert(const T & val, TreeNode * parent = this->_root)
			{
				TreeNode **	insertion_size = nullptr;

				if (this->_comparator(val, parent.value))
					insertion_size = &parent->left;
				else if (this->_comparator(parent.value, val))
					insertion_size = &parent->right;
				else
					return (ft::make_pair(parent, false));
				
				if (*insertion_size)
					return (this->insert(val, *insertion_size));
				
				*insertion_size = this->_allocator.allocate(1);
				*insertion_size = TreeNode(val);

				return (ft::make_pair(*insertion_size, true));
			};

			size_type	erase(TreeNode * node)
			{
				// if (node->left)
				// 	erased = this->erase(node->left, erased);
				// if (node->right)
				// 	erased = this->erase(node->right, erased);
				
				// if (node->parent)
				// {
				// 	if (node->parent->left == node)
				// 		node->parent->left = nullptr;
				// 	else
				// 		node->parent->right = nullptr;
				// }

				// this->_allocator.destroy(node);
				// this->_allocator.deallocate(node, 1);

				// return (++erased);
			};

			~RedBlackTree();
	};
}

#endif
