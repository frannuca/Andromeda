#pragma once

#include <utility>
#include "Series.h"
#include <algorithm>
#include "framevaluetypes.h"
#include "variadictypeiterator.h"
namespace data
{
	using namespace std;
		
	template<typename C, typename K, typename ... V>
	class Frame: protected VariadicTypeIterator<C,K,V ...>
	{		
		public:
		
		Frame(){};
		~Frame(){}

		template<typename T>
		Frame<C,K, V...>& addColumn(const C& columnkey,Series<K, T>& series);
		
		template<typename T>
		Frame<C, K,V...>& addColumn(const C& columnkey, Series<K, T>&& series);


		template<class T>
		Series<K, T>& getColAs(const C& col);
					
		

		set<K> RowIndex()
		{
			map<string, vector<K>> m;
			RowIndex2<V ...>(m,true);
			std::set<K> idx;
			for (auto kv : m) {
				auto v = kv.second;
				idx.insert(v.begin(), v.end());
			}
			
			return idx;
		}

		set<K> GetIntersectedRowIndex()
		{
			map<string, vector<K>> m;
			RowIndex2<V ...>(m,false);
			std::set<K> idx;
			for (auto kv : m) {
				auto v = kv.second;
				if(idx.empty())
				{
					idx.insert(v.begin(), v.end());
				}
				else
				{
					set<K> aux;
					set_intersection(v.begin(), v.end(), idx.begin(), idx.end(), inserter(aux, aux.begin()));
					idx = aux;
				}							
			}

			return idx;
		}
		
		set<C> ColumnsIndex()
		{
			map<string, vector<C>> m;
			ColumnsIndex2<V...>(m);

			std::set<C> idx;
			for (auto kv : m) {
				auto v = kv.second;
				idx.insert(v.begin(), v.end());
			}
			return idx;
		}
				
	};

	template <typename C, typename K, typename ... V>
	template <typename T>
	Frame<C, K,V...>& Frame<C, K, V...>::addColumn(const C& columnkey, Series<K, T>& series)
	{
		this->FrameValueTypes<C, K, T>::withdata({ make_pair(columnkey, series) });
		return *this;
	}

	template <typename C, typename K, typename ... V>
	template <typename T>
	Frame<C, K,V ...>& Frame<C, K, V...>::addColumn(const C& columnkey, Series<K, T>&& series)
	{
		this->FrameValueTypes<C, K, T>::withdata({ make_pair(columnkey, std::move(series)) });
		return *this;
	}

	template <typename C, typename K, typename ... V>
	template <class T>
	Series<K, T>& Frame<C, K, V...>::getColAs(const C& col)
	{
		return this->FrameValueTypes<C, K, T>::operator[](col);
	}
	
}

