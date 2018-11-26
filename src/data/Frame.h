#pragma once

#include <utility>
#include "Series.h"
#include <algorithm>

namespace data
{
	using namespace std;
	
	struct FRAMETYPES
	{
		enum class ValueType { INT_TYPE, DOUBLE_TYPE, BOOL_TYPE, STRING_TYPE };
	};
	template<typename C,typename K, typename T>
	class FrameValueTypes: public Series<C, Series<K, T>>
	{		
		T tvalue;
	private:		
		vector<K> getRowIndex()
		{
			set<K> indices;
			std::vector<C> cols = this->Series<C, Series<K, T>>::Index();
			for(auto c:cols)
			{
				vector<K>& keys = (this->Series<C, Series<K, T>>::operator[](c)).Index();				
				indices.insert(keys.begin(), keys.end());
			}
			vector<K> aux;
			std::transform(indices.begin(), indices.end(), std::back_inserter(aux), [](const K& a) {return a; });
			return aux;
		}
		
		vector<C> getColumnIndex()
		{			
			return this->Series<C, Series<K, T>>::Index();			
		}

		public:		

		void RowKeys(map<string, vector<K>>& indices)
		{			
			indices[typeid(tvalue).name()] = getRowIndex();
		}

		void ColumnKeys(map<string, vector<C>>& indices)
		{			
			indices[typeid(tvalue).name()] = getColumnIndex();			
		}

	};

	template<typename C, typename K, typename ... V>
	class Frame: protected FrameValueTypes<C,K,V>...
	{		
	protected:
		std::set<C> columns;
		std::set<K> index;

		public:
		
		Frame(){};
		~Frame(){}

		template<typename T>
		Frame<C,K, V...>& addColumn(const C& columnkey,Series<K, T>& series);
		
		template<typename T>
		Frame<C, K,V...>& addColumn(const C& columnkey, Series<K, T>&& series);


		template<class T>
		Series<K, T>& getColAs(const C& col);
					
		template<typename T1, typename T2,typename T3, typename T4, typename T5>
		void ColumnsIndex2(map<string, vector<C>>& m)
		{
			this->FrameValueTypes<C, K, T1>::ColumnKeys(m);			
			ColumnsIndex2<T2,T3,T4,T5>(m);
		}

		template<typename T2, typename T3, typename T4, typename T5>
		void ColumnsIndex2(map<string, vector<C>>& m)
		{
			this->FrameValueTypes<C, K, T2>::ColumnKeys(m);
			ColumnsIndex2<T3, T4, T5>(m);
		}

		template<typename T3, typename T4, typename T5>
		void ColumnsIndex2(map<string, vector<C>>& m)
		{
			this->FrameValueTypes<C, K, T3>::ColumnKeys(m);
			ColumnsIndex2<T4, T5>(m);
		}

		template<typename T4, typename T5>
		void ColumnsIndex2(map<string, vector<C>>& m)
		{
			this->FrameValueTypes<C, K, T4>::ColumnKeys(m);
			ColumnsIndex2<T5>(m);
		}

		template<typename T5>
		void ColumnsIndex2(map<string, vector<C>>& m)
		{
			this->FrameValueTypes<C, K, T5>::ColumnKeys(m);			
		}

		template<typename T1, typename T2,typename T3, typename T4>
		void RowIndex2(map<string, vector<K>> &m)
		{
			this->FrameValueTypes<C, K, T1>::RowKeys(m);			
			RowIndex2<T2,T3,T4>(m);
		}

		template<typename T2, typename T3, typename T4>
		void RowIndex2(map<string, vector<K>> &m)
		{
			this->FrameValueTypes<C, K, T2>::RowKeys(m);
			RowIndex2<T3, T4>(m);
		}

		template<typename T3, typename T4>
		void RowIndex2(map<string, vector<K>> &m)
		{
			this->FrameValueTypes<C, K, T3>::RowKeys(m);
			RowIndex2<T4>(m);
		}

		template<typename T4>
		void RowIndex2(map<string, vector<K>> &m)
		{
			this->FrameValueTypes<C, K, T4>::RowKeys(m);			
		}

		map<string, vector<K>> RowIndex()
		{
			map<string, vector<K>> m;
			RowIndex2<V ...>(m);
			return m;
		}

		map<string, vector<C>> ColumnsIndex()
		{
			map<string, vector<C>> m;
			ColumnsIndex2<V...>(m);
			return m;
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

