#ifndef VECTOR_ITERATOR_HPP
# define VECTOR_ITERATOR_HPP

namespace ft
{
	template <typename Iterator>
	class _iterator
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
			pointer	_current;

		public:
			_iterator(void) {};

			_iterator(const _iterator & src)
			{
				*this = src;
			};

			_iterator(const pointer & src) : _current(src) {};

			~_iterator() {};

			_iterator &	operator=(const _iterator & rhd)
			{
				this->_current = rhd._current;
				return (*this);
			};

			reference	operator*(void)	const
			{
				return (*_current);
			};

			pointer	operator->(void)	const
			{
				return (_current);
			};

			reference	operator[](difference_type n)	const
			{
				return (this->_current[n]);
			};

			_iterator	operator+(difference_type n)	const
			{
				return (_iterator(this->_current + n));
			};

			_iterator &	operator+=(difference_type n)
			{
				this->_current += n;
				return (*this);
			};

			_iterator &	operator++(void)
			{
				this->_current++;
				return (*this);
			};

			_iterator	operator++(int)
			{
				return (_iterator(this->_current++));
			};

			_iterator	operator-(difference_type n)	const
			{
				return (_iterator(this->_current - n));
			};

			_iterator &	operator-=(difference_type n)
			{
				this->_current -= n;
				return (*this);
			};

			_iterator &	operator--(void)
			{
				this->_current--;
				return (*this);
			};

			_iterator	operator--(int)
			{
				return (_iterator(this->_current--));
			};

			const pointer &	base(void)	const
			{
				return (this->_current);
			};
	};

	template <typename IterL, typename IterR>
	bool	operator==(_iterator<IterL> const & lhd, _iterator<IterR> const & rhd)
	{
		return (lhd.base() == rhd.base());
	};

	template <typename Iter>
	bool	operator==(_iterator<Iter> const & lhd, _iterator<Iter> const & rhd)
	{
		return (lhd.base() == rhd.base());
	};

	template <typename IterL, typename IterR>
	bool	operator!=(_iterator<IterL> const & lhd, _iterator<IterR> const & rhd)
	{
		return (lhd.base() != rhd.base());
	};

	template <typename Iter>
	bool	operator!=(_iterator<Iter> const & lhd, _iterator<Iter> const & rhd)
	{
		return (lhd.base() != rhd.base());
	};

	template <typename IterL, typename IterR>
	bool	operator>(_iterator<IterL> const & lhd, _iterator<IterR> const & rhd)
	{
		return (lhd.base() > rhd.base());
	};

	template <typename Iter>
	bool	operator>(_iterator<Iter> const & lhd, _iterator<Iter> const & rhd)
	{
		return (lhd.base() > rhd.base());
	};

	template <typename IterL, typename IterR>
	bool	operator>=(_iterator<IterL> const & lhd, _iterator<IterR> const & rhd)
	{
		return (lhd.base() >= rhd.base());
	};

	template <typename Iter>
	bool	operator>=(_iterator<Iter> const & lhd, _iterator<Iter> const & rhd)
	{
		return (lhd.base() >= rhd.base());
	};

	template <typename IterL, typename IterR>
	bool	operator<(_iterator<IterL> const & lhd, _iterator<IterR> const & rhd)
	{
		return (lhd.base() < rhd.base());
	};

	template <typename Iter>
	bool	operator<(_iterator<Iter> const & lhd, _iterator<Iter> const & rhd)
	{
		return (lhd.base() < rhd.base());
	};

	template <typename IterL, typename IterR>
	bool	operator<=(_iterator<IterL> const & lhd, _iterator<IterR> const & rhd)
	{
		return (lhd.base() <= rhd.base());
	};

	template <typename Iter>
	bool	operator<=(_iterator<Iter> const & lhd, _iterator<Iter> const & rhd)
	{
		return (lhd.base() <= rhd.base());
	};

	template <typename Iter>
	typename _iterator<Iter>::difference_type	operator-(_iterator<Iter> const & lhd, _iterator<Iter> const & rhd)
	{
		return (rhd.base() - lhd.base());
	}

	template <typename Iter>
	_iterator<Iter>	operator+(typename _iterator<Iter>::difference_type n, _iterator<Iter> const & rhd)
	{
		return (_iterator<Iter>(rhd.base() + n));
	}
}
#endif
