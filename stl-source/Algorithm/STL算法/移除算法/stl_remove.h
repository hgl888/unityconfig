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