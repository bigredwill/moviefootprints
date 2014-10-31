#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <leptonica/allheaders.h>
#include <iostream>

using namespace std;
using namespace cv;
Mat getHSVHistogram(Mat src);
Mat getRGBHistogram(Mat src);
Mat quantizeImage (Mat src, int width, int height);
void displayHistogram(Mat src);

	const int MAX_CLUSTERS = 5;

int main(int argc, char const *argv[]) {

	if(argc != 2) {
		cout << endl << "proper usage: videofootprint video.mp4" << endl << endl;
		return -1;
	}

	string clipname = argv[1];

	cout << clipname << endl;

	VideoCapture cap(clipname);

	if(!cap.isOpened()) {
		cout << "can't read video file: " << clipname << endl;
		return -1;
	}

	int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int frameCount = cap.get(CV_CAP_PROP_FRAME_COUNT);
	int fps = cap.get(CV_CAP_PROP_FPS);
	int vidLength = (int)(frameCount / fps);

	cout << "video height: " << height << "\nvideo width: " << width << "\nvideo length: " << vidLength << " seconds" << endl;
	
	Mat hsv, rgb, frame, quant;
	for(int i = 0; i < frameCount; i++) {
        //read in frame from video
    	cap >> frame;
    	if(i%5==0){
			hsv = getHSVHistogram(frame);
			rgb = getRGBHistogram(frame);
			// quant = quantizeImage(frame, width, height);
	        
	        imshow("frame", frame);
	        imshow("hsv histogram", hsv);
	        imshow("rgb histogram", rgb);
	        //	imshow("QuantizeImage", quant);

	        // moveWindow("quantizeImage",350, height+20);
	        moveWindow("rgb histogram", 0, 0);
	        moveWindow("frame",300,0);
	        moveWindow("hsv histogram", 0, 350);
	    }

        //to avoid 0x0 frame error
		// if (!frame.empty())
	 //    	imshow("footprint",frame);
    	
    	char key = (char) waitKey('q');
    	if(key == 'q')  break;

    		
    	
    }

}

Mat quantizeImage (Mat src, int width, int height) {
	Mat ret;
	cvtColor(src,ret, CV_BGR2Lab);
	ret = ret.reshape(3, width*height);




	// Mat out;
	// kmeans(ret, 4, out, 1, , KMEANS_RANDOM_CENTERS);

	RNG rng(12345);

	int k, clusterCount = rng.uniform(2, MAX_CLUSTERS+1);
    int i, sampleCount = rng.uniform(1, 1001);
    Mat points(sampleCount, 1, CV_32FC2), labels;

    clusterCount = MIN(clusterCount, sampleCount);
    Mat centers;


	kmeans(points, clusterCount, labels,
            TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 10, 1.0),
               3, KMEANS_PP_CENTERS, centers);

	ret = ret.reshape(3,height);
	cvtColor(ret,ret, CV_Lab2BGR);
	return ret;
}

Mat getRGBHistogram(Mat src) {
	Mat rgb;
	vector<Mat> bgr_planes;
	split(src, bgr_planes);

	int histSize = 256;

	float range[] = { 0, 256 } ;

	const float* histRange = { range };

	bool uniform = true;
	bool accumulate = false;

	Mat b_hist, g_hist, r_hist;

	//compute histograms

	calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

	int hist_w = 300;
	int hist_h = 300;

	int bin_w = cvRound( (double) hist_w/histSize );

	Mat histImage ( hist_h, hist_w, CV_8UC3, Scalar(0,0,0) );

	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

	for( int i = 1; i < histSize; i++ )
  	{
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                       Scalar( 0, 255, 0), 2, 8, 0  );
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                       Scalar( 0, 0, 255), 2, 8, 0  );
  	}

  	return histImage;
}

Mat getHSVHistogram(Mat src) {

	Mat hsv;
	cvtColor(src, hsv, CV_BGR2HSV);

	// Quantize the hue to 30 levels
	// Saturation to 32 levels

	int hbins = 30, sbins = 32;
	int histSize[] = {hbins, sbins};
	// hue varies from 0 to 179
	float hranges[] = { 0, 180 };
	// saturation varies from 0 (black gray white) to 255 (pure spectrum color)
	float sranges[] =  { 0, 256 };
	const float* ranges[] = { hranges, sranges };
	MatND hist;
	//compute histogram from the 0-th and 1-st channels
	int channels[] = { 0, 1 };
	//http://docs.opencv.org/modules/imgproc/doc/histograms.html
	calcHist( &hsv, 1, channels, Mat(), hist, 2, histSize, ranges, true, false );
	double maxVal = 0;
	minMaxLoc(hist, 0, &maxVal, 0, 0);

	int scale = 10;
	Mat histImg = Mat::zeros(sbins*scale, hbins*scale, CV_8UC3);

	for( int h = 0; h < hbins; h++ )
	{
		for( int s = 0; s < sbins; s++ )
		{
			float binVal = hist.at<float>(h, s);
			int intensity = cvRound(binVal * 255 / maxVal);
			rectangle(histImg, 
						Point(h*scale, s*scale), 
						Point( (h+1)*scale - 1,
						(s+1)*scale - 1), 
						Scalar::all(intensity), 
						CV_FILLED);

			// line(histImg, Point(h*scale, s*scale), Point( (h+1)*scale - 1,
			// 			(s+1)*scale - 1), Scalar(0,50,255) );
		}
	}


	return histImg;
}

void displayHistogram(MatND src) {
	// imshow(src);
}