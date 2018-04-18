#ifndef __SGI_STL_INTERNAL_QUEUE_H
#define __SGI_STL_INTERNAL_QUEUE_H

#include <sequence_concepts.h>

__STL_BEGIN_NAMESPACE

// Forward declarations of operators < and ==, needed for friend declaration.
//Ĭ�ϵײ�����Ϊdeque����
template <class _Tp, 
          class _Sequence __STL_DEPENDENT_DEFAULT_TMPL(deque<_Tp>) >
class queue;

template <class _Tp, class _Seq>
inline bool operator==(const queue<_Tp, _Seq>&, const queue<_Tp, _Seq>&);

template <class _Tp, class _Seq>
inline bool operator<(const queue<_Tp, _Seq>&, const queue<_Tp, _Seq>&);


template <class _Tp, class _Sequence>
class queue {

  // requirements:

  __STL_CLASS_REQUIRES(_Tp, _Assignable);
  __STL_CLASS_REQUIRES(_Sequence, _FrontInsertionSequence);
  __STL_CLASS_REQUIRES(_Sequence, _BackInsertionSequence);
  typedef typename _Sequence::value_type _Sequence_value_type;
  __STL_CLASS_REQUIRES_SAME_TYPE(_Tp, _Sequence_value_type);


#ifdef __STL_MEMBER_TEMPLATES 
  template <class _Tp1, class _Seq1>
  friend bool operator== (const queue<_Tp1, _Seq1>&,
                          const queue<_Tp1, _Seq1>&);
  template <class _Tp1, class _Seq1>
  friend bool operator< (const queue<_Tp1, _Seq1>&,
                         const queue<_Tp1, _Seq1>&);
#else /* __STL_MEMBER_TEMPLATES */
  friend bool __STD_QUALIFIER
  operator== __STL_NULL_TMPL_ARGS (const queue&, const queue&);
  friend bool __STD_QUALIFIER
  operator<  __STL_NULL_TMPL_ARGS (const queue&, const queue&);
#endif /* __STL_MEMBER_TEMPLATES */

public:
	// queue��֧�ֶ�ͷ����β���Ĳ���, ���Բ�����STLҪ���  
  // pointer, iterator, difference_type 
  typedef typename _Sequence::value_type      value_type;
  typedef typename _Sequence::size_type       size_type;
  typedef          _Sequence                  container_type;

  typedef typename _Sequence::reference       reference;
  typedef typename _Sequence::const_reference const_reference;
protected:
  _Sequence c;//�ײ�������Ĭ��Ϊdeque�������û�������ָ����������
public:
	//�����queue��ά����ȫ�����ڵײ������Ĳ��� 
  queue() : c() {}
  explicit queue(const _Sequence& __c) : c(__c) {}

  //�ж������Ƿ�Ϊ��
  bool empty() const { return c.empty(); }
  //����������Ԫ�صĸ���
  size_type size() const { return c.size(); }
  //���ض�ͷԪ�ص�����
  reference front() { return c.front(); }
  const_reference front() const { return c.front(); }
  //���ض�βԪ�ص�����
  reference back() { return c.back(); }
  const_reference back() const { return c.back(); }
  //ֻ���ڶ�β����Ԫ��
  void push(const value_type& __x) { c.push_back(__x); }
  //ֻ���ڶ�ͷ�Ƴ�Ԫ��
  void pop() { c.pop_front(); }
};

//�����������ڵײ������Ĳ��������  
template <class _Tp, class _Sequence>
bool 
operator==(const queue<_Tp, _Sequence>& __x, const queue<_Tp, _Sequence>& __y)
{
  return __x.c == __y.c;
}

template <class _Tp, class _Sequence>
bool
operator<(const queue<_Tp, _Sequence>& __x, const queue<_Tp, _Sequence>& __y)
{
  return __x.c < __y.c;
}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

template <class _Tp, class _Sequence>
bool
operator!=(const queue<_Tp, _Sequence>& __x, const queue<_Tp, _Sequence>& __y)
{
  return !(__x == __y);
}

template <class _Tp, class _Sequence>
bool 
operator>(const queue<_Tp, _Sequence>& __x, const queue<_Tp, _Sequence>& __y)
{
  return __y < __x;
}

