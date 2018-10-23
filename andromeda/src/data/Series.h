#pragma once
#include <map>
#include <vector>
#include <functional>
#include <algorithm>
#include <memory>
#include "assertion.h"
#include <utility>
#include <functional>
#include <boost/optional.hpp>

namespace data
{
	using namespace std;
			
	template<typename K, typename V>
	class Series: Has_AritmeticOps<V>, public:Storage<K,V>
	{		
	public:
		
		using  TSeries = Series<K, V>;
		
		//ctors
		Series();
		explicit Series(TVEC& kv);	
		Series(const TSeries& that);	
		Series(TSeries&& that) noexcept;

		
		vector<K> Index();
		vector<V> Values();

		V& operator[](const K& k);
	};

	template <typename K, typename V>
	Series<K, V>::Series():data(new TVEC())
	{
	}

	template <typename K, typename V>
	Series<K, V>::Series(TVEC& kv): data(new TVEC())
	{
		std::transform(kv.begin(), kv.end(), std::back_inserter(*data), [](auto x) {return x; });
	}

	template <typename K, typename V>
	Series<K, V>::Series(const TSeries& that) : data(new TVEC())
	{
		std::transform(that.begin(), that.end(), std::back_inserter(*data), [](auto x) {return x; });
	}

	template <typename K, typename V>
	Series<K, V>::Series(TSeries&& that) noexcept
	{
		data.reset(that.data.release());
	}

	template <typename K, typename V>
	vector<K> Series<K, V>::Index()
	{
		std::vector<K> keys;
		std::transform(begin(), end(), std::back_inserter(keys), [](const pair<K, V>* x) {return x->first; });
		return keys;
	}

	template <typename K, typename V>
	vector<V> Series<K, V>::Values()
	{
		std::vector<V> values;
		std::transform(begin(), end(), std::back_inserter(values), [](const pair<K, V>* x) {return x->second; });
		return values;
	}
}
