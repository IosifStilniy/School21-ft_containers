#ifndef ITERATOR_HPP
# define ITERATOR_HPP

namespace ft
{
	class input_iterator_tag {};
	class output_iterator_tag {};
	class forward_iterator_tag {};
	class bidirectional_iterator_tag {};
	class random_access_iterator_tag {};
};

namespace ft
{
	template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T& >
	struct iterator
	{
		typedef typename	Category	iterator_category;
		typedef typename	T			value_type;
		typedef typename	Distance	difference_type;
		typedef typename	Pointer		pointer;
		typedef typename	Reference	reference;
	};
};

#endif
