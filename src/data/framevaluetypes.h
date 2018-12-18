#pragma once
#include <utility>
#include "Series.h"
#include <algorithm>

namespace data
{
	using namespace std;
	template<typename C, typename K, typename T>
	class FrameValueTypes : public Series<C, Series<K, T>>
	{
		T tvalue;
	private:
		vector<K> getRowIndex()
		{
			set<K> indices;
			std::vector<C> cols = this->Series<C, Series<K, T>>::Index();
			for (auto c : cols)
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
}