#pragma once
#include <map>
#include <vector>
#include <functional>
#include <algorithm>
#include <set>
#include "assertion.h"

namespace data
{
	using namespace std;
			
	template<typename K, typename V,typename cmp= typename std::map<K,V>::key_compare>
	class Series: Has_AritmeticOps<V>
	{	
		map<K, V,cmp> data;
		const std::vector<K> index_;
	public:
		explicit Series(std::vector<pair<K,V>>& kv);
	};

	template <typename K, typename V, typename cmp>
	Series<K, V, cmp>::Series(std::vector<pair<K, V>>& kv)
	{
		for (auto x : kv)
		{
			data.insert(x);
		}
	}
}
