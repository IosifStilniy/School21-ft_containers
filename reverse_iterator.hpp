#ifndef REVERSE_ITERATOR_HPP
# define REVERSE_ITERATOR_HPP

# include "iterator_traits.hpp"

namespace ft
{
	template <typename Iterator>
	class reverse_iterator : public std::iterator <
		typename ft::iterator_traits<Iterator>::iterator_category,
		typename ft::iterator_traits<Iterator>::value_type,
		typename ft::iterator_traits<Iterator>::difference_type,
		typename ft::iterator_traits<Iterator>::pointer,
		typename ft::iterator_traits<Iterator>::reference
		>
	{
		protected:
			Iterator	m_current;

			typedef typename	ft::iterator_traits<Iterator>	_traits_type;

		public:
			typedef				Iterator						iterator_type;
			typedef	typename	_traits_type::difference_type	difference_type;
			typedef	typename	_traits_type::pointer			pointer;
			typedef	typename	_traits_type::reference			reference;

			reverse_iterator(void) : m_current() {};
			
			explicit reverse_iterator(iterator_type it) : m_current(it) {};

			reverse_iterator(const reverse_iterator & src)
				: m_current(src.base())
			{};

			template <typename Iter>
			reverse_iterator(const reverse_iterator<Iter> & src)
				: m_current(src.base())
			{};


			~reverse_iterator() {};

			reverse_iterator &	operator=(const iterator_type & rhd)
			{
				this->m_current = rhd.m_current;
				return (*this);
			};

			template <typename Iter>
			reverse_iterator &	operator=(const reverse_iterator<Iter> & rhd)
			{
				this->m_current = rhd.base();
				return (*this);
			}

			reverse_iterator	operator+(difference_type n)	const
			{
				return (reverse_iterator(this->m_current - n));
			};

			reverse_iterator &	operator++(void)
			{
				--this->m_current;
				return (*this);
			};

			reverse_iterator	operator++(int)
			{
				reverse_iterator	buf(*this);
				--this->m_current;
				return (buf);
			};

			reverse_iterator	operator-(difference_type n)	const
			{
				return (reverse_iterator(this->m_current + n));
			};

			reverse_iterator &	operator--(void)
			{
				++this->m_current;
				return (*this);
			};

			reverse_iterator	operator--(int)
			{
				reverse_iterator	buf(*this);
				++this->m_current;
				return (buf);
			};

			reverse_iterator &	operator+=(difference_type n)
			{
				this->m_current -= n;
				return (*this);
			};

			reverse_iterator &	operator-=(difference_type n)
			{
				this->m_current += n;
				return (*this);
			};

			reference	operator*(void)	const
			{
				iterator_type	buf = this->m_current;
				--buf;
				return (*buf);
			};

			reference	operator[](difference_type n)	const
			{
				return (*(*this + n));
			};

			pointer	operator->(void)	const
			{
				iterator_type	buf = this->m_current;
				--buf;
				return (&(*buf));
			};

			iterator_type	base(void)	const
			{
				return (this->m_current);
			};

	};

	template <class Iterator>
	typename ft::reverse_iterator<Iterator>
		operator+(typename ft::reverse_iterator<Iterator>::difference_type n, const ft::reverse_iterator<Iterator> & rhd)
	{
		return (reverse_iterator<Iterator>(rhd.base() - n));
	};

	template <class Iterator>
	typename reverse_iterator<Iterator>::difference_type
		operator-(const reverse_iterator<Iterator> & lhd, const reverse_iterator<Iterator> & rhd)
	{
		return (rhd.base() - lhd.base());
	};

	template <class Iterator>
	bool	operator==(const reverse_iterator<Iterator> & lhd, const reverse_iterator<Iterator> & rhd)
	{
		return (rhd.base() == lhd.base());
	};

	template <class IteratorL, class IteratorR>
	bool	operator==(const reverse_iterator<IteratorL> & lhd, const reverse_iterator<IteratorR> & rhd)
	{
		return (rhd.base() == lhd.base());
	};

	template <class Iterator>
	bool	operator!=(const reverse_iterator<Iterator> & lhd, const reverse_iterator<Iterator> & rhd)
	{
		return (rhd.base() != lhd.base());
	};

	template <class IteratorL, class IteratorR>
	bool	operator!=(const reverse_iterator<IteratorL> & lhd, const reverse_iterator<IteratorR> & rhd)
	{
		return (rhd.base() != lhd.base());
	};

	template <class Iterator>
	bool	operator<=(const reverse_iterator<Iterator> & lhd, const reverse_iterator<Iterator> & rhd)
	{
		return (rhd.base() <= lhd.base());
	};

	template <class IteratorL, class IteratorR>
	bool	operator<=(const reverse_iterator<IteratorL> & lhd, const reverse_iterator<IteratorR> & rhd)
	{
		return (rhd.base() <= lhd.base());
	};

	template <class Iterator>
	bool	operator>=(const reverse_iterator<Iterator> & lhd, const reverse_iterator<Iterator> & rhd)
	{
		return (rhd.base() >= lhd.base());
	};

	template <class IteratorL, class IteratorR>
	bool	operator>=(const reverse_iterator<IteratorL> & lhd, const reverse_iterator<IteratorR> & rhd)
	{
		return (rhd.base() >= lhd.base());
	};

	template <class Iterator>
	bool	operator>(const reverse_iterator<Iterator> & lhd, const reverse_iterator<Iterator> & rhd)
	{
		return (rhd.base() > lhd.base());
	};

	template <class IteratorL, class IteratorR>
	bool	operator>(const reverse_iterator<IteratorL> & lhd, const reverse_iterator<IteratorR> & rhd)
	{
		return (rhd.base() > lhd.base());
	};

	template <class Iterator>
	bool	operator<(const reverse_iterator<Iterator> & lhd, const reverse_iterator<Iterator> & rhd)
	{
		return (rhd.base() < lhd.base());
	};

	template <class IteratorL, class IteratorR>
	bool	operator<(const reverse_iterator<IteratorL> & lhd, const reverse_iterator<IteratorR> & rhd)
	{
		return (rhd.base() < lhd.base());
	};
};

#endif
