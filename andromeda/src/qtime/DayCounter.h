#pragma once
#include "libdef.h"
#include <string>
#include "QDate.h"
#include "Calendar.h"
#include <memory>

namespace qtime
{
	class EXPORT_SYMBOL DayCounter
	{		
	public:
		virtual ~DayCounter() = default;

		DayCounter();
		virtual std::string name() = 0;
		virtual double dayCount(const QDate& d1, const QDate& d2) const = 0;
		virtual double yearfraction(const QDate& d1, const QDate& d2) const = 0;
		virtual double daysperyear() const = 0;
	};

	class EXPORT_SYMBOL Actual360 :public DayCounter
	{
	private:
		bool _includeLastDay;
	public:
		Actual360() = delete;
		Actual360(bool includelastday);
		std::string name() override;
		double dayCount(const QDate& d1, const QDate& d2) const override;
		double yearfraction(const QDate& d1, const QDate& d2) const override;

		double daysperyear() const override;
	};

	class EXPORT_SYMBOL Actual365fixed :public DayCounter
	{
	
	public:
		Actual365fixed();
		std::string name() override;
		double dayCount(const QDate& d1, const QDate& d2) const override;
		double yearfraction(const QDate& d1, const QDate& d2) const override;

		double daysperyear() const override;
	};

	class EXPORT_SYMBOL Thirty360 :public DayCounter
	{

	public:
		enum class CONVENTION {USA=0, BONDBASIS, EUROPEAN, EUROBONDBASIS, ITALIAN, GERMAN};
		Thirty360() = delete;
		Thirty360(CONVENTION c, bool isLastPeriod);
		std::string name() override;
		double dayCount(const QDate& d1, const QDate& d2) const override;
		double yearfraction(const QDate& d1, const QDate& d2) const override;

		double daysperyear() const override;
	private:
		std::function<double(const QDate&, const QDate&)> daycount_functor;
		CONVENTION _convention;
		bool _isLastPeriod;
	};

	class EXPORT_SYMBOL SimpleDayCounter :public DayCounter
	{

	public:		
		SimpleDayCounter();		
		std::string name() override;
		double dayCount(const QDate& d1, const QDate& d2) const override;
		double yearfraction(const QDate& d1, const QDate& d2) const override;
		double daysperyear() const override;
	private:
		std::unique_ptr<DayCounter> fallback;
	};

	
}
