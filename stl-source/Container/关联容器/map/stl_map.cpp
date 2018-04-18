#ifndef __SGI_STL_INTERNAL_MAP_H
#define __SGI_STL_INTERNAL_MAP_H

#include <concept_checks.h>

__STL_BEGIN_NAMESPACE

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1174
#pragma set woff 1375
#endif
/*
	map�����нڵ�Ԫ�ض���pair,pair��������Ա����first��second
	��һ��first�Ǽ�ֵkey,�ڶ���second��ʵֵvalue
	�й�pair�Ķ����ǰ��<stl_pair.h>����
*/

//map�ڲ�Ԫ�ظ��ݼ�ֵkeyĬ��ʹ�õ�������less
//�û��������ƶ��Ƚ�����
//�ڲ�ά�������ݽṹ�Ǻ����, ���зǳ������������ʱ�临�Ӷ�  
//ע�⣺map��ֵ��ʵֵ�Ƿֿ��ģ�map�ļ�ֵkey��Ψһ�ģ�ʵֵvalue�����ظ�
//����ͨ���������޸�map�ļ�ֵkey,������������Ƕ���ΪRB-Tree��const_iterator
//���ǿ���ͨ���������޸�map��ʵֵvalue

// Forward declarations of operators == and <, needed for friend declarations.
template <class _Key, class _Tp, 
          class _Compare __STL_DEPENDENT_DEFAULT_TMPL(less<_Key>),
          class _Alloc = __STL_DEFAULT_ALLOCATOR(_Tp) >
class map;

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator==(const map<_Key,_Tp,_Compare,_Alloc>& __x, 
                       const map<_Key,_Tp,_Compare,_Alloc>& __y);

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator<(const map<_Key,_Tp,_Compare,_Alloc>& __x, 
                      const map<_Key,_Tp,_Compare,_Alloc>& __y);

//map����
template <class _Key, class _Tp, class _Compare, class _Alloc>
class map {
public:

// requirements:

  __STL_CLASS_REQUIRES(_Tp, _Assignable);
  __STL_CLASS_BINARY_FUNCTION_CHECK(_Compare, bool, _Key, _Key);

// typedefs:

  typedef _Key                  key_type;//��ֵkey����
  typedef _Tp                   data_type;//����(ʵֵ)value����
  typedef _Tp                   mapped_type;
  typedef pair<const _Key, _Tp> value_type;//Ԫ���ͱ�,����(��ֵ/ʵֵ),const��֤��ֵkey�����޸�
  typedef _Compare              key_compare;//��ֵkey�ȽϺ���
    
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
  };

private:
	//�ײ������RB-Tree
	//��map���ͣ�һ��pair���ĵ�һ��������ΪTB-tree�ļ�ֵ����.  
	//������RB-tree�У���ֵkey�����޸�
  typedef _Rb_tree<key_type, value_type, 
                   _Select1st<value_type>, key_compare, _Alloc> _Rep_type;
  _Rep_type _M_t;  // red-black tree representing map
public:
  typedef typename _Rep_type::pointer pointer;
  typedef typename _Rep_type::const_pointer const_pointer;
  typedef typename _Rep_type::reference reference;
  typedef typename _Rep_type::const_reference const_reference;
  //map�ĵ�������ֱ�Ӷ���Ϊconst_iterator,���Ƿֱ���iterator,const_iterator
  //����Ϊmap�ļ�ֵkey���ܱ��޸�,���Ա��붨��Ϊconst_iterator
  //��map��ʵֵvalue���Ա��޸�,����Ϊiterator
  typedef typename _Rep_type::iterator iterator;
  typedef typename _Rep_type::const_iterator const_iterator;
  typedef typename _Rep_type::reverse_iterator reverse_iterator;
  typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;
  typedef typename _Rep_type::size_type size_type;
  typedef typename _Rep_type::difference_type difference_type;
  typedef typename _Rep_type::allocator_type allocator_type;

  // allocation/deallocation
  // mapֻ��ʹ��RB-tree��insert-unique(),����ʹ��insert-equal() 
  //��Ϊ���뱣֤��ֵΨһ
  /*
				���캯��
	map();
	explicit map (const key_compare& comp = key_compare(),
              const allocator_type& alloc = allocator_type());
	
	template <class InputIterator>
	map (InputIterator first, InputIterator last,
       const key_compare& comp = key_compare(),
       const allocator_type& alloc = allocator_type());
	
	map (const map& x);
  */
  /*
	example:
	#include <iostream>
	#include <map>

	bool fncomp (char lhs, char rhs) {return lhs<rhs;}

	struct classcomp {
	  bool operator() (const char& lhs, const char& rhs) const
	  {return lhs<rhs;}
	};

	int main ()
	{
	  std::map<char,int> first;

	  first['a']=10;
	  first['b']=30;
	  first['c']=50;
	  first['d']=70;

	  std::map<char,int> second (first.begin(),first.end());

	  std::map<char,int> third (second);

	  std::map<char,int,classcomp> fourth;                 // class as Compare

	  bool(*fn_pt)(char,char) = fncomp;
	  std::map<char,int,bool(*)(char,char)> fifth (fn_pt); // function pointer as Compare

	  return 0;
	}
  */

  map() : _M_t(_Compare(), allocator_type()) {}
  explicit map(const _Compare& __comp,
               const allocator_type& __a = allocator_type())
    : _M_t(__comp, __a) {}

#ifdef __STL_MEMBER_TEMPLATES
  template <class _InputIterator>
  map(_InputIterator __first, _InputIterator __last)
    : _M_t(_Compare(), allocator_type())
    { _M_t.insert_unique(__first, __last); }

  template <class _InputIterator>
  map(_InputIterator __first, _InputIterator __last, const _Compare& __comp,
      const allocator_type& __a = allocator_type())
    : _M_t(__comp, __a) { _M_t.insert_unique(__first, __last); }
#else
  map(const value_type* __first, const value_type* __last)
    : _M_t(_Compare(), allocator_type())
    { _M_t.insert_unique(__first, __last); }

  map(const value_type* __first,
      const value_type* __last, const _Compare& __comp,
      const allocator_type& __a = allocator_type())
    : _M_t(__comp, __a) { _M_t.insert_unique(__first, __last); }

  map(const_iterator __first, const_iterator __last)
    : _M_t(_Compare(), allocator_type()) 
    { _M_t.insert_unique(__first, __last); }

  map(const_iterator __first, const_iterator __last, const _Compare& __comp,
      const allocator_type& __a = allocator_type())
    : _M_t(__comp, __a) { _M_t.insert_unique(__first, __last); }

#endif /* __STL_MEMBER_TEMPLATES */

  //�������캯��
  map(const map<_Key,_Tp,_Compare,_Alloc>& __x) : _M_t(__x._M_t) {}
  //�����ṩ��operator=,������ͨ��=��ʼ������
  map<_Key,_Tp,_Compare,_Alloc>&
  operator=(const map<_Key, _Tp, _Compare, _Alloc>& __x)
  {
    _M_t = __x._M_t;
    return *this; 
  }

  // accessors:

  //���µ���RB-Tree�Ĳ���
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
  //��÷�����������
  allocator_type get_allocator() const { return _M_t.get_allocator(); }

  iterator begin() { return _M_t.begin(); }
  const_iterator begin() const { return _M_t.begin(); }
  iterator end() { return _M_t.end(); }
  const_iterator end() const { return _M_t.end(); }
  reverse_iterator rbegin() { return _M_t.rbegin(); }
  const_reverse_iterator rbegin() const { return _M_t.rbegin(); }
  reverse_iterator rend() { return _M_t.rend(); }
  const_reverse_iterator rend() const { return _M_t.rend(); }
  bool empty() const { return _M_t.empty(); }
  size_type size() const { return _M_t.size(); }
  size_type max_size() const { return _M_t.max_size(); }
  //����operator[],������ʵֵvalue(��pair.second)������
  //ע��:����ԭ��û�ж���map����,������ʵļ�ֵkey������,����Զ��½�һ��map����
  //��ֵkeyΪ����ʵļ�ֵkey,ʵֵvalueΪ��,����������Ӿ�������
    _Tp& operator[](const key_type& __k) {
    iterator __i = lower_bound(__k);
    // __i->first is greater than or equivalent to __k.
    if (__i == end() || key_comp()(__k, (*__i).first))
      __i = insert(__i, value_type(__k, _Tp()));
    return (*__i).second;
	//��ʵ�򵥵ķ�ʽ��ֱ�ӷ���
	//return (*((insert(value_type(k, T()))).first)).second;
  }
	/*
	example:
	#include <iostream>
	#include <map>
	#include <string>

	int main ()
	{
	  std::map<char,std::string> mymap;

	  mymap['a']="an element";
	  mymap['b']="another element";
	  mymap['c']=mymap['b'];

	  std::cout << "mymap['a'] is " << mymap['a'] << '\n';
	  std::cout << "mymap['b'] is " << mymap['b'] << '\n';
	  std::cout << "mymap['c'] is " << mymap['c'] << '\n';
	  std::cout << "mymap['d'] is " << mymap['d'] << '\n';

	  std::cout << "mymap now contains " << mymap.size() << " elements.\n";

	  return 0;
	}
	Ouput:
	mymap['a'] is an element
	mymap['b'] is another element
	mymap['c'] is another element
	mymap['d'] is
	mymap now contains 4 elements.
  */
	//����map���������
  void swap(map<_Key,_Tp,_Compare,_Alloc>& __x) { _M_t.swap(__x._M_t); }

  // insert/erase
  /*
	����Ԫ��
	single element (1):	
	pair<iterator,bool> insert (const value_type& val);

	with hint (2):
	iterator insert (iterator position, const value_type& val);

	range (3):	
	template <class InputIterator>
	void insert (InputIterator first, InputIterator last);
  */

  //����Ԫ�ؽڵ�,����RB-Tree��insert_unique(__x);
  //���ܲ�����ͬ��ֵ��Ԫ��
  pair<iterator,bool> insert(const value_type& __x) 
    { return _M_t.insert_unique(__x); }
  //��ָ��λ�ò���Ԫ�أ����ǻ��ȱ����ü���,�ж��Ƿ������ͬԪ��
  //�������ڲ���ָ��λ�ò����Ԫ��
  iterator insert(iterator position, const value_type& __x)
    { return _M_t.insert_unique(position, __x); }
#ifdef __STL_MEMBER_TEMPLATES
  template <class _InputIterator>
  void insert(_InputIterator __first, _InputIterator __last) {
    _M_t.insert_unique(__first, __last);
  }
#else
  void insert(const value_type* __first, const value_type* __last) {
    _M_t.insert_unique(__first, __last);
  }
  void insert(const_iterator __first, const_iterator __last) {
    _M_t.insert_unique(__first, __last);
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
  //���map
  void clear() { _M_t.clear(); }

  // map operations:

  //����ָ����ֵ�Ľڵ�
  iterator find(const key_type& __x) { return _M_t.find(__x); }
  const_iterator find(const key_type& __x) const { return _M_t.find(__x); }
  //����ָ����ֵԪ�صĸ���
  size_type count(const key_type& __x) const {
    return _M_t.find(__x) == _M_t.end() ? 0 : 1; 
  }

  /*
	Example:
	#include <iostream>
	#include <map>

	int main ()
	{
	  std::map<char,int> mymap;
	  std::map<char,int>::iterator itlow,itup;

	  mymap['a']=20;
	  mymap['b']=40;
	  mymap['c']=60;
	  mymap['d']=80;
	  mymap['e']=100;

	  itlow=mymap.lower_bound ('b');  // itlow points to b
	  itup=mymap.upper_bound ('d');   // itup points to e (not d!)

	  mymap.erase(itlow,itup);        // erases [itlow,itup)

	  // print content:
	  for (std::map<char,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
		std::cout << it->first << " => " << it->second << '\n';

	  return 0;
	}
	Output:
	a => 20
	e => 100
  */
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
  /*
  Example:
	#include <iostream>
	#include <map>

	int main ()
	{
	  std::map<char,int> mymap;

	  mymap['a']=10;
	  mymap['b']=20;
	  mymap['c']=30;

	  std::pair<std::map<char,int>::iterator,std::map<char,int>::iterator> ret;
	  ret = mymap.equal_range('b');

	  std::cout << "lower bound points to: ";
	  std::cout << ret.first->first << " => " << ret.first->second << '\n';

	  std::cout << "upper bound points to: ";
	  std::cout << ret.second->first << " => " << ret.second->second << '\n';

	  return 0;
	}
	Output:
		lower bound points to: 'b' => 20
		upper bound points to: 'c' => 30
  */

//�����ǲ���������
#ifdef __STL_TEMPLATE_FRIENDS 
  template <class _K1, class _T1, class _C1, class _A1>
  friend bool operator== (const map<_K1, _T1, _C1, _A1>&,
                          const map<_K1, _T1, _C1, _A1>&);
  template <class _K1, class _T1, class _C1, class _A1>
  friend bool operator< (const map<_K1, _T1, _C1, _A1>&,
                         const map<_K1, _T1, _C1, _A1>&);
#else /* __STL_TEMPLATE_FRIENDS */
  friend bool __STD_QUALIFIER
  operator== __STL_NULL_TMPL_ARGS (const map&, const map&);
  friend bool __STD_QUALIFIER
  operator< __STL_NULL_TMPL_ARGS (const map&, const map&);
#endif /* __STL_TEMPLATE_FRIENDS */
};

//�Ƚ�����map������
template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator==(const map<_Key,_Tp,_Compare,_Alloc>& __x, 
                       const map<_Key,_Tp,_Compare,_Alloc>& __y) {
  return __x._M_t == __y._M_t;
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator<(const map<_Key,_Tp,_Compare,_Alloc>& __x, 
                      const map<_Key,_Tp,_Compare,_Alloc>& __y) {
  return __x._M_t < __y._M_t;
}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator!=(const map<_Key,_Tp,_Compare,_Alloc>& __x, 
                       const map<_Key,_Tp,_Compare,_Alloc>& __y) {
  return !(__x == __y);
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator>(const map<_Key,_Tp,_Compare,_Alloc>& __x, 
                      const map<_Key,_Tp,_Compare,_Alloc>& __y) {
  return __y < __x;
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator<=(const map<_Key,_Tp,_Compare,_Alloc>& __x, 
                       const map<_Key,_Tp,_Compare,_Alloc>& __y) {
  return !(__y < __x);
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator>=(const map<_Key,_Tp,_Compare,_Alloc>& __x, 
                       const map<_Key,_Tp,_Compare,_Alloc>& __y) {
  return !(__x < __y);
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline void swap(map<_Key,_Tp,_Compare,_Alloc>& __x, 
                 map<_Key,_Tp,_Compare,_Alloc>& __y) {
  __x.swap(__y);
}

#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1174
#pragma reset woff 1375
#endif

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_MAP_H */

// Local Variables:
// mode:C++
// End: