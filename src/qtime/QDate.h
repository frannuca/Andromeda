#pragma once
#include "../inc/libdef.h"
#include <time.h>
#include <iostream>
#include "CalendarCommons.h"
namespace boost { namespace gregorian{ class date; } }

namespace qtime {
	
class EXPORT_SYMBOL QDate {
protected:
	boost::gregorian::date *_d;
public:
	//ctors
	QDate();
	QDate(int day, int month, int year);	
	QDate(int day, MONTH month, int year);
	explicit QDate(const long& unixtime);

	//copy
	QDate(const QDate& date);
	QDate(QDate&& date) noexcept;
	QDate& operator=(const QDate& other);
	QDate& operator=(QDate&& other) noexcept;
	WEEKDAY DayOfTheWeek() const;
	int DayOfTheMonth() const;
	int DayOfYear() const;
	bool IsWeekend() const;
	bool IsMonthEnd() const;
	MONTH Month() const;
	int Year() const;
	//dtor
	~QDate();

	std::string toString() const;
	QDate MonthEnd() const;
	QDate operator+(int days) const;
	QDate operator-(int days) const;
	int operator-(const QDate& that) const;
	bool operator>(const QDate& that) const;
	bool operator<(const QDate& that) const;
	bool operator<=(const QDate& that) const;
	bool operator>=(const QDate& that) const;

	bool operator==(const QDate& that) const;
	time_t toEpoch() const;

	static bool isLeapYear(int year);
	const static QDate ZERO;
	friend EXPORT_SYMBOL std::ostream&  operator<<(std::ostream& os, const QDate& dt);
	
};
    std::ostream&  operator<<(std::ostream& os, const QDate& dt);
	
}
