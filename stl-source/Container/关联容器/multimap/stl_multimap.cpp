#ifndef __SGI_STL_INTERNAL_MULTIMAP_H
#define __SGI_STL_INTERNAL_MULTIMAP_H

#include <concept_checks.h>

__STL_BEGIN_NAMESPACE

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1174
#pragma set woff 1375
#endif

// Forward declaration of operators < and ==, needed for friend declaration.

//multimap�����Լ����÷���map��ȫ��ͬ��Ψһ���������multimap�����ֵkey�ظ�
//���multimap�Ĳ���������õ��ǵײ�RB-Tree��insert_equal()����insert_unique()
//�й�map������������ǰ�沩��
//map�ڲ�Ԫ�ظ��ݼ�ֵkeyĬ��ʹ�õ�������less
//�û��������ƶ��Ƚ�����
//�ڲ�ά�������ݽṹ�Ǻ����, ���зǳ������������ʱ�临�Ӷ�
//ע��:multimap����Ԫ���ظ�,����ֵ��ʵֵ�������ظ�,�����map��ͬ
template <class _Key, class _Tp, 
          class _Compare __STL_DEPENDENT_DEFAULT_TMPL(less<_Key>),
          class _Alloc = __STL_DEFAULT_ALLOCATOR(_Tp) >
class multimap;

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator==(const multimap<_Key,_Tp,_Compare,_Alloc>& __x, 
                       const multimap<_Key,_Tp,_Compare,_Alloc>& __y);

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator<(const multimap<_Key,_Tp,_Compare,_Alloc>& __x, 
                      const multimap<_Key,_Tp,_Compare,_Alloc>& __y);

template <class _Key, class _Tp, class _Compare, class _Alloc>
class multimap {
  // requirements:

  __STL_CLASS_REQUIRES(_Tp, _Assignable);
  __STL_CLASS_BINARY_FUNCTION_CHECK(_Compare, bool, _Key, _Key);

public:

// typedefs:

  //����Ķ�����map��ͬ
  typedef _Key                  key_type;
  typedef _Tp                   data_type;
  typedef _Tp                   mapped_type;
  typedef pair<const _Key, _Tp> value_type;
  typedef _Compare              key_compare;

   //Ƕ����,�ṩ��ֵkey�ȽϺ����ӿ�
  //�̳���<stl_function.h>�е�binary_function
  /*
	template <class _Arg1, class _Arg2, class _Result>
	struct binary_function {
		typedef _Arg1 first_argument_type;
		typedef _Arg2 second_argument_type;
		typedef _Result result_type;
	}; 
  */
  class value_compare : public binary_function<value_type, value_type, bool> {
  friend class multimap<_Key,_Tp,_Compare,_Alloc>;
  protected:
    _Compare comp;
    value_compare(_Compare __c) : comp(__c) {}
  public:
    bool operator()(const value_type& __x, const value_type& __y) const {
      return comp(__x.first, __y.first);
    }
  };

private:
	//�ײ������RB-Tree
  typedef _Rb_tree<key_type, value_type, 
                  _Select1st<value_type>, key_compare, _Alloc> _Rep_type;
  _Rep_type _M_t;  // red-black tree representing multimap
public:
  typedef typename _Rep_type::pointer pointer;
  typedef typename _Rep_type::const_pointer const_pointer;
  typedef typename _Rep_type::reference reference;
  typedef typename _Rep_type::const_reference const_reference;
  //map�ĵ�������ֱ�Ӷ���Ϊconst_iterator,���Ƿֱ���iterator,const_iterator
  //����Ϊmap�ļ�ֵkey���ܱ��޸�,��Ϊ�������رȽϺ������������,���Ա��붨��Ϊconst_iterator
  //��map��ʵֵvalue���Ա��޸�,����Ϊiterator
  typedef typename _Rep_type::iterator iterator;
  typedef typename _Rep_type::const_iterator const_iterator; 
  typedef typename _Rep_type::reverse_iterator reverse_iterator;
  typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;
  typedef typename _Rep_type::size_type size_type;
  typedef typename _Rep_type::difference_type difference_type;
  typedef typename _Rep_type::allocator_type allocator_type;

// allocation/deallocation
  // ע��:multimapֻ��ʹ��RB-tree��insert-equal(),����ʹ��insert-unique() 

  /*
  ���캯��
		multimap();
		explicit multimap (const key_compare& comp = key_compare(),
					   const allocator_type& alloc = allocator_type());
		
		template <class InputIterator>
		multimap (InputIterator first, InputIterator last,
				const key_compare& comp = key_compare(),
				const allocator_type& alloc = allocator_type());
	
		multimap (const multimap& x);
  */

  multimap() : _M_t(_Compare(), allocator_type()) { }
  explicit multimap(const _Compare& __comp,
                    const allocator_type& __a = allocator_type())
    : _M_t(__comp, __a) { }

#ifdef __STL_MEMBER_TEMPLATES  
  template <class _InputIterator>
  multimap(_InputIterator __first, _InputIterator __last)
    : _M_t(_Compare(), allocator_type())
    { _M_t.insert_equal(__first, __last); }

  template <class _InputIterator>
  multimap(_InputIterator __first, _InputIterator __last,
           const _Compare& __comp,
           const allocator_type& __a = allocator_type())
    : _M_t(__comp, __a) { _M_t.insert_equal(__first, __last); }
#else
  multimap(const value_type* __first, const value_type* __last)
    : _M_t(_Compare(), allocator_type())
    { _M_t.insert_equal(__first, __last); }
  multimap(const value_type* __first, const value_type* __last,
           const _Compare& __comp,
           const allocator_type& __a = allocator_type())
    : _M_t(__comp, __a) { _M_t.insert_equal(__first, __last); }

  multimap(const_iterator __first, const_iterator __last)
    : _M_t(_Compare(), allocator_type())
    { _M_t.insert_equal(__first, __last); }
  multimap(const_iterator __first, const_iterator __last,
           const _Compare& __comp,
           const allocator_type& __a = allocator_type())
    : _M_t(__comp, __a) { _M_t.insert_equal(__first, __last); }
#endif /* __STL_MEMBER_TEMPLATES */
  //�������캯��
  multimap(const multimap<_Key,_Tp,_Compare,_Alloc>& __x) : _M_t(__x._M_t) { }
  //�����ṩ��operator=,������ͨ��=��ʼ������
  multimap<_Key,_Tp,_Compare,_Alloc>&
  operator=(const multimap<_Key,_Tp,_Compare,_Alloc>& __x) {
    _M_t = __x._M_t;
    return *this; 
  }

  // accessors:
   //���ؼ�ֵ�ıȽϺ���,�����ǵ���RB-Tree��key_comp()
  key_compare key_comp() const { return _M_t.key_comp(); }
   //����ʵֵ�ıȽϺ���
  //������õ���mapǶ�����ж���ıȽϺ���
  /*
	  class value_compare
		: public binary_function<value_type, value_type, bool> {
	  friend class map<_Key,_Tp,_Compare,_Alloc>;
	  protected :
		_Compare comp;
		value_compare(_Compare __c) : comp(__c) {}
	  public:
		bool operator()(const value_type& __x, const value_type& __y) const {
		  return comp(__x.first, __y.first);//�Լ�ֵ���ñȽϺ���
		}
  */
  //ʵ�������ջ��ǵ��ü�ֵkey�ıȽϺ������������ǵ���ͬһ���ȽϺ���
  value_compare value_comp() const { return value_compare(_M_t.key_comp()); }
  allocator_type get_allocator() const { return _M_t.get_allocator(); }

  iterator begin() { return _M_t.begin(); }
  const_iterator begin() const { return _M_t.begin(); }
  iterator end() { return _M_t.end(); }
  const_iterator end() const { return _M_t.end(); }
  reverse_iterator rbegin() { return _M_t.rbegin(); }
  const_reverse_iterator rbegin() const { return _M_t.rbegin(); }
  reverse_iterator rend() { return _M_t.rend(); }
  const_reverse_iterator rend() const { return _M_t.rend(); }
  //�ж�����multimap�Ƿ�Ϊ��
  bool empty() const { return _M_t.empty(); }
  //��������multimap�Ĵ�С
  size_type size() const { return _M_t.size(); }
  size_type max_size() const { return _M_t.max_size(); }
  //����multimap���������
  void swap(multimap<_Key,_Tp,_Compare,_Alloc>& __x) { _M_t.swap(__x._M_t); }

  // insert/erase
  /*
	multimapֻ��ʹ��RB-tree��insert-equal()
	����Ԫ��
	iterator insert (const value_type& val);
	
	iterator insert (iterator position, const value_type& val);
	
	template <class InputIterator>
	void insert (InputIterator first, InputIterator last);
  */
  //����Ԫ�ؽڵ�,����RB-Tree��insert-equal();
  //����Ԫ�صļ�ֵkey�����ظ�
  iterator insert(const value_type& __x) { return _M_t.insert_equal(__x); }
  //��ָ��λ�ò���Ԫ��
  iterator insert(iterator __position, const value_type& __x) {
    return _M_t.insert_equal(__position, __x);
  }
#ifdef __STL_MEMBER_TEMPLATES  
  //����[first,last)Ԫ��
  template <class _InputIterator>
  void insert(_InputIterator __first, _InputIterator __last) {
    _M_t.insert_equal(__first, __last);
  }
#else
  void insert(const value_type* __first, const value_type* __last) {
    _M_t.insert_equal(__first, __last);
  }
  void insert(const_iterator __first, const_iterator __last) {
    _M_t.insert_equal(__first, __last);
  }
#endif /* __STL_MEMBER_TEMPLATES */
  /*
	����Ԫ��
	void erase (iterator position);

	size_type erase (const key_type& k);
	
    void erase (iterator first, iterator last);
  */
  //��ָ��λ�ò���Ԫ��
  void erase(iterator __position) { _M_t.erase(__position); }
  //����ָ����ֵ�Ľڵ�
  size_type erase(const key_type& __x) { return _M_t.erase(__x); }
  //����ָ������Ľڵ�
  void erase(iterator __first, iterator __last)
    { _M_t.erase(__first, __last); }
  //�������
  void clear() { _M_t.clear(); }

  // multimap operations:

  //����ָ����ֵ�Ľڵ�
  iterator find(const key_type& __x) { return _M_t.find(__x); }
  const_iterator find(const key_type& __x) const { return _M_t.find(__x); }
  //����ָ����ֵԪ�صĸ���
  size_type count(const key_type& __x) const { return _M_t.count(__x); }
   //Returns an iterator pointing to the first element in the container 
  //whose key is not considered to go before k (i.e., either it is equivalent or goes after).
  //this->first is greater than or equivalent to __x.
  iterator lower_bound(const key_type& __x) {return _M_t.lower_bound(__x); }
  const_iterator lower_bound(const key_type& __x) const {
    return _M_t.lower_bound(__x); 
  }
  //Returns an iterator pointing to the first element that is greater than key.
  iterator upper_bound(const key_type& __x) {return _M_t.upper_bound(__x); }
  const_iterator upper_bound(const key_type& __x) const {
    return _M_t.upper_bound(__x); 
  }
  //Returns the bounds of a range that includes all the elements in the container 
  //which have a key equivalent to k
  //Because the elements in a map container have unique keys, 
  //the range returned will contain a single element at most.
   pair<iterator,iterator> equal_range(const key_type& __x) {
    return _M_t.equal_range(__x);
  }
  pair<const_iterator,const_iterator> equal_range(const key_type& __x) const {
    return _M_t.equal_range(__x);
  }


  //�����ǲ���������
#ifdef __STL_TEMPLATE_FRIENDS 
  template <class _K1, class _T1, class _C1, class _A1>
  friend bool operator== (const multimap<_K1, _T1, _C1, _A1>&,
                          const multimap<_K1, _T1, _C1, _A1>&);
  template <class _K1, class _T1, class _C1, class _A1>
  friend bool operator< (const multimap<_K1, _T1, _C1, _A1>&,
                         const multimap<_K1, _T1, _C1, _A1>&);
#else /* __STL_TEMPLATE_FRIENDS */
  friend bool __STD_QUALIFIER
  operator== __STL_NULL_TMPL_ARGS (const multimap&, const multimap&);
  friend bool __STD_QUALIFIER
  operator< __STL_NULL_TMPL_ARGS (const multimap&, const multimap&);
#endif /* __STL_TEMPLATE_FRIENDS */
};

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator==(const multimap<_Key,_Tp,_Compare,_Alloc>& __x, 
                       const multimap<_Key,_Tp,_Compare,_Alloc>& __y) {
  return __x._M_t == __y._M_t;
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator<(const multimap<_Key,_Tp,_Compare,_Alloc>& __x, 
                      const multimap<_Key,_Tp,_Compare,_Alloc>& __y) {
  return __x._M_t < __y._M_t;
}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator!=(const multimap<_Key,_Tp,_Compare,_Alloc>& __x, 
                       const multimap<_Key,_Tp,_Compare,_Alloc>& __y) {
  return !(__x == __y);
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator>(const multimap<_Key,_Tp,_Compare,_Alloc>& __x, 
                      const multimap<_Key,_Tp,_Compare,_Alloc>& __y) {
  return __y < __x;
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator<=(const multimap<_Key,_Tp,_Compare,_Alloc>& __x, 
                       const multimap<_Key,_Tp,_Compare,_Alloc>& __y) {
  return !(__y < __x);
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator>=(const multimap<_Key,_Tp,_Compare,_Alloc>& __x, 
                       const multimap<_Key,_Tp,_Compare,_Alloc>& __y) {
  return !(__x < __y);
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline void swap(multimap<_Key,_Tp,_Compare,_Alloc>& __x, 
                 multimap<_Key,_Tp,_Compare,_Alloc>& __y) {
  __x.swap(__y);
}

#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1174
#pragma reset woff 1375
#endif

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_MULTIMAP_H */

// Local Variables:
// mode:C++
// End:
