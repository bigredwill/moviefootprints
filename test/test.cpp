#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <leptonica/allheaders.h>
#include <iostream>

using namespace std;

static const l_int32 SPACE = 30;
static const l_int32 MAX_WIDTH = 350;
static const char *image[4] = {"shirt.png",
                               "test24.jpg",
                               "juditharismax.jpg",
                               "hardlight2_2.jpg"};

vector<int> colors(3);
RGBA_QUAD *cta;
int green;
int blue;
int red;

static l_int32 TestImage(const char   *filename, l_int32 i, int quantN, string outputFolder);
static void PixSave32(PIXA *pixa, PIX *pixc, int quantN, string outputFolder);


int main(int    argc, char **argv)
{
    l_int32       i;
    
    int quantN;
    string outputFolder;
    if(argc == 1) {
        cout << argv[0] << endl;
        return 0;
    }
    if(argc == 2){
        quantN = stoi(argv[1]);
    } else {
        quantN = 16;
    }

    if (argc == 3) {
        outputFolder = argv[2];
    } else {
        outputFolder = "";
    }


    for (i = 0; i < 1; i++) {
        TestImage(image[i], i, quantN, outputFolder);
    }

}


static l_int32
TestImage(const char *filename, l_int32 i, int quantN, string outputFolder) {

    char buf[256];
    l_int32 w, h;
    l_float32 factor;
    PIX *pix, *pixs, *pixc, *pix32, *pixt, *pixd;
    PIXA *pixa;

    PROCNAME("TestImage");

    if ((pix = pixRead(filename)) == NULL) {
        return ERROR_INT("pix not made", procName, 1);
    }
    pixGetDimensions(pix, &w, &h, NULL);
    if (w > MAX_WIDTH) {
        factor = (l_float32)MAX_WIDTH / (l_float32)w;
        pixs = pixScale(pix, factor, factor);
    }
    else
        pixs = pixClone(pix);
    pixDestroy(&pix);

    pixa = pixaCreate(0);

        /* Median cut quantizer (no dither; 5 sigbits) */
    pixSaveTiled(pixs, pixa, 1.0, 1, SPACE, 32);
    pixc = pixMedianCutQuantGeneral(pixs, 0, 0, quantN, 5, 1, 1);
    PixSave32(pixa, pixc, quantN, "pixMedianCutQuantGeneral5sigbits");


        /* Median cut quantizer (with dither; 5 sigbits) */
    pixSaveTiled(pixs, pixa, 1.0, 1, SPACE, 0);
    pixc = pixMedianCutQuantGeneral(pixs, 1, 0, quantN, 5, 1, 1);
    PixSave32(pixa, pixc, quantN, "pixMedianCutQuantGeneral5sigbits");


        /* Median cut quantizer (no dither; 6 sigbits) */
    pixSaveTiled(pixs, pixa, 1.0, 1, SPACE, 32);
    pixc = pixMedianCutQuantGeneral(pixs, 0, 0, quantN, 6, 1, 1);
    PixSave32(pixa, pixc, quantN, "pixMedianCutQuantGeneralNoDither6sigbits");

        /* Median cut quantizer (with dither; 6 sigbits) */
    pixSaveTiled(pixs, pixa, 1.0, 1, SPACE, 0);
    pixc = pixMedianCutQuantGeneral(pixs, 1, 0, quantN, 6, 1, 1);
    PixSave32(pixa, pixc, quantN, "pixMedianCutQuantGeneralWithDither6sigbits");

        /* Median cut quantizer (mixed color/gray) */
    pixSaveTiled(pixs, pixa, 1.0, 1, SPACE, 0);
    pixc = pixMedianCutQuantMixed(pixs, 20, quantN, 0, 0, 0);
    PixSave32(pixa, pixc, quantN, "pixMedianCutQuantMixed");

        /* Simple 256 cube octcube quantizer */
    pixSaveTiled(pixs, pixa, 1.0, 1, SPACE, 0);
    pixc = pixFixedOctcubeQuant256(pixs, 1);  /* dither */
    PixSave32(pixa, pixc, quantN, "pixFixedOctcubeQuant256");

        /* 2-pass octree quantizer */
    pixSaveTiled(pixs, pixa, 1.0, 1, SPACE, 0);
    pixc = pixOctreeColorQuant(pixs, 128, 0);  /* no dither */
    PixSave32(pixa, pixc, quantN, "pixOctreeColorQuantNoDither");
    pixc = pixOctreeColorQuant(pixs, 128, 1);  /* dither */
    PixSave32(pixa, pixc, quantN, "pixOctreeColorQuantDither");

     



        /* Generate 32 bpp RGB image with num colors <= 256 */
    pixt = pixOctreeQuantNumColors(pixs, quantN, 0);   /* cmapped version */
    pix32 = pixRemoveColormap(pixt, REMOVE_CMAP_BASED_ON_SRC);

        /* Quantize image with few colors at fixed octree leaf level */
    pixSaveTiled(pixt, pixa, 1.0, 1, SPACE, 0);
    pixc = pixFewColorsOctcubeQuant1(pix32, 4);   /* level 4 */
    PixSave32(pixa, pixc, quantN, "pixFewColorsOctcubeQuant1");



        /* Mixed color/gray octree quantizer */
    pixSaveTiled(pixt, pixa, 1.0, 1, SPACE, 0);
    pixc = pixOctcubeQuantMixedWithGray(pix32, 8, quantN, 30);  /* max delta = 30 */
    PixSave32(pixa, pixc, quantN, "pixOctcubeQuantMixedWithGray");


        /* Run the high-level converter */
    pixSaveTiled(pixt, pixa, 1.0, 1, SPACE, 0);
    pixc = pixConvertRGBToColormap(pix32, 1);
    PixSave32(pixa, pixc, quantN, "pixConvertRGBToColormap");

    pixc = pixMedianCutQuantGeneral(pixs, 0, 0, quantN, 5, 1, 1);
    PixSave32(pixa, pixc, quantN, "pixMedianCutQuantGeneral");

    pixc = pixOctcubeQuantMixedWithGray(pix32, 8, quantN, 50);  /* max delta = 50 */
    PixSave32(pixa, pixc, quantN, "pixOctcubeQuantMixedWithGray");

    pixDestroy(&pix32);
    pixDestroy(&pixt);

    pixDestroy(&pixs);
    // pixDestroy(&pixd);
    pixaDestroy(&pixa);

    return 0;
}


static void
PixSave32(PIXA *pixa, PIX *pixc, int quantN, string outputName)
{
    cout << outputName << endl;
    PIX  *pix32;

    pix32 = pixConvertTo32(pixc);
    pixSaveTiled(pix32, pixa, 1.0, 0, SPACE, 0);

    PIXCMAP* map = pixGetColormap(pixc);   
    int number;
    number  = map -> n;
    cout << number << endl;

    cta = (RGBA_QUAD *)map->array;
    
    // int green;
    // int blue;
    // int red;



     string output = "testout/" + outputName+  to_string(quantN) + ".jpg";

     cout << outputName << endl;

    for (int i = 0; i < number; i++)
    {
        // pixcmapGetColor(map,i,&red, &green, &blue);
        // cout << "r: " << red;
        // cout << " g: " << green;
        // cout << " b:  " << blue;
        // cout << endl;

        // pixcmapGetColor(map,i,&colors[0], &colors[1], &colors[2]);
        // cout << "r: " << colors[0];
        // cout << " g: " << colors[1];
        // cout << " b:  " << colors[2];
        // cout << endl;



        cta = (RGBA_QUAD *)map->array;
        colors[0] = cta[i].red;
        colors[1] = cta[i].green;
        colors[2] = cta[i].blue;



    }

    cout << output.size()<<endl;

    int oSize = output.size();
 

    const char* x = (char*) malloc(oSize);

    x = output.c_str();

    cout << &pixc << "   " << &pix32 << "   " << &map << endl;

    pixWriteJpeg(output.c_str(),pix32,90,1);
    pixDestroy(&pixc);
    pixDestroy(&pix32);




}
