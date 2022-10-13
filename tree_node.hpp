#ifndef TREE_NODE_HPP
# define TREE_NODE_HPP

# include <stdexcept>

namespace ft
{
	template <typename T>
	class tree_node
	{
		public:
			typedef T	value_type;

			T				value;
			bool			red;
			tree_node	*	left;
			tree_node	*	right;
			tree_node	*	parent;
			tree_node	**	dirs[2];

			tree_node(void)
				: value(), red(false), left(nullptr), right(nullptr), parent(nullptr)
			{
				this->dirs[0] = &this->left;
				this->dirs[1] = &this->right;
			};

			tree_node(const T & value, tree_node * parent, const bool red = true)
				: value(value)
			{
				*this = tree_node();
				this->parent = parent;
				this->red = red;

				this->dirs[0] = &this->left;
				this->dirs[1] = &this->right;
			};

			tree_node(const tree_node & src)
				: value(src.value)
			{
				*this = src;

				this->dirs[0] = &this->left;
				this->dirs[1] = &this->right;
			};

			~tree_node() {};

			tree_node &	operator=(tree_node const & rhd)
			{
				this->red = rhd.red;
				this->left = rhd.left;
				this->right = rhd.right;
				this->parent = rhd.parent;

				return (*this);
			};

			tree_node *	get_uncle(void)	const
			{
				if (!this->parent || !this->parent->parent)
					return (nullptr);
				
				return (*this->parent->parent->dirs[!this->parent->get_dir()]);
			};
			
			bool	is_outer_grandchild(void)	const
			{
				if (!this->parent || !this->parent->parent)
					throw std::range_error("node have no ancestors");
				
				for (int i = 0; i < 2; i++)
					if (*this->parent->parent->dirs[i] == this->parent && *this->parent->dirs[i] == this)
						return (true);

				return (false);
			};

			int	get_dir(void)	const
			{
				if (!this->parent)
					return (-1);
				
				if (*this->parent->dirs[0] == this)
					return (0);
				return (1);
			};

			void	get_on_surface(void)
			{
				if (!this->parent)
					throw std::range_error("node have no parent");

				int		i = this->get_dir();

				*this->parent->dirs[i] = *this->dirs[!i];
				if (*this->dirs[!i])
					(*this->dirs[!i])->parent = this->parent;
				*this->dirs[!i] = this->parent;
				this->parent = this->parent->parent;
				(*this->dirs[!i])->parent = this;

				if (!this->parent)
					return ;

				if (*this->parent->dirs[i] != *this->dirs[!i])
					i = !i;

				*this->parent->dirs[i] = this;
			};

			void	steal_links(const tree_node & src)
			{
				this->parent = src.parent;
				this->left = src.left;
				this->right = src.right;
				this->red = src.red;

				if (this->left)
					this->left->parent = this;
				if (this->right)
					this->right->parent = this;
				if (this->parent)
					*this->parent->dirs[src.get_dir()] = this;
			};

			void	steal_links(const tree_node * src)
			{
				this->steal_links(*src);
			};

			tree_node *	get_child(void)	const
			{
				if (this->left)
					return (this->left);
				return (this->right);
			};

			tree_node *	get_sibling(void)	const
			{
				if (!this->parent)
					throw std::range_error("node dosn't have a parent and have no siblings");
				
				return (*this->parent->dirs[!this->get_dir()]);
			};

			bool	all_childrens_black(void)	const
			{
				return ((!this->left || !this->left->red) && (!this->right || !this->right->red));
			};
	};
};

#endif