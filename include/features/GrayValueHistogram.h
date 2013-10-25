#ifndef GRAYVALUEHISTOGRAM_H
#define GRAYVALUEHISTOGRAM_H

#include "BaseFeature.h"
#include "EuclideanDistance.h"





struct GVHParameters : public BaseFeatureParams
{

};





class GrayValueHistogram
        : public BaseFeature<GVHParameters, std::vector<double> >
{
public:
    typedef BaseFeature<GVHParameters, std::vector<double> > base_type;

public:
    GrayValueHistogram();
    virtual ~GrayValueHistogram();

    virtual OutputType compute(const cv::Mat& img);
    virtual double distance(const OutputType& f1, const OutputType& f2) const;

    virtual bool writeToFile(const std::string& fname) const;
    virtual bool readFromFile(const std::string& fname);
};

#endif