template <class _Tp, class _Sequence>
bool 
operator<=(const queue<_Tp, _Sequence>& __x, const queue<_Tp, _Sequence>& __y)
{
  return !(__y < __x);
}

template <class _Tp, class _Sequence>
bool 
operator>=(const queue<_Tp, _Sequence>& __x, const queue<_Tp, _Sequence>& __y)
{
  return !(__x < __y);
}

#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

template <class _Tp, 
          class _Sequence __STL_DEPENDENT_DEFAULT_TMPL(vector<_Tp>),
          class _Compare
          __STL_DEPENDENT_DEFAULT_TMPL(less<typename _Sequence::value_type>) >
class priority_queue {

  // requirements:

  __STL_CLASS_REQUIRES(_Tp, _Assignable);
  __STL_CLASS_REQUIRES(_Sequence, _Sequence);
  __STL_CLASS_REQUIRES(_Sequence, _RandomAccessContainer);
  typedef typename _Sequence::value_type _Sequence_value_type;
  __STL_CLASS_REQUIRES_SAME_TYPE(_Tp, _Sequence_value_type);
  __STL_CLASS_BINARY_FUNCTION_CHECK(_Compare, bool, _Tp, _Tp);

public:
	// priority_queue��֧�ֶ�ͷ����β���Ĳ���, ���Բ�����STLҪ���  
  // pointer, iterator, difference_type 
  typedef typename _Sequence::value_type      value_type;
  typedef typename _Sequence::size_type       size_type;
  typedef          _Sequence                  container_type;

  typedef typename _Sequence::reference       reference;
  typedef typename _Sequence::const_reference const_reference;
protected:
  _Sequence c;//�ײ�������Ĭ��Ϊvector���û�������ָ����������
  _Compare comp;//���ȼ����߷�ʽ
public:
	//***********************************************************
	//*	���캯��
	//*	priority_queue() 
	//*	explicit priority_queue(const Compare& __x) 
	//*	explicit priority_queue (const Compare& comp = Compare(),
    //*                    const Container& ctnr = Container());
	//*	template <class InputIterator>
    //* priority_queue (InputIterator first, InputIterator last,
    //*                    const Compare& comp = Compare(),
    //*                    const Container& ctnr = Container());
	//***********************************************************
  priority_queue() : c() {}
  explicit priority_queue(const _Compare& __x) :  c(), comp(__x) {}
  priority_queue(const _Compare& __x, const _Sequence& __s) 
    : c(__s), comp(__x) 
    { make_heap(c.begin(), c.end(), comp); }

#ifdef __STL_MEMBER_TEMPLATES
  template <class _InputIterator>
  priority_queue(_InputIterator __first, _InputIterator __last) 
    : c(__first, __last) { make_heap(c.begin(), c.end(), comp); }

  template <class _InputIterator>
  priority_queue(_InputIterator __first, 
                 _InputIterator __last, const _Compare& __x)
    : c(__first, __last), comp(__x) 
    { make_heap(c.begin(), c.end(), comp); }

  template <class _InputIterator>
  priority_queue(_InputIterator __first, _InputIterator __last,
                 const _Compare& __x, const _Sequence& __s)
  : c(__s), comp(__x)
  { 
    c.insert(c.end(), __first, __last);
    make_heap(c.begin(), c.end(), comp);
  }

#else /* __STL_MEMBER_TEMPLATES */
  priority_queue(const value_type* __first, const value_type* __last) 
    : c(__first, __last) { make_heap(c.begin(), c.end(), comp); }

  priority_queue(const value_type* __first, const value_type* __last, 
                 const _Compare& __x) 
    : c(__first, __last), comp(__x)
    { make_heap(c.begin(), c.end(), comp); }

  priority_queue(const value_type* __first, const value_type* __last, 
                 const _Compare& __x, const _Sequence& __c)
    : c(__c), comp(__x) 
  { 
    c.insert(c.end(), __first, __last);
    make_heap(c.begin(), c.end(), comp);
  }
#endif /* __STL_MEMBER_TEMPLATES */

