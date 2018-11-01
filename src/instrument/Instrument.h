#pragma once
#include "../inc/libdef.h"
#include "../qtime/QDate.h"
#include "../qtime/DayCounter.h"
#include <boost/optional.hpp>
#include "../qtime/tenor.h"

namespace instrument
{
	class EXPORT_SYMBOL Instrument
	{
	public:
		explicit Instrument(qtime::QDate maturity);
		enum  class INSTRUMENTCLASS {BOND=0,OPTION,FUTURE,FORWARD,SWAP,CASHFLOW,ANNUITY,xIBOR};
		boost::optional<qtime::QDate> startdate;
		const qtime::QDate maturity;
		boost::optional<std::string>  currency;
		std::unique_ptr<qtime::DayCounter> dc_;
		virtual ~Instrument();
		virtual INSTRUMENTCLASS getclass() = 0;
	};

	class EXPORT_SYMBOL CashFlow: public Instrument
	{
	public:
		CashFlow(const qtime::QDate& maturity,const qtime::Tenor<qtime::SMONTH>& tenor, double nominal);		
		const qtime::Tenor<qtime::SMONTH> tenor;
		const double nominal;
		~CashFlow() override;

		INSTRUMENTCLASS getclass() override;
	};

	class EXPORT_SYMBOL Swap:public Instrument
	{
	public:
		Swap(const qtime::QDate& maturity,const double& c, const CashFlow& floating_leg, const CashFlow& fixed_leg);

		const double C;
		const CashFlow* floating_leg;
		const CashFlow* fixed_leg;
		~Swap() override;
		INSTRUMENTCLASS getclass() override;
	};

	class EXPORT_SYMBOL xIbor :public Instrument
	{
	public:
		const double C;
		qtime::Tenor<qtime::SMONTH> tenor;

		xIbor(const qtime::QDate& maturity,const double &c, const qtime::Tenor<qtime::SMONTH>& tenor);
		~xIbor() override;
		INSTRUMENTCLASS getclass() override;
	};
}
