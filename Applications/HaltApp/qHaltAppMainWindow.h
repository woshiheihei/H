/*==============================================================================

  Copyright (c) Kitware, Inc.

  See http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Julien Finet, Kitware, Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qHaltAppMainWindow_h
#define __qHaltAppMainWindow_h

// Halt includes
#include "qHaltAppExport.h"
class qHaltAppMainWindowPrivate;

// Slicer includes
#include "qSlicerMainWindow.h"

class Q_HALT_APP_EXPORT qHaltAppMainWindow : public qSlicerMainWindow
{
  Q_OBJECT
public:
  typedef qSlicerMainWindow Superclass;

  qHaltAppMainWindow(QWidget *parent=0);
  virtual ~qHaltAppMainWindow();

public slots:
  void on_HelpAboutHaltAppAction_triggered();

protected:
  qHaltAppMainWindow(qHaltAppMainWindowPrivate* pimpl, QWidget* parent);

private:
  Q_DECLARE_PRIVATE(qHaltAppMainWindow);
  Q_DISABLE_COPY(qHaltAppMainWindow);
};

#endif
