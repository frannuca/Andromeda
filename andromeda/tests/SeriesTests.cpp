
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE SeriesTests
#include <cmath>
#include <boost/test/included/unit_test.hpp> // this work
#include "../src/qtime/QDate.h"
#include <utility>
#include "../src/data/Series.h"
#include <random>
using namespace std;
using namespace data;
using namespace qtime;
namespace tt = boost::test_tools;

typedef std::mt19937 MyRNG;

QDate t0(01, 01, 2001);

vector<pair<QDate,double>> GetSeriesData()
{
	uint32_t seed_val = 42;
	std::uniform_int_distribution<unsigned int> u_dist(0,100);
	MyRNG rng(42);

	srand(time(nullptr));
	vector<pair<QDate, double>> x;
	
	int nt = 1000;
	for (int i = 0; i < nt; ++i) x.push_back(make_pair(t0 + i, u_dist(rng)));

	return x;
}

BOOST_AUTO_TEST_CASE(SERIES)
{
	auto data = GetSeriesData();
	cout<<("Constructor tests")<<endl;	
	//checking constructors
	{
		//Testing Series ctor:
		Series<QDate, double> series1(data);
		Series<QDate, double> series2;
		series2.withdata(data);
		BOOST_CHECK_MESSAGE(series1 == series2, "Construction of series failed");

		Series<QDate, double> series3(data);
		Series<QDate, double> series4 = std::move(series3);
		
		BOOST_CHECK_MESSAGE(series4.empty(), "After moving operand the object is not empty");
		BOOST_CHECK_MESSAGE(series1 == series4, "Moved copy contructor did not copy data correctly");
				
	}
	
	cout<<("Operators tests (3 + series1)-series1 equals 3")<<endl;
	//operators sum, substraction
	{
		
		Series<QDate, double> series1(data);
		Series<QDate, double> series2 = 3.0 + series1;;
		Series<QDate, double> series3 = series2 - series1;
		for(auto x: series3)
		{
			BOOST_CHECK_EQUAL(x.second,3.0, "Operator scalar multiplication with substraction failed");
		}

	}

	std::cout<<("Operators tests (3*series1)/series1 equals 3")<<std::endl;
	//operators product, division
	{

		Series<QDate, double> series1(data);
		Series<QDate, double> series2 = 3.0 * series1;;
		Series<QDate, double> series3 = series2/series1;
		series3.fillNaNs(3.0);
		series3.fillInf(3.0);
		for (auto x : series3)
		{
			BOOST_CHECK_CLOSE_FRACTION(x.second, 3.0, 1e-12);
		}

	}

	std::cout<<("Operators tests (series1)**3/(series1*series1*series1) equals 1")<<std::endl;
	//operators power
	{

		Series<QDate, double> series1(data);
		Series<QDate, double> series2 = power(series1, 3.0);
		Series<QDate, double> series3 = series1 * series1 * series1;
		Series<QDate, double> series4 = series2 / series3;
		series4.fillNaNs(1.0);
		for (auto x : series4)
		{
			BOOST_CHECK_CLOSE(x.second, 1.0, 1e-12);
		}

	}
	//starting checks on access:
	//Series<QDate,double> 
}
