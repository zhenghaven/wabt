#ifndef SGX_STRING_VIEW_H_
#define SGX_STRING_VIEW_H_

#ifdef INTEL_SGX

// -*- C++ -*-
//===------------------------ string_view ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <experimental/__config>

#include <string>
#include <algorithm>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <iomanip>

#include <__debug>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_LFTS

    template<class _CharT, class _Traits = _VSTD::char_traits<_CharT> >
    class _LIBCPP_TYPE_VIS_ONLY basic_string_view {
    public:
        // types
        typedef _Traits                                    traits_type;
        typedef _CharT                                     value_type;
        typedef const _CharT*                              pointer;
        typedef const _CharT*                              const_pointer;
        typedef const _CharT&                              reference;
        typedef const _CharT&                              const_reference;
        typedef const_pointer                              const_iterator; // See [string.view.iterators]
        typedef const_iterator                             iterator;
        typedef _VSTD::reverse_iterator<const_iterator>    const_reverse_iterator;
        typedef const_reverse_iterator                     reverse_iterator;
        typedef size_t                                     size_type;
        typedef ptrdiff_t                                  difference_type;
        static _LIBCPP_CONSTEXPR const size_type npos = -1; // size_type(-1);

        // [string.view.cons], construct/copy
        _LIBCPP_CONSTEXPR _LIBCPP_INLINE_VISIBILITY
        basic_string_view() _NOEXCEPT : __data (nullptr), __size(0) {}

        _LIBCPP_CONSTEXPR _LIBCPP_INLINE_VISIBILITY
        basic_string_view(const basic_string_view&) _NOEXCEPT = default;

        _LIBCPP_INLINE_VISIBILITY
        basic_string_view& operator=(const basic_string_view&) _NOEXCEPT = default;

        template<class _Allocator>
        _LIBCPP_INLINE_VISIBILITY
        basic_string_view(const basic_string<_CharT, _Traits, _Allocator>& __str) _NOEXCEPT
            : __data (__str.data()), __size(__str.size()) {}

        _LIBCPP_CONSTEXPR _LIBCPP_INLINE_VISIBILITY
        basic_string_view(const _CharT* __s, size_type __len)
            : __data(__s), __size(__len)
        {
//             _LIBCPP_ASSERT(__len == 0 || __s != nullptr, "string_view::string_view(_CharT *, size_t): received nullptr");
        }

        _LIBCPP_CONSTEXPR _LIBCPP_INLINE_VISIBILITY
        basic_string_view(const _CharT* __s)
            : __data(__s), __size(_Traits::length(__s)) {}

        // [string.view.iterators], iterators
        _LIBCPP_CONSTEXPR _LIBCPP_INLINE_VISIBILITY
        const_iterator begin()  const _NOEXCEPT { return cbegin(); }

        _LIBCPP_CONSTEXPR _LIBCPP_INLINE_VISIBILITY
        const_iterator end()    const _NOEXCEPT { return cend(); }

        _LIBCPP_CONSTEXPR _LIBCPP_INLINE_VISIBILITY
        const_iterator cbegin() const _NOEXCEPT { return __data; }

        _LIBCPP_CONSTEXPR _LIBCPP_INLINE_VISIBILITY
        const_iterator cend()   const _NOEXCEPT { return __data + __size; }

        _LIBCPP_INLINE_VISIBILITY
        const_reverse_iterator rbegin()   const _NOEXCEPT { return const_reverse_iterator(cend()); }

        _LIBCPP_INLINE_VISIBILITY
        const_reverse_iterator rend()     const _NOEXCEPT { return const_reverse_iterator(cbegin()); }

        _LIBCPP_INLINE_VISIBILITY
        const_reverse_iterator crbegin()  const _NOEXCEPT { return const_reverse_iterator(cend()); }

        _LIBCPP_INLINE_VISIBILITY
        const_reverse_iterator crend()    const _NOEXCEPT { return const_reverse_iterator(cbegin()); }

        // [string.view.capacity], capacity
        _LIBCPP_CONSTEXPR _LIBCPP_INLINE_VISIBILITY
        size_type size()     const _NOEXCEPT { return __size; }

        _LIBCPP_CONSTEXPR _LIBCPP_INLINE_VISIBILITY
        size_type length()   const _NOEXCEPT { return __size; }

        _LIBCPP_CONSTEXPR _LIBCPP_INLINE_VISIBILITY
        size_type max_size() const _NOEXCEPT { return _VSTD::numeric_limits<size_type>::max(); }

        _LIBCPP_CONSTEXPR bool _LIBCPP_INLINE_VISIBILITY
        empty()         const _NOEXCEPT { return __size == 0; }

        // [string.view.access], element access
        _LIBCPP_CONSTEXPR _LIBCPP_INLINE_VISIBILITY
        const_reference operator[](size_type __pos) const { return __data[__pos]; }

        _LIBCPP_CONSTEXPR _LIBCPP_INLINE_VISIBILITY
        const_reference at(size_type __pos) const
        {
            return __pos >= size()
                ? ((throw out_of_range("string_view::at")), __data[0])
                : __data[__pos];
        }

        _LIBCPP_CONSTEXPR _LIBCPP_INLINE_VISIBILITY
        const_reference front() const
        {
            return _LIBCPP_ASSERT(!empty(), "string_view::front(): string is empty"), __data[0];
        }

        _LIBCPP_CONSTEXPR _LIBCPP_INLINE_VISIBILITY
        const_reference back() const
        {
            return _LIBCPP_ASSERT(!empty(), "string_view::back(): string is empty"), __data[__size-1];
        }

        _LIBCPP_CONSTEXPR _LIBCPP_INLINE_VISIBILITY
        const_pointer data() const _NOEXCEPT { return __data; }

        // [string.view.modifiers], modifiers:
        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        void clear() _NOEXCEPT
        {
            __data = nullptr;
            __size = 0;
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        void remove_prefix(size_type __n) _NOEXCEPT
        {
            _LIBCPP_ASSERT(__n <= size(), "remove_prefix() can't remove more than size()");
            __data += __n;
            __size -= __n;
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        void remove_suffix(size_type __n) _NOEXCEPT
        {
            _LIBCPP_ASSERT(__n <= size(), "remove_suffix() can't remove more than size()");
            __size -= __n;
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        void swap(basic_string_view& __other) _NOEXCEPT
        {
            const value_type *__p = __data;
            __data = __other.__data;
            __other.__data = __p;

            size_type __sz = __size;
            __size = __other.__size;
            __other.__size = __sz;
//             _VSTD::swap( __data, __other.__data );
//             _VSTD::swap( __size, __other.__size );
        }

        // [string.view.ops], string operations:
        template<class _Allocator>
        _LIBCPP_INLINE_VISIBILITY
        _LIBCPP_EXPLICIT operator basic_string<_CharT, _Traits, _Allocator>() const
        { return basic_string<_CharT, _Traits, _Allocator>( begin(), end()); }

        template<class _Allocator = allocator<_CharT> >
        _LIBCPP_INLINE_VISIBILITY
        basic_string<_CharT, _Traits, _Allocator>
        to_string( const _Allocator& __a = _Allocator()) const
        { return basic_string<_CharT, _Traits, _Allocator> ( begin(), end(), __a ); }

        size_type copy(_CharT* __s, size_type __n, size_type __pos = 0) const
        {
            if ( __pos > size())
                (throw out_of_range("string_view::copy"));
            size_type __rlen = _VSTD::min( __n, size() - __pos );
            _VSTD::copy_n(begin() + __pos, __rlen, __s );
            return __rlen;
        }

        _LIBCPP_CONSTEXPR
        basic_string_view substr(size_type __pos = 0, size_type __n = npos) const
        {
//             if (__pos > size())
//                 throw out_of_range("string_view::substr");
//             size_type __rlen = _VSTD::min( __n, size() - __pos );
//             return basic_string_view(data() + __pos, __rlen);
            return __pos > size()
                ? (((throw out_of_range("string_view::substr"))), basic_string_view())
                : basic_string_view(data() + __pos, _VSTD::min(__n, size() - __pos));
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 int compare(basic_string_view __sv) const _NOEXCEPT
        {
            size_type __rlen = _VSTD::min( size(), __sv.size());
            int __retval = _Traits::compare(data(), __sv.data(), __rlen);
            if ( __retval == 0 ) // first __rlen chars matched
                __retval = size() == __sv.size() ? 0 : ( size() < __sv.size() ? -1 : 1 );
            return __retval;
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        int compare(size_type __pos1, size_type __n1, basic_string_view __sv) const
        {
            return substr(__pos1, __n1).compare(__sv);
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        int compare(                       size_type __pos1, size_type __n1,
                    basic_string_view _sv, size_type __pos2, size_type __n2) const
        {
            return substr(__pos1, __n1).compare(_sv.substr(__pos2, __n2));
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        int compare(const _CharT* __s) const
        {
            return compare(basic_string_view(__s));
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        int compare(size_type __pos1, size_type __n1, const _CharT* __s) const
        {
            return substr(__pos1, __n1).compare(basic_string_view(__s));
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        int compare(size_type __pos1, size_type __n1, const _CharT* __s, size_type __n2) const
        {
            return substr(__pos1, __n1).compare(basic_string_view(__s, __n2));
        }

        // find
        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type find(basic_string_view __s, size_type __pos = 0) const _NOEXCEPT
        {
            _LIBCPP_ASSERT(__s.size() == 0 || __s.data() != nullptr, "string_view::find(): received nullptr");
            return _VSTD::__str_find<value_type, size_type, traits_type, npos>
                (data(), size(), __s.data(), __pos, __s.size());
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type find(_CharT __c, size_type __pos = 0) const _NOEXCEPT
        {
            return _VSTD::__str_find<value_type, size_type, traits_type, npos>
                (data(), size(), __c, __pos);
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type find(const _CharT* __s, size_type __pos, size_type __n) const
        {
            _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "string_view::find(): received nullptr");
            return _VSTD::__str_find<value_type, size_type, traits_type, npos>
                (data(), size(), __s, __pos, __n);
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type find(const _CharT* __s, size_type __pos = 0) const
        {
            _LIBCPP_ASSERT(__s != nullptr, "string_view::find(): received nullptr");
            return _VSTD::__str_find<value_type, size_type, traits_type, npos>
                (data(), size(), __s, __pos, traits_type::length(__s));
        }

        // rfind
        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type rfind(basic_string_view __s, size_type __pos = npos) const _NOEXCEPT
        {
            _LIBCPP_ASSERT(__s.size() == 0 || __s.data() != nullptr, "string_view::find(): received nullptr");
            return _VSTD::__str_rfind<value_type, size_type, traits_type, npos>
                (data(), size(), __s.data(), __pos, __s.size());
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type rfind(_CharT __c, size_type __pos = npos) const _NOEXCEPT
        {
            return _VSTD::__str_rfind<value_type, size_type, traits_type, npos>
                (data(), size(), __c, __pos);
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type rfind(const _CharT* __s, size_type __pos, size_type __n) const
        {
            _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "string_view::rfind(): received nullptr");
            return _VSTD::__str_rfind<value_type, size_type, traits_type, npos>
                (data(), size(), __s, __pos, __n);
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type rfind(const _CharT* __s, size_type __pos=npos) const
        {
            _LIBCPP_ASSERT(__s != nullptr, "string_view::rfind(): received nullptr");
            return _VSTD::__str_rfind<value_type, size_type, traits_type, npos>
                (data(), size(), __s, __pos, traits_type::length(__s));
        }

        // find_first_of
        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type find_first_of(basic_string_view __s, size_type __pos = 0) const _NOEXCEPT
        {
            _LIBCPP_ASSERT(__s.size() == 0 || __s.data() != nullptr, "string_view::find_first_of(): received nullptr");
            return _VSTD::__str_find_first_of<value_type, size_type, traits_type, npos>
                (data(), size(), __s.data(), __pos, __s.size());
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type find_first_of(_CharT __c, size_type __pos = 0) const _NOEXCEPT
        { return find(__c, __pos); }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type find_first_of(const _CharT* __s, size_type __pos, size_type __n) const
        {
            _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "string_view::find_first_of(): received nullptr");
            return _VSTD::__str_find_first_of<value_type, size_type, traits_type, npos>
                (data(), size(), __s, __pos, __n);
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type find_first_of(const _CharT* __s, size_type __pos=0) const
        {
            _LIBCPP_ASSERT(__s != nullptr, "string_view::find_first_of(): received nullptr");
            return _VSTD::__str_find_first_of<value_type, size_type, traits_type, npos>
                (data(), size(), __s, __pos, traits_type::length(__s));
        }

        // find_last_of
        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type find_last_of(basic_string_view __s, size_type __pos=npos) const _NOEXCEPT
        {
            _LIBCPP_ASSERT(__s.size() == 0 || __s.data() != nullptr, "string_view::find_last_of(): received nullptr");
            return _VSTD::__str_find_last_of<value_type, size_type, traits_type, npos>
                (data(), size(), __s.data(), __pos, __s.size());
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type find_last_of(_CharT __c, size_type __pos = npos) const _NOEXCEPT
        { return rfind(__c, __pos); }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type find_last_of(const _CharT* __s, size_type __pos, size_type __n) const
        {
            _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "string_view::find_last_of(): received nullptr");
            return _VSTD::__str_find_last_of<value_type, size_type, traits_type, npos>
                (data(), size(), __s, __pos, __n);
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type find_last_of(const _CharT* __s, size_type __pos=npos) const
        {
            _LIBCPP_ASSERT(__s != nullptr, "string_view::find_last_of(): received nullptr");
            return _VSTD::__str_find_last_of<value_type, size_type, traits_type, npos>
                (data(), size(), __s, __pos, traits_type::length(__s));
        }

        // find_first_not_of
        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type find_first_not_of(basic_string_view __s, size_type __pos=0) const _NOEXCEPT
        {
            _LIBCPP_ASSERT(__s.size() == 0 || __s.data() != nullptr, "string_view::find_first_not_of(): received nullptr");
            return _VSTD::__str_find_first_not_of<value_type, size_type, traits_type, npos>
                (data(), size(), __s.data(), __pos, __s.size());
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type find_first_not_of(_CharT __c, size_type __pos=0) const _NOEXCEPT
        {
            return _VSTD::__str_find_first_not_of<value_type, size_type, traits_type, npos>
                (data(), size(), __c, __pos);
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type find_first_not_of(const _CharT* __s, size_type __pos, size_type __n) const
        {
            _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "string_view::find_first_not_of(): received nullptr");
            return _VSTD::__str_find_first_not_of<value_type, size_type, traits_type, npos>
                (data(), size(), __s, __pos, __n);
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type find_first_not_of(const _CharT* __s, size_type __pos=0) const
        {
            _LIBCPP_ASSERT(__s != nullptr, "string_view::find_first_not_of(): received nullptr");
            return _VSTD::__str_find_first_not_of<value_type, size_type, traits_type, npos>
                (data(), size(), __s, __pos, traits_type::length(__s));
        }

        // find_last_not_of
        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type find_last_not_of(basic_string_view __s, size_type __pos=npos) const _NOEXCEPT
        {
            _LIBCPP_ASSERT(__s.size() == 0 || __s.data() != nullptr, "string_view::find_last_not_of(): received nullptr");
            return _VSTD::__str_find_last_not_of<value_type, size_type, traits_type, npos>
                (data(), size(), __s.data(), __pos, __s.size());
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type find_last_not_of(_CharT __c, size_type __pos=npos) const _NOEXCEPT
        {
            return _VSTD::__str_find_last_not_of<value_type, size_type, traits_type, npos>
                (data(), size(), __c, __pos);
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type find_last_not_of(const _CharT* __s, size_type __pos, size_type __n) const
        {
            _LIBCPP_ASSERT(__n == 0 || __s != nullptr, "string_view::find_last_not_of(): received nullptr");
            return _VSTD::__str_find_last_not_of<value_type, size_type, traits_type, npos>
                (data(), size(), __s, __pos, __n);
        }

        _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
        size_type find_last_not_of(const _CharT* __s, size_type __pos=npos) const
        {
            _LIBCPP_ASSERT(__s != nullptr, "string_view::find_last_not_of(): received nullptr");
            return _VSTD::__str_find_last_not_of<value_type, size_type, traits_type, npos>
                (data(), size(), __s, __pos, traits_type::length(__s));
        }

    private:
        const   value_type* __data;
        size_type           __size;
    };


    // [string.view.comparison]
    // operator ==
    template<class _CharT, class _Traits>
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    bool operator==(basic_string_view<_CharT, _Traits> __lhs,
                    basic_string_view<_CharT, _Traits> __rhs) _NOEXCEPT
    {
        if ( __lhs.size() != __rhs.size()) return false;
        return __lhs.compare(__rhs) == 0;
    }

    template<class _CharT, class _Traits>
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    bool operator==(basic_string_view<_CharT, _Traits> __lhs,
                    typename _VSTD::common_type<basic_string_view<_CharT, _Traits> >::type __rhs) _NOEXCEPT
    {
        if ( __lhs.size() != __rhs.size()) return false;
        return __lhs.compare(__rhs) == 0;
    }

    template<class _CharT, class _Traits>
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    bool operator==(typename _VSTD::common_type<basic_string_view<_CharT, _Traits> >::type __lhs,
                    basic_string_view<_CharT, _Traits> __rhs) _NOEXCEPT
    {
        if ( __lhs.size() != __rhs.size()) return false;
        return __lhs.compare(__rhs) == 0;
    }


    // operator !=
    template<class _CharT, class _Traits>
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    bool operator!=(basic_string_view<_CharT, _Traits> __lhs, basic_string_view<_CharT, _Traits> __rhs) _NOEXCEPT
    {
        if ( __lhs.size() != __rhs.size())
            return true;
        return __lhs.compare(__rhs) != 0;
    }

    template<class _CharT, class _Traits>
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    bool operator!=(basic_string_view<_CharT, _Traits> __lhs,
                    typename _VSTD::common_type<basic_string_view<_CharT, _Traits> >::type __rhs) _NOEXCEPT
    {
        if ( __lhs.size() != __rhs.size())
            return true;
        return __lhs.compare(__rhs) != 0;
    }

    template<class _CharT, class _Traits>
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    bool operator!=(typename _VSTD::common_type<basic_string_view<_CharT, _Traits> >::type __lhs,
                    basic_string_view<_CharT, _Traits> __rhs) _NOEXCEPT
    {
        if ( __lhs.size() != __rhs.size())
            return true;
        return __lhs.compare(__rhs) != 0;
    }


    // operator <
    template<class _CharT, class _Traits>
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    bool operator<(basic_string_view<_CharT, _Traits> __lhs, basic_string_view<_CharT, _Traits> __rhs) _NOEXCEPT
    {
        return __lhs.compare(__rhs) < 0;
    }

    template<class _CharT, class _Traits>
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    bool operator<(basic_string_view<_CharT, _Traits> __lhs,
                    typename _VSTD::common_type<basic_string_view<_CharT, _Traits> >::type __rhs) _NOEXCEPT
    {
        return __lhs.compare(__rhs) < 0;
    }

    template<class _CharT, class _Traits>
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    bool operator<(typename _VSTD::common_type<basic_string_view<_CharT, _Traits> >::type __lhs,
                    basic_string_view<_CharT, _Traits> __rhs) _NOEXCEPT
    {
        return __lhs.compare(__rhs) < 0;
    }


    // operator >
    template<class _CharT, class _Traits>
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    bool operator> (basic_string_view<_CharT, _Traits> __lhs, basic_string_view<_CharT, _Traits> __rhs) _NOEXCEPT
    {
        return __lhs.compare(__rhs) > 0;
    }

    template<class _CharT, class _Traits>
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    bool operator>(basic_string_view<_CharT, _Traits> __lhs,
                    typename _VSTD::common_type<basic_string_view<_CharT, _Traits> >::type __rhs) _NOEXCEPT
    {
        return __lhs.compare(__rhs) > 0;
    }

    template<class _CharT, class _Traits>
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    bool operator>(typename _VSTD::common_type<basic_string_view<_CharT, _Traits> >::type __lhs,
                    basic_string_view<_CharT, _Traits> __rhs) _NOEXCEPT
    {
        return __lhs.compare(__rhs) > 0;
    }


    // operator <=
    template<class _CharT, class _Traits>
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    bool operator<=(basic_string_view<_CharT, _Traits> __lhs, basic_string_view<_CharT, _Traits> __rhs) _NOEXCEPT
    {
        return __lhs.compare(__rhs) <= 0;
    }

    template<class _CharT, class _Traits>
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    bool operator<=(basic_string_view<_CharT, _Traits> __lhs,
                    typename _VSTD::common_type<basic_string_view<_CharT, _Traits> >::type __rhs) _NOEXCEPT
    {
        return __lhs.compare(__rhs) <= 0;
    }

    template<class _CharT, class _Traits>
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    bool operator<=(typename _VSTD::common_type<basic_string_view<_CharT, _Traits> >::type __lhs,
                    basic_string_view<_CharT, _Traits> __rhs) _NOEXCEPT
    {
        return __lhs.compare(__rhs) <= 0;
    }


    // operator >=
    template<class _CharT, class _Traits>
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    bool operator>=(basic_string_view<_CharT, _Traits> __lhs, basic_string_view<_CharT, _Traits> __rhs) _NOEXCEPT
    {
        return __lhs.compare(__rhs) >= 0;
    }


    template<class _CharT, class _Traits>
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    bool operator>=(basic_string_view<_CharT, _Traits> __lhs,
                    typename _VSTD::common_type<basic_string_view<_CharT, _Traits> >::type __rhs) _NOEXCEPT
    {
        return __lhs.compare(__rhs) >= 0;
    }

    template<class _CharT, class _Traits>
    _LIBCPP_CONSTEXPR_AFTER_CXX11 _LIBCPP_INLINE_VISIBILITY
    bool operator>=(typename _VSTD::common_type<basic_string_view<_CharT, _Traits> >::type __lhs,
                    basic_string_view<_CharT, _Traits> __rhs) _NOEXCEPT
    {
        return __lhs.compare(__rhs) >= 0;
    }


    // // [string.view.io]
    // template<class _CharT, class _Traits>
    // basic_ostream<_CharT, _Traits>&
    // operator<<(basic_ostream<_CharT, _Traits>& __os, basic_string_view<_CharT, _Traits> __sv)
    // {
    //     return _VSTD::__put_character_sequence(__os, __sv.data(), __sv.size());
    // }

  typedef basic_string_view<char>     string_view;
  typedef basic_string_view<char16_t> u16string_view;
  typedef basic_string_view<char32_t> u32string_view;
  typedef basic_string_view<wchar_t>  wstring_view;

_LIBCPP_END_NAMESPACE_LFTS
_LIBCPP_BEGIN_NAMESPACE_STD

// [string.view.hash]
// Shamelessly stolen from <string>
template<class _CharT, class _Traits>
struct _LIBCPP_TYPE_VIS_ONLY hash<std::experimental::basic_string_view<_CharT, _Traits> >
    : public unary_function<std::experimental::basic_string_view<_CharT, _Traits>, size_t>
{
    size_t operator()(const std::experimental::basic_string_view<_CharT, _Traits>& __val) const _NOEXCEPT;
};

template<class _CharT, class _Traits>
size_t
hash<std::experimental::basic_string_view<_CharT, _Traits> >::operator()(
        const std::experimental::basic_string_view<_CharT, _Traits>& __val) const _NOEXCEPT
{
    return __do_string_hash(__val.data(), __val.data() + __val.size());
}

#if _LIBCPP_STD_VER > 11
template <class _CharT, class _Traits>
__quoted_output_proxy<_CharT, const _CharT *, _Traits>
quoted ( std::experimental::basic_string_view <_CharT, _Traits> __sv,
             _CharT __delim = _CharT('"'), _CharT __escape=_CharT('\\'))
{
    return __quoted_output_proxy<_CharT, const _CharT *, _Traits>
         ( __sv.data(), __sv.data() + __sv.size(), __delim, __escape );
}
#endif

_LIBCPP_END_NAMESPACE_STD

namespace std
{
  using string_view = std::experimental::string_view;
}
#endif

#endif /* SGX_STRING_VIEW_H_ */
