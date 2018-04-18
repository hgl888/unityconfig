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
