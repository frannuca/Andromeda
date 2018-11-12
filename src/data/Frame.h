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
	template<typename C,typename K>
	class FrameValueTypes: protected Series<C, Series<K, int>>, 
						   protected Series<C, Series<K, double>>,
						   protected Series<C, Series<K, bool>>,
						   protected Series<C, Series<K, string>>
	{
	private:

		template<typename T>
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

		template<typename T>
		vector<C> getColumnIndex()
		{			
			return this->Series<C, Series<K, T>>::Index();			
		}

		public:		

		map<FRAMETYPES::ValueType,vector<K>> RowKeys()
		{
			map<FRAMETYPES::ValueType, vector<K>> indices;
			indices[FRAMETYPES::ValueType::INT_TYPE] = getRowIndex<int>();
			indices[FRAMETYPES::ValueType::DOUBLE_TYPE] = getRowIndex<double>();
			indices[FRAMETYPES::ValueType::BOOL_TYPE] = getRowIndex<bool>();
			indices[FRAMETYPES::ValueType::STRING_TYPE] = getRowIndex<string>();
			return indices;
		}

		map<FRAMETYPES::ValueType,vector<C>> ColumnKeys()
		{
			map<FRAMETYPES::ValueType, vector<C>> indices;
			indices[FRAMETYPES::ValueType::INT_TYPE] = getColumnIndex<int>();
			indices[FRAMETYPES::ValueType::DOUBLE_TYPE] = getColumnIndex<double>();
			indices[FRAMETYPES::ValueType::BOOL_TYPE] = getColumnIndex<bool>();
			indices[FRAMETYPES::ValueType::STRING_TYPE] = getColumnIndex<string>();
			return indices;
		}

	};

	template<typename C, typename K>
	class Frame: protected FrameValueTypes<C,K>
	{		
	protected:
		std::set<C> columns;
		std::set<K> index;

		public:
		
		Frame(){};
		~Frame(){}

		template<typename T>
		Frame<C,K>& addColumn(const C& columnkey,Series<K, T>& series);
		
		template<typename T>
		Frame<C, K>& addColumn(const C& columnkey, Series<K, T>&& series);


		template<class T>
		Series<K, T>& getColAs(const C& col);
			
		map<FRAMETYPES::ValueType, vector<C>> ColumnsIndex() { return this->FrameValueTypes<C, K>::ColumnKeys(); }
		map<FRAMETYPES::ValueType, vector<K>> RowIndex() { return this->FrameValueTypes<C, K>::RowKeys(); }

	};

	template <typename C, typename K>
	template <typename T>
	Frame<C, K>& Frame<C, K>::addColumn(const C& columnkey, Series<K, T>& series)
	{
		this->Series<C, Series<K, T>>::withdata({ make_pair(columnkey, series) });
		columns.insert(columnkey);
		std::vector<K> idx = series.Index();
		index.insert(idx.begin(), idx.end());
		return *this;
	}

	template <typename C, typename K>
	template <typename T>
	Frame<C, K>& Frame<C, K>::addColumn(const C& columnkey, Series<K, T>&& series)
	{
		columns.insert(columnkey);
		std::vector<K> idx = series.Index();
		index.insert(idx.begin(), idx.end());

		this->Series<C, Series<K, T>>::withdata({ make_pair(columnkey, std::move(series)) });
		return *this;
	}

	template <typename C, typename K>
	template <class T>
	Series<K, T>& Frame<C, K>::getColAs(const C& col)
	{
		return this->Series<C, Series<K, T>>::operator[](col);
	}	

	
}

