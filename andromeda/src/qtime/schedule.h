#pragma once
#include "libdef.h"
#include "QDate.h"
#include "tenor.h"
#include "dategeneration.h"
#include <vector>

namespace qtime
{
	class BusinessDayConvention;
	class EXPORT_SYMBOL Calendar;

	class Schedule
	{
	public:
		
		typedef std::vector<QDate>::const_iterator const_iterator;
		Schedule(QDate effectiveDate_, const QDate& terminationDate_, const Tenor<SMONTH> tenor_, const qtime::Calendar* calendar_, const BusinessDayConvention* convention_, const BusinessDayConvention* terminationDateConvention_, DateGeneration::Rule rule_);
		const_iterator begin() const;
		const_iterator end() const;	
		const std::vector<QDate>& Dates() const;
	private:
		mutable const_iterator piter;
		std::vector<QDate> dates_;
		QDate effectiveDate;
		QDate terminationDate;
		Tenor<SMONTH> tenor;
		const qtime::Calendar* calendar;
		const qtime::BusinessDayConvention* convention;
		const qtime::BusinessDayConvention* terminationDateConvention;
		DateGeneration::Rule rule;			
		void generaterawdates();		
	};
}
