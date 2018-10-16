#pragma once
#include "libdef.h"
#include <vector>
#include <boost/optional.hpp>
#include <armadillo>
#include <numeric>
#include "assertion.h"
#include <set>
#include <limits>
#include <random>

namespace data
{

	template<typename TROWS, typename TCOLS>
	class EXPORT_SYMBOL Frame
	{
	private:
		mutable std::unique_ptr<arma::mat> data_;
		std::vector<TROWS> index_;
		std::vector<TCOLS> columns_;

		template<typename K>
		std::pair<int, int> getIndexBoundaries(const TROWS& item, const std::vector<K>& items) const;
		Frame applyOps(std::function<double(double, double)> op, const Frame<TROWS, TCOLS>& that);
		bool hasIndex(const TROWS& index) const;
	public:					
		typedef TROWS value_row_type;
		typedef TCOLS value_cols_type;
		Frame(const size_t& nindex, const size_t& ncolumns);		
		Frame(const size_t& nindex, const std::vector<TCOLS>& columns);		
		Frame(const std::vector<TROWS>& index, const std::vector<TCOLS>& columns);	
		Frame(Frame<TROWS, TCOLS>&& rhs) noexcept;
		
		Frame& operator=(Frame<TROWS, TCOLS>&& rhs) noexcept;		
		Frame& operator=(const Frame<TROWS, TCOLS>& rhs) noexcept;
		
		size_t number_rows() const;
		size_t number_cols() const;		
		

	};

	/*****************************************************************/
	/******************Scalar multiplicattion*************************/
	template<typename TROWS,typename TCOLS>
	Frame<TROWS,TCOLS> operator*(const double& l, const Frame<TROWS,TCOLS>& frame){
		Frame<TROWS, TCOLS> outframe(frame.index_, frame.columns_);
		outframe.data_.reset(new arma::mat((*frame.data_)*l));
		return std::move(frame);
	}
	
	template<typename TROWS, typename TCOLS>
	Frame<TROWS, TCOLS> operator*(const Frame<TROWS, TCOLS>& frame, const double& l) {
		Frame<TROWS, TCOLS> outframe(frame.index_, frame.columns_);
		outframe.data_.reset(new arma::mat((*frame.data_)*l));
		return std::move(frame);
	}

	template<typename TROWS, typename TCOLS>
	Frame<TROWS, TCOLS> operator/(const double& l, const Frame<TROWS, TCOLS>& frame) {
		Frame<TROWS, TCOLS> outframe(frame.index_, frame.columns_);
		outframe.data_.reset(new arma::mat(l/(*frame.data_)));
		return std::move(frame);
	}

	template<typename TROWS, typename TCOLS>
	Frame<TROWS, TCOLS> operator/(const Frame<TROWS, TCOLS>& frame, const double& l) {
		Frame<TROWS, TCOLS> outframe(frame.index_, frame.columns_);
		outframe.data_.reset(new arma::mat((*frame.data_)/l));
		return std::move(frame);
	}

	/****************************************************************/
	/***************************IMPLEMENTATION***********************/
	/****************************************************************/
		
	template <typename TROWS, typename TCOLS>
	bool Frame<TROWS, TCOLS>::hasIndex(const TROWS& index) const
	{
		int l, u;
		std::tie(l, u) = getIndexBoundaries(index, index_);
		return l == u && index_[l] == index;
	}


	template <typename TROWS, typename TCOLS>
	template <typename K>
	std::pair<int, int> Frame<TROWS, TCOLS>::getIndexBoundaries(const TROWS& item, const std::vector<K>& items) const
	{
		int l = 0, u = items.size();
		int m = 0.5*(l + u);
		int maxiter = u;
		int counter = 0;
		while (++counter < maxiter)
		{
			const TROWS& x = items[m];
			if (x == item)
			{
				return std::make_pair(m, m);
			}
			else if (x < item)
			{
				u = m;
			}
			else
			{
				l = m;
			}
		}

		return std::make_pair(l, m);
	}

	template <typename TROWS, typename TCOLS>
	Frame<TROWS, TCOLS> Frame<TROWS, TCOLS>::applyOps(std::function<double(double, double)> op,  const Frame<TROWS, TCOLS>& that)
	{
		auto indexthat = that.index_;
		std::set<TROWS> indexsum;

		for (auto n : index_) indexsum.insert(n);
		for (auto n : that.index_) indexsum.insert(n);

		std::vector<TROWS> newindex;
		for (auto item : indexsum) newindex.push_back(item);
		Frame<TROWS, TCOLS> frame(newindex, columns_);

		for (const TROWS& n : indexsum)
		{
			bool thishasindex = hasIndex(n);
			bool thathasinde = that.hasIndex(n);
			for(const TCOLS& col: columns_)
			{
				if (thishasindex && thathasinde)
				{
					frame(n,col) = op(this->operator()(n,col), that(n,col));
				}
				else if (thishasindex)
				{
					frame[n] = this->operator()(n,col);
				}
				else if (thathasinde)
				{
					frame[n] = that(n,col);
				}
				else
				{
					frame[n] = arma::rowvec(std::vector<double>(frame.number_cols(), std::numeric_limits<double>::quiet_NaN()));
				}
			}
			
		}
		return frame;
	}

	template <typename TROWS, typename TCOLS>
	Frame<TROWS, TCOLS>::Frame(const size_t& nindex, const size_t& ncolumns)
	{
		std::vector<int> v(nindex);
		std::iota(v.begin(), v.end(), 0);
		index_ = std::move(v);

		std::vector<int> w(ncolumns);
		std::iota(w.begin(), w.end(), 0);
		columns_ = std::move(w);

		data_.reset(new arma::mat(index_.size(), columns_.size()));
	}

	template <typename TROWS, typename TCOLS>
	Frame<TROWS, TCOLS>::Frame(const size_t& nindex, const std::vector<TCOLS>& columns)
	{
		std::vector<int> v(nindex);
		std::iota(v.begin(), v.end(), 0);
		index_ = std::move(v);
		columns_ = columns;
		std::sort(columns_.begin(), columns_.end());
		data_.reset(new arma::mat(index_.size(), columns_.size()));
	}

	template <typename TROWS, typename TCOLS>
	Frame<TROWS, TCOLS>::Frame(const std::vector<TROWS>& index, const std::vector<TCOLS>& columns)
	{
		index_ = index;
		columns_ = columns;
		std::sort(columns_.begin(), columns_.end());
		std::sort(index_.begin(), index_.end());

		data_.reset(new arma::mat(index_.size(), columns_.size()));
	}

	template <typename TROWS, typename TCOLS>
	Frame<TROWS, TCOLS>::Frame(Frame<TROWS, TCOLS>&& rhs) noexcept
	{
		data_ = rhs.data_;
		rhs.data_.reset();
		index_ = std::move(rhs.index_);
		columns_ = std::move(rhs.columns_);
	}

	template <typename TROWS, typename TCOLS>
	Frame<TROWS, TCOLS>& Frame<TROWS, TCOLS>::operator=(Frame<TROWS, TCOLS>&& rhs) noexcept
	{
		data_ = rhs.data_;
		rhs.data_.reset();
		index_ = std::move(rhs.index_);
		columns_ = std::move(rhs.columns_);
		return *this;
	}

	template <typename TROWS, typename TCOLS>
	Frame<TROWS, TCOLS>& Frame<TROWS, TCOLS>::operator=(const Frame<TROWS, TCOLS>& rhs) noexcept
	{
		data_.reset(new arma::mat(*rhs.data_));
		rhs.data_.reset();
		index_ = std::move(rhs.index_);
		columns_ = std::move(rhs.columns_);
		return *this;
	}

	template <typename TROWS, typename TCOLS>
	size_t Frame<TROWS, TCOLS>::number_rows() const
	{
		return index_.size();
	}

	template <typename TROWS, typename TCOLS>
	size_t Frame<TROWS, TCOLS>::number_cols() const
	{
		return columns_.size();
	}

}
