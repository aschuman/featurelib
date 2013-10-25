#include "features/GrayValueHistogram.h"

typedef unsigned char uchar;

using namespace std;





GrayValueHistogram::GrayValueHistogram()
{}



GrayValueHistogram::~GrayValueHistogram()
{}



GrayValueHistogram::OutputType GrayValueHistogram::compute(const cv::Mat &img)
{
    cv::Mat img_gray(img.rows, img.cols, CV_8UC1);
    cv::cvtColor(img, img_gray, CV_RGB2GRAY);

    featureBuffer().resize(256, 0.0);
    for (int ii = 0; ii < img_gray.rows; ++ii)
    {
        for (int jj = 0; jj < img_gray.cols; ++jj)
        {
            ++featureBuffer()[(int)img_gray.at<uchar>(ii,jj)];
        }
    }

    return featureBuffer();
}



double GrayValueHistogram::distance(const OutputType &f1,
                                    const OutputType &f2) const
{
    EuclideanDistance<vector<double> > d;
    return d.compute(f1, f2);
}



bool GrayValueHistogram::writeToFile(const string &fname) const
{
    cout << "writing..." << endl;
}



bool GrayValueHistogram::readFromFile(const string &fname)
{
    cout << "reading..." << endl;
}
