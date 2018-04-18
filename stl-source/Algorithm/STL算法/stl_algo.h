#ifndef __SGI_STL_INTERNAL_ALGO_H
#define __SGI_STL_INTERNAL_ALGO_H

#include <stl_heap.h>//��������ѵ�����㷨
//���ѵ�����㷨�Ѿ����ܹ��ˣ�����Ȥ���
//http://blog.csdn.net/chenhanzhun/article/details/39434491

// See concept_checks.h for the concept-checking macros 
// __STL_REQUIRES, __STL_CONVERTIBLE, etc.


__STL_BEGIN_NAMESPACE

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1209
#endif

// __median (an extension, not present in the C++ standard).
//�������ܣ�ȡ����Ԫ��a��b��c���м�ֵ
//�汾һ����Ĭ�ϵ�operator<����
template <class _Tp>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c) {
  __STL_REQUIRES(_Tp, _LessThanComparable);
  if (__a < __b)
    if (__b < __c)
      return __b;//��a<b<c���򷵻�b
    else if (__a < __c)
      return __c;//��a<c<b���򷵻�c
    else
      return __a;//��c<a<b���򷵻�a
  else if (__a < __c)
    return __a;//��b<a<c���򷵻�a
  else if (__b < __c)
    return __c;//��b<c<a���򷵻�c
  else
    return __b;//���򷵻�b
}
//�汾�����÷º���comp�Ƚ�
template <class _Tp, class _Compare>
inline const _Tp&
__median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp) {
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool, _Tp, _Tp);
  if (__comp(__a, __b))
    if (__comp(__b, __c))
      return __b;
    else if (__comp(__a, __c))
      return __c;
    else
      return __a;
  else if (__comp(__a, __c))
    return __a;
  else if (__comp(__b, __c))
    return __c;
  else
    return __b;
}

// for_each.  Apply a function to every element of a range.
//���ܣ�Applies function fn to each of the elements in the range [first,last).
//���º���fӦ����[first,last)�����ڵ�ÿһ��Ԫ����
//ע�����ܸı�[first,last)��Ԫ��ֵ
template <class _InputIter, class _Function>
_Function for_each(_InputIter __first, _InputIter __last, _Function __f) {
  __STL_REQUIRES(_InputIter, _InputIterator);
  for ( ; __first != __last; ++__first)
    __f(*__first);//���÷º���f
  return __f;
}
//for_each����������
/*
	#include <iostream>     // std::cout
	#include <algorithm>    // std::for_each
	#include <vector>       // std::vector

	void myfunction (int i) {  // function:
	  std::cout << ' ' << i;
	}

	struct myclass {           // function object type:
	  void operator() (int i) {std::cout << ' ' << i;}
	} myobject;

	int main () {
	  std::vector<int> myvector;
	  myvector.push_back(10);
	  myvector.push_back(20);
	  myvector.push_back(30);

	  std::cout << "myvector contains:";
	  for_each (myvector.begin(), myvector.end(), myfunction);
	  std::cout << '\n';

	  // or:
	  std::cout << "myvector contains:";
	  for_each (myvector.begin(), myvector.end(), myobject);
	  std::cout << '\n';

	  return 0;
	}
	Output:
	myvector contains: 10 20 30
	myvector contains: 10 20 30
*/

// find and find_if.
//��������[first,last)��Ԫ�ص�һ����valueֵ��ȵ�Ԫ�أ���������λ��
//����find�����ǲ���Ĭ�ϵ�equality����operator==
//find_if�ǲ����û�����ָ���Ĳ���pred

//��find������ȡ�����ĵ���������Ϊ���������input_iterator_tag������ô˺���
template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last,
                       const _Tp& __val,
                       input_iterator_tag)
{//����δ���������β�ˣ���δ�ҵ�ƥ���ֵ�����������
  while (__first != __last && !(*__first == __val))
    ++__first;
  //���ҵ�ƥ���ֵ���򷵻ظ�λ��
  //���Ҳ���������������β�ˣ���ʱfirst=last���򷵻�first
  return __first;
}
//��find_if������ȡ�����ĵ���������Ϊ���������input_iterator_tag������ô˺���
template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last,
                          _Predicate __pred,
                          input_iterator_tag)
{//����δ���������β�ˣ���δ�ҵ�ƥ���ֵ�����������
  while (__first != __last && !__pred(*__first))
    ++__first;
  //���ҵ�ƥ���ֵ���򷵻ظ�λ��
  //���Ҳ���������������β�ˣ���ʱfirst=last���򷵻�first
  return __first;
}

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
//��find������ȡ�����ĵ���������Ϊ������ʵ�����random_access_iterator_tag������ô˺���
template <class _RandomAccessIter, class _Tp>
_RandomAccessIter find(_RandomAccessIter __first, _RandomAccessIter __last,
                       const _Tp& __val,
                       random_access_iterator_tag)
{
  typename iterator_traits<_RandomAccessIter>::difference_type __trip_count
    = (__last - __first) >> 2;

  for ( ; __trip_count > 0 ; --__trip_count) {
    if (*__first == __val) return __first;
    ++__first;

    if (*__first == __val) return __first;
    ++__first;

    if (*__first == __val) return __first;
    ++__first;

    if (*__first == __val) return __first;
    ++__first;
  }

  switch(__last - __first) {
  case 3:
    if (*__first == __val) return __first;
    ++__first;
  case 2:
    if (*__first == __val) return __first;
    ++__first;
  case 1:
    if (*__first == __val) return __first;
    ++__first;
  case 0:
  default:
    return __last;
  }
}
//��find_if������ȡ�����ĵ���������Ϊ������ʵ�����random_access_iterator_tag������ô˺���
template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter find_if(_RandomAccessIter __first, _RandomAccessIter __last,
                          _Predicate __pred,
                          random_access_iterator_tag)
{
  typename iterator_traits<_RandomAccessIter>::difference_type __trip_count
    = (__last - __first) >> 2;

  for ( ; __trip_count > 0 ; --__trip_count) {
    if (__pred(*__first)) return __first;
    ++__first;

    if (__pred(*__first)) return __first;
    ++__first;

    if (__pred(*__first)) return __first;
    ++__first;

    if (__pred(*__first)) return __first;
    ++__first;
  }

  switch(__last - __first) {
  case 3:
    if (__pred(*__first)) return __first;
    ++__first;
  case 2:
    if (__pred(*__first)) return __first;
    ++__first;
  case 1:
    if (__pred(*__first)) return __first;
    ++__first;
  case 0:
  default:
    return __last;
  }
}

#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */
/*find�������ܣ�Returns an iterator to the first element in the range [first,last) that compares equal to val. 
If no such element is found, the function returns last.
find����ԭ�ͣ�
	template <class InputIterator, class T>
	InputIterator find (InputIterator first, InputIterator last, const T& val);
*/
//find��������ӿ�
template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last,
                       const _Tp& __val)
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool, 
            typename iterator_traits<_InputIter>::value_type, _Tp);
  //������ȡ��first�����������ͣ����ݵ����������͵��ò�ͬ�ĺ���
  return find(__first, __last, __val, __ITERATOR_CATEGORY(__first));
}
/*find_if�������ܣ�Returns an iterator to the first element in the range [first,last) for which pred returns true. 
If no such element is found, the function returns last.
find_if����ԭ�ͣ�
	template <class InputIterator, class UnaryPredicate>
	InputIterator find_if (InputIterator first, InputIterator last, UnaryPredicate pred);
*/
//find_if ��������ӿ�
template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last,
                          _Predicate __pred) {
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_UNARY_FUNCTION_CHECK(_Predicate, bool,
          typename iterator_traits<_InputIter>::value_type);
  //������ȡ��first�����������ͣ����ݵ����������͵��ò�ͬ�ĺ���
  return find_if(__first, __last, __pred, __ITERATOR_CATEGORY(__first));
}
//find��find_if����������
/*
	#include <iostream>     // std::cout
	#include <algorithm>    // std::find_if
	#include <vector>       // std::vector

	bool IsOdd (int i) {
	  return ((i%2)==1);
	}

	int main () {
	  std::vector<int> myvector;

	  myvector.push_back(10);
	  myvector.push_back(25);
	  myvector.push_back(40);
	  myvector.push_back(55);

	  std::vector<int>::iterator it = std::find_if (myvector.begin(), myvector.end(), IsOdd);
	  std::cout << "The first odd value is " << *it << '\n';

	  // using std::find with vector and iterator:
	  it = find (myvector.begin(), myvector.end(), 40);
	  if (it != myvector.end())
		std::cout << "Element found in myvector: " << *it << '\n';
	  else
		std::cout << "Element not found in myints\n";

	  return 0;
	}
	Output:
	The first odd value is 25
	Element found in myvector: 40
 
*/

// adjacent_find.

//��������[first,last)�ڵ�һ���ظ�������Ԫ��
//�����ڷ�������Ԫ�صĵ�һ��Ԫ��λ��
//�������ڷ���lastλ��
/*�ú����������汾����һ�汾��Ĭ�ϲ���operator==���ڶ��汾���û�ָ���Ķ�Ԫ����pred
��������ӿڵ�ԭ�ͣ�
equality (1)��Ĭ�ϲ�����operator==
	template <class ForwardIterator>
	ForwardIterator adjacent_find (ForwardIterator first, ForwardIterator last);
predicate (2)���û�ָ���Ķ�Ԫ����pred	
	template <class ForwardIterator, class BinaryPredicate>
	ForwardIterator adjacent_find (ForwardIterator first, ForwardIterator last,
                                  BinaryPredicate pred);

*/
//�汾һ��Ĭ�ϲ�����operator==
template <class _ForwardIter>
_ForwardIter adjacent_find(_ForwardIter __first, _ForwardIter __last) {
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES(typename iterator_traits<_ForwardIter>::value_type,
                 _EqualityComparable);
  /*
  ���1������������Ϊ�գ���ֱ�ӷ���β��last��
  ���2�����������䲻Ϊ�գ��Ҵ��������ظ�Ԫ�أ��򷵻�����Ԫ�صĵ�һ��Ԫ�ص�λ�ã�
  ���3�����������䲻Ϊ�գ����ǲ����������ظ�Ԫ�أ���ֱ�ӷ���β��last��
  */
  //���1��
  if (__first == __last)//����������Ϊ��
    return __last;//ֱ�ӷ���last
  //���2��
  _ForwardIter __next = __first;//���嵱ǰλ�õ���һ��λ��(����ǰԪ�ص�����Ԫ��)
  while(++__next != __last) {//����û����β�ˣ�ִ��whileѭ��
    if (*__first == *__next)//����Ԫ��ֵ��ȣ����ҵ������ظ�Ԫ��
      return __first;//���ص�һ��Ԫ�ص�λ��
    __first = __next;//����ʱ�Ҳ�����������ң�ֱ����������β��
  }
  //���3��
  return __last;//ֱ�ӷ���β��last
}

//�汾�����û�ָ���Ķ�Ԫ����pred	
//ʵ�ֹ��̺Ͱ汾һһ����ֻ���жϹ���ͬ
template <class _ForwardIter, class _BinaryPredicate>
_ForwardIter adjacent_find(_ForwardIter __first, _ForwardIter __last,
                           _BinaryPredicate __binary_pred) {
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_BINARY_FUNCTION_CHECK(_BinaryPredicate, bool,
          typename iterator_traits<_ForwardIter>::value_type,
          typename iterator_traits<_ForwardIter>::value_type);
  if (__first == __last)
    return __last;
  _ForwardIter __next = __first;
  while(++__next != __last) {
	  //����ҵ�����Ԫ�ط����û�ָ���������ͷ��ص�һԪ��λ��
    if (__binary_pred(*__first, *__next))
      return __first;
    __first = __next;
  }
  return __last;
}
//adjacent_find����������
/*
	#include <iostream>     // std::cout
	#include <algorithm>    // std::adjacent_find
	#include <vector>       // std::vector

	bool myfunction (int i, int j) {
	  return (i==j);
	}

	int main () {
	  int myints[] = {5,20,5,30,30,20,10,10,20};
	  std::vector<int> myvector (myints,myints+8);
	  std::vector<int>::iterator it;

	  // using default comparison:
	  it = std::adjacent_find (myvector.begin(), myvector.end());

	  if (it!=myvector.end())
		std::cout << "the first pair of repeated elements are: " << *it << '\n';

	  //using predicate comparison:
	  it = std::adjacent_find (++it, myvector.end(), myfunction);

	  if (it!=myvector.end())
		std::cout << "the second pair of repeated elements are: " << *it << '\n';

	  return 0;
	}
	Output:
	the first pair of repeated elements are: 30
	the second pair of repeated elements are: 10

*/

// count and count_if.  There are two version of each, one whose return type
// type is void and one (present only if we have partial specialization)
// whose return type is iterator_traits<_InputIter>::difference_type.  The
// C++ standard only has the latter version, but the former, which was present
// in the HP STL, is retained for backward compatibility.

//����ָ��Ԫ�صĸ���
//SGI STL���ṩ�����汾������C++��׼ֻ�ṩһ�ְ汾
/*���ܣ�Returns the number of elements in the range [first,last) that compare equal to val.
C++��׼ֻ�ṩһ��countԭ�ͣ�
	template <class InputIterator, class T>
		typename iterator_traits<InputIterator>::difference_type
	count (InputIterator first, InputIterator last, const T& val);
*/
//SGI STL�ṩ�İ汾һcount������C++��׼��Ĭ��ʹ��operator==
template <class _InputIter, class _Tp, class _Size>
void count(_InputIter __first, _InputIter __last, const _Tp& __value,
           _Size& __n) {
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(typename iterator_traits<_InputIter>::value_type,
                 _EqualityComparable);
  __STL_REQUIRES(_Tp, _EqualityComparable);
  //������[first,last)�ڵ�Ԫ�غ�ָ��ֵvalue�Ƚ�
  //��û��������β�ˣ���������
  for ( ; __first != __last; ++__first)
    if (*__first == __value)//��������ȵ�ֵ
      ++__n;//�������ۼ�1
}

/*���ܣ�Returns the number of elements in the range [first,last) for which pred is true.
C++��׼ֻ�ṩһ��count_ifԭ�ͣ�
	template <class InputIterator, class Predicate>
		typename iterator_traits<InputIterator>::difference_type
    count_if (InputIterator first, InputIterator last, UnaryPredicate pred);
*/
//SGI STL�ṩ�İ汾һcount_if������C++��׼��Ĭ��ʹ��operator==
template <class _InputIter, class _Predicate, class _Size>
void count_if(_InputIter __first, _InputIter __last, _Predicate __pred,
              _Size& __n) {
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_UNARY_FUNCTION_CHECK(_Predicate, bool, 
                  typename iterator_traits<_InputIter>::value_type);
  //������[first,last)�ڵ�Ԫ�غ�ָ��ֵvalue�Ƚ�
  //��û��������β�ˣ���������
  for ( ; __first != __last; ++__first)
    if (__pred(*__first))//���ڷ��Ϲ����Ԫ��
      ++__n;//�������ۼ�1
}

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
//SGI STL�ṩ�İ汾��count��ҲC++��׼�ṩ�İ汾
template <class _InputIter, class _Tp>
typename iterator_traits<_InputIter>::difference_type
count(_InputIter __first, _InputIter __last, const _Tp& __value) {
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(typename iterator_traits<_InputIter>::value_type,
                 _EqualityComparable);
  __STL_REQUIRES(_Tp, _EqualityComparable);
  typename iterator_traits<_InputIter>::difference_type __n = 0;
  //������[first,last)�ڵ�Ԫ�غ�ָ��ֵvalue�Ƚ�
  //��û��������β�ˣ���������
  for ( ; __first != __last; ++__first)
    if (*__first == __value)//������ȵ�Ԫ��
      ++__n;//�������ۼ�1
  return __n;
}

//SGI STL�ṩ�İ汾��count_if��ҲC++��׼�ṩ�İ汾
template <class _InputIter, class _Predicate>
typename iterator_traits<_InputIter>::difference_type
count_if(_InputIter __first, _InputIter __last, _Predicate __pred) {
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_UNARY_FUNCTION_CHECK(_Predicate, bool, 
                  typename iterator_traits<_InputIter>::value_type);
  typename iterator_traits<_InputIter>::difference_type __n = 0;
  //������[first,last)�ڵ�Ԫ�غ�ָ��ֵvalue�Ƚ�
  //��û��������β�ˣ���������
  for ( ; __first != __last; ++__first)
    if (__pred(*__first))//���ڷ��Ϲ����Ԫ��
      ++__n;//�������ۼ�1
  return __n;
}

//�������count��count_if����������
/*
	#include <iostream>     // std::cout
	#include <algorithm>    // std::count
	#include <vector>       // std::vector

	bool IsOdd (int i) { return ((i%2)==1); }

	int main () {
	  // counting elements in array:
	  int myints[] = {10,20,31,30,21,10,11,20};   // 8 elements
	  int mycount = std::count (myints, myints+8, 10);
	  std::cout << "10 appears " << mycount << " times.\n";

	  // counting elements in container:
	  std::vector<int> myvector (myints, myints+8);
	  mycount = std::count (myvector.begin(), myvector.end(), 20);
	  std::cout << "20 appears " << mycount  << " times.\n";
  
	  mycount = count_if (myvector.begin(), myvector.end(), IsOdd);
	  std::cout << "myvector contains " << mycount  << " odd values.\n";

	  return 0;
	}
	Output:
	10 appears 2 times.
	20 appears 2 times.
	myvector contains 3 odd values.
*/
#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

// search.
//������һ[first1,last1)�����ǵ������У��������ж�[first2,last2)���״γ��ֵ�
//�ò��Һ����������汾��
//�汾һ��ʹ��Ĭ�ϵ�equality����operator==
//�汾�����û�������Ҫ����ָ����������
/*search�������ܣ�Searches the range [first1,last1) for the first occurrence of the sequence defined by [first2,last2), 
and returns an iterator to its first element, or last1 if no occurrences are found.

search������ԭ�ͣ�
equality (1)���汾һ
	template <class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1 search (ForwardIterator1 first1, ForwardIterator1 last1,
                            ForwardIterator2 first2, ForwardIterator2 last2);
predicate (2)���汾��	
	template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
	ForwardIterator1 search (ForwardIterator1 first1, ForwardIterator1 last1,
                            ForwardIterator2 first2, ForwardIterator2 last2,
                            BinaryPredicate pred);
*/
//�汾һ��ʹ��Ĭ�ϵ�equality����operator==
template <class _ForwardIter1, class _ForwardIter2>
_ForwardIter1 search(_ForwardIter1 __first1, _ForwardIter1 __last1,
                     _ForwardIter2 __first2, _ForwardIter2 __last2) 
{
  __STL_REQUIRES(_ForwardIter1, _ForwardIterator);
  __STL_REQUIRES(_ForwardIter2, _ForwardIterator);
  __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool,
   typename iterator_traits<_ForwardIter1>::value_type,
   typename iterator_traits<_ForwardIter2>::value_type);

  // Test for empty ranges
  if (__first1 == __last1 || __first2 == __last2)
    return __first1;

  // Test for a pattern of length 1.
  _ForwardIter2 __tmp(__first2);
  ++__tmp;
  if (__tmp == __last2)
    return find(__first1, __last1, *__first2);

  // General case.

  _ForwardIter2 __p1, __p;

  __p1 = __first2; ++__p1;

  _ForwardIter1 __current = __first1;

  while (__first1 != __last1) {//����û��������β��
    __first1 = find(__first1, __last1, *__first2);//����*first2������[first1,last1)�״γ��ֵ�λ��
    if (__first1 == __last1)//����[first1,last1)�в�����*first2������[first1,last1)������������[first2,last2)
      return __last1;//��ֱ�ӷ�������β��

    __p = __p1;
    __current = __first1; 
    if (++__current == __last1)//��[first1,last1)ֻ��һ��Ԫ�أ�������[first1,last1)С������[first2,last2)
      return __last1;//�����ܳ�Ϊ�������У�����last1

    while (*__current == *__p) {//�������������Ӧ��ֵ��ͬ
      if (++__p == __last2)//������[first2,last2)ֻ������Ԫ�أ���������һƥ��
        return __first1;//�򷵻�ƥ����״�λ��
      if (++__current == __last1)//����һ������С�ڵڶ�������
        return __last1;//����last1
    }

    ++__first1;
  }
  return __first1;
}

//�汾�����û�������Ҫ����ָ����������
template <class _ForwardIter1, class _ForwardIter2, class _BinaryPred>
_ForwardIter1 search(_ForwardIter1 __first1, _ForwardIter1 __last1,
                     _ForwardIter2 __first2, _ForwardIter2 __last2,
                     _BinaryPred  __predicate) 
{
  __STL_REQUIRES(_ForwardIter1, _ForwardIterator);
  __STL_REQUIRES(_ForwardIter2, _ForwardIterator);
  __STL_BINARY_FUNCTION_CHECK(_BinaryPred, bool,
   typename iterator_traits<_ForwardIter1>::value_type,
   typename iterator_traits<_ForwardIter2>::value_type);

  // Test for empty ranges
  if (__first1 == __last1 || __first2 == __last2)
    return __first1;

  // Test for a pattern of length 1.
  _ForwardIter2 __tmp(__first2);
  ++__tmp;
  if (__tmp == __last2) {
    while (__first1 != __last1 && !__predicate(*__first1, *__first2))
      ++__first1;
    return __first1;    
  }

  // General case.

  _ForwardIter2 __p1, __p;

  __p1 = __first2; ++__p1;

  _ForwardIter1 __current = __first1;

  while (__first1 != __last1) {
    while (__first1 != __last1) {
      if (__predicate(*__first1, *__first2))
        break;
      ++__first1;
    }
    while (__first1 != __last1 && !__predicate(*__first1, *__first2))
      ++__first1;
    if (__first1 == __last1)
      return __last1;

    __p = __p1;
    __current = __first1; 
    if (++__current == __last1) return __last1;

    while (__predicate(*__current, *__p)) {
      if (++__p == __last2)
        return __first1;
      if (++__current == __last1)
        return __last1;
    }

    ++__first1;
  }
  return __first1;
}

// search_n.  Search for __count consecutive copies of __val.
//������[first,last)��������count����������ֵvalueԪ�ص�λ��
//�ò��Һ����������汾��
//�汾һ��ʹ��Ĭ�ϵ�equality����operator==
//�汾�����û�������Ҫ����ָ����������
/*search_n�������ܣ�Searches the range [first,last) for a sequence of count elements, 
each comparing equal to val (or for which pred returns true).


search_n������ԭ�ͣ�
equality (1)���汾һ	
	template <class ForwardIterator, class Size, class T>
	ForwardIterator search_n (ForwardIterator first, ForwardIterator last,
                             Size count, const T& val);
predicate (2)���汾��	
	template <class ForwardIterator, class Size, class T, class BinaryPredicate>
	ForwardIterator search_n ( ForwardIterator first, ForwardIterator last,
                              Size count, const T& val, BinaryPredicate pred );
*/
//�汾һ��ʹ��Ĭ�ϵ�equality����operator==
template <class _ForwardIter, class _Integer, class _Tp>
_ForwardIter search_n(_ForwardIter __first, _ForwardIter __last,
                      _Integer __count, const _Tp& __val) {
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES(typename iterator_traits<_ForwardIter>::value_type,
                 _EqualityComparable);
  __STL_REQUIRES(_Tp, _EqualityComparable);

  if (__count <= 0)
    return __first;
  else {//���Ȳ���value��һ�γ��ֵ�λ��
    __first = find(__first, __last, __val);
    while (__first != __last) {//�����ֵ�λ�ò�������β��
      _Integer __n = __count - 1;//���¸���������ֻ�����n=count-1��������ͬvalue����
      _ForwardIter __i = __first;
      ++__i;//�ӵ�ǰλ�õ���һ��λ�ÿ�ʼ����
	  //��û�е�������β�ˣ��Ҹ���n����0��������Ԫ����valueֵ���
      while (__i != __last && __n != 0 && *__i == __val) {
        ++__i;//��������
        --__n;//���ٲ��ҵĴ�������Ϊ�Ѿ��ҵ�value�ٴγ���
      }
      if (__n == 0)//��������δ����β�ˣ�����count��value�Ѿ����ҵ�
        return __first;//��������ҵ����״γ���value��λ��
      else
        __first = find(__i, __last, __val);//����δ�ҵ�����count��valueֵ��λ�ã����ҳ�value�´γ��ֵ�λ�ã���׼����һ��whileѭ��
    }
    return __last;
  }
}
//�汾�����û�������Ҫ����ָ����������
template <class _ForwardIter, class _Integer, class _Tp, class _BinaryPred>
_ForwardIter search_n(_ForwardIter __first, _ForwardIter __last,
                      _Integer __count, const _Tp& __val,
                      _BinaryPred __binary_pred) {
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_BINARY_FUNCTION_CHECK(_BinaryPred, bool, 
             typename iterator_traits<_ForwardIter>::value_type, _Tp);
  if (__count <= 0)
    return __first;
  else {
    while (__first != __last) {
      if (__binary_pred(*__first, __val))
        break;
      ++__first;
    }
    while (__first != __last) {
      _Integer __n = __count - 1;
      _ForwardIter __i = __first;
      ++__i;
      while (__i != __last && __n != 0 && __binary_pred(*__i, __val)) {
        ++__i;
        --__n;
      }
      if (__n == 0)
        return __first;
      else {
        while (__i != __last) {
          if (__binary_pred(*__i, __val))
            break;
          ++__i;
        }
        __first = __i;
      }
    }
    return __last;
  }
} 
//search��search_n����������
/*
	#include <iostream>     // std::cout
	#include <algorithm>    // std::search_n
	#include <vector>       // std::vector

	bool mypredicate (int i, int j) {
	  return (i==j);
	}

	int main () {
	  int myints[]={10,20,30,30,20,10,10,20};
	  std::vector<int> myvector (myints,myints+8);
	  std::vector<int>::iterator it;

	  // using default comparison:
	  it = std::search_n (myvector.begin(), myvector.end(), 2, 30);
	  if (it!=myvector.end())
		std::cout << "two 30s found at position " << (it-myvector.begin()) << '\n';
	  else
		std::cout << "match not found\n";

	  // using predicate comparison:
	  it = std::search_n (myvector.begin(), myvector.end(), 2, 10, mypredicate);
	  if (it!=myvector.end())
		std::cout << "two 10s found at position " << int(it-myvector.begin()) << '\n';
	  else
		std::cout << "match not found\n";
    
	  int needle1[] = {10,20};
  
	  // using default comparison:
	  it = std::search (myvector.begin(), myvector.end(), needle1, needle1+2);  
	   if (it!=myvector.end())
		std::cout << "needle1 found at position " << (it-myvector.begin()) << '\n';
	  else
		std::cout << "needle1 not found\n";
    
	  // using predicate comparison:
	  int needle2[] = {30,20,10};
	  it = std::search (myvector.begin(), myvector.end(), needle2, needle2+3, mypredicate);
	  if (it!=myvector.end())
		std::cout << "needle2 found at position " << (it-myvector.begin()) << '\n';
	  else
		std::cout << "needle2 not found\n";

	  return 0;
	}
	Output:
	two 30s found at position 2
	two 10s found at position 5
	needle1 found at position 0
	needle2 found at position 3
*/

// swap_ranges
//������[first1,last1)�ڵ�Ԫ���롰��first2��ʼ��������ͬ����Ԫ���໥����
//���������п�λ��ͬһ��������ͬ����
//����ڶ�����С�ڵ�һ���г��ȣ�������������ͬһ�������ص�������δ��Ԥ��
//Exchanges the values of each of the elements in the range [first1,last1) 
//with those of their respective elements in the range beginning at first2.
template <class _ForwardIter1, class _ForwardIter2>
_ForwardIter2 swap_ranges(_ForwardIter1 __first1, _ForwardIter1 __last1,
                          _ForwardIter2 __first2) {
  __STL_REQUIRES(_ForwardIter1, _Mutable_ForwardIterator);
  __STL_REQUIRES(_ForwardIter2, _Mutable_ForwardIterator);
  __STL_CONVERTIBLE(typename iterator_traits<_ForwardIter1>::value_type,
                    typename iterator_traits<_ForwardIter2>::value_type);
  __STL_CONVERTIBLE(typename iterator_traits<_ForwardIter2>::value_type,
                    typename iterator_traits<_ForwardIter1>::value_type);
  for ( ; __first1 != __last1; ++__first1, ++__first2)//������һ������
    iter_swap(__first1, __first2);//������������ָ��Ԫ��
  return __first2;
}
//swap_ranges����������
/*
	#include <iostream>     // std::cout
	#include <algorithm>    // std::swap_ranges
	#include <vector>       // std::vector

	int main () {
	  std::vector<int> foo (5,10);        // foo: 10 10 10 10 10
	  std::vector<int> bar (5,33);        // bar: 33 33 33 33 33

	  std::swap_ranges(foo.begin()+1, foo.end()-1, bar.begin());

	  // print out results of swap:
	  std::cout << "foo contains:";
	  for (std::vector<int>::iterator it=foo.begin(); it!=foo.end(); ++it)
		std::cout << ' ' << *it;
	  std::cout << '\n';

	  std::cout << "bar contains:";
	  for (std::vector<int>::iterator it=bar.begin(); it!=bar.end(); ++it)
		std::cout << ' ' << *it;
	  std::cout << '\n';

	  return 0;
	}
	Output:
	foo contains: 10 33 33 33 10
	bar contains: 10 10 10 33 33
*/

// transform
//�����汾
/*
����ԭ�ͣ�
unary operation(1)���汾һ	
	template <class InputIterator, class OutputIterator, class UnaryOperation>
	OutputIterator transform (InputIterator first1, InputIterator last1,
                            OutputIterator result, UnaryOperation op);
binary operation(2)���汾��	
	template <class InputIterator1, class InputIterator2,
          class OutputIterator, class BinaryOperation>
	OutputIterator transform (InputIterator1 first1, InputIterator1 last1,
                            InputIterator2 first2, OutputIterator result,
                            BinaryOperation binary_op);
�������ܣ�
(1) unary operation
	Applies op to each of the elements in the range [first1,last1) and stores the value 
	returned by each operation in the range that begins at result.
(2) binary operation
	Calls binary_op using each of the elements in the range [first1,last1) as first argument, 
	and the respective argument in the range that begins at first2 as second argument. 
	The value returned by each call is stored in the range that begins at result.
*/
//��һ���汾���Էº���opr������[first,last)�е�ÿһ��Ԫ�أ���������������һ���µ�����
template <class _InputIter, class _OutputIter, class _UnaryOperation>
_OutputIter transform(_InputIter __first, _InputIter __last,
                      _OutputIter __result, _UnaryOperation __opr) {
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);

  for ( ; __first != __last; ++__first, ++__result)
    *__result = __opr(*__first);
  return __result;
}
//�ڶ����汾���Էº���binary_op������һ˫Ԫ���ϣ�����һ��Ԫ������[first1,last1)����һ��Ԫ�����ԡ���first2��ʼ�����С���
//��������������һ���µ�����
template <class _InputIter1, class _InputIter2, class _OutputIter,
          class _BinaryOperation>
_OutputIter transform(_InputIter1 __first1, _InputIter1 __last1,
                      _InputIter2 __first2, _OutputIter __result,
                      _BinaryOperation __binary_op) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  for ( ; __first1 != __last1; ++__first1, ++__first2, ++__result)
    *__result = __binary_op(*__first1, *__first2);
  return __result;
}
//transform����������
/*
	#include <vector>       // std::vector
	#include <functional>   // std::plus

	int op_increase (int i) { return ++i; }

	int main () {
	  std::vector<int> foo;
	  std::vector<int> bar;

	  // set some values:
	  for (int i=1; i<6; i++)
		foo.push_back (i*10);                         // foo: 10 20 30 40 50

	  bar.resize(foo.size());                         // allocate space

	  std::transform (foo.begin(), foo.end(), bar.begin(), op_increase);
													  // bar: 11 21 31 41 51
	  std::cout << "bar contains:";
	  for (std::vector<int>::iterator it=bar.begin(); it!=bar.end(); ++it)
		std::cout << ' ' << *it;
		std::cout << '\n';

	  // std::plus adds together its two arguments:
	  std::transform (foo.begin(), foo.end(), bar.begin(), foo.begin(), std::plus<int>());
													  // foo: 21 41 61 81 101

	  std::cout << "foo contains:";
	  for (std::vector<int>::iterator it=foo.begin(); it!=foo.end(); ++it)
		std::cout << ' ' << *it;
	  std::cout << '\n';

	  return 0;
	}
	Output:
	bar contains: 11 21 31 41 51
	foo contains: 21 41 61 81 101
*/

// replace, replace_if, replace_copy, replace_copy_if
//������[first,last)�ڵ�����old_value����new_value���.
template <class _ForwardIter, class _Tp>
void replace(_ForwardIter __first, _ForwardIter __last,
             const _Tp& __old_value, const _Tp& __new_value) {
  __STL_REQUIRES(_ForwardIter, _Mutable_ForwardIterator);
  __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool,
         typename iterator_traits<_ForwardIter>::value_type, _Tp);
  __STL_CONVERTIBLE(_Tp, typename iterator_traits<_ForwardIter>::value_type);
  for ( ; __first != __last; ++__first)
	  //������������old_value����new_value���.
    if (*__first == __old_value)
      *__first = __new_value;
}
//������[first,last)�ڵ����б�pred�ж�Ϊtrue��Ԫ�ض���new_value���.
template <class _ForwardIter, class _Predicate, class _Tp>
void replace_if(_ForwardIter __first, _ForwardIter __last,
                _Predicate __pred, const _Tp& __new_value) {
  __STL_REQUIRES(_ForwardIter, _Mutable_ForwardIterator);
  __STL_CONVERTIBLE(_Tp, typename iterator_traits<_ForwardIter>::value_type);
  __STL_UNARY_FUNCTION_CHECK(_Predicate, bool,
             typename iterator_traits<_ForwardIter>::value_type);
  for ( ; __first != __last; ++__first)
    if (__pred(*__first))//pred�ж�Ϊtrue
      *__first = __new_value;//�޸���ֵ
}
//������[first,last)�ڵ�����old_value����new_value���.�������и��Ƶ�result��ָ��������
//ԭʼ���������ݲ�����ı�
template <class _InputIter, class _OutputIter, class _Tp>
_OutputIter replace_copy(_InputIter __first, _InputIter __last,
                         _OutputIter __result,
                         const _Tp& __old_value, const _Tp& __new_value) {
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool,
         typename iterator_traits<_InputIter>::value_type, _Tp);
  for ( ; __first != __last; ++__first, ++__result)
    *__result = *__first == __old_value ? __new_value : *__first;
  return __result;
}
//������[first,last)�ڵ����б�pred�ж�Ϊtrue��Ԫ�ض���new_value���.�������и��Ƶ�result��ָ��������
//ԭʼ���������ݲ�����ı�
template <class _InputIter, class _OutputIter, class _Predicate, class _Tp>
_OutputIter replace_copy_if(_InputIter __first, _InputIter __last,
                            _OutputIter __result,
                            _Predicate __pred, const _Tp& __new_value) {
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_UNARY_FUNCTION_CHECK(_Predicate, bool,
                typename iterator_traits<_InputIter>::value_type);
  for ( ; __first != __last; ++__first, ++__result)
    *__result = __pred(*__first) ? __new_value : *__first;
  return __result;
}

// generate and generate_n
//���º���gen�Ĵ����������[first, last)����������Ԫ���ϣ���ν��д
//�����õ�������ָԪ�ص�assignment����
//ע�⣺�����û��Զ�������Ҫ�ṩoperator =() 

template <class _ForwardIter, class _Generator>
void generate(_ForwardIter __first, _ForwardIter __last, _Generator __gen) {
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_GENERATOR_CHECK(_Generator, 
          typename iterator_traits<_ForwardIter>::value_type);
  for ( ; __first != __last; ++__first)//������������
    *__first = __gen();
}
//���º���gen�Ĵ����������first��ʼ��n��Ԫ���ϣ���ν��д
//�����õ�������ָԪ�ص�assignment����
template <class _OutputIter, class _Size, class _Generator>
_OutputIter generate_n(_OutputIter __first, _Size __n, _Generator __gen) {
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  for ( ; __n > 0; --__n, ++__first)//ֻ����n��Ԫ��
    *__first = __gen();
  return __first;
}
//generate��generate_n����������
/*
	#include <iostream>     // std::cout
	#include <algorithm>    // std::generate_n

	int current = 0;
	int UniqueNumber () { return ++current; }

	int main () {
	  int myarray[9];

	  std::generate_n (myarray, 9, UniqueNumber);

	  std::cout << "myarray contains:";
	  for (int i=0; i<9; ++i)
		std::cout << ' ' << myarray[i];
	  std::cout << '\n';
	  std::cout <<"the value of current: "<<current;
	  std::cout << '\n';

	  std::vector<int> myvector (6);
	  std::generate (myvector.begin(), myvector.end(), UniqueNumber);

	  std::cout << "myvector contains:";
	  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
		std::cout << ' ' << *it;
	  std::cout << '\n';

	  return 0;
	}
	Output:
	myarray contains: 1 2 3 4 5 6 7 8 9
	the value of current: 9
	myvector contains: 10 11 12 13 14 15
*/

// remove, remove_if, remove_copy, remove_copy_if

//�Ƴ�[first,last)������������valueֵ��ȵ�Ԫ�أ������������Ĵ�������ɾ����ЩԪ��(ԭ���������ݲ���ı�)
//���ǽ�������Ƶ�һ����resultΪ��ʼλ�õ������С�������������ԭ�����ص�
template <class _InputIter, class _OutputIter, class _Tp>
_OutputIter remove_copy(_InputIter __first, _InputIter __last,
                        _OutputIter __result, const _Tp& __value) {
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool,
       typename iterator_traits<_InputIter>::value_type, _Tp);
  for ( ; __first != __last; ++__first)//��������
    if (!(*__first == __value)) {//��������
      *__result = *__first;//��ֵ��������
      ++__result;//������ǰ��һ��λ��
    }
  return __result;
}
//�Ƴ�[first,last)�����ڱ��º���pred�ж�Ϊtrue��Ԫ��,�����������Ĵ�������ɾ����ЩԪ��(ԭ���������ݲ���ı�)
//���ǽ�������Ƶ�һ����resultΪ��ʼλ�õ������С�������������ԭ�����ص�
template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter remove_copy_if(_InputIter __first, _InputIter __last,
                           _OutputIter __result, _Predicate __pred) {
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_UNARY_FUNCTION_CHECK(_Predicate, bool,
             typename iterator_traits<_InputIter>::value_type);
  for ( ; __first != __last; ++__first)//��������
    if (!__pred(*__first)) {//��pred�ж�Ϊfalse
      *__result = *__first;//��ֵ��������
      ++__result;//������ǰ��һ��λ��
    }
  return __result;
}
//�Ƴ�[first,last)������������valueֵ��ȵ�Ԫ��,�ò�������ı�������С��ֻ��������Ԫ��ֵ�ı�
//���Ƴ�֮��������������������
template <class _ForwardIter, class _Tp>
_ForwardIter remove(_ForwardIter __first, _ForwardIter __last,
                    const _Tp& __value) {
  __STL_REQUIRES(_ForwardIter, _Mutable_ForwardIterator);
  __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool,
       typename iterator_traits<_ForwardIter>::value_type, _Tp);
  __STL_CONVERTIBLE(_Tp, typename iterator_traits<_ForwardIter>::value_type);
  __first = find(__first, __last, __value);//����˳������ҳ���һ����value��ȵ�Ԫ��
  _ForwardIter __i = __first;
  //�������remove_copy
  return __first == __last ? __first 
                           : remove_copy(++__i, __last, __first, __value);
}
//�Ƴ�[first,last)���������б�pred�ж�Ϊtrue��Ԫ��,�ò�������ı�������С��ֻ��������Ԫ��ֵ�ı�
//���Ƴ�֮��������������������
template <class _ForwardIter, class _Predicate>
_ForwardIter remove_if(_ForwardIter __first, _ForwardIter __last,
                       _Predicate __pred) {
  __STL_REQUIRES(_ForwardIter, _Mutable_ForwardIterator);
  __STL_UNARY_FUNCTION_CHECK(_Predicate, bool,
               typename iterator_traits<_ForwardIter>::value_type);
  __first = find_if(__first, __last, __pred);//����˳������ҳ���һ����value��ȵ�Ԫ��
  _ForwardIter __i = __first;
  //�������remove_copy_if
  return __first == __last ? __first 
                           : remove_copy_if(++__i, __last, __first, __pred);
}
//�����ĸ��Ƴ�����������
/*
	#include <iostream>     // std::cout
	#include <algorithm>    // std::remove

	bool IsOdd (int i) { return ((i%2)==1); }

	int main () {
	  int myints[] = {10,20,31,30,20,11,10,20};      // 10 20 31 30 20 11 10 20

	  std::vector<int> myvector (8);
	  std::remove_copy (myints,myints+8,myvector.begin(),20); // 10 31 30 11 10 0 0 0
	  std::cout << "myvector contains:";
	  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
		std::cout << ' ' << *it;
	  std::cout << '\n';
  
	  // bounds of range:
	  int* pbegin = myints;                          // ^
	  int* pend = myints+sizeof(myints)/sizeof(int); // ^                       ^
	  pend = std::remove (pbegin, pend, 20);         // 10 31 30 11 10 ?  ?  ?
													 // ^              ^
	  std::cout << "range contains:";
	  for (int* p=pbegin; p!=pend; ++p)
		std::cout << ' ' << *p;
	  std::cout << '\n';
  
	  std::vector<int> myvector2 (7);
	  std::remove_copy_if (myints,myints+7,myvector2.begin(),IsOdd);
	  std::cout << "myvector2 contains:";
	  for (std::vector<int>::iterator it=myvector2.begin(); it!=myvector2.end(); ++it)
		std::cout << ' ' << *it;
	  std::cout << '\n';
  
	  pend = std::remove_if (pbegin, pend, IsOdd);   // 10 30 10 ? ? ? ? ?
													 // ^       ^
	  std::cout << "the range contains:";
	  for (int* p=pbegin; p!=pend; ++p)
		std::cout << ' ' << *p;
	  std::cout << '\n';  

	  return 0;
	}
	Output:
	myvector contains: 10 31 30 11 10 0 0 0
	range contains: 10 31 30 11 10
	myvector2 contains: 10 30 10 10 0 0 0
	the range contains: 10 30 10
*/

// unique and unique_copy

template <class _InputIter, class _OutputIter, class _Tp>
_OutputIter __unique_copy(_InputIter __first, _InputIter __last,
                          _OutputIter __result, _Tp*) {
  _Tp __value = *__first;
  *__result = __value;
  while (++__first != __last)
    if (!(__value == *__first)) {
      __value = *__first;
      *++__result = __value;
    }
  return ++__result;
}
//��result����Ϊoutput_iterator_tag������øú���
template <class _InputIter, class _OutputIter>
inline _OutputIter __unique_copy(_InputIter __first, _InputIter __last,
                                 _OutputIter __result, 
                                 output_iterator_tag) {
		//�ж�first��value_type���ͣ����ݲ�ͬ���͵��ò�ͬ����
  return __unique_copy(__first, __last, __result, __VALUE_TYPE(__first));
}
//��result����Ϊforward_iterator_tag������øú���
template <class _InputIter, class _ForwardIter>
_ForwardIter __unique_copy(_InputIter __first, _InputIter __last,
                           _ForwardIter __result, forward_iterator_tag) {
  *__result = *__first;//��¼��һ��Ԫ��
  while (++__first != __last)//��������
	  //�������������ظ�Ԫ�أ��������¼��Ŀ����result
    if (!(*__result == *__first))
      *++__result = *__first;//��¼Ԫ�ص�Ŀ����
  return ++__result;
}
////unique_copy������[first,last)��Ԫ�ظ��Ƶ���result��ͷ�������ϣ�����������������ظ�Ԫ��ʱ��ֻ�������е�һ��Ԫ��
//��uniqueһ��������Ҳ�������汾
/*
����ԭ�ͣ�
equality (1)	
	template <class InputIterator, class OutputIterator>
	OutputIterator unique_copy (InputIterator first, InputIterator last,
                              OutputIterator result);
predicate (2)	
	template <class InputIterator, class OutputIterator, class BinaryPredicate>
	OutputIterator unique_copy (InputIterator first, InputIterator last,
                              OutputIterator result, BinaryPredicate pred);
*/
//�汾һ
template <class _InputIter, class _OutputIter>
inline _OutputIter unique_copy(_InputIter __first, _InputIter __last,
                               _OutputIter __result) {
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES(typename iterator_traits<_InputIter>::value_type,
                 _EqualityComparable);
  if (__first == __last) return __result;
  //����result�����������ͣ����ò�ͬ�ĺ���
  return __unique_copy(__first, __last, __result,
                       __ITERATOR_CATEGORY(__result));
}

