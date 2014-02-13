#include "features/LocDirectPattern.h"



typedef unsigned char uchar;

using namespace std;




LocDirectPattern::LocDirectPattern() 
{
    dist_ = new EuclideanDistance<vector<double> >();

}


LocDirectPattern::~LocDirectPattern()
{}



LocDirectPattern::OutputType LocDirectPattern::compute(const cv::Mat &img)
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
	
	// mapping of 256 histogram bins to 56 
	int mapp_arr[56] = {7,11,13,14,19,21,22,25,26,28,35,37,38,41,42,44,49,50,52,56,67,69,70,73,74,76,81,82,84,88,97,98,100,104,112,131,133,134,137,138,140,145,146,148,152,161,162,164,168,176,193,194,196,200,208,224};
	
	// define Kirsch kernels
	int ddepth = CV_32F;
	int kernel_size = 3;

	float m1[3][3] = {{-3, -3, 5}, {-3, 0, 5}, {-3, -3, 5}}; //East
	cv::Mat kernel1 = cv::Mat(kernel_size, kernel_size, ddepth, m1);
	float m2[3][3] = {{-3, 5, 5}, {-3, 0, 5}, {-3, -3, -3}}; //North East
	cv::Mat kernel2 = cv::Mat(kernel_size, kernel_size, ddepth, m2);
	float m3[3][3] = {{5, 5, 5}, {-3, 0, -3}, {-3, -3, -3}}; //North
	cv::Mat kernel3 = cv::Mat(kernel_size, kernel_size, ddepth, m3);
	float m4[3][3] = {{5, 5, -3}, {5, 0, -3}, {-3, -3, -3}}; //North West
	cv::Mat kernel4 = cv::Mat(kernel_size, kernel_size, ddepth, m4);
	float m5[3][3] = {{5, -3, -3}, {5, 0, -3}, {5, -3, -3}}; //West
	cv::Mat kernel5 = cv::Mat(kernel_size, kernel_size, ddepth, m5);
	float m6[3][3] = {{-3, -3, -3}, {5, 0, -3}, {5, 5, -3}}; //South-West
	cv::Mat kernel6 = cv::Mat(kernel_size, kernel_size, ddepth, m6);
	float m7[3][3] = {{-3, -3, -3}, {-3, 0, -3}, {5, 5, 5}}; //South
	cv::Mat kernel7 = cv::Mat(kernel_size, kernel_size, ddepth, m7);
	float m8[3][3] = {{-3, -3, -3}, {-3, 0, 5}, {-3, 5, 5}}; //South East
	cv::Mat kernel8 = cv::Mat(kernel_size, kernel_size, ddepth, m8);
	
	// handle for filtered images
	cv::Mat fimg1; 
	cv::Mat fimg2; 
	cv::Mat fimg3; 
	cv::Mat fimg4; 
	cv::Mat fimg5; 
	cv::Mat fimg6; 
	cv::Mat fimg7; 
	cv::Mat fimg8; 
	
	// filtering img with Kirsch masks
	cv::filter2D(img_gray, fimg1, ddepth, kernel1);
	cv::filter2D(img_gray, fimg2, ddepth, kernel2);
	cv::filter2D(img_gray, fimg3, ddepth, kernel3);
	cv::filter2D(img_gray, fimg4, ddepth, kernel4);
	cv::filter2D(img_gray, fimg5, ddepth, kernel5);
	cv::filter2D(img_gray, fimg6, ddepth, kernel6);
	cv::filter2D(img_gray, fimg7, ddepth, kernel7);
	cv::filter2D(img_gray, fimg8, ddepth, kernel8);	
	
	// find for each pixel 3 top edge reponse values and compute decimal LDP representation
	int sz[] = {8};
	cv::Mat all_kirschval(1, sz, CV_32F, cv::Scalar::all(0)), sort_kirschval; 
	cv::Mat img_pattern(img.rows, img.cols, CV_8U);
		 
	for (int ii = 0; ii < fimg1.rows; ii++)
    {
		for (int jj = 0; jj < fimg1.cols; jj++)
        {
			// get 8 edge response values
			all_kirschval.at<float>(0) = fimg1.at<float>(ii,jj);		
			all_kirschval.at<float>(1) = fimg2.at<float>(ii,jj);
			all_kirschval.at<float>(2) = fimg3.at<float>(ii,jj); 
			all_kirschval.at<float>(3) = fimg4.at<float>(ii,jj);
			all_kirschval.at<float>(4) = fimg5.at<float>(ii,jj);
			all_kirschval.at<float>(5) = fimg6.at<float>(ii,jj);
			all_kirschval.at<float>(6) = fimg7.at<float>(ii,jj);
			all_kirschval.at<float>(7) = fimg8.at<float>(ii,jj);
			
			// sort values
			cv::sortIdx(all_kirschval, sort_kirschval, CV_SORT_EVERY_COLUMN + CV_SORT_ASCENDING);									
			
			// get top values
			int top1 = sort_kirschval.at<int>(0,0);
			int top2 = sort_kirschval.at<int>(1,0);
			int top3 = sort_kirschval.at<int>(2,0);					
			
			// convert LDP feature value to decimal number
			float pattern_val = pow(float(2), top1) + pow(float(2), top2) + pow(float(2), top3);
			
			// create matrix with LDP values
			img_pattern.at<uchar>(ii,jj) = int(pattern_val);								
						
       }
    }

	// create histogram 
	std::vector<int> buffer_all;
	buffer_all.resize(256,0.0);
		
    for (int ii = 0; ii < img_gray.rows; ++ii)
    {
        for (int jj = 0; jj < img_gray.cols; ++jj)
        {
            ++buffer_all[(int)img_pattern.at<uchar>(ii,jj)];
        }
    }
	feature_buffer_.clear();

	// map to histogram with 56 bins
	feature_buffer_.resize(56, 0.0);
	for (int kk = 0; kk < 56; ++kk)
	{
		int mappvar = mapp_arr[kk];
		feature_buffer_[kk] = buffer_all[mappvar];
	}

    return feature_buffer_;
}



double LocDirectPattern::distance(const OutputType &f1,
                                    const OutputType &f2) const
{
    return dist_->compute(f1, f2);
}



bool LocDirectPattern::writeToFile(const string &fname) const
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



bool LocDirectPattern::readFromFile(const string &fname)
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
        output[0] = convertPtr2Mat<LocDirectPattern>(new LocDirectPattern);
        return;
    }
    
    // Check there is a second input, which should be the class instance handle
    if (nInput < 2)
		mexErrMsgTxt("Second input should be a class instance handle.");
    
    // Delete
    if (!strcmp("delete", cmd)) {
        // Destroy the C++ object
        destroyObject<LocDirectPattern>(input[1]);
        // Warn if other commands were ignored
        if (nOutput != 0 || nInput != 2)
            mexWarnMsgTxt("Delete: Unexpected arguments ignored.");
        return;
    }
    
    // Get the class instance pointer from the second input
    LocDirectPattern* LocDirectPattern_instance = convertMat2Ptr<LocDirectPattern>(input[1]);
    
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
        LocDirectPattern::OutputType feature = LocDirectPattern_instance->compute(image);	
		
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
		LocDirectPattern::OutputType ft1;		
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
		LocDirectPattern::OutputType ft2;		
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
        double distance = LocDirectPattern_instance->distance(ft1, ft2);
				
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
        bool successful = LocDirectPattern_instance->writeToFile(path);			
		
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
        bool successful = LocDirectPattern_instance->readFromFile(path);			
		
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


