#pragma once

#include <utility>
#include "Series.h"

namespace data
{
	using namespace std;


	template<typename C, typename K, typename V>
	class Frame:public Series<C,Series<K,V>>
	{		
	public:

		Frame() :Series<C, Series<K, V>>(){};
		~Frame();
		explicit Frame(Series<C, Series<K, V>>&& s)
			: Series<C, Series<K, V>>(s)
		{
		}

		explicit Frame(const Series<C, Series<K, V>>& s)
			: Series<C, Series<K, V>>(s)
		{
		}

		Frame& withSeries(const C& columnkey,Series<K, V>& series);
		Frame& withSeries(const C& columnkey,Series<K, V>&& series);
		Series<K, V>& col(const C& col);				
		
	};

	template <typename C, typename K, typename V>
	Frame<C, K, V>::~Frame()
	{
	}

	template <typename C, typename K, typename V>
	Frame<C, K, V>& Frame<C, K, V>::withSeries(const C& columnkey, Series<K, V>& series)
	{
		withdata({ std::pair<C,decltype(series)>(columnkey, series) });
		return *this;
	}

	template <typename C, typename K, typename V>
	Frame<C, K, V>& Frame<C, K, V>::withSeries(const C& columnkey, Series<K, V>&& series)
	{
		withdata({ std::pair<C, decltype(series)>(columnkey, std::move(series)) });
		return *this;
	}

	template <typename C, typename K, typename V>
	Series<K, V>& Frame<C, K, V>::col(const C& col)
	{
		return this->operator[](col);
	}
	
}
