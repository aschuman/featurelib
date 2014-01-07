#ifndef MODCENSUSPATTERN_H
#define MODCENSUSPATTERN_H

#include "ImageFeature.h"
#include "EuclideanDistance.h"
#include <math.h>  
#include <fstream>
  






struct MCPParameters : public FeatureParameters
{
    MCPParameters()
        : normalize(false)
    {}

    bool normalize;
};





class ModCensusPattern : public ImageFeature<MCPParameters, std::vector<double> >
{
public:
    typedef ImageFeature<MCPParameters, std::vector<double> > BaseType;

public:
    ModCensusPattern();
    virtual ~ModCensusPattern();

    virtual OutputType compute(const cv::Mat& img);
    virtual double distance(const OutputType& f1, const OutputType& f2) const;

    virtual bool writeToFile(const std::string& fname) const;
    virtual bool readFromFile(const std::string& fname);

};

#endif

