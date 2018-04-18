#ifndef __SGI_STL_HASH_FUN_H
#define __SGI_STL_HASH_FUN_H

#include <stddef.h>

__STL_BEGIN_NAMESPACE

//hash function �Ǽ���Ԫ��λ�õĺ���
//��Щ�������Զ�hashtable����ȡģ����
//����hashtable���ṩ��ɢ�к�����ȡģ���������

/*
SGI hashtable�������޵Ķ������ͣ�
	struct hash<char*>
	struct hash<const char*>
	struct hash<char> 
	struct hash<unsigned char> 
	struct hash<signed char>
	struct hash<short>
	struct hash<unsigned short> 
	struct hash<int> 
	struct hash<unsigned int>
	struct hash<long> 
	struct hash<unsigned long>
�������ﶨ������ͣ�����ʹ�ã����û���Ҫʹ�ã�������Լ����塣���磺string��double��float
*/
/*�����Լ��Ĺ�ϣ����ʱҪע�����¼��㣺
	[1]ʹ��struct��Ȼ������operator().
	[2]������size_t
	[3]��������Ҫhash��key�����͡�
	[4]������const���͵ġ�
*/

template <class _Key> struct hash { };

//��const char* �ṩ�ַ���ת������
inline size_t __stl_hash_string(const char* __s)
{
  unsigned long __h = 0; 
  for ( ; *__s; ++__s)
    __h = 5*__h + *__s;
  
  return size_t(__h);
}

__STL_TEMPLATE_NULL struct hash<char*>
{
  size_t operator()(const char* __s) const { return __stl_hash_string(__s); }
};

__STL_TEMPLATE_NULL struct hash<const char*>
{
  size_t operator()(const char* __s) const { return __stl_hash_string(__s); }
};

//�����hash��������ֱ�ӷ���ԭֵ
//����char,unsigned char,signed char,int,unsigned int, 
//short, unsigned short, long,unsigned long��ֻ�Ƿ�����ֵ����
__STL_TEMPLATE_NULL struct hash<char> {
  size_t operator()(char __x) const { return __x; }
};
__STL_TEMPLATE_NULL struct hash<unsigned char> {
  size_t operator()(unsigned char __x) const { return __x; }
};
__STL_TEMPLATE_NULL struct hash<signed char> {
  size_t operator()(unsigned char __x) const { return __x; }
};
__STL_TEMPLATE_NULL struct hash<short> {
  size_t operator()(short __x) const { return __x; }
};
__STL_TEMPLATE_NULL struct hash<unsigned short> {
  size_t operator()(unsigned short __x) const { return __x; }
};
__STL_TEMPLATE_NULL struct hash<int> {
  size_t operator()(int __x) const { return __x; }
};
__STL_TEMPLATE_NULL struct hash<unsigned int> {
  size_t operator()(unsigned int __x) const { return __x; }
};
__STL_TEMPLATE_NULL struct hash<long> {
  size_t operator()(long __x) const { return __x; }
};
__STL_TEMPLATE_NULL struct hash<unsigned long> {
  size_t operator()(unsigned long __x) const { return __x; }
};

__STL_END_NAMESPACE

#endif /* __SGI_STL_HASH_FUN_H */

// Local Variables:
// mode:C++
// End: