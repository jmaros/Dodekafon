// DebugPrint.cpp

#include <iostream>
#include <utility>
#include <memory>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <future>

#include "DebugPrint.h"


struct named_print {
	void print()const
	{
		std::cout << '\n';
	}
	template<class T>
	void print(T const& t)const
	{
		std::cout << t;
		print();
	}
	template<class T, class...Rest>
	void print(T const& t, Rest const&... rest)const
	{
		std::cout << t << ',';
		print(rest...);
	}
	template<class...Args>
	void operator()(std::string name, Args const&...args)const
	{
		std::cout << name << ":";
		print(args...);
	}
};

template<class Out>
struct forward_as {
	template<class In,
		std::enable_if_t < std::is_convertible<In&&, Out>{} && !std::is_base_of<Out, In>{}, int > * = nullptr
	>
		Out operator()(In && in)const { return std::forward<In>(in); }
	Out&& operator()(Out&& in)const { return std::forward<Out>(in); }
	template<class In,
		std::enable_if_t < std::is_convertible<In&, Out&>{}, int > * = nullptr
	>
		Out & operator()(In & in)const { return in; }
	template<class In,
		std::enable_if_t < std::is_convertible<In const&, Out const&>{}, int > * = nullptr
	>
		Out const& operator()(In const& in)const { return in; }
};

struct my_struct {
	int foo;
	std::string bar;
	double baz;
};

template <typename V, typename ... Ss,
	decltype(std::void_t<
			 std::result_of_t<forward_as<my_struct>(Ss)>...
	>(), int())* = nullptr
>
void apply_visitor(V&& v, Ss && ... ss)
{
	auto convert = forward_as<my_struct>{};

	std::forward<V>(v)("foo", (convert(std::forward<Ss>(ss)).foo)...);
	std::forward<V>(v)("bar", (convert(std::forward<Ss>(ss)).bar)...);
	std::forward<V>(v)("baz", (convert(std::forward<Ss>(ss)).baz)...);
}

struct evil {
	char foo;
	int* bar;
	std::string baz;
};
struct good : my_struct {
	good(my_struct const& s) :my_struct(s) {}
};
struct strange {
	my_struct s;
	operator my_struct()const
	{
		return s;
	}
};

void TestPrint()
{
	apply_visitor(named_print{}, good(my_struct{ 1, "hello", 3.14 }), my_struct{ 2, "world", 2.7 });
	// apply_visitor( named_print{}, evil{6,6,"6"} ); // does not build
	apply_visitor(named_print{}, strange{ my_struct{3, "strange", -0.5} });
}
