#ifndef TREE_NODE
# define TREE_NODE

namespace ft
{
	template <typename T>
	class TreeNode
	{
		private:
			TreeNode(const TreeNode * parent = nullptr)
				: value(), red(false), left(nullptr), right(nullptr), parent(parent)
			{
				this->dirs[0] = &this->left;
				this->dirs[1] = &this->right;
			}

		public:

			T				value;
			bool			red;
			TreeNode	*	left;
			TreeNode	*	right;
			TreeNode	*	parent;
			TreeNode	**	dirs[2];

			TreeNode(const T & value, const TreeNode * parent, const bool red = true)
				: TreeNode(parent), value(value), red(red)
			{};

			TreeNode(const TreeNode & src)
				: TreeNode(), value(src.value), red(src.red)
			{};

			~TreeNode() {};

			TreeNode *	getUncle(void)	const
			{
				if (!this->parent || !this->parent->parent)
					throw std::exception("node have no ancestors");
				
				return (*this->parent->parent->dirs[this->parent->getDir()]);
			};
			
			bool	isOuterGrandchild(void)	const
			{
				if (!this->parent || !this->parent->parent)
					throw std::exception("node have no ancestors");
				
				for (int i = 0; i < 2; i++)
					if (*this->parent->parent->dirs[i] == this->parent && *this->parent->dirs[i] == this)
						return (true);

				return (false);
			};

			int	getDir(void)	const
			{
				if (!this->parent)
					throw std::exception("node have no parent");
				
				if (*this->parent->dirs[0] == this)
					return (0);
				return (1);
			};

			void	getOnSurface(void)
			{
				if (!this->parent)
					throw std::exception("node have no parent");

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

			inline void	stealLinks(const TreeNode * src)
			{
				this->stealLinks(*src);
			};

			TreeNode *	getChild(void)	const
			{
				if (this->left && this->right)
					throw std::exception("node have both childs");
				
				if (this->left)
					return (this->left);
				return (this->right);
			};

			TreeNode *	getSibling(void)	const
			{
				if (!this->parent)
					throw std::exception("node dosn't have a parent and have no siblings");
				
				return (*this->parent->dirs[!this->getDir()]);
			};

			inline bool	allChildrensBlack(void)	const
			{
				return ((!this->left || !this->left->red) && (!this->right || !this->right->red));
			};
	};
};

#endif