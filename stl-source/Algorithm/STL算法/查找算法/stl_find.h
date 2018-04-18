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