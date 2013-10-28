#ifndef GRAYVALUEHISTOGRAM_H
#define GRAYVALUEHISTOGRAM_H

#include "ImageFeature.h"
#include "EuclideanDistance.h"





struct GVHParameters : public FeatureParameters
{
    GVHParameters()
        : normalize(false)
    {}

    bool normalize;
};





class GrayValueHistogram
        : public ImageFeature<GVHParameters, std::vector<double> >
{
public:
    typedef ImageFeature<GVHParameters, std::vector<double> > BaseType;

public:
    GrayValueHistogram();
    virtual ~GrayValueHistogram();

    virtual OutputType compute(const cv::Mat& img);
    virtual double distance(const OutputType& f1, const OutputType& f2) const;

    virtual bool writeToFile(const std::string& fname) const;
    virtual bool readFromFile(const std::string& fname);
};

#endif

