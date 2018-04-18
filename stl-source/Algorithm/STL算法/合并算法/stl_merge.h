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