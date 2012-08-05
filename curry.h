#ifndef _MEGA_CURRY_H_
#define _MEGA_CURRY_H_

#include <functional>

namespace mega
{

// unpack operator() for functors
template <typename Functor>
struct function_traits
    : public function_traits<decltype(&Functor::operator())>
{};
 
template <typename R, typename T0>
struct function_traits<R(*)(T0)>
{ typedef R  result_type; typedef T0 argument_type; static const int arity = 1; };
 
template <typename C, typename R, typename T0>
struct function_traits<R(C::*)(T0)>
{ typedef R  result_type; typedef T0 argument_type; static const int arity = 1; };
 
template <typename C, typename R, typename T0>
struct function_traits<R(C::*)(T0) const>
{ typedef R  result_type; typedef T0 argument_type; static const int arity = 1; };

template <typename R, typename T0, typename T1>
struct function_traits<R(*)(T0,T1)>
{ typedef R  result_type; typedef T0 first_argument_type; typedef T1 second_argument_type; static const int arity = 2; };
 
template <typename C, typename R, typename T0, typename T1>
struct function_traits<R(C::*)(T0,T1)>
{ typedef R  result_type; typedef T0 first_argument_type; typedef T1 second_argument_type; static const int arity = 2; };
 
template <typename C, typename R, typename T0, typename T1>
struct function_traits<R(C::*)(T0,T1) const>
{ typedef R  result_type; typedef T0 first_argument_type; typedef T1 second_argument_type; static const int arity = 2; };
 
template <typename R, typename T0, typename T1, typename T2>
struct function_traits<R(*)(T0,T1,T2)>
{ typedef R  result_type; typedef T0 first_argument_type; typedef T1 second_argument_type; typedef T2 third_argument_type; static const int arity = 3; };

template <typename C, typename R, typename T0, typename T1, typename T2>
struct function_traits<R(C::*)(T0,T1,T2)>
{ typedef R  result_type; typedef T0 first_argument_type; typedef T1 second_argument_type; typedef T2 third_argument_type; static const int arity = 3; };
 
template <typename C, typename R, typename T0, typename T1, typename T2>
struct function_traits<R(C::*)(T0,T1,T2) const>
{ typedef R  result_type; typedef T0 first_argument_type; typedef T1 second_argument_type; typedef T2 third_argument_type; static const int arity = 3; };
 
template<typename Functor, int NArgs>
struct count_args
    : std::enable_if<function_traits<Functor>::arity == NArgs>
{ static_assert(NArgs >= 0, "Negative number? WTF?"); };

template <typename R, typename T0, typename T1>
std::function<std::function<R(T1)>(T0)> curry_(std::function<R(T0,T1)> f)
{
    return [=] (T0 const & t0) -> std::function<R(T1)>
    {
        return [=] (T1 const & t1) { return f(t0, t1); };
    };
}

template <typename R, typename T0, typename T1, typename T2>
std::function<std::function<std::function<R(T2)>(T1)>(T0)> curry_(std::function<R(T0,T1,T2)> f)
{
    return [=] (T0 const & t0) -> std::function<std::function<R(T2)>(T1)>
    {
        return curry_<R,T1,T2>([=] (T1 const & t1, T2 const & t2)
        {
            return f(t0,t1,t2);
        });
    };
}

template <typename Functor>
Functor curry(Functor f, typename count_args<Functor, 1>::type * = 0)
{
    return f;
}

template <typename Functor>
std::function<
    std::function<
        typename function_traits<Functor>::result_type(typename function_traits<Functor>::second_argument_type)
    >(typename function_traits<Functor>::first_argument_type)
>
    curry(Functor f, typename count_args<Functor, 2>::type * = 0)
{
    return curry_
            < typename function_traits<Functor>::result_type
            , typename function_traits<Functor>::first_argument_type
            , typename function_traits<Functor>::second_argument_type
            > (f);
}

template <typename Functor>
std::function<
    std::function<
        std::function<
            typename function_traits<Functor>::result_type(typename function_traits<Functor>::third_argument_type)
        >(typename function_traits<Functor>::second_argument_type)
    >(typename function_traits<Functor>::first_argument_type)
>
    curry(Functor f, typename count_args<Functor, 3>::type * = 0)
{
    return curry_
            < typename function_traits<Functor>::result_type
            , typename function_traits<Functor>::first_argument_type
            , typename function_traits<Functor>::second_argument_type
            , typename function_traits<Functor>::third_argument_type
            > (f);
}

}   // namespace mega

#endif // _MEGA_CURRY_H_
