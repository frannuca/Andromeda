#include <iostream>
#include <armadillo>
#include "../src/data/Series.h"
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

	std::vector<std::pair<std::string, double>> points;
	points.push_back(std::make_pair("onesssss", 1.0));
	points.push_back(std::make_pair("two", 2.0));
	points.push_back(std::make_pair("three", 3.0));
	data::Series<std::string, double, ByLength> series(points);
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
