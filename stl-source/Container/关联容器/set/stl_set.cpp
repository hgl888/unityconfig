#ifndef __SGI_STL_INTERNAL_SET_H
#define __SGI_STL_INTERNAL_SET_H

#include <concept_checks.h>

__STL_BEGIN_NAMESPACE

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1174
#pragma set woff 1375
#endif
//set�ڲ�Ԫ��Ĭ��ʹ�õ�������less
//�û��������ƶ��Ƚ�����
//�ڲ�ά�������ݽṹ�Ǻ����, ���зǳ������������ʱ�临�Ӷ�  
//ע��: set��ֵ��ʵֵ����ͬ��,set�ڲ������ظ�Ԫ�صĴ���, ��������ظ�Ԫ��,�����Բ������ 
//�Ҳ���ͨ���������޸�set��Ԫ��ֵ,������������Ƕ���ΪRB-Tree��const_iterator

// Forward declarations of operators < and ==, needed for friend declaration.

template <class _Key, class _Compare __STL_DEPENDENT_DEFAULT_TMPL(less<_Key>),
          class _Alloc = __STL_DEFAULT_ALLOCATOR(_Key) >
class set;

template <class _Key, class _Compare, class _Alloc>
inline bool operator==(const set<_Key,_Compare,_Alloc>& __x, 
                       const set<_Key,_Compare,_Alloc>& __y);

template <class _Key, class _Compare, class _Alloc>
inline bool operator<(const set<_Key,_Compare,_Alloc>& __x, 
                      const set<_Key,_Compare,_Alloc>& __y);


template <class _Key, class _Compare, class _Alloc>
class set {
  // requirements:

  __STL_CLASS_REQUIRES(_Key, _Assignable);
  __STL_CLASS_BINARY_FUNCTION_CHECK(_Compare, bool, _Key, _Key);

public:
  // typedefs:
 //��set��key����value, valueͬʱҲ��key 
  typedef _Key     key_type;
  typedef _Key     value_type;
  //ע�⣺����key_compare��value_compareʹ����ͬ�ıȽϺ��� 
  typedef _Compare key_compare;
  typedef _Compare value_compare;
private:
	//set�ĵײ�����ǲ���RB-Tree���ݽṹ����<stl_tree.h>ʵ��
  typedef _Rb_tree<key_type, value_type, 
                  _Identity<value_type>, key_compare, _Alloc> _Rep_type;
  _Rep_type _M_t;  // red-black tree representing set
public:
  typedef typename _Rep_type::const_pointer pointer;
  typedef typename _Rep_type::const_pointer const_pointer;
  typedef typename _Rep_type::const_reference reference;
  typedef typename _Rep_type::const_reference const_reference;
  //set�ĵ�����iterator ����ΪRB-Tree��const_iterator���������û�ͨ���������޸�set��Ԫ��ֵ  
  //��Ϊset��Ԫ����һ�������ţ��޸���ֵ���ƻ�������� 
  typedef typename _Rep_type::const_iterator iterator;
  typedef typename _Rep_type::const_iterator const_iterator;
  typedef typename _Rep_type::const_reverse_iterator reverse_iterator;
  typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;
  typedef typename _Rep_type::size_type size_type;
  typedef typename _Rep_type::difference_type difference_type;
  typedef typename _Rep_type::allocator_type allocator_type;

  // allocation/deallocation

 // setֻ��ʹ��RB-tree��insert-unique(),����ʹ��insert-equal() 
  //��Ҫ�����ֵ���Ѿ����ڵļ�ֵ��ͬʱ���ͻᱻ���� 
  /*���캯��ԭ��
	set();
	explicit set (const key_compare& comp = key_compare(),
              const allocator_type& alloc = allocator_type());
	
	template <class InputIterator>
	set (InputIterator first, InputIterator last,
       const key_compare& comp = key_compare(),
       const allocator_type& alloc = allocator_type());
	
	set (const set& x);
  */
  set() : _M_t(_Compare(), allocator_type()) {}
  explicit set(const _Compare& __comp,
               const allocator_type& __a = allocator_type())
    : _M_t(__comp, __a) {}

#ifdef __STL_MEMBER_TEMPLATES
  template <class _InputIterator>
  set(_InputIterator __first, _InputIterator __last)
    : _M_t(_Compare(), allocator_type())
    { _M_t.insert_unique(__first, __last); }

  template <class _InputIterator>
  set(_InputIterator __first, _InputIterator __last, const _Compare& __comp,
      const allocator_type& __a = allocator_type())
    : _M_t(__comp, __a) { _M_t.insert_unique(__first, __last); }
#else
  set(const value_type* __first, const value_type* __last) 
    : _M_t(_Compare(), allocator_type()) 
    { _M_t.insert_unique(__first, __last); }

  set(const value_type* __first, 
      const value_type* __last, const _Compare& __comp,
      const allocator_type& __a = allocator_type())
    : _M_t(__comp, __a) { _M_t.insert_unique(__first, __last); }

  set(const_iterator __first, const_iterator __last)
    : _M_t(_Compare(), allocator_type()) 
    { _M_t.insert_unique(__first, __last); }

  set(const_iterator __first, const_iterator __last, const _Compare& __comp,
      const allocator_type& __a = allocator_type())
    : _M_t(__comp, __a) { _M_t.insert_unique(__first, __last); }
#endif /* __STL_MEMBER_TEMPLATES */

  set(const set<_Key,_Compare,_Alloc>& __x) : _M_t(__x._M_t) {}
  set<_Key,_Compare,_Alloc>& operator=(const set<_Key, _Compare, _Alloc>& __x)
  { 
    _M_t = __x._M_t;//�����˵ײ�������operator=��������
    return *this;
  }

  //�������е�set������Ϊ��RB-tree�����ṩ������setֻҪ���ü��� 
  // accessors:

   //��������key�Ƚϵĺ���,����RB-Tree��key_comp()
  key_compare key_comp() const { return _M_t.key_comp(); }
  //����set������, value��keyʹ��ͬһ���ȽϺ���
  value_compare value_comp() const { return _M_t.key_comp(); }
  //��ȡ����������
  allocator_type get_allocator() const { return _M_t.get_allocator(); }

  //������һЩset�Ļ�����������ָλ��
  iterator begin() const { return _M_t.begin(); }
  iterator end() const { return _M_t.end(); }
  reverse_iterator rbegin() const { return _M_t.rbegin(); } 
  reverse_iterator rend() const { return _M_t.rend(); }
  //���µĺ������ǵ�����RB-Tree��ʵ��,set�����Լ�����
  bool empty() const { return _M_t.empty(); }
  size_type size() const { return _M_t.size(); }
  size_type max_size() const { return _M_t.max_size(); }
  //������õ�swap()������ר����RB-Tree��swap()��������STL��swap()�㷨
  void swap(set<_Key,_Compare,_Alloc>& __x) { _M_t.swap(__x._M_t); }

  // insert/erase

  // ���ݷ���ֵ��������ж��Ƿ�����Ԫ��
  //pair.secondΪtrue���ʾ�Ѳ����Ԫ��  
  //Ϊfalse���ʾset���Ѵ������������ͬ��Ԫ��, �����ظ�����  
  pair<iterator,bool> insert(const value_type& __x) { 
    pair<typename _Rep_type::iterator, bool> __p = _M_t.insert_unique(__x); 
    return pair<iterator, bool>(__p.first, __p.second);
  }
  //��ָ��λ�ò���Ԫ�أ����ǻ��ȱ����ü���,�ж��Ƿ������ͬԪ��
  //�������ڲ���ָ��λ�ò����Ԫ��
  iterator insert(iterator __position, const value_type& __x) {
    typedef typename _Rep_type::iterator _Rep_iterator;
    return _M_t.insert_unique((_Rep_iterator&)__position, __x);
  }
#ifdef __STL_MEMBER_TEMPLATES
  template <class _InputIterator>
  void insert(_InputIterator __first, _InputIterator __last) {
    _M_t.insert_unique(__first, __last);
  }
#else
  void insert(const_iterator __first, const_iterator __last) {
    _M_t.insert_unique(__first, __last);
  }
  void insert(const value_type* __first, const value_type* __last) {
    _M_t.insert_unique(__first, __last);
  }
#endif /* __STL_MEMBER_TEMPLATES */
  //����ָ��λ�õ�Ԫ��
  void erase(iterator __position) { 
    typedef typename _Rep_type::iterator _Rep_iterator;
    _M_t.erase((_Rep_iterator&)__position); 
  }
  //����Ԫ��ֵΪx�Ľڵ�
  size_type erase(const key_type& __x) { 
    return _M_t.erase(__x); 
  }
  //����ָ������Ľڵ�
  void erase(iterator __first, iterator __last) { 
    typedef typename _Rep_type::iterator _Rep_iterator;
    _M_t.erase((_Rep_iterator&)__first, (_Rep_iterator&)__last); 
  }
  //���set
  void clear() { _M_t.clear(); }

  // set operations:

  //����Ԫ��ֵΪx�Ľڵ�
  iterator find(const key_type& __x) const { return _M_t.find(__x); }
  //����ָ��Ԫ�صĸ���
  size_type count(const key_type& __x) const {
    return _M_t.find(__x) == _M_t.end() ? 0 : 1;
  }
  //���ز�С�ڵ�ǰԪ�صĵ�һ���ɲ����λ�� 
  iterator lower_bound(const key_type& __x) const {
    return _M_t.lower_bound(__x);
  }
  // ���ش��ڵ�ǰԪ�صĵ�һ���ɲ����λ�� 
  iterator upper_bound(const key_type& __x) const {
    return _M_t.upper_bound(__x); 
  }
  pair<iterator,iterator> equal_range(const key_type& __x) const {
    return _M_t.equal_range(__x);
  }

#ifdef __STL_TEMPLATE_FRIENDS
  template <class _K1, class _C1, class _A1>
  friend bool operator== (const set<_K1,_C1,_A1>&, const set<_K1,_C1,_A1>&);
  template <class _K1, class _C1, class _A1>
  friend bool operator< (const set<_K1,_C1,_A1>&, const set<_K1,_C1,_A1>&);
#else /* __STL_TEMPLATE_FRIENDS */
  friend bool __STD_QUALIFIER
  operator== __STL_NULL_TMPL_ARGS (const set&, const set&);
  friend bool __STD_QUALIFIER
  operator<  __STL_NULL_TMPL_ARGS (const set&, const set&);
#endif /* __STL_TEMPLATE_FRIENDS */
};

template <class _Key, class _Compare, class _Alloc>
inline bool operator==(const set<_Key,_Compare,_Alloc>& __x, 
                       const set<_Key,_Compare,_Alloc>& __y) {
  return __x._M_t == __y._M_t;
}

template <class _Key, class _Compare, class _Alloc>
inline bool operator<(const set<_Key,_Compare,_Alloc>& __x, 
                      const set<_Key,_Compare,_Alloc>& __y) {
  return __x._M_t < __y._M_t;
}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

template <class _Key, class _Compare, class _Alloc>
inline bool operator!=(const set<_Key,_Compare,_Alloc>& __x, 
                       const set<_Key,_Compare,_Alloc>& __y) {
  return !(__x == __y);
}

template <class _Key, class _Compare, class _Alloc>
inline bool operator>(const set<_Key,_Compare,_Alloc>& __x, 
                      const set<_Key,_Compare,_Alloc>& __y) {
  return __y < __x;
}

template <class _Key, class _Compare, class _Alloc>
inline bool operator<=(const set<_Key,_Compare,_Alloc>& __x, 
                       const set<_Key,_Compare,_Alloc>& __y) {
  return !(__y < __x);
}

template <class _Key, class _Compare, class _Alloc>
inline bool operator>=(const set<_Key,_Compare,_Alloc>& __x, 
                       const set<_Key,_Compare,_Alloc>& __y) {
  return !(__x < __y);
}

template <class _Key, class _Compare, class _Alloc>
inline void swap(set<_Key,_Compare,_Alloc>& __x, 
                 set<_Key,_Compare,_Alloc>& __y) {
  __x.swap(__y);
}

#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1174
#pragma reset woff 1375
#endif

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_SET_H */

// Local Variables:
// mode:C++
// End: