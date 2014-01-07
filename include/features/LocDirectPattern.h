#ifndef LOCDIRECTPATTERN_H
#define LOCDIRECTPATTERN_H

#include "ImageFeature.h"
#include "EuclideanDistance.h"
#include <math.h>  
#include <fstream>
  





struct LDPParameters : public FeatureParameters
{
    LDPParameters()
        : normalize(false)
    {}

    bool normalize;
};





class LocDirectPattern : public ImageFeature<LDPParameters, std::vector<double> >
{
public:
    typedef ImageFeature<LDPParameters, std::vector<double> > BaseType;

public:
    LocDirectPattern();
    virtual ~LocDirectPattern();

    virtual OutputType compute(const cv::Mat& img);
    virtual double distance(const OutputType& f1, const OutputType& f2) const;

    virtual bool writeToFile(const std::string& fname) const;
    virtual bool readFromFile(const std::string& fname);

};

#endif

