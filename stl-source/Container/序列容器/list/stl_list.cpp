
//������list����ڵ�����ݽṹ
struct _List_node_base {
  _List_node_base* _M_next;//ָ��ֱ�Ӻ�̽ڵ�
  _List_node_base* _M_prev;//ָ��ֱ��ǰ���ڵ�
};

template <class _Tp>
struct _List_node : public _List_node_base {
  _Tp _M_data;//�ڵ�洢������
};

//����������List_iterator_base�ĵ�����
struct _List_iterator_base {
	//��������
  typedef size_t                     size_type;
  typedef ptrdiff_t                  difference_type;
  //list��������������˫�������bidirectional_iterator
  typedef bidirectional_iterator_tag iterator_category;

  //����ָ������ڵ��ָ��
  _List_node_base* _M_node;

  //���캯��
  _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
  _List_iterator_base() {}

  //���½ڵ�ָ�룬ָ��ֱ��ǰ����ֱ�Ӻ�̽ڵ�
  void _M_incr() { _M_node = _M_node->_M_next; }
  void _M_decr() { _M_node = _M_node->_M_prev; }

  //����������
  bool operator==(const _List_iterator_base& __x) const {
    return _M_node == __x._M_node;
  }
  bool operator!=(const _List_iterator_base& __x) const {
    return _M_node != __x._M_node;
  }
};  

//����������List_iterator�ĵ�����
template<class _Tp, class _Ref, class _Ptr>
struct _List_iterator : public _List_iterator_base {
  typedef _List_iterator<_Tp,_Tp&,_Tp*>             iterator;
  typedef _List_iterator<_Tp,const _Tp&,const _Tp*> const_iterator;
  typedef _List_iterator<_Tp,_Ref,_Ptr>             _Self;

  typedef _Tp value_type;
  typedef _Ptr pointer;
  typedef _Ref reference;
  typedef _List_node<_Tp> _Node;

  //���캯��
  _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
  _List_iterator() {}
  _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}

  //���¶��ǻ���������������,ȡ���ڵ�����
  reference operator*() const { return ((_Node*) _M_node)->_M_data; }

#ifndef __SGI_STL_NO_ARROW_OPERATOR
  pointer operator->() const { return &(operator*()); }
#endif /* __SGI_STL_NO_ARROW_OPERATOR */

  _Self& operator++() { 
    this->_M_incr();
    return *this;
  }
  _Self operator++(int) { 
    _Self __tmp = *this;
    this->_M_incr();
    return __tmp;
  }
  _Self& operator--() { 
    this->_M_decr();
    return *this;
  }
  _Self operator--(int) { 
    _Self __tmp = *this;
    this->_M_decr();
    return __tmp;
  }
};

#ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

//���ص�����������
inline bidirectional_iterator_tag
iterator_category(const _List_iterator_base&)
{
  return bidirectional_iterator_tag();
}

template <class _Tp, class _Ref, class _Ptr>
inline _Tp*
value_type(const _List_iterator<_Tp, _Ref, _Ptr>&)
{
  return 0;
}

inline ptrdiff_t*
distance_type(const _List_iterator_base&)
{
  return 0;
}

#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */


// Base class that encapsulates details of allocators.  Three cases:
// an ordinary standard-conforming allocator, a standard-conforming
// allocator with no non-static data, and an SGI-style allocator.
// This complexity is necessary only because we're worrying about backward
// compatibility and because we want to avoid wasting storage on an 
// allocator instance if it isn't necessary.

#ifdef __STL_USE_STD_ALLOCATORS

// Base for general standard-conforming allocators.
template <class _Tp, class _Allocator, bool _IsStatic>
class _List_alloc_base {
public:
  typedef typename _Alloc_traits<_Tp, _Allocator>::allocator_type
          allocator_type;//���ؽڵ�������
  allocator_type get_allocator() const { return _Node_allocator; }

  _List_alloc_base(const allocator_type& __a) : _Node_allocator(__a) {}

protected:
  _List_node<_Tp>* _M_get_node()
   { return _Node_allocator.allocate(1); }
  void _M_put_node(_List_node<_Tp>* __p)
    { _Node_allocator.deallocate(__p, 1); }

protected:
  typename _Alloc_traits<_List_node<_Tp>, _Allocator>::allocator_type
           _Node_allocator;
  _List_node<_Tp>* _M_node;
};

// Specialization for instanceless allocators.
//instanceless������ƫ�ػ���
template <class _Tp, class _Allocator>
class _List_alloc_base<_Tp, _Allocator, true> {
public:
	//�������������
  typedef typename _Alloc_traits<_Tp, _Allocator>::allocator_type
          allocator_type;
  //���ؽڵ�������
  allocator_type get_allocator() const { return allocator_type(); }

  //���캯��
  _List_alloc_base(const allocator_type&) {}

protected:
  typedef typename _Alloc_traits<_List_node<_Tp>, _Allocator>::_Alloc_type
          _Alloc_type;
  //����һ���ڵ�ռ�
  _List_node<_Tp>* _M_get_node() { return _Alloc_type::allocate(1); }
  //����һ���ڵ�ռ�
  void _M_put_node(_List_node<_Tp>* __p) { _Alloc_type::deallocate(__p, 1); }

protected:
	//����ڵ�ָ��
  _List_node<_Tp>* _M_node;
};

template <class _Tp, class _Alloc>
class _List_base 
  : public _List_alloc_base<_Tp, _Alloc,
                            _Alloc_traits<_Tp, _Alloc>::_S_instanceless>
{
public:
  typedef _List_alloc_base<_Tp, _Alloc,
                           _Alloc_traits<_Tp, _Alloc>::_S_instanceless>
          _Base; 
  //allocator_type����������
  typedef typename _Base::allocator_type allocator_type;

 //���캯��
  _List_base(const allocator_type& __a) : _Base(__a) {
    _M_node = _M_get_node();//����һ���ڵ�ռ�
    _M_node->_M_next = _M_node;//
    _M_node->_M_prev = _M_node;
  }
  //��������
  ~_List_base() {
    clear();//�������
    _M_put_node(_M_node);//����һ���ڵ��ڴ�ռ�
  }

  void clear();//�������
};

#else /* __STL_USE_STD_ALLOCATORS */

template <class _Tp, class _Alloc>
class _List_base 
{
public:
  typedef _Alloc allocator_type;//��÷���������
  allocator_type get_allocator() const { return allocator_type(); }

  //���캯��
  _List_base(const allocator_type&) {
    _M_node = _M_get_node();//����һ���ڵ�ռ�
	//�ڵ�ǰ���ͺ��ָ��ָ���Լ�����ʾ��һ��������
    _M_node->_M_next = _M_node;
    _M_node->_M_prev = _M_node;
  }
  //��������
  ~_List_base() {
    clear();//�������
    _M_put_node(_M_node);//����һ���ڵ��ڴ�ռ�
  }

  void clear();//�������

protected:
	//����������
  typedef simple_alloc<_List_node<_Tp>, _Alloc> _Alloc_type;
  //����һ���ڵ��ڴ�ռ�
  _List_node<_Tp>* _M_get_node() { return _Alloc_type::allocate(1); }
  //����һ���ڵ��ڴ�ռ�
  void _M_put_node(_List_node<_Tp>* __p) { _Alloc_type::deallocate(__p, 1); } 

protected:
  _List_node<_Tp>* _M_node;//����Ľڵ�ָ��
};

#endif /* __STL_USE_STD_ALLOCATORS */

//clear()������ʵ�֣����������
template <class _Tp, class _Alloc>
void 
_List_base<_Tp,_Alloc>::clear() 
{
 //ѡȡ_M_node->_M_next��Ϊ��ǰ�ڵ�
 _List_node<_Tp>* __cur = (_List_node<_Tp>*) _M_node->_M_next;
  while (__cur != _M_node) {//����ÿһ���ڵ�
    _List_node<_Tp>* __tmp = __cur;//����һ���ڵ���ʱ����
    __cur = (_List_node<_Tp>*) __cur->_M_next;//ָ����һ���ڵ�
    _Destroy(&__tmp->_M_data);//�������ݶ���
    _M_put_node(__tmp);//���սڵ�tmpָ����ڴ�ռ�
  }
  //������,��ǰ���ͺ��ָ�붼ָ���Լ�
  _M_node->_M_next = _M_node;
  _M_node->_M_prev = _M_node;
}

//������˫������list��Ķ��壬������_AllocĬ��Ϊ�ڶ���������
template <class _Tp, class _Alloc = __STL_DEFAULT_ALLOCATOR(_Tp) >
class list : protected _List_base<_Tp, _Alloc> {
  // requirements:

  __STL_CLASS_REQUIRES(_Tp, _Assignable);

  typedef _List_base<_Tp, _Alloc> _Base;
protected:
  typedef void* _Void_pointer;//����ָ������

public: //��������Ƕ�ͱ�
  typedef _Tp value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef _List_node<_Tp> _Node;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  typedef typename _Base::allocator_type allocator_type;//����������
  allocator_type get_allocator() const { return _Base::get_allocator(); }

public:
	//������������
  typedef _List_iterator<_Tp,_Tp&,_Tp*>             iterator;
  typedef _List_iterator<_Tp,const _Tp&,const _Tp*> const_iterator;

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
  typedef reverse_iterator<const_iterator> const_reverse_iterator;
  typedef reverse_iterator<iterator>       reverse_iterator;
#else /* __STL_CLASS_PARTIAL_SPECIALIZATION */
  typedef reverse_bidirectional_iterator<const_iterator,value_type,
                                         const_reference,difference_type>
          const_reverse_iterator;
  typedef reverse_bidirectional_iterator<iterator,value_type,reference,
                                         difference_type>
          reverse_iterator; 
#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

protected:
#ifdef __STL_HAS_NAMESPACES
  using _Base::_M_node;
  using _Base::_M_put_node;
  using _Base::_M_get_node;
#endif /* __STL_HAS_NAMESPACES */

protected:
	//����ֵΪx�Ľڵ㣬�����ظýڵ�ĵ�ַ
  _Node* _M_create_node(const _Tp& __x)
  {
    _Node* __p = _M_get_node();//����һ���ڵ�ռ�
    __STL_TRY {//��xֵ����ָ���ĵ�ַ������dataֵ
      _Construct(&__p->_M_data, __x);
    }
    __STL_UNWIND(_M_put_node(__p));
    return __p;//���ؽڵ��ַ
  }

  //����Ĭ��ֵ�Ľڵ�
  _Node* _M_create_node()
  {
    _Node* __p = _M_get_node();
    __STL_TRY {
      _Construct(&__p->_M_data);
    }
    __STL_UNWIND(_M_put_node(__p));
    return __p;
  }

public:
	

  //�����ǵ������Ķ���
  iterator begin()             { return (_Node*)(_M_node->_M_next); }
  const_iterator begin() const { return (_Node*)(_M_node->_M_next); }

  iterator end()             { return _M_node; }
  const_iterator end() const { return _M_node; }

  reverse_iterator rbegin() 
    { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const 
    { return const_reverse_iterator(end()); }

  reverse_iterator rend()
    { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const
    { return const_reverse_iterator(begin()); }

  //�ж������Ƿ�Ϊ������
  bool empty() const { return _M_node->_M_next == _M_node; }
 
  //��������Ĵ�С
  size_type size() const {
    size_type __result = 0;
	//��������������֮��ľ���
    distance(begin(), end(), __result);
	//���������Ԫ�ظ���
    return __result;
  }
  size_type max_size() const { return size_type(-1); }

  //���ص�һ���ڵ����ݵ����ã�reference�൱��value_type&
  reference front() { return *begin(); }
  const_reference front() const { return *begin(); }
  //�������һ���ڵ����ݵ�����
  reference back() { return *(--end()); }
  const_reference back() const { return *(--end()); }

  //������������������
  void swap(list<_Tp, _Alloc>& __x) { __STD::swap(_M_node, __x._M_node); }

 //**********************************************************************
 //*********************����ڵ�*****************************************
  /******************�����ǲ���ڵ㺯����ԭ�ͣ�Ҳ�ǹ����ӿ�**************
	//��ָ����λ��pos֮ǰ����ֵΪvalue�����ݽڵ�
	iterator insert( iterator pos, const T& value );
	iterator insert( const_iterator pos, const T& value );

	//��ָ����λ��pos֮ǰ����n��ֵΪvalue�����ݽڵ�
	void insert( iterator pos, size_type count, const T& value );
	iterator insert( const_iterator pos, size_type count, const T& value );

	//��ָ����λ��pos֮ǰ����[first,last)֮������ݽڵ�
	template< class InputIt >
	void insert( iterator pos, InputIt first, InputIt last);
	template< class InputIt >
	iterator insert( const_iterator pos, InputIt first, InputIt last );
  ***********************************************************************/
  /**����������Ĳ����У���������Ƿǳ���Ҫ�ģ��ܶ��Ա��������øú���**/
//***********************************************************************
  //��ָ����λ�ò����ʼֵΪx�Ľڵ�
  iterator insert(iterator __position, const _Tp& __x) {
	  //���ȴ���һ����ʼֵΪx�Ľڵ㣬�����ظýڵ�ĵ�ַ
    _Node* __tmp = _M_create_node(__x);
	//�����ڵ�ָ�룬���½ڵ���뵽ָ��λ��
    __tmp->_M_next = __position._M_node;
    __tmp->_M_prev = __position._M_node->_M_prev;
    __position._M_node->_M_prev->_M_next = __tmp;
    __position._M_node->_M_prev = __tmp;
	//�����½ڵ��ַ
    return __tmp;
  }
  //��ָ����λ�ò���ΪĬ��ֵ�Ľڵ�
  iterator insert(iterator __position) { return insert(__position, _Tp()); }

  //��ָ��λ�ò���n����ʼֵΪx�Ľڵ�
  void insert(iterator __pos, size_type __n, const _Tp& __x)
    { _M_fill_insert(__pos, __n, __x); }
  void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x); 

#ifdef __STL_MEMBER_TEMPLATES
  // Check whether it's an integral type.  If so, it's not an iterator.
  //�������__type_traits����
 
   //��ָ��λ�ò���ָ����Χ�ڵ�����
  //�����ж��������������_InputIterator�Ƿ�Ϊ��������
  template <class _InputIterator>
  void insert(iterator __pos, _InputIterator __first, _InputIterator __last) {
    typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
    _M_insert_dispatch(__pos, __first, __last, _Integral());
  }

  
  //���������������_InputIterator��Ϊ�������ͣ����ô˺���
  template<class _Integer>
  void _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __x,
                          __true_type) {
    _M_fill_insert(__pos, (size_type) __n, (_Tp) __x);
  }

  //���������������_InputIterator�ǲ�Ϊ�������ͣ����ô˺���
  template <class _InputIterator>
  void _M_insert_dispatch(iterator __pos,
                          _InputIterator __first, _InputIterator __last,
                          __false_type);

 #else /* __STL_MEMBER_TEMPLATES */
  void insert(iterator __position, const _Tp* __first, const _Tp* __last);
  void insert(iterator __position,
              const_iterator __first, const_iterator __last);
#endif /* __STL_MEMBER_TEMPLATES */
  
  //������ͷ����ڵ�
  void push_front(const _Tp& __x) { insert(begin(), __x); }
  void push_front() {insert(begin());}
  //������β����ڵ�
  void push_back(const _Tp& __x) { insert(end(), __x); }
  void push_back() {insert(end());}

  //***********************************************************
  //********************��ָ��λ��ɾ���ڵ�*********************
  //********************������ɾ���ڵ�Ĺ����ӿ�***************
  /************************************************************
	//ɾ��ָ��λ��pos�Ľڵ�
	iterator erase( iterator pos );
	iterator erase( const_iterator pos );

	//ɾ��ָ����Χ[first,last)�����ݽڵ�
	iterator erase( iterator first, iterator last );
	iterator erase( const_iterator first, const_iterator last );
  ************************************************************/
  //***********************************************************
  //��ָ��λ��positionɾ���ڵ㣬������ֱ�Ӻ�̽ڵ�ĵ�ַ
  iterator erase(iterator __position) {
	  //����ǰ���ͺ�̽ڵ��λ��
    _List_node_base* __next_node = __position._M_node->_M_next;
    _List_node_base* __prev_node = __position._M_node->_M_prev;
    _Node* __n = (_Node*) __position._M_node;
    __prev_node->_M_next = __next_node;
    __next_node->_M_prev = __prev_node;
    _Destroy(&__n->_M_data);
    _M_put_node(__n);
    return iterator((_Node*) __next_node);
  }
  //ɾ������������֮��Ľڵ�
  iterator erase(iterator __first, iterator __last);
  //������������ǵ��ø����clear()����
  void clear() { _Base::clear(); }

  //��������Ĵ�С
  void resize(size_type __new_size, const _Tp& __x);
  void resize(size_type __new_size) { this->resize(__new_size, _Tp()); }

  //ȡ����һ�����ݽڵ�
  void pop_front() { erase(begin()); }
  //ȡ�����һ�����ݽڵ�
  void pop_back() { 
    iterator __tmp = end();
    erase(--__tmp);
  }

  //**********************************************************************
  /***********************�����ǹ��캯��**********************************
  //*******************Ĭ�Ϲ��캯��***************************************
	explicit list( const Allocator& alloc = Allocator() );
  //**********************���г�ֵ�ʹ�С�Ĺ��캯��************************
	explicit list( size_type count,
               const T& value = T(),
               const Allocator& alloc = Allocator());
         list( size_type count,
               const T& value,
               const Allocator& alloc = Allocator());
  //**************ֻ�д�С�Ĺ��캯��**************************************
	explicit list( size_type count );
  //************ĳ����Χ��ֵΪ��ʼֵ�Ĺ��캯��****************************
	template< class InputIt >
	list( InputIt first, InputIt last,
      const Allocator& alloc = Allocator() );
 //************�������캯��***********************************************
	list( const list& other );
  */
  //**********************************************************************
  //���캯��
  //�����Ĭ�Ϲ��캯��
  explicit list(const allocator_type& __a = allocator_type()) : _Base(__a) {}
  list(size_type __n, const _Tp& __value,
       const allocator_type& __a = allocator_type())
    : _Base(__a)
    { insert(begin(), __n, __value); }
  explicit list(size_type __n)
    : _Base(allocator_type())
    { insert(begin(), __n, _Tp()); }

#ifdef __STL_MEMBER_TEMPLATES

  // We don't need any dispatching tricks here, because insert does all of
  // that anyway.  
  template <class _InputIterator>
  list(_InputIterator __first, _InputIterator __last,
       const allocator_type& __a = allocator_type())
    : _Base(__a)
    { insert(begin(), __first, __last); }

#else /* __STL_MEMBER_TEMPLATES */

  list(const _Tp* __first, const _Tp* __last,
       const allocator_type& __a = allocator_type())
    : _Base(__a)
    { this->insert(begin(), __first, __last); }
  list(const_iterator __first, const_iterator __last,
       const allocator_type& __a = allocator_type())
    : _Base(__a)
    { this->insert(begin(), __first, __last); }

#endif /* __STL_MEMBER_TEMPLATES */
  list(const list<_Tp, _Alloc>& __x) : _Base(__x.get_allocator())
    { insert(begin(), __x.begin(), __x.end()); }//�������캯��

  ~list() { }//��������

  //��ֵ����
  list<_Tp, _Alloc>& operator=(const list<_Tp, _Alloc>& __x);
  //���캯����������������ֵ���� ���嵽�˽���
  //*******************************************************************

public:
  // assign(), a generalized assignment member function.  Two
  // versions: one that takes a count, and one that takes a range.
  // The range version is a member template, so we dispatch on whether
  // or not the type is an integer.
  /*********************************************************************
  assign()�����������汾ԭ�ͣ����������Ѷ����list�������ֵ
	void assign( size_type count, const T& value );

	template< class InputIt >
	void assign( InputIt first, InputIt last );
  *******************************************************************
	**���ӣ�
	#include <list>
	#include <iostream>
 
	int main()
	{
		std::list<char> characters;
		������charactersʱ����ʼ��Ϊ�ַ�b�������������һ����Ч
		//std::list<char>characters(5,'b')
 
		characters.assign(5, 'a');
 
		for (char c : characters) {
			std::cout << c << ' ';
		}
 
		return 0;
	}
	**��������a a a a a
  *********************************************************************/
	//�����ǵ�һ���汾void assign( size_type count, const T& value );
  void assign(size_type __n, const _Tp& __val) { _M_fill_assign(__n, __val); }

  //����ΪʲôҪ��_M_fill_assign�����������public�أ��������������Ǹ����𣿣�
  void _M_fill_assign(size_type __n, const _Tp& __val);

#ifdef __STL_MEMBER_TEMPLATES

  //���������assign()�����ĵڶ����汾
  /*
	template< class InputIt >
	void assign( InputIt first, InputIt last );
	������ƫ�ػ��������ж��������������Ƿ�Ϊ�����ͱ�
  */
  template <class _InputIterator>
  void assign(_InputIterator __first, _InputIterator __last) {
    typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
    _M_assign_dispatch(__first, __last, _Integral());
  }

  //��������������Ϊ�����ͱ������͵��˺���
  template <class _Integer>
  void _M_assign_dispatch(_Integer __n, _Integer __val, __true_type)
    { _M_fill_assign((size_type) __n, (_Tp) __val); }

  //�������������Ͳ��������ͱ������͵��˺���
  template <class _InputIterator>
  void _M_assign_dispatch(_InputIterator __first, _InputIterator __last,
                          __false_type);

#endif /* __STL_MEMBER_TEMPLATES */
  //assign()�����������
  //*****************************************************************

protected:
	//������[first,last���Ľڵ����ݲ��뵽ָ���ڵ�position֮ǰ,position�����������ڲ�
	//���������list���protected���ԣ����ǹ����ӿڣ�ֻΪlist���Ա����
	//Ϊ����ƴ�Ӻ���void splice()����
  void transfer(iterator __position, iterator __first, iterator __last) {
    if (__position != __last) {
      // Remove [first, last) from its old position.
      __last._M_node->_M_prev->_M_next     = __position._M_node;
      __first._M_node->_M_prev->_M_next    = __last._M_node;
      __position._M_node->_M_prev->_M_next = __first._M_node; 

      // Splice [first, last) into its new position.
      _List_node_base* __tmp      = __position._M_node->_M_prev;
      __position._M_node->_M_prev = __last._M_node->_M_prev;
      __last._M_node->_M_prev     = __first._M_node->_M_prev; 
      __first._M_node->_M_prev    = __tmp;
    }
  }

public:
	//**********************************************************
	//*******************ƴ�Ӳ�������ӿ�***********************
	//������ƴ�ӵ���ǰ����ָ��λ��position֮ǰ
	/*void splice(const_iterator pos, list& other);
	
	//��it������other��ָ��λ��ƴ�ӵ���ǰ����pos֮ǰ��it��pos��ָ��ͬһ����
	void splice(const_iterator pos, list& other, const_iterator it);

	//������other�Ľڵ㷶Χ[first,last)ƴ���ڵ�ǰ������ָ����λ��pos֮ǰ
	//[first,last)��pos��ָ��ͬһ����
	void splice(const_iterator pos, list& other,
            const_iterator first, const_iterator last);
	*************************************************************/
	//**********************************************************
	//������xƴ�ӵ���ǰ�����ָ��λ��position֮ǰ
	//����x��*this���벻ͬ������������ͬ������
  void splice(iterator __position, list& __x) {
    if (!__x.empty()) 
      this->transfer(__position, __x.begin(), __x.end());
  }
  //��i��ָ��Ľڵ�ƴ�ӵ�position��ָλ��֮ǰ
  //ע�⣺i��position����ָ��ͬһ������
  void splice(iterator __position, list&, iterator __i) {
    iterator __j = __i;
    ++__j;
	//��i��positionָ��ͬһ��������ָ��ͬһλ��
	//����i��positionָ��ͬһ�������Ҿ���position��ֱ��ǰ��λ��
	//�����������������������κβ���
    if (__position == __i || __position == __j) return;
	//���򣬽���ƴ�Ӳ���
    this->transfer(__position, __i, __j);
  }
  //����Χ[first,last)�����нڵ�ƴ�ӵ�position��ָλ��֮ǰ
  //ע�⣺[first,last)��position��ָ��ͬһ������
  //����position������[first,last)��Χ֮��
  void splice(iterator __position, list&, iterator __first, iterator __last) {
    if (__first != __last) 
      this->transfer(__position, __first, __last);
  }
  //�����ǳ�Ա����������������list����ʵ��
  //************************************************************
  //ɾ��������ֵ����value�����нڵ�
  void remove(const _Tp& __value);
  //ɾ�������ظ���Ԫ�ؽڵ㣬ʹ֮Ψһ
  //ע�⣺���������ظ�Ԫ��
  void unique();
  //�ϲ����������������
  void merge(list& __x);
  //��ת��������������
  void reverse();
  //������������������
  void sort();

#ifdef __STL_MEMBER_TEMPLATES
  template <class _Predicate> void remove_if(_Predicate);
  template <class _BinaryPredicate> void unique(_BinaryPredicate);
  template <class _StrictWeakOrdering> void merge(list&, _StrictWeakOrdering);
  template <class _StrictWeakOrdering> void sort(_StrictWeakOrdering);
#endif /* __STL_MEMBER_TEMPLATES */
};
//list�������
//**************************************************************

//**************************************************************
//*****************�����ǱȽ����������������*******************
//**************************************************************
template <class _Tp, class _Alloc>
inline bool 
operator==(const list<_Tp,_Alloc>& __x, const list<_Tp,_Alloc>& __y)
{
  typedef typename list<_Tp,_Alloc>::const_iterator const_iterator;
  const_iterator __end1 = __x.end();
  const_iterator __end2 = __y.end();

  const_iterator __i1 = __x.begin();
  const_iterator __i2 = __y.begin();
  while (__i1 != __end1 && __i2 != __end2 && *__i1 == *__i2) {
    ++__i1;
    ++__i2;
  }
  return __i1 == __end1 && __i2 == __end2;
}

template <class _Tp, class _Alloc>
inline bool operator<(const list<_Tp,_Alloc>& __x,
                      const list<_Tp,_Alloc>& __y)
{
  return lexicographical_compare(__x.begin(), __x.end(),
                                 __y.begin(), __y.end());
}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

template <class _Tp, class _Alloc>
inline bool operator!=(const list<_Tp,_Alloc>& __x,
                       const list<_Tp,_Alloc>& __y) {
  return !(__x == __y);
}

template <class _Tp, class _Alloc>
inline bool operator>(const list<_Tp,_Alloc>& __x,
                      const list<_Tp,_Alloc>& __y) {
  return __y < __x;
}

template <class _Tp, class _Alloc>
inline bool operator<=(const list<_Tp,_Alloc>& __x,
                       const list<_Tp,_Alloc>& __y) {
  return !(__y < __x);
}

template <class _Tp, class _Alloc>
inline bool operator>=(const list<_Tp,_Alloc>& __x,
                       const list<_Tp,_Alloc>& __y) {
  return !(__x < __y);
}


//����������������
template <class _Tp, class _Alloc>
inline void 
swap(list<_Tp, _Alloc>& __x, list<_Tp, _Alloc>& __y)
{
  __x.swap(__y);
}

#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */
//���������ؽ���
//**************************************************************

//������list���Ա�����ľ��嶨��
//**************************************************************
#ifdef __STL_MEMBER_TEMPLATES

template <class _Tp, class _Alloc> template <class _InputIter>
void 
list<_Tp, _Alloc>::_M_insert_dispatch(iterator __position,
                                      _InputIter __first, _InputIter __last,
                                      __false_type)
{
  for ( ; __first != __last; ++__first)//������Χ[first,last)
    insert(__position, *__first);//һ��һ���ڵ����
}

#else /* __STL_MEMBER_TEMPLATES */

template <class _Tp, class _Alloc>
void 
list<_Tp, _Alloc>::insert(iterator __position, 
                          const _Tp* __first, const _Tp* __last)
{
  for ( ; __first != __last; ++__first)//������Χ[first,last)
    insert(__position, *__first);//һ��һ���ڵ����
}

template <class _Tp, class _Alloc>
void 
list<_Tp, _Alloc>::insert(iterator __position,
                         const_iterator __first, const_iterator __last)
{
  for ( ; __first != __last; ++__first)//������Χ[first,last)
    insert(__position, *__first);//һ��һ���ڵ����
}

#endif /* __STL_MEMBER_TEMPLATES */

template <class _Tp, class _Alloc>
void 
list<_Tp, _Alloc>::_M_fill_insert(iterator __position,
                                  size_type __n, const _Tp& __x)
{
  for ( ; __n > 0; --__n)//����n���ڵ�
    insert(__position, __x);//��position֮ǰ����x�ڵ�
}

template <class _Tp, class _Alloc>
typename list<_Tp,_Alloc>::iterator list<_Tp, _Alloc>::erase(iterator __first, 
                                                             iterator __last)
{
  while (__first != __last)//������Χ[first,last)
    erase(__first++);//һ��һ���ڵ�ɾ��
  return __last;
}

//���µ��������Ĵ�С
template <class _Tp, class _Alloc>
void list<_Tp, _Alloc>::resize(size_type __new_size, const _Tp& __x)
{
  iterator __i = begin();
  size_type __len = 0;//��ʾ������ԭʼ��С
  for ( ; __i != end() && __len < __new_size; ++__i, ++__len)
    ;
  if (__len == __new_size)//�������µĳ��ȱ�ԭ����С������������Ԫ��
    erase(__i, end());
  else//�������µĳ��ȱ�ԭ���Ĵ�������ʼ��Ϊxֵ   // __i == end()
    insert(end(), __new_size - __len, __x);
}

//��ֵ����
template <class _Tp, class _Alloc>
list<_Tp, _Alloc>& list<_Tp, _Alloc>::operator=(const list<_Tp, _Alloc>& __x)
{
  if (this != &__x) {
    iterator __first1 = begin();
    iterator __last1 = end();
    const_iterator __first2 = __x.begin();
    const_iterator __last2 = __x.end();
    while (__first1 != __last1 && __first2 != __last2) 
      *__first1++ = *__first2++;
    if (__first2 == __last2)//����ǰ�����Ĵ�С����x������С
      erase(__first1, __last1);//��������ಿ��
    else//����ǰ������СС��x������С�����x����ʣ�µ����ݲ��뵽��ǰ����β
      insert(__last1, __first2, __last2);
	//����if�����������������������
	/*
		clear();
		this->assign(__x.begin(),__x.end());
	*/
  }
  return *this;
}

//���Ѷ���list���������n����ʼֵΪval�Ľڵ�
template <class _Tp, class _Alloc>
void list<_Tp, _Alloc>::_M_fill_assign(size_type __n, const _Tp& __val) {
  iterator __i = begin();
  for ( ; __i != end() && __n > 0; ++__i, --__n)
    *__i = __val;
  if (__n > 0)//��������С�����洢n���ڵ㣬��ʹ�ò��뺯��
    insert(end(), __n, __val);
  else//������ԭ�������ݴ�С��n����������������
    erase(__i, end());
  //ע��������Ϊ�ú���Ҳ��������ʵ�֣�
  //�����������ԭ��������
  //Ȼ������������n��ֵΪval�����ݽڵ�
  /*
  _Tp tmp = __val;
  clear();
  insert(begin(),__n,__val);
  */
}

#ifdef __STL_MEMBER_TEMPLATES

//�������������Ͳ��������ͱ�ʱ��assign(_InputIter __first, _InputIter __last)���øú���
//��[first,last)ʵ�������ֵ����
template <class _Tp, class _Alloc> template <class _InputIter>
void
list<_Tp, _Alloc>::_M_assign_dispatch(_InputIter __first2, _InputIter __last2,
                                      __false_type)
{
  //��ȡԭʼ�����Ĵ�С
  iterator __first1 = begin();
  iterator __last1 = end();
  //��ԭʼ������[first2,last2)��С��Ϊ0��1������и�ֵ����
  for ( ; __first1 != __last1 && __first2 != __last2; ++__first1, ++__first2)
    *__first1 = *__first2;
  if (__first2 == __last2)//��ԭʼ�����Ĵ�С��[first2,last2)��
    erase(__first1, __last1);
  else
	  //��ԭʼ�����Ĵ�С��[first2,last2)С
    insert(__last1, __first2, __last2);
}

#endif /* __STL_MEMBER_TEMPLATES */

//ɾ��������ֵΪvalue���������ݽڵ�
template <class _Tp, class _Alloc>
void list<_Tp, _Alloc>::remove(const _Tp& __value)
{
  iterator __first = begin();
  iterator __last = end();
  while (__first != __last) {//������������
    iterator __next = __first;
    ++__next;
    if (*__first == __value) erase(__first);//�����ڸ�ֵ�������
    __first = __next;//�������ң�ֱ��first == last
  }
}

//
template <class _Tp, class _Alloc>
void list<_Tp, _Alloc>::unique()
{
  iterator __first = begin();
  iterator __last = end();
  if (__first == __last) return;//��Ϊ�����������˳�
  iterator __next = __first;
  while (++__next != __last) {//��������С����1������whileѭ��
    if (*__first == *__next)//������Ԫ����ͬ
      erase(__next);//�����
    else//���򣬲�����һ�ڵ�
      __first = __next;
    __next = __first;
  }
}

//�ϲ����������������,�ϲ����������Ȼ�������
template <class _Tp, class _Alloc>
void list<_Tp, _Alloc>::merge(list<_Tp, _Alloc>& __x)
{
  iterator __first1 = begin();
  iterator __last1 = end();
  iterator __first2 = __x.begin();
  iterator __last2 = __x.end();
  while (__first1 != __last1 && __first2 != __last2)
    if (*__first2 < *__first1) {
      iterator __next = __first2;
      transfer(__first1, __first2, ++__next);//��first2ƴ����first1֮ǰ
      __first2 = __next;
    }
    else
      ++__first1;
  //������x�ȵ�ǰ���������ʣ������ݽڵ�ƴ�ӵ���ǰ�����β��
  if (__first2 != __last2) transfer(__last1, __first2, __last2);
}


