#ifndef __SGI_STL_INTERNAL_STACK_H
#define __SGI_STL_INTERNAL_STACK_H

#include <sequence_concepts.h>

__STL_BEGIN_NAMESPACE

// Forward declarations of operators == and <, needed for friend declaration.
//����Ĭ�ϵĵײ�����������deque����
template <class _Tp, 
          class _Sequence __STL_DEPENDENT_DEFAULT_TMPL(deque<_Tp>) >
class stack;

template <class _Tp, class _Seq>
bool operator==(const stack<_Tp,_Seq>& __x, const stack<_Tp,_Seq>& __y);

template <class _Tp, class _Seq>
bool operator<(const stack<_Tp,_Seq>& __x, const stack<_Tp,_Seq>& __y);


template <class _Tp, class _Sequence>
class stack {

  // requirements:

  __STL_CLASS_REQUIRES(_Tp, _Assignable);
  __STL_CLASS_REQUIRES(_Sequence, _BackInsertionSequence);
  typedef typename _Sequence::value_type _Sequence_value_type;
  __STL_CLASS_REQUIRES_SAME_TYPE(_Tp, _Sequence_value_type);


#ifdef __STL_MEMBER_TEMPLATES
  template <class _Tp1, class _Seq1>
  friend bool operator== (const stack<_Tp1, _Seq1>&,
                          const stack<_Tp1, _Seq1>&);
  template <class _Tp1, class _Seq1>
  friend bool operator< (const stack<_Tp1, _Seq1>&,
                         const stack<_Tp1, _Seq1>&);
#else /* __STL_MEMBER_TEMPLATES */
  friend bool __STD_QUALIFIER
  operator== __STL_NULL_TMPL_ARGS (const stack&, const stack&);
  friend bool __STD_QUALIFIER
  operator< __STL_NULL_TMPL_ARGS (const stack&, const stack&);
#endif /* __STL_MEMBER_TEMPLATES */

public:
	// ����stack��֧�ֶ�ջ��Ԫ�صĲ���, ���Բ�����STLҪ���  
	// pointer, iterator, difference_type 
  typedef typename _Sequence::value_type      value_type;
  typedef typename _Sequence::size_type       size_type;
  typedef          _Sequence                  container_type;

  typedef typename _Sequence::reference       reference;
  typedef typename _Sequence::const_reference const_reference;
protected:
  _Sequence c;//�ײ��������ͣ�Ĭ��Ϊdeque����
public:
	//�����stack��ά����ȫ�����ڵײ������Ĳ���
  stack() : c() {}
  explicit stack(const _Sequence& __s) : c(__s) {}

  //�ж������Ƿ�Ϊ��
  bool empty() const { return c.empty(); }
  //��ȡ�����Ĵ�С����������Ԫ�صĸ���
  size_type size() const { return c.size(); }
  //����ջ��Ԫ�ص�����
  reference top() { return c.back(); }
  const_reference top() const { return c.back(); }
  //��ջ��׷��Ԫ��
  void push(const value_type& __x) { c.push_back(__x); }
  //����ջ����Ԫ�أ����������κ�����
  void pop() { c.pop_back(); }
};

//�����������ڵײ������Ĳ��������
template <class _Tp, class _Seq>
bool operator==(const stack<_Tp,_Seq>& __x, const stack<_Tp,_Seq>& __y)
{
  return __x.c == __y.c;
}

template <class _Tp, class _Seq>
bool operator<(const stack<_Tp,_Seq>& __x, const stack<_Tp,_Seq>& __y)
{
  return __x.c < __y.c;
}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

template <class _Tp, class _Seq>
bool operator!=(const stack<_Tp,_Seq>& __x, const stack<_Tp,_Seq>& __y)
{
  return !(__x == __y);
}

template <class _Tp, class _Seq>
bool operator>(const stack<_Tp,_Seq>& __x, const stack<_Tp,_Seq>& __y)
{
  return __y < __x;
}

template <class _Tp, class _Seq>
bool operator<=(const stack<_Tp,_Seq>& __x, const stack<_Tp,_Seq>& __y)
{
  return !(__y < __x);
}

template <class _Tp, class _Seq>
bool operator>=(const stack<_Tp,_Seq>& __x, const stack<_Tp,_Seq>& __y)
{
  return !(__x < __y);
}

#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_STACK_H */

// Local Variables:
// mode:C++
// End:


// constructing stacks
#include <iostream>       // std::cout
#include <stack>          // std::stack
#include <vector>         // std::vector
#include <deque>          // std::deque

int main ()
{
  std::deque<int> mydeque (3,100);          // deque with 3 elements
  std::vector<int> myvector (2,200);        // vector with 2 elements

  std::stack<int> first;                    // empty stack
  std::stack<int> second (mydeque);         // stack initialized to copy of deque

  std::stack<int,std::vector<int> > third;  // empty stack using vector
  std::stack<int,std::vector<int> > fourth (myvector);

  std::cout << "size of first: " << first.size() << '\n';
  std::cout << "size of second: " << second.size() << '\n';
  std::cout << "size of third: " << third.size() << '\n';
  std::cout << "size of fourth: " << fourth.size() << '\n';
  second.push(2);
  std::cout << "The element at the top of stack second is: "
        << second.top( ) << "." << std::endl;
  std::cout << "size of second: " << second.size() << '\n';

  return 0;
}

Output:
size of first: 0
size of second: 3
size of third: 0
size of fourth: 2
The element at the top of stack second is:2 .
size of second: 4