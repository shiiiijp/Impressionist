// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//


#include <FL/fl_ask.H>
#include <algorithm>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "impBrush.h"

// Include individual brush headers here.
//ブラシ追加
#include "pointBrush.h"
#include "triangleBrush.h"
#include "circleBrush.h"
#include "lineBrush.h"
#include "scatteredPointBrush.h"
#include "scatteredCircleBrush.h"
#include "scatteredLineBrush.h"
#include "splitMilkBrush.h"
#include "reversedCircleBrush.h"
#include "reversedScatteredCircleBrush.h"
#include "reversedScatteredLineBrush.h"


#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucBitmap		= NULL;
	m_ucPainting	= NULL;
	m_ucPreviewBackup = NULL;


	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );
	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_TRIANGLES] = new TriangleBrush(this, "Triangles");
	ImpBrush::c_pBrushes[BRUSH_CIRCLES] = new CircleBrush(this, "Circles");
	ImpBrush::c_pBrushes[BRUSH_LINES] = new LineBrush(this, "Lines");
	ImpBrush::c_pBrushes[BRUSH_SCATTEREDPOINTS] = new ScatteredPointBrush(this, "ScatteredPoints");
	ImpBrush::c_pBrushes[BRUSH_SCATTEREDCIRCLES] = new ScatteredCircleBrush(this, "ScatteredCircles");
	ImpBrush::c_pBrushes[BRUSH_SCATTEREDLINES] = new ScatteredLineBrush(this, "ScatteredLines");
	ImpBrush::c_pBrushes[BRUSH_SPLITMILK] = new SplitMilkBrush(this, "SplitMilk");
	ImpBrush::c_pBrushes[BRUSH_REVERSEDCIRCLES] = new ReversedCircleBrush(this, "ReversedCircles");
	ImpBrush::c_pBrushes[BRUSH_REVERSEDSCATTEREDCIRCLES] = new ReversedScatteredCircleBrush(this, "ReversedScatteredCircles");
	ImpBrush::c_pBrushes[BRUSH_REVERSEDSCATTEREDLINES] = new ReversedScatteredLineBrush(this, "ReversedScatteredLines");

	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];

}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

int ImpressionistDoc::getAngle()
{
	return m_pUI->getAngle();
}

int ImpressionistDoc::getWidth()
{
	return m_pUI->getWidth();
}

int ImpressionistDoc::getAlpha()
{
	return m_pUI->getAlpha();
}

int ImpressionistDoc::getAmount()
{
	return m_pUI->getAmount();
}

int ImpressionistDoc::getRad()
{
	return m_pUI->getRad();
}

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=readBMP(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	// release old storage
	delete [] m_ucBitmap;
	delete [] m_ucPainting;
	delete [] m_ucPreviewBackup;

	m_ucBitmap		= data;

	// allocate space for draw view
	m_ucPainting		= new unsigned char [width*height*3];
	m_ucPreviewBackup	= new unsigned char [width*height*3];
	memset(m_ucPainting, 0, width*height*3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();


	return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) 
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}

// Apply the filter specified by filter_kernel to the 
// each pixel in the source buffer and place the resulting
// pixel in the destination buffer.  

// This is called from the UI when the
// "preview" or "apply" button is pressed in the filter dialog.



/*
 *	INPUT: 
 *		sourceBuffer:		the original image buffer, 
 *		srcBufferWidth:		the width of the image buffer
 *		srcBufferHeight:	the height of the image buffer
 *							the buffer is arranged such that 
 *							origImg[3*(row*srcBufferWidth+column)+0], 
 *							origImg[3*(row*srcBufferWidth+column)+1], 
 *							origImg[3*(row*srcBufferWidth+column)+2]
 *							are R, G, B values for pixel at (column, row).
 *		destBuffer:			the image buffer to put the resulting
 *							image in.  It is always the same size
 *							as the source buffer.
 *
 *      filterKernel:		the 2D filter kernel,
 *		knlWidth:			the width of the kernel
 *		knlHeight:			the height of the kernel
 *
 *		divisor, offset:	each pixel after filtering should be
 *							divided by divisor and then added by offset
 */
void ImpressionistDoc::applyFilter( const unsigned char* sourceBuffer,
		int srcBufferWidth, int srcBufferHeight,
		unsigned char* destBuffer,
		const double *filterKernel, 
		int knlWidth, int knlHeight, 
		double divisor, double offset )
{
	// This needs to be implemented for image filtering to work.
	 double bufR,bufB,bufG;
     int i,j,k,l;

	 for (i = 0; i < srcBufferWidth; i++) {
		 for (j = 0; j < srcBufferHeight; j++) {
			 destBuffer[3 * (i + srcBufferWidth * j)] = 0;
			 destBuffer[3 * (i + srcBufferWidth * j) + 1] = 0;
			 destBuffer[3 * (i + srcBufferWidth * j) + 2] = 0;
		 }
	 }
	 for (i = 2; i < srcBufferWidth - 2; i++) {
		 for (j = 2; j < srcBufferHeight - 2; j++) {
			 bufR = bufG = bufB = offset;
			 for (k = 0; k < knlWidth; k++) {
				 for (l = 0; l < knlHeight; l++) {
					 bufR += filterKernel[k + knlWidth * l] * sourceBuffer[3 * (i + k-2 + srcBufferWidth * (j + l - 2))] / divisor;
					 bufG += filterKernel[k + knlWidth * l] * sourceBuffer[3 * (i + k-2 + srcBufferWidth * (j + l - 2)) + 1] / divisor;
					 bufB += filterKernel[k + knlWidth * l] * sourceBuffer[3 * (i + k-2 + srcBufferWidth * (j + l - 2)) + 2] / divisor;
				 }
			 }
			 if (bufR > 255)	bufR = 255;
			 else if (bufR < 0)	bufR = 0;
			 if (bufG > 255)	bufG = 255;
			 else if (bufG < 0)	bufG = 0;
			 if (bufB > 255)	bufB = 255;
			 else if (bufB < 0)	bufB = 0;
			 destBuffer[3 * (i + srcBufferWidth * j)] = (unsigned char)bufR;
			 destBuffer[3 * (i + srcBufferWidth * j) + 1] = (unsigned char)bufG;
			 destBuffer[3 * (i + srcBufferWidth * j) + 2] = (unsigned char)bufB;
		 }
	 }

}



//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}



//３回目実習　FilterKernel
int ImpressionistDoc::copyImageToCanvas() {
     if ( m_ucPainting ) {
          // キャンバスのバッファを再確保
          delete [] m_ucPainting;
          m_nPaintWidth = m_nWidth;
          m_nPaintHeight = m_nHeight;
          m_ucPainting = new unsigned char [m_nPaintWidth*m_nPaintHeight*3];

          // 値をコピー
          memcpy( m_ucPainting, m_ucBitmap, m_nPaintWidth * m_nPaintHeight * 3 );
          m_pUI->m_paintView->refresh();            // 再描画
     }
     return 0;
}

