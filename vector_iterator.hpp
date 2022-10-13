#ifndef VECTOR_ITERATOR_HPP
# define VECTOR_ITERATOR_HPP

# include "type_traits.hpp"
# include "iterator_traits.hpp"

namespace ft
{
	template <typename Iterator, typename Pointer>
	class vector_iterator
	{
		private:
			typedef				ft::iterator_traits<Iterator>		_traits_type;

		public:
			typedef	typename	_traits_type::iterator_category		iterator_category;
			typedef	typename	_traits_type::value_type			value_type;
			typedef	typename	_traits_type::difference_type		difference_type;
			typedef	typename	_traits_type::reference				reference;
			typedef	typename	_traits_type::pointer				pointer;

		private:
			pointer	m_current;

		public:
			vector_iterator(void) {};

			vector_iterator(const vector_iterator & src)
				: m_current(src.m_current)
			{};

			template <typename Iter>
			vector_iterator(const vector_iterator<
													Iter,
													typename ft::enable_if<ft::is_same<Iter, Pointer>::value, Pointer>::type
													> & src)
					: m_current(src.base())
			{};

			vector_iterator(const pointer & src) : m_current(src) {};

			~vector_iterator() {};

			vector_iterator &	operator=(const vector_iterator & rhd)
			{
				this->m_current = rhd.m_current;
				return (*this);
			};

			template <typename Iter>
			vector_iterator &	operator=(const vector_iterator<
																Iter,
																typename ft::enable_if<ft::is_same<Iter, Pointer>::value, Pointer>::type
													> & rhd)
			{
				this->m_current = rhd.base();
				return (*this);
			}

			reference	operator*(void)	const
			{
				return (*m_current);
			};

			pointer	operator->(void)	const
			{
				return (m_current);
			};

			reference	operator[](difference_type n)	const
			{
				return (this->m_current[n]);
			};

			vector_iterator	operator+(difference_type n)	const
			{
				return (vector_iterator(this->m_current + n));
			};

			vector_iterator &	operator+=(difference_type n)
			{
				this->m_current += n;
				return (*this);
			};

			vector_iterator &	operator++(void)
			{
				this->m_current++;
				return (*this);
			};

			vector_iterator	operator++(int)
			{
				return (vector_iterator(this->m_current++));
			};

			vector_iterator	operator-(difference_type n)	const
			{
				return (vector_iterator(this->m_current - n));
			};

			vector_iterator &	operator-=(difference_type n)
			{
				this->m_current -= n;
				return (*this);
			};

			vector_iterator &	operator--(void)
			{
				this->m_current--;
				return (*this);
			};

			vector_iterator	operator--(int)
			{
				return (vector_iterator(this->m_current--));
			};

			const pointer &	base(void)	const
			{
				return (this->m_current);
			};
	};

	template <typename IterL, typename IterR, typename Pointer>
	inline bool	operator==(vector_iterator<IterL, Pointer> const & lhd, vector_iterator<IterR, Pointer> const & rhd)
	{
		return (lhd.base() == rhd.base());
	};

	template <typename Iter, typename Pointer>
	inline bool	operator==(vector_iterator<Iter, Pointer> const & lhd, vector_iterator<Iter, Pointer> const & rhd)
	{
		return (lhd.base() == rhd.base());
	};

	template <typename IterL, typename IterR, typename Pointer>
	inline bool	operator!=(vector_iterator<IterL, Pointer> const & lhd, vector_iterator<IterR, Pointer> const & rhd)
	{
		return (lhd.base() != rhd.base());
	};

	template <typename Iter, typename Pointer>
	inline bool	operator!=(vector_iterator<Iter, Pointer> const & lhd, vector_iterator<Iter, Pointer> const & rhd)
	{
		return (lhd.base() != rhd.base());
	};

	template <typename IterL, typename IterR, typename Pointer>
	inline bool	operator>(vector_iterator<IterL, Pointer> const & lhd, vector_iterator<IterR, Pointer> const & rhd)
	{
		return (lhd.base() > rhd.base());
	};

	template <typename Iter, typename Pointer>
	inline bool	operator>(vector_iterator<Iter, Pointer> const & lhd, vector_iterator<Iter, Pointer> const & rhd)
	{
		return (lhd.base() > rhd.base());
	};

	template <typename IterL, typename IterR, typename Pointer>
	inline bool	operator>=(vector_iterator<IterL, Pointer> const & lhd, vector_iterator<IterR, Pointer> const & rhd)
	{
		return (lhd.base() >= rhd.base());
	};

	template <typename Iter, typename Pointer>
	inline bool	operator>=(vector_iterator<Iter, Pointer> const & lhd, vector_iterator<Iter, Pointer> const & rhd)
	{
		return (lhd.base() >= rhd.base());
	};

	template <typename IterL, typename IterR, typename Pointer>
	inline bool	operator<(vector_iterator<IterL, Pointer> const & lhd, vector_iterator<IterR, Pointer> const & rhd)
	{
		return (lhd.base() < rhd.base());
	};

	template <typename Iter, typename Pointer>
	inline bool	operator<(vector_iterator<Iter, Pointer> const & lhd, vector_iterator<Iter, Pointer> const & rhd)
	{
		return (lhd.base() < rhd.base());
	};

	template <typename IterL, typename IterR, typename Pointer>
	inline bool	operator<=(vector_iterator<IterL, Pointer> const & lhd, vector_iterator<IterR, Pointer> const & rhd)
	{
		return (lhd.base() <= rhd.base());
	};

	template <typename Iter, typename Pointer>
	inline bool	operator<=(vector_iterator<Iter, Pointer> const & lhd, vector_iterator<Iter, Pointer> const & rhd)
	{
		return (lhd.base() <= rhd.base());
	};

	template <typename Iter, typename Pointer>
	inline typename vector_iterator<Iter, Pointer>::difference_type	operator-(vector_iterator<Iter, Pointer> const & lhd, vector_iterator<Iter, Pointer> const & rhd)
	{
		return (lhd.base() - rhd.base());
	}

	template <typename Iter, typename Pointer>
	inline vector_iterator<Iter, Pointer>	operator+(typename vector_iterator<Iter, Pointer>::difference_type n, vector_iterator<Iter, Pointer> const & rhd)
	{
		return (vector_iterator<Iter, Pointer>(rhd.base() + n));
	}
}
#endif
