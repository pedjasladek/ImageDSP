
#include "ImageProcessing.h"
#include "ImageInterpolation.h"

#include <QDebug>

int upto(int x, int y) {
    return (x + y - 1) & ~(y - 1);
}

void imageProcessingFun(const QString& progName, QImage* const outImgs, const QImage* const inImgs, const QVector<double>& params) 
{
	int X_SIZE = inImgs->width();
	int Y_SIZE = inImgs->height();


	if(progName == "Sample and hold") 
	{	
        double vertical_factor = params[0], horizontal_factor = params[1];
        int newX_SIZE = upto(horizontal_factor * X_SIZE , 4);
        int newY_SIZE = upto(vertical_factor * Y_SIZE , 4);

        *outImgs = *(new QImage(newX_SIZE, newY_SIZE, inImgs->format()));

        sampleAndHold(inImgs->bits() , X_SIZE , Y_SIZE , outImgs->bits() , newX_SIZE , newY_SIZE);
	}
	else if (progName == "Bilinear") 
	{

            int X_SIZE_NEW = upto(X_SIZE * params[1], 4);
            int Y_SIZE_NEW = upto(X_SIZE * params[0], 4);

            new (outImgs) QImage(X_SIZE_NEW, Y_SIZE_NEW, inImgs->format());

            bilinearInterpolate(inImgs->bits(), X_SIZE, Y_SIZE, outImgs->bits(), X_SIZE_NEW, Y_SIZE_NEW);
	}
	else if (progName == "Bicubic")
	{
		/* Input image data in RGB format can be obtained with inImgs->bits() */
		/* Vertical scale factor is params[0] */
		/* Horizontal scale factor is params[1] */

		/* TO DO: Calculate output image resolution and construct output image object */

		/* TO DO: Perform Bicubic interpolation  */

	}
	else if(progName == "Rotation") 
	{	
        uchar* output=(uchar*)malloc(3*sizeof(uchar)*X_SIZE*Y_SIZE);


        imageRotate((uchar*)inImgs->bits(),X_SIZE,Y_SIZE,output,X_SIZE/2,Y_SIZE/2,params[0]);

        QImage img(output, X_SIZE, Y_SIZE, inImgs->format());
        *outImgs = img;

	}
	else if (progName == "Rotation Bilinear") 
    {

            uchar* output=(uchar*)malloc(3*sizeof(uchar)*X_SIZE*Y_SIZE);

            imageRotateBilinear((uchar*)inImgs->bits(),X_SIZE,Y_SIZE,output,X_SIZE/2,Y_SIZE/2,params[0]);

            QImage img(output, X_SIZE, Y_SIZE, inImgs->format());
            *outImgs = img;
        }


}

