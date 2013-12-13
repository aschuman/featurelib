#include <iostream>
#include <cstdlib>

#include "featurelib.h"
#include <opencv2/opencv.hpp>

using namespace std;




int main(int argc, char* argv[])
{
	string imgname;
    if (argc < 2)
	{
		cout << "Usage: " << argv[0] << " imagename" << endl;		
		cout << "No imagename found, using default image instead." << endl;		
		imgname = "../../data/lena.bmp";
    } else {
		imgname = argv[1];
	}
    cv::Mat img = cv::imread(imgname);

    GrayValueHistogram gvh;
    gvh.compute(img);
    gvh.writeToFile("./tmp.txt");
    GrayValueHistogram gvh2;
    gvh2.readFromFile("./tmp.txt");

    cout << "dist is " << gvh.distance(gvh.getFeature(), gvh2.getFeature())
            << endl;

    cv::imshow("testwin", img);
    cv::waitKey();

    return EXIT_SUCCESS;
}
