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
		vector<K> getRowIndex(bool applyunion=true)
		{
			set<K> indices;
			std::vector<C> cols = this->Series<C, Series<K, T>>::Index();
			for (auto c : cols)
			{
				vector<K>& keys = (this->Series<C, Series<K, T>>::operator[](c)).Index();
				if(applyunion)
				{
					indices.insert(keys.begin(), keys.end());
				}
				else
				{
					if (indices.empty()) copy(keys.begin(), keys.end(), inserter(indices, indices.begin()));
					else
					{
						set<K> aux;
						set_intersection(keys.begin(), keys.end(), indices.begin(), indices.end(), inserter(aux, aux.begin()));
						indices = std::move(aux);
					}
				}
				
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

		void RowKeys(map<string, vector<K>>& indices,bool applyunion=true)
		{
			indices[typeid(tvalue).name()] = getRowIndex(applyunion);
		}

		void ColumnKeys(map<string, vector<C>>& indices)
		{
			indices[typeid(tvalue).name()] = getColumnIndex();
		}

	};
}