inline void __List_base_reverse(_List_node_base* __p)
{
  _List_node_base* __tmp = __p;
  do {
    __STD::swap(__tmp->_M_next, __tmp->_M_prev);//����ָ����ָ�Ľڵ��ַ
    __tmp = __tmp->_M_prev;     // Old next node is now prev.
  } while (__tmp != __p);
}

//�ѵ�ǰ��������
template <class _Tp, class _Alloc>
inline void list<_Tp, _Alloc>::reverse() 
{
  __List_base_reverse(this->_M_node);
}    

//�������������list����ĵ���������ʱ˫�������
//��ΪSTL�������㷨����sort()�ǽ���������ʵ������������ﲢ���ʺ�
template <class _Tp, class _Alloc>
void list<_Tp, _Alloc>::sort()
{
  // Do nothing if the list has length 0 or 1.
  if (_M_node->_M_next != _M_node && _M_node->_M_next->_M_next != _M_node) 
  {
    list<_Tp, _Alloc> __carry;//carry�����𵽰��˵�����
	//counter�������м�洢����
	/*
	*���ж���counter[i]�������Ĵ洢����Ϊ2^(i+1)���ڵ�
	*�����������λ��λ��counter[i+1]
	*/
    list<_Tp, _Alloc> __counter[64];
    int __fill = 0;
    while (!empty()) 
	{//�����ǿ�����
		//��һ����
      __carry.splice(__carry.begin(), *this, begin());//�ѵ�ǰ����ĵ�һ���ڵ����carry����ͷ
      int __i = 0;
      while(__i < __fill && !__counter[__i].empty()) 
	  {
        //�ڶ�����
		  __counter[__i].merge(__carry);//������carry�ϲ���counter[i]
        //��������
		  __carry.swap(__counter[__i++]);//��������carry��counter[i]����
      }
      //���Ĳ���
	  __carry.swap(__counter[__i]);//��������carry��counter[i]����         
      //���岽��
	  if (__i == __fill) ++__fill;
    } 

    for (int __i = 1; __i < __fill; ++__i)
      //��������
		__counter[__i].merge(__counter[__i-1]);//�ѵ�λ�������λ��ʣ������ȫ������ĺϲ�����һλ
    //���߲���
	swap(__counter[__fill-1]);//����������õ��������ݽ�������ǰ����
  }
}

#ifdef __STL_MEMBER_TEMPLATES

template <class _Tp, class _Alloc> template <class _Predicate>
void list<_Tp, _Alloc>::remove_if(_Predicate __pred)
{
  iterator __first = begin();
  iterator __last = end();
  while (__first != __last) {
    iterator __next = __first;
    ++__next;
    if (__pred(*__first)) erase(__first);
    __first = __next;
  }
}

template <class _Tp, class _Alloc> template <class _BinaryPredicate>
void list<_Tp, _Alloc>::unique(_BinaryPredicate __binary_pred)
{
  iterator __first = begin();
  iterator __last = end();
  if (__first == __last) return;
  iterator __next = __first;
  while (++__next != __last) {
    if (__binary_pred(*__first, *__next))
      erase(__next);
    else
      __first = __next;
    __next = __first;
  }
}

template <class _Tp, class _Alloc> template <class _StrictWeakOrdering>
void list<_Tp, _Alloc>::merge(list<_Tp, _Alloc>& __x,
                              _StrictWeakOrdering __comp)
{
  iterator __first1 = begin();
  iterator __last1 = end();
  iterator __first2 = __x.begin();
  iterator __last2 = __x.end();
  while (__first1 != __last1 && __first2 != __last2)
    if (__comp(*__first2, *__first1)) {
      iterator __next = __first2;
      transfer(__first1, __first2, ++__next);
      __first2 = __next;
    }
    else
      ++__first1;
  if (__first2 != __last2) transfer(__last1, __first2, __last2);
}

template <class _Tp, class _Alloc> template <class _StrictWeakOrdering>
void list<_Tp, _Alloc>::sort(_StrictWeakOrdering __comp)
{
  // Do nothing if the list has length 0 or 1.
  if (_M_node->_M_next != _M_node && _M_node->_M_next->_M_next != _M_node) {
    list<_Tp, _Alloc> __carry;
    list<_Tp, _Alloc> __counter[64];
    int __fill = 0;
    while (!empty()) {
      __carry.splice(__carry.begin(), *this, begin());
      int __i = 0;
      while(__i < __fill && !__counter[__i].empty()) {
        __counter[__i].merge(__carry, __comp);
        __carry.swap(__counter[__i++]);
      }
      __carry.swap(__counter[__i]);         
      if (__i == __fill) ++__fill;
    } 

    for (int __i = 1; __i < __fill; ++__i) 
      __counter[__i].merge(__counter[__i-1], __comp);
    swap(__counter[__fill-1]);
  }
}

#endif /* __STL_MEMBER_TEMPLATES */

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1174
#pragma reset woff 1375
#endif

__STL_END_NAMESPACE 

#endif /* __SGI_STL_INTERNAL_LIST_H */

// Local Variables:
// mode:C++
// End:
