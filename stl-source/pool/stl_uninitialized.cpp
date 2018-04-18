/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

#ifndef __SGI_STL_INTERNAL_UNINITIALIZED_H
#define __SGI_STL_INTERNAL_UNINITIALIZED_H

__STL_BEGIN_NAMESPACE

// uninitialized_copy

// Valid if copy construction is equivalent to assignment, and if the
//  destructor is trivial.
template <class _InputIter, class _ForwardIter>
/*�ú�����������������������������first���������ʼ��ַ��
*������last������Ľ�����ַ��������result���������ʼ��ַ
*�������ݸ��Ƶ�[result,result+(last-first)]�����Χ
*Ϊ�����Ч�ʣ�������__VALUE_TYPE()��ȡ��������result���ͱ�value_type
*������__type_traits�жϸ��ͱ��Ƿ�ΪPOD�ͱ�
*/
inline _ForwardIter
  uninitialized_copy(_InputIter __first, _InputIter __last,
                     _ForwardIter __result)
{
  return __uninitialized_copy(__first, __last, __result,
                              __VALUE_TYPE(__result));
}

template <class _InputIter, class _ForwardIter, class _Tp>
inline _ForwardIter
/*����__type_traits�жϸ��ͱ��Ƿ�ΪPOD�ͱ�*/
__uninitialized_copy(_InputIter __first, _InputIter __last,
                     _ForwardIter __result, _Tp*)
{
  typedef typename __type_traits<_Tp>::is_POD_type _Is_POD;
  return __uninitialized_copy_aux(__first, __last, __result, _Is_POD());
}

template <class _InputIter, class _ForwardIter>
_ForwardIter //������POD�ͱ𣬾����͵�����
__uninitialized_copy_aux(_InputIter __first, _InputIter __last,
                         _ForwardIter __result,
                         __false_type)
{
  _ForwardIter __cur = __result;
  __STL_TRY {//����������쳣�������
    for ( ; __first != __last; ++__first, ++__cur)
      _Construct(&*__cur, *__first);//������󣬱�����һ��һ��Ԫ�صĹ��죬��������
    return __cur;
  }
  __STL_UNWIND(_Destroy(__result, __cur));//��������
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter //����POD�ͱ𣬾����͵�����
__uninitialized_copy_aux(_InputIter __first, _InputIter __last,
                         _ForwardIter __result,
                         __true_type)
{
	/*����STL���㷨copy()
	*����ԭ�ͣ�template< class InputIt, class OutputIt >
	* OutputIt copy( InputIt first, InputIt last, OutputIt d_first );
	*/
	return copy(__first, __last, __result);
}
//���������char*��wchar_t* ��uninitialized_copy()�ػ��汾
inline char* uninitialized_copy(const char* __first, const char* __last,
                                char* __result) {
/* void* memmove( void* dest, const void* src, std::size_t count );
* destָ���������ʼ��ַ
* srcָ���������ʵ��ַ
* countҪ���Ƶ��ֽ���
*/
  memmove(__result, __first, __last - __first);
  return __result + (__last - __first);
}

inline wchar_t* 
uninitialized_copy(const wchar_t* __first, const wchar_t* __last,
                   wchar_t* __result)
{
  memmove(__result, __first, sizeof(wchar_t) * (__last - __first));
  return __result + (__last - __first);
}


// Valid if copy construction is equivalent to assignment, and if the
// destructor is trivial.
template <class _ForwardIter, class _Tp>
/*����POD�ͱ�����ô˺���
	*/
inline void
__uninitialized_fill_aux(_ForwardIter __first, _ForwardIter __last, 
                         const _Tp& __x, __true_type)
{
/*����ԭ�ͣ�template< class ForwardIt, class T >
  * void fill( ForwardIt first, ForwardIt last, const T& value );  
  */
	fill(__first, __last, __x);
}

template <class _ForwardIter, class _Tp>
/*������POD�ͱ�����ô˺���
	*/
void
__uninitialized_fill_aux(_ForwardIter __first, _ForwardIter __last, 
                         const _Tp& __x, __false_type)
{
  _ForwardIter __cur = __first;
  __STL_TRY {
    for ( ; __cur != __last; ++__cur)
      _Construct(&*__cur, __x);
  }
  __STL_UNWIND(_Destroy(__first, __cur));
}

template <class _ForwardIter, class _Tp, class _Tp1>
//��__type_traits�����жϸ��ͱ��Ƿ�
ΪPOD�ͱ�
inline void __uninitialized_fill(_ForwardIter __first, 
                                 _ForwardIter __last, const _Tp& __x, _Tp1*)
{
  typedef typename __type_traits<_Tp1>::is_POD_type _Is_POD;
  __uninitialized_fill_aux(__first, __last, __x, _Is_POD());
                   
}

template <class _ForwardIter, class _Tp>
/*�ú�����������������
*������firstָ������ʼ���Ŀռ���ʼ��ַ
*������lastָ������ʼ���Ŀռ������ַ
*x��ʾ��ֵ
*��������__VALUE_TYPE()��ȡ��������first���ͱ�value_type
*Ȼ����__type_traits�����жϸ��ͱ��Ƿ�ΪPOD�ͱ�
*/
inline void uninitialized_fill(_ForwardIter __first,
                               _ForwardIter __last, 
                               const _Tp& __x)
{
  __uninitialized_fill(__first, __last, __x, __VALUE_TYPE(__first));
}

// Valid if copy construction is equivalent to assignment, and if the
//  destructor is trivial.
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter
	/*����POD�ͱ�����ô˺���
	*/
__uninitialized_fill_n_aux(_ForwardIter __first, _Size __n,
                           const _Tp& __x, __true_type)
{
  /*����STL�㷨
  *ԭ�ͣ�template< class OutputIt, class Size, class T >
  * void fill_n( OutputIt first, Size count, const T& value );
  * template< class OutputIt, class Size, class T >
  * OutputIt fill_n( OutputIt first, Size count, const T& value );
  */
	return fill_n(__first, __n, __x);
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter
	/*������POD�ͱ�����ô˺���
	*/
__uninitialized_fill_n_aux(_ForwardIter __first, _Size __n,
                           const _Tp& __x, __false_type)
{
  _ForwardIter __cur = __first;
  __STL_TRY {
    for ( ; __n > 0; --__n, ++__cur)
      _Construct(&*__cur, __x);
    return __cur;
  }
  __STL_UNWIND(_Destroy(__first, __cur));
}

template <class _ForwardIter, class _Size, class _Tp, class _Tp1>
inline _ForwardIter 
	//��__type_traits�����жϸ��ͱ��Ƿ�ΪPOD�ͱ�
__uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, _Tp1*)
{
  typedef typename __type_traits<_Tp1>::is_POD_type _Is_POD;
  //_Is_POD()�ж�value_type�Ƿ�ΪPOD�ͱ�
  return __uninitialized_fill_n_aux(__first, __n, __x, _Is_POD());
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter 
/*�ú�����������������
*������firstָ������ʼ���Ŀռ���ʼ��ַ
*n��ʾ����ʼ���ռ��С
*x��ʾ��ֵ
*��������__VALUE_TYPE()��ȡ��������first���ͱ�value_type
*Ȼ����__type_traits�����жϸ��ͱ��Ƿ�ΪPOD�ͱ�
*/
uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
  //__VALUE_TYPE(__first)��ȡ��first���ͱ�value_type
	return __uninitialized_fill_n(__first, __n, __x, __VALUE_TYPE(__first));
}



__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_UNINITIALIZED_H */

// Local Variables:
// mode:C++
// End:
