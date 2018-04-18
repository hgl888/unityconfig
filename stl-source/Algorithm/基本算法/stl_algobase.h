#ifndef __SGI_STL_INTERNAL_ALGOBASE_H
#define __SGI_STL_INTERNAL_ALGOBASE_H

#ifndef __STL_CONFIG_H
#include <stl_config.h>
#endif
#ifndef __SGI_STL_INTERNAL_RELOPS
#include <stl_relops.h>//���ͷ�ļ����ṩ����������
#endif
#ifndef __SGI_STL_INTERNAL_PAIR_H
#include <stl_pair.h>
#endif
#ifndef __TYPE_TRAITS_H
#include <type_traits.h>//����ļ��ṩ��ȡ����
#endif

#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stddef.h>
#include <new.h>

#ifdef __STL_USE_NEW_IOSTREAMS 
#include <iosfwd>
#else /* __STL_USE_NEW_IOSTREAMS */
#include <iostream.h>
#endif /* __STL_USE_NEW_IOSTREAMS */

#ifndef __SGI_STL_INTERNAL_ITERATOR_H
#include <stl_iterator_base.h>
#include <stl_iterator.h>
#endif

// We pick up concept_checks.h from stl_iterator_base.h.

__STL_BEGIN_NAMESPACE

// swap and iter_swap
//��������������a��b��ָ������
/*����ӿڣ�
	template <class ForwardIterator1, class ForwardIterator2>
	void iter_swap (ForwardIterator1 a, ForwardIterator2 b);
*/
template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*) {
  _Tp __tmp = *__a;
  *__a = *__b;
  *__b = __tmp;
}

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b) {
  __STL_REQUIRES(_ForwardIter1, _Mutable_ForwardIterator);
  __STL_REQUIRES(_ForwardIter2, _Mutable_ForwardIterator);
  __STL_CONVERTIBLE(typename iterator_traits<_ForwardIter1>::value_type,
                    typename iterator_traits<_ForwardIter2>::value_type);
  __STL_CONVERTIBLE(typename iterator_traits<_ForwardIter2>::value_type,
                    typename iterator_traits<_ForwardIter1>::value_type);
  //��������ļ򵥽�����������ȡ��a���ͱ�
  __iter_swap(__a, __b, __VALUE_TYPE(__a));
}

//����a��b��ֵ
//����������ô���
template <class _Tp>
inline void swap(_Tp& __a, _Tp& __b) {
  __STL_REQUIRES(_Tp, _Assignable);
  _Tp __tmp = __a;
  __a = __b;
  __b = __tmp;
}

//--------------------------------------------------
// min and max

/*
������Сֵ��Returns the smallest of a and b. If both are equivalent, a is returned.
default (1)	��Ĭ���ǲ���operator<���бȽ�
	template <class T> const T& min (const T& a, const T& b);
custom (2)	���û��Լ��ṩ��comp
	template <class T, class Compare>
	const T& min (const T& a, const T& b, Compare comp);
*/
/*
�������ֵ��Returns the largest of a and b. If both are equivalent, a is returned.
default (1)	Ĭ���ǲ���operator<���бȽ�
	template <class T> const T& max (const T& a, const T& b);
custom (2)	�û��Լ��ṩ��comp
	template <class T, class Compare>
	const T& max (const T& a, const T& b, Compare comp);
*/
#if !defined(__BORLANDC__) || __BORLANDC__ >= 0x540 /* C++ Builder 4.0 */

#undef min
#undef max

template <class _Tp>
inline const _Tp& min(const _Tp& __a, const _Tp& __b) {
  __STL_REQUIRES(_Tp, _LessThanComparable);
  return __b < __a ? __b : __a;
}

template <class _Tp>
inline const _Tp& max(const _Tp& __a, const _Tp& __b) {
  __STL_REQUIRES(_Tp, _LessThanComparable);
  return  __a < __b ? __b : __a;
}

#endif /* __BORLANDC__ */

template <class _Tp, class _Compare>
inline const _Tp& min(const _Tp& __a, const _Tp& __b, _Compare __comp) {
  return __comp(__b, __a) ? __b : __a;
}

