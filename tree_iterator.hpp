#ifndef TREE_ITERATOR_HPP
# define TREE_ITERATOR_HPP

namespace ft
{
	template <typename T, typename Node>
	class tree_iterator
	{
		public:
			typedef				ptrdiff_t							difference_type;
			typedef				T									value_type;
			typedef				T *									pointer;
			typedef typename	T &									reference;
			typedef typename	std::bidirectional_iterator_tag		iterator_category;

			Node *	current;

		private:
			void	_go(bool forward)
			{
				if (*this->current->dirs[forward])
				{
					this->current = *this->current->dirs[forward];
					while (*this->current->dirs[!forward])
						this->current = *this->current->dirs[!forward];
				}
				else if (!this->current->getDir() != forward)
					this->current = this->current->parent;
				else
				{
					this->current = this->current->parent;

					if (!this->current)
						return ;

					while (this->current->getDir() == forward)
						this->current = this->current->parent;
					this->current = this->current->parent;
				}
			};

			void	_goForward(void)
			{
				if (this->current->right)
				{
					this->current = this->current->right;
					while (this->current->left)
						this->current = this->current->left;
				}
				else if (!this->current->getDir())
					this->current = this->current->parent;
				else
				{
					this->current = this->current->parent;

					if (!this->current)
						return ;

					while (this->current->getDir())
						this->current = this->current->parent;
					this->current = this->current->parent;
				}
			};

			void	_goBackward(void)
			{
				if (this->current->left)
				{
					this->current = this->current->left;
					while (this->current->right)
						this->current = this->current->right;
				}
				else if (this->current->getDir())
					this->current = this->current->parent;
				else
				{
					this->current = this->current->parent;

					if (!this->current)
						return ;

					while (!this->current->getDir())
						this->current = this->current->parent;
					this->current = this->current->parent;
				}
			};

		public:
			explicit tree_iterator(Node * node) : current(node) {};
			explicit tree_iterator(const tree_iterator & it) : current(it.current) {};
			~tree_iterator() {};

			tree_iterator &	operator=(const tree_iterator & rhd)
			{
				this->current = rhd.current;
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
				if (!this->current)
					return (tree_iterator(this->current));

				this->_go(true);

				return (tree_iterator(this->current));
			};

			tree_iterator	operator++(int)
			{
				tree_iterator	it = tree_iterator(this->current);

				if (!this->current)
					return (it);

				this->_go(true);

				return (it);
			};

			tree_iterator	operator--(void)
			{
				if (!this->current)
					return (tree_iterator(this->current));

				this->_go(false);

				return (tree_iterator(this->current));
			};

			tree_iterator	operator--(int)
			{
				tree_iterator	it = tree_iterator(this->current);

				if (!this->current)
					return (it);

				this->_go(false);

				return (it);
			};

			bool	operator==(const tree_iterator & lhd)	const
			{
				return (this->current == lhd.current);
			};

			bool	operator!=(const tree_iterator & lhd)	const
			{
				return (this->current != lhd.current);
			};
	};
};

#endif
