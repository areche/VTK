/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkSampleFunction.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


Copyright (c) 1993-1998 Ken Martin, Will Schroeder, Bill Lorensen.

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
// .NAME vtkSampleFunction - sample an implicit function over a structured point set
// .SECTION Description
// vtkSampleFunction is a source object that evaluates an implicit function
// and normals at each point in a vtkStructuredPoints. The user can specify
// the sample dimensions and location in space to perform the sampling. To
// create closed surfaces (in conjunction with the vtkContourFilter), capping
// can be turned on to set a particular value on the boundaries of the sample
// space.

// .SECTION See Also
// vtkImplicitModeller

#ifndef __vtkSampleFunction_h
#define __vtkSampleFunction_h

#include "vtkStructuredPointsSource.h"
#include "vtkImplicitFunction.h"

class VTK_EXPORT vtkSampleFunction : public vtkStructuredPointsSource
{
public:
  const char *GetClassName() {return "vtkSampleFunction";};
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Construct with ModelBounds=(-1,1,-1,1,-1,1), SampleDimensions=(50,50,50),
  // Capping turned off, and normal generation on.
  static vtkSampleFunction *New() {return new vtkSampleFunction;};

  // Description:
  // Specify the implicit function to use to generate data.
  vtkSetObjectMacro(ImplicitFunction,vtkImplicitFunction);
  vtkGetObjectMacro(ImplicitFunction,vtkImplicitFunction);

  // Description:
  // Control the type of the vtkScalars object by explicitly providing a scalar
  // object.  vtkSampleFunction() will allocate space (as necessary)
  // in the scalar object.
  vtkSetObjectMacro(Scalars,vtkScalars);

  // Description:
  // Specify the dimensions of the data on which to sample.
  void SetSampleDimensions(int i, int j, int k);

  // Description:
  // Specify the dimensions of the data on which to sample.
  void SetSampleDimensions(int dim[3]);
  vtkGetVectorMacro(SampleDimensions,int,3);

  // Description:
  // Specify the region in space over which the sampling occurs.
  vtkSetVector6Macro(ModelBounds,float);
  vtkGetVectorMacro(ModelBounds,float,6);

  // Description:
  // Turn on/off capping. If capping is on, then the outer boundaries of the
  // structured point set are set to cap value. This can be used to insure
  // surfaces are closed.
  vtkSetMacro(Capping,int);
  vtkGetMacro(Capping,int);
  vtkBooleanMacro(Capping,int);
  
  // Description:
  // Set the cap value.
  vtkSetMacro(CapValue,float);
  vtkGetMacro(CapValue,float);

  // Description:
  // Turn on/off the computation of normals.
  vtkSetMacro(ComputeNormals,int);
  vtkGetMacro(ComputeNormals,int);
  vtkBooleanMacro(ComputeNormals,int);

  // Description:
  // Return the MTime also considering the implicit function.
  unsigned long GetMTime();

protected:
  vtkSampleFunction();
  ~vtkSampleFunction();
  void Execute();
  void ExecuteInformation();
  void Cap(vtkScalars *s);

  int SampleDimensions[3];
  float ModelBounds[6];
  vtkScalars *Scalars;
  int Capping;
  float CapValue;
  vtkImplicitFunction *ImplicitFunction;
  int ComputeNormals;
};

#endif


