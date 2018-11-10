#pragma once

#include <utility>
#include "Series.h"
#include <algorithm>

namespace data
{
	using namespace std;


	template<typename C, typename K, typename V>
	class Frame:public Series<C,Series<K,V>>
	{	
		boost::optional<std::function<bool(const K&, const K&)>> rowcomparator_;
	public:
		using TCOLC = C;
		using TROWS = K;
		using TSERIES = Series<K, V>;

		
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
		void FillSparseWith(const V& v);
		void DropSparseRows();
		std::vector<K> RowIndex();
		void sortRowsWith(std::function<bool(const K&, const K&)> fcomp);
	};

	template <typename C, typename K, typename V>
	Frame<C, K, V>::~Frame()
	{
	}

	template <typename C, typename K, typename V>
	Frame<C, K, V>& Frame<C, K, V>::withSeries(const C& columnkey, Series<K, V>& series)
	{
		if (rowcomparator_)			
			series.sortIndex(*rowcomparator_);

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
	
	template <typename C, typename K, typename V>
	void Frame<C, K, V>::FillSparseWith(const V& v)
	{		
		std::vector<K> vrowindex = this->RowIndex();
		std::set<K> rowindex(vrowindex.begin(), vrowindex.end());
		
		for(auto col:this->Index())
		{
			Series<K,V>& series = (*this)[col];
			auto vind = series.Index();
			std::set<K> keys(vind.begin(),vind.end());
			set<K> diff;
			std::set_difference(rowindex.begin(), rowindex.end(), keys.begin(), keys.end(), std::inserter(diff, diff.begin()));
			std::vector<std::pair<K, V>> additions;
			for(K k:diff)
			{
				additions.push_back(std::make_pair(k, v));				
			}
			series.withdata(additions);
			/*series.sortIndex();*/
		}
	}

	template <typename C, typename K, typename V>
	void Frame<C, K, V>::DropSparseRows()
	{
		std::set<K> index;
		for(std::pair<C,Series<K,V>>& d:*this)
		{
			std::vector<K> x = d.second.Index();
			index.insert(x.begin(), x.end());
		}
		std::vector<K> index2drop;
		for(const K& k:index)
		{
			bool hasindex = true;
			for (const std::pair<C,Series<K,V>>& ser : *this)
			{
				hasindex &= ser.second.hasIndex(k);
				if (!hasindex) break;
			}
			if(!hasindex)
			{
				index2drop.push_back(k);
			}
		}

		for (std::pair<C, Series<K, V>>& d : *this)
		{
			d.second.dropItems(index2drop);
		}
	}

	template <typename C, typename K, typename V>
	std::vector<K> Frame<C, K, V>::RowIndex()
	{
		if (!rowcomparator_)
		{
			rowcomparator_ = [](const K& a, const K&b) {return a < b; };
		}
						
		std::set<K> index;
		for (std::pair<C, Series<K, V>>& x : *this)
		{
			std::vector<K> ind = x.second.Index();
			index.insert(ind.begin(), ind.end());
		}
		std::vector<K> vout;
		std::transform(index.begin(), index.end(), std::back_inserter(vout), [](const K& x) {return x; });
		std::sort(vout.begin(), vout.end(), *rowcomparator_);
		return vout;		
	}

	template <typename C, typename K, typename V>
	void Frame<C, K, V>::sortRowsWith(std::function<bool(const K&, const K&)> fcomp)
	{
		rowcomparator_ = fcomp;
		for(std::pair<C,Series<K,V>>& x:*this)
		{
			x.second.sortIndex(*rowcomparator_);
		}
	}
}

