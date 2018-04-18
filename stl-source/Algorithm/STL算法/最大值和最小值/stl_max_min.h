
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