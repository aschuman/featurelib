#ifndef LOCTERNPATTERN_H
#define LOCTERNPATTERN_H

#include "ImageFeature.h"
#include "EuclideanDistance.h"
#include <math.h>  
#include <fstream>
  






struct LTPParameters : public FeatureParameters
{
    LTPParameters()
        : normalize(false)
    {}

    bool normalize;
};





class LocTernPattern : public ImageFeature<LTPParameters, std::vector<double> >
{
public:
    typedef ImageFeature<LTPParameters, std::vector<double> > BaseType;

public:
    LocTernPattern();
    virtual ~LocTernPattern();

    virtual OutputType compute(const cv::Mat& img);
    virtual double distance(const OutputType& f1, const OutputType& f2) const;

    virtual bool writeToFile(const std::string& fname) const;
    virtual bool readFromFile(const std::string& fname);

};

#endif

