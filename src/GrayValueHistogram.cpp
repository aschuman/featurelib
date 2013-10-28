#include "features/GrayValueHistogram.h"

typedef unsigned char uchar;

using namespace std;





GrayValueHistogram::GrayValueHistogram()
{}



GrayValueHistogram::~GrayValueHistogram()
{}



GrayValueHistogram::OutputType GrayValueHistogram::compute(const cv::Mat &img)
{
	// adjust data type
	cv::Mat img_byte;
	if (img.depth() != CV_8U) {
		img.convertTo(img_byte, CV_8U);
	} else {
		img_byte = img;
	}

	// adjust color space
	cv::Mat img_gray;	
	if (img_byte.channels() == 1) {
		img_gray = img_byte;
	} else if (img_byte.channels() == 3 || img_byte.channels() == 4) {
		cv::cvtColor(img_byte, img_gray, CV_RGB2GRAY);
	} 

    feature_buffer_.resize(256, 0.0);
    for (int ii = 0; ii < img_gray.rows; ++ii)
    {
        for (int jj = 0; jj < img_gray.cols; ++jj)
        {
            ++feature_buffer_[(int)img_gray.at<uchar>(ii,jj)];
        }
    }

    return feature_buffer_;
}



double GrayValueHistogram::distance(const OutputType &f1,
                                    const OutputType &f2) const
{
    EuclideanDistance<double> d;
    return d.compute(f1, f2);
}



bool GrayValueHistogram::writeToFile(const string &fname) const
{
    cout << "writing..." << endl;
	return false;
}



bool GrayValueHistogram::readFromFile(const string &fname)
{
    cout << "reading..." << endl;
	return false;
}



#ifdef HAVE_MATLAB
// parameters: input:
//				input[0]: string which specifies the desired method 
//				input[1]: object handle
//				input[2..n]: method parameters
void mexFunction(int nOutput, mxArray* output[], int nInput, const mxArray* input[]) {
	 
	//// Get the command string
    char cmd[64];
	if (nInput < 1 || mxGetString(input[0], cmd, sizeof(cmd)))
		mexErrMsgTxt("First input should be a command string less than 64 characters long.");
        
	//// Construction / Destruction
    // New
    if (!strcmp("new", cmd)) {
        // Check parameters
        if (nOutput != 1)
            mexErrMsgTxt("New: One output expected.");
        // Return a handle to a new C++ instance
        output[0] = convertPtr2Mat<GrayValueHistogram>(new GrayValueHistogram);
        return;
    }
    
    // Check there is a second input, which should be the class instance handle
    if (nInput < 2)
		mexErrMsgTxt("Second input should be a class instance handle.");
    
    // Delete
    if (!strcmp("delete", cmd)) {
        // Destroy the C++ object
        destroyObject<GrayValueHistogram>(input[1]);
        // Warn if other commands were ignored
        if (nOutput != 0 || nInput != 2)
            mexWarnMsgTxt("Delete: Unexpected arguments ignored.");
        return;
    }
    
    // Get the class instance pointer from the second input
    GrayValueHistogram* GrayValueHistogram_instance = convertMat2Ptr<GrayValueHistogram>(input[1]);
    
    //// Call the various class methods
    // Compute    
    if (!strcmp("compute", cmd)) {
        //// Check parameters
        if (nOutput < 1 || nInput < 3) {
            mexErrMsgTxt("compute: Unexpected arguments.");
		}		
		
		//// prepare method parameters, conversion from matlab datatypes to c++ datatypes
		// Check if the input image is in double format
		if (!(mxIsDouble(input[2]))) {
		    mexErrMsgTxt("Input image must be gray scale and of type double");
		}
		
		// Copy input pointer 
		// This carries the input grayscale image that was sent from Matlab
		mxArray* xData = (mxArray *)input[2];

		//Get the matrix from the input data
		// The matrix is rasterized in a column wise read
		double* xValues =  mxGetPr(xData);
		int nCols = mxGetN(xData); // Gives the number of Columns in the image
		int nRows = mxGetM(xData); // Gives the number of Rows in the image
		
		/* Get the number of dimensions in the input Image */
		int number_of_dims = mxGetNumberOfDimensions(input[2]);
		if (number_of_dims > 2)
			mexErrMsgTxt("Input image should be gray scale and of type double");
		
		// Create a cv::Mat from the data so feature evaluation can be run on it
		cv::Mat grayMat(nRows, nCols, CV_8UC1);
		
		// Load the column wise vector into the Mat
		// Mat data is read in a rowwise manner
		// Appropriate conversion is carried out here
		for(int i=0;i<nCols;i++) {
		   for(int j=0;j<nRows;j++)
			{
				int value = xValues[(i*nRows)+j];
				grayMat.at<uchar>(j,i) = value;
			}
		}				
			
        //// Call the method
        GrayValueHistogram::OutputType feature = GrayValueHistogram_instance->compute(grayMat);	
		
		//// process method results, conversion from c++ datatypes to matlab datatypes		
        output[0] = mxCreateDoubleMatrix(1, feature.size(), mxREAL);
        double* dataPtr = mxGetPr(output[0]); // Get the pointer to output variable
		memcpy(dataPtr, feature.data(), sizeof(double)*feature.size());		
		
        return;
    }
    // Distance    
    if (!strcmp("distance", cmd)) {
        //// Check parameters
        if (nOutput < 1 || nInput < 4) {
            mexErrMsgTxt("distance: Unexpected arguments.");
		}
		
		//// prepare method parameters, conversion from matlab datatypes to c++ datatypes
		// Check if the input feature is in double format
		if (!(mxIsDouble(input[2])) || !(mxIsDouble(input[3]))) {
		    mexErrMsgTxt("Input feature must be of type double");
		}
		
		
		// Get the features from the input data
		// first feature
		GrayValueHistogram::OutputType ft1;		
		double* ft1Data =  mxGetPr(input[2]);
		int nCols = mxGetN(input[2]); // Gives the number of Columns
		int nRows = mxGetM(input[2]); // Gives the number of Rows
		for(int i = 0; i < nCols; i++) {
		   for(int j = 0; j < nRows; j++) {
				double value = ft1Data[(i*nRows)+j];
				ft1.push_back(value);
			}
		}
		// second feature
		GrayValueHistogram::OutputType ft2;		
		double* ft2Data =  mxGetPr(input[3]);
		nCols = mxGetN(input[3]); // Gives the number of Columns
		nRows = mxGetM(input[3]); // Gives the number of Rows
		for(int i = 0; i < nCols; i++) {
		   for(int j = 0; j < nRows; j++) {
				double value = ft2Data[(i*nRows)+j];
				ft2.push_back(value);
			}
		}
		
        //// Call the method
        double distance = GrayValueHistogram_instance->distance(ft1, ft2);
				
		//// process method results, conversion from c++ datatypes to matlab datatypes	
        output[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
        double* dataPtr = mxGetPr(output[0]); // Get the pointer to output variable
		*dataPtr = distance;
		
        return;
    }
    
    // Got here, so command not recognized
    mexErrMsgTxt("Command not recognized.");
}
#endif
