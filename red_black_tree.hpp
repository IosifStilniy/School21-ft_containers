#ifndef RED_BLACK_TREE_HPP
# define RED_BLACK_TREE_HPP

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
	class red_black_tree
	{
		public:
			typedef typename	ft::tree_node<T>									node;

		public:
			typedef typename	Alloc::template rebind<node>::other					allocator_type;
			typedef typename	allocator_type::size_type							size_type;
			typedef typename	ft::tree_iterator<T, node >							iterator;
			typedef typename	ft::tree_iterator<const T, node >					const_iterator;
			typedef typename	ft::reverse_iterator<iterator>						reverse_iterator;
			typedef typename	ft::reverse_iterator<const_iterator>				const_reverse_iterator;
			typedef typename	ft::iterator_traits<iterator>::difference_type		difference_type;

		private:

			node			*	m_root;
			allocator_type		m_allocator;
			Comparator			m_comparator;
			size_type			m_size;
			node			*	m_patch;

		public:

			explicit red_black_tree(allocator_type const & alloc = allocator_type(), Comparator const & m_comparator = Comparator())
				: m_root(nullptr), m_allocator(allocator_type(alloc)), m_comparator(Comparator(m_comparator)), m_size(0)
			{
				m_patch = this->m_allocator.allocate(1);
				this->m_allocator.construct(m_patch, node());
			};

			red_black_tree(const red_black_tree & src)
				: m_root(nullptr), m_allocator(src.m_allocator), m_comparator(src.m_comparator), m_size(src.m_size)
			{
				this->_M_copy(src.m_root, &this->m_root);

				m_patch = this->m_allocator.allocate(1);
				this->m_allocator.construct(m_patch, node());
			};

			~red_black_tree()
			{
				this->clear();

				this->m_allocator.destroy(m_patch);
				this->m_allocator.deallocate(m_patch, 1);
			};

			red_black_tree &	operator=(const red_black_tree & src)
			{
				this->clear();
				this->m_size = src.m_size;

				this->_M_copy(src.m_root, &this->m_root);

				return (*this);
			};

		private:

			void	_M_copy(const node * src, node ** dst, node * parent = nullptr)
			{
				if (!src)
				{
					*dst = nullptr;
					return ;
				}

				*dst = this->m_allocator.allocate(1);
				this->m_allocator.construct(*dst, *src);
				(*dst)->parent = parent;
				this->_M_copy(src->left, &(*dst)->left, *dst);
				this->_M_copy(src->right, &(*dst)->right, *dst);
			};

			void	_M_update_root(void)
			{
				if (!this->m_root)
					return ;
					
				while (this->m_root->parent)
					this->m_root = this->m_root->parent;				
			};

			static void	_insertion_rebalance(node * start)
			{
				node *	uncle = start->get_uncle();

				if (!start->parent)
					start->red = false;
				else if (start->parent->red && !start->parent->parent)
					start->parent->red = false;
				else if (start->parent->red && (!uncle || !uncle->red))
				{
					if (!start->is_outer_grandchild())
					{
						node *	buf = start->parent;

						start->get_on_surface();
						start = buf;
					}

					start->parent->get_on_surface();
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
					_insertion_rebalance(start->parent->parent);
				}
			};

			static void	_deletion_rebalance(node * start)
			{
				if (!start->parent)
					return ;
				
				node *	sibling = start->get_sibling();

				if (!sibling->red && sibling->all_childrens_black())
				{
					sibling->red = true;

					if (!start->parent->red)
						_deletion_rebalance(start->parent);
					else
						start->parent->red = false;
				}
				else if (!sibling->red)
				{
					node *	closest = *sibling->dirs[start->get_dir()];

					if (closest && closest->red)
					{
						closest->get_on_surface();
						closest->red = false;
						sibling = closest;
					}

					sibling->get_on_surface();
					sibling->red = start->parent->red;
					start->parent->red = false;
					(*sibling->dirs[!start->get_dir()])->red = false;
				}
				else if (sibling->red)
				{
					sibling->get_on_surface();
					sibling->red = false;
					start->parent->red = true;
					_deletion_rebalance(start);
				}
			};

			void	_M_check_branches(node * start, int ref_height, int cur_height = 0)	const
			{
				if (start && start->red && start->parent && start->parent->red)
					throw std::logic_error("red child have red parent");

				cur_height += (!start || !start->red);

				if (!start)
				{
					if (cur_height != ref_height)
						throw std::logic_error("number of black nodes volatile");

					return ;
				}

				this->_M_check_branches(start->left, ref_height, cur_height);
				this->_M_check_branches(start->right, ref_height, cur_height);
			};

			void	_M_check_tree(void)	const
			{
				if (!this->m_root)
					return ;

				node *	cursor = this->m_root;
				int		height = 1;

				while (cursor)
				{
					height += !cursor->red;
					cursor = cursor->left;
				}

				this->_M_check_branches(this->m_root, height);
			};

			void	_M_replace_node(node * src, node * dst)
			{
				if (src)
					src->steal_links(dst);
				else
					*dst->parent->dirs[dst->get_dir()] = nullptr;

				this->m_allocator.destroy(dst);
				this->m_allocator.deallocate(dst, 1);
			};

			void	_M_delete_node(node * _node)
			{
				node *	child = _node->get_child();

				if (_node->parent)
					*_node->parent->dirs[_node->get_dir()] = child;
				if (child)
				{
					child->parent = _node->parent;
					child->red = _node->red;
				}

				this->m_allocator.destroy(_node);
				this->m_allocator.deallocate(_node, 1);
			};

			node *	_M_internal_deletion_handler(node * _node)
			{
				node *	cursor = nullptr;

				if (_node->left && _node->right)
				{
					cursor = _node->left;
					while (cursor->right)
						cursor = cursor->right;
				}
				else
					return (_node);
				
				node *	dummy = this->m_allocator.allocate(1);

				this->m_allocator.construct(dummy, node());

				dummy->steal_links(cursor);

				this->_M_replace_node(cursor, _node);

				return (dummy);
			};

			node *	_M_iterator_routine(bool end)	const
			{
				node *	cursor = this->m_root;

				while (cursor && *cursor->dirs[end])
					cursor = *cursor->dirs[end];

				return (cursor);
			};

			node *	_M_find_place_for_insert(const T & val, node * hint = nullptr)	const
			{
				if (hint && hint != this->m_root)
				{
					if (this->m_comparator(val, hint->value))
						return (this->_M_find_place_for_insert(val));
					if (this->m_comparator(hint->parent->value, val))
						return (this->_M_find_place_for_insert(val));
					if (!this->m_comparator(hint->parent->value, val) && !this->m_comparator(val, hint->parent->value))
						return (hint->parent);
				}
				else if (!hint)
					hint = this->m_root;

				node *	crsr = hint;

				while (crsr)
				{
					if (crsr->left && this->m_comparator(val, crsr->value))
						crsr = crsr->left;
					else if (crsr->right && this->m_comparator(crsr->value, val))
						crsr = crsr->right;
					else
						break;
				}

				return (crsr);
			};

			static void	_print_value(node * _node, std::string before = std::string(""), std::string after = std::string(""))
			{
				if (before.size())
					std::cout << before;
				if (_node)
					std::cout << _node->value.first << " " << _node->value.second << " color: " << _node->red;
				else
					std::cout << "(null)";
				if (after.size())
					std::cout << after;
				std::cout << std::endl;
			};

		public:

			ft::pair<iterator, bool>	insert(const T & val, node * hint = nullptr)
			{
				node *	parent = this->_M_find_place_for_insert(val, hint);
				node **	insertion_side = &parent;

				if (!parent)
					insertion_side = &this->m_root;
				else if (this->m_comparator(val, parent->value))
					insertion_side = &parent->left;
				else if (this->m_comparator(parent->value, val))
					insertion_side = &parent->right;
				else
					return (ft::make_pair(iterator(parent, this->m_patch), false));

				*insertion_side = this->m_allocator.allocate(1);
				this->m_allocator.construct(*insertion_side, node(val, parent));

				node *	inserted = *insertion_side;

				_insertion_rebalance(inserted);
				this->_M_update_root();
				// this->_M_check_tree();

				return (ft::make_pair(iterator(inserted, this->m_patch), bool(++this->m_size)));
			};

			iterator	find(const T & val)
			{
				node *	founded = this->_M_find_place_for_insert(val);

				if (founded
					&& !this->m_comparator(val, founded->value)
					&& !this->m_comparator(founded->value, val))
					return (iterator(founded, this->m_patch));
				return (this->end());
			};

			const_iterator	find(const T & val)	const
			{
				node *	founded = this->_M_find_place_for_insert(val);

				if (founded
					&& !this->m_comparator(val, founded->value)
					&& !this->m_comparator(founded->value, val))
					return (const_iterator(founded, this->m_patch));
				return (this->cend());
			};

			iterator	lower_bound(const T & key)
			{
				node *	founded = this->_M_find_place_for_insert(key);

				if (!founded || this->m_comparator(key, founded->value)
					|| (!this->m_comparator(founded->value, key)
						&& !this->m_comparator(key, founded->value)))
					return (iterator(founded, this->m_patch));
				return (++iterator(founded, this->m_patch));
			};

			const_iterator	lower_bound(const T & key)	const
			{
				node *	founded = this->_M_find_place_for_insert(key);

				if (!founded || this->m_comparator(key, founded->value)
					|| (!this->m_comparator(founded->value, key)
						&& !this->m_comparator(key, founded->value)))
					return (const_iterator(founded, this->m_patch));
				return (++const_iterator(founded, this->m_patch));
			};

			iterator	upper_bound(const T & key)
			{
				iterator	it = this->lower_bound(key);

				if (it != this->end()
					&& !this->m_comparator(*it, key) && !this->m_comparator(key, *it))
					return (++it);
				return (it);
			};

			const_iterator	upper_bound(const T & key)	const
			{
				const_iterator	it = this->lower_bound(key);

				if (it != this->cend()
					&& !this->m_comparator(*it, key) && !this->m_comparator(key, *it))
					return (++it);
				return (it);
			};

			void	erase(node * _node)
			{
				if (!_node)
					return ;

				if (_node == this->m_root)
				{
					this->m_root = _node->left;
					if (!_node->left)
						this->m_root = _node->right;
				}

				_node = _M_internal_deletion_handler(_node);
				
				node *	child = _node->get_child();

				if (!_node->red && !child)
					_deletion_rebalance(_node);
				
				this->_M_delete_node(_node);
				this->_M_update_root();
				// this->_M_check_tree();
				this->m_size--;
			};

			void	clear(node * start = nullptr)
			{
				if (!start)
					start = this->m_root;
				if (!start)
					return ;

				if (start->left)
					this->clear(start->left);
				if (start->right)
					this->clear(start->right);
				
				this->m_allocator.destroy(start);
				this->m_allocator.deallocate(start, 1);

				if (start != this->m_root)
					return ;

				this->m_root = nullptr;
				this->m_size = 0;
			};

			iterator	begin(void)
			{
				return (iterator(this->_M_iterator_routine(false), this->m_patch));
			};

			const_iterator	cbegin(void)	const
			{
				return (const_iterator(this->_M_iterator_routine(false), this->m_patch));
			};

			iterator	end(void)
			{
				return (++iterator(this->_M_iterator_routine(true), this->m_patch));
			};

			const_iterator	cend(void)	const
			{
				return (++const_iterator(this->_M_iterator_routine(true), this->m_patch));
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
				return (const_reverse_iterator(this->cbegin()));
			};

			void	swap(red_black_tree & ref)
			{
				node *		buf = this->m_root;
				size_type	buf_size = this->m_size;

				this->m_root = ref.m_root;
				this->m_size = ref.m_size;
				ref.m_root = buf;
				ref.m_size = buf_size;
			};

			size_type	size(void)	const
			{
				return (this->m_size);
			};

			allocator_type	get_allocator(void)	const
			{
				return (this->m_allocator);
			};

			Comparator	value_comp(void)	const
			{
				return (this->m_comparator);
			};
	};
}

#endif
