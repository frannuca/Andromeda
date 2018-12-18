
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE SeriesTests
#include <cmath>
#include <boost/test/included/unit_test.hpp> // this work
#include "../src/qtime/QDate.h"
#include <utility>
#include "../src/data/Series.h"
#include <random>
#include "../src/data/Frame.h"
#include <chrono>
//#include "../src/data/Frame.h"

using namespace std;
using namespace data;
using namespace qtime;
namespace tt = boost::test_tools;

typedef std::mt19937 MyRNG;

QDate t0(01, 01, 2001);

vector<pair<QDate,double>> GetSeriesData(bool odd=false)
{
	uint32_t seed_val = 42;
	std::uniform_int_distribution<unsigned int> u_dist(0,100);
	MyRNG rng(42);

	srand(time(nullptr));
	vector<pair<QDate, double>> x;
	
	int nt = 100000;
	if(odd)
	{
		for (int i = 0; i < nt; ++i) if (i%2==0) x.push_back(make_pair(t0 + i, u_dist(rng)));
	}
	else
	{
		for (int i = 0; i < nt; ++i) x.push_back(make_pair(t0 + i, u_dist(rng)));
	}


	return x;
};

vector<pair<QDate, string>> GetSeriesStringData(bool odd = false)
{
	vector<pair<QDate, string>> x;

	int nt = 100;
	if (odd)
	{
		for (int i = 0; i < nt; ++i) if (i % 2 == 0) x.push_back(make_pair(t0 + i, to_string(i)));
	}
	else
	{
		for (int i = 0; i < nt; ++i) x.push_back(make_pair(t0 + i, to_string(i)));
	}

	return x;
};




BOOST_AUTO_TEST_CASE(SERIES)
{
	auto data = GetSeriesData();
	auto data2 = GetSeriesData(true);
	auto datastr = GetSeriesStringData();


	
	//cout<<("Constructor tests")<<endl;	
	//{
	//	chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

	//	Series<QDate, double> series1(data);

	//	chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
	//	auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

	//	cout<<"time=" << duration;

	//}
	//

	////checking constructors
	//{
	//	chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

	//	//Testing Series ctor:
	//	Series<QDate, double> series1(data);
	//	Series<QDate, double> series2;
	//	series2.withdata(data);
	//	Series<QDate, double> series2b = data;
	//	BOOST_CHECK_MESSAGE(series1 == series2, "Construction of series failed");
	//	BOOST_CHECK_MESSAGE(series1 == series2b, "Copy Construction of series failed");

	//	Series<QDate, double> series3(data);
	//	Series<QDate, double> series4 = std::move(series3);
	//	
	//	BOOST_CHECK_MESSAGE(series4.empty(), "After moving operand the object is not empty");
	//	BOOST_CHECK_MESSAGE(series1 == series4, "Moved copy contructor did not copy data correctly");
	//	
	//	chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
	//	auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

	//	cout << "time=" << duration;

	//}
	//
	//cout<<("Operators tests (3 + series1)-series1 equals 3")<<endl;
	////operators sum, substraction
	//{
	//	
	//	Series<QDate, double> series1(data);
	//	Series<QDate, double> series2 = 3.0 + series1;;
	//	Series<QDate, double> series3 = series2 - series1;
	//	for(auto x: series3)
	//	{
	//		BOOST_CHECK_EQUAL(x.second,3.0);
	//	}

	//}

	//std::cout<<("Operators tests (3*series1)/series1 equals 3")<<std::endl;
	////operators product, division
	//{

	//	Series<QDate, double> series1(data);
	//	Series<QDate, double> series2 = 3.0 * series1;;
	//	Series<QDate, double> series3 = series2/series1;
	//	series3.fillNaNs(3.0);
	//	series3.fillInf(3.0);
	//	for (auto x : series3)
	//	{
	//		BOOST_CHECK_CLOSE_FRACTION(x.second, 3.0, 1e-12);
	//	}

	//}

	//std::cout<<("Operators tests (series1)**3/(series1*series1*series1) equals 1")<<std::endl;
	////operators power
	//{

	//	Series<QDate, double> series1(data);
	//	Series<QDate, double> series2 = power(series1, 3.0);
	//	Series<QDate, double> series3 = series1 * series1 * series1;
	//	Series<QDate, double> series4 = series2 / series3;
	//	series4.fillNaNs(1.0);
	//	for (auto x : series4)
	//	{
	//		BOOST_CHECK_CLOSE(x.second, 1.0, 1e-12);
	//	}
	//}

	//std::cout << ("Operators tests -s + s equals 0") << std::endl;
	////operators negation
	//{

	//	Series<QDate, double> series1(data);		
	//	Series<QDate, double> series2 = -series1 + series1;		
	//	for (auto x : series2)
	//	{
	//		BOOST_CHECK_CLOSE(x.second, 0.0, 1e-12);
	//	}
	//}

	//std::cout << ("sortting on keys") << std::endl;	
	//{

	//	Series<QDate, double> series1(data);
	//	auto comparator_lt = [](const QDate& a, const QDate& b) {return a < b; };
	//	auto comparator_gt = [](const QDate& a, const QDate& b) {return a > b; };
	//	series1.sortIndex(comparator_lt);
	//	auto keys = series1.Index();
	//	for (int i=1;i<keys.size();++i)
	//	{
	//		BOOST_CHECK_LT(keys[i-1], keys[i]);
	//	}


	//	series1.sortIndex(comparator_gt);
	//	keys = series1.Index();
	//	for (int i = 1; i < keys.size(); ++i)
	//	{
	//		BOOST_CHECK_GT(keys[i - 1], keys[i]);
	//	}
	//}

	//std::cout << ("Subview tests") << std::endl;
	////subviews
	//{
	//	Series<QDate, double> series1(data);
	//	{
	//		double refx = series1[t0 + 20];
	//		auto view1_3 = series1.subview({ t0,t0 + 1,t0 + 2,t0 + 20 });
	//		view1_3[t0 + 20] = 20;
	//		
	//		BOOST_CHECK_EQUAL(series1[t0 + 20], 20);

	//		auto view1_3copy = series1.subviewCopy({ t0,t0 + 1,t0 + 2,t0 + 20 });
	//		view1_3[t0 + 20] = 20;
	//		BOOST_CHECK_NE(series1[t0 + 20], refx);
	//	}

	//	auto a0 = series1[t0];
	//	auto a1 = series1[t0+1];
	//	auto a2 = series1[t0 + 2];
	//	auto a20 = series1[t0 + 20];
	//	

	//}

	cout << "Frame ctor" << endl;
	{
		Frame<string, qtime::QDate,double,string> frame;
		Series<QDate, double> series1(data);
		Series<QDate, string> series2(datastr);
		
		frame.addColumn("series1", series1)
			 .addColumn("series2", series2);
		
		Series<qtime::QDate, double>& col1 = frame.getColAs<double>("series1");
		Series<qtime::QDate, string>& col2 = frame.getColAs<string>("series2");

		auto co1x2 =  2.0 * col1 ;
		co1x2.withdata({ std::make_pair(qtime::QDate(1,1,1900.0),1900) });
		frame.addColumn("new column",co1x2);
		auto rowkeys = frame.RowIndex();
		auto colkeys = frame.ColumnsIndex();
		//BOOST_CHECK_THROW(frame.getColAs<double>("series2"),string);
		
	}	
}
