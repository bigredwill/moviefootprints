#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <json/json.h>

using namespace std;
using namespace cv;
using namespace Json;

int quantN;
string outputName;
string inputName;
map<string,int> totalColors;
Reader reader;
Value footprint(arrayValue);

void bhQuantizeImage(Mat* srcImage,Mat* dstImage,int colorCount,int frameNum);
Value getRGBColors(Mat* src, int frameNum);
Value getHSLColors(Mat src, int frameNum);
Mat getRGBHistogram(Mat src);
Value getNMostColors(int numColors);


int main(int argc, char **argv)
{


    if(argc == 1) {
        cout << argv[0] << endl;
        return 0;
    }
    if(argc >= 2){
        quantN = stoi(argv[1]);
    } else {
        quantN = 16;
    }

    if (argc >= 3) {
       inputName = argv[2];
    } else {
        cout << "need an input!!" << endl;
        return -1;
    }

    if (argc >= 4) {
        outputName = argv[3];
    } else {
        outputName = "test.json";
    } 

    int SIZE_NEEDED = 255*255*255;

    if(totalColors.max_size() < SIZE_NEEDED) {
        cout << totalColors.max_size() << " is less than " << SIZE_NEEDED << endl;
        return 1;
    }
    cout << totalColors.max_size() << endl << endl;

    cout << "in: " << inputName << endl;
    cout << "out: " << outputName << endl;

    VideoCapture cap(inputName);

    //is video open
    if(!cap.isOpened()){
            cout << "can't read video: " << inputName << endl;
            return -1;
    }

    Mat frame;
    Value frames(arrayValue);
    Value movie;

    //construct movie json info
    movie["name"] = inputName;
    movie["fps"] = (int)cap.get(CV_CAP_PROP_FPS);
    movie["width"] = (int)cap.get(CV_CAP_PROP_FRAME_WIDTH);
    movie["height"] = (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    movie["framecount"] = (int)cap.get(CV_CAP_PROP_FRAME_COUNT);
    movie["numColors"] = quantN;
    movie["lengthInSeconds"] = (int)(cap.get(CV_CAP_PROP_FRAME_COUNT)/cap.get(CV_CAP_PROP_FPS));
    movie["format"] = cap.get(CV_CAP_PROP_FOURCC);




    time_t now = time(0);
    tm *ltm = localtime(&now);
    string dt = to_string(ltm->tm_hour) + ":" + to_string(ltm->tm_min)+ ":" + to_string(ltm->tm_sec) + " " + to_string(ltm->tm_mon + 1) + "/" + to_string(ltm->tm_mday) + "/" + to_string(ltm->tm_year);
    movie["date"] = dt; 

    cout << movie << endl;

    //gather color information
    std::clock_t start;
    double duration;
    start = std::clock();


    ofstream ofs(outputName,ofstream::out);


    int writeInterval = (movie["fps"].asInt());
    for(int i = 0; i < movie["framecount"].asInt(); i++) {
        cap >> frame;
        if (!frame.empty()){
           if(i % writeInterval == 0) {
               bhQuantizeImage(&frame, &frame, quantN, i);
               cout << i << " " << endl; 
           }

        } else {
            cout << "frame empty" << endl;
            break;
        }
        
    }

    // Value nMost = getNMostColors( quantN);

    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    cout<< duration << endl;

    movie["timeToProcess"] = duration;
    movie["frames"] = footprint;
    // movie["topColors"] = nMost;
    cout << movie << endl;

    ofs << movie;
    ofs.close();

    return 0;
}

//http://answers.opencv.org/question/5293/colour-quantization/
void bhQuantizeImage(Mat* srcImage,Mat* dstImage,int colorCount, int frameNum)
{
    int i, size;

    CvMat *clusters;
    CvMat *points;
    CvMat *color = cvCreateMat (colorCount, 1, CV_32FC3);
    CvMat *count = cvCreateMat (colorCount, 1, CV_32SC1);


    size = srcImage->size().width * srcImage->size().height;
    *dstImage = srcImage->clone();
    clusters = cvCreateMat (size, 1, CV_32SC1);
    points = cvCreateMat (size, 1, CV_32FC3);

    for (i = 0; i < size; i++) {
        points->data.fl[i * 3 + 0] = (uchar) srcImage->data[i * 3 + 0];
        points->data.fl[i * 3 + 1] = (uchar) srcImage->data[i * 3 + 1];
        points->data.fl[i * 3 + 2] = (uchar) srcImage->data[i * 3 + 2];

    }

    cvKMeans2 (points, colorCount, clusters, cvTermCriteria (CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0));

    cvSetZero (color);
    cvSetZero (count);

    
    for (i = 0; i < size; i++) {
        int idx = clusters->data.i[i];
        int j = ++count->data.i[idx];

        color->data.fl[idx * 3 + 0] = color->data.fl[idx * 3 + 0] * (j - 1) / j + points->data.fl[i * 3 + 0] / j;
        color->data.fl[idx * 3 + 1] = color->data.fl[idx * 3 + 1] * (j - 1) / j + points->data.fl[i * 3 + 1] / j;
        color->data.fl[idx * 3 + 2] = color->data.fl[idx * 3 + 2] * (j - 1) / j + points->data.fl[i * 3 + 2] / j;
    }

    map<string,int> avg;
    string temp;
    for (i = 0; i < size; i++) {
        int idx = clusters->data.i[i];
        int r,g,b;
        b = color->data.fl[idx * 3 + 0];
        g = color->data.fl[idx * 3 + 1];
        r = color->data.fl[idx * 3 + 2];

        temp = "\"r\":"+to_string(r)+",\"g\":"+to_string(g)+",\"b\":"+to_string(b)+"";

        if(avg[temp]) {
            avg[temp] ++;
        } else {
            avg[temp] = 1;
        }


        dstImage->data[i * 3 + 0] = (char) b;
        dstImage->data[i * 3 + 1] = (char) g;
        dstImage->data[i * 3 + 2] = (char) r; 
        // dstImage->data[i * 3 + 0] = (char) color->data.fl[idx * 3 + 0];
        // dstImage->data[i * 3 + 1] = (char) color->data.fl[idx * 3 + 1];
        // dstImage->data[i * 3 + 2] = (char) color->data.fl[idx * 3 + 2];
    }

    Value tMap(arrayValue);
    Value tVal;
    int percent = 0;
    temp = "";
    string fn = to_string(frameNum);
    for (map<string,int>::iterator it = avg.begin(); it!=avg.end(); ++it) {
        
        percent = (int)((1000*(it->second)/size));

        temp = "{" + it->first + ",\"amount\":" + to_string(percent) + ",\"frame\":"+fn+"}";
        reader.parse(temp,tVal);
        tMap.append(tVal);
    }

    footprint.append(tMap);

    cvReleaseMat (&clusters);
    cvReleaseMat (&points);
    cvReleaseMat (&color);
    cvReleaseMat (&count);
}

Value getNMostColors(int numColors) {

    vector<pair<string,int>> pairs;
    for( auto itr = totalColors.begin(); itr != totalColors.end(); ++itr) {
        pairs.push_back(*itr);
    }
    sort(pairs.begin(), pairs.end(), [=](pair<string, int>& a, pair<string, int>& b)
        {
            return a.second > b.second;
        }
    );
    Value jpairs(arrayValue);

    Value tVal;

    vector<pair<string,int>> numPairs;
    string str;

    for( int i = 0; i < numColors; i++) {
        // cout << pairs[i].first << " " << pairs[i].second << endl;
        numPairs.push_back(pairs[i]);

        str = pairs[i].first + ", \"amount\":" + to_string(pairs[i].second) + "}";

        reader.parse(str,tVal);
        jpairs.append(tVal);
    }
    cout << jpairs << endl << endl;
    return jpairs;
}

Value getRGBColors(Mat* src, int frameNum) {

    int size = src->size().width * src->size().height,
                i;

    Value footprint;
    map<string,int> colors;
    int r, g, b;
    string clr;

    //just put in to a json "string" object
    int darkThresh = 40;
    for(i = 0; i < size; i ++) {
        
        clr = "{";

        b = src->data[i * 3 + 0];
        g = src->data[i * 3 + 1];
        r = src->data[i * 3 + 2];

        

        clr += "\"r\":" + to_string(r) + ",";
        clr += "\"g\":" + to_string(g) + ",";
        clr += "\"b\":" + to_string(b) + "";

        
        
        if( colors[clr] ) {
            colors[clr] ++;


        } else {
            colors[clr] = 1;
        }

        // if(r < darkThresh && b < darkThresh && g < darkThresh )
        // {
        //     continue;
        // }
        // if (r == b && r == g)
        // {
        //     continue;
        // }


    }

    string toReturn = "";

    string temp = "";
    Value tVal;

    map<string,int> avg;

    for (map<string,int>::iterator it = colors.begin(); it!=colors.end(); ++it) {

        temp = it -> first;
        temp += ",\"amount\":" + to_string(it->second/size) + ",";
        temp += "\"frameNum\":" + to_string(frameNum) + "}";

        reader.parse(temp, tVal);
        footprint.append(tVal);
    }

    return footprint;
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
