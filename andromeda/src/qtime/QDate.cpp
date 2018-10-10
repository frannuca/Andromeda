#include "QDate.h"

#include <boost/date_time/gregorian/gregorian_types.hpp> 
#include <ctime>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include "boost/date_time/local_time/local_time.hpp"

namespace pt = boost::posix_time;
namespace g = boost::gregorian;

qtime::QDate::QDate()
{
	_d = new boost::gregorian::date(1900, 01, 01);
}

qtime::QDate::QDate(int day, int month, int year) {
	_d = new boost::gregorian::date(year, month, day);	
}

qtime::QDate::QDate(int day, MONTH month, int year)
{
	_d = new boost::gregorian::date(year, int(month), day);
}

qtime::QDate::QDate(const QDate& that) {
	_d = new boost::gregorian::date(*that._d);
}

qtime::QDate::QDate(QDate&& that) noexcept
{
	if (this != &that)
	{
		_d = that._d;		
		that._d = nullptr;
	}	
}

qtime::QDate::QDate(const long &btime) {
	_d = new boost::gregorian::date(boost::posix_time::from_time_t(btime).date());
}

qtime::QDate::~QDate() {
	delete _d;
}

qtime::QDate qtime::QDate::MonthEnd() const
{
	auto d = _d->end_of_month();
	return QDate(d.day(),d.month(),d.year());
}

qtime::QDate& qtime::QDate::operator=(const qtime::QDate& other) {
	if (this != &other) {
		delete _d;
		_d = new boost::gregorian::date(*other._d);
	}
	return *this;
}

qtime::QDate& qtime::QDate::operator=(qtime::QDate&& other) noexcept
{
	if (this != &other) {
		delete _d;		
		_d = std::exchange(other._d, nullptr);
	}
	return *this;
}

qtime::QDate qtime::QDate::operator+(int ndays) const {
	auto date = *_d + boost::gregorian::days(ndays);
	QDate d(date.day(), date.month(), date.year());
	return d;
}
qtime::WEEKDAY qtime::QDate::DayOfTheWeek() const{	
	auto iday = _d->day_of_week();
	auto wd = boost::lexical_cast<std::string>(_d->day_of_week());
	return qtime::WEEKDAY((int)iday);			
}

int qtime::QDate::DayOfTheMonth() const
{
	return _d->day();
}

int qtime::QDate::DayOfYear() const
{
	return _d->day_of_year();
}

bool qtime::QDate::IsWeekend() const
{
	auto dw = DayOfTheWeek();
	return dw == WEEKDAY::SATURDAY || dw == WEEKDAY::SUNDAY;
}

bool qtime::QDate::IsMonthEnd() const
{
	auto de = boost::gregorian::gregorian_calendar::end_of_month_day(_d->year(), _d->month());
	auto td = _d->day();
	return td == de;
}

qtime::MONTH qtime::QDate::Month() const
{	
	auto m = _d->month().as_enum();	
	return MONTH(m);	
}

int qtime::QDate::Year() const
{
	return _d->year();
}

qtime::QDate qtime::QDate::operator-(int ndays) const
{
	auto date = *_d - boost::gregorian::days(ndays);
	QDate d(date.day(), date.month(), date.year());
	return d;
}

int qtime::QDate::operator-(const QDate& that) const
{
	return (*_d - *(that._d)).days();
}

bool qtime::QDate::operator>(const QDate& that) const
{
	return (*_d) > *(that._d);
}

bool qtime::QDate::operator<(const QDate& that) const
{
	return (*_d) < *(that._d);
}

bool qtime::QDate::operator<=(const QDate& that) const
{
	return (*_d) <= *(that._d);
}

bool qtime::QDate::operator>=(const QDate& that) const
{
	return (*_d) >= *(that._d);
}

bool qtime::QDate::operator==(const QDate& that) const
{
	return *_d == *(that._d);
}


time_t qtime::QDate::toEpoch() const
{
	using namespace boost::posix_time;
	static ptime epoch(boost::gregorian::date(1970, 1, 1));
	time_duration::sec_type secs = (ptime(*_d, seconds(0)) - epoch).total_seconds();
	return time_t(secs);
}

bool qtime::QDate::isLeapYear(int year)
{
	bool cond1 = year % 4 == 0 && year % 100 == 0 && year % 400;	
	bool cond2 = year % 4 == 0 && year % 100 != 0;

	return cond1 || cond2;
}

const std::locale fmt(std::locale::classic(),
	new boost::gregorian::date_facet("%m/%d/%Y"));

std::string dateAsMMDDYYYY(const boost::gregorian::date& date)
{
	std::ostringstream os;
	os.imbue(fmt);
	os << date;
	return os.str();
}
std::ostream&  qtime::operator<<(std::ostream& os, const qtime::QDate& dt) {
	os.imbue(fmt);
	os << *(dt._d);
	return os;
}

const qtime::QDate qtime::QDate::ZERO = QDate(01, 01, 1900);