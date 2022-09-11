#ifndef ITERATOR_TRAITS_HPP
# define ITERATOR_TRAITS_HPP

# include "iterator.hpp"

namespace ft
{
	typedef	long	ptrdiff_t;

	template <class Iterator>
	struct iterator_traits
	{
		typedef typename	Iterator::difference_type		difference_type;
		typedef typename	Iterator::value_type			value_type;
		typedef typename	Iterator::pointer				pointer;
		typedef typename	Iterator::reference				reference;
		typedef typename	Iterator::iterator_category		iterator_category;
	};

	template <class T>
	struct iterator_traits <T *>
	{
		typedef typename	ptrdiff_t						difference_type;
		typedef typename	T								value_type;
		typedef typename	T *								pointer;
		typedef typename	T &								reference;
		typedef typename	ft::random_access_iterator_tag	iterator_category;
	};

	template <class T>
	struct iterator_traits <const T *>
	{
		typedef typename	ptrdiff_t						difference_type;
		typedef				T								value_type;
		typedef				const T *						pointer;
		typedef				const T &						reference;
		typedef typename	ft::random_access_iterator_tag	iterator_category;
	};
};

#endif