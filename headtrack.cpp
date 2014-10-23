

#include <opencv2/core/core.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char const *argv[])
{

	// std::string clipname = "kittenmittens.avi";
	std::string clipname = "tards.mp4";

	cout << clipname << endl;

	vector<Mat> images;

	VideoCapture cap(clipname);

	if(!cap.isOpened()){
            cout << "can't read video: " << clipname << endl;
            return -1;
        }
    cout << "video height: " <<cap.get(CV_CAP_PROP_FRAME_WIDTH) << endl;
	cout << "video width: " << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;

	int fcount = (int)cap.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "frame count: " << fcount << endl;


	Mat frame;
    // while(cap.read(frame)){
    // 	cout << "FUCK"<<frame.size() << endl;
    // }

    for(int i = 0; i < fcount; i++) {

    	cap >> frame;


		 if (!frame.empty())
	    	imshow("kittenmittens",frame);
    	
    	char key = (char) waitKey('q');

    	if(key == 'q')  break;
    	
    }
    


	return 0;
}