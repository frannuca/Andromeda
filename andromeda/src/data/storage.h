#pragma once
#include <memory>
#include <vector>
#include <boost/optional.hpp>
#include <sstream>
#include <initializer_list>

template<typename K, typename V>
class Storage
{
public:
	using TPAIR = std::pair<K, V>;
	using TVEC = std::vector<TPAIR>;
	using ItemIterator = typename TVEC::iterator;

protected:
	std::unique_ptr<TVEC> data;
	boost::optional<std::function<bool(const V&, const V&)>> fcomparer;

private:
	std::pair<int,int>  binary_search(const K& key);
	std::pair<int, int> linear_search(const K& key);


public:
	
	Storage();	
	Storage(Storage&& that) noexcept;
	Storage(const Storage& that);

	//members
	ItemIterator begin() const { return data->begin(); }
	ItemIterator end() const { return data->end(); }

	Storage& withdata(std::initializer_list<TPAIR> l);
	void sortIndex(std::function<bool(const K&, const K&)> fcomp);

	std::pair<int, int> locate(const K& key);
	
};

template <typename K, typename V>
std::pair<int,int> Storage<K, V>::binary_search(const K& key)
{
	int l = 0, h = data->size()-1;
	int m = (h + l)*0.5;
	int maxiter = h + 1;
	int counter = 0;
	while(l<h && ++counter<maxiter)
	{		
		auto xl = (*data)[l].first();
		auto xm = (*data)[m].first();
		auto xh = (*data)[h].first();
		if(xm==key)
		{
			l = m;
			h = m;
		}
		else if(fcomparer(key,xm))
		{
			h=m;
		}
		else
		{
			l = m;
		}
	}

	return std::make_pair(l,h);
}

template <typename K, typename V>
std::pair<int, int> Storage<K, V>::linear_search(const K& key)
{
	typename TVEC::const_iterator piter = std::find_if(data->begin(), data->end(), [&key](auto x) {return x.first == key; });
	if(piter == data->end())
	{
		std::ostringstream msg;
		msg << "Key " << key << " not present in the index";
		throw msg.str();
	}
	int n = std::distance(data->begin(), piter);
	return std::make_pair(n, n);
}

template <typename K, typename V>
Storage<K, V>::Storage(): data(new std::vector<std::pair<K, V>>())
{
}

template <typename K, typename V>
Storage<K, V>::Storage(Storage&& that) noexcept
{
	data.reset(that.data.release());
}

template <typename K, typename V>
Storage<K, V>::Storage(const Storage& that)
{
	data.release(new TVEC(*that.data));
}

template <typename K, typename V>
Storage<K, V>& Storage<K, V>::withdata(std::initializer_list<TPAIR> l)
{
	data->insert(data->end(), l.begin(), l.end());
	return *this;
}

template <typename K, typename V>
void Storage<K, V>::sortIndex(std::function<bool(const K&, const K&)> fcomp)
{
	std::sort(data->begin(), data->end(), [fcomp](const TPAIR& lhs, const TPAIR& rhs) {return fcomp(lhs.first, rhs.first); });
	fcomparer = fcomp;
}

template <typename K, typename V>
std::pair<int, int> Storage<K, V>::locate(const K& key)
{
	int l, h;
	if(fcomparer)
	{
		std::tie(l, h) = binary_search(key);
	}
	else
	{
		std::tie(l, h) = linear_search(key);
	}

	return std::make_pair(l, h);
}


