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

// Halt includes
#include "qHaltAppMainWindow.h"
#include "qHaltAppMainWindow_p.h"

// Qt includes
#include <QDesktopWidget>
#include <QLabel>
#include <QStatusBar>
#include <QToolBar>
#include <QDockWidget>

// STL includes
#include <vector>

// VTK includes
#include <vtkMRMLNode.h>
#include <vtkMRMLScene.h>
#include <vtkMRMLSliceNode.h>
#include <vtkMRMLAbstractViewNode.h>
#include <vtkMRMLViewNode.h>
// Slicer includes
#include "qSlicerApplication.h"
#include "qSlicerAboutDialog.h"
#include "qSlicerMainWindow_p.h"
#include "qSlicerModuleSelectorToolBar.h"
#include "qMRMLWidget.h"

//-----------------------------------------------------------------------------
// qHaltAppMainWindowPrivate methods

qHaltAppMainWindowPrivate::qHaltAppMainWindowPrivate(qHaltAppMainWindow& object)
  : Superclass(object)
{
}

//-----------------------------------------------------------------------------
qHaltAppMainWindowPrivate::~qHaltAppMainWindowPrivate()
{
}

//-----------------------------------------------------------------------------
void qHaltAppMainWindowPrivate::init()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 7, 0))
  QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
  Q_Q(qHaltAppMainWindow);
  this->Superclass::init();
}

//-----------------------------------------------------------------------------
void qHaltAppMainWindowPrivate::setupUi(QMainWindow * mainWindow)
{
  qSlicerApplication * app = qSlicerApplication::application();

  //----------------------------------------------------------------------------
  // Add actions
  //----------------------------------------------------------------------------
  QAction* helpAboutSlicerAppAction = new QAction(mainWindow);
  helpAboutSlicerAppAction->setObjectName("HelpAboutHaltAppAction");
  helpAboutSlicerAppAction->setText(qHaltAppMainWindow::tr("About %1").arg(qSlicerApplication::application()->mainApplicationDisplayName()));

  //----------------------------------------------------------------------------
  // Calling "setupUi()" after adding the actions above allows the call
  // to "QMetaObject::connectSlotsByName()" done in "setupUi()" to
  // successfully connect each slot with its corresponding action.
  this->Superclass::setupUi(mainWindow);

  // Add Help Menu Action
  this->HelpMenu->addAction(helpAboutSlicerAppAction);

  //----------------------------------------------------------------------------
  // Configure
  //----------------------------------------------------------------------------
  mainWindow->setWindowIcon(QIcon(":/Icons/Medium/DesktopIcon.png"));

  QLabel* logoLabel = new QLabel();
  logoLabel->setObjectName("LogoLabel");
  
  // Load logo with high DPI support for better clarity
  QIcon logoIcon(":/HALT.png");
  // Get the largest available size for best quality
  QSize iconSize = logoIcon.availableSizes().isEmpty() ? QSize(938, 386) : logoIcon.availableSizes().last();
  QPixmap logoPixmap = logoIcon.pixmap(iconSize);
  
  // Scale to appropriate size for title bar (max height 80px, maintain aspect ratio)
  int maxHeight = 80;
  if (logoPixmap.height() > maxHeight)
  {
    // Use Qt::SmoothTransformation for high quality scaling
    logoPixmap = logoPixmap.scaledToHeight(maxHeight, Qt::SmoothTransformation);
  }
  
  // Set device pixel ratio for high DPI displays
  #if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
  logoPixmap.setDevicePixelRatio(qApp->devicePixelRatio());
  #endif
  
  logoLabel->setPixmap(logoPixmap);
  logoLabel->setScaledContents(false);
  logoLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  // Set margins: left, top, right, bottom (no bottom margin)
  logoLabel->setContentsMargins(8, 8, 8, 8);
  logoLabel->setStyleSheet("QLabel { padding: 0px; }");
  this->PanelDockWidget->setTitleBarWidget(logoLabel);

  // Hide the menus
  this->menubar->setVisible(false);
  this->FileMenu->setVisible(false);
  this->EditMenu->setVisible(false);
  this->ViewMenu->setVisible(false);
  this->LayoutMenu->setVisible(false);
  this->HelpMenu->setVisible(false);

  //----------------------------------------------------------------------------
  // Hide additional UI elements (similar to Python setSlicerUIVisible(False))
  //----------------------------------------------------------------------------
  
  // Hide Data Probe (bottom status bar area)
  QWidget* dataProbeWidget = mainWindow->findChild<QWidget*>("DataProbeCollapsibleWidget");
  if (dataProbeWidget)
  {
    dataProbeWidget->setVisible(false);
  }

  // Hide status bar
  if (mainWindow->statusBar())
  {
    mainWindow->statusBar()->setVisible(false);
  }

  // Hide module panel title
  QWidget* modulePanelTitle = mainWindow->findChild<QWidget*>("ModulePanelDockWidgetTitleBar");
  if (modulePanelTitle)
  {
    modulePanelTitle->setVisible(false);
  }

  // Hide module help section
  QWidget* moduleHelp = mainWindow->findChild<QWidget*>("ModuleHelpCollapsibleWidget");
  if (moduleHelp)
  {
    moduleHelp->setVisible(false);
  }

  // Hide Python console
  QWidget* pythonConsole = mainWindow->findChild<QWidget*>("PythonConsoleWidget");
  if (pythonConsole)
  {
    pythonConsole->setVisible(false);
  }
  QDockWidget* pythonConsoleDock = mainWindow->findChild<QDockWidget*>("PythonConsoleDockWidget");
  if (pythonConsoleDock)
  {
    pythonConsoleDock->setVisible(false);
  }

  // Hide application logo (except our custom one)
  QLabel* appLogo = mainWindow->findChild<QLabel*>("LogoLabel");
  if (appLogo && appLogo != logoLabel)
  {
    appLogo->setVisible(false);
  }

  // Hide toolbars except important ones for medical imaging
  // Keep visible: MainToolBar, ViewToolBar, MouseModeToolBar (crosshair/window-level)
  QList<QToolBar*> toolbars = mainWindow->findChildren<QToolBar*>();
  QStringList keepToolbars;
  keepToolbars << "ViewToolBar" << "MouseModeToolBar";
  
  foreach (QToolBar* toolbar, toolbars)
  {
    QString toolbarName = toolbar->objectName();
    // Hide toolbars not in the keep list
    if (!keepToolbars.contains(toolbarName))
    {
      toolbar->setVisible(false);
    }
  }

  // Hide module selector toolbar
  qSlicerModuleSelectorToolBar* moduleSelectorToolBar = 
    mainWindow->findChild<qSlicerModuleSelectorToolBar*>();
  if (moduleSelectorToolBar)
  {
    moduleSelectorToolBar->setVisible(false);
  }

  // Remove Load/Save toolbar toggle from the UI (hide MainToolBar entirely)
  QToolBar* loadSaveToolBar = mainWindow->findChild<QToolBar*>("MainToolBar");
  if (loadSaveToolBar)
  {
    loadSaveToolBar->setVisible(false);
    if (QAction* toggleAction = loadSaveToolBar->toggleViewAction())
    {
      toggleAction->setEnabled(false);
      toggleAction->setVisible(false);
    }
  }

  //----------------------------------------------------------------------------
  // Set 3D view background color to black
  //----------------------------------------------------------------------------
  vtkMRMLScene* scene = app ? app->mrmlScene() : nullptr;
  if (scene)
  {
    std::vector<vtkMRMLNode*> viewNodes;
    scene->GetNodesByClass("vtkMRMLViewNode", viewNodes);

    for (vtkMRMLNode* node : viewNodes)
    {
      vtkMRMLViewNode* viewNode = vtkMRMLViewNode::SafeDownCast(node);
      if (!viewNode)
      {
        continue;
      }
      viewNode->SetBackgroundColor(0.0, 0.0, 0.0);
      viewNode->SetBackgroundColor2(0.0, 0.0, 0.0);
      // Set orientation marker to human type
      viewNode->SetOrientationMarkerType(vtkMRMLAbstractViewNode::OrientationMarkerTypeHuman);
      viewNode->SetOrientationMarkerSize(vtkMRMLAbstractViewNode::OrientationMarkerSizeMedium);
      // Hide bounding box and axis labels
      viewNode->SetBoxVisible(0);
      viewNode->SetAxisLabelsVisible(0);
    }    std::vector<vtkMRMLNode*> sliceNodes;
    scene->GetNodesByClass("vtkMRMLSliceNode", sliceNodes);

    for (vtkMRMLNode* node : sliceNodes)
    {
      vtkMRMLSliceNode* sliceNode = vtkMRMLSliceNode::SafeDownCast(node);
      if (!sliceNode)
      {
        continue;
      }
      sliceNode->SetOrientationMarkerType(vtkMRMLAbstractViewNode::OrientationMarkerTypeHuman);
      sliceNode->SetOrientationMarkerSize(vtkMRMLAbstractViewNode::OrientationMarkerSizeMedium);
    }
  }
}

//-----------------------------------------------------------------------------
// qHaltAppMainWindow methods

//-----------------------------------------------------------------------------
qHaltAppMainWindow::qHaltAppMainWindow(QWidget* windowParent)
  : Superclass(new qHaltAppMainWindowPrivate(*this), windowParent)
{
  Q_D(qHaltAppMainWindow);
  d->init();
}

//-----------------------------------------------------------------------------
qHaltAppMainWindow::qHaltAppMainWindow(
  qHaltAppMainWindowPrivate* pimpl, QWidget* windowParent)
  : Superclass(pimpl, windowParent)
{
  // init() is called by derived class.
}

//-----------------------------------------------------------------------------
qHaltAppMainWindow::~qHaltAppMainWindow()
{
}

//-----------------------------------------------------------------------------
void qHaltAppMainWindow::on_HelpAboutHaltAppAction_triggered()
{
  qSlicerAboutDialog about(this);
  about.setLogo(QPixmap(":/Logo.png"));
  about.exec();
}
