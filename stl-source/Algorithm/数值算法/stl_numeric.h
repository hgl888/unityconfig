#ifndef __SGI_STL_INTERNAL_NUMERIC_H
#define __SGI_STL_INTERNAL_NUMERIC_H

__STL_BEGIN_NAMESPACE
/*
	sum (1)	:The default operation is to add the elements up;
����汾��Ĭ�ϲ���ʱ�ۼӣ�
		template <class InputIterator, class T>
		T accumulate (InputIterator first, InputIterator last, T init);

	custom (2):	a different operation can be specified as binary_op;
����汾�Ĳ����������û�ͨ��binary_op����ָ����ָ��������<stl_function.h>���壬Ҳ���Լ�����
		template <class InputIterator, class T, class BinaryOperation>
		T accumulate (InputIterator first, InputIterator last, T init,
					 BinaryOperation binary_op);
*/
//��һ���汾��Ĭ�ϲ������ۼ�
//����[first,last)����Ԫ����init�ĺ�
//����һ������
template <class _InputIterator, class _Tp>
_Tp accumulate(_InputIterator __first, _InputIterator __last, _Tp __init)
{
  __STL_REQUIRES(_InputIterator, _InputIterator);
  for ( ; __first != __last; ++__first)//����ָ����ΧԪ��
    __init = __init + *__first;//��ÿ��Ԫ���ۼӵ���ʼֵinit��
  return __init;
}
//�ڶ����汾���û�������ָ����Ԫ��������
template <class _InputIterator, class _Tp, class _BinaryOperation>
_Tp accumulate(_InputIterator __first, _InputIterator __last, _Tp __init,
               _BinaryOperation __binary_op)
{
  __STL_REQUIRES(_InputIterator, _InputIterator);
  for ( ; __first != __last; ++__first)//����ָ����ΧԪ��
    __init = __binary_op(__init, *__first);//��ÿ��Ԫ��ִ�ж�Ԫ����
  return __init;
}
//��������ӣ�
/*accumulate example:
	#include <iostream>     // std::cout
	#include <functional>   // std::minus
	#include <numeric>      // std::accumulate

	int myfunction (int x, int y) {return x+2*y;}
	struct myclass {
		int operator()(int x, int y) {return x+3*y;}
	} myobject;

	int main () {
	  int init = 100;
	  int numbers[] = {10,20,30};

	  std::cout << "using default accumulate: ";
	  std::cout << std::accumulate(numbers,numbers+3,init);
	  std::cout << '\n';

	  std::cout << "using functional's minus: ";
	  std::cout << std::accumulate (numbers, numbers+3, init, std::minus<int>());
	  std::cout << '\n';

	  std::cout << "using custom function: ";
	  std::cout << std::accumulate (numbers, numbers+3, init, myfunction);
	  std::cout << '\n';

	  std::cout << "using custom object: ";
	  std::cout << std::accumulate (numbers, numbers+3, init, myobject);
	  std::cout << '\n';

	  return 0;
	}
	Output:
	using default accumulate: 160
	using functional's minus: 40
	using custom function: 220
	using custom object: 280
	*/

/*Ĭ�ϲ����ǰ��ڻ�(���)��ֵ���ʼֵinit���
�û���������ָ����������
	sum/multiply (1)	
		template <class InputIterator1, class InputIterator2, class T>
		T inner_product (InputIterator1 first1, InputIterator1 last1,
						InputIterator2 first2, T init);
	custom (2)	
		template <class InputIterator1, class InputIterator2, class T,
			  class BinaryOperation1, class BinaryOperation2>
		T inner_product (InputIterator1 first1, InputIterator1 last1,
						InputIterator2 first2, T init,
						BinaryOperation1 binary_op1,
						BinaryOperation2 binary_op2);
*/
//������The two default operations (to add up the result of multiplying the pairs) 
//may be overridden by the arguments binary_op1 and binary_op2.
//���ܣ�Returns the result of accumulating init with the inner products of the pairs 
//formed by the elements of two ranges starting at first1 and first2.

//�汾һ��ʹ��Ĭ�ϲ���
template <class _InputIterator1, class _InputIterator2, class _Tp>
_Tp inner_product(_InputIterator1 __first1, _InputIterator1 __last1,
                  _InputIterator2 __first2, _Tp __init)
{
  __STL_REQUIRES(_InputIterator2, _InputIterator);
  __STL_REQUIRES(_InputIterator2, _InputIterator);
//�Ե�һ�����е�Ԫ�ظ���Ϊ�ݣ����������ж���һ��
  for ( ; __first1 != __last1; ++__first1, ++__first2)��
    __init = __init + (*__first1 * *__first2);//ִ���������е��ڻ����ʼֵinit���
  return __init;
}