template <class _InputIter, class _OutputIter, class _BinaryPredicate,
          class _Tp>
_OutputIter __unique_copy(_InputIter __first, _InputIter __last,
                          _OutputIter __result,
                          _BinaryPredicate __binary_pred, _Tp*) {
  __STL_BINARY_FUNCTION_CHECK(_BinaryPredicate, bool, _Tp, _Tp);
  _Tp __value = *__first;
  *__result = __value;
  while (++__first != __last)
    if (!__binary_pred(__value, *__first)) {
      __value = *__first;
      *++__result = __value;
    }
  return ++__result;
}

template <class _InputIter, class _OutputIter, class _BinaryPredicate>
inline _OutputIter __unique_copy(_InputIter __first, _InputIter __last,
                                 _OutputIter __result,
                                 _BinaryPredicate __binary_pred,
                                 output_iterator_tag) {
  return __unique_copy(__first, __last, __result, __binary_pred,
                       __VALUE_TYPE(__first));
}

template <class _InputIter, class _ForwardIter, class _BinaryPredicate>
_ForwardIter __unique_copy(_InputIter __first, _InputIter __last,
                           _ForwardIter __result, 
                           _BinaryPredicate __binary_pred,
                           forward_iterator_tag) {
  __STL_BINARY_FUNCTION_CHECK(_BinaryPredicate, bool,
     typename iterator_traits<_ForwardIter>::value_type,
     typename iterator_traits<_InputIter>::value_type);
  *__result = *__first;
  while (++__first != __last)
    if (!__binary_pred(*__result, *__first)) *++__result = *__first;
  return ++__result;
}
//�汾��
template <class _InputIter, class _OutputIter, class _BinaryPredicate>
inline _OutputIter unique_copy(_InputIter __first, _InputIter __last,
                               _OutputIter __result,
                               _BinaryPredicate __binary_pred) {
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  if (__first == __last) return __result;
  //����result�����������ͣ����ò�ͬ�ĺ���
  return __unique_copy(__first, __last, __result, __binary_pred,
                       __ITERATOR_CATEGORY(__result));
}
//�Ƴ�����[first,last)���������ظ���Ԫ��
//unique�������汾
//���ܣ�Removes all but the first element from every consecutive group of equivalent elements in the range [first,last).
/*
����ԭ�ͣ�
equality (1)���汾һ����operator==	
	template <class ForwardIterator>
	ForwardIterator unique (ForwardIterator first, ForwardIterator last);
predicate (2)���汾������pred����	
	template <class ForwardIterator, class BinaryPredicate>
	ForwardIterator unique (ForwardIterator first, ForwardIterator last,
                          BinaryPredicate pred);
*/
//�汾һ
template <class _ForwardIter>
_ForwardIter unique(_ForwardIter __first, _ForwardIter __last) {
  __STL_REQUIRES(_ForwardIter, _Mutable_ForwardIterator);
  __STL_REQUIRES(typename iterator_traits<_ForwardIter>::value_type,
                 _EqualityComparable);
  __first = adjacent_find(__first, __last);//�ҳ���һ������Ԫ�ص���ʼλ��
  return unique_copy(__first, __last, __first);//����unique_copy��ɲ���
}
//�汾��
template <class _ForwardIter, class _BinaryPredicate>
_ForwardIter unique(_ForwardIter __first, _ForwardIter __last,
                    _BinaryPredicate __binary_pred) {
  __STL_REQUIRES(_ForwardIter, _Mutable_ForwardIterator);
  __STL_BINARY_FUNCTION_CHECK(_BinaryPredicate, bool, 
      typename iterator_traits<_ForwardIter>::value_type,
      typename iterator_traits<_ForwardIter>::value_type);
  __first = adjacent_find(__first, __last, __binary_pred);//�ҳ���һ������Ԫ�ص���ʼλ��
  return unique_copy(__first, __last, __first, __binary_pred);//����unique_copy��ɲ���
}

// reverse and reverse_copy, and their auxiliary functions

//������������Ϊbidirectional_iterator_tag������ô˺���
template <class _BidirectionalIter>
void __reverse(_BidirectionalIter __first, _BidirectionalIter __last, 
               bidirectional_iterator_tag) {
  while (true)
    if (__first == __last || __first == --__last)//������ע�⣬ÿ���ж�last�������������һλ
      return;
    else
      iter_swap(__first++, __last);//���򽻻���������ָ��Ԫ��
}
//������������Ϊrandom_access_iterator_tag������ô˺���
template <class _RandomAccessIter>
void __reverse(_RandomAccessIter __first, _RandomAccessIter __last,
               random_access_iterator_tag) {
  while (__first < __last)//��������
    iter_swap(__first++, --__last);//�������˵�������ָ��Ԫ��
}
//������[first,last)������Ԫ����ԭ�����еߵ�����
template <class _BidirectionalIter>
inline void reverse(_BidirectionalIter __first, _BidirectionalIter __last) {
  __STL_REQUIRES(_BidirectionalIter, _Mutable_BidirectionalIterator);
  //������ȡ��������������
  __reverse(__first, __last, __ITERATOR_CATEGORY(__first));
}
//��Ϊ����reverse���������������лᱻ������resultָ����������
template <class _BidirectionalIter, class _OutputIter>
_OutputIter reverse_copy(_BidirectionalIter __first,
                         _BidirectionalIter __last,
                         _OutputIter __result) {
  __STL_REQUIRES(_BidirectionalIter, _BidirectionalIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  while (__first != __last) {//��������
    --__last;//β��ǰ��һ��λ��
    *__result = *__last;//result��������ʼλ��Ԫ��ֵΪԭʼ����β��Ԫ��ֵ
    ++__result;//����result��ʹ��ǰ��һ��λ��
  }
  return __result;
}

// rotate and rotate_copy, and their auxiliary functions

template <class _EuclideanRingElement>
_EuclideanRingElement __gcd(_EuclideanRingElement __m,
                            _EuclideanRingElement __n)
{
  while (__n != 0) {
    _EuclideanRingElement __t = __m % __n;
    __m = __n;
    __n = __t;
  }
  return __m;
}
//����������Ϊforward_iterator_tag�����ô˺���
template <class _ForwardIter, class _Distance>
_ForwardIter __rotate(_ForwardIter __first,
                      _ForwardIter __middle,
                      _ForwardIter __last,
                      _Distance*,
                      forward_iterator_tag) {
  if (__first == __middle)
    return __last;
  if (__last  == __middle)
    return __first;

  _ForwardIter __first2 = __middle;
  do {
    swap(*__first++, *__first2++);
    if (__first == __middle)
      __middle = __first2;
  } while (__first2 != __last);

  _ForwardIter __new_middle = __first;

  __first2 = __middle;

  while (__first2 != __last) {
    swap (*__first++, *__first2++);
    if (__first == __middle)
      __middle = __first2;
    else if (__first2 == __last)
      __first2 = __middle;
  }

  return __new_middle;
}

//����������Ϊbidirectional_iterator_tag�����ô˺���
template <class _BidirectionalIter, class _Distance>
_BidirectionalIter __rotate(_BidirectionalIter __first,
                            _BidirectionalIter __middle,
                            _BidirectionalIter __last,
                            _Distance*,
                            bidirectional_iterator_tag) {
  __STL_REQUIRES(_BidirectionalIter, _Mutable_BidirectionalIterator);
  if (__first == __middle)
    return __last;
  if (__last  == __middle)
    return __first;

  __reverse(__first,  __middle, bidirectional_iterator_tag());
  __reverse(__middle, __last,   bidirectional_iterator_tag());

  while (__first != __middle && __middle != __last)
    swap (*__first++, *--__last);

  if (__first == __middle) {
    __reverse(__middle, __last,   bidirectional_iterator_tag());
    return __last;
  }
  else {
    __reverse(__first,  __middle, bidirectional_iterator_tag());
    return __first;
  }
}
//����������ΪRandom_iterator_tag�����ô˺���
template <class _RandomAccessIter, class _Distance, class _Tp>
_RandomAccessIter __rotate(_RandomAccessIter __first,
                           _RandomAccessIter __middle,
                           _RandomAccessIter __last,
                           _Distance *, _Tp *) {
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  _Distance __n = __last   - __first;
  _Distance __k = __middle - __first;
  _Distance __l = __n - __k;
  _RandomAccessIter __result = __first + (__last - __middle);

  if (__k == 0)
    return __last;

  else if (__k == __l) {
    swap_ranges(__first, __middle, __middle);
    return __result;
  }

  _Distance __d = __gcd(__n, __k);

  for (_Distance __i = 0; __i < __d; __i++) {
    _Tp __tmp = *__first;
    _RandomAccessIter __p = __first;

    if (__k < __l) {
      for (_Distance __j = 0; __j < __l/__d; __j++) {
        if (__p > __first + __l) {
          *__p = *(__p - __l);
          __p -= __l;
        }

        *__p = *(__p + __k);
        __p += __k;
      }
    }

    else {
      for (_Distance __j = 0; __j < __k/__d - 1; __j ++) {
        if (__p < __last - __k) {
          *__p = *(__p + __k);
          __p += __k;
        }

        *__p = * (__p - __l);
        __p -= __l;
      }
    }

    *__p = __tmp;
    ++__first;
  }

  return __result;
}
//������[first,middle)�ڵ�Ԫ�غ�[middle,last)�ڵ�Ԫ�ػ�����minddle��ָ��Ԫ�ػ��Ϊ�����ĵ�һ��Ԫ��
//���������{1,2,3,4,5,6,7}����Ԫ��3������ת����,����Ϊ{3,4,5,6,7,1,2}
template <class _ForwardIter>
inline _ForwardIter rotate(_ForwardIter __first, _ForwardIter __middle,
                           _ForwardIter __last) {
  __STL_REQUIRES(_ForwardIter, _Mutable_ForwardIterator);
  //��ȡ�������������ͣ����ݵ����������͵��ò�ͬ�ĺ���
  return __rotate(__first, __middle, __last,
                  __DISTANCE_TYPE(__first),
                  __ITERATOR_CATEGORY(__first));
}
//������[first,middle)�ڵ�Ԫ�غ�[middle,last)�ڵ�Ԫ�ػ�����minddle��ָ��Ԫ�ػ��Ϊ������result�ĵ�һ��Ԫ��
template <class _ForwardIter, class _OutputIter>
_OutputIter rotate_copy(_ForwardIter __first, _ForwardIter __middle,
                        _ForwardIter __last, _OutputIter __result) {
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  //����ֱ�Ӳ��ø��Ʋ������Ȱ�[middle,last)���Ƶ�result�����У�
  //�ٰ�[first,middle)���ݸ��Ƶ�result������
  return copy(__first, __middle, copy(__middle, __last, __result));
}

// Return a random number in the range [0, __n).  This function encapsulates
// whether we're using rand (part of the standard C library) or lrand48
// (not standard, but a much better choice whenever it's available).

template <class _Distance>
inline _Distance __random_number(_Distance __n) {
#ifdef __STL_NO_DRAND48
  return rand() % __n;
#else
  return lrand48() % __n;
#endif
}

// random_shuffle
//������[first,last)�ڵ�Ԫ���������
//�����汾�Ĳ�ͬ���������ȡ��
//�汾һ��ʹ���ڲ������������
//�汾����ʹ��һ�������������ķº���

/*
�������ܣ�Rearranges the elements in the range [first,last) randomly.
����ԭ�ͣ�
generator by default (1)	
	template <class RandomAccessIterator>
	void random_shuffle (RandomAccessIterator first, RandomAccessIterator last);
specific generator (2)	
	template <class RandomAccessIterator, class RandomNumberGenerator>
	void random_shuffle (RandomAccessIterator first, RandomAccessIterator last,
                       RandomNumberGenerator& gen);
*/
//�汾һ
template <class _RandomAccessIter>
inline void random_shuffle(_RandomAccessIter __first,
                           _RandomAccessIter __last) {
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  if (__first == __last) return;
  for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
    iter_swap(__i, __first + __random_number((__i - __first) + 1));
}
//�汾��
template <class _RandomAccessIter, class _RandomNumberGenerator>
void random_shuffle(_RandomAccessIter __first, _RandomAccessIter __last,
                    _RandomNumberGenerator& __rand) {
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  if (__first == __last) return;
  for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
    iter_swap(__i, __first + __rand((__i - __first) + 1));
}

// random_sample and random_sample_n (extensions, not part of the standard).

template <class _ForwardIter, class _OutputIter, class _Distance>
_OutputIter random_sample_n(_ForwardIter __first, _ForwardIter __last,
                            _OutputIter __out, const _Distance __n)
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  _Distance __remaining = 0;
  distance(__first, __last, __remaining);
  _Distance __m = min(__n, __remaining);

  while (__m > 0) {
    if (__random_number(__remaining) < __m) {
      *__out = *__first;
      ++__out;
      --__m;
    }

    --__remaining;
    ++__first;
  }
  return __out;
}

template <class _ForwardIter, class _OutputIter, class _Distance,
          class _RandomNumberGenerator>
_OutputIter random_sample_n(_ForwardIter __first, _ForwardIter __last,
                            _OutputIter __out, const _Distance __n,
                            _RandomNumberGenerator& __rand)
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_UNARY_FUNCTION_CHECK(_RandomNumberGenerator, _Distance, _Distance);
  _Distance __remaining = 0;
  distance(__first, __last, __remaining);
  _Distance __m = min(__n, __remaining);

  while (__m > 0) {
    if (__rand(__remaining) < __m) {
      *__out = *__first;
      ++__out;
      --__m;
    }

    --__remaining;
    ++__first;
  }
  return __out;
}

template <class _InputIter, class _RandomAccessIter, class _Distance>
_RandomAccessIter __random_sample(_InputIter __first, _InputIter __last,
                                  _RandomAccessIter __out,
                                  const _Distance __n)
{
  _Distance __m = 0;
  _Distance __t = __n;
  for ( ; __first != __last && __m < __n; ++__m, ++__first) 
    __out[__m] = *__first;

  while (__first != __last) {
    ++__t;
    _Distance __M = __random_number(__t);
    if (__M < __n)
      __out[__M] = *__first;
    ++__first;
  }

  return __out + __m;
}

template <class _InputIter, class _RandomAccessIter,
          class _RandomNumberGenerator, class _Distance>
_RandomAccessIter __random_sample(_InputIter __first, _InputIter __last,
                                  _RandomAccessIter __out,
                                  _RandomNumberGenerator& __rand,
                                  const _Distance __n)
{
  __STL_UNARY_FUNCTION_CHECK(_RandomNumberGenerator, _Distance, _Distance);
  _Distance __m = 0;
  _Distance __t = __n;
  for ( ; __first != __last && __m < __n; ++__m, ++__first)
    __out[__m] = *__first;

  while (__first != __last) {
    ++__t;
    _Distance __M = __rand(__t);
    if (__M < __n)
      __out[__M] = *__first;
    ++__first;
  }

  return __out + __m;
}

template <class _InputIter, class _RandomAccessIter>
inline _RandomAccessIter
random_sample(_InputIter __first, _InputIter __last,
              _RandomAccessIter __out_first, _RandomAccessIter __out_last) 
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  return __random_sample(__first, __last,
                         __out_first, __out_last - __out_first);
}


template <class _InputIter, class _RandomAccessIter, 
          class _RandomNumberGenerator>
inline _RandomAccessIter
random_sample(_InputIter __first, _InputIter __last,
              _RandomAccessIter __out_first, _RandomAccessIter __out_last,
              _RandomNumberGenerator& __rand) 
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  return __random_sample(__first, __last,
                         __out_first, __rand,
                         __out_last - __out_first);
}

// partition, stable_partition, and their auxiliary functions
//��������������Ϊforward_iterator_tag������ô˺���
template <class _ForwardIter, class _Predicate>
_ForwardIter __partition(_ForwardIter __first,
		         _ForwardIter __last,
			 _Predicate   __pred,
			 forward_iterator_tag) {
  if (__first == __last) return __first;//��Ϊ�գ�ֱ���˳�

  while (__pred(*__first))//��pred��first��ֵΪtrue
    if (++__first == __last) return __first;//���ƶ�������first�����ж��Ƿ񵽴�β��last

  _ForwardIter __next = __first;//�����ж�

  while (++__next != __last)//����һ��λ����Ȼ����β��
    if (__pred(*__next)) {//����pred��next��ֵ����Ϊtrue
      swap(*__first, *__next);//����ֵ
      ++__first;//������һλ��
    }

  return __first;
}
//��������������Ϊbidirectional_iterator_tag������ô˺���
template <class _BidirectionalIter, class _Predicate>
_BidirectionalIter __partition(_BidirectionalIter __first,
                               _BidirectionalIter __last,
			       _Predicate __pred,
			       bidirectional_iterator_tag) {
  while (true) {
    while (true)
      if (__first == __last)//��Ϊ��
        return __first;//ֱ���˳�
      else if (__pred(*__first))//first��ֵ���ϲ��ƶ����������ƶ���ֵ
        ++__first;//ֻ�ƶ�������
      else//��ͷָ������ƶ�
        break;//����ѭ��
    --__last;//βָ�����
    while (true)
      if (__first == __last)//ͷָ�����βָ��
        return __first;//��������
      else if (!__pred(*__last))//βָ���Ԫ�ط��ϲ��ƶ�����
        --__last;//���ƶ��������������ƶ�����Ԫ��
      else//βָ���Ԫ�ط����ƶ�����
        break;//����ѭ��
    iter_swap(__first, __last);//ͷβָ�뽻��Ԫ��
    ++__first;//׼����һ��ѭ��
  }
}
//������[first,last)��Ԫ�ؽ������򣬱�pred�ж�Ϊtrue�ķ��������ǰ�Σ��ж�Ϊfalse�ķ���������
//��������ܻ�ʹԪ�ص�Ԫ��λ�÷����ı�.
/*
�㷨���ܣ�Rearranges the elements from the range [first,last), in such a way that all the elements
for which pred returns true precede all those for which it returns false. 
The iterator returned points to the first element of the second group.

�㷨ԭ�ͣ�
	template <class BidirectionalIterator, class UnaryPredicate>
	BidirectionalIterator partition (BidirectionalIterator first,
                                   BidirectionalIterator last, UnaryPredicate pred);
*/
template <class _ForwardIter, class _Predicate>
inline _ForwardIter partition(_ForwardIter __first,
   			      _ForwardIter __last,
			      _Predicate   __pred) {
  __STL_REQUIRES(_ForwardIter, _Mutable_ForwardIterator);
  __STL_UNARY_FUNCTION_CHECK(_Predicate, bool, 
        typename iterator_traits<_ForwardIter>::value_type);
  //������ȡ��������first�����ͣ����ݵ����������͵��ò�ͬ�ĺ���
  return __partition(__first, __last, __pred, __ITERATOR_CATEGORY(__first));
}
//partition����������
/*
	#include <iostream>     // std::cout
	#include <algorithm>    // std::partition
	#include <vector>       // std::vector

	bool IsOdd (int i) { return (i%2)==1; }

	int main () {
	  std::vector<int> myvector;

	  // set some values:
	  for (int i=1; i<10; ++i) myvector.push_back(i); // 1 2 3 4 5 6 7 8 9

	  std::vector<int>::iterator bound;
	  bound = std::partition (myvector.begin(), myvector.end(), IsOdd);

	  // print out content:
	  std::cout << "odd elements:";
	  for (std::vector<int>::iterator it=myvector.begin(); it!=bound; ++it)
		std::cout << ' ' << *it;
	  std::cout << '\n';

	  std::cout << "even elements:";
	  for (std::vector<int>::iterator it=bound; it!=myvector.end(); ++it)
		std::cout << ' ' << *it;
	  std::cout << '\n';

	  return 0;
	}
	Output:
	odd elements: 1 9 3 7 5
	even elements: 6 4 8 2
 
*/

template <class _ForwardIter, class _Predicate, class _Distance>
_ForwardIter __inplace_stable_partition(_ForwardIter __first,
                                        _ForwardIter __last,
                                        _Predicate __pred, _Distance __len) {
  if (__len == 1)
    return __pred(*__first) ? __last : __first;
  _ForwardIter __middle = __first;
  advance(__middle, __len / 2);
  return rotate(__inplace_stable_partition(__first, __middle, __pred, 
                                           __len / 2),
                __middle,
                __inplace_stable_partition(__middle, __last, __pred,
                                           __len - __len / 2));
}

template <class _ForwardIter, class _Pointer, class _Predicate, 
          class _Distance>
_ForwardIter __stable_partition_adaptive(_ForwardIter __first,
                                         _ForwardIter __last,
                                         _Predicate __pred, _Distance __len,
                                         _Pointer __buffer,
                                         _Distance __buffer_size) 
{
  if (__len <= __buffer_size) {
    _ForwardIter __result1 = __first;
    _Pointer __result2 = __buffer;
    for ( ; __first != __last ; ++__first)
      if (__pred(*__first)) {
        *__result1 = *__first;
        ++__result1;
      }
      else {
        *__result2 = *__first;
        ++__result2;
      }
    copy(__buffer, __result2, __result1);
    return __result1;
  }
  else {
    _ForwardIter __middle = __first;
    advance(__middle, __len / 2);
    return rotate(__stable_partition_adaptive(
                          __first, __middle, __pred,
                          __len / 2, __buffer, __buffer_size),
                    __middle,
                    __stable_partition_adaptive(
                          __middle, __last, __pred,
                          __len - __len / 2, __buffer, __buffer_size));
  }
}

template <class _ForwardIter, class _Predicate, class _Tp, class _Distance>
inline _ForwardIter
__stable_partition_aux(_ForwardIter __first, _ForwardIter __last, 
                       _Predicate __pred, _Tp*, _Distance*)
{
  _Temporary_buffer<_ForwardIter, _Tp> __buf(__first, __last);
  if (__buf.size() > 0)
    return __stable_partition_adaptive(__first, __last, __pred,
                                       _Distance(__buf.requested_size()),
                                       __buf.begin(), __buf.size());
  else
    return __inplace_stable_partition(__first, __last, __pred, 
                                      _Distance(__buf.requested_size()));
}

template <class _ForwardIter, class _Predicate>
inline _ForwardIter stable_partition(_ForwardIter __first,
                                     _ForwardIter __last, 
                                     _Predicate __pred) {
  __STL_REQUIRES(_ForwardIter, _Mutable_ForwardIterator);
  __STL_UNARY_FUNCTION_CHECK(_Predicate, bool,
      typename iterator_traits<_ForwardIter>::value_type);
  if (__first == __last)
    return __first;
  else
    return __stable_partition_aux(__first, __last, __pred,
                                  __VALUE_TYPE(__first),
                                  __DISTANCE_TYPE(__first));
}
//�ҳ������������Ŧλ��
//�汾һ����operator<
template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __unguarded_partition(_RandomAccessIter __first, 
                                        _RandomAccessIter __last, 
                                        _Tp __pivot) 
{
	//�ҳ���Ŧ���λ��
	//��ͷ�˵�������β�˷����ƶ���β�˵�������ͷ���ƶ���
	//��*first��С����Ŧֵʱ����ͣ��������*last��������ŦֵʱҲͣ������Ȼ���������������Ƿ񽻴�
	//���first��Ȼ������last��Ȼ���Ҳ࣬�ͽ�������Ԫ�أ�Ȼ����Ե���λ�ã�������ƽ����ټ���ִ����ͬ����Ϊ.
	//ֱ��first��last����������������ʱ��ʾ���ҵ���Ŧ��λ�ü�first���ڵ�λ��
  while (true) {
    while (*__first < __pivot)
      ++__first;//first��β���ƶ���ֱ��������С����Ŧֵʱ��ֹͣ
    --__last;
    while (__pivot < *__last)
      --__last;//last��ͷ���ƶ���ֱ��������������Ŧֵʱ��ֹͣ
    if (!(__first < __last))//��������������Ƿ񽻴�
      return __first;//�������ʱ���ҵ�����Ϊfirst��������ָλ��
    iter_swap(__first, __last);//���򽻻���������ָ��Ԫ��
    ++__first;//����ִ����ͬ��Ϊ
  }
}    
//�汾һ����__comp
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __unguarded_partition(_RandomAccessIter __first, 
                                        _RandomAccessIter __last, 
                                        _Tp __pivot, _Compare __comp) 
{
  while (true) {
    while (__comp(*__first, __pivot))
      ++__first;
    --__last;
    while (__comp(__pivot, *__last))
      --__last;
    if (!(__first < __last))
      return __first;
    iter_swap(__first, __last);
    ++__first;
  }
}

const int __stl_threshold = 16;

// sort() and its auxiliary functions. 
//__insertion_sort�汾һ�ĸ�������
template <class _RandomAccessIter, class _Tp>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val) {
  _RandomAccessIter __next = __last;
  --__next;
  //__insertion_sort����ѭ��
  //ע�⣺һ�����ٳ�����ת�ԣ�ѭ���ͽ���
  while (__val < *__next) {//������ת��
    *__last = *__next;//����Ԫ��
    __last = __next;//����������
    --__next;//����һ��λ��
  }
  *__last = __val;//value����ȷ����λ��
}
//__insertion_sort�汾���ĸ�������
template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, 
                               _Compare __comp) {
  _RandomAccessIter __next = __last;
  --__next;  
  while (__comp(__val, *__next)) {
    *__last = *__next;
    __last = __next;
    --__next;
  }
  *__last = __val;
}
//__insertion_sort�汾һ�ĸ�������
template <class _RandomAccessIter, class _Tp>
inline void __linear_insert(_RandomAccessIter __first, 
                            _RandomAccessIter __last, _Tp*) {
  _Tp __val = *__last;//��¼βԪ��
  if (__val < *__first) {//βԪ�ر�ͷԪ�ػ�С
	  //����������������һ��λ��
    copy_backward(__first, __last, __last + 1);
    *__first = __val;//��ͷԪ�ص���ԭ�ȵ�βԪ��
	//������������Ĺ�������ڽ�������Ԫ��
  }
  else//βԪ�ز�С��ͷԪ��
    __unguarded_linear_insert(__last, __val);
}
//__insertion_sort�汾���ĸ�������
template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __linear_insert(_RandomAccessIter __first, 
                            _RandomAccessIter __last, _Tp*, _Compare __comp) {
  _Tp __val = *__last;
  if (__comp(__val, *__first)) {
    copy_backward(__first, __last, __last + 1);
    *__first = __val;
  }
  else
    __unguarded_linear_insert(__last, __val, __comp);
}
//__insertion_sort��˫��ѭ����ʽ���С���ѭ�������������У�ÿ�ε���������һ�������䣻
//��ѭ�����������䣬���������ڵ�ÿһ������ת�ԡ���ת���������һ�������ڡ���ת�ԡ�����ʾ������ϡ�
//����ת�ԡ����ָ�κ�����������i��j��i<j,��*i>*j.
//�汾һ
template <class _RandomAccessIter>
void __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last) {
  if (__first == __last) return; //������Ϊ�գ����˳�
  for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)//��ѭ����������������
	  //[first,i)�γɵ��ӿռ�
    __linear_insert(__first, __i, __VALUE_TYPE(__first));
}
//�汾��
template <class _RandomAccessIter, class _Compare>
void __insertion_sort(_RandomAccessIter __first,
                      _RandomAccessIter __last, _Compare __comp) {
  if (__first == __last) return;
  for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
    __linear_insert(__first, __i, __VALUE_TYPE(__first), __comp);
}

template <class _RandomAccessIter, class _Tp>
void __unguarded_insertion_sort_aux(_RandomAccessIter __first, 
                                    _RandomAccessIter __last, _Tp*) {
  for (_RandomAccessIter __i = __first; __i != __last; ++__i)
    __unguarded_linear_insert(__i, _Tp(*__i));
}
//sort�汾һ�ĸ�������
template <class _RandomAccessIter>
inline void __unguarded_insertion_sort(_RandomAccessIter __first, 
                                _RandomAccessIter __last) {
  __unguarded_insertion_sort_aux(__first, __last, __VALUE_TYPE(__first));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_insertion_sort_aux(_RandomAccessIter __first, 
                                    _RandomAccessIter __last,
                                    _Tp*, _Compare __comp) {
  for (_RandomAccessIter __i = __first; __i != __last; ++__i)
    __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template <class _RandomAccessIter, class _Compare>
inline void __unguarded_insertion_sort(_RandomAccessIter __first, 
                                       _RandomAccessIter __last,
                                       _Compare __comp) {
  __unguarded_insertion_sort_aux(__first, __last, __VALUE_TYPE(__first),
                                 __comp);
}
//sort�汾һ�ĸ�������
template <class _RandomAccessIter>
void __final_insertion_sort(_RandomAccessIter __first, 
                            _RandomAccessIter __last) {
  if (__last - __first > __stl_threshold) {//�ж�Ԫ�ظ����Ƿ����16
	  //�������ָ�����Σ�һ�˳���Ϊ16����һ��Ϊʣ��ĳ���
    __insertion_sort(__first, __first + __stl_threshold);
    __unguarded_insertion_sort(__first + __stl_threshold, __last);
  }
  else//��������16��ֱ�ӵ��ò�������
    __insertion_sort(__first, __last);
}

template <class _RandomAccessIter, class _Compare>
void __final_insertion_sort(_RandomAccessIter __first, 
                            _RandomAccessIter __last, _Compare __comp) {
  if (__last - __first > __stl_threshold) {
    __insertion_sort(__first, __first + __stl_threshold, __comp);
    __unguarded_insertion_sort(__first + __stl_threshold, __last, __comp);
  }
  else
    __insertion_sort(__first, __last, __comp);
}
//_lg()�������������Ʒָ�񻯵����
//�ú����ҳ�2^k <= n �����ֵk;
//���磺n=7,��k=2; n=20,��k=4; n=8,��k=3; 
template <class _Size>
inline _Size __lg(_Size __n) {
  _Size __k;
  for (__k = 0; __n != 1; __n >>= 1) ++__k;
  return __k;
}
//sort�汾һ�ĸ�������
//����__depth_limit��ʾ���ķָ����
template <class _RandomAccessIter, class _Tp, class _Size>
void __introsort_loop(_RandomAccessIter __first,
                      _RandomAccessIter __last, _Tp*,
                      _Size __depth_limit)
{
	//__stl_thresholdΪȫ�ֳ�������ֵΪ16
  while (__last - __first > __stl_threshold) {//�����䳤�ȴ���16
    if (__depth_limit == 0) {//��ʾ�ָ��
      partial_sort(__first, __last, __last);//ת�����ö�����heap_sort()
      return;
    }
    --__depth_limit;
	//����ָ��cut����Ŧֵ�ǲ����ס�β�������������м�ֵ
    _RandomAccessIter __cut =
      __unguarded_partition(__first, __last,
                            _Tp(__median(*__first,
                                         *(__first + (__last - __first)/2),
                                         *(__last - 1))));
	//���Ұ벿�ֵݹ�ؽ�������
    __introsort_loop(__cut, __last, (_Tp*) 0, __depth_limit);
    __last = __cut;//����������벿�ֵݹ�ؽ�������
  }
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __introsort_loop(_RandomAccessIter __first,
                      _RandomAccessIter __last, _Tp*,
                      _Size __depth_limit, _Compare __comp)
{
  while (__last - __first > __stl_threshold) {
    if (__depth_limit == 0) {
      partial_sort(__first, __last, __last, __comp);
      return;
    }
    --__depth_limit;
    _RandomAccessIter __cut =
      __unguarded_partition(__first, __last,
                            _Tp(__median(*__first,
                                         *(__first + (__last - __first)/2),
                                         *(__last - 1), __comp)),
       __comp);
    __introsort_loop(__cut, __last, (_Tp*) 0, __depth_limit, __comp);
    __last = __cut;
  }
}
//SGI STL�������㷨�����������������ͱ�����������ʵ�����_RandomAccessIter
/*
�������ܣ�Sorts the elements in the range [first,last) into ascending order.
����ԭ�ͣ�
default (1)	���汾һ����Ĭ�ϵ�operator<
	template <class RandomAccessIterator>
	void sort (RandomAccessIterator first, RandomAccessIterator last);
custom (2)	���汾�����÷º���comp
	template <class RandomAccessIterator, class Compare>
	void sort (RandomAccessIterator first, RandomAccessIterator last, Compare comp);
 */
//�汾һ
template <class _RandomAccessIter>
inline void sort(_RandomAccessIter __first, _RandomAccessIter __last) {
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_REQUIRES(typename iterator_traits<_RandomAccessIter>::value_type,
                 _LessThanComparable);
  //_lg()�������������Ʒָ�񻯵����
  if (__first != __last) {
    __introsort_loop(__first, __last,
                     __VALUE_TYPE(__first),
                     __lg(__last - __first) * 2);
	//���в�������
    __final_insertion_sort(__first, __last);
  }
}
//�汾��
template <class _RandomAccessIter, class _Compare>
inline void sort(_RandomAccessIter __first, _RandomAccessIter __last,
                 _Compare __comp) {
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
       typename iterator_traits<_RandomAccessIter>::value_type,
       typename iterator_traits<_RandomAccessIter>::value_type);
  if (__first != __last) {
    __introsort_loop(__first, __last,
                     __VALUE_TYPE(__first),
                     __lg(__last - __first) * 2,
                     __comp);
    __final_insertion_sort(__first, __last, __comp);
  }
}

// stable_sort() and its auxiliary functions.

template <class _RandomAccessIter>
void __inplace_stable_sort(_RandomAccessIter __first,
                           _RandomAccessIter __last) {
  if (__last - __first < 15) {
    __insertion_sort(__first, __last);
    return;
  }
  _RandomAccessIter __middle = __first + (__last - __first) / 2;
  __inplace_stable_sort(__first, __middle);
  __inplace_stable_sort(__middle, __last);
  __merge_without_buffer(__first, __middle, __last,
                         __middle - __first,
                         __last - __middle);
}

template <class _RandomAccessIter, class _Compare>
void __inplace_stable_sort(_RandomAccessIter __first,
                           _RandomAccessIter __last, _Compare __comp) {
  if (__last - __first < 15) {
    __insertion_sort(__first, __last, __comp);
    return;
  }
  _RandomAccessIter __middle = __first + (__last - __first) / 2;
  __inplace_stable_sort(__first, __middle, __comp);
  __inplace_stable_sort(__middle, __last, __comp);
  __merge_without_buffer(__first, __middle, __last,
                         __middle - __first,
                         __last - __middle,
                         __comp);
}

template <class _RandomAccessIter1, class _RandomAccessIter2,
          class _Distance>
void __merge_sort_loop(_RandomAccessIter1 __first,
                       _RandomAccessIter1 __last, 
                       _RandomAccessIter2 __result, _Distance __step_size) {
  _Distance __two_step = 2 * __step_size;

  while (__last - __first >= __two_step) {
    __result = merge(__first, __first + __step_size,
                     __first + __step_size, __first + __two_step,
                     __result);
    __first += __two_step;
  }

  __step_size = min(_Distance(__last - __first), __step_size);
  merge(__first, __first + __step_size, __first + __step_size, __last,
        __result);
}

template <class _RandomAccessIter1, class _RandomAccessIter2,
          class _Distance, class _Compare>
void __merge_sort_loop(_RandomAccessIter1 __first,
                       _RandomAccessIter1 __last, 
                       _RandomAccessIter2 __result, _Distance __step_size,
                       _Compare __comp) {
  _Distance __two_step = 2 * __step_size;

  while (__last - __first >= __two_step) {
    __result = merge(__first, __first + __step_size,
                     __first + __step_size, __first + __two_step,
                     __result,
                     __comp);
    __first += __two_step;
  }
  __step_size = min(_Distance(__last - __first), __step_size);

  merge(__first, __first + __step_size,
        __first + __step_size, __last,
        __result,
        __comp);
}

const int __stl_chunk_size = 7;
        
template <class _RandomAccessIter, class _Distance>
void __chunk_insertion_sort(_RandomAccessIter __first, 
                            _RandomAccessIter __last, _Distance __chunk_size)
{
  while (__last - __first >= __chunk_size) {
    __insertion_sort(__first, __first + __chunk_size);
    __first += __chunk_size;
  }
  __insertion_sort(__first, __last);
}

template <class _RandomAccessIter, class _Distance, class _Compare>
void __chunk_insertion_sort(_RandomAccessIter __first, 
                            _RandomAccessIter __last,
                            _Distance __chunk_size, _Compare __comp)
{
  while (__last - __first >= __chunk_size) {
    __insertion_sort(__first, __first + __chunk_size, __comp);
    __first += __chunk_size;
  }
  __insertion_sort(__first, __last, __comp);
}

template <class _RandomAccessIter, class _Pointer, class _Distance>
void __merge_sort_with_buffer(_RandomAccessIter __first, 
                              _RandomAccessIter __last,
                              _Pointer __buffer, _Distance*) {
  _Distance __len = __last - __first;
  _Pointer __buffer_last = __buffer + __len;

  _Distance __step_size = __stl_chunk_size;
  __chunk_insertion_sort(__first, __last, __step_size);

  while (__step_size < __len) {
    __merge_sort_loop(__first, __last, __buffer, __step_size);
    __step_size *= 2;
    __merge_sort_loop(__buffer, __buffer_last, __first, __step_size);
    __step_size *= 2;
  }
}

template <class _RandomAccessIter, class _Pointer, class _Distance,
          class _Compare>
void __merge_sort_with_buffer(_RandomAccessIter __first, 
                              _RandomAccessIter __last, _Pointer __buffer,
                              _Distance*, _Compare __comp) {
  _Distance __len = __last - __first;
  _Pointer __buffer_last = __buffer + __len;

  _Distance __step_size = __stl_chunk_size;
  __chunk_insertion_sort(__first, __last, __step_size, __comp);

  while (__step_size < __len) {
    __merge_sort_loop(__first, __last, __buffer, __step_size, __comp);
    __step_size *= 2;
    __merge_sort_loop(__buffer, __buffer_last, __first, __step_size, __comp);
    __step_size *= 2;
  }
}

template <class _RandomAccessIter, class _Pointer, class _Distance>
void __stable_sort_adaptive(_RandomAccessIter __first, 
                            _RandomAccessIter __last, _Pointer __buffer,
                            _Distance __buffer_size) {
  _Distance __len = (__last - __first + 1) / 2;
  _RandomAccessIter __middle = __first + __len;
  if (__len > __buffer_size) {
    __stable_sort_adaptive(__first, __middle, __buffer, __buffer_size);
    __stable_sort_adaptive(__middle, __last, __buffer, __buffer_size);
  }
  else {
    __merge_sort_with_buffer(__first, __middle, __buffer, (_Distance*)0);
    __merge_sort_with_buffer(__middle, __last, __buffer, (_Distance*)0);
  }
  __merge_adaptive(__first, __middle, __last, _Distance(__middle - __first), 
                   _Distance(__last - __middle), __buffer, __buffer_size);
}

template <class _RandomAccessIter, class _Pointer, class _Distance, 
          class _Compare>
void __stable_sort_adaptive(_RandomAccessIter __first, 
                            _RandomAccessIter __last, _Pointer __buffer,
                            _Distance __buffer_size, _Compare __comp) {
  _Distance __len = (__last - __first + 1) / 2;
  _RandomAccessIter __middle = __first + __len;
  if (__len > __buffer_size) {
    __stable_sort_adaptive(__first, __middle, __buffer, __buffer_size, 
                           __comp);
    __stable_sort_adaptive(__middle, __last, __buffer, __buffer_size, 
                           __comp);
  }
  else {
    __merge_sort_with_buffer(__first, __middle, __buffer, (_Distance*)0,
                               __comp);
    __merge_sort_with_buffer(__middle, __last, __buffer, (_Distance*)0,
                               __comp);
  }
  __merge_adaptive(__first, __middle, __last, _Distance(__middle - __first), 
                   _Distance(__last - __middle), __buffer, __buffer_size,
                   __comp);
}

template <class _RandomAccessIter, class _Tp, class _Distance>
inline void __stable_sort_aux(_RandomAccessIter __first,
                              _RandomAccessIter __last, _Tp*, _Distance*) {
  _Temporary_buffer<_RandomAccessIter, _Tp> buf(__first, __last);
  if (buf.begin() == 0)
    __inplace_stable_sort(__first, __last);
  else 
    __stable_sort_adaptive(__first, __last, buf.begin(),
                           _Distance(buf.size()));
}

template <class _RandomAccessIter, class _Tp, class _Distance, class _Compare>
inline void __stable_sort_aux(_RandomAccessIter __first,
                              _RandomAccessIter __last, _Tp*, _Distance*,
                              _Compare __comp) {
  _Temporary_buffer<_RandomAccessIter, _Tp> buf(__first, __last);
  if (buf.begin() == 0)
    __inplace_stable_sort(__first, __last, __comp);
  else 
    __stable_sort_adaptive(__first, __last, buf.begin(),
                           _Distance(buf.size()),
                           __comp);
}

template <class _RandomAccessIter>
inline void stable_sort(_RandomAccessIter __first,
                        _RandomAccessIter __last) {
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_REQUIRES(typename iterator_traits<_RandomAccessIter>::value_type,
                 _LessThanComparable);
  __stable_sort_aux(__first, __last,
                    __VALUE_TYPE(__first),
                    __DISTANCE_TYPE(__first));
}

template <class _RandomAccessIter, class _Compare>
inline void stable_sort(_RandomAccessIter __first,
                        _RandomAccessIter __last, _Compare __comp) {
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
       typename iterator_traits<_RandomAccessIter>::value_type,
       typename iterator_traits<_RandomAccessIter>::value_type);
  __stable_sort_aux(__first, __last,
                    __VALUE_TYPE(__first),
                    __DISTANCE_TYPE(__first), 
                    __comp);
}

// partial_sort, partial_sort_copy, and auxiliary functions.
//���°�������[first,last)��ʹ����ǰ�벿��middle-first����СԪ���Ե���˳�����򣬲���������[first,middle)
//����last-middle��Ԫ�ز�ָ���κ����򣬲���������[middle,last)
//ע�⣺������middle����[first,last)��Χ֮��

/*
�������ܣ�Rearranges the elements in the range [first,last), 
in such a way that the elements before middle are the smallest elements in the entire range 
and are sorted in ascending order, while the remaining elements are left without any specific order.

����ԭ�ͣ�
default (1)	�汾һ operator< 
	template <class RandomAccessIterator>
	void partial_sort (RandomAccessIterator first, RandomAccessIterator middle,
                     RandomAccessIterator last);
custom (2) �汾�� comp	
	template <class RandomAccessIterator, class Compare>
	void partial_sort (RandomAccessIterator first, RandomAccessIterator middle,
                     RandomAccessIterator last, Compare comp);
*/

template <class _RandomAccessIter, class _Tp>
void __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle,
                    _RandomAccessIter __last, _Tp*) {
		//����heap��֪ʶ����SGI STL�У��ǲ�������
		//��[first,middle)�����Ԫ�ش���������
		//�ٸ������ѵ����ʣ�һ��һ�������ѣ������䱣�棬��������
  make_heap(__first, __middle);//�������ѣ�������<stl_heap.h>�ļ�
  //��������������[first,last)�ҳ�middle-first����СԪ��
  //������ǽ���벿��[middle,last)��Ԫ�����������ѵĸ��ڵ�Ԫ��(���ѵ����Ԫ��)�Ƚ�
  //��С�ڶѵ����Ԫ�أ�����ѵ����Ԫ�ؽ������������ѣ�ʹ�����γ�Ϊ����
  //����С�ڶѵ����Ԫ�أ������κβ���
  for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
    if (*__i < *__first) 
      __pop_heap(__first, __middle, __i, _Tp(*__i),
                 __DISTANCE_TYPE(__first));
  sort_heap(__first, __middle);//�����ѽ��ж�����
}
//�汾һ
template <class _RandomAccessIter>
inline void partial_sort(_RandomAccessIter __first,
                         _RandomAccessIter __middle,
                         _RandomAccessIter __last) {
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_REQUIRES(typename iterator_traits<_RandomAccessIter>::value_type,
                 _LessThanComparable);
  __partial_sort(__first, __middle, __last, __VALUE_TYPE(__first));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle,
                    _RandomAccessIter __last, _Tp*, _Compare __comp) {
  make_heap(__first, __middle, __comp);
  for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
    if (__comp(*__i, *__first))
      __pop_heap(__first, __middle, __i, _Tp(*__i), __comp,
                 __DISTANCE_TYPE(__first));
  sort_heap(__first, __middle, __comp);
}
//�汾��
template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first,
                         _RandomAccessIter __middle,
                         _RandomAccessIter __last, _Compare __comp) {
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool, 
      typename iterator_traits<_RandomAccessIter>::value_type,
      typename iterator_traits<_RandomAccessIter>::value_type);
  __partial_sort(__first, __middle, __last, __VALUE_TYPE(__first), __comp);
}
//partial_sort_copy��partial_sort��ʵ�ֻ�������ͬ��ֻ��partial_sort_copy��Ԫ������������result��ʼ��������
template <class _InputIter, class _RandomAccessIter, class _Distance,
          class _Tp>
