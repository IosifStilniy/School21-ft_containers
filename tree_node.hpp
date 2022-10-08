#ifndef TREE_NODE
# define TREE_NODE

# include <stdexcept>

namespace ft
{
	template <typename T>
	class TreeNode
	{
		public:

			T				value;
			bool			red;
			TreeNode	*	left;
			TreeNode	*	right;
			TreeNode	*	parent;
			TreeNode	**	dirs[2];

			TreeNode(void)
				: value(), red(false), left(nullptr), right(nullptr), parent(nullptr)
			{
				this->dirs[0] = &this->left;
				this->dirs[1] = &this->right;
			};

			TreeNode(const T & value, TreeNode * parent, const bool red = true)
				: value(value)
			{
				*this = TreeNode();
				this->parent = parent;
				this->red = red;

				this->dirs[0] = &this->left;
				this->dirs[1] = &this->right;
			};

			TreeNode(const TreeNode & src)
				: value(src.value)
			{
				*this = src;

				this->dirs[0] = &this->left;
				this->dirs[1] = &this->right;
			};

			~TreeNode() {};

			TreeNode &	operator=(TreeNode const & rhd)
			{
				this->red = rhd.red;
				this->left = rhd.left;
				this->parent = rhd.parent;

				return (*this);
			};

			TreeNode *	getUncle(void)	const
			{
				if (!this->parent || !this->parent->parent)
					return (nullptr);
				
				return (*this->parent->parent->dirs[!this->parent->getDir()]);
			};
			
			bool	isOuterGrandchild(void)	const
			{
				if (!this->parent || !this->parent->parent)
					throw std::range_error("node have no ancestors");
				
				for (int i = 0; i < 2; i++)
					if (*this->parent->parent->dirs[i] == this->parent && *this->parent->dirs[i] == this)
						return (true);

				return (false);
			};

			int	getDir(void)	const
			{
				if (!this->parent)
					return (-1);
				
				if (*this->parent->dirs[0] == this)
					return (0);
				return (1);
			};

			void	getOnSurface(void)
			{
				if (!this->parent)
					throw std::range_error("node have no parent");

				int		i = this->getDir();

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

			void	stealLinks(const TreeNode & src)
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
					*this->parent->dirs[src.getDir()] = this;
			};

			void	stealLinks(const TreeNode * src)
			{
				this->stealLinks(*src);
			};

			TreeNode *	getChild(void)	const
			{
				if (this->left && this->right)
					throw std::range_error("node have both childs");
				
				if (this->left)
					return (this->left);
				return (this->right);
			};

			TreeNode *	getSibling(void)	const
			{
				if (!this->parent)
					throw std::range_error("node dosn't have a parent and have no siblings");
				
				return (*this->parent->dirs[!this->getDir()]);
			};

			bool	allChildrensBlack(void)	const
			{
				return ((!this->left || !this->left->red) && (!this->right || !this->right->red));
			};
	};
};

#endif