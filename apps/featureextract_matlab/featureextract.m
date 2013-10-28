% Example application for featurelib. Loads two images and compares them with gray value histogram feature. Image paths are optional.
function [] = featureextract(imagePath1, imagePath2)

    % load images
    if (nargin < 1)
        img1 = imread('../../data/lena.bmp');
        img2 = imread('../../data/lena_bright.bmp');
    elseif (nargin < 2)
        img1 = imread(imagePath1);
        img2 = imread('../../data/lena.bmp');
    else
        img1 = imread(imagePath1);
        img2 = imread(imagePath2);        
    end
    
    % temporarilly add mex path to Matlab path
    addpath('../../mex/');  
    
    % convert to grayscale and double datatype
    imgGray1 = double(rgb2gray(img1));    
    imgGray2 = double(rgb2gray(img2));    
    
    % compute features
    gvh = GrayValueHistogram();
    feature1 = gvh.compute(imgGray1);
    feature2 = gvh.compute(imgGray2);
    
    % show images
    imshow(img1);   
    imshow(img2);
    
    % get feature distance
    distance = gvh.distance(feature1, feature2);
    fprintf(1, 'Feature distance is %f', distance);
    
    % TODO: remove path does not work
    % remove mex path from Matlab path
    % rmpath('../../bin/mex/');
end

