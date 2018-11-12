#pragma once

#include <utility>
#include "Series.h"
#include <algorithm>

namespace data
{
	using namespace std;


	template<typename C, typename K,typename ...V>
	class Frame: Series<C,Series<K,V>> ...
	{	
	public:
		
		Frame(){};
		~Frame(){}

		template<typename T>
		Frame<C,K,V...>& addColumn(const C& columnkey,Series<K, T>& series);
		
		template<typename T>
		Frame<C, K,V...>& addColumn(const C& columnkey, Series<K, T>&& series);


		template<class T>
		Series<K, T>& getColAs(const C& col);			

	};

	template <typename C, typename K, typename ... V>
	template <typename T>
	Frame<C, K, V...>& Frame<C, K, V...>::addColumn(const C& columnkey, Series<K, T>& series)
	{
		this->Series<C, Series<K, T>>::withdata({ make_pair(columnkey, series) });
		return *this;
	}

	template <typename C, typename K, typename ... V>
	template <typename T>
	Frame<C, K, V...>& Frame<C, K, V...>::addColumn(const C& columnkey, Series<K, T>&& series)
	{
		this->Series<C, Series<K, T>>::withdata({ make_pair(columnkey, std::move(series)) });
		return *this;
	}

	template <typename C, typename K, typename ... V>
	template <class T>
	Series<K, T>& Frame<C, K, V...>::getColAs(const C& col)
	{
		return this->Series<C, Series<K, T>>::operator[](col);
	}
	
}

