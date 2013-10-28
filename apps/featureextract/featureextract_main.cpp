#include <iostream>
#include <cstdlib>

#include "featurelib.h"
#include <opencv2/opencv.hpp>

using namespace std;




int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " imagename" << endl;
        exit(1);
    }

    string imgname = argv[1];
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
