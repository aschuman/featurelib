#ifndef MODCENSUSPATTERN_H
#define MODCENSUSPATTERN_H

#include "ImageFeature.h"
#include "EuclideanDistance.h"
#include <math.h>  
#include <fstream>
  

/**
 *  Parameters for class ModCensusPattern.
 *        @see ModCensusPattern
 */
struct MCPParameters : public FeatureParameters
{
    MCPParameters()
        : normalize(false)
    {}
	/// Flag that indicates if histogram will be normalized. Default: false.
    bool normalize;
};




/**
 *  Feature is a modified Census Transform pattern of the input image. The features are defined as structure kernels of size 3x3. The kernel structure information is coded as binary information. 
 This leads to a number of 2^9 - 1 = 511 possible local structure kernels regarding the redundancy that a kernel with all elements 0 and that with all 1 contains the same information. 
 The modified Census Transform computes the structure kernels by comparing each pixel of the kernel with the mean of all kernel pixels. 
 If a pixel has a larger value than the mean value, the corresponding binary value is set to 1, otherwise to 0. By sliding the kernel of the given image, a binary feature value is computed for each image pixel.
 After converting the binary feature value to the decimal system, the histogram of all feature values is generated and returned as feature vector. 

 Reference: Froba, B.; Ernst, A., "Face Detection with the Modified Census Transform", Sixth IEEE International Conference on Automatic Face and Gesture Recognition Proceedings, 2004.
 */
class ModCensusPattern : public ImageFeature<MCPParameters, std::vector<double> >
{
public:
    typedef ImageFeature<MCPParameters, std::vector<double> > BaseType;

public:
    ModCensusPattern();
    virtual ~ModCensusPattern();
	 
	 /** Computes feature in given image.
    * @param img Input image. It is internally converted to grayscale and 32F Bit depth if not given in this format.
    * @return Feature of output type is returned. Currently histogram with 511 bins.
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

