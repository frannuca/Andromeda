#include "schedule.h"
#include "buinessdayconvention.h"
#include "Calendar.h"


qtime::Schedule::Schedule(QDate effectiveDate_, 
						  const QDate& terminationDate_, 
	                      const qtime::Tenor<qtime::SMONTH> tenor_, 
						  const qtime::Calendar* calendar_, 
	                      const BusinessDayConvention* convention_, 
	                      const BusinessDayConvention* terminationDateConvention_, 
	                      DateGeneration::Rule rule_)
	:effectiveDate(effectiveDate_), terminationDate(terminationDate_),tenor(tenor_), calendar(calendar_),convention(convention_),terminationDateConvention(terminationDateConvention_),rule(rule_)
{
	generaterawdates();
}

qtime::Schedule::const_iterator qtime::Schedule::begin() const
{ 
	return dates_.begin(); 
}

qtime::Schedule::const_iterator qtime::Schedule::end() const
{
	return dates_.end();
}

const std::vector<qtime::QDate>& qtime::Schedule::Dates() const
{
	return dates_;
}

void qtime::Schedule::generaterawdates()
{
	switch (rule)
	{
		case DateGeneration::Backward:
		{
			QDate x = terminationDate;
			dates_.push_back(terminationDateConvention->adapt(terminationDate));
			x = x - tenor;
			while (x >= effectiveDate)
			{
				dates_.push_back(convention->adapt(x));
				x = x - tenor;				
			}
			std::reverse(dates_.begin(), dates_.end());
			break;
		}
		case DateGeneration::Forward:
		{
			QDate x = effectiveDate;
			dates_.push_back(convention->adapt(x));			
			x = x + tenor;
			while (x <= terminationDate)
			{
				dates_.push_back(convention->adapt(x));			
				x = x + tenor;				
			}
			
			break;
		}
		case DateGeneration::Zero:
		{
			dates_.push_back(convention->adapt(effectiveDate));
			dates_.push_back(terminationDateConvention->adapt(terminationDate));
			break;
		}
		case DateGeneration::EndOfMonth:
		{
			QDate x = effectiveDate;
			dates_.push_back(convention->adapt(x));
			x = x + tenor;
			x = x.MonthEnd();
			while (x < terminationDate)
			{
				dates_.push_back(convention->adapt(x));
				x = x + tenor;
				x = x.MonthEnd();
			}
			dates_.push_back(terminationDateConvention->adapt(terminationDate));
			break;
		}		
	}	
	for (auto dd : dates_) std::cout << dd << std::endl;
}

