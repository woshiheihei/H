/*==============================================================================

  Copyright (c) Kitware, Inc.

  See http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware, Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// Halt includes
#include "qHaltAppMainWindow.h"
#include "Widgets/qAppStyle.h"

// Slicer includes
#include "qSlicerApplication.h"
#include "qSlicerApplicationHelper.h"
#include "vtkSlicerConfigure.h" // For Slicer_MAIN_PROJECT_APPLICATION_NAME
#include "vtkSlicerVersionConfigure.h" // For Slicer_MAIN_PROJECT_VERSION_FULL
// Qt includes (fonts)
#include <QFont>
#include <QFontDatabase>

namespace
{

//----------------------------------------------------------------------------
int SlicerAppMain(int argc, char* argv[])
{
  typedef qHaltAppMainWindow SlicerMainWindowType;

  qSlicerApplicationHelper::preInitializeApplication(argv[0], new qAppStyle);

  qSlicerApplication app(argc, argv);
  if (app.returnCode() != -1)
    {
    return app.returnCode();
    }

#ifdef Q_OS_WIN
  // Prefer Microsoft YaHei for better CJK rendering on Windows.
  // Keep the system default point size, only change the family.
  {
    const QStringList yaheiCandidates = {
      QString::fromLatin1("Microsoft YaHei UI"),
      QString::fromLatin1("Microsoft YaHei"),
      // Some systems expose the localized family name
      QString::fromUtf8("微软雅黑")
    };

    QString chosenFamily;
    const QStringList families = QFontDatabase().families();
    for (const QString& candidate : yaheiCandidates)
      {
      if (families.contains(candidate, Qt::CaseInsensitive))
        {
        chosenFamily = candidate;
        break;
        }
      }

    if (!chosenFamily.isEmpty())
      {
      QFont f = app.font(); // preserve size/weight
      f.setFamily(chosenFamily);
      // Prefer anti-aliased glyphs for UI fonts
      f.setStyleStrategy(QFont::PreferAntialias);
      app.setFont(f);
      // Help resolve generic families to YaHei where possible
      QFont::insertSubstitution(QString::fromLatin1("Sans Serif"), chosenFamily);
      QFont::insertSubstitution(QString::fromLatin1("MS Shell Dlg 2"), chosenFamily);
      }
  }
#endif

  QScopedPointer<SlicerMainWindowType> window;
  QScopedPointer<QSplashScreen> splashScreen;

  qSlicerApplicationHelper::postInitializeApplication<SlicerMainWindowType>(
        app, splashScreen, window);

  if (!window.isNull())
    {
    QString windowTitle = QString("%1 %2").arg(Slicer_MAIN_PROJECT_APPLICATION_DISPLAY_NAME).arg(Slicer_MAIN_PROJECT_VERSION_FULL);
    window->setWindowTitle(windowTitle);
    }

  return app.exec();
}

} // end of anonymous namespace

#include "qSlicerApplicationMainWrapper.cxx"
