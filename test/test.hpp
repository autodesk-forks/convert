// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_TEST_HPP
#define BOOST_CONVERT_TEST_HPP

#include <boost/make_default.hpp>
#include <boost/static_assert.hpp>
#include <string>
#include <istream>
#include <memory.h> // Is needed for 'memset'
#include <boost/convert/detail/forward.hpp>
#include "./forward.hpp"

//[change_declaration
struct change
{
    typedef change this_type;

    enum value_type { no, up, dn };

    change(value_type v =no) : value_(v) {}

    friend std::istream& operator>>(std::istream& stream, this_type& chg)
    {
        std::string str; stream >> str;

        /**/ if (str == "up") chg.value_ = up;
        else if (str == "dn") chg.value_ = dn;
        else if (str == "no") chg.value_ = no;
        else stream.setstate(std::ios_base::failbit);

        return stream;
    }
    friend std::ostream& operator<<(std::ostream& stream, this_type const& chg)
    {
        return stream << (chg.value_ == up ? "up" : chg.value_ == dn ? "dn" : "no");
    }

    value_type value() const { return value_; }

    private: value_type value_;
};
//]
//[direction_declaration
struct direction
{
    // Note: the class does NOT have the default constructor.

    enum value_type { up, dn };

    direction(value_type value) : value_(value) {}
    bool operator==(direction const& that) const { return value_ == that.value_; }

    friend std::istream& operator>>(std::istream& stream, direction& dir)
    {
        std::string str; stream >> str;

        /**/ if (str == "up") dir.value_ = up;
        else if (str == "dn") dir.value_ = dn;
        else stream.setstate(std::ios_base::failbit);

        return stream;
    }
    friend std::ostream& operator<<(std::ostream& stream, direction const& dir)
    {
        return stream << (dir.value_ == up ? "up" : "dn");
    }

    private: value_type value_;
};
//]
//[direction_declaration_make_default
namespace boost
{
    template<>
    inline
    direction
    make_default<direction>()
    {
        return direction(direction::up);
    }
}
//]

// Quick and dirty small-string implementation for performance tests to make sure we can have
// and array of my_strings in one piece.
//[my_string_declaration
struct my_string
{
    typedef my_string        this_type;
    typedef char const* const_iterator;

    my_string ();
    my_string (const_iterator, const_iterator);

    char const*    c_str () const { return storage_; }
    const_iterator begin () const { return storage_; }
    const_iterator   end () const { return storage_ + strlen(storage_); }
    this_type& operator= (char const*);

    private:

    static size_t const size_ = 12;
    char storage_[size_];
};
//]
inline
my_string::my_string()
{
    storage_[0] = 0;
}

inline
my_string::my_string(const_iterator beg, const_iterator end)
{
    std::size_t const sz = end - beg;

    BOOST_ASSERT(sz < size_);
    memcpy(storage_, beg, sz); storage_[sz] = 0;
}

inline
my_string&
my_string::operator=(char const* str)
{
    BOOST_ASSERT(strlen(str) < size_);
    strcpy(storage_, str);
    return *this;
}

inline
bool
operator==(char const* s1, my_string const& s2)
{
    return strcmp(s1, s2.c_str()) == 0;
}

namespace test
{
    struct cnv
    {
#if defined(_MSC_VER)
        static bool const    is_msc = true;
        static bool const    is_gcc = false;
#elif defined(__CYGWIN__) 
        static bool const    is_msc = false;
        static bool const    is_gcc = true;
#elif defined(__GNUC__)
        static bool const    is_msc = false;
        static bool const    is_gcc = true;
#else
#error "Add here."
#endif

        static void      is_converter ();
        static void        scratchpad ();
        static void            sfinae ();
        static void        encryption ();
        static void         callables ();
        static void         fallbacks ();
        static void   lcast_converter ();
        static void  stream_converter ();
        static void  printf_converter ();
        static void  strtol_converter ();
        static void  spirit_converter ();
        static void     int_to_string ();
        static void         user_type ();

        template<typename Cnv> static void  str_to_int (Cnv const&);
        template<typename Cnv> static void  int_to_str (Cnv const&);
    };
}

#endif // BOOST_CONVERT_TEST_HPP
