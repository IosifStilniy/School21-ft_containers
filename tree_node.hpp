#ifndef TREE_NODE
# define TREE_NODE

namespace ft
{
	template <typename T>
	class TreeNode
	{
		public:

			T			value;
			bool		red;
			TreeNode	*	left;
			TreeNode	*	right;
			TreeNode	*	parent;
			TreeNode	**	dirs[2];

			TreeNode(const T & value, const TreeNode & parent, const bool red = true)
				: value(value), left(nullptr), right(nullptr), red(red), parent(parent)
			{
				this->dirs[0] = &this->left;
				this->dirs[1] = &this->right;
			};

			~TreeNode() {};

			TreeNode *	getUncle(void)	const
			{
				if (!this->parent || !this->parent->parent)
					return (nullptr);
				
				if (this->parent->parent->left == this->parent)
					return (this->parent->parent->right);
				return (this->parent->parent->left);
			};
			
			bool	isOuterGrandchild(void)	const
			{
				if (!this->parent || !this->parent->parent)
					return (false);
				
				for (int i = 0; i < 2; i++)
					if (*this->parent->parent->dirs[i] == this->parent && *this->parent->dirs[i] == this)
						return (true);

				return (false);
			};

			int	getDir(void)	const
			{
				if (!this->parent)
					return (0);
				
				if (*this->parent->dirs[0] == this)
					return (0);
				return (1);
			};

			void	getOnSurface(void)
			{
				if (!this->parent)
					return ;

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
	};
};

#endif