template <class _Tp, class _Compare>
inline const _Tp& max(const _Tp& __a, const _Tp& __b, _Compare __comp) {
  return __comp(__a, __b) ? __b : __a;
}

//--------------------------------------------------
// copy

// All of these auxiliary functions serve two purposes.  (1) Replace
// calls to copy with memmove whenever possible.  (Memmove, not memcpy,
// because the input and output ranges are permitted to overlap.)
// (2) If we're using random access iterators, then write the loop as
// a for loop with an explicit count.

//������������Ϊ���������������ô˺���
template <class _InputIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_InputIter __first, _InputIter __last,
                          _OutputIter __result,
                          input_iterator_tag, _Distance*)
{
  for ( ; __first != __last; ++__result, ++__first)//ֻ��һ��һ���ı�����������
    *__result = *__first;//һ��һ���ĸ�ֵoperator=
  return __result;
}
//��������������Ϊ������ʵ�����������ô˺���
template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter
__copy(_RandomAccessIter __first, _RandomAccessIter __last,
       _OutputIter __result, random_access_iterator_tag, _Distance*)
{
  for (_Distance __n = __last - __first; __n > 0; --__n) {//������������
    *__result = *__first;//��Ӧ��ֵoperator=
    ++__first;
    ++__result;
  }
  return __result;
}

//ֱ�ӵ���memmove���ƣ�����Ҫ��������
template <class _Tp>
inline _Tp*
__copy_trivial(const _Tp* __first, const _Tp* __last, _Tp* __result) {
//memmove()���ƶ����ƣ�ע�⣺source��destination���ڴ�ռ�����ص�
/*
ԭ�ͣ�void * memmove ( void * destination, const void * source, size_t num );
���ܣ�
Copies the values of num bytes from the location pointed by source to the memory block pointed by destination. 
Copying takes place as if an intermediate buffer were used, allowing the destination and source to overlap.
ע�⣺memmove��һ�����ܾ������Ȼ���������ݸ������������������������������������ʼλ���ص�
������example
	#include <stdio.h>
	#include <string.h>

	int main ()
	{
	  char str[] = "memmove can be very useful......";
	  //�������������ʼλ�������������ص�
	  //����memmove�Ĺ���
	  memmove (str+20,str+15,11);
	  puts (str);
	  return 0;
	}
	Output��
	memmove can be very very useful.
*/
  memmove(__result, __first, sizeof(_Tp) * (__last - __first));
  return __result + (__last - __first);
}

#if defined(__STL_FUNCTION_TMPL_PARTIAL_ORDER)
//�����ṩ��ֵ����assignment_operator������ô˺���
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_aux2(_InputIter __first, _InputIter __last,
                               _OutputIter __result, __false_type) {
//������ȡ��������������
  return __copy(__first, __last, __result,
                __ITERATOR_CATEGORY(__first),
                __DISTANCE_TYPE(__first));
}
//���ṩ��ֵ����assignment_operator������ô˺���
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_aux2(_InputIter __first, _InputIter __last,
                               _OutputIter __result, __true_type) {
	//������ȡ��������������
  return __copy(__first, __last, __result,
                __ITERATOR_CATEGORY(__first),
                __DISTANCE_TYPE(__first));
}
#ifndef __USLC__

//ƫ�ػ�������ΪT*
template <class _Tp>
inline _Tp* __copy_aux2(_Tp* __first, _Tp* __last, _Tp* __result,
                        __true_type) {
  return __copy_trivial(__first, __last, __result);
}

#endif /* __USLC__ */
//ƫ�ػ�������Ϊconst T*
//���ṩ��ֵ����assignment_operator������ô˺���
template <class _Tp>
inline _Tp* __copy_aux2(const _Tp* __first, const _Tp* __last, _Tp* __result,
                        __true_type) {
  return __copy_trivial(__first, __last, __result);//ת�����øú���
}

//���øú���
template <class _InputIter, class _OutputIter, class _Tp>
inline _OutputIter __copy_aux(_InputIter __first, _InputIter __last,
                              _OutputIter __result, _Tp*) {
  typedef typename __type_traits<_Tp>::has_trivial_assignment_operator
          _Trivial;
  //�����ж��Ƿ��ṩ��ֵ����assignment_operator
  return __copy_aux2(__first, __last, __result, _Trivial());
}

