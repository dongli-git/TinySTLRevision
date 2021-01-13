#ifndef MYTINYSTL_CONSTRUCT_H_
#define MYTINYSTL_CONSTRUCT_H_

// 这个头文件包含两个函数 construct，destroy
// construct : 负责对象的构造
// destroy   : 负责对象的析构

#include <new>

#include "type_traits.h"
#include "iterator.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)  // unused parameter
#endif // _MSC_VER

namespace mystl
{

// construct 构造对象

template <class Ty>
void construct(Ty* ptr)
{
  //这是placement new，正常的new操作符包含两个动作 
  //1是在堆上分配内存，2是在分配好的内存上创建对象
  //placement new直接在已知的内存上创建对象
  //调用形式为：new （address）（type） initializer
  ::new ((void*)ptr) Ty(); //双冒号前无对象表示是全局的对象
}

template <class Ty1, class Ty2>
void construct(Ty1* ptr, const Ty2& value)
{
  ::new ((void*)ptr) Ty1(value);
}


//variadic templates(可变参数模板)
// +　Forwarding（转发） Reference 
// = perfect reference(完美转发)
template <class Ty, class... Args>
void construct(Ty* ptr, Args&&... args)
{
  ::new ((void*)ptr) Ty(mystl::forward<Args>(args)...);
}

// destroy 将对象析构
//类型自带有trivial destructor 无需额外操作
template <class Ty>
void destroy_one(Ty*, std::true_type) {}


template <class Ty>
void destroy_one(Ty* pointer, std::false_type)
{
  if (pointer != nullptr)
  {
    //How to delete the memory allocated by placement new
    //通过主动调用析构函数destructor进行内存的释放
    pointer->~Ty();
  }
}

//对于系统自带有析构函数的类型，不需要做额外操作
template <class ForwardIter>
void destroy_cat(ForwardIter , ForwardIter , std::true_type) {}

//系统无自带的析构函数，区别在于第三个参数。
template <class ForwardIter>
void destroy_cat(ForwardIter first, ForwardIter last, std::false_type)
{
  for (; first != last; ++first)
    destroy(&*first);
}


//is_trivially_destructible：元素的数值型别（value type）
//是否有destructor
template <class Ty>
void destroy(Ty* pointer)
{
  destroy_one(pointer, std::is_trivially_destructible<Ty>{});
}

template <class ForwardIter>
void destroy(ForwardIter first, ForwardIter last)
{
  destroy_cat(first, last, std::is_trivially_destructible<
              typename iterator_traits<ForwardIter>::value_type>{});
}

} // namespace mystl

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif // !MYTINYSTL_CONSTRUCT_H_

