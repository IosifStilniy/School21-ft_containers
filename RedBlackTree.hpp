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

		public:

			Node			*	root;
			allocator_type		allocator;
			Comparator			comparator;
			size_type			size;

			explicit RedBlackTree(allocator_type const & alloc = allocator_type(), Comparator const & comparator = Comparator())
				: root(nullptr), allocator(alloc), comparator(comparator), size(0) {};

			explicit RedBlackTree(const T & val, allocator_type const & alloc = allocator_type(), Comparator const & comparator = Comparator())
				: root(this->allocator.allocate(1)), allocator(alloc), comparator(comparator), size(0)
			{
				this->allocator.construct(this->root, Node(val, false));
				size++;
			};

			~RedBlackTree()
			{
				this->clear();
			};

			RedBlackTree &	operator=(const RedBlackTree & src)
			{
				this->clear();
				this->size = src.size;

				_copy(src.root, this->root);
			};

		private:

			static void	_copy(const Node * src, Node ** dst, Node * parent = nullptr)
			{
				*dst = this->allocator.allocate(1);
				this->allocator.construct(*dst, *src);
				if (src->left)
					_copy(src->left, &(*dst)->left, *dst);
				if (src->right)
					_copy(src->left, &(*dst)->right, *dst);
			};

			void	_updateRoot(void)
			{
				while (this->root->parent)
					this->root = this->root->parent;				
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
			};

			void	_deletionBlackSiblingBlackChildRebalance(Node * start, Node * sibling)
			{
				if (sibling)
					sibling->red = true;
				else
					start->red = true;

				if (start->parent->red)
					start->parent->red = false;
				else
					this->_deletionRebalance(start->parent);
			}

			void	_deletionRedSiblingRebalance(Node * start, Node * sibling)
			{
				sibling->getOnSurface();
				sibling->red = false;

				if (start->parent->red)
					throw std::exception("parent must be black");
				
				if (*start->parent->dirs[!start->getDir()])
					(*start->parent->dirs[!start->getDir()])->red = true;
			}

			void	_deletionBlackSiblingRedChildRebalance(Node * start, Node * sibling)
			{
				Node *	red_child = *sibling->dirs[sibling->getDir()];

				if (!red_child || !red_child->red)
					red_child = *sibling->dirs[!red_child->getDir()];
				
				if (!red_child || !red_child->red)
					throw std::exception("expected at least one red child, got 0");

				if (red_child->getDir() == sibling->getDir())
				{
					sibling->getOnSurface();
					red_child->red = false;
				}
				else
				{
					red_child->getOnSurface();
					red_child->getOnSurface();
					red_child->red = false;
				}
			};

			void	_deletionRebalance(Node * start)
			{
				if (!start->parent)
					return ;
				
				Node *	sibling = start->getSibling();

				if (!sibling
					|| (!sibling->red && sibling->allChildrensBlack()))
					this->_deletionBlackSiblingBlackChildRebalance(start, sibling);
				else if (sibling->red)
					this->_deletionRedSiblingRebalance(start, sibling);
				else
					this->_deletionBlackSiblingRedChildRebalance(start, sibling);
			};

			void	_checkBranches(int ref_height, int cur_height = 0, Node * start = this->root)	const
			{
				if (start->red && start->parent && start->parent->red)
					throw std::exception("red child have red parent");

				cur_height += !start->red;
				cur_height += !start->left;

				if (start->left)
					this->_checkBranches(ref_height, cur_height, start->left);
				if (start->right)
					this->_checkBranches(ref_height, cur_height, start->right);

				if (!start->left && !start->right && cur_height != ref_height)
					throw std::exception("number of black nodes volatile");
			};

			void	_checkTree(void)	const
			{
				Node *	cursor = this->root;
				int		height = 0;

				while (cursor)
				{
					height += !cursor->red;
					height += !cursor->left;
					cursor = cursor->left;
				}

				this->_checkBranches(height);
			};

			void	_replaceNode(Node * src, Node * dst)
			{
				if (src)
				{
					if (src->left || src->right)
						throw std::exception("moved during replacing node have childs");

					src->stealLinks(dst);
				}
				else
					*dst->dirs[dst->getDir()] = nullptr;

				this->allocator.destroy(dst);
				this->allocator.deallocate(dst, 1);
			};
			
			Node *	_internalDeletionHandler(Node * node)
			{
				Node *	cursor = nullptr;

				if (node->left)
				{
					cursor = node->left;
					while (cursor->right)
						cursor = cursor->right;
				}
				else if (node->right)
				{
					cursor = node->right;
					while (cursor->left)
						cursor = cursor->left;
				}
				else
					return (node);

				Node *	dummy = this->allocator.allocate(1);

				this->allocator.construct(dummy, Node());

				dummy->stealLinks(cursor);

				this->_replaceNode(dummy, node);

				return (dummy);
			};

			Node *	_iteratorRoutine(bool end)	const
			{
				Node *	cursor = this->root;

				while (cursor && *cursor->dirs[end])
					cursor = *cursor->dirs[end];
				
				return (cursor);
			};

		public:

			ft::pair<iterator, bool>	insert(const T & val, Node * hint = this->root)
			{
				Node *	parent = this->root;
				Node **	insertion_side = &this->root;

				while (*insertion_side)
				{
					if (this->comparator(val, parent->value))
						insertion_side = &parent->left;
					else if (this->comparator(parent->value, val))
						insertion_side = &parent->right;
					else
						return (ft::make_pair(iterator(parent), false));

					if (*insertion_side)
						parent = *insertion_side;
				}
				
				*insertion_side = this->allocator.allocate(1);
				this->allocator.construct(*insertion_side, Node(val, parent));

				this->_insertionRebalance(*insertion_side);
				this->_updateRoot();
				this->_checkTree();

				return (ft::make_pair(iterator(*insertion_side), ++this->size));
			};

			Node *	find(const T & val)
			{
				Node *	cursor = this->root;

				while (cursor)
				{
					if (this->comparator(val, cursor->value))
						cursor = cursor->left;
					else if (this->comparator(cursor->value, val))
						cursor = cursor->right;
					else
						break ;
				}

				return (cursor);
			};

			void	erase(Node * node)
			{
				this->size--;

				if (node == this->root)
				{
					this->root = node->left;
					if (!node->left)
						this->root = node->right;
				}

				node = this->_internalDeletionHandler(node);

				Node *	child = node->getChild();

				if (node->red || (child && child->red))
				{
					this->_replaceNode(child, node);
					return ;
				}

				this->_deletionRebalance(node);
				this->_replaceNode(child, node);
				this->_updateRoot();
				this->_checkTree();
			};

			void	clear(Node * start = this->root)
			{
				if (start->left)
					this->clear(start->left);
				if (start->right)
					this->clear(start->right);
				
				this->allocator.destroy(start);
				this->allocator.deallocate(start, 1);

				if (start != this->root)
					return ;

				this->root = nullptr;
				this->size = 0;
			};

			inline iterator	begin(void)
			{
				return (iterator(this->_iteratorRoutine(true)));
			};

			inline const_iterator	cbegin(void)	const
			{
				return (const_iterator(this->_iteratorRoutine(true)));
			};

			inline iterator	end(void)
			{
				return (++iterator(this->_iteratorRoutine(true)));
			};

			inline const_iterator	cend(void)	const
			{
				return (++const_iterator(this->_iteratorRoutine(true)));
			};
	};
}

#endif
