#include <iostream>
#include <armadillo>
#include "../src/data/Series.h"
#include "../src/qtime/QDate.h"
#include <string>
#include <utility>
using namespace std;
struct ByLength 
{
	bool operator()(const string& lhs, const string& rhs) const
	{
		return lhs.length() > rhs.length();
	}
};


int main(int argc, const char **argv) {

	std::vector<std::pair<qtime::QDate, double>> points;
	
	points.push_back(std::make_pair(qtime::QDate(01, 02, 2018), 2.0));
	points.push_back(std::make_pair(qtime::QDate(01, 03, 2018), 3.0));
	points.push_back(std::make_pair(qtime::QDate(01, 01, 2018), 1.0));
	data::Series<qtime::QDate, double> series;
	series.withdata(points);

	auto keys = series.Index();
	auto values = series.Values();
	data::Series<qtime::QDate, double> series2 = series;
	data::Series<qtime::QDate, double> series3 = std::move(series2);

	series3.sortIndex([](auto a, auto b) {return a < b; });
	auto index2 = series3.Index();
	auto values2 = series3.Values();

	auto x1 = series3[qtime::QDate(01, 02, 2018)];
	series3[qtime::QDate(01, 02, 2018)] = 99;
	auto x2 = series3[qtime::QDate(01, 02, 2018)];
	auto s1 = series3;
	auto s2 = series3;
	auto s12 = s1 + s2;
	auto p12 = s1 * s2;
	auto p_3xs1 = 3.0 * s1;
	auto p_s1x3 = s1 * 3.0;
	auto p_1divs1 = 1.0 / s1;
	auto p_s1di3 = s1/3.0;
	auto spow2 = power(s1, 2.0);

	data::Series<qtime::QDate, double> scheck1 = spow2;
	auto equal = scheck1 == spow2;
	scheck1[qtime::QDate(01, 02, 2018)] = 881;
	auto equal2 = scheck1 == spow2;

	scheck1.fillNaNs(0.0);
	spow2[qtime::QDate(01, 02, 2018)] = std::numeric_limits<double>::quiet_NaN();
	spow2.dropNaNs();
	//datacontainer::Series<std::string, double, ByLength> movedseries(std::move(series));
	/*auto a = frame(0, 0);
	auto frame_x2 = frame + frame;
	auto b = frame_x2(0, 0);
*/
    // Initialize the random generator
 arma::arma_rng::set_seed_random();
	    // Create a 4x4 random matrix and print it on the screen
	     arma::Mat<double> A = arma::randu(4, 4);

		 arma::subview_row<double> rowa = A.row(0);
		 arma::rowvec x(std::vector<double>({ 0.0,1.0,2.0,3.0 }));
		 rowa = x;
		 std::cout << "A:\n" << A << "\n";
		 auto A2 = A * A.t();

		 auto cholesky = arma::chol(A2, "upper");

	     std::cout << "A2:\n" << cholesky << "\n";
		 
		     // Multiply A with his transpose:
		     std::cout << "A * A.t() =\n";
	     std::cout << A * A.t() << "\n";

		     // Access/Modify rows and columns from the array:
		     A.row(0) = A.row(1) + A.row(3);
	     A.col(3).zeros();
	     std::cout << "add rows 1 and 3, store result in row 0, also fill 4th column with zeros:\n";
	     std::cout << "A:\n" << A << "\n";
	
		     // Create a new diagonal matrix using the main diagonal of A:
		     arma::Mat<double>B = arma::diagmat(A);
	     std::cout << "B:\n" << B << "\n";
	
		     // Save matrices A and B:
		     A.save("A_mat.txt", arma::arma_ascii);
	     B.save("B_mat.txt", arma::arma_ascii);
	
		     return 0;
	
}
