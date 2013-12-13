#ifndef GRAYVALUEHISTOGRAM_H
#define GRAYVALUEHISTOGRAM_H

#include "ImageFeature.h"
#include "EuclideanDistance.h"




/**
 *  Parameters for class GrayValueHistogram.
 *	@see GrayValueHistogram
 */
struct GVHParameters : public FeatureParameters
{
    GVHParameters()
        : normalize(false)
    {}
	
	/// Flag that indicates if histogram will be normalized. Default: false.
    bool normalize;
};




/**
 *  Feature is a gray value histogram of the image.
 */
class GrayValueHistogram
        : public ImageFeature<GVHParameters, std::vector<double> >
{
public:
    typedef ImageFeature<GVHParameters, std::vector<double> > BaseType;

public:
    GrayValueHistogram();
    virtual ~GrayValueHistogram();
	
	/** Computes feature in given image.
    * @param img Input image. It is internally converted to grayscale and 8 Bit depth if not given in this format.
    * @return Feature of output type is returned. Currently histogram with 256 bins.
    */
    virtual OutputType compute(const cv::Mat& img);
	/** Computes distance between to gray value histogram features. Default is euclidean distance. 
    * @param f1 First feature.
    * @param f2 Second feature.
    * @return Scalar distance between the features. Distance is greater or equal than zero.
    */
    virtual double distance(const OutputType& f1, const OutputType& f2) const;

	/** Writes last computed feature to a file.
    * @param fname Filepath where feature will be saved.
    * @return Returns true if writing was successful.
    */
    virtual bool writeToFile(const std::string& fname) const;
	/** Reads feature from file and saves it internally.
    * @param fname Filepath from which feature should be loaded.
    * @return Returns true if reading was successful.
    */
    virtual bool readFromFile(const std::string& fname);
};

#endif