  //�ж������Ƿ�Ϊ��
  bool empty() const { return c.empty(); }
  //��������Ԫ�صĸ���
  size_type size() const { return c.size(); }
  //�������ȼ����Ԫ�ص�����
  const_reference top() const { return c.front(); }
  //����Ԫ�أ����������ȼ�������
  void push(const value_type& __x) {
    __STL_TRY {
      c.push_back(__x); 
      push_heap(c.begin(), c.end(), comp);
    }
    __STL_UNWIND(c.clear());
  }
  //�������ȼ���ߵ�Ԫ��
  void pop() {
    __STL_TRY {
      pop_heap(c.begin(), c.end(), comp);
      c.pop_back();
    }
    __STL_UNWIND(c.clear());
  }
};

// no equality is provided

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_QUEUE_H */

// Local Variables:
// mode:C++
// End:


// constructing queues
#include <iostream>       // std::cout
#include <deque>          // std::deque
#include <list>           // std::list
#include <queue>          // std::queue

int main ()
{
  std::deque<int> mydeck (3,100);        // deque with 3 elements
  std::list<int> mylist (2,200);         // list with 2 elements

  std::queue<int> first;                 // empty queue
  std::queue<int> second (mydeck);       // queue initialized to copy of deque

  std::queue<int,std::list<int> > third; // empty queue with list as underlying container
  std::queue<int,std::list<int> > fourth (mylist);

  std::cout << "size of first: " << first.size() << '\n';
  std::cout << "size of second: " << second.size() << '\n';
  std::cout << "size of third: " << third.size() << '\n';
  std::cout << "size of fourth: " << fourth.size() << '\n';
  std::cout << "The element at the front of queue second is: "
        << second.front( )  << std::endl;
  second.push(10);
  std::cout << "The element at the back of queue second is: "
        << second.back( ) << std::endl;

  return 0;
}

Output:
size of first: 0
size of second: 3
size of third: 0
size of fourth: 2
The element at the front of queue second is: 100
The element at the back of queue second is: 10


// constructing priority queues
#include <iostream>       // std::cout
#include <queue>          // std::priority_queue
#include <vector>         // std::vector
#include <functional>     // std::greater

class mycomparison
{
  bool reverse;
public:
  mycomparison(const bool& revparam=false)
    {reverse=revparam;}
  bool operator() (const int& lhs, const int&rhs) const
  {
    if (reverse) return (lhs>rhs);
    else return (lhs<rhs);
  }
};

int main ()
{
  int myints[]= {10,60,50,20};

  std::priority_queue<int> first;
  std::priority_queue<int> second (myints,myints+4);
  std::priority_queue<int, std::vector<int>, std::greater<int> >
                            third (myints,myints+4);
   std::cout << "third  = ( ";
   while ( !third.empty( ) )
   {
      std::cout << third.top( ) << " ";
      third.pop( );
   }
   std::cout << ")" << std::endl;
  // using mycomparison:
  typedef std::priority_queue<int,std::vector<int>,mycomparison> mypq_type;

  mypq_type fourth (myints,myints+4);                       // less-than comparison
  mypq_type fifth (mycomparison(true));   // greater-than comparison
    std::cout << "fourth  = ( ";
   while ( !fourth.empty( ) )
   {
      std::cout << fourth.top( ) << " ";
      fourth.pop( );
   }
   std::cout << ")" << std::endl;
   std::cout << "fifth  = ( ";
   while ( !fifth.empty( ) )
   {
      std::cout << fifth.top( ) << " ";
      fifth.pop( );
   }
   std::cout << ")" << std::endl;
   
   for (int i = 0; i < 5; i++)
   {
	   fifth.push(i*10);
   }
	std::cout <<"after push the elements,fifth size is :"<<fifth.size()<<std::endl;
    std::cout << "after push the elements,fifth = ( ";
   while ( !fifth.empty( ) )
   {
      std::cout << fifth.top( ) << " ";
      fifth.pop( );
   }
   std::cout << ")" << std::endl;
   

  system("pause");
  return 0;
}
Output:
third  = ( 10 20 50 60 )
fourth  = ( 60 50 20 10 )
fifth  = ( )
after push the elements,fifth size is :5
after push the elements,fifth = ( 0 10 20 30 40 )


