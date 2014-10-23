

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

	std::string clipname = "video.mp4";
	cout << clipname << endl;

	VideoCapture cap(clipname);

    //is video open
	if(!cap.isOpened()){
            cout << "can't read video: " << clipname << endl;
            return -1;
    }

    cout << "video height: " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << endl;
	cout << "video width: "  << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;

    //get amount of frames in video
	int fcount = (int)cap.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "frame count: " << fcount << endl;

	Mat frame;
    for(int i = 0; i < fcount; i++) {
        //read in frame from video
    	cap >> frame;

        //to avoid 0x0 frame error
		if (!frame.empty())
	    	imshow("footprint",frame);
    	
    	char key = (char) waitKey('q');
    	if(key == 'q')  break;
    	
    }
    


	return 0;
}