#pragma once
#include <memory>
#include <vector>
#include <boost/optional.hpp>
#include <sstream>
#include <initializer_list>
#include <iterator>
#include <vector>
#include <set>

template<typename K, typename V>
class Storage
{
public:
	using TPAIR = std::pair<K, V>;
	using TVEC = std::vector<TPAIR>;
	using ItemIterator = typename TVEC::iterator;

protected:
	std::unique_ptr<TVEC> datacontainer;
	boost::optional<std::function<bool(const K&, const K&)>> fcomparer;

private:
	std::pair<int,int>  binary_search(const K& key) const;
	std::pair<int, int> linear_search(const K& key) const;


public:
	
	Storage();	
	Storage(Storage&& that) noexcept;
	Storage(const Storage& that);
	Storage(const TVEC& l);

	//members
	ItemIterator begin() const { return datacontainer->begin(); }
	ItemIterator end() const { return datacontainer->end(); }

	Storage& withdata(std::initializer_list<TPAIR> l);
	Storage& withdata(const TVEC& l);
	void sortIndex(std::function<bool(const K&, const K&)> fcomp);

	std::pair<int, int> locate(const K& key) const;

	std::vector<K> Index() const;
	std::vector<V> Values() const;

	bool hasIndex(const K& n) const;
	void operator=(const Storage<K, V>& rhs);
	void operator=(Storage<K, V>&& rhs) noexcept;
	V& operator[](const K& k) const;

	bool operator==(const Storage& rhs) const;
};

template <typename K, typename V>
std::pair<int,int> Storage<K, V>::binary_search(const K& key) const
{
	int l = 0, h = datacontainer->size()-1;
	int m = (h + l)*0.5;
	int maxiter = h + 1;
	int counter = 0;
	while(l<h && ++counter<maxiter)
	{		
		auto xl = (*datacontainer)[l].first;
		auto xm = (*datacontainer)[m].first;
		auto xh = (*datacontainer)[h].first;
		if(xm==key)
		{
			l = m;
			h = m;
		}
		else if((*fcomparer)(key,xm))
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
std::pair<int, int> Storage<K, V>::linear_search(const K& key) const
{
	typename TVEC::iterator piter = std::find_if(datacontainer->begin(), datacontainer->end(), [&key](auto x) {return x.first == key; });
	if(piter == datacontainer->end())
	{
		std::ostringstream msg;
		msg << "Key " << key << " not present in the index";
		throw msg.str();
	}
	int n = std::distance(datacontainer->begin(), piter);
	return std::make_pair(n, n);
}

template <typename K, typename V>
Storage<K, V>::Storage(): datacontainer(new std::vector<std::pair<K, V>>())
{
}

template <typename K, typename V>
Storage<K, V>::Storage(Storage&& that) noexcept
{
	datacontainer.reset(that.datacontainer.release());
}

template <typename K, typename V>
Storage<K, V>::Storage(const Storage& that)
{
	datacontainer.reset(new TVEC(*that.datacontainer));
}

template <typename K, typename V>
Storage<K, V>::Storage(const TVEC& l):datacontainer(new TVEC())
{
	datacontainer->insert(datacontainer->end(), l.begin(), l.end());	
}

template <typename K, typename V>
Storage<K, V>& Storage<K, V>::withdata(std::initializer_list<TPAIR> l)
{
	datacontainer->insert(datacontainer->end(), l.begin(), l.end());
	return *this;
}

template <typename K, typename V>
Storage<K, V>& Storage<K, V>::withdata(const TVEC& l)
{
	datacontainer->insert(datacontainer->end(), l.begin(), l.end());
	return *this;
}

template <typename K, typename V>
void Storage<K, V>::sortIndex(std::function<bool(const K&, const K&)> fcomp)
{
	std::sort(datacontainer->begin(), datacontainer->end(), [fcomp](const TPAIR& lhs, const TPAIR& rhs) {return fcomp(lhs.first, rhs.first); });
	fcomparer = fcomp;
}

template <typename K, typename V>
std::pair<int, int> Storage<K, V>::locate(const K& key) const
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

template <typename K, typename V>
std::vector<K> Storage<K, V>::Index() const
{
	std::vector<K> keys;
	std::transform(begin(), end(), std::back_inserter(keys), [](const TPAIR& x) {return x.first; });
	return keys;
}

template <typename K, typename V>
std::vector<V> Storage<K, V>::Values() const
{
	std::vector<V> values;
	std::transform(begin(), end(), std::back_inserter(values), [](const TPAIR& x) {return x.second; });
	return values;
}

template <typename K, typename V>
bool Storage<K, V>::hasIndex(const K& n) const
{
	auto indices = this->Index();
	return std::find(indices.begin(), indices.end(), n) != indices.end();
}

template <typename K, typename V>
void Storage<K, V>::operator=(const Storage<K, V>& rhs)
{
	datacontainer.reset(new TVEC(*rhs.datacontainer));	
}

template <typename K, typename V>
void Storage<K, V>::operator=(Storage<K, V>&& rhs) noexcept
{
	datacontainer.reset(new TVEC(*rhs.datacontainer.release()));
}

template <typename K, typename V>
V& Storage<K, V>::operator[](const K& k) const
{
	int l, h;
	std::tie(l, h) = locate(k);
	if(l==h)
	{
		return (*datacontainer)[l].second;
	}
	else
	{
		throw "not implemented";
	}
}

template <typename K, typename V>
bool Storage<K, V>::operator==(const Storage& rhs) const
{
	if(this == &rhs) return true;
	
	if (!datacontainer && !rhs.datacontainer)	return true;
	
	if (!datacontainer || !(rhs.datacontainer)) return false;

	if(datacontainer->size() != datacontainer->size()) return false;
	auto index = Index();
	for(auto k:index)
	{
		if(!rhs.hasIndex(k)) return false;
		if(this->operator[](k) != rhs[k]) return false;
	}

	return true;
}


