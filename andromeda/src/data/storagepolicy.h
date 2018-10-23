#pragma once
#include "libdef.h"
#include <memory>
#include <boost/random/sobol.hpp>
#include <armadillo_bits/typedef_mat_fixed.hpp>

namespace data
{
	template<typename T>
	class Storage
	{
	protected:
		ContainerT pdata_;

	public:
		Storage() = delete;
		Storage(const Storage&);
		Storage(Storage&& storage) noexcept(true); //to guarantee no copy_if_fail is triggered			
		explicit Storage(std::unique_ptr<T> pdata) noexcept(true); //to guarantee no copy_if_fail is triggered

		double& operator()(const int& i, const int& j);
	};

	template <typename T>
	Storage<T>::Storage(const Storage& s)
	{
		
		pdata_.reset(new T(*s.pdata_.get()));
	}

	template <typename T>
	Storage<T>::Storage(Storage&& storage) noexcept(true)
	{
		pdata_.reset(storage.pdata_.release());		
	}

	template <typename T>
	Storage<T>::Storage(std::unique_ptr<T> pdat) noexcept(true)
	{
		pdata_ = pdat.release();		
	}

	template <typename T>
	double& Storage<T>::operator()(const int& i, const int& j)
	{
		return pdata_.operator()(i, j);
	}
}
