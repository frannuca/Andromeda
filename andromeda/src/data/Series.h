#pragma once
#include <map>
#include <vector>
#include <functional>
#include <algorithm>
#include <memory>
#include "assertion.h"
#include <utility>
#include <functional>
#include <boost/optional.hpp>
#include "storage.h"
#include <set>
#include <algorithm>
#include <limits>
#include <math.h> 

namespace data
{
	using namespace std;
			
	template<typename K, typename V>
	class Series: Has_AritmeticOps<V>,  public Storage<K,V>
	{		
	public:
		
		using  TSeries = Series<K, V>;		
		using Storage<K, V>::Storage;
		using Storage<K, V>::operator=;
		using Storage<K, V>::operator[];	
			
		Series(Series&& s) noexcept;
		Series(const Series& s);
		void fillNaNs(const V& v);
		void dropNaNs();
		void fillInf(const V& v);
		void dropInf();

		template<typename K_, typename V_>
		friend Series<K_, V_> applyop(const Series<K_, V_>& lhs, const Series<K_, V_>& rhs, std::function<V(const V&, const V&)> op);

		template<typename K_, typename V_>
		friend Series<K_, V_> operator+(const Series<K_, V_>& lhs, const Series<K_, V_>& rhs);

		template<typename K_, typename V_>
		friend Series<K_, V_> operator+(const V_& l, const Series<K_, V_>& rhs);

		template<typename K_, typename V_>
		friend Series<K_, V_> operator-(const Series<K_, V_>& lhs, const Series<K_, V_>& rhs);

		template<typename K_, typename V_>
		friend Series<K_, V_> operator-(const Series<K_, V_>& lhs, const Series<K_, V_>& rhs);

		template<typename K_, typename V_>
		friend Series<K_, V_> operator-(const V_& l, const Series<K_, V_>& rhs);

		template<typename K_, typename V_>
		friend Series<K_, V_> operator*(const Series<K_, V_>& lhs, const Series<K_, V_>& rhs);

		template<typename K_, typename V_>
		friend Series<K_, V_> operator*(const V_& lhs, const Series<K_, V_>& rhs);

		template<typename K_, typename V_>
		friend Series<K_, V_> operator*(const Series<K_, V_>& lhs, const V_& rhs);

		template<typename K_, typename V_>
		friend Series<K_, V_> operator*(const V_& lhs, const Series<K_, V_>& rhs);

		template<typename K_, typename V_>
		friend Series<K_, V_> operator/(const Series<K_, V_>& lhs, const V_& rhs);

		
		template<typename K_, typename V_>
		friend Series<K_, V_> operator/(const Series<K_, V_>& lhs, const Series<K_, V_>& rhs);

		template<typename K_, typename V_>
		friend Series<K_, V_> operator-(const Series<K_, V_>& rhs);

		template<typename K_, typename V_>
		friend Series<K_, V_> power(const Series<K_, V_>& s,const double& x);

		
	};

	template <typename K, typename V>
	Series<K, V>::Series(Series&& s) noexcept	
	{	
		this->Storage<K, V>::datacontainer.reset(s.datacontainer.release());
	}

	template <typename K, typename V>
	Series<K, V>::Series(const Series& s):Storage<K,V>(*s.datacontainer)
	{}

	template <typename K, typename V>
	void Series<K, V>::fillNaNs(const V& v)
	{
		for(int i= datacontainer->size()-1; i >=0 ; --i)
		{
			if((*datacontainer)[i].second != (*datacontainer)[i].second)
			{
				(*datacontainer)[i].second = v;
			}
		}
	}

	template <typename K, typename V>
	void Series<K, V>::dropNaNs()
	{
		for (int i = datacontainer->size() - 1; i >= 0; --i)
		{
			if ((*datacontainer)[i].second != (*datacontainer)[i].second)
			{
				datacontainer->erase(datacontainer->begin() + i);
			}
		}
	}

	template <typename K, typename V>
	void Series<K, V>::fillInf(const V& v)
	{
		for (int i = datacontainer->size() - 1; i >= 0; --i)
		{
			if ((*datacontainer)[i].second == std::numeric_limits<V>::infinity())
			{
				(*datacontainer)[i].second = v;
			}
		}
	}

	template <typename K, typename V>
	void Series<K, V>::dropInf()
	{
		for (int i = datacontainer->size() - 1; i >= 0; --i)
		{
			if ((*datacontainer)[i].second == std::numeric_limits<V>::infinity())
			{
				datacontainer->erase(datacontainer->begin() + i);
			}
		}
	}

