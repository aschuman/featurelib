#ifndef BASEFEATURE_H
#define BASEFEATURE_H

#include "BaseDistance.h"

#include <opencv2/opencv.hpp>

#include <build_matlab.h>
#ifdef HAVE_MATLAB
	#include "mex.h"
	#include "class_handle.h"
#endif





// should this be part of the class below to have access to the types?
struct BaseFeatureParams
{
public:
    BaseFeatureParams() : dim(-1) {}
    int dim;
};





template <class FeatureParameters_T, typename OutputType_T>
class BaseFeature
{
public:
    typedef FeatureParameters_T FeatureParameters;
    typedef OutputType_T        OutputType;

public:
    BaseFeature();
    BaseFeature(const FeatureParameters& p);
    virtual ~BaseFeature();

    // warp this into computeandkeep
    virtual OutputType compute(const cv::Mat& img) = 0;
    // add distance that compares to self
    // check that correct distance is used (can/should that be done in basefeatureparams?)
    virtual double distance(const OutputType& f1, const OutputType& f2) const = 0;

    // add methods that accept outputtype as parameter
    virtual bool writeToFile(const std::string& fname) const = 0;
    virtual bool readFromFile(const std::string& fname) = 0;

    virtual OutputType getFeature() const;
    virtual void setParameters(const FeatureParameters& params);
    virtual FeatureParameters getParameters() const;

protected:
    // remove those, make below stuff protected
    OutputType& featureBuffer();
    FeatureParameters& parameters();

private:
    OutputType feature_buffer_;
    FeatureParameters parameters_;
    // basedistance
};





template <class FP_T, typename OT_T>
BaseFeature<FP_T, OT_T>::BaseFeature()
{}



template <class FP_T, typename OT_T>
BaseFeature<FP_T, OT_T>::BaseFeature(const FP_T& p)
{
    setParameters(p);
}



template <class FP_T, typename OT_T>
BaseFeature<FP_T, OT_T>::~BaseFeature()
{}



template <class FP_T, typename OT_T>
OT_T BaseFeature<FP_T, OT_T>::getFeature() const
{
    return feature_buffer_;
}



template <class FP_T, typename OT_T>
void BaseFeature<FP_T, OT_T>::setParameters(const FP_T& params)
{
    //TODO: with c++11 there should be a static assert somewhere that checks
    //      the base class of FeatureParameters to be BaseFeatureParams
    parameters_ = params;
}



template <class FP_T, typename OT_T>
FP_T BaseFeature<FP_T, OT_T>::getParameters() const
{
    return parameters_;
}



template <class FP_T, typename OT_T>
OT_T& BaseFeature<FP_T, OT_T>::featureBuffer()
{
    return feature_buffer_;
}



template <class FP_T, typename OT_T>
FP_T& BaseFeature<FP_T, OT_T>::parameters()
{
    return parameters_;
}

#endif
