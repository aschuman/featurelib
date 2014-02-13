#ifndef LOCDIRECTPATTERN_H
#define LOCDIRECTPATTERN_H

#include "ImageFeature.h"
#include "EuclideanDistance.h"
#include <math.h>  
#include <fstream>
  


/**
 *  Parameters for class LocDirectPattern.
 *        @see LocDirectPattern
 */
struct LDPParameters : public FeatureParameters
{
    LDPParameters()
        : normalize(false)
    {}

	/// Flag that indicates if histogram will be normalized. Default: false.
    bool normalize;
};




/**
 *  Feature is a local directional pattern (LDP) of the input image. An LDP pattern is computed by obtaining edge response values in eight directions at each pixel position and generating a code from the most prominent directions. 
 By using the Kirsch operator the edges are computed in the directions E, NE, N, NW, W, SW, S, SE. For each pixel the edge response values are sorted to find three top directions. Each direction represents a binary position.  
 The 8-bit LDP code is generated by setting the binary digits of the top direction to 1. All other binary digits are set to 0. After converting to decimal system, a histogram of all LDP values of the image is generated and returned as feature vector. 

 Reference: Jabid, T.; Kabir, M.H.; Chae, O., "Local Directional Pattern for face recognition", ICCE, 2010
 */
class LocDirectPattern : public ImageFeature<LDPParameters, std::vector<double> >
{
public:
    typedef ImageFeature<LDPParameters, std::vector<double> > BaseType;

public:
    LocDirectPattern();
    virtual ~LocDirectPattern();

	 /** Computes feature in given image.
    * @param img Input image. It is internally converted to grayscale and 8 Bit depth if not given in this format.
    * @return Feature of output type is returned. Currently histogram with 56 bins.
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

