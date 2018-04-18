/*tihs program is in the file of stl_alloc.h from line 588 to 628 */

template <class _Tp>
class allocator {
  typedef alloc _Alloc;          // The underlying allocator.
public:							//�������͵ĳ�Ա�����ں����½�(traits��̼���)����
  typedef size_t     size_type;
  typedef ptrdiff_t  difference_type;
  typedef _Tp*       pointer;
  typedef const _Tp* const_pointer;
  typedef _Tp&       reference;
  typedef const _Tp& const_reference;
  typedef _Tp        value_type;

  template <class _Tp1> struct rebind {//Ƕ��һ��template,�ҽ�����Ψһ��Աother,��һ��typedef;
    typedef allocator<_Tp1> other;
  };
  //�����ǳ�Ա����
  allocator() __STL_NOTHROW {}	//Ĭ�Ϲ��캯��,__STL_NOTHROW�� stl_config.h�ж���,ҪôΪ��,ҪôΪ throw()�쳣����
  allocator(const allocator&) __STL_NOTHROW {}	//���ƹ��캯��
  template <class _Tp1> allocator(const allocator<_Tp1>&) __STL_NOTHROW {}//�����ĸ��ƹ��캯��
  ~allocator() __STL_NOTHROW {}//��������

  pointer address(reference __x) const { return &__x; }//���ض���ĵ�ַ
  const_pointer address(const_reference __x) const { return &__x; }//����const����ĵ�ַ

  // __n is permitted to be 0.  The C++ standard says nothing about what
  // the return value is when __n == 0.
  _Tp* allocate(size_type __n, const void* = 0) {// ���ÿռ䣬�������Ŀռ������Ϊ0����ô���� _Alloc Ҳ�� alloc �� allocate �����������ڴ棬
 //����� alloc �� SGI STL ��Ĭ��ʹ�õ���__default_alloc_template<__NODE_ALLOCATOR_THREADS, 0>���ʵ�֣�����402�У�
    return __n != 0 ? static_cast<_Tp*>(_Alloc::allocate(__n * sizeof(_Tp))) 
                    : 0;
  }

  // __p is not permitted to be a null pointer.
  void deallocate(pointer __p, size_type __n)//�ͷ������õĿռ�
    { _Alloc::deallocate(__p, __n * sizeof(_Tp)); }

  size_type max_size() const __STL_NOTHROW //���ؿɳɹ����õ����ֵ
    { return size_t(-1) / sizeof(_Tp); }

  void construct(pointer __p, const _Tp& __val) { new(__p) _Tp(__val); }//���죬��ͬ��new ((void*)p) T(x)
  void destroy(pointer __p) { __p->~_Tp(); }//��������ͬ��p->~T()
};

//�쳣����
/*tihs program is in the file of stl_alloc.h*/
//line 109 to 118
class __malloc_alloc_template {

private:
//�ڴ治���쳣����
  static void* _S_oom_malloc(size_t);
  static void* _S_oom_realloc(void*, size_t);

#ifndef __STL_STATIC_TEMPLATE_MEMBER_BUG
  static void (* __malloc_alloc_oom_handler)();
#endif
  //line 141 to 146
  //ָ���Լ����쳣����
  static void (* __set_malloc_handler(void (*__f)()))()
  {
    void (* __old)() = __malloc_alloc_oom_handler;
    __malloc_alloc_oom_handler = __f;
    return(__old);
  }
//line 152 to 155
#ifndef __STL_STATIC_TEMPLATE_MEMBER_BUG
template <int __inst>
void (* __malloc_alloc_template<__inst>::__malloc_alloc_oom_handler)() = 0;
#endif
//line 41 to 50
#ifndef __THROW_BAD_ALLOC
#  if defined(__STL_NO_BAD_ALLOC) || !defined(__STL_USE_EXCEPTIONS)
#    include <stdio.h>
#    include <stdlib.h>
//Ĭ�ϵ�ǿ���˳�
#    define __THROW_BAD_ALLOC fprintf(stderr, "out of memory\n"); exit(1)
#  else /* Standard conforming out-of-memory handling */
#    include <new>
//�׳��û�����쳣��������
#    define __THROW_BAD_ALLOC throw std::bad_alloc()
#  endif
#endif


