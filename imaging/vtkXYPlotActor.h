/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkXYPlotActor.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 1993-1995 Ken Martin, Will Schroeder, Bill Lorensen.

This software is copyrighted by Ken Martin, Will Schroeder and Bill Lorensen.
The following terms apply to all files associated with the software unless
explicitly disclaimed in individual files. This copyright specifically does
not apply to the related textbook "The Visualization Toolkit" ISBN
013199837-4 published by Prentice Hall which is covered by its own copyright.

The authors hereby grant permission to use, copy, and distribute this
software and its documentation for any purpose, provided that existing
copyright notices are retained in all copies and that this notice is included
verbatim in any distributions. Additionally, the authors grant permission to
modify this software and its documentation for any purpose, provided that
such modifications are not distributed without the explicit consent of the
authors and that existing copyright notices are retained in all copies. Some
of the algorithms implemented by this software are patented, observe all
applicable patent law.

IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF,
EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON AN
"AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.


=========================================================================*/
// .NAME vtkXYPlotActor - generate an x-y plot from input dataset(s)
// .SECTION Description
// vtkXYPlotActor creates an x-y plot of data from one or more input data sets.
// The class plots the scalar values (y-axis) of the data set against the 
// points (x-axis). The x-axis values are generated by taking the point ids,
// computing a cumulative arc length, or a normalized arc length. More than
// one input data set can be specified to generate multiple plots.
//
// To use this class, you must specify an input data set containing scalar
// and point data, and define it's location in the viewport. You'll probably
// also want to invoke a method to control how the point coordinates are
// converted into x values (by default point ids are used). The
// PositionCoordinate defines the lower-left location of the x-y plot
// (specified in normalized viewport coordinates) and the Position2Coordinate
// define the upper-right corner. (Note: the Position2Coordinate is relative
// to PositionCoordinate, so you can move the vtkXYPlotActor around the
// viewport by setting just the PositionCoordinate.) The combination of the two
// position coordinates specifies a rectangle in which the plot will lie.
//
// Optional features include the ability to specify axes labels, label
// format, plot title, and control font properties and type. You can also
// manually specify the x and y plot ranges (by default they are computed
// automatically). The Border instance variable is used to create space 
// between the boundary of the plot window (specified by PositionCoordinate
// and Position2Coordinate) and the plot itself.

// .SECTION Caveats
// If you are interested in plotting something other than scalar data, you
// can use the vtk data shuffling filters (e.g., 
// vtkAttributeDataToFieldDataFilter snd vtkFieldDataToAttributeDataFilter) 
// to convert the data into scalar data and/or points.

// .SECTION See Also
// vtkActor2D vtkTextMapper vtkScalarBarActor vtkAxisActor2D vtkCubeAxesActor
// vtkAttributeDataToFieldDataFilter vtkFieldDataToAttributeDataFilter

#ifndef __vtkXYPlotActor_h
#define __vtkXYPlotActor_h

#define VTK_XYPLOT_INDEX                 0
#define VTK_XYPLOT_ARC_LENGTH            1
#define VTK_XYPLOT_NORMALIZED_ARC_LENGTH 2

#include "vtkAxisActor2D.h"
class vtkDataSetCollection;

class VTK_EXPORT vtkXYPlotActor : public vtkActor2D
{
public:
  const char *GetClassName() {return "vtkXYPlotActor";};
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Instantiate object with autorange computation; bold, italic, and shadows
  // on; arial font family; the number of labels set to 5 for the x and y
  // axes; a label format of "%-#6.3g"; and x coordinates computed from point
  // ids.
  static vtkXYPlotActor *New() {return new vtkXYPlotActor;};
  
  // Description: Access the Position2 instance variable. This variable
  // controls the upper right corner of the x-y plot. It is by default
  // relative to (the superclasses') Position and in normalized viewport
  // coordinates.
  vtkViewportCoordinateMacro(Position2);
  
  // Description:
  // Draw the x-y plot.
  int RenderOpaqueGeometry(vtkViewport*);
  int RenderOverlay(vtkViewport*);
  int RenderTranslucentGeometry(vtkViewport *viewport) {return 0;}

  // Description:
  // Add a dataset to the list of data to append.
  void AddInput(vtkDataSet *in);

  // Description:
  // Remove a dataset from the list of data to append.
  void RemoveInput(vtkDataSet *in);

  // Description:
  // Return the list of inputs to this filter.
  vtkDataSetCollection *GetInputList() {return this->InputList;}

  // Description:
  // Set/Get the height and width of the x-y plot. The value is expressed
  // as a fraction of the viewport. This really is just another way of
  // setting the Position2 instance variable.
  void SetWidth(float w);
  float GetWidth();
  void SetHeight(float h);
  float GetHeight();
  
  // Description:
  // Set/Get the title of the x-y plot, and the title along the 
  // x and y axes.
  vtkSetStringMacro(Title);
  vtkGetStringMacro(Title);
  vtkSetStringMacro(XTitle);
  vtkGetStringMacro(XTitle);
  vtkSetStringMacro(YTitle);
  vtkGetStringMacro(YTitle);

  // Description:
  // Set the plot range (range of independent and dependent variables)
  // to plot. Data outside of the range will be clipped. If the plot
  // range of either the x or y variables is set to (v1,v2), where
  // v1 == v2, then the range will be computed automatically. Note that
  // the x-range values should be consistent with the way the independent
  // variable is created (via INDEX, DISTANCE, or ARC_LENGTH).
  vtkSetVector2Macro(XRange,float);
  vtkGetVectorMacro(XRange,float,2);
  vtkSetVector2Macro(YRange,float);
  vtkGetVectorMacro(YRange,float,2);
  void SetPlotRange(float xmin, float ymin, float xmax, float ymax)
    {this->SetXRange(xmin,xmax); this->SetYRange(ymin,ymax);}
  
  // Description:
  // Specify how the independent (x) variable is computed from the points.
  // The independent variable can be the scalar/point index (i.e., 
  // point id), the accumulated arc length along the points, or the
  // normalized arc length.
  vtkSetClampMacro(XValues,int,
		   VTK_XYPLOT_INDEX,VTK_XYPLOT_NORMALIZED_ARC_LENGTH);
  vtkGetMacro(XValues,int);
  void SetXValuesToIndex(){this->SetXValues(VTK_XYPLOT_INDEX);};
  void SetXValuesToArcLength() {this->SetXValues(VTK_XYPLOT_ARC_LENGTH);};
  void SetXValuesToNormalizedArcLength()
    {this->SetXValues(VTK_XYPLOT_NORMALIZED_ARC_LENGTH);};
  char *GetXValuesAsString();

  // Description:
  // Set/Get the number of annotation labels to show along the x and y axes.
  // This values is a suggestion: the number of labels may vary depending
  // on the particulars of the data. The convenience method 
  // SetNumberOfLables() sets the number of x and y labels to the same value.
  vtkSetClampMacro(NumberOfXLabels, int, 0, 50);
  vtkGetMacro(NumberOfXLabels, int);
  vtkSetClampMacro(NumberOfYLabels, int, 0, 50);
  vtkGetMacro(NumberOfYLabels, int);
  void SetNumberOfLabels(int num)
    {this->SetNumberOfXLabels(num); this->SetNumberOfYLabels(num);}
  
  // Description:
  // Enable/Disable bolding annotation text.
  vtkSetMacro(Bold, int);
  vtkGetMacro(Bold, int);
  vtkBooleanMacro(Bold, int);

  // Description:
  // Enable/Disable italicizing annotation text.
  vtkSetMacro(Italic, int);
  vtkGetMacro(Italic, int);
  vtkBooleanMacro(Italic, int);

  // Description:
  // Enable/Disable creating shadows on the annotation text. Shadows make 
  // the text easier to read.
  vtkSetMacro(Shadow, int);
  vtkGetMacro(Shadow, int);
  vtkBooleanMacro(Shadow, int);

  // Description:
  // Set/Get the font family for the annotation text. Three font types 
  // are available: Arial (VTK_ARIAL), Courier (VTK_COURIER), and 
  // Times (VTK_TIMES).
  vtkSetMacro(FontFamily, int);
  vtkGetMacro(FontFamily, int);
  void SetFontFamilyToArial() {this->SetFontFamily(VTK_ARIAL);};
  void SetFontFamilyToCourier() {this->SetFontFamily(VTK_COURIER);};
  void SetFontFamilyToTimes() {this->SetFontFamily(VTK_TIMES);};

  // Description:
  // Set/Get the format with which to print the labels on the scalar
  // bar.
  vtkSetStringMacro(LabelFormat);
  vtkGetStringMacro(LabelFormat);

  // Description:
  // Set/Get the spacing between the plot window and the plot. The value
  // is specified in pixels.
  vtkSetClampMacro(Border, int, 0, 50);
  vtkGetMacro(Border, int);

  // Description:
  // Release any graphics resources that are being consumed by this actor.
  // The parameter window could be used to determine which graphic
  // resources to release.
  void ReleaseGraphicsResources(vtkWindow *);

protected:
  vtkXYPlotActor();
  ~vtkXYPlotActor();

  vtkCoordinate *Position2Coordinate;
  vtkDataSetCollection *InputList; //list of data sets to plot
  char  *Title;
  char  *XTitle;
  char  *YTitle;
  int   XValues;
  int   NumberOfXLabels;
  int   NumberOfYLabels;
  int	Bold;
  int   Italic;
  int   Shadow;
  int   FontFamily;
  char  *LabelFormat;
  float XRange[2];
  float YRange[2];
  int Border;
  
  vtkTextMapper *TitleMapper;
  vtkActor2D    *TitleActor;

  vtkAxisActor2D      *XAxis;
  vtkAxisActor2D      *YAxis;

  vtkPolyData         *PlotData; //the data drawn within the axes
  vtkPolyDataMapper2D *PlotMapper;
  vtkActor2D          *PlotActor;
  
  vtkTimeStamp  BuildTime;

  void ComputeXRange(float range[2], float *lengths);
  void ComputeYRange(float range[2]);
  virtual void CreatePlotData(int *pos, int *pos2, float xRange[2], 
			      float yRange[2], float *norms);
  void PlaceAxes(vtkViewport *viewport, int *size, int pos[2], int pos2[2]);
  
};


#endif