//�汾�����û�������ָ����Ԫ��������
template <class _InputIterator1, class _InputIterator2, class _Tp,
          class _BinaryOperation1, class _BinaryOperation2>
_Tp inner_product(_InputIterator1 __first1, _InputIterator1 __last1,
                  _InputIterator2 __first2, _Tp __init, 
                  _BinaryOperation1 __binary_op1,
                  _BinaryOperation2 __binary_op2)
{
  __STL_REQUIRES(_InputIterator2, _InputIterator);
  __STL_REQUIRES(_InputIterator2, _InputIterator);
  //�Ե�һ�����е�Ԫ�ظ���Ϊ�ݣ����������ж���һ��
  for ( ; __first1 != __last1; ++__first1, ++__first2)
	  //����ָ��__binary_op2��������ָ��__binary_op1����
    __init = __binary_op1(__init, __binary_op2(*__first1, *__first2));
  return __init;
}
//��������ӣ�
/*inner_product example:
	#include <iostream>     // std::cout
	#include <functional>   // std::minus, std::divides
	#include <numeric>      // std::inner_product

	int myaccumulator (int x, int y) {return x-y;}
	int myproduct (int x, int y) {return x+y;}

	int main () {
	  int init = 10;
	  int series1[] = {10,20,30};
	  int series2[] = {1,2,3};

	  std::cout << "using default inner_product: ";
	  std::cout << std::inner_product(series1,series1+3,series2,init);
	  std::cout << '\n';

	  std::cout << "using functional operations: ";
	  std::cout << std::inner_product(series1,series1+3,series2,init,
									  std::minus<int>(),std::divides<int>());
	  std::cout << '\n';

	  std::cout << "using custom functions: ";
	  std::cout << std::inner_product(series1,series1+3,series2,init,
									  myaccumulator,myproduct);
	  std::cout << '\n';

	  return 0;
	}
	Output:
	using default inner_product: 150
	using functional operations: -20
	using custom functions: -56
*/

//�ֲ����
/*������������
Assigns to every element in the range starting at result the partial sum of 
the corresponding elements in the range [first,last).

If x represents an element in [first,last) and y represents an element in result, the ys can be calculated as:

y0 = x0 
y1 = x0 + x1 
y2 = x0 + x1 + x2 
y3 = x0 + x1 + x2 + x3 
y4 = x0 + x1 + x2 + x3 + x4 
... ... ...
*/
/*�汾һ��Ĭ�ϲ����ǣ�The default operation is to add the elements up;
	sum (1)	
		template <class InputIterator, class OutputIterator>
		OutputIterator partial_sum (InputIterator first, InputIterator last,
								   OutputIterator result);
�汾�����û���������ָ����Ԫ������operation can be specified as binary_op instead.
	custom (2)	
		template <class InputIterator, class OutputIterator, class BinaryOperation>
		OutputIterator partial_sum (InputIterator first, InputIterator last,
								   OutputIterator result, BinaryOperation binary_op);
*/

//�汾һ��Ĭ�ϲ�������
template <class _InputIterator, class _OutputIterator, class _Tp>
_OutputIterator 
__partial_sum(_InputIterator __first, _InputIterator __last,
              _OutputIterator __result, _Tp*)
{
  _Tp __value = *__first;
  while (++__first != __last) {//��������Ԫ��
    __value = __value + *__first;//����ǰn��Ԫ�ص��ܺ�
    *++__result = __value;//��Ԫ�ظ��������
  }
  return ++__result;
}

template <class _InputIterator, class _OutputIterator>
_OutputIterator 
partial_sum(_InputIterator __first, _InputIterator __last,
            _OutputIterator __result)
{
  __STL_REQUIRES(_InputIterator, _InputIterator);
  __STL_REQUIRES(_OutputIterator, _OutputIterator);
  if (__first == __last) return __result;//��Ϊ��
  *__result = *__first;//��ʼֵ
  //��������ĺ�������ȡ��first�����ͷ������溯��ʹ��
  return __partial_sum(__first, __last, __result, __VALUE_TYPE(__first));
}
//�汾�����û�ָ����Ԫ��������
template <class _InputIterator, class _OutputIterator, class _Tp,
          class _BinaryOperation>
_OutputIterator 
__partial_sum(_InputIterator __first, _InputIterator __last, 
              _OutputIterator __result, _Tp*, _BinaryOperation __binary_op)
{
  _Tp __value = *__first;
  while (++__first != __last) {//��������Ԫ��
    __value = __binary_op(__value, *__first);//����ǰn��Ԫ�ص�__binary_op
    *++__result = __value;//��Ԫ�ظ��������
  }
  return ++__result;
}

template <class _InputIterator, class _OutputIterator, class _BinaryOperation>
_OutputIterator 
partial_sum(_InputIterator __first, _InputIterator __last,
            _OutputIterator __result, _BinaryOperation __binary_op)
{
  __STL_REQUIRES(_InputIterator, _InputIterator);
  __STL_REQUIRES(_OutputIterator, _OutputIterator);
  if (__first == __last) return __result;
  *__result = *__first;
  //��������ĺ�������ȡ��first�����ͷ������溯��ʹ��
  return __partial_sum(__first, __last, __result, __VALUE_TYPE(__first), 
                       __binary_op);
}
//���������
/*partial_sum example:
	#include <iostream>     // std::cout
	#include <functional>   // std::multiplies
	#include <numeric>      // std::partial_sum

	int myop (int x, int y) {return x+y+1;}

	int main () {
	  int val[] = {1,2,3,4,5};
	  int result[5];

	  std::partial_sum (val, val+5, result);
	  std::cout << "using default partial_sum: ";
	  for (int i=0; i<5; i++) std::cout << result[i] << ' ';
	  std::cout << '\n';

	  std::partial_sum (val, val+5, result, std::multiplies<int>());
	  std::cout << "using functional operation multiplies: ";
	  for (int i=0; i<5; i++) std::cout << result[i] << ' ';
	  std::cout << '\n';

	  std::partial_sum (val, val+5, result, myop);
	  std::cout << "using custom function: ";
	  for (int i=0; i<5; i++) std::cout << result[i] << ' ';
	  std::cout << '\n';
	  return 0;
	}
	Output:
	using default partial_sum: 1 3 6 10 15 
	using functional operation multiplies: 1 2 6 24 120 
	using custom function: 1 4 8 13 19
*/


/*������������
Assigns to every element in the range starting at result the difference 
between its corresponding element in the range [first,last) 
and the one preceding it (except for *result, which is assigned *first).

If x represents an element in [first,last) and y represents an element in result, the ys can be calculated as:

y0 = x0 
y1 = x1 - x0 
y2 = x2 - x1 
y3 = x3 - x2 
y4 = x4 - x3 
... ... ...

The default operation is to calculate the difference, but some other operation can be specified as binary_op instead.

*/
/*
	difference (1)	
		template <class InputIterator, class OutputIterator>
		OutputIterator adjacent_difference (InputIterator first, InputIterator last,
										   OutputIterator result);
	custom (2)	
		template <class InputIterator, class OutputIterator, class BinaryOperation>
		OutputIterator adjacent_difference ( InputIterator first, InputIterator last,
											OutputIterator result, BinaryOperation binary_op );
*/
//�汾һ��Ĭ�ϲ�������
template <class _InputIterator, class _OutputIterator, class _Tp>
_OutputIterator 
__adjacent_difference(_InputIterator __first, _InputIterator __last,
                      _OutputIterator __result, _Tp*)
{
  _Tp __value = *__first;
  while (++__first != __last) {//��������
    _Tp __tmp = *__first;//��ʼ��tmp
    *++__result = __tmp - __value;//����������Ԫ�صĲ��(��-ǰ)�������������
    __value = __tmp;//���µ�ǰֵ
  }
  return ++__result;
}

template <class _InputIterator, class _OutputIterator>
_OutputIterator
adjacent_difference(_InputIterator __first,
                    _InputIterator __last, _OutputIterator __result)
{
  __STL_REQUIRES(_InputIterator, _InputIterator);
  __STL_REQUIRES(_OutputIterator, _OutputIterator);
  if (__first == __last) return __result;//��Ϊ��ֱ�ӷ���
  *__result = *__first;//��ʼֵ
  //��������ĺ���__adjacent_difference()
  return __adjacent_difference(__first, __last, __result,
                               __VALUE_TYPE(__first));
}
//�汾������ָ����������
template <class _InputIterator, class _OutputIterator, class _Tp, 
          class _BinaryOperation>
_OutputIterator
__adjacent_difference(_InputIterator __first, _InputIterator __last, 
                      _OutputIterator __result, _Tp*,
                      _BinaryOperation __binary_op) {
  _Tp __value = *__first;
  while (++__first != __last) {//��������
    _Tp __tmp = *__first;//��ʼ��tmp
    *++__result = __binary_op(__tmp, __value);//����������Ԫ�صĲ����������������
    __value = __tmp;//���µ�ǰֵ
  }
  return ++__result;
}

template <class _InputIterator, class _OutputIterator, class _BinaryOperation>
_OutputIterator 
adjacent_difference(_InputIterator __first, _InputIterator __last,
                    _OutputIterator __result, _BinaryOperation __binary_op)
{
  __STL_REQUIRES(_InputIterator, _InputIterator);
  __STL_REQUIRES(_OutputIterator, _OutputIterator);
  if (__first == __last) return __result;//��Ϊ��ֱ�ӷ���
  *__result = *__first;//��ʼֵ
  //��������ĺ���__adjacent_difference()
  return __adjacent_difference(__first, __last, __result,
                               __VALUE_TYPE(__first),
                               __binary_op);
}
//��������ӣ�
/*adjacent_difference example:
	#include <iostream>     // std::cout
	#include <functional>   // std::multiplies
	#include <numeric>      // std::adjacent_difference

	int myop (int x, int y) {return x+y;}

	int main () {
	  int val[] = {1,2,3,5,9,11,12};
	  int result[7];

	  std::adjacent_difference (val, val+7, result);
	  std::cout << "using default adjacent_difference: ";
	  for (int i=0; i<7; i++) std::cout << result[i] << ' ';
	  std::cout << '\n';

	  std::adjacent_difference (val, val+7, result, std::multiplies<int>());
	  std::cout << "using functional operation multiplies: ";
	  for (int i=0; i<7; i++) std::cout << result[i] << ' ';
	  std::cout << '\n';

	  std::adjacent_difference (val, val+7, result, myop);
	  std::cout << "using custom function: ";
	  for (int i=0; i<7; i++) std::cout << result[i] << ' ';
	  std::cout << '\n';
	  return 0;
	}
	output:
	using default adjacent_difference: 1 1 1 2 4 2 1
	using functional operation multiplies: 1 2 6 15 45 99 132
	using custom function: 1 3 5 8 14 20 23
*/



// Returns __x ** __n, where __n >= 0.  _Note that "multiplication"
// is required to be associative, but not necessarily commutative.
//powerΪSGIר����������STL��׼֮�У�����������ĳ����n�ݴη���  
  
  
// �汾һ���ݴη������ָ��Ϊ�˷����㣬��n >= 0 ʱ���� x^n��  
// ע�⣬"multiplication" �����������ɣ�associative����  
// ���������㽻���ɣ�commutative����  
template <class _Tp, class _Integer, class _MonoidOperation>
_Tp __power(_Tp __x, _Integer __n, _MonoidOperation __opr)
{
  if (__n == 0)
    return identity_element(__opr);
  else {
    while ((__n & 1) == 0) {
      __n >>= 1;
      __x = __opr(__x, __x);
    }

    _Tp __result = __x;
    __n >>= 1;
    while (__n != 0) {
      __x = __opr(__x, __x);
      if ((__n & 1) != 0)
        __result = __opr(__result, __x);
      __n >>= 1;
    }
    return __result;
  }
}
//�汾������ multiplies<_Tp>()�������ð汾һ
template <class _Tp, class _Integer>
inline _Tp __power(_Tp __x, _Integer __n)
{
  return __power(__x, __n, multiplies<_Tp>());
}

// Alias for the internal name __power.  Note that power is an extension,
// not part of the C++ standard.
//�����溯���ķ�װ
template <class _Tp, class _Integer, class _MonoidOperation>
inline _Tp power(_Tp __x, _Integer __n, _MonoidOperation __opr)
{
  return __power(__x, __n, __opr);
}

template <class _Tp, class _Integer>
inline _Tp power(_Tp __x, _Integer __n)
{
  return __power(__x, __n);
}

// iota is not part of the C++ standard.  It is an extension.
//C++11�Ѿ��������ΪSTL�ı�׼
//�趨ĳ����������ݣ�ʹ��ÿ��Ԫ�ش�ָ��ֵvalue��ʼ�����ֵ���
//�� [first,last) ��Χ�ڃ�����value, value+1, value+2...  
template <class _ForwardIter, class _Tp>
void 
iota(_ForwardIter __first, _ForwardIter __last, _Tp __value)
{
  __STL_REQUIRES(_ForwardIter, _Mutable_ForwardIterator);
  __STL_CONVERTIBLE(_Tp, typename iterator_traits<_ForwardIter>::value_type);
  while (__first != __last)
    *__first++ = __value++;
}
//�����ӣ�
/*iota example
	#include <iostream>     // std::cout
	#include <numeric>      // std::iota

	int main () {
	  int numbers[10];

	  std::iota (numbers,numbers+10,100);

	  std::cout << "numbers:";
	  for (int& i:numbers) std::cout << ' ' << i;
	  std::cout << '\n';

	  return 0;
	}
	output:
	numbers: 100 101 102 103 104 105 106 107 108 109
*/

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_NUMERIC_H */

// Local Variables:
// mode:C++
// End: