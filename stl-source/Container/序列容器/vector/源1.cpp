//Alloc��SGI STL�Ŀռ���������Ĭ���ǵڶ���������
template <class _Tp, class _Alloc = __STL_DEFAULT_ALLOCATOR(_Tp) >
class vector : protected _Vector_base<_Tp, _Alloc> 
{
  
public://vector����Ƕ�ͱ���,��iterator_traits<I>���������
  typedef _Tp value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type* iterator;//vector�����ĵ���������ָͨ��
  typedef const value_type* const_iterator;
  ...

  public://���¶���vector������
  iterator begin() { return _M_start; }//ָ����ʹ�ÿռ�ͷ�ĵ�����
  const_iterator begin() const { return _M_start; }
  iterator end() { return _M_finish; }//ָ����ʹ�ÿռ�β�ĵ�����
  const_iterator end() const { return _M_finish; }

  reverse_iterator rbegin()
    { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const
    { return const_reverse_iterator(end()); }
  reverse_iterator rend()
    { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const
    { return const_reverse_iterator(begin()); }
	...
};



 /*������vector�����Ĺ��캯��*******************************************
/**********************************************************************
***	//Ĭ�Ϲ��캯��*****************************************************
*	explicit vector( const Allocator& alloc = Allocator() );		  *
***	//���г�ʼֵ��������С�Ĺ��캯��***********************************
*	explicit vector( size_type count,								  *
*                 const T& value = T(),                               *
*                 const Allocator& alloc = Allocator());              *
*         vector( size_type count,                                    *
*                 const T& value,                                     *
*                 const Allocator& alloc = Allocator());              *
***	//ֻ��������С�Ĺ��캯��*******************************************
*	explicit vector( size_type count );                               *
***	//�����������������ʾ������С�Ĺ��캯��***************************
*	template< class InputIt >                                         *
*	vector( InputIt first, InputIt last,                              *
*        const Allocator& alloc = Allocator() );                      *  
***	//�������캯��*****************************************************
*	vector( const vector& other );                                    *
*	vector( const vector& other, const Allocator& alloc );            * 
***	//�ƶ����캯��*****************************************************
*	vector( vector&& other );                                         *
*	vector( vector&& other, const Allocator& alloc );                 *
***	//�ó�ʼ�б��ֵ�����������б��ڵ�Ԫ��ֵ���Բ�ͬ*******************
*	vector( std::initializer_list<T> init,                            *
*        const Allocator& alloc = Allocator() );                      *
***********************************************************************/ 
  explicit vector(const allocator_type& __a = allocator_type())
    : _Base(__a) {}//Ĭ�Ϲ��캯��

  vector(size_type __n, const _Tp& __value,
         const allocator_type& __a = allocator_type()) 
    : _Base(__n, __a)//���캯�����������n����ʼֵΪvalue��Ԫ��
	//ȫ�ֺ��������ֵ���������ӵ�ַM_start��ʼ�������n����ʼֵΪvalue��Ԫ��
    { _M_finish = uninitialized_fill_n(_M_start, __n, __value); }

  explicit vector(size_type __n)//�ù��캯�������ܳ�ʼֵ��ֻ�������װ���Ԫ�صĸ���n
    : _Base(__n, allocator_type())
    { _M_finish = uninitialized_fill_n(_M_start, __n, _Tp()); }

  vector(const vector<_Tp, _Alloc>& __x) 
    : _Base(__x.size(), __x.get_allocator())//�������캯��
    { _M_finish = uninitialized_copy(__x.begin(), __x.end(), _M_start); }

#ifdef __STL_MEMBER_TEMPLATES
  // Check whether it's an integral type.  If so, it's not an iterator.
  /*�����ĳ������Ĺ��캯���������ж������Ƿ�Ϊ����_Integral()
  *����__type_traits����
  */
  template <class _InputIterator>
  vector(_InputIterator __first, _InputIterator __last,
         const allocator_type& __a = allocator_type()) : _Base(__a) {
    typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
    _M_initialize_aux(__first, __last, _Integral());
  }

  template <class _Integer>
  //������Ϊ����������øú���
  void _M_initialize_aux(_Integer __n, _Integer __value, __true_type) {
    _M_start = _M_allocate(__n);
    _M_end_of_storage = _M_start + __n; 
    _M_finish = uninitialized_fill_n(_M_start, __n, __value);
  }

  template <class _InputIterator>
  //�����벻�������������Traits���������жϵ�����������
  void _M_initialize_aux(_InputIterator __first, _InputIterator __last,
                         __false_type) {
    _M_range_initialize(__first, __last, __ITERATOR_CATEGORY(__first));
  }

#else
  vector(const _Tp* __first, const _Tp* __last,
         const allocator_type& __a = allocator_type())
    : _Base(__last - __first, __a) 
    { _M_finish = uninitialized_copy(__first, __last, _M_start); }
#endif /* __STL_MEMBER_TEMPLATES */

  ~vector() { destroy(_M_start, _M_finish); }//��������


    /*������������һЩ��Ա����*/
  size_type size() const//vector������С(��ʹ�ÿռ��С)���������ڴ洢Ԫ�صĸ���
    { return size_type(end() - begin()); }
  size_type max_size() const//���ؿ��������Ԫ����
    { return size_type(-1) / sizeof(_Tp); }
  size_type capacity() const//vector�������ÿռ�Ĵ�С
    { return size_type(_M_end_of_storage - begin()); }
  bool empty() const//�ж������Ƿ�Ϊ��
    { return begin() == end(); }

  reference operator[](size_type __n) { return *(begin() + __n); }//����ָ��λ�õ�Ԫ��
  const_reference operator[](size_type __n) const { return *(begin() + __n); }

#ifdef __STL_THROW_RANGE_ERRORS
  //���û�Ҫ��Ŀռ���ڿ��ÿռ䣬�׳���ȥ��Ϣ����Խ����
  void _M_range_check(size_type __n) const {
    if (__n >= this->size())
      __stl_throw_range_error("vector");
  }

  reference at(size_type __n)//����ָ��Ԫ�أ����ҽ���Խ����
    { _M_range_check(__n); return (*this)[__n]; }//����ǰ���Ƚ���Խ����
  const_reference at(size_type __n) const
    { _M_range_check(__n); return (*this)[__n]; }
#endif /* __STL_THROW_RANGE_ERRORS */

    void reserve(size_type __n) {//�ı���ÿռ��ڴ��С
    if (capacity() < __n) {
      const size_type __old_size = size();
	  //���·����СΪn���ڴ�ռ䣬����ԭ�����ݸ��Ƶ��·���ռ�
      iterator __tmp = _M_allocate_and_copy(__n, _M_start, _M_finish);
      destroy(_M_start, _M_finish);//�ͷ�����Ԫ�ض���
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);//����ԭ�����ڴ�ռ�
	  //������������ָ�ĵ�ַ,��Ϊԭ����������ָ�ĵ�ַ�Ѿ�ʧЧ
      _M_start = __tmp;
      _M_finish = __tmp + __old_size;
      _M_end_of_storage = _M_start + __n;
    }
  }

  reference front() { return *begin(); }//���ص�һ��Ԫ��
  const_reference front() const { return *begin(); }
  reference back() { return *(end() - 1); }//�����������һ��Ԫ��
  const_reference back() const { return *(end() - 1); }

  void push_back(const _Tp& __x) {//����β�˲���Ԫ��
    if (_M_finish != _M_end_of_storage) {//���п��õ��ڴ�ռ�
      construct(_M_finish, __x);//�������
      ++_M_finish;
    }
    else//��û�п��õ��ڴ�ռ�,�������º�������x���뵽ָ��λ��
      _M_insert_aux(end(), __x);
  }
  void push_back() {
    if (_M_finish != _M_end_of_storage) {
      construct(_M_finish);
      ++_M_finish;
    }
    else
      _M_insert_aux(end());
  }
  void swap(vector<_Tp, _Alloc>& __x) {
	  /*��������������
	  *����ʹ�õķ����ǽ�����������ָ�ĵ�ַ
	  */
    __STD::swap(_M_start, __x._M_start);
    __STD::swap(_M_finish, __x._M_finish);
    __STD::swap(_M_end_of_storage, __x._M_end_of_storage);
  }

  iterator insert(iterator __position, const _Tp& __x) {//��xֵ���뵽ָ����λ��
    size_type __n = __position - begin();
    if (_M_finish != _M_end_of_storage && __position == end()) {
      construct(_M_finish, __x);
      ++_M_finish;
    }
    else
      _M_insert_aux(__position, __x);
    return begin() + __n;
  }
  iterator insert(iterator __position) {
    size_type __n = __position - begin();
    if (_M_finish != _M_end_of_storage && __position == end()) {
      construct(_M_finish);
      ++_M_finish;
    }
    else
      _M_insert_aux(__position);
    return begin() + __n;
  }

  void insert (iterator __pos, size_type __n, const _Tp& __x)
    { //��posλ����������n����ʼֵΪx��Ԫ��
		_M_fill_insert(__pos, __n, __x); }
 
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, 
                                         const _Tp& __x)
{
  if (__n != 0) {//��n��Ϊ0���������Ч
    if (size_type(_M_end_of_storage - _M_finish) >= __n) {//�����㹻�Ŀ��ÿռ�,�����ÿռ䲻С���²���Ԫ�ظ���
      _Tp __x_copy = __x;
      const size_type __elems_after = _M_finish - __position;//��������֮�������Ԫ�ظ���
      iterator __old_finish = _M_finish;

	  //case1-a�������֮�������Ԫ�ظ��������²���Ԫ�ظ���
      if (__elems_after > __n) {
        uninitialized_copy(_M_finish - __n, _M_finish, _M_finish);//��[finish-n,finish)֮������ݸ���[finish,finish+n)
        _M_finish += __n;//����������finish��ָ��λ��
        copy_backward(__position, __old_finish - __n, __old_finish);//��[position,old_finish-n)֮������ݸ���[old_finish-n,old_finish)
        fill(__position, __position + __n, __x_copy);//��ָ��λ��(�����)����ʼֵ
      }

	  //case1-b�������֮�������Ԫ�ظ����������²���Ԫ�ظ���
      else {
        uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy);//���ڿ��ÿռ�����n-elems_after����ʼֵx
        _M_finish += __n - __elems_after;//����������finish
        uninitialized_copy(__position, __old_finish, _M_finish);//��[position,old_finish)֮������ݸ��Ƶ�[old_finish,finish)
        _M_finish += __elems_after;
        fill(__position, __old_finish, __x_copy);
      }
    }

	//case2�������ÿռ�С���²���Ԫ�ظ���
    else {//�����ÿռ�С���²���Ԫ�ظ�����������µĿռ�
		//����ԭʼ���ݸ��Ƶ��µĿռ䣬����������
      const size_type __old_size = size(); //��ȡԭʼ�ռ�Ĵ�С  
	  //�µĿռ�Ϊ�ɿռ����������Ϊ�ɿռ�+������Ԫ�ظ���
      const size_type __len = __old_size + max(__old_size, __n);
	  //�����µĿռ�
      iterator __new_start = _M_allocate(__len);
      iterator __new_finish = __new_start;
      __STL_TRY {//�Ѳ����֮ǰ��ԭʼ���ݸ��Ƶ��µĿռ�
        __new_finish = uninitialized_copy(_M_start, __position, __new_start);
		//���¼������������[new_finish,new_finish+n)
        __new_finish = uninitialized_fill_n(__new_finish, __n, __x);
		//�������֮���ԭʼ���ݸ��Ƶ��¿ռ�
        __new_finish
          = uninitialized_copy(__position, _M_finish, __new_finish);
      }
	  //�ͷ�ԭ���ռ�Ķ�����ڴ�
      __STL_UNWIND((destroy(__new_start,__new_finish), 
                    _M_deallocate(__new_start,__len)));
      destroy(_M_start, _M_finish);
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
	  //������������ָ��λ��
      _M_start = __new_start;
      _M_finish = __new_finish;
      _M_end_of_storage = __new_start + __len;
    }
  }
}

  void _M_fill_insert (iterator __pos, size_type __n, const _Tp& __x);

  void pop_back() {//ȡ����β��Ԫ��
    --_M_finish;
    destroy(_M_finish);//��������
  }
  iterator erase(iterator __position) {//����ָ��λ��Ԫ��
    if (__position + 1 != end())
      copy(__position + 1, _M_finish, __position);//����Ԫ��ǰ��һλ
    --_M_finish;
    destroy(_M_finish);//��������
    return __position;
  }
  iterator erase(iterator __first, iterator __last) {//�������������������Ԫ��
    iterator __i = copy(__last, _M_finish, __first);//�Ѳ�������Ԫ��ǰ��
    destroy(__i, _M_finish);//��������
    _M_finish = _M_finish - (__last - __first);//����finish����ָ��λ��
    return __first;
  }

  void resize(size_type __new_size, const _Tp& __x) {//�ı������пɴ洢��Ԫ�ظ���������������µĿռ�
    if (__new_size < size()) //����������ڴ�ռ��ԭ����С
      erase(begin() + __new_size, end());//���������Ԫ��
    else
      insert(end(), __new_size - size(), __x);//��ԭ������Ŀռ䶼�����ֵx
  }
  void resize(size_type __new_size) { resize(__new_size, _Tp()); }
  void clear() { erase(begin(), end()); }//�������
     // assign(), a generalized assignment member function.  Two
  // versions: one that takes a count, and one that takes a range.
  // The range version is a member template, so we dispatch on whether
  // or not the type is an integer.

  /*�ú������������ͣ�
	void assign( size_type count, const T& value );

	template< class InputIt >
	void assign( InputIt first, InputIt last );
	*/

  //�����������滻Ϊn����ʼֵΪvalue
  void assign(size_type __n, const _Tp& __val) { _M_fill_assign(__n, __val); }
  void _M_fill_assign(size_type __n, const _Tp& __val);

#ifdef __STL_MEMBER_TEMPLATES
  
  template <class _InputIterator>
  void assign(_InputIterator __first, _InputIterator __last) {
    typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
    _M_assign_dispatch(__first, __last, _Integral());
  }

  template <class _Integer>
  void _M_assign_dispatch(_Integer __n, _Integer __val, __true_type)
    { _M_fill_assign((size_type) __n, (_Tp) __val); }

  template <class _InputIter>
  void _M_assign_dispatch(_InputIter __first, _InputIter __last, __false_type)
    { _M_assign_aux(__first, __last, __ITERATOR_CATEGORY(__first)); }

  template <class _InputIterator>
  void _M_assign_aux(_InputIterator __first, _InputIterator __last,
                     input_iterator_tag);

  template <class _ForwardIterator>
  void _M_assign_aux(_ForwardIterator __first, _ForwardIterator __last,
                     forward_iterator_tag); 

#endif /* __STL_MEMBER_TEMPLATES */