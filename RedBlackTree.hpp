#ifndef REDBLACKTREE_HPP
# define REDBLACKTREE_HPP

# include <stdexcept>
# include <memory>
# include "pair.hpp"
# include "tree_iterator.hpp"
# include "tree_node.hpp"
# include "reverse_iterator.hpp"

# include <iostream>
# include <string>

namespace ft
{
	template <typename T, typename Comparator, typename Alloc>
	class RedBlackTree
	{
		public:
			typedef typename	ft::TreeNode<T>										Node;

		public:
			typedef typename	Alloc::template rebind<Node>::other					allocator_type;
			typedef typename	allocator_type::size_type							size_type;
			typedef typename	ft::tree_iterator<T, Node >							iterator;
			typedef typename	ft::tree_iterator<const T, Node >					const_iterator;
			typedef typename	ft::reverse_iterator<iterator>						reverse_iterator;
			typedef typename	ft::reverse_iterator<const_iterator>				const_reverse_iterator;
			typedef typename	ft::iterator_traits<iterator>::difference_type		difference_type;

		public:

			Node			*	root;
			allocator_type		allocator;
			Comparator			comparator;
			size_type			size;

		public:

			explicit RedBlackTree(allocator_type const & alloc = allocator_type(), Comparator const & comparator = Comparator())
				: root(nullptr), allocator(allocator_type(alloc)), comparator(Comparator(comparator)), size(0) {};

			explicit RedBlackTree(const T & val, allocator_type const & alloc = allocator_type(), Comparator const & comparator = Comparator())
				: root(this->allocator.allocate(1)), allocator(allocator_type(alloc)), comparator(Comparator(comparator)), size(0)
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

				this->_copy(src.root, this->root);
			};

		private:

			void	_copy(const Node * src, Node ** dst, Node * parent = nullptr)
			{
				*dst = this->allocator.allocate(1);
				this->allocator.construct(*dst, *src);
				if (src->left)
					this->_copy(src->left, &(*dst)->left, *dst);
				if (src->right)
					this->_copy(src->left, &(*dst)->right, *dst);
			};

			void	_updateRoot(void)
			{
				if (!this->root)
					return ;
					
				while (this->root->parent)
					this->root = this->root->parent;				
			};

			static void	_insertionRebalance(Node * start)
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
					_insertionRebalance(start->parent->parent);
				}
			};

			static void	_deletionBlackSiblingBlackChildRebalance(Node * start, Node * sibling)
			{
				if (sibling)
					sibling->red = true;
				else
					start->red = true;

				if (start->parent->red)
					start->parent->red = false;
				else
					_deletionRebalance(start->parent);
			}

			static void	_deletionRedSiblingRebalance(Node * start, Node * sibling)
			{
				sibling->getOnSurface();
				sibling->red = false;

				if (start->parent->red)
					throw std::logic_error("parent must be black");
				
				if (*start->parent->dirs[!start->getDir()])
					(*start->parent->dirs[!start->getDir()])->red = true;
			}

			static void	_deletionBlackSiblingRedChildRebalance(Node * start, Node * sibling)
			{
				Node *	red_child = *sibling->dirs[sibling->getDir()];

				if (!red_child || !red_child->red)
					red_child = *sibling->dirs[!red_child->getDir()];
				
				if (!red_child || !red_child->red)
					throw std::range_error("expected at least one red child, got 0");

				if (red_child->getDir() == sibling->getDir())
				{
					sibling->getOnSurface();
					sibling->red = true;
					red_child->red = false;
					if (*sibling->dirs[!red_child->getDir()])
						(*sibling->dirs[!red_child->getDir()])->red = false;
				}
				else
				{
					red_child->getOnSurface();
					red_child->getOnSurface();
					red_child->red = false;
				}
			};

			static void	_deletionRebalance(Node * start)
			{
				if (!start->parent)
					return ;
				
				Node *	sibling = start->getSibling();

				if (!sibling
					|| (!sibling->red && sibling->allChildrensBlack()))
					_deletionBlackSiblingBlackChildRebalance(start, sibling);
				else if (sibling->red)
					_deletionRedSiblingRebalance(start, sibling);
				else
					_deletionBlackSiblingRedChildRebalance(start, sibling);
			};

			void	_checkBranches(Node * start, int ref_height, int cur_height = 0)	const
			{
				if (start && start->red && start->parent && start->parent->red)
				{
					_print_value(this->root, "root: ");
					_print_value(start, "red child: ");
					_print_value(start->parent, "red parent: ");
					_print_value(start->parent->parent, "grandpa: ");
					std::cout << "grandpa color: " << start->parent->parent->red << std::endl;
					_print_value(start->getUncle(), "uncle: ");
					std::cout << "uncle color: " << start->getUncle()->red << std::endl;
					throw std::logic_error("red child have red parent");
				}

				cur_height += (!start || !start->red);

				if (!start)
				{
					if (cur_height != ref_height)
						throw std::logic_error("number of black nodes volatile");

					return ;
				}

				this->_checkBranches(start->left, ref_height, cur_height);
				this->_checkBranches(start->right, ref_height, cur_height);
			};

			void	_checkTree(void)	const
			{
				if (!this->root)
					return ;

				Node *	cursor = this->root;
				int		height = 0;

				while (cursor)
				{
					height += !cursor->red;
					height += !cursor->left;
					cursor = cursor->left;
				}

				this->_checkBranches(this->root, height);
			};

			void	_replaceNode(Node * src, Node * dst)
			{
				if (src)
				{
					if (src->left || src->right)
						throw std::range_error("node have childs, it's cannot be moved");

					src->stealLinks(dst);
				}
				else
					*dst->parent->dirs[dst->getDir()] = nullptr;

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

				this->_replaceNode(cursor, node);

				return (dummy);
			};

			Node *	_iteratorRoutine(bool end)	const
			{
				Node *	cursor = this->root;

				while (cursor && *cursor->dirs[end])
					cursor = *cursor->dirs[end];

				return (cursor);
			};

			Node *	_findPlaceForInsert(const T & val, Node * hint = nullptr)
			{
				if (hint
					&& (
						(this->comparator(val, hint->value) && this->comparator(val, hint->parent->value))
						|| (this->comparator(hint->value, val) && this->comparator(hint->parent->value, val))
						)
					)
					return (this->_findPlaceForInsert(val));
				else if (!hint)
					hint = this->root;

				Node *	crsr = hint;

				while (crsr)
				{
					if (crsr->left && this->comparator(val, crsr->value))
						crsr = crsr->left;
					else if (crsr->right && this->comparator(crsr->value, val))
						crsr = crsr->right;
					else
						break;
				}
				
				return (crsr);
			};

			static void	_print_value(Node * node, std::string before = std::string(""), std::string after = std::string(""))
			{
				if (before.size())
					std::cout << before;
				std::cout << node->value.first << " " << node->value.second;
				if (after.size())
					std::cout << after;
				std::cout << std::endl;
			};

		public:

			ft::pair<iterator, bool>	insert(const T & val, Node * hint = nullptr)
			{
				Node *	parent = this->_findPlaceForInsert(val, hint);
				Node **	insertion_side = &parent;

				if (!parent)
					insertion_side = &this->root;
				else if (this->comparator(val, parent->value))
					insertion_side = &parent->left;
				else if (this->comparator(parent->value, val))
					insertion_side = &parent->right;
				else
					return (ft::make_pair(iterator(parent), false));

				*insertion_side = this->allocator.allocate(1);
				this->allocator.construct(*insertion_side, Node(val, parent));

				_insertionRebalance(*insertion_side);
				this->_updateRoot();
				this->_checkTree();

				return (ft::make_pair(iterator(*insertion_side), bool(++this->size)));
			};

			Node *	find(const T & val)
			{
				return (this->_findPlaceForInsert(val));
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

				node = _internalDeletionHandler(node);
				
				_print_value(node->parent, "after replace: ");
				Node *	child = node->getChild();

				if (node->red || (child && child->red))
				{
					this->_replaceNode(child, node);
					return ;
				}

				_deletionRebalance(node);
				this->_replaceNode(child, node);
				this->_updateRoot();
				this->_checkTree();
			};

			void	clear(Node * start = nullptr)
			{
				if (!start)
					start = this->root;
				if (!start)
					return ;

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

			iterator	begin(void)
			{
				return (iterator(this->_iteratorRoutine(false)));
			};

			const_iterator	cbegin(void)	const
			{
				return (const_iterator(this->_iteratorRoutine(false)));
			};

			iterator	end(void)
			{
				return (++iterator(this->_iteratorRoutine(true)));
			};

			const_iterator	cend(void)	const
			{
				return (++const_iterator(this->_iteratorRoutine(true)));
			};

			reverse_iterator	rbegin(void)
			{
				return (reverse_iterator(this->end()));
			};

			const_reverse_iterator	crbegin(void)	const
			{
				return (const_reverse_iterator(this->cend()));
			};

			reverse_iterator	rend(void)
			{
				return (reverse_iterator(this->begin()));
			};

			const_reverse_iterator	crend(void)	const
			{
				return (const_reverse_iterator(this->begin()));
			};

			void	swap(const RedBlackTree & ref)
			{
				Node *	buf = this->root;

				this->root = ref.root;
				ref.root = buf;
			};
	};
}

#endif
