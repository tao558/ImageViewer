/*=========================================================================

Library:   TubeTK

Copyright Kitware Inc.

All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/
#ifndef __TextBoxWidget_h
#define __TextBoxWidget_h

#include <QTextEdit>
#include <QString>

#include "itkPoint.h"
#include <QOpenGLWidget>
#include "QtGlSliceView.h"

#include <vector>

class QtGlSliceView; // cross-referencing due to the callbacks for the OpenGL painting

//TODO: Remember comments
struct TextBoxToolMetaData {
  /**
  * Monotonically increasing ID.
  */
  int sortId;

  /**
  * Default text
  */
  std::string defaultText;

};

/**
* Functor for generating new meta data for newly created boxes.  Allows for rudimentary workflow.
*/
class TextBoxMetaDataGenerator {
public:
    TextBoxMetaDataGenerator(const std::string& defaultText="");
    std::unique_ptr< TextBoxToolMetaData > operator()(void);
protected:
    int curId = 0;
    std::string defaultText;
};

/**
* Factory class for meta data for new text boxes.  Uses MetaDataGenerators.  Used to "refund" deleted text boxes, i.e. if a user deletes
* a text box the next text box they draw has the deleted meta data.
*/
class TextBoxToolMetaDataFactory {
public:
    TextBoxToolMetaDataFactory(std::unique_ptr< TextBoxMetaDataGenerator > generator);

    /**
    * Get the appropriate meta data for the next text box.  Recycles deleted meta data.
    */
    std::unique_ptr< TextBoxToolMetaData > getNext();

    /**
    * Return a deleted meta data for reuse.
    */
    void refund(std::unique_ptr< TextBoxToolMetaData > box_meta);


protected:
    std::unique_ptr< TextBoxMetaDataGenerator > generator;
    std::vector< std::unique_ptr< TextBoxToolMetaData > > refunds;
};

class TextBoxTool
{
public:

  using PointType3D = itk::Point<double, 3>;
  using PointType2D = itk::Point<double, 3>;

  TextBoxTool() = delete;

  /**
  * \param parent QtGlSliceView
  * \param text The text for the textbox
  * \param metaData metaData for the box
  */
  TextBoxTool(QtGlSliceView* parent, std::unique_ptr< TextBoxToolMetaData > metaData);

  virtual ~TextBoxTool();

  std::unique_ptr< TextBoxToolMetaData > metaData;

  void paint();

  void hide();
protected:
    QtGlSliceView* parent;


private:
  PointType3D points[2];
  PointType3D indices[2];
  QTextEdit textEdit;
};

#endif