_RandomAccessIter __partial_sort_copy(_InputIter __first,
                                      _InputIter __last,
                                      _RandomAccessIter __result_first,
                                      _RandomAccessIter __result_last, 
                                      _Distance*, _Tp*) {
  if (__result_first == __result_last) return __result_last;
  _RandomAccessIter __result_real_last = __result_first;
  while(__first != __last && __result_real_last != __result_last) {
    *__result_real_last = *__first;
    ++__result_real_last;
    ++__first;
  }
  make_heap(__result_first, __result_real_last);
  while (__first != __last) {
    if (*__first < *__result_first) 
      __adjust_heap(__result_first, _Distance(0),
                    _Distance(__result_real_last - __result_first),
                    _Tp(*__first));
    ++__first;
  }
  sort_heap(__result_first, __result_real_last);
  return __result_real_last;
}

template <class _InputIter, class _RandomAccessIter>
inline _RandomAccessIter
partial_sort_copy(_InputIter __first, _InputIter __last,
                  _RandomAccessIter __result_first,
                  _RandomAccessIter __result_last) {
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_CONVERTIBLE(typename iterator_traits<_InputIter>::value_type,
                    typename iterator_traits<_RandomAccessIter>::value_type);
  __STL_REQUIRES(typename iterator_traits<_RandomAccessIter>::value_type,
                 _LessThanComparable);
  __STL_REQUIRES(typename iterator_traits<_InputIter>::value_type,
                 _LessThanComparable);
  return __partial_sort_copy(__first, __last, __result_first, __result_last, 
                             __DISTANCE_TYPE(__result_first),
                             __VALUE_TYPE(__first));
}

template <class _InputIter, class _RandomAccessIter, class _Compare,
          class _Distance, class _Tp>
_RandomAccessIter __partial_sort_copy(_InputIter __first,
                                         _InputIter __last,
                                         _RandomAccessIter __result_first,
                                         _RandomAccessIter __result_last,
                                         _Compare __comp, _Distance*, _Tp*) {
  if (__result_first == __result_last) return __result_last;
  _RandomAccessIter __result_real_last = __result_first;
  while(__first != __last && __result_real_last != __result_last) {
    *__result_real_last = *__first;
    ++__result_real_last;
    ++__first;
  }
  make_heap(__result_first, __result_real_last, __comp);
  while (__first != __last) {
    if (__comp(*__first, *__result_first))
      __adjust_heap(__result_first, _Distance(0),
                    _Distance(__result_real_last - __result_first),
                    _Tp(*__first),
                    __comp);
    ++__first;
  }
  sort_heap(__result_first, __result_real_last, __comp);
  return __result_real_last;
}

template <class _InputIter, class _RandomAccessIter, class _Compare>
inline _RandomAccessIter
partial_sort_copy(_InputIter __first, _InputIter __last,
                  _RandomAccessIter __result_first,
                  _RandomAccessIter __result_last, _Compare __comp) {
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_CONVERTIBLE(typename iterator_traits<_InputIter>::value_type,
                    typename iterator_traits<_RandomAccessIter>::value_type);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
     typename iterator_traits<_RandomAccessIter>::value_type,
     typename iterator_traits<_RandomAccessIter>::value_type);  
  return __partial_sort_copy(__first, __last, __result_first, __result_last,
                             __comp,
                             __DISTANCE_TYPE(__result_first),
                             __VALUE_TYPE(__first));
}

// nth_element() and its auxiliary functions.  
//nth_element�汾һ��������
template <class _RandomAccessIter, class _Tp>
void __nth_element(_RandomAccessIter __first, _RandomAccessIter __nth,
                   _RandomAccessIter __last, _Tp*) {
  while (__last - __first > 3) {//���䳤�ȴ���3
	  //��ȡ�ָ��cut
    _RandomAccessIter __cut =
      __unguarded_partition(__first, __last,
                            _Tp(__median(*__first,
                                         *(__first + (__last - __first)/2),
                                         *(__last - 1))));
    if (__cut <= __nth)//���ָ��С��ָ��λ�ã���nthλ�����Ұ��
      __first = __cut;//�ٶ��Ұ�ν��зָ�
    else //���򣬶����ν��зָ�
      __last = __cut;
  }
  __insertion_sort(__first, __last);
}
//������������[first,last)��ʹ������nth��ָ��Ԫ�أ��롰����[first,last)�������������ͬһλ�õ�Ԫ�ء�ֵͬ.
//���⣬���뱣֤[nth,last)�ڵ�����Ԫ�ز�С��[first,nth)�ڵ�Ԫ�أ����Ƕ�������[first,nth)������[nth,last)�ڵ�Ԫ�ص�����˳����ȷ��.

/*
�������ܣ�Rearranges the elements in the range [first,last), 
in such a way that the element at the nth position is the element that would be in that position in a sorted sequence.
����ԭ�ͣ�
default (1)	
	template <class RandomAccessIterator>
	void nth_element (RandomAccessIterator first, RandomAccessIterator nth,
                    RandomAccessIterator last);
custom (2)	
	template <class RandomAccessIterator, class Compare>
	void nth_element (RandomAccessIterator first, RandomAccessIterator nth,
                    RandomAccessIterator last, Compare comp);
*/
//nth_element�汾һ
template <class _RandomAccessIter>
inline void nth_element(_RandomAccessIter __first, _RandomAccessIter __nth,
                        _RandomAccessIter __last) {
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_REQUIRES(typename iterator_traits<_RandomAccessIter>::value_type,
                 _LessThanComparable);
  __nth_element(__first, __nth, __last, __VALUE_TYPE(__first));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __nth_element(_RandomAccessIter __first, _RandomAccessIter __nth,
                   _RandomAccessIter __last, _Tp*, _Compare __comp) {
  while (__last - __first > 3) {
    _RandomAccessIter __cut =
      __unguarded_partition(__first, __last,
                            _Tp(__median(*__first,
                                         *(__first + (__last - __first)/2), 
                                         *(__last - 1),
                                         __comp)),
                            __comp);
    if (__cut <= __nth)
      __first = __cut;
    else 
      __last = __cut;
  }
  __insertion_sort(__first, __last, __comp);
}

template <class _RandomAccessIter, class _Compare>
inline void nth_element(_RandomAccessIter __first, _RandomAccessIter __nth,
                        _RandomAccessIter __last, _Compare __comp) {
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
     typename iterator_traits<_RandomAccessIter>::value_type,
     typename iterator_traits<_RandomAccessIter>::value_type);
  __nth_element(__first, __nth, __last, __VALUE_TYPE(__first), __comp);
}


// Binary search (lower_bound, upper_bound, equal_range, binary_search).

template <class _ForwardIter, class _Tp, class _Distance>
_ForwardIter __lower_bound(_ForwardIter __first, _ForwardIter __last,
                           const _Tp& __val, _Distance*) 
{
  _Distance __len = 0;
  distance(__first, __last, __len);//��ȡ��������ĳ���len
  _Distance __half;
  _ForwardIter __middle;//����������м������

  while (__len > 0) {//�����䲻Ϊ�գ���������[first,last)��ʼ����valueֵ
    __half = __len >> 1;//������һλ���൱�ڳ���2����ȡ������м�ֵ
    __middle = __first;//middle��ʼ��Ϊ�������ʼλ��
    advance(__middle, __half);//middle�����halfλ����ʱmiddleΪ������м�ֵ
    if (*__middle < __val) {//��valueֵ���м�ֵ�Ƚϣ����Ƕ��ֲ���,���м�ֵС��value������������Ұ벿��
		//����������firstָ��middle����һ��λ��
      __first = __middle;
      ++__first;
      __len = __len - __half - 1;//������������ĳ���
    }
    else
      __len = __half;//���������벿��
  }
  return __first;
}
//������������[first,last)����valueֵ
//�������������value��ȵ�Ԫ�أ��򷵻�ָ���һ����value��ȵĵ�����
//�������䲻������value��ȵ�Ԫ�أ��򷵻�ָ���һ����С��valueֵ�ĵ�����
//����������κ�Ԫ�ض���valueֵС���򷵻�last
/*
�������ܣ�Returns an iterator pointing to the first element in the range [first,last) which does not compare less than val.
����ԭ�ͣ�
default (1)	���汾һ����operator<�Ƚ�
	template <class ForwardIterator, class T>
	ForwardIterator lower_bound (ForwardIterator first, ForwardIterator last,
                               const T& val);
custom (2)	���汾�����÷º���comp�ȽϹ���
	template <class ForwardIterator, class T, class Compare>
	ForwardIterator lower_bound (ForwardIterator first, ForwardIterator last,
                               const T& val, Compare comp);
*/
//�汾һ
template <class _ForwardIter, class _Tp>
inline _ForwardIter lower_bound(_ForwardIter __first, _ForwardIter __last,
				const _Tp& __val) {
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES_SAME_TYPE(_Tp,
      typename iterator_traits<_ForwardIter>::value_type);
  __STL_REQUIRES(_Tp, _LessThanComparable);
  return __lower_bound(__first, __last, __val,
                       __DISTANCE_TYPE(__first));
}

template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __lower_bound(_ForwardIter __first, _ForwardIter __last,
                              const _Tp& __val, _Compare __comp, _Distance*)
{
  _Distance __len = 0;
  distance(__first, __last, __len);//��ȡ��������ĳ���len
  _Distance __half;
  _ForwardIter __middle;//����������м������

  while (__len > 0) {//�����䲻Ϊ�գ���������[first,last)��ʼ����valueֵ
    __half = __len >> 1;//������һλ���൱�ڳ���2����ȡ������м�ֵ
    __middle = __first;//middle��ʼ��Ϊ�������ʼλ��
    advance(__middle, __half);//middle�����halfλ����ʱmiddleΪ������м�ֵ
    if (__comp(*__middle, __val)) {//��comp�ж�Ϊtrue����������Ұ벿�ֲ���
		//����������firstָ��middle����һ��λ��
      __first = __middle;
      ++__first;
      __len = __len - __half - 1;//������������ĳ���
    }
    else
      __len = __half;//���������벿��
  }
  return __first;
}
//�汾����
template <class _ForwardIter, class _Tp, class _Compare>
inline _ForwardIter lower_bound(_ForwardIter __first, _ForwardIter __last,
                                const _Tp& __val, _Compare __comp) {
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES_SAME_TYPE(_Tp,
      typename iterator_traits<_ForwardIter>::value_type);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool, _Tp, _Tp);
  return __lower_bound(__first, __last, __val, __comp,
                       __DISTANCE_TYPE(__first));
}

template <class _ForwardIter, class _Tp, class _Distance>
_ForwardIter __upper_bound(_ForwardIter __first, _ForwardIter __last,
                           const _Tp& __val, _Distance*)
{
  _Distance __len = 0;
  distance(__first, __last, __len);//��ȡ��������ĳ���len
  _Distance __half;
  _ForwardIter __middle;//����������м������

  while (__len > 0) {//�����䲻Ϊ�գ���������[first,last)��ʼ����valueֵ
    __half = __len >> 1;//������һλ���൱�ڳ���2����ȡ������м�ֵ
    __middle = __first;//middle��ʼ��Ϊ�������ʼλ��
    advance(__middle, __half);//middle�����halfλ����ʱmiddleΪ������м�ֵ
    if (__val < *__middle)//��valueС���м�Ԫ��ֵ
      __len = __half;//������벿��
    else {
		//����������firstָ��middle����һ��λ��
      __first = __middle;
      ++__first;
      __len = __len - __half - 1;//����len��ֵ
    }
  }
  return __first;
}
//������������[first,last)����valueֵ
//���ش���valueֵ�ĵ�һ��Ԫ�صĵ�����
/*
�������ܣ�Returns an iterator pointing to the first element in the range [first,last) which compares greater than val.
����ԭ�ͣ�
default (1)	���汾һ����operator<�Ƚ�
	template <class ForwardIterator, class T>
	ForwardIterator upper_bound (ForwardIterator first, ForwardIterator last,
                               const T& val);
custom (2)	���汾�����÷º���comp�ȽϹ���
	template <class ForwardIterator, class T, class Compare>
	ForwardIterator upper_bound (ForwardIterator first, ForwardIterator last,
                               const T& val, Compare comp);
*/
//�汾һ
template <class _ForwardIter, class _Tp>
inline _ForwardIter upper_bound(_ForwardIter __first, _ForwardIter __last,
                                const _Tp& __val) {
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES_SAME_TYPE(_Tp,
      typename iterator_traits<_ForwardIter>::value_type);
  __STL_REQUIRES(_Tp, _LessThanComparable);
  return __upper_bound(__first, __last, __val,
                       __DISTANCE_TYPE(__first));
}

template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __upper_bound(_ForwardIter __first, _ForwardIter __last,
                           const _Tp& __val, _Compare __comp, _Distance*)
{
  _Distance __len = 0;
  distance(__first, __last, __len);
  _Distance __half;
  _ForwardIter __middle;

  while (__len > 0) {
    __half = __len >> 1;
    __middle = __first;
    advance(__middle, __half);
    if (__comp(__val, *__middle))
      __len = __half;
    else {
      __first = __middle;
      ++__first;
      __len = __len - __half - 1;
    }
  }
  return __first;
}
//�汾��
template <class _ForwardIter, class _Tp, class _Compare>
inline _ForwardIter upper_bound(_ForwardIter __first, _ForwardIter __last,
                                const _Tp& __val, _Compare __comp) {
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES_SAME_TYPE(_Tp,
      typename iterator_traits<_ForwardIter>::value_type);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool, _Tp, _Tp);
  return __upper_bound(__first, __last, __val, __comp,
                       __DISTANCE_TYPE(__first));
}
//��������
/*
	#include <iostream>     // std::cout
	#include <algorithm>    // std::lower_bound, std::upper_bound, std::sort
	#include <vector>       // std::vector

	int main () {
	  int myints[] = {10,20,30,30,20,10,10,20};
	  std::vector<int> v(myints,myints+8);           // 10 20 30 30 20 10 10 20

	  std::sort (v.begin(), v.end());                // 10 10 10 20 20 20 30 30

	  std::vector<int>::iterator low,up;
	  low=std::lower_bound (v.begin(), v.end(), 20); //          ^
	  up= std::upper_bound (v.begin(), v.end(), 20); //                   ^

	  std::cout << "lower_bound at position " << (low- v.begin()) << '\n';
	  std::cout << "upper_bound at position " << (up - v.begin()) << '\n';

	  return 0;
	}
	Output:
	lower_bound at position 3
	upper_bound at position 6
*/
template <class _ForwardIter, class _Tp, class _Distance>
pair<_ForwardIter, _ForwardIter>
__equal_range(_ForwardIter __first, _ForwardIter __last, const _Tp& __val,
              _Distance*)
{
  _Distance __len = 0;
  distance(__first, __last, __len);//��������ĳ���len
  _Distance __half;
  _ForwardIter __middle, __left, __right;

  while (__len > 0) {//������ǿ�
    __half = __len >> 1;//len����һλ������ڳ���2����halfΪ����ĳ��ȵ�һ��
    __middle = __first;//��ʼ��middle��ֵ
    advance(__middle, __half);//ǰ��middleλ�ã�ʹ��ָ�������м�λ��
    if (*__middle < __val) {//��ָ��Ԫ��value�����м�Ԫ��ֵ�������Ұ벿�ּ�������
		//��������ʹfirstָ��middle����һ��λ�ã����Ұ��������ʼλ��
      __first = __middle;
      ++__first;
      __len = __len - __half - 1;//���´���������ĳ���
    }
    else if (__val < *__middle)//��ָ��Ԫ��valueС���м�Ԫ��ֵ��������벿�ּ�������
      __len = __half;//���´���������ĳ���
    else {//��ָ��Ԫ��value�����м�Ԫ��ֵ
		//��ǰ�벿����lower_boundλ��
      __left = lower_bound(__first, __middle, __val);
      advance(__first, __len);
	  //�ں�벿����upper_bound
      __right = upper_bound(++__middle, __first, __val);
      return pair<_ForwardIter, _ForwardIter>(__left, __right);//����pair���󣬵�һ��������Ϊleft���ڶ���������Ϊright
    }
  }
  return pair<_ForwardIter, _ForwardIter>(__first, __first);
}
//����������value��ȵ������ظ�Ԫ�ص���ʼλ�úͽ���λ��
//ע�⣺[first,last)��������˼�뻹�ǲ��ö��ֲ��ҷ�
//ͬ��Ҳ�������汾
/*
�������ܣ�Returns the bounds of the subrange that includes all the elements of the range [first,last) with values equivalent to val.
����ԭ�ͣ�
default (1)	���汾һĬ��operator<
	template <class ForwardIterator, class T>
		pair<ForwardIterator,ForwardIterator>
    equal_range (ForwardIterator first, ForwardIterator last, const T& val);
custom (2)	���汾�����÷º���comp
	template <class ForwardIterator, class T, class Compare>
		pair<ForwardIterator,ForwardIterator>
    equal_range (ForwardIterator first, ForwardIterator last, const T& val,
                  Compare comp);
*/
//�汾һ
template <class _ForwardIter, class _Tp>
inline pair<_ForwardIter, _ForwardIter>
equal_range(_ForwardIter __first, _ForwardIter __last, const _Tp& __val) {
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES_SAME_TYPE(_Tp, 
       typename iterator_traits<_ForwardIter>::value_type);
  __STL_REQUIRES(_Tp, _LessThanComparable);
  return __equal_range(__first, __last, __val,
                       __DISTANCE_TYPE(__first));
}

template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
pair<_ForwardIter, _ForwardIter>
__equal_range(_ForwardIter __first, _ForwardIter __last, const _Tp& __val,
              _Compare __comp, _Distance*)
{
  _Distance __len = 0;
  distance(__first, __last, __len);
  _Distance __half;
  _ForwardIter __middle, __left, __right;

  while (__len > 0) {
    __half = __len >> 1;
    __middle = __first;
    advance(__middle, __half);
    if (__comp(*__middle, __val)) {
      __first = __middle;
      ++__first;
      __len = __len - __half - 1;
    }
    else if (__comp(__val, *__middle))
      __len = __half;
    else {
      __left = lower_bound(__first, __middle, __val, __comp);
      advance(__first, __len);
      __right = upper_bound(++__middle, __first, __val, __comp);
      return pair<_ForwardIter, _ForwardIter>(__left, __right);
    }
  }
  return pair<_ForwardIter, _ForwardIter>(__first, __first);
}           
//�汾��
template <class _ForwardIter, class _Tp, class _Compare>
inline pair<_ForwardIter, _ForwardIter>
equal_range(_ForwardIter __first, _ForwardIter __last, const _Tp& __val,
            _Compare __comp) {
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES_SAME_TYPE(_Tp, 
       typename iterator_traits<_ForwardIter>::value_type);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool, _Tp, _Tp);
  return __equal_range(__first, __last, __val, __comp,
                       __DISTANCE_TYPE(__first));
} 
//equal_range����������
/*
	#include <iostream>     // std::cout
	#include <algorithm>    // std::equal_range, std::sort
	#include <vector>       // std::vector

	bool mygreater (int i,int j) { return (i>j); }

	int main () {
	  int myints[] = {10,20,30,30,20,10,10,20};
	  std::vector<int> v(myints,myints+8);                         // 10 20 30 30 20 10 10 20
	  std::pair<std::vector<int>::iterator,std::vector<int>::iterator> bounds;

	  // using default comparison:
	  std::sort (v.begin(), v.end());                              // 10 10 10 20 20 20 30 30
	  bounds=std::equal_range (v.begin(), v.end(), 20);            //          ^        ^
  
	  std::cout << "bounds at positions " << (bounds.first - v.begin());
	  std::cout << " and " << (bounds.second - v.begin()) << '\n';
  
	  // using "mygreater" as comp:
	  std::sort (v.begin(), v.end(), mygreater);                   // 30 30 20 20 20 10 10 10
	  bounds=std::equal_range (v.begin(), v.end(), 20, mygreater); //       ^        ^

	  std::cout << "bounds at positions " << (bounds.first - v.begin());
	  std::cout << " and " << (bounds.second - v.begin()) << '\n';

	  return 0;
	}
	Output��
	bounds at positions 3 and 6
	bounds at positions 2 and 5 
*/

//���ֲ��ҷ�
//ע�⣺[first,last)��������
//ͬ��Ҳ�������汾
/*
�������ܣ�Returns true if any element in the range [first,last) is equivalent to val, and false otherwise.
����ԭ�ͣ�
default (1)	���汾һĬ��operator<
	template <class ForwardIterator, class T>
	bool binary_search (ForwardIterator first, ForwardIterator last,
                      const T& val);
custom (2)	���汾�����÷º���comp
	template <class ForwardIterator, class T, class Compare>
	bool binary_search (ForwardIterator first, ForwardIterator last,
                      const T& val, Compare comp);
*/
template <class _ForwardIter, class _Tp>
bool binary_search(_ForwardIter __first, _ForwardIter __last,
                   const _Tp& __val) {
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES_SAME_TYPE(_Tp,
        typename iterator_traits<_ForwardIter>::value_type);
  __STL_REQUIRES(_Tp, _LessThanComparable);
  _ForwardIter __i = lower_bound(__first, __last, __val);//���ö��ֲ��Һ����������ز�С��valueֵ�ĵ�һ��������λ��i
  return __i != __last && !(__val < *__i);
}


template <class _ForwardIter, class _Tp, class _Compare>
bool binary_search(_ForwardIter __first, _ForwardIter __last,
                   const _Tp& __val,
                   _Compare __comp) {
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES_SAME_TYPE(_Tp,
        typename iterator_traits<_ForwardIter>::value_type);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool, _Tp, _Tp);
  _ForwardIter __i = lower_bound(__first, __last, __val, __comp);//���ö��ֲ��Һ����������ز�С��valueֵ�ĵ�һ��������λ��i
  return __i != __last && !__comp(__val, *__i);
}

// merge, with and without an explicitly supplied comparison function.
//�����������������[first1,last1)������[first2,last2)�ϲ�
/*
�������ܣ�Combines the elements in the sorted ranges [first1,last1) and [first2,last2), 
into a new range beginning at result with all its elements sorted.

����ԭ�ͣ�
default (1)	���汾һ
	template <class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator merge (InputIterator1 first1, InputIterator1 last1,
                        InputIterator2 first2, InputIterator2 last2,
                        OutputIterator result);
custom (2)	���汾��
	template <class InputIterator1, class InputIterator2,
          class OutputIterator, class Compare>
	OutputIterator merge (InputIterator1 first1, InputIterator1 last1,
                        InputIterator2 first2, InputIterator2 last2,
                        OutputIterator result, Compare comp);
*/
//�汾һ��
template <class _InputIter1, class _InputIter2, class _OutputIter>
_OutputIter merge(_InputIter1 __first1, _InputIter1 __last1,
                  _InputIter2 __first2, _InputIter2 __last2,
                  _OutputIter __result) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_SAME_TYPE(
          typename iterator_traits<_InputIter1>::value_type,
          typename iterator_traits<_InputIter2>::value_type);
  __STL_REQUIRES(typename iterator_traits<_InputIter1>::value_type,
                 _LessThanComparable);
  //�������ж���δ����β�ˣ���ִ��whileѭ��
  /*
  ���1�������ж�Ԫ�ؽ�С,���¼��Ŀ���������ƶ����ж��ĵ���������������һ�ĵ���������.
  ���2��������һԪ�ؽ�С�����,���¼��Ŀ���������ƶ�����һ�ĵ��������������ж��ĵ���������.
  ��󣺰�ʣ��Ԫ�ص����и��Ƶ�Ŀ����
  */
  while (__first1 != __last1 && __first2 != __last2) {
	  //���1
    if (*__first2 < *__first1) {//�����ж�Ԫ�ؽ�С
      *__result = *__first2;//��Ԫ�ؼ�¼��Ŀ����
      ++__first2;//�ƶ�������
    }
	//���2
    else {//������һԪ�ؽ�С�����
      *__result = *__first1;//��Ԫ�ؼ�¼��Ŀ����
      ++__first1;//�ƶ�������
    }
    ++__result;//����Ŀ����λ�ã��Ա��´μ�¼����
  }
  //�������е���β�ˣ����û����β�˵�����ʣ��Ԫ�ظ��Ƶ�Ŀ����
  //��ʱ������[first1,last1)������[first2,last2)����һ���ض�Ϊ��
  return copy(__first2, __last2, copy(__first1, __last1, __result));
}
//�汾��
template <class _InputIter1, class _InputIter2, class _OutputIter,
          class _Compare>
_OutputIter merge(_InputIter1 __first1, _InputIter1 __last1,
                  _InputIter2 __first2, _InputIter2 __last2,
                  _OutputIter __result, _Compare __comp) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES_SAME_TYPE(
          typename iterator_traits<_InputIter1>::value_type,
          typename iterator_traits<_InputIter2>::value_type);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
          typename iterator_traits<_InputIter1>::value_type,
          typename iterator_traits<_InputIter1>::value_type);
  while (__first1 != __last1 && __first2 != __last2) {
    if (__comp(*__first2, *__first1)) {
      *__result = *__first2;
      ++__first2;
    }
    else {
      *__result = *__first1;
      ++__first1;
    }
    ++__result;
  }
  return copy(__first2, __last2, copy(__first1, __last1, __result));
}
//merge����������
/*
	#include <iostream>     // std::cout
	#include <algorithm>    // std::merge, std::sort
	#include <vector>       // std::vector

	int main () {
	  int first[] = {5,10,15,20,25};
	  int second[] = {50,40,30,20,10};
	  std::vector<int> v(10);

	  std::sort (first,first+5);
	  std::sort (second,second+5);
	  std::merge (first,first+5,second,second+5,v.begin());

	  std::cout << "The resulting vector contains:";
	  for (std::vector<int>::iterator it=v.begin(); it!=v.end(); ++it)
		std::cout << ' ' << *it;
	  std::cout << '\n';

	  return 0;
	}
	Output:
	The resulting vector contains: 5 10 10 15 20 20 25 30 40 50
*/

// inplace_merge and its auxiliary functions. 
//�汾һ�ĸ����������޻������Ĳ���
template <class _BidirectionalIter, class _Distance>
void __merge_without_buffer(_BidirectionalIter __first,
                            _BidirectionalIter __middle,
                            _BidirectionalIter __last,
                            _Distance __len1, _Distance __len2) {
  if (__len1 == 0 || __len2 == 0)
    return;
  if (__len1 + __len2 == 2) {
    if (*__middle < *__first)
      iter_swap(__first, __middle);
    return;
  }
  _BidirectionalIter __first_cut = __first;
  _BidirectionalIter __second_cut = __middle;
  _Distance __len11 = 0;
  _Distance __len22 = 0;
  if (__len1 > __len2) {
    __len11 = __len1 / 2;
    advance(__first_cut, __len11);
    __second_cut = lower_bound(__middle, __last, *__first_cut);
    distance(__middle, __second_cut, __len22);
  }
  else {
    __len22 = __len2 / 2;
    advance(__second_cut, __len22);
    __first_cut = upper_bound(__first, __middle, *__second_cut);
    distance(__first, __first_cut, __len11);
  }
  _BidirectionalIter __new_middle
    = rotate(__first_cut, __middle, __second_cut);
  __merge_without_buffer(__first, __first_cut, __new_middle,
                         __len11, __len22);
  __merge_without_buffer(__new_middle, __second_cut, __last, __len1 - __len11,
                         __len2 - __len22);
}

template <class _BidirectionalIter, class _Distance, class _Compare>
void __merge_without_buffer(_BidirectionalIter __first,
                            _BidirectionalIter __middle,
                            _BidirectionalIter __last,
                            _Distance __len1, _Distance __len2,
                            _Compare __comp) {
  if (__len1 == 0 || __len2 == 0)
    return;
  if (__len1 + __len2 == 2) {
    if (__comp(*__middle, *__first))
      iter_swap(__first, __middle);
    return;
  }
  _BidirectionalIter __first_cut = __first;
  _BidirectionalIter __second_cut = __middle;
  _Distance __len11 = 0;
  _Distance __len22 = 0;
  if (__len1 > __len2) {
    __len11 = __len1 / 2;
    advance(__first_cut, __len11);
    __second_cut = lower_bound(__middle, __last, *__first_cut, __comp);
    distance(__middle, __second_cut, __len22);
  }
  else {
    __len22 = __len2 / 2;
    advance(__second_cut, __len22);
    __first_cut = upper_bound(__first, __middle, *__second_cut, __comp);
    distance(__first, __first_cut, __len11);
  }
  _BidirectionalIter __new_middle
    = rotate(__first_cut, __middle, __second_cut);
  __merge_without_buffer(__first, __first_cut, __new_middle, __len11, __len22,
                         __comp);
  __merge_without_buffer(__new_middle, __second_cut, __last, __len1 - __len11,
                         __len2 - __len22, __comp);
}
//�汾һ�ĸ����������л������Ĳ���
template <class _BidirectionalIter1, class _BidirectionalIter2,
          class _Distance>
_BidirectionalIter1 __rotate_adaptive(_BidirectionalIter1 __first,
                                      _BidirectionalIter1 __middle,
                                      _BidirectionalIter1 __last,
                                      _Distance __len1, _Distance __len2,
                                      _BidirectionalIter2 __buffer,
                                      _Distance __buffer_size) {
  _BidirectionalIter2 __buffer_end;
  if (__len1 > __len2 && __len2 <= __buffer_size) {//�������㹻�������ж�
    __buffer_end = copy(__middle, __last, __buffer);
    copy_backward(__first, __middle, __last);
    return copy(__buffer, __buffer_end, __first);
  }
  else if (__len1 <= __buffer_size) {//�������㹻��������һ
    __buffer_end = copy(__first, __middle, __buffer);
    copy(__middle, __last, __first);
    return copy_backward(__buffer, __buffer_end, __last);
  }
  else//����������Ȼ�����������STL�㷨rotate����ʹ�û�����
    return rotate(__first, __middle, __last);
}

template <class _BidirectionalIter1, class _BidirectionalIter2,
          class _BidirectionalIter3>
_BidirectionalIter3 __merge_backward(_BidirectionalIter1 __first1,
                                     _BidirectionalIter1 __last1,
                                     _BidirectionalIter2 __first2,
                                     _BidirectionalIter2 __last2,
                                     _BidirectionalIter3 __result) {
  if (__first1 == __last1)
    return copy_backward(__first2, __last2, __result);
  if (__first2 == __last2)
    return copy_backward(__first1, __last1, __result);
  --__last1;
  --__last2;
  while (true) {
    if (*__last2 < *__last1) {
      *--__result = *__last1;
      if (__first1 == __last1)
        return copy_backward(__first2, ++__last2, __result);
      --__last1;
    }
    else {
      *--__result = *__last2;
      if (__first2 == __last2)
        return copy_backward(__first1, ++__last1, __result);
      --__last2;
    }
  }
}

template <class _BidirectionalIter1, class _BidirectionalIter2,
          class _BidirectionalIter3, class _Compare>
_BidirectionalIter3 __merge_backward(_BidirectionalIter1 __first1,
                                     _BidirectionalIter1 __last1,
                                     _BidirectionalIter2 __first2,
                                     _BidirectionalIter2 __last2,
                                     _BidirectionalIter3 __result,
                                     _Compare __comp) {
  if (__first1 == __last1)
    return copy_backward(__first2, __last2, __result);
  if (__first2 == __last2)
    return copy_backward(__first1, __last1, __result);
  --__last1;
  --__last2;
  while (true) {
    if (__comp(*__last2, *__last1)) {
      *--__result = *__last1;
      if (__first1 == __last1)
        return copy_backward(__first2, ++__last2, __result);
      --__last1;
    }
    else {
      *--__result = *__last2;
      if (__first2 == __last2)
        return copy_backward(__first1, ++__last1, __result);
      --__last2;
    }
  }
}
//�汾һ�ĸ����������л������Ĳ���
template <class _BidirectionalIter, class _Distance, class _Pointer>
void __merge_adaptive(_BidirectionalIter __first,
                      _BidirectionalIter __middle, 
                      _BidirectionalIter __last,
                      _Distance __len1, _Distance __len2,
                      _Pointer __buffer, _Distance __buffer_size) {
  if (__len1 <= __len2 && __len1 <= __buffer_size) {
	  //case1��������һ���ڻ�����
    _Pointer __buffer_end = copy(__first, __middle, __buffer);
	//ֱ�ӵ��ù鲢����merge
    merge(__buffer, __buffer_end, __middle, __last, __first);
  }
  else if (__len2 <= __buffer_size) {
	  //case2�������ж����ڻ�����
    _Pointer __buffer_end = copy(__middle, __last, __buffer);
    __merge_backward(__first, __middle, __buffer, __buffer_end, __last);
  }
  else {//case3����������������κ�һ������
    _BidirectionalIter __first_cut = __first;
    _BidirectionalIter __second_cut = __middle;
    _Distance __len11 = 0;
    _Distance __len22 = 0;
    if (__len1 > __len2) {//������һ�Ƚϳ�
      __len11 = __len1 / 2;//��������һ��һ��
      advance(__first_cut, __len11);//��first_cutָ������һ���м�λ��
	  //�ҳ�*__first_cut��[middle,last)�����еĵ�һ����С��*__first_cut��Ԫ��λ��
      __second_cut = lower_bound(__middle, __last, *__first_cut);
	  //����middle��__second_cut֮��ľ��룬������__len22
      distance(__middle, __second_cut, __len22); 
    }
    else {//�����ж��Ƚϳ�
      __len22 = __len2 / 2;//�������ж���һ��
      advance(__second_cut, __len22);//��__second_cutָ�����ж����м�λ��
	  //�ҳ�*__second_cut��[first,middle)�����еĵ�һ������*__second_cut��Ԫ��λ��
      __first_cut = upper_bound(__first, __middle, *__second_cut);
	  //����__first��__first_cut֮��ľ��룬������__len11
      distance(__first, __first_cut, __len11);
    }
    _BidirectionalIter __new_middle =
      __rotate_adaptive(__first_cut, __middle, __second_cut, __len1 - __len11,
                        __len22, __buffer, __buffer_size);
	//�����εݹ����
    __merge_adaptive(__first, __first_cut, __new_middle, __len11,
                     __len22, __buffer, __buffer_size);
	//���Ұ�εݹ����
    __merge_adaptive(__new_middle, __second_cut, __last, __len1 - __len11,
                     __len2 - __len22, __buffer, __buffer_size);
  }
}

template <class _BidirectionalIter, class _Distance, class _Pointer,
          class _Compare>
void __merge_adaptive(_BidirectionalIter __first, 
                      _BidirectionalIter __middle, 
                      _BidirectionalIter __last,
                      _Distance __len1, _Distance __len2,
                      _Pointer __buffer, _Distance __buffer_size,
                      _Compare __comp) {
  if (__len1 <= __len2 && __len1 <= __buffer_size) {
    _Pointer __buffer_end = copy(__first, __middle, __buffer);
    merge(__buffer, __buffer_end, __middle, __last, __first, __comp);
  }
  else if (__len2 <= __buffer_size) {
    _Pointer __buffer_end = copy(__middle, __last, __buffer);
    __merge_backward(__first, __middle, __buffer, __buffer_end, __last,
                     __comp);
  }
  else {
    _BidirectionalIter __first_cut = __first;
    _BidirectionalIter __second_cut = __middle;
    _Distance __len11 = 0;
    _Distance __len22 = 0;
    if (__len1 > __len2) {
      __len11 = __len1 / 2;
      advance(__first_cut, __len11);
      __second_cut = lower_bound(__middle, __last, *__first_cut, __comp);
      distance(__middle, __second_cut, __len22);   
    }
    else {
      __len22 = __len2 / 2;
      advance(__second_cut, __len22);
      __first_cut = upper_bound(__first, __middle, *__second_cut, __comp);
      distance(__first, __first_cut, __len11);
    }
    _BidirectionalIter __new_middle =
      __rotate_adaptive(__first_cut, __middle, __second_cut, __len1 - __len11,
                        __len22, __buffer, __buffer_size);
    __merge_adaptive(__first, __first_cut, __new_middle, __len11,
                     __len22, __buffer, __buffer_size, __comp);
    __merge_adaptive(__new_middle, __second_cut, __last, __len1 - __len11,
                     __len2 - __len22, __buffer, __buffer_size, __comp);
  }
}
//�汾һ�ĸ�������
template <class _BidirectionalIter, class _Tp, class _Distance>
inline void __inplace_merge_aux(_BidirectionalIter __first,
                                _BidirectionalIter __middle,
                                _BidirectionalIter __last, _Tp*, _Distance*) {
  _Distance __len1 = 0;
  distance(__first, __middle, __len1);//��������һ�ĳ���
  _Distance __len2 = 0;
  distance(__middle, __last, __len2);//�������ж��ĳ���

  //ʹ����ʱ������
  _Temporary_buffer<_BidirectionalIter, _Tp> __buf(__first, __last);
  if (__buf.begin() == 0)//������������ʧ��
	  //����ò�ʹ�û������ĺϲ�����
    __merge_without_buffer(__first, __middle, __last, __len1, __len2);
  else//������ɹ�
	  //����þ��л������ĺϲ�����
    __merge_adaptive(__first, __middle, __last, __len1, __len2,
                     __buf.begin(), _Distance(__buf.size()));
}

template <class _BidirectionalIter, class _Tp, 
          class _Distance, class _Compare>
inline void __inplace_merge_aux(_BidirectionalIter __first,
                                _BidirectionalIter __middle,
                                _BidirectionalIter __last, _Tp*, _Distance*,
                                _Compare __comp) {
  _Distance __len1 = 0;
  distance(__first, __middle, __len1);
  _Distance __len2 = 0;
  distance(__middle, __last, __len2);

  _Temporary_buffer<_BidirectionalIter, _Tp> __buf(__first, __last);
  if (__buf.begin() == 0)
    __merge_without_buffer(__first, __middle, __last, __len1, __len2, __comp);
  else
    __merge_adaptive(__first, __middle, __last, __len1, __len2,
                     __buf.begin(), _Distance(__buf.size()),
                     __comp);
}
//�����������������[first,middle)��[middle,last)�ϲ��ɵ�һ��������.
//��ԭ������������Ҳ�ǵ���������ԭ���ǵݼ���������Ҳ�ǵݼ�����
/*
�������ܣ�Merges two consecutive sorted ranges: [first,middle) and [middle,last), 
putting the result into the combined sorted range [first,last).
����ԭ�ͣ�
default (1)	���汾һ
	template <class BidirectionalIterator>
	void inplace_merge (BidirectionalIterator first, BidirectionalIterator middle,
                      BidirectionalIterator last);
custom (2)	���汾��
	template <class BidirectionalIterator, class Compare>
	void inplace_merge (BidirectionalIterator first, BidirectionalIterator middle,
                      BidirectionalIterator last, Compare comp);
*/
//�汾һ
template <class _BidirectionalIter>
inline void inplace_merge(_BidirectionalIter __first,
                          _BidirectionalIter __middle,
                          _BidirectionalIter __last) {
  __STL_REQUIRES(_BidirectionalIter, _Mutable_BidirectionalIterator);
  __STL_REQUIRES(typename iterator_traits<_BidirectionalIter>::value_type,
                 _LessThanComparable);
  if (__first == __middle || __middle == __last)//���п����У���֮�䷵��
    return;
  __inplace_merge_aux(__first, __middle, __last,
                      __VALUE_TYPE(__first), __DISTANCE_TYPE(__first));
}
//�汾��
template <class _BidirectionalIter, class _Compare>
inline void inplace_merge(_BidirectionalIter __first,
                          _BidirectionalIter __middle,
                          _BidirectionalIter __last, _Compare __comp) {
  __STL_REQUIRES(_BidirectionalIter, _Mutable_BidirectionalIterator);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
           typename iterator_traits<_BidirectionalIter>::value_type,
           typename iterator_traits<_BidirectionalIter>::value_type);
  if (__first == __middle || __middle == __last)
    return;
  __inplace_merge_aux(__first, __middle, __last,
                      __VALUE_TYPE(__first), __DISTANCE_TYPE(__first),
                      __comp);
}
//inplace_merge����������
/*
	#include <iostream>     // std::cout
	#include <algorithm>    // std::inplace_merge, std::sort, std::copy
	#include <vector>       // std::vector

	int main () {
	  int first[] = {5,10,15,20,25};
	  int second[] = {50,40,30,20,10};
	  std::vector<int> v(10);
	  std::vector<int>::iterator it;

	  std::sort (first,first+5);
	  std::sort (second,second+5);

	  it=std::copy (first, first+5, v.begin());
		 std::copy (second,second+5,it);

	  std::inplace_merge (v.begin(),v.begin()+5,v.end());

	  std::cout << "The resulting vector contains:";
	  for (it=v.begin(); it!=v.end(); ++it)
		std::cout << ' ' << *it;
	  std::cout << '\n';

	  return 0;
	}
	Output��
	The resulting vector contains: 5 10 10 15 20 20 25 30 40 50
*/

// Set algorithms: includes, set_union, set_intersection, set_difference,
// set_symmetric_difference.  All of these algorithms have the precondition
// that their input ranges are sorted and the postcondition that their output
// ranges are sorted.

/*
�����Ǽ���set���ϵ�����㷨���ֱ��ǲ���set_union���set_difference������set_intersection
�ͶԳƲset_symmetric_difference�����Ǹ��������ṩ�������汾�ĺ���ԭ��
��һ���汾�ǲ���Ĭ�ϵ�����ȽϷ�ʽ operator<
�ڶ����汾���û�ͨ��comp����ָ������ʽ
ע�⣺���ĸ��㷨���ܵ��������䶼������ģ����Ҳ�������
*/

// Set algorithms: includes, set_union, set_intersection, set_difference,
// set_symmetric_difference.  All of these algorithms have the precondition
// that their input ranges are sorted and the postcondition that their output
// ranges are sorted.

// �ж�[first1, last1)�Ƿ����[first2, last2),  
// ע��: ��������Ҫ��֤����,Ĭ������ʽ��operator<����Ҫ���ж�������ʽ������õڶ��汾;
template <class _InputIter1, class _InputIter2>
bool includes(_InputIter1 __first1, _InputIter1 __last1,
              _InputIter2 __first2, _InputIter2 __last2) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES_SAME_TYPE(
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);
  __STL_REQUIRES(typename iterator_traits<_InputIter1>::value_type,
                 _LessThanComparable);
  while (__first1 != __last1 && __first2 != __last2)//������������
    if (*__first2 < *__first1)//first2С��first1��ʾ������
      return false;//����FALSE
    else if(*__first1 < *__first2)//��first1С��first2 
      ++__first1;//Ѱ�ҵ�һ��������һ��λ��
    else
      ++__first1, ++__first2;//��first2����first1,�������������һλ��

  return __first2 == __last2;//���ڶ��������ȵ���β�ˣ��򷵻�TRUE
}

//�汾�����û�ͨ��comp����ָ������ʽ
template <class _InputIter1, class _InputIter2, class _Compare>
bool includes(_InputIter1 __first1, _InputIter1 __last1,
              _InputIter2 __first2, _InputIter2 __last2, _Compare __comp) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES_SAME_TYPE(
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);
  while (__first1 != __last1 && __first2 != __last2)
    if (__comp(*__first2, *__first1))
      return false;
    else if(__comp(*__first1, *__first2)) 
      ++__first1;
    else
      ++__first1, ++__first2;

  return __first2 == __last2;
}

//������������Ĳ�����ͬ��Ҳ�������汾
//�������[first1, last1)�������[first2, last2)�ڵ�����Ԫ��
//ע�⣺�������������������
/*
default (1)	:Ĭ����operator<����������ʽ
	template <class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator set_union (InputIterator1 first1, InputIterator1 last1,
                            InputIterator2 first2, InputIterator2 last2,
                            OutputIterator result);
custom (2)	:�û�ָ��������ʽ
	template <class InputIterator1, class InputIterator2,
          class OutputIterator, class Compare>
	OutputIterator set_union (InputIterator1 first1, InputIterator1 last1,
                            InputIterator2 first2, InputIterator2 last2,
                            OutputIterator result, Compare comp);
*/
//�汾һ��Ĭ����operator<����������ʽ
template <class _InputIter1, class _InputIter2, class _OutputIter>
_OutputIter set_union(_InputIter1 __first1, _InputIter1 __last1,
                      _InputIter2 __first2, _InputIter2 __last2,
                      _OutputIter __result) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_SAME_TYPE(
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);
  __STL_REQUIRES(typename iterator_traits<_InputIter1>::value_type,
                 _LessThanComparable);
  //�������䶼��δ��������β�ˣ�ִ�����²���
  while (__first1 != __last1 && __first2 != __last2) {
	  /*
	  ���������ڷֱ��ƶ������������Ƚ�Ԫ�ؽ�С��(����ΪA��)��¼��Ŀ����result
	  �ƶ�A��������ʹ��ǰ����ͬʱ��һ�����ĵ��������䡣Ȼ�����һ���µıȽϣ�
	  ��¼��Сֵ���ƶ�������...ֱ������������һ������β�ˡ������������Ԫ����ȣ�
	  Ĭ�ϼ�¼��һ�����Ԫ�ص�Ŀ����result.
	  */
    if (*__first1 < *__first2) {//first1С��first2
      *__result = *__first1;//��result��ʼֵΪfirst1
      ++__first1;//������һ���������һ��Ԫ��λ��
    }
    else if (*__first2 < *__first1) {//first2С��first1
      *__result = *__first2;//�ڶ�����Ԫ��ֵ��¼��Ŀ����
      ++__first2;//�ƶ��ڶ�����ĵ�����
    }
    else {//�������������ȵ�Ԫ�أ��ѵ�һ����Ԫ�ؼ�¼��Ŀ����
		//ͬʱ�ƶ���������ĵ�����
      *__result = *__first1;
      ++__first1;
      ++__first2;
    }
    ++__result;//����Ŀ����λ�ã��Ա�������һ�μ�¼��������
  }
  /*
  ֻҪ������֮����һ�����䵽��β�ˣ��ͽ��������whileѭ��
  ���½���δ����β�˵�����ʣ���Ԫ�ؿ�����Ŀ����
  �˿̣�[first1, last1)��[first2, last2)������һ���ǿ�����
  */
  return copy(__first2, __last2, copy(__first1, __last1, __result));
}
//�汾�����û����ݷº���compָ���������
template <class _InputIter1, class _InputIter2, class _OutputIter,
          class _Compare>
_OutputIter set_union(_InputIter1 __first1, _InputIter1 __last1,
                      _InputIter2 __first2, _InputIter2 __last2,
                      _OutputIter __result, _Compare __comp) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_SAME_TYPE(
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);
  while (__first1 != __last1 && __first2 != __last2) {
    if (__comp(*__first1, *__first2)) {
      *__result = *__first1;
      ++__first1;
    }
    else if (__comp(*__first2, *__first1)) {
      *__result = *__first2;
      ++__first2;
    }
    else {
      *__result = *__first1;
      ++__first1;
      ++__first2;
    }
    ++__result;
  }
  return copy(__first2, __last2, copy(__first1, __last1, __result));
}
/*����:
	#include <iostream>     // std::cout
	#include <algorithm>    // std::set_union, std::sort
	#include <vector>       // std::vector

	int main () {
	  int first[] = {5,10,15,20,25};
	  int second[] = {50,40,30,20,10};
	  std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
	  std::vector<int>::iterator it;

	  std::sort (first,first+5);     //  5 10 15 20 25
	  std::sort (second,second+5);   // 10 20 30 40 50

	  it=std::set_union (first, first+5, second, second+5, v.begin());
												   // 5 10 15 20 25 30 40 50  0  0
	  v.resize(it-v.begin());                      // 5 10 15 20 25 30 40 50

	  std::cout << "The union has " << (v.size()) << " elements:\n";
	  for (it=v.begin(); it!=v.end(); ++it)
		std::cout << ' ' << *it;
	  std::cout << '\n';

	  return 0;
	}
	Output:
	The union has 8 elements:
	5 10 15 20 25 30 40 50
*/

 //������������Ľ�����ͬ��Ҳ�������汾
//�������[first1, last1)�Ҵ�����[first2, last2)�ڵ�����Ԫ��
//ע�⣺���������������������������ÿ��Ԫ�ص��������͵�һ���������������ͬ
/*
default (1)	:Ĭ����operator<����������ʽ
		template <class InputIterator1, class InputIterator2, class OutputIterator>
		OutputIterator set_intersection (InputIterator1 first1, InputIterator1 last1,
                                   InputIterator2 first2, InputIterator2 last2,
                                   OutputIterator result);


custom (2)	:�û�ָ��������ʽ
		template <class InputIterator1, class InputIterator2,
          class OutputIterator, class Compare>
		OutputIterator set_intersection (InputIterator1 first1, InputIterator1 last1,
                                   InputIterator2 first2, InputIterator2 last2,
                                   OutputIterator result, Compare comp);
*/
//�汾һ��Ĭ����operator<����������ʽ
template <class _InputIter1, class _InputIter2, class _OutputIter>
_OutputIter set_intersection(_InputIter1 __first1, _InputIter1 __last1,
                             _InputIter2 __first2, _InputIter2 __last2,
                             _OutputIter __result) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_SAME_TYPE(
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);
  __STL_REQUIRES(typename iterator_traits<_InputIter1>::value_type,
                 _LessThanComparable);
  //���������䶼��δ����β�ˣ���ִ�����²���
  while (__first1 != __last1 && __first2 != __last2) 
	//����������ֱ��ƶ���������ֱ���������Ԫ�أ���¼��Ŀ����
	//�����ƶ�������...ֱ��������֮���е���β��
    if (*__first1 < *__first2) //��һ������Ԫ��С�ڵڶ�����Ԫ��
      ++__first1;//�ƶ���һ����ĵ���������ʱ�ڶ�����ĵ���������
    else if (*__first2 < *__first1) //�ڶ������Ԫ��С�ڵ�һ����Ԫ��
      ++__first2;//�ƶ��ڶ�����Ԫ�أ���ʱ��һ����ĵ���������
    else {//����һ����Ԫ�ص��ڵڶ�����Ԫ��
      *__result = *__first1;//����һ�������������¼��Ŀ����
	  //�ֱ��ƶ�������ĵ�����
      ++__first1;
      ++__first2;
	  //����Ŀ�������������Ա������¼Ԫ��
      ++__result;
    }
	//�������䵽��β������ֹͣwhileѭ��
	//��ʱ������Ŀ����
  return __result;
}
//�汾�����û����ݷº���compָ���������
template <class _InputIter1, class _InputIter2, class _OutputIter,
          class _Compare>
_OutputIter set_intersection(_InputIter1 __first1, _InputIter1 __last1,
                             _InputIter2 __first2, _InputIter2 __last2,
                             _OutputIter __result, _Compare __comp) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_SAME_TYPE(
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);

  while (__first1 != __last1 && __first2 != __last2)
    if (__comp(*__first1, *__first2))
      ++__first1;
    else if (__comp(*__first2, *__first1))
      ++__first2;
    else {
      *__result = *__first1;
      ++__first1;
      ++__first2;
      ++__result;
    }
  return __result;
}
/*���ӣ�
	#include <iostream>     // std::cout
	#include <algorithm>    // std::set_intersection, std::sort
	#include <vector>       // std::vector

	int main () {
	  int first[] = {5,10,15,20,25};
	  int second[] = {50,40,30,20,10};
	  std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
	  std::vector<int>::iterator it;

	  std::sort (first,first+5);     //  5 10 15 20 25
	  std::sort (second,second+5);   // 10 20 30 40 50

	  it=std::set_intersection (first, first+5, second, second+5, v.begin());
												   // 10 20 0  0  0  0  0  0  0  0
	  v.resize(it-v.begin());                      // 10 20

	  std::cout << "The intersection has " << (v.size()) << " elements:\n";
	  for (it=v.begin(); it!=v.end(); ++it)
		std::cout << ' ' << *it;
	  std::cout << '\n';

	  return 0;
	}
	Output:
	The intersection has 2 elements:
	10 20
*/

 //������������Ĳ��ͬ��Ҳ�������汾
//�������[first1, last1)����������[first2, last2)�ڵ�����Ԫ��
//ע�⣺���������������������������ÿ��Ԫ�ص��������͵�һ���������������ͬ
/*
default (1)	:Ĭ����operator<����������ʽ
		template <class InputIterator1, class InputIterator2, class OutputIterator>
		OutputIterator set_difference (InputIterator1 first1, InputIterator1 last1,
                                 InputIterator2 first2, InputIterator2 last2,
                                 OutputIterator result);



custom (2)	:�û�ָ��������ʽ
		template <class InputIterator1, class InputIterator2,
          class OutputIterator, class Compare>
		OutputIterator set_difference (InputIterator1 first1, InputIterator1 last1,
                                 InputIterator2 first2, InputIterator2 last2,
                                 OutputIterator result, Compare comp);
*/
//�汾һ��Ĭ����operator<����������ʽ
template <class _InputIter1, class _InputIter2, class _OutputIter>
_OutputIter set_difference(_InputIter1 __first1, _InputIter1 __last1,
                           _InputIter2 __first2, _InputIter2 __last2,
                           _OutputIter __result) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_SAME_TYPE(
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);
  __STL_REQUIRES(typename iterator_traits<_InputIter1>::value_type,
                 _LessThanComparable);
  //���������䶼��δ����β�ˣ���ִ�����²���
  while (__first1 != __last1 && __first2 != __last2)
	 /*
	 ����������ֱ��ƶ�������������һ����Ԫ�ص��ڵڶ�����Ԫ��ʱ����ʾ�����乲ͬ���ڸ�Ԫ��
	 ��ͬʱ�ƶ���������
	 ����һ����Ԫ�ش��ڵڶ�����Ԫ��ʱ�����õڶ����������ǰ����
	 ��һ����Ԫ��С�ڵڶ�����Ԫ��ʱ���ѵ�һ����Ԫ�ؼ�¼��Ŀ����
	�����ƶ�������...ֱ��������֮���е���β��
	*/
    if (*__first1 < *__first2) {//��һ����Ԫ��С�ڵڶ�����Ԫ��
      *__result = *__first1;//�ѵ�һ����Ԫ�ؼ�¼��Ŀ����
      ++__first1;//�ƶ���һ���������
      ++__result;//����Ŀ�������Ա������¼����
    }
    else if (*__first2 < *__first1)//����һ�����Ԫ�ش��ڵڶ������Ԫ��
      ++__first2;//�ƶ��ڶ����������,ע�⣺���ﲻ��¼�κ�Ԫ��
    else {//���������Ԫ�����ʱ,ͬʱ�ƶ�������ĵ�����
      ++__first1;
      ++__first2;
    }
	//���ڶ������ȵ���β�ˣ���ѵ�һ����ʣ���Ԫ�ظ��Ƶ�Ŀ����
  return copy(__first1, __last1, __result);
}
//�汾�����û����ݷº���compָ���������
template <class _InputIter1, class _InputIter2, class _OutputIter, 
          class _Compare>
_OutputIter set_difference(_InputIter1 __first1, _InputIter1 __last1,
                           _InputIter2 __first2, _InputIter2 __last2, 
                           _OutputIter __result, _Compare __comp) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_SAME_TYPE(
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);

  while (__first1 != __last1 && __first2 != __last2)
    if (__comp(*__first1, *__first2)) {
      *__result = *__first1;
      ++__first1;
      ++__result;
    }
    else if (__comp(*__first2, *__first1))
      ++__first2;
    else {
      ++__first1;
      ++__first2;
    }
  return copy(__first1, __last1, __result);
}
/*���ӣ�
	#include <iostream>     // std::cout
	#include <algorithm>    // std::set_difference, std::sort
	#include <vector>       // std::vector

	int main () {
	  int first[] = {5,10,15,20,25};
	  int second[] = {50,40,30,20,10};
	  std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
	  std::vector<int>::iterator it;

	  std::sort (first,first+5);     //  5 10 15 20 25
	  std::sort (second,second+5);   // 10 20 30 40 50

	  it=std::set_difference (first, first+5, second, second+5, v.begin());
												   //  5 15 25  0  0  0  0  0  0  0
	  v.resize(it-v.begin());                      //  5 15 25

	  std::cout << "The difference has " << (v.size()) << " elements:\n";
	  for (it=v.begin(); it!=v.end(); ++it)
		std::cout << ' ' << *it;
	  std::cout << '\n';

	  return 0;
	}
	Output��
	The difference has 3 elements:
	5 15 25
*/

 //������������ĶԳƲ��ͬ��Ҳ�������汾
//�������[first1, last1)����������[first2, last2)�ڵ�����Ԫ���Լ�������[first2, last2)����������[first1, last1)
//ע�⣺�������������������
/*
default (1)	:Ĭ����operator<����������ʽ
		template <class InputIterator1, class InputIterator2, class OutputIterator>
		OutputIterator set_symmetric_difference (InputIterator1 first1, InputIterator1 last1,
                                           InputIterator2 first2, InputIterator2 last2,
                                           OutputIterator result);


custom (2)	:�û�ָ��������ʽ
		template <class InputIterator1, class InputIterator2,
          class OutputIterator, class Compare>
		OutputIterator set_symmetric_difference (InputIterator1 first1, InputIterator1 last1,
                                           InputIterator2 first2, InputIterator2 last2,
                                           OutputIterator result, Compare comp);
*/
//�汾һ��Ĭ����operator<����������ʽ
template <class _InputIter1, class _InputIter2, class _OutputIter>
_OutputIter 
set_symmetric_difference(_InputIter1 __first1, _InputIter1 __last1,
                         _InputIter2 __first2, _InputIter2 __last2,
                         _OutputIter __result) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_SAME_TYPE(
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);
  __STL_REQUIRES(typename iterator_traits<_InputIter1>::value_type,
                 _LessThanComparable);
  //���������䶼��δ����β�ˣ���ִ������Ĳ���
  while (__first1 != __last1 && __first2 != __last2)
	  /*
	  ���1����������Ԫ����ȣ���ͬʱ�ƶ�������ĵ�����.
	  ���2������һ�����Ԫ��С�ڵڶ�����Ԫ�أ���ѵ�һ����Ԫ�ؼ�¼��Ŀ���������ƶ���һ���������.
	  ���3������һ�����Ԫ�ش��ڵڶ�����Ԫ�أ���ѵڶ�����Ԫ�ؼ�¼��Ŀ���������ƶ��ڶ����������.
	  */
    if (*__first1 < *__first2) {//�������2
      *__result = *__first1;//�ѵ�һ����Ԫ�ؼ�¼��Ŀ����
      ++__first1;//�ƶ���һ���������.��ʱ�ڶ��������������
      ++__result;
    }
    else if (*__first2 < *__first1) {//�������3
      *__result = *__first2;//�ѵڶ�����Ԫ�ؼ�¼��Ŀ����
      ++__first2;//�ƶ��ڶ����������.��ʱ��һ�������������
      ++__result;
    }
    else {//�������1
		//ͬʱ�ƶ�������ĵ�����
      ++__first1;
      ++__first2;
    }
	 /*
  ֻҪ������֮����һ�����䵽��β�ˣ��ͽ��������whileѭ��
  ���½���δ����β�˵�����ʣ���Ԫ�ؿ�����Ŀ����
  �˿̣�[first1, last1)��[first2, last2)������һ���ǿ�����
  */
  return copy(__first2, __last2, copy(__first1, __last1, __result));
}

//�汾�����û����ݷº���compָ���������
template <class _InputIter1, class _InputIter2, class _OutputIter,
          class _Compare>
_OutputIter 
set_symmetric_difference(_InputIter1 __first1, _InputIter1 __last1,
                         _InputIter2 __first2, _InputIter2 __last2,
                         _OutputIter __result,
                         _Compare __comp) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_SAME_TYPE(
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);
  while (__first1 != __last1 && __first2 != __last2)
    if (__comp(*__first1, *__first2)) {
      *__result = *__first1;
      ++__first1;
      ++__result;
    }
    else if (__comp(*__first2, *__first1)) {
      *__result = *__first2;
      ++__first2;
      ++__result;
    }
    else {
      ++__first1;
      ++__first2;
    }
  return copy(__first2, __last2, copy(__first1, __last1, __result));
}
/*���ӣ�
	#include <iostream>     // std::cout
	#include <algorithm>    // std::set_symmetric_difference, std::sort
	#include <vector>       // std::vector

	int main () {
	  int first[] = {5,10,15,20,25};
	  int second[] = {50,40,30,20,10};
	  std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
	  std::vector<int>::iterator it;

	  std::sort (first,first+5);     //  5 10 15 20 25
	  std::sort (second,second+5);   // 10 20 30 40 50

	  it=std::set_symmetric_difference (first, first+5, second, second+5, v.begin());
												   //  5 15 25 30 40 50  0  0  0  0
	  v.resize(it-v.begin());                      //  5 15 25 30 40 50

	  std::cout << "The symmetric difference has " << (v.size()) << " elements:\n";
	  for (it=v.begin(); it!=v.end(); ++it)
		std::cout << ' ' << *it;
	  std::cout << '\n';

	  return 0;
	}
	Output��
	The symmetric difference has 6 elements:
	5 15 25 30 40 50
*/

// min_element and max_element, with and without an explicitly supplied
// comparison function.
//��������[first,last)�����Ԫ�ص�λ��
/*
default (1):�汾һ	
	template <class ForwardIterator>
	ForwardIterator max_element (ForwardIterator first, ForwardIterator last);
custom (2):�汾��	
	template <class ForwardIterator, class Compare>
	ForwardIterator max_element (ForwardIterator first, ForwardIterator last,
                               Compare comp);
*/
//�汾һ��
template <class _ForwardIter>
_ForwardIter max_element(_ForwardIter __first, _ForwardIter __last) {
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES(typename iterator_traits<_ForwardIter>::value_type,
                 _LessThanComparable);
  if (__first == __last) return __first;//��Ϊ�գ�ֱ�ӷ���
  _ForwardIter __result = __first;//����Ϊ�գ��ӵ�һ��Ԫ�ؿ�ʼ�����ѵ�һ��Ԫ����ʱ����Ϊ���ֵ
  while (++__first != __last) //��˳��������ֵ
    if (*__result < *__first)//���и����ֵ
      __result = __first;//��������ֵλ��
  return __result;//�������ֵλ��
}
//�汾��
template <class _ForwardIter, class _Compare>
_ForwardIter max_element(_ForwardIter __first, _ForwardIter __last,
			 _Compare __comp) {
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
    typename iterator_traits<_ForwardIter>::value_type,
    typename iterator_traits<_ForwardIter>::value_type);
  if (__first == __last) return __first;
  _ForwardIter __result = __first;
  while (++__first != __last) 
    if (__comp(*__result, *__first)) __result = __first;
  return __result;
}
//��������[first,last)����СԪ�ص�λ��
/*
default (1):�汾һ	
	template <class ForwardIterator>
	ForwardIterator min_element (ForwardIterator first, ForwardIterator last);
custom (2):�汾��	
	template <class ForwardIterator, class Compare>
	ForwardIterator min_element (ForwardIterator first, ForwardIterator last,
                               Compare comp);
*/
//�汾һ��
template <class _ForwardIter>
_ForwardIter min_element(_ForwardIter __first, _ForwardIter __last) {
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES(typename iterator_traits<_ForwardIter>::value_type,
                 _LessThanComparable);
  if (__first == __last) return __first;//��Ϊ�գ�ֱ�ӷ���
  _ForwardIter __result = __first;//����Ϊ�գ��ӵ�һ��Ԫ�ؿ�ʼ�����ѵ�һ��Ԫ����ʱ����Ϊ��Сֵ
  while (++__first != __last) //��˳�������Сֵ
    if (*__first < *__result)//�����ڸ�С��ֵ
      __result = __first;//�������Сֵλ��
  return __result;//������Сֵλ��
}
//�汾��
template <class _ForwardIter, class _Compare>
_ForwardIter min_element(_ForwardIter __first, _ForwardIter __last,
			 _Compare __comp) {
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
    typename iterator_traits<_ForwardIter>::value_type,
    typename iterator_traits<_ForwardIter>::value_type);
  if (__first == __last) return __first;
  _ForwardIter __result = __first;
  while (++__first != __last) 
    if (__comp(*__first, *__result))
      __result = __first;
  return __result;
}
//max_element��min_element��������
/*
	#include <iostream>     // std::cout
	#include <algorithm>    // std::min_element, std::max_element

	bool myfn(int i, int j) { return i<j; }

	struct myclass {
	  bool operator() (int i,int j) { return i<j; }
	} myobj;

	int main () {
	  int myints[] = {3,7,2,5,6,4,9};

	  // using default comparison:
	  std::cout << "The smallest element is " << *std::min_element(myints,myints+7) << '\n';
	  std::cout << "The largest element is "  << *std::max_element(myints,myints+7) << '\n';

	  // using function myfn as comp:
	  std::cout << "The smallest element is " << *std::min_element(myints,myints+7,myfn) << '\n';
	  std::cout << "The largest element is "  << *std::max_element(myints,myints+7,myfn) << '\n';

	  // using object myobj as comp:
	  std::cout << "The smallest element is " << *std::min_element(myints,myints+7,myobj) << '\n';
	  std::cout << "The largest element is "  << *std::max_element(myints,myints+7,myobj) << '\n';

	  return 0;
	}
	Output:
	The smallest element is 2
	The largest element is 9
	The smallest element is 2
	The largest element is 9
	The smallest element is 2
	The largest element is 9
*/

// next_permutation and prev_permutation, with and without an explicitly 
// supplied comparison function.
//next_permutation��ȡ[first,last)��������ʾ���е���һ��������ϣ�����û����һ��������ϣ��򷵻�false;���򷵻�true;
/*
�������ܣ�Rearranges the elements in the range [first,last) into the next lexicographically greater permutation.
����ԭ�ͣ�
default (1)	���汾һ����less-than������
	template <class BidirectionalIterator>
	bool next_permutation (BidirectionalIterator first,
                         BidirectionalIterator last);
custom (2)	���汾�����÷º���comp����
	template <class BidirectionalIterator, class Compare>
	bool next_permutation (BidirectionalIterator first,
                         BidirectionalIterator last, Compare comp);
*/
//�汾һ
template <class _BidirectionalIter>
bool next_permutation(_BidirectionalIter __first, _BidirectionalIter __last) {
  __STL_REQUIRES(_BidirectionalIter, _BidirectionalIterator);
  __STL_REQUIRES(typename iterator_traits<_BidirectionalIter>::value_type,
                 _LessThanComparable);
  if (__first == __last)
    return false;//��Ϊ�գ��򷵻�false
  _BidirectionalIter __i = __first;
  ++__i;
  if (__i == __last)//����ֻ��һ��Ԫ��
    return false;
  //������Ԫ�ظ�����С������
  __i = __last;//iָ��β��
  --__i;//���Ϻ���

  for(;;) {
	  //������������ii��i��Ϊ���ڵ�Ԫ��
	  //����iΪ��һ��Ԫ�أ�iiΪ�ڶ���Ԫ��
    _BidirectionalIter __ii = __i;//
    --__i;
	//����������Ԫ���ж�
    if (*__i < *__ii) {//��ǰһ��Ԫ��С�ں�һ��Ԫ�أ�
		//���ٴ���β�˿�ʼ��ǰ��飬�ҳ���һ������*i��Ԫ�أ����Ԫ��Ϊ*j����*i��*j����
		//�ٽ�ii֮�������Ԫ�صߵ�����
      _BidirectionalIter __j = __last;//��jָ����β��
      while (!(*__i < *--__j))//��β����ǰ��飬ֱ��������*i���Ԫ��
        {}
      iter_swap(__i, __j);//����������i�͵�����j��ָ��Ԫ��
      reverse(__ii, __last);//��ii֮���Ԫ��ȫ����������
      return true;
    }
    if (__i == __first) {//���е���ǰ��
      reverse(__first, __last);//��������ȫ����������
      return false;
    }
  }
}
//�汾��
template <class _BidirectionalIter, class _Compare>
bool next_permutation(_BidirectionalIter __first, _BidirectionalIter __last,
                      _Compare __comp) {
  __STL_REQUIRES(_BidirectionalIter, _BidirectionalIterator);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
    typename iterator_traits<_BidirectionalIter>::value_type,
    typename iterator_traits<_BidirectionalIter>::value_type);
  if (__first == __last)
    return false;
  _BidirectionalIter __i = __first;
  ++__i;
  if (__i == __last)
    return false;
  __i = __last;
  --__i;

  for(;;) {
    _BidirectionalIter __ii = __i;
    --__i;
    if (__comp(*__i, *__ii)) {
      _BidirectionalIter __j = __last;
      while (!__comp(*__i, *--__j))
        {}
      iter_swap(__i, __j);
      reverse(__ii, __last);
      return true;
    }
    if (__i == __first) {
      reverse(__first, __last);
      return false;
    }
  }
}
//next_permutation����������
/*
	#include <iostream>     // std::cout
	#include <algorithm>    // std::next_permutation, std::sort

	int main () {
	  int myints[] = {1,2,3,4};

	  std::sort (myints,myints+4);

	  std::cout << "The 3! possible permutations with 3 elements:\n";
	  do {
		std::cout << myints[0] << ' ' << myints[1] << ' ' << myints[2] <<' ' << myints[3]<< '\n';
	  } while ( std::next_permutation(myints,myints+4) );
 
	  //std::next_permutation(myints,myints+4);
	  std::cout << "After loop: " << myints[0] << ' ' << myints[1] << ' ' << myints[2] << ' ' << myints[3]<<'\n';

	  return 0;
	}
	Output:
	The 3! possible permutations with 3 elements:
	1 2 3 4
	1 2 4 3
	1 3 2 4
	1 3 4 2
	1 4 2 3
	1 4 3 2
	2 1 3 4
	2 1 4 3
	2 3 1 4
	2 3 4 1
	2 4 1 3
	2 4 3 1
	3 1 2 4
	3 1 4 2
	3 2 1 4
	3 2 4 1
	3 4 1 2
	3 4 2 1
	4 1 2 3
	4 1 3 2
	4 2 1 3
	4 2 3 1
	4 3 1 2
	4 3 2 1
	After loop: 1 2 3 4
*/

//prev_permutation��ȡ[first,last)��������ʾ���е���һ��������ϣ�����û����һ��������ϣ��򷵻�false;���򷵻�true;
/*
�������ܣ�Rearranges the elements in the range [first,last) into the previous lexicographically-ordered permutation.
����ԭ�ͣ�
default (1)	���汾һ����less-than������
	template <class BidirectionalIterator>
	bool prev_permutation (BidirectionalIterator first,
                         BidirectionalIterator last );
custom (2)	���汾�����÷º���comp
	template <class BidirectionalIterator, class Compare>
	bool prev_permutation (BidirectionalIterator first,
                         BidirectionalIterator last, Compare comp);
*/
//�汾һ
template <class _BidirectionalIter>
bool prev_permutation(_BidirectionalIter __first, _BidirectionalIter __last) {
  __STL_REQUIRES(_BidirectionalIter, _BidirectionalIterator);
  __STL_REQUIRES(typename iterator_traits<_BidirectionalIter>::value_type,
                 _LessThanComparable);
  if (__first == __last)
    return false;//������Ϊ�գ�����false
  _BidirectionalIter __i = __first;
  ++__i;
  if (__i == __last)//����ֻ��һ��Ԫ��
    return false;//����false
  //������Ԫ�ظ�����С������
  __i = __last;
  --__i;

  for(;;) {
	  //������������ii��i��Ϊ���ڵ�Ԫ��
	  //����iΪ��һ��Ԫ�أ�iiΪ�ڶ���Ԫ��
    _BidirectionalIter __ii = __i;
    --__i;
	//����������Ԫ���ж�
    if (*__ii < *__i) {//��ǰһ��Ԫ�ش��ں�һ��Ԫ�أ�
		//���ٴ���β�˿�ʼ��ǰ��飬�ҳ���һ��С��*i��Ԫ�أ����Ԫ��Ϊ*j����*i��*j����
		//�ٽ�ii֮�������Ԫ�صߵ�����
      _BidirectionalIter __j = __last;//��jָ����β��      
      while (!(*--__j < *__i))//��β����ǰ��飬ֱ��������*iС��Ԫ��
        {}
      iter_swap(__i, __j); //����������i�͵�����j��ָ��Ԫ��
      reverse(__ii, __last);//��ii֮���Ԫ��ȫ����������
      return true;
    }
    if (__i == __first) {//���е���ǰ��
      reverse(__first, __last);//����������Ԫ����������
      return false;
    }
  }
}
//�汾��
template <class _BidirectionalIter, class _Compare>
bool prev_permutation(_BidirectionalIter __first, _BidirectionalIter __last,
                      _Compare __comp) {
  __STL_REQUIRES(_BidirectionalIter, _BidirectionalIterator);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
    typename iterator_traits<_BidirectionalIter>::value_type,
    typename iterator_traits<_BidirectionalIter>::value_type);
  if (__first == __last)
    return false;
  _BidirectionalIter __i = __first;
  ++__i;
  if (__i == __last)
    return false;
  __i = __last;
  --__i;

  for(;;) {
    _BidirectionalIter __ii = __i;
    --__i;
    if (__comp(*__ii, *__i)) {
      _BidirectionalIter __j = __last;
      while (!__comp(*--__j, *__i))
        {}
      iter_swap(__i, __j);
      reverse(__ii, __last);
      return true;
    }
    if (__i == __first) {
      reverse(__first, __last);
      return false;
    }
  }
}
//prev_permutation��������
/*
	#include <iostream>     // std::cout
	#include <algorithm>    // std::next_permutation, std::sort, std::reverse

	int main () {
	  int myints[] = {1,2,3};

	  std::sort (myints,myints+3);
	  std::reverse (myints,myints+3);

	  std::cout << "The 3! possible permutations with 3 elements:\n";
	  do {
		std::cout << myints[0] << ' ' << myints[1] << ' ' << myints[2] << '\n';
	  } while ( std::prev_permutation(myints,myints+3) );

	  std::cout << "After loop: " << myints[0] << ' ' << myints[1] << ' ' << myints[2] << '\n';

	  return 0;
	}
	Output:
	The 3! possible permutations with 3 elements:
	3 2 1
	3 1 2
	2 3 1
	2 1 3
	1 3 2
	1 2 3
	After loop: 3 2 1
*/

// find_first_of, with and without an explicitly supplied comparison function.
//��[first2,last2)�����ڵ�ĳЩԪ��Ϊ����Ŀ�꣬Ѱ��������[first1,last1)�����״γ��ֵ�λ��
//find_first_of�����������汾��
//�汾һ���ṩĬ�ϵ�equality����operator==
//�汾�����ṩ�û�����ָ���Ĳ�������comp
/*
�������ܣ�Returns an iterator to the first element in the range [first1,last1) that matches any of the elements in [first2,last2). 
If no such element is found, the function returns last1.
����ԭ�ͣ�
equality (1)���汾һ	
	template <class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1 find_first_of (ForwardIterator1 first1, ForwardIterator1 last1,
                                   ForwardIterator2 first2, ForwardIterator2 last2);
predicate (2)���汾��	
	template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
	ForwardIterator1 find_first_of (ForwardIterator1 first1, ForwardIterator1 last1,
                                   ForwardIterator2 first2, ForwardIterator2 last2,
                                   BinaryPredicate pred);
*/
//�汾һ���ṩĬ�ϵ�equality����operator==
template <class _InputIter, class _ForwardIter>
_InputIter find_first_of(_InputIter __first1, _InputIter __last1,
                         _ForwardIter __first2, _ForwardIter __last2)
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool, 
     typename iterator_traits<_InputIter>::value_type,
     typename iterator_traits<_ForwardIter>::value_type);

  for ( ; __first1 != __last1; ++__first1) //������һ��Ϊ�գ����������һ��ÿ��ָ��һ��Ԫ��
	  //���£��������ж���ÿ��Ԫ��
    for (_ForwardIter __iter = __first2; __iter != __last2; ++__iter)
      if (*__first1 == *__iter)//������һ��Ԫ�ص������ж���Ԫ�أ����ʾ�ҵ�
        return __first1;//�����ҵ���λ��
  return __last1;//����û�ҵ�
}
//�汾�����ṩ�û�����ָ���Ĳ�������comp
template <class _InputIter, class _ForwardIter, class _BinaryPredicate>
_InputIter find_first_of(_InputIter __first1, _InputIter __last1,
                         _ForwardIter __first2, _ForwardIter __last2,
                         _BinaryPredicate __comp)
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_BINARY_FUNCTION_CHECK(_BinaryPredicate, bool,
     typename iterator_traits<_InputIter>::value_type,
     typename iterator_traits<_ForwardIter>::value_type);

  for ( ; __first1 != __last1; ++__first1) 
    for (_ForwardIter __iter = __first2; __iter != __last2; ++__iter)
      if (__comp(*__first1, *__iter))
        return __first1;
  return __last1;
}
//find_first_of����������
/*
	#include <iostream>     // std::cout
	#include <algorithm>    // std::find_first_of
	#include <vector>       // std::vector
	#include <cctype>       // std::tolower

	bool comp_case_insensitive (char c1, char c2) {
	  return (std::tolower(c1)==std::tolower(c2));
	}

	int main () {
	  int mychars[] = {'a','b','c','A','B','C'};
	  std::vector<char> haystack (mychars,mychars+6);
	  std::vector<char>::iterator it;

	  int needle[] = {'A','B','C'};

	  // using default comparison:
	  it = find_first_of (haystack.begin(), haystack.end(), needle, needle+3);

	  if (it!=haystack.end())
		std::cout << "The first match is: " << *it << '\n';

	  // using predicate comparison:
	  it = find_first_of (haystack.begin(), haystack.end(),
						  needle, needle+3, comp_case_insensitive);

	  if (it!=haystack.end())
		std::cout << "The first match is: " << *it << '\n';

	  return 0;
	}
	Output:
	The first match is: A
	The first match is: a
*/

// find_end, with and without an explicitly supplied comparison function.
// Search [first2, last2) as a subsequence in [first1, last1), and return
// the *last* possible match.  Note that find_end for bidirectional iterators
// is much faster than for forward iterators.

// find_end for forward iterators. 
//����ȡ�����ĵ���������Ϊ���������forward_iterator_tag������ô˺���
template <class _ForwardIter1, class _ForwardIter2>
_ForwardIter1 __find_end(_ForwardIter1 __first1, _ForwardIter1 __last1,
                         _ForwardIter2 __first2, _ForwardIter2 __last2,
                         forward_iterator_tag, forward_iterator_tag)
{
  if (__first2 == __last2)//���ڶ�������Ϊ��
    return __last1;//��ֱ�ӷ��ص�һ�������β��
  else {
    _ForwardIter1 __result = __last1;
    while (1) {
		//��������search�������ҳ�ĳ�������е��״γ��ֵ㣻���Ҳ���ֱ�ӷ���last1
      _ForwardIter1 __new_result

        = search(__first1, __last1, __first2, __last2);
      if (__new_result == __last1)//�����ص�λ��Ϊβ�ˣ����ʾû�ҵ�
        return __result;//����last1
      else {//����[first1,last1)���ҵ�[first2,last2)�״γ��ֵ�λ�ã�����׼����һ�β���
		  
        __result = __new_result;//���·��ص�λ��
        __first1 = __new_result;//���²��ҵ���ʼλ��
        ++__first1;//ȷ����ȷ������ʼλ��
      }
    }
  }
}
//�汾����ָ������
template <class _ForwardIter1, class _ForwardIter2,
          class _BinaryPredicate>
_ForwardIter1 __find_end(_ForwardIter1 __first1, _ForwardIter1 __last1,
                         _ForwardIter2 __first2, _ForwardIter2 __last2,
                         forward_iterator_tag, forward_iterator_tag,
                         _BinaryPredicate __comp)
{
  if (__first2 == __last2)
    return __last1;
  else {
    _ForwardIter1 __result = __last1;
    while (1) {
      _ForwardIter1 __new_result
        = search(__first1, __last1, __first2, __last2, __comp);
      if (__new_result == __last1)
        return __result;
      else {
        __result = __new_result;
        __first1 = __new_result;
        ++__first1;
      }
    }
  }
}

// find_end for bidirectional iterators.  Requires partial specialization.
#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
//����ȡ�����ĵ���������Ϊ˫�������bidirectional_iterator_tag������ô˺���
template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter1
__find_end(_BidirectionalIter1 __first1, _BidirectionalIter1 __last1,
           _BidirectionalIter2 __first2, _BidirectionalIter2 __last2,
           bidirectional_iterator_tag, bidirectional_iterator_tag)
{
  __STL_REQUIRES(_BidirectionalIter1, _BidirectionalIterator);
  __STL_REQUIRES(_BidirectionalIter2, _BidirectionalIterator);
  //���÷���������ܿ�Ϳ����ҵ�
  typedef reverse_iterator<_BidirectionalIter1> _RevIter1;
  typedef reverse_iterator<_BidirectionalIter2> _RevIter2;

  _RevIter1 __rlast1(__first1);
  _RevIter2 __rlast2(__first2);
  //����ʱ������һ�����ж��淽��
  _RevIter1 __rresult = search(_RevIter1(__last1), __rlast1,
                               _RevIter2(__last2), __rlast2);

  if (__rresult == __rlast1)//��ʾû�ҵ�
    return __last1;
  else {//�ҵ���
    _BidirectionalIter1 __result = __rresult.base();//ת������������
    advance(__result, -distance(__first2, __last2));//�����ص������е���ʼλ��
    return __result;
  }
}
//�汾����ָ������comp
template <class _BidirectionalIter1, class _BidirectionalIter2,
          class _BinaryPredicate>
_BidirectionalIter1
__find_end(_BidirectionalIter1 __first1, _BidirectionalIter1 __last1,
           _BidirectionalIter2 __first2, _BidirectionalIter2 __last2,
           bidirectional_iterator_tag, bidirectional_iterator_tag, 
           _BinaryPredicate __comp)
{
  __STL_REQUIRES(_BidirectionalIter1, _BidirectionalIterator);
  __STL_REQUIRES(_BidirectionalIter2, _BidirectionalIterator);
  typedef reverse_iterator<_BidirectionalIter1> _RevIter1;
  typedef reverse_iterator<_BidirectionalIter2> _RevIter2;

  _RevIter1 __rlast1(__first1);
  _RevIter2 __rlast2(__first2);
  _RevIter1 __rresult = search(_RevIter1(__last1), __rlast1,
                               _RevIter2(__last2), __rlast2,
                               __comp);

  if (__rresult == __rlast1)
    return __last1;
  else {
    _BidirectionalIter1 __result = __rresult.base();
    advance(__result, -distance(__first2, __last2));
    return __result;
  }
}
#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

// Dispatching functions for find_end.
//find_end�����������汾��
//�汾һ���ṩĬ�ϵ�equality����operator==
//�汾�����ṩ�û�����ָ���Ĳ�������comp
//ע�⣺����Ҳ��ƫ�ػ���֪ʶ
/*�������ܣ�Searches the range [first1,last1) for the last occurrence of the sequence defined by [first2,last2), 
and returns an iterator to its first element, or last1 if no occurrences are found.
����ԭ�ͣ�
equality (1)���汾һ	
	template <class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1 find_end (ForwardIterator1 first1, ForwardIterator1 last1,
                              ForwardIterator2 first2, ForwardIterator2 last2);
predicate (2)���汾��
	template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
	ForwardIterator1 find_end (ForwardIterator1 first1, ForwardIterator1 last1,
                              ForwardIterator2 first2, ForwardIterator2 last2,
                              BinaryPredicate pred);
*/
//����ӿڵİ汾һ
template <class _ForwardIter1, class _ForwardIter2>
inline _ForwardIter1 
find_end(_ForwardIter1 __first1, _ForwardIter1 __last1, 
         _ForwardIter2 __first2, _ForwardIter2 __last2)
{
  __STL_REQUIRES(_ForwardIter1, _ForwardIterator);
  __STL_REQUIRES(_ForwardIter2, _ForwardIterator);
  __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool,
   typename iterator_traits<_ForwardIter1>::value_type,
   typename iterator_traits<_ForwardIter2>::value_type);
  //����ͨ��iterator_traits��ȡ��first1��first2�ĵ���������
  //���ݲ�ͬ�ĵ��������͵��ò�ͬ�ĺ���
  return __find_end(__first1, __last1, __first2, __last2,
                    __ITERATOR_CATEGORY(__first1),
                    __ITERATOR_CATEGORY(__first2));
}
//����ӿڵİ汾һ
template <class _ForwardIter1, class _ForwardIter2, 
          class _BinaryPredicate>
inline _ForwardIter1 
find_end(_ForwardIter1 __first1, _ForwardIter1 __last1, 
         _ForwardIter2 __first2, _ForwardIter2 __last2,
         _BinaryPredicate __comp)
{
  __STL_REQUIRES(_ForwardIter1, _ForwardIterator);
  __STL_REQUIRES(_ForwardIter2, _ForwardIterator);
  __STL_BINARY_FUNCTION_CHECK(_BinaryPredicate, bool,
   typename iterator_traits<_ForwardIter1>::value_type,
   typename iterator_traits<_ForwardIter2>::value_type);
  //����ͨ��iterator_traits��ȡ��first1��first2�ĵ���������
  //���ݲ�ͬ�ĵ��������͵��ò�ͬ�ĺ���
  return __find_end(__first1, __last1, __first2, __last2,
                    __ITERATOR_CATEGORY(__first1),
                    __ITERATOR_CATEGORY(__first2),
                    __comp);
}
//find_end����������
/*
	#include <iostream>     // std::cout
	#include <algorithm>    // std::find_end
	#include <vector>       // std::vector

	bool myfunction (int i, int j) {
	  return (i==j);
	}

	int main () {
	  int myints[] = {1,2,3,4,5,1,2,3,4,5};
	  std::vector<int> haystack (myints,myints+10);

	  int needle1[] = {1,2,3};

	  // using default comparison:
	  std::vector<int>::iterator it;
	  it = std::find_end (haystack.begin(), haystack.end(), needle1, needle1+3);

	  if (it!=haystack.end())
		std::cout << "needle1 last found at position " << (it-haystack.begin()) << '\n';

	  int needle2[] = {4,5,1};

	  // using predicate comparison:
	  it = std::find_end (haystack.begin(), haystack.end(), needle2, needle2+3, myfunction);

	  if (it!=haystack.end())
		std::cout << "needle2 last found at position " << (it-haystack.begin()) << '\n';

	  return 0;
	}
	Output:
	needle1 found at position 5
	needle2 found at position 3
*/

// is_heap, a predicate testing whether or not a range is
// a heap.  This function is an extension, not part of the C++
// standard.

template <class _RandomAccessIter, class _Distance>
bool __is_heap(_RandomAccessIter __first, _Distance __n)
{
  _Distance __parent = 0;
  for (_Distance __child = 1; __child < __n; ++__child) {
    if (__first[__parent] < __first[__child]) 
      return false;
    if ((__child & 1) == 0)
      ++__parent;
  }
  return true;
}

template <class _RandomAccessIter, class _Distance, class _StrictWeakOrdering>
bool __is_heap(_RandomAccessIter __first, _StrictWeakOrdering __comp,
               _Distance __n)
{
  _Distance __parent = 0;
  for (_Distance __child = 1; __child < __n; ++__child) {
    if (__comp(__first[__parent], __first[__child]))
      return false;
    if ((__child & 1) == 0)
      ++__parent;
  }
  return true;
}

template <class _RandomAccessIter>
inline bool is_heap(_RandomAccessIter __first, _RandomAccessIter __last)
{
  __STL_REQUIRES(_RandomAccessIter, _RandomAccessIterator);
  __STL_REQUIRES(typename iterator_traits<_RandomAccessIter>::value_type,
                 _LessThanComparable);
  return __is_heap(__first, __last - __first);
}


template <class _RandomAccessIter, class _StrictWeakOrdering>
inline bool is_heap(_RandomAccessIter __first, _RandomAccessIter __last,
                    _StrictWeakOrdering __comp)
{
  __STL_REQUIRES(_RandomAccessIter, _RandomAccessIterator);
  __STL_BINARY_FUNCTION_CHECK(_StrictWeakOrdering, bool, 
         typename iterator_traits<_RandomAccessIter>::value_type, 
         typename iterator_traits<_RandomAccessIter>::value_type);
  return __is_heap(__first, __comp, __last - __first);
}

// is_sorted, a predicated testing whether a range is sorted in
// nondescending order.  This is an extension, not part of the C++
// standard.

template <class _ForwardIter>
bool is_sorted(_ForwardIter __first, _ForwardIter __last)
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES(typename iterator_traits<_ForwardIter>::value_type,
                 _LessThanComparable);
  if (__first == __last)
    return true;

  _ForwardIter __next = __first;
  for (++__next; __next != __last; __first = __next, ++__next) {
    if (*__next < *__first)
      return false;
  }

  return true;
}

template <class _ForwardIter, class _StrictWeakOrdering>
bool is_sorted(_ForwardIter __first, _ForwardIter __last,
               _StrictWeakOrdering __comp)
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_BINARY_FUNCTION_CHECK(_StrictWeakOrdering, bool, 
        typename iterator_traits<_ForwardIter>::value_type,
        typename iterator_traits<_ForwardIter>::value_type);
  if (__first == __last)
    return true;

  _ForwardIter __next = __first;
  for (++__next; __next != __last; __first = __next, ++__next) {
    if (__comp(*__next, *__first))
      return false;
  }

  return true;
}

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1209
#endif

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_ALGO_H */

// Local Variables:
// mode:C++
// End:
