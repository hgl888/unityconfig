#ifndef __SGI_STL_INTERNAL_PAIR_H
#define __SGI_STL_INTERNAL_PAIR_H

__STL_BEGIN_NAMESPACE

/*
pair�ڹ���������ʹ�úܹ㷺,����STL��ģ������,���Դ洢������Ա����
pair���õ���struct�ṹ,struct�ĳ�ԱĬ��������public
*/
template <class _T1, class _T2>
struct pair {
  typedef _T1 first_type;
  typedef _T2 second_type;

  //pair��������Ա����,��������public
  _T1 first;
  _T2 second;
  //�����ǹ��캯��
  pair() : first(_T1()), second(_T2()) {}
  pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}

#ifdef __STL_MEMBER_TEMPLATES
  //�����ԵĿ������캯��
  //��������ָ����pair�����Ϳ��Բ�ͬ,���Ǳ������ת��
  template <class _U1, class _U2>
  pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}
  //ע��:��pair��ʼ����һ��pairʱ,ֻ��ͨ���������캯������,����ͨ����ֵ����
  //��Ϊ����û���ṩoperator=������������
#endif
};

//operator==����������
//����pair���ʱ����ζ��������Ա��������Ӧ���
template <class _T1, class _T2>
inline bool operator==(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{ 
  return __x.first == __y.first && __x.second == __y.second; 
}

//operator<����������
//�Ƚ�����pairʱ,�Ե�һ����Ա����firstΪ��,����һ����Ա����first�����жϱ��ʽ�Ĵ�С
//�����ڶ�����Ա����second���бȽ�
template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{ 
  return __x.first < __y.first || 
         (!(__y.first < __x.first) && __x.second < __y.second); 
}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

//����Ĳ��������ض��ǻ�������operator<��operator==��������.
//operator!=,operator>,operator<=,operator>=������������
template <class _T1, class _T2>
inline bool operator!=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) {
  return !(__x == __y);
}

template <class _T1, class _T2>
inline bool operator>(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) {
  return __y < __x;
}

template <class _T1, class _T2>
inline bool operator<=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) {
  return !(__y < __x);
}

template <class _T1, class _T2>
inline bool operator>=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) {
  return !(__x < __y);
}

#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

//����������ֵ������һ��pair
template <class _T1, class _T2>
inline pair<_T1, _T2> make_pair(const _T1& __x, const _T2& __y)
{
  return pair<_T1, _T2>(__x, __y);
}

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_PAIR_H */

// Local Variables:
// mode:C++
// End:
