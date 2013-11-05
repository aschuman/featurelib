#include "features/GrayValueHistogram.h"

#include <fstream>

typedef unsigned char uchar;

using namespace std;





GrayValueHistogram::GrayValueHistogram()
{
    dist_ = new EuclideanDistance<vector<double> >();

}



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
    return dist_->compute(f1, f2);
}



bool GrayValueHistogram::writeToFile(const string &fname) const
{
    ofstream ofs(fname.c_str());
    if (!ofs.is_open())
    {
        cerr << "could not open file for writing..." << endl;
        return false;
    }

    ofs << feature_buffer_.size() << " ";
    for (size_t ii = 0; ii < feature_buffer_.size(); ++ii)
    {
        ofs << feature_buffer_[ii] << " ";
    }
    ofs << endl;
    ofs.close();
    return true;
}



bool GrayValueHistogram::readFromFile(const string &fname)
{
    ifstream ifs(fname.c_str());
    if (!ifs.is_open())
    {
        cerr << "could not open file for reading..." << endl;
        return false;
    }

    size_t s = 0;
    ifs >> s;
    feature_buffer_.resize(s, 0);

    for (size_t ii = 0; ii < feature_buffer_.size(); ++ii)
    {
        ifs >> feature_buffer_[ii];
    }
    ifs.close();
    return true;
}



#ifdef HAVE_MATLAB
/** Interface function for Matlab access.
 * @param classid data type of matlab's mxArray. e.g., mxDOUBLE_CLASS.
 * @param nOutput number of output arguments
 * @param output pointers to output mxArrays
 * @param nInput number of input arguments
 * @param input pointers to input mxArrays - input[0]: string which specifies the desired method - input[1]: object handle - input[2..n]: method input parameters			
 */
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
        if (nOutput > 1 || nInput != 3) {
            mexErrMsgTxt("compute: Wrong number of arguments");
		}		
		
		//// prepare method parameters, conversion from Matlab datatypes to C++ datatypes
		// argument vector
		vector<MxArray> args(input, input + nInput);		
		// convert Matlab image to OpenCV image
		cv::Mat image(args[2].toMat());
			
        //// Call the method
        GrayValueHistogram::OutputType feature = GrayValueHistogram_instance->compute(image);	
		
		//// process method results, conversion from C++ datatypes to Matlab datatypes		
        output[0] = mxCreateDoubleMatrix(1, feature.size(), mxREAL);
        double* dataPtr = mxGetPr(output[0]); // Get the pointer to output variable
		memcpy(dataPtr, feature.data(), sizeof(double)*feature.size());		
		
        return;
    }
    // Distance    
    if (!strcmp("distance", cmd)) {
        //// Check parameters
        if (nOutput > 1 || nInput != 4) {
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
	// Write to file
    if (!strcmp("writeToFile", cmd)) {
        //// Check parameters
        if (nOutput > 1 || nInput != 3) {
            mexErrMsgTxt("writeToFile: Wrong number of arguments");
		}
		
		//// Get the file path string
		char* path = mxArrayToString(input[2]);
		if (path == NULL) {
			mexErrMsgTxt("writeToFile: Conversion of file name string failed.");
		}
					
        //// Call the method
        bool successful = GrayValueHistogram_instance->writeToFile(path);			
		
		//// convert results, conversion from C++ datatypes to Matlab datatypes		
		int dims[2] = {1,1};
        output[0] = mxCreateNumericArray(2, dims, mxLOGICAL_CLASS, mxREAL);
        bool* dataPtr = (bool*)mxGetData(output[0]); // Get the pointer to output variable
		*dataPtr = successful;	
		
		// cleanup
		mxFree(path);
		
        return;
    }
	// Read from file
    if (!strcmp("readFromFile", cmd)) {
        //// Check parameters
        if (nOutput > 1 || nInput != 3) {
            mexErrMsgTxt("readFromFile: Wrong number of arguments");
		}
		
		//// Get the file path string
		char* path = mxArrayToString(input[2]);
		if (path == NULL) {
			mexErrMsgTxt("readFromFile: Conversion of file name string failed.");
		}
					
        //// Call the method
        bool successful = GrayValueHistogram_instance->readFromFile(path);			
		
		//// convert results, conversion from C++ datatypes to Matlab datatypes		
		int dims[2] = {1,1};
        output[0] = mxCreateNumericArray(2, dims, mxLOGICAL_CLASS, mxREAL);
        bool* dataPtr = (bool*)mxGetData(output[0]); // Get the pointer to output variable
		*dataPtr = successful;	
		
		// cleanup
		mxFree(path);
		
        return;
    }
    
    // Got here, so command not recognized
    mexErrMsgTxt("Command not recognized.");
}
#endif
