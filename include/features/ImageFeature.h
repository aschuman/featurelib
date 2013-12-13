#ifndef BASEFEATURE_H
#define BASEFEATURE_H

#include "Distance.h"
#include "EuclideanDistance.h"
#include "build_matlab.h"
#ifdef HAVE_MATLAB
    #include <mex.h>
	#include "mexopencv.hpp"
    #include "class_handle.h"
#endif

#include <opencv2/opencv.hpp>






struct FeatureParameters
{
public:
    FeatureParameters() : dim(-1) {}
    int dim;
};





template <class FP_T, typename O_T>
class ImageFeature
{
public:
    typedef FP_T    Parameters;
    typedef O_T     OutputType;

public:
    ImageFeature();
    ImageFeature(const Parameters& p);
    virtual ~ImageFeature();

    // warp this into computeandkeep
    virtual OutputType compute(const cv::Mat& img) = 0;
    // add distance that compares to self
    virtual double distance(const OutputType& f1, const OutputType& f2) const = 0;

    // add methods that accept outputtype as parameter
    virtual bool writeToFile(const std::string& fname) const = 0;
    virtual bool readFromFile(const std::string& fname) = 0;

	virtual OutputType getFeature() const;
	virtual void       setDistance(const Distance<OutputType>& dist);
	virtual void       setParameters(const Parameters& params);
    virtual Parameters getParameters() const;

protected:
    OutputType feature_buffer_;
    Parameters parameters_;
	const Distance<OutputType>* dist_;	// constant distance, pointer is not constant
};





template <class FP_T, typename O_T>
ImageFeature<FP_T, O_T>::ImageFeature()
    : dist_(NULL)
{}

template <class FP_T, typename O_T>
ImageFeature<FP_T, O_T>::ImageFeature(const FP_T& p)
{
    setParameters(p);
}



template <class FP_T, typename O_T>
ImageFeature<FP_T, O_T>::~ImageFeature()
{
    if (dist_ != NULL) delete dist_;
}



template <class FP_T, typename O_T>
O_T ImageFeature<FP_T, O_T>::getFeature() const
{
    return feature_buffer_;
}



template <class FP_T, typename O_T>
void ImageFeature<FP_T, O_T>::setDistance(const Distance<O_T>& dist)
{
	//TODO: check that the distance in has corrent O_T
	dist_ = &dist;
}


template <class FP_T, typename O_T>
void ImageFeature<FP_T, O_T>::setParameters(const FP_T& params)
{
	//TODO: check that params has the right base class (boost?)
	parameters_ = params;
}



template <class FP_T, typename O_T>
FP_T ImageFeature<FP_T, O_T>::getParameters() const
{
    return parameters_;
}

#endif