	template<typename K, typename V>
	Series<K, V> applyop(const Series<K, V>& lhs, const Series<K, V>& rhs, std::function<V(const V&,const V&)> op){
		std::vector<std::pair<K, V>> vdata;
		std::set<K> sindex1,sindex2;
		std::vector<K> index1 = lhs.Index();
		std::vector<K> index2 = rhs.Index();

		sindex1.insert(index1.begin(), index1.end());
		sindex2.insert(index2.begin(), index2.end());

		std::vector<K> intersection,difference;
		std::set_intersection(sindex1.begin(), sindex1.end(), sindex2.begin(), sindex2.end(), std::back_inserter(intersection));
		std::set_symmetric_difference(sindex1.begin(), sindex1.end(), sindex2.begin(), sindex2.end(), std::back_inserter(difference));
		for(auto n:intersection)
		{
			vdata.push_back(std::make_pair(n, op(lhs[n], rhs[n])));
		}

		for (auto n : difference)
		{
			vdata.push_back(std::make_pair(n, std::numeric_limits<V>::quiet_NaN()));
		}
		return Series<K, V>(vdata);
	}

	template<typename K, typename V>
	Series<K, V> applyop(const Series<K, V>& rhs, std::function<V(const V&)> op) {
		std::vector<K> index2 = rhs.Index();		
		std::vector<std::pair<K, V>> vdata;

		for (auto n : index2)
		{
			vdata.push_back(std::make_pair(n, op(rhs[n])));
		}	
		return Series<K, V>(vdata);
	}

	template<typename K_, typename V_>
	Series<K_, V_> operator+(const Series<K_, V_>& lhs, const Series<K_, V_>& rhs){
		return applyop<K_,V_>(lhs, rhs, [](const V_& a, const V_& b) {return a + b; });
	}

	template<typename K_, typename V_>
	Series<K_, V_> operator+(const V_& l, const Series<K_, V_>& rhs) {
		return applyop<K_, V_>(rhs, [l](const V_& a) {return a + l; });
	}

	template<typename K_, typename V_>
	Series<K_, V_> operator+(const Series<K_, V_>& rhs, const V_& l ) {
		return l + rhs;
	}

	template<typename K_, typename V_>
	Series<K_, V_> operator-(const Series<K_, V_>& lhs, const Series<K_, V_>& rhs) {
		return applyop<K_, V_>(lhs, rhs, [](const V_& a, const V_& b) {return a - b; });
	}

	template<typename K_, typename V_>
	Series<K_, V_> operator-(const V_& l, const Series<K_, V_>& rhs) {
		return applyop<K_, V_>(rhs, [l](const V_& a) {return l - a ; });
	}

	template<typename K_, typename V_>
	Series<K_, V_> operator-(const Series<K_, V_>& rhs, const V_& l) {
		return applyop<K_, V_>(rhs, [l](const V_& a) {return a - l; });
	}

	template<typename K_, typename V_>
	Series<K_, V_> operator-(const Series<K_, V_>& rhs) {
		return applyop<K_, V_>(rhs, [](const V_& a) {return -a; });
	}

	template<typename K_, typename V_>
	Series<K_, V_> operator*(const Series<K_, V_>& lhs, const Series<K_, V_>& rhs) {
		return applyop<K_, V_>(lhs, rhs, [](const V_& a, const V_& b) {return a * b; });
	}

	template<typename K_, typename V_>
	Series<K_, V_> operator*(const V_& l, const Series<K_, V_>& rhs) {
		return applyop<K_, V_>(rhs, [l](const V_& a) {return a * l; });
	}

	template<typename K_, typename V_>
	Series<K_, V_> operator*(const Series<K_, V_>& lhs, const V_& l) {
		return l * lhs;
	}
	
	template<typename K_, typename V_>
	Series<K_, V_> operator/(const Series<K_, V_>& lhs, const Series<K_, V_>& rhs) {
		return applyop<K_, V_>(lhs, rhs, [](const V_& a, const V_& b) {return a / b; });
	}

	template<typename K_, typename V_>
	Series<K_, V_> operator/(const V_& l, const Series<K_, V_>& rhs) {
		return applyop<K_, V_>(rhs, [l](const V_& a) {return l/a; });
	}

	template<typename K_, typename V_>
	Series<K_, V_> operator/(const Series<K_, V_>& lhs, const V_& l) {
		return applyop<K_, V_>(lhs, [l](const V_& a) {return  a/l; });
	}

	template<typename K_, typename V_>
	Series<K_, V_> power(const Series<K_, V_>& s, const double& x)
	{
		return applyop<K_, V_>(s, [x](const V_& a) {return (V_)std::pow(double(a),double(x)) ; });
	}

}