/*
���ƺ�����
	template <class InputIterator, class OutputIterator>
	OutputIterator copy (InputIterator first, InputIterator last, OutputIterator result);
���ܣ�Copies the elements in the range [first,last) into the range beginning at result.
����������[first,last)�����ݸ��Ƶ�[result,result+(last-first))��

ע�⣺result����λ��[first,last)�ڲ���������˵���ʼλ�ò���λ�����������ڲ���
����������������β�������������ص�������copy_backward�෴

���������䲻�ص���copy()��copy_backward()������ʹ��
*/

//��ȫ�����棬����ӿ�
template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last,
                        _OutputIter __result) {
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  //������ȡ��first�����ͣ�Ȼ������ض����͵��ö�Ӧ�ĺ���__copy_aux()
  return __copy_aux(__first, __last, __result, __VALUE_TYPE(__first));
}


// Hack for compilers that don't have partial ordering of function templates
// but do have partial specialization of class templates.
#elif defined(__STL_CLASS_PARTIAL_SPECIALIZATION)

//��ȫ�����汾
template <class _InputIter, class _OutputIter, class _BoolType>
struct __copy_dispatch {
  static _OutputIter copy(_InputIter __first, _InputIter __last,
                          _OutputIter __result) {
    typedef typename iterator_traits<_InputIter>::iterator_category _Category;
    typedef typename iterator_traits<_InputIter>::difference_type _Distance;
    return __copy(__first, __last, __result, _Category(), (_Distance*) 0);
  }
};

//ƫ�ػ�1��������������T*
template <class _Tp>
struct __copy_dispatch<_Tp*, _Tp*, __true_type>
{
  static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
    return __copy_trivial(__first, __last, __result);
  }
};

//ƫ�ػ�2����һ��������const T*���ڶ���������T*
template <class _Tp>
struct __copy_dispatch<const _Tp*, _Tp*, __true_type>
{
  static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
    return __copy_trivial(__first, __last, __result);
  }
};

//����ӿڣ���ȫ����
template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last,
                        _OutputIter __result) {
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  typedef typename iterator_traits<_InputIter>::value_type _Tp;
  typedef typename __type_traits<_Tp>::has_trivial_assignment_operator
          _Trivial;
  return __copy_dispatch<_InputIter, _OutputIter, _Trivial>
    ::copy(__first, __last, __result);
}

// Fallback for compilers with neither partial ordering nor partial
// specialization.  Define the faster version for the basic builtin
// types.
#else /* __STL_CLASS_PARTIAL_SPECIALIZATION */

template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last,
                        _OutputIter __result)
{
  return __copy(__first, __last, __result,
                __ITERATOR_CATEGORY(__first),
                __DISTANCE_TYPE(__first));
}

#define __SGI_STL_DECLARE_COPY_TRIVIAL(_Tp)                                \
  inline _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) { \
    memmove(__result, __first, sizeof(_Tp) * (__last - __first));          \
    return __result + (__last - __first);                                  \
  }

__SGI_STL_DECLARE_COPY_TRIVIAL(char)
__SGI_STL_DECLARE_COPY_TRIVIAL(signed char)
__SGI_STL_DECLARE_COPY_TRIVIAL(unsigned char)
__SGI_STL_DECLARE_COPY_TRIVIAL(short)
__SGI_STL_DECLARE_COPY_TRIVIAL(unsigned short)
__SGI_STL_DECLARE_COPY_TRIVIAL(int)
__SGI_STL_DECLARE_COPY_TRIVIAL(unsigned int)
__SGI_STL_DECLARE_COPY_TRIVIAL(long)
__SGI_STL_DECLARE_COPY_TRIVIAL(unsigned long)
#ifdef __STL_HAS_WCHAR_T
__SGI_STL_DECLARE_COPY_TRIVIAL(wchar_t)
#endif
#ifdef _STL_LONG_LONG
__SGI_STL_DECLARE_COPY_TRIVIAL(long long)
__SGI_STL_DECLARE_COPY_TRIVIAL(unsigned long long)
#endif 
__SGI_STL_DECLARE_COPY_TRIVIAL(float)
__SGI_STL_DECLARE_COPY_TRIVIAL(double)
__SGI_STL_DECLARE_COPY_TRIVIAL(long double)

#undef __SGI_STL_DECLARE_COPY_TRIVIAL
#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

//--------------------------------------------------
// copy_backward
/*ԭ��
	template <class BidirectionalIterator1, class BidirectionalIterator2>
	BidirectionalIterator2 copy_backward (BidirectionalIterator1 first,
                                        BidirectionalIterator1 last,
                                        BidirectionalIterator2 result);
*/

//������������˫��������ĵ���
template <class _BidirectionalIter1, class _BidirectionalIter2, 
          class _Distance>
inline _BidirectionalIter2 __copy_backward(_BidirectionalIter1 __first, 
                                           _BidirectionalIter1 __last, 
                                           _BidirectionalIter2 __result,
                                           bidirectional_iterator_tag,
                                           _Distance*)
{
  while (__first != __last)//������������
    *--__result = *--__last;//����ֵoperator=
  return __result;
}
//������������������ʵ������ĵ���
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, 
                                          _RandomAccessIter __last, 
                                          _BidirectionalIter __result,
                                          random_access_iterator_tag,
                                          _Distance*)
{
  for (_Distance __n = __last - __first; __n > 0; --__n)//������������
    *--__result = *--__last;//����ֵoperator=
  return __result;
}

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION 

// This dispatch class is a workaround for compilers that do not 
// have partial ordering of function templates.  All we're doing is
// creating a specialization so that we can turn a call to copy_backward
// into a memmove whenever possible.
//��ȫ������
template <class _BidirectionalIter1, class _BidirectionalIter2,
          class _BoolType>
struct __copy_backward_dispatch
{
  typedef typename iterator_traits<_BidirectionalIter1>::iterator_category 
          _Cat;
  typedef typename iterator_traits<_BidirectionalIter1>::difference_type
          _Distance;

  static _BidirectionalIter2 copy(_BidirectionalIter1 __first, 
                                  _BidirectionalIter1 __last, 
                                  _BidirectionalIter2 __result) {
    return __copy_backward(__first, __last, __result, _Cat(), (_Distance*) 0);
  }
};

//ƫ�ػ�������Ϊԭ��ָ��T*
template <class _Tp>
struct __copy_backward_dispatch<_Tp*, _Tp*, __true_type>
{
  static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
    const ptrdiff_t _Num = __last - __first;
    memmove(__result - _Num, __first, sizeof(_Tp) * _Num);
    return __result - _Num;
  }
};

//ƫ�ػ�����һ������Ϊconst T*���ڶ�������ΪT*
template <class _Tp>
struct __copy_backward_dispatch<const _Tp*, _Tp*, __true_type>
{
  static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
    return  __copy_backward_dispatch<_Tp*, _Tp*, __true_type>
      ::copy(__first, __last, __result);
  }
};

//����ӿ�
template <class _BI1, class _BI2>
inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result) {
  __STL_REQUIRES(_BI1, _BidirectionalIterator);
  __STL_REQUIRES(_BI2, _Mutable_BidirectionalIterator);
  __STL_CONVERTIBLE(typename iterator_traits<_BI1>::value_type,
                    typename iterator_traits<_BI2>::value_type);
  typedef typename __type_traits<typename iterator_traits<_BI2>::value_type>
                        ::has_trivial_assignment_operator
          _Trivial;
  return __copy_backward_dispatch<_BI1, _BI2, _Trivial>
              ::copy(__first, __last, __result);
}

#else /* __STL_CLASS_PARTIAL_SPECIALIZATION */

template <class _BI1, class _BI2>
inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result) {
  return __copy_backward(__first, __last, __result,
                         __ITERATOR_CATEGORY(__first),
                         __DISTANCE_TYPE(__first));
}

#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

//--------------------------------------------------
// copy_n (not part of the C++ standard)
/*ԭ��
	template <class InputIterator, class Size, class OutputIterator>
	OutputIterator copy_n (InputIterator first, Size n, OutputIterator result);

���ӣ�
	#include <iostream>     // std::cout
	#include <algorithm>    // std::copy
	#include <vector>       // std::vector

	int main () {
	  int myints[]={10,20,30,40,50,60,70};
	  std::vector<int> myvector;

	  myvector.resize(7);   // allocate space for 7 elements

	  std::copy_n ( myints, 7, myvector.begin() );

	  std::cout << "myvector contains:";
	  for (std::vector<int>::iterator it = myvector.begin(); it!=myvector.end(); ++it)
		std::cout << ' ' << *it;

	  std::cout << '\n';

	  return 0;
	}
	Output��
	myvector contains: 10 20 30 40 50 60 70
*/
//C++11�Ѿ�����������׼��

//����������Ϊ���������
//��ָ��λ�ÿ�ʼ����__count����*firstֵ��ȵ�ֵ
template <class _InputIter, class _Size, class _OutputIter>
pair<_InputIter, _OutputIter> __copy_n(_InputIter __first, _Size __count,
                                       _OutputIter __result,
                                       input_iterator_tag) {
  for ( ; __count > 0; --__count) {//��������С��0������в���
    *__result = *__first;//��ֵoperator=
    ++__first;
    ++__result;
  }
  return pair<_InputIter, _OutputIter>(__first, __result);
}

//������Ϊ������ʵ�����
template <class _RAIter, class _Size, class _OutputIter>
inline pair<_RAIter, _OutputIter>
__copy_n(_RAIter __first, _Size __count,
         _OutputIter __result,
         random_access_iterator_tag) {
  _RAIter __last = __first + __count;
  return pair<_RAIter, _OutputIter>(__last, copy(__first, __last, __result));
}

//��ȡ������������
template <class _InputIter, class _Size, class _OutputIter>
inline pair<_InputIter, _OutputIter>
__copy_n(_InputIter __first, _Size __count, _OutputIter __result) {
  return __copy_n(__first, __count, __result,
                  __ITERATOR_CATEGORY(__first));
}

//����ӿڣ���ȫ����
template <class _InputIter, class _Size, class _OutputIter>
inline pair<_InputIter, _OutputIter>
copy_n(_InputIter __first, _Size __count, _OutputIter __result) {
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  return __copy_n(__first, __count, __result);
}

//--------------------------------------------------
// fill and fill_n

/*
���ֵ��Assigns val to all the elements in the range [first,last).
	template <class ForwardIterator, class T>
	void fill (ForwardIterator first, ForwardIterator last, const T& val);
*/
//������[first,last)��ֵ�����Ϊvalue
template <class _ForwardIter, class _Tp>
void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __value) {
  __STL_REQUIRES(_ForwardIter, _Mutable_ForwardIterator);
  for ( ; __first != __last; ++__first)//��������
    *__first = __value;//��ǰλ�ø���ֵvalue
}
/*
��ָ��λ���������n��ֵ��Assigns val to the first n elements of the sequence pointed by first.
	template <class OutputIterator, class Size, class T>
	void fill_n (OutputIterator first, Size n, const T& val);
*/
//��first��ʼ�������n��valueֵ
template <class _OutputIter, class _Size, class _Tp>
_OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value) {
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  for ( ; __n > 0; --__n, ++__first)//��first��ʼ����n��λ��
    *__first = __value;//��ǰλ�ø���ֵvalue
  return __first;
}

// Specialization: for one-byte types we can use memset.

inline void fill(unsigned char* __first, unsigned char* __last,
                 const unsigned char& __c) {
  unsigned char __tmp = __c;
  //���ֽ����
  //Sets the first num bytes of the block of memory pointed by ptr to the specified value (interpreted as an unsigned char).
  memset(__first, __tmp, __last - __first);
}

inline void fill(signed char* __first, signed char* __last,
                 const signed char& __c) {
  signed char __tmp = __c;
  memset(__first, static_cast<unsigned char>(__tmp), __last - __first);
}

inline void fill(char* __first, char* __last, const char& __c) {
  char __tmp = __c;
  memset(__first, static_cast<unsigned char>(__tmp), __last - __first);
}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

template <class _Size>
inline unsigned char* fill_n(unsigned char* __first, _Size __n,
                             const unsigned char& __c) {
  fill(__first, __first + __n, __c);
  return __first + __n;
}

template <class _Size>
inline signed char* fill_n(char* __first, _Size __n,
                           const signed char& __c) {
  fill(__first, __first + __n, __c);
  return __first + __n;
}

template <class _Size>
inline char* fill_n(char* __first, _Size __n, const char& __c) {
  fill(__first, __first + __n, __c);
  return __first + __n;
}

#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

//--------------------------------------------------
// equal and mismatch

/*
�ж���������ĵ�һ����ƥ��㣬����һ����������������ɵ�pair��
���е�һ��������ָ���һ������Ĳ�ƥ��㣬�ڶ���������ָ��ڶ�������Ĳ�ƥ���
�����ƥ�䣬���ص���ָ�����������last��������

quality (1)	������Ĭ��operator==�Ƚ�
	template <class InputIterator1, class InputIterator2>
	pair<InputIterator1, InputIterator2>
    mismatch (InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2);
predicate (2)��ָ���ȽϹ���pred
	template <class InputIterator1, class InputIterator2, class BinaryPredicate>
	pair<InputIterator1, InputIterator2>
    mismatch (InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, BinaryPredicate pred);
*/
//�汾һ������Ĭ�ϱȽϹ���operator==
template <class _InputIter1, class _InputIter2>
pair<_InputIter1, _InputIter2> mismatch(_InputIter1 __first1,
                                        _InputIter1 __last1,
                                        _InputIter2 __first2) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(typename iterator_traits<_InputIter1>::value_type,
                 _EqualityComparable);
  __STL_REQUIRES(typename iterator_traits<_InputIter2>::value_type,
                 _EqualityComparable);
  //�������䣬Ѱ�Ҳ�ƥ���
  //ע�⣺��һ�������Ԫ�ز��ܱȵڶ�������Ԫ�ض�
  while (__first1 != __last1 && *__first1 == *__first2) {
    ++__first1;
    ++__first2;
  }
  return pair<_InputIter1, _InputIter2>(__first1, __first2);
}
//�汾�����Զ���ȽϹ���
template <class _InputIter1, class _InputIter2, class _BinaryPredicate>
pair<_InputIter1, _InputIter2> mismatch(_InputIter1 __first1,
                                        _InputIter1 __last1,
                                        _InputIter2 __first2,
                                        _BinaryPredicate __binary_pred) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  //�������䣬Ѱ�Ҳ�ƥ���
  while (__first1 != __last1 && __binary_pred(*__first1, *__first2)) {
    ++__first1;
    ++__first2;
  }
  return pair<_InputIter1, _InputIter2>(__first1, __first2);
}
/*
Compares the elements in the range [first1,last1) with those in the range beginning at first2, 
and returns true if all of the elements in both ranges match.
�Ƚ���������ָ����Χ��С���������������[first,last)������ȣ��򷵻�true
		����ڶ������е�Ԫ�رȽ϶࣬�������Ԫ�ز��迼��;
equality (1):Ĭ�ϲ���operator==�Ƚ�
	template <class InputIterator1, class InputIterator2>
	bool equal (InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2);
predicate (2):�û���ָ���ȽϹ���
	template <class InputIterator1, class InputIterator2, class BinaryPredicate>
	bool equal (InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, BinaryPredicate pred);
*/
//�汾һ��������Ĭ�ϱȽϹ���operator==
template <class _InputIter1, class _InputIter2>
inline bool equal(_InputIter1 __first1, _InputIter1 __last1,
                  _InputIter2 __first2) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(typename iterator_traits<_InputIter1>::value_type,
                 _EqualityComparable);
  __STL_REQUIRES(typename iterator_traits<_InputIter2>::value_type,
                 _EqualityComparable);
  //��������
  for ( ; __first1 != __last1; ++__first1, ++__first2)
    if (*__first1 != *__first2)
      return false;//����ӦԪ�ز���ȣ�����FALSE
  return true;//����ȫ�����
}
//�汾���������û��Զ���ıȽϹ���
template <class _InputIter1, class _InputIter2, class _BinaryPredicate>
inline bool equal(_InputIter1 __first1, _InputIter1 __last1,
                  _InputIter2 __first2, _BinaryPredicate __binary_pred) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  //��������
  for ( ; __first1 != __last1; ++__first1, ++__first2)
    if (!__binary_pred(*__first1, *__first2))
      return false;//����ӦԪ�ز����Ϲ��򣬷���FALSE
  return true;//����ȫ������
}

//--------------------------------------------------
// lexicographical_compare and lexicographical_compare_3way.
// (the latter is not part of the C++ standard.)

/*���ܣ�Returns true if the range [first1,last1) compares lexicographically less than the range [first2,last2).
default (1)	
	template <class InputIterator1, class InputIterator2>
	bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2);
custom (2)	
	template <class InputIterator1, class InputIterator2, class Compare>
	bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
                                Compare comp);
*/
/*
����������[first1,last1)��[first2,last2)���бȽϣ��Ƚϲ�������������ж�Ӧλ���ϵ�Ԫ�ؽ��У�
����������
��1��ĳһ���ӦԪ�ز����
��2��ͬʱ�ﵽlast1��last2(���������д�С���)
��3���ﵽlast1��last2(�������д�С�����)
*/

//�汾һ��Ĭ�ϱȽϲ���Ϊless
template <class _InputIter1, class _InputIter2>
bool lexicographical_compare(_InputIter1 __first1, _InputIter1 __last1,
                             _InputIter2 __first2, _InputIter2 __last2) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(typename iterator_traits<_InputIter1>::value_type,
                 _LessThanComparable);
  __STL_REQUIRES(typename iterator_traits<_InputIter2>::value_type,
                 _LessThanComparable);
  //�����κ�һ�����е���β�ˣ�����������������о���ӦԪ�ؽ��бȽ�
  for ( ; __first1 != __last1 && __first2 != __last2
        ; ++__first1, ++__first2) {
    if (*__first1 < *__first2)//����һ���ж�ӦԪ��С�ڵڶ����ж�ӦԪ��
      return true;//����TRUE
    if (*__first2 < *__first1)//���ڶ����ж�ӦԪ��С�ڵ�һ���ж�ӦԪ��
      return false;//����FALSE
	//�������ж�ӦԪ����ȣ������������һ���ӦԪ�رȽ�
  }
  //�����һ�����ѵ���β�ˣ����ڶ����л�����Ԫ�أ����һ����С�ڵڶ�����
  return __first1 == __last1 && __first2 != __last2;
}

//�汾�����û�������ָ���ȽϹ���
template <class _InputIter1, class _InputIter2, class _Compare>
bool lexicographical_compare(_InputIter1 __first1, _InputIter1 __last1,
                             _InputIter2 __first2, _InputIter2 __last2,
                             _Compare __comp) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  //�����κ�һ�����е���β�ˣ�����������������о���ӦԪ�ؽ��бȽ�
  for ( ; __first1 != __last1 && __first2 != __last2
        ; ++__first1, ++__first2) {
    if (__comp(*__first1, *__first2))//����һ���ж�ӦԪ�ط��Ϲ����ڵڶ����ж�ӦԪ��
      return true;//����TRUE
    if (__comp(*__first2, *__first1))//���ڶ����ж�ӦԪ�ط��Ϲ����ڵ�һ���ж�ӦԪ��
      return false;//����FALSE
	//�������ж�ӦԪ����ȣ������������һ���ӦԪ�رȽ�
  }
  //�����һ�����ѵ���β�ˣ����ڶ����л�����Ԫ�أ����һ���з��Ϲ����ڵڶ�����
  return __first1 == __last1 && __first2 != __last2;
}

//�������const unsigned cahr*���ػ��汾
inline bool 
lexicographical_compare(const unsigned char* __first1,
                        const unsigned char* __last1,
                        const unsigned char* __first2,
                        const unsigned char* __last2)
{
  const size_t __len1 = __last1 - __first1;//��һ���г���
  const size_t __len2 = __last2 - __first2;//�ڶ����г���
  //�ȱȽϳ�����ͬ��һС��
  /*
  memcmp������������
  ԭ�ͣ�int memcmp ( const void * ptr1, const void * ptr2, size_t num );

Compares the first num bytes of the block of memory pointed by ptr1 to the first num bytes pointed by ptr2, 
returning zero if they all match or a value different from zero representing which is greater if they do not.
  */
  const int __result = memcmp(__first1, __first2, min(__len1, __len2));
  //���ݷ��ؽ��result��ֵ��0�ȽϽ����ж�
  //result<0����һ����С�ڵڶ�����
  //result>0����һ���д��ڵڶ�����
  //result=0�������������
  return __result != 0 ? __result < 0 : __len1 < __len2;
}

//���const char*���ػ��汾
inline bool lexicographical_compare(const char* __first1, const char* __last1,
                                    const char* __first2, const char* __last2)
{
#if CHAR_MAX == SCHAR_MAX
  return lexicographical_compare((const signed char*) __first1,
                                 (const signed char*) __last1,
                                 (const signed char*) __first2,
                                 (const signed char*) __last2);
#else /* CHAR_MAX == SCHAR_MAX */
  return lexicographical_compare((const unsigned char*) __first1,
                                 (const unsigned char*) __last1,
                                 (const unsigned char*) __first2,
                                 (const unsigned char*) __last2);
#endif /* CHAR_MAX == SCHAR_MAX */
}

//������strcmp()�ķ����汾
//Ĭ�ϱȽϲ�����less
template <class _InputIter1, class _InputIter2>
int __lexicographical_compare_3way(_InputIter1 __first1, _InputIter1 __last1,
                                   _InputIter2 __first2, _InputIter2 __last2)
{
//�����κ�һ�����е���β�ˣ�����������������о���ӦԪ�ؽ��бȽ�
  while (__first1 != __last1 && __first2 != __last2) {
    if (*__first1 < *__first2)//����һ��Ԫ��С�ڵڶ����ж�Ӧ��Ԫ��
      return -1;//�򷵻ظ�ֵ-1
    if (*__first2 < *__first1)//���ڶ�����Ԫ��С�ڵ�һ���ж�ӦԪ��
      return 1;//�򷵻���ֵ1
	//��������
	//�������ж�ӦԪ����ȣ������������һ���ӦԪ�رȽ�
    ++__first1;
    ++__first2;	
  }
  //���ڶ������ѵ���β��
  if (__first2 == __last2) {
	  //����һ����Ҳͬʱ����β�ˣ���ʾ��������ȣ��򷵻�0��
	  //����һ����û����β�ˣ����ʾ��һ���д��ڵڶ�����
	  //�򷵻���ֵ1
    return !(__first1 == __last1);
  }
  else {//����һ�����ѵ���β�ˣ����ڶ����л�û����β��
	  //�򷵻ظ�ֵ-1
    return -1;
  }
}

//���������const unsigned char*
inline int
__lexicographical_compare_3way(const unsigned char* __first1,
                               const unsigned char* __last1,
                               const unsigned char* __first2,
                               const unsigned char* __last2)
{
  const ptrdiff_t __len1 = __last1 - __first1;
  const ptrdiff_t __len2 = __last2 - __first2;
  const int __result = memcmp(__first1, __first2, min(__len1, __len2));
  return __result != 0 ? __result 
                       : (__len1 == __len2 ? 0 : (__len1 < __len2 ? -1 : 1));
}
//����const char*�ػ���
inline int 
__lexicographical_compare_3way(const char* __first1, const char* __last1,
                               const char* __first2, const char* __last2)
{
#if CHAR_MAX == SCHAR_MAX
  return __lexicographical_compare_3way(
                                (const signed char*) __first1,
                                (const signed char*) __last1,
                                (const signed char*) __first2,
                                (const signed char*) __last2);
#else
  return __lexicographical_compare_3way((const unsigned char*) __first1,
                                        (const unsigned char*) __last1,
                                        (const unsigned char*) __first2,
                                        (const unsigned char*) __last2);
#endif
}
//������溯���ķ�װ
template <class _InputIter1, class _InputIter2>
int lexicographical_compare_3way(_InputIter1 __first1, _InputIter1 __last1,
                                 _InputIter2 __first2, _InputIter2 __last2)
{
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(typename iterator_traits<_InputIter1>::value_type,
                 _LessThanComparable);
  __STL_REQUIRES(typename iterator_traits<_InputIter2>::value_type,
                 _LessThanComparable);
  return __lexicographical_compare_3way(__first1, __last1, __first2, __last2);
}

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_ALGOBASE_H */

// Local Variables:
// mode:C++
// End:
