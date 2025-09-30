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

// Qt includes
#include <QDebug>
#include <QLinearGradient>
#include <QMenuBar>
#include <QPainter>
#include <QPalette>
#include <QPushButton>
#include <QStyleFactory>
#include <QStyleOption>
#include <QToolBar>

// CTK includes
#include <ctkCollapsibleButton.h>

// Halt includes
#include "qAppStyle.h"

// --------------------------------------------------------------------------
// qAppStyle methods

// --------------------------------------------------------------------------
qAppStyle::qAppStyle()
{
  // Slicer uses a QCleanlooksStyle as base style.
  this->setBaseStyle(new QProxyStyle(QStyleFactory::create("fusion")));
}

// --------------------------------------------------------------------------
qAppStyle::~qAppStyle()
{
}

//------------------------------------------------------------------------------
QPalette qAppStyle::standardPalette()const
{
  QPalette palette = this->Superclass::standardPalette();

  // Claude风格现代简约配色方案
  // 主背景 - 温暖的奶白色，提供舒适的视觉体验
  palette.setColor(QPalette::Active, QPalette::Window, "#fefefe");
  palette.setColor(QPalette::Inactive, QPalette::Window, "#fefefe");
  palette.setColor(QPalette::Disabled, QPalette::Window, "#f8f9fa");
  
  // 主文本 - 深邃的炭黑色，确保优秀的可读性
  palette.setColor(QPalette::Active, QPalette::WindowText, "#1a1a1a");
  palette.setColor(QPalette::Inactive, QPalette::WindowText, "#1a1a1a");
  palette.setColor(QPalette::Disabled, QPalette::WindowText, "#6b7280");
  
  // 输入框文本
  palette.setColor(QPalette::Active, QPalette::Text, "#1a1a1a");
  palette.setColor(QPalette::Inactive, QPalette::Text, "#1a1a1a");
  palette.setColor(QPalette::Disabled, QPalette::Text, "#9ca3af");
  
  // 输入框背景 - 纯净白色
  palette.setColor(QPalette::Active, QPalette::Base, "#ffffff");
  palette.setColor(QPalette::Inactive, QPalette::Base, "#ffffff");
  palette.setColor(QPalette::Disabled, QPalette::Base, "#f9fafb");

  // 辅助颜色
  palette.setColor(QPalette::Light, "#ffffff");
  palette.setColor(QPalette::Button, "#f3f4f6");  // 淡灰色按钮背景
  palette.setColor(QPalette::Mid, "#d97706");     // Claude风格橙色强调色
  palette.setColor(QPalette::Dark, "#d97706");    // Claude品牌橙色
  
  // 按钮文本 - Claude品牌橙色
  palette.setColor(QPalette::Active, QPalette::ButtonText, "#d97706");
  palette.setColor(QPalette::Inactive, QPalette::ButtonText, "#d97706");
  palette.setColor(QPalette::Disabled, QPalette::ButtonText, "#9ca3af");
  
  // 阴影色 - 柔和的灰色
  palette.setColor(QPalette::Shadow, "#374151");

  // 高亮选择 - Claude风格温暖橙色
  palette.setColor(QPalette::Highlight, "#fb923c");
  palette.setColor(QPalette::HighlightedText, "#ffffff");

  return palette;
}

//------------------------------------------------------------------------------
void qAppStyle::drawComplexControl(ComplexControl control,
                                   const QStyleOptionComplex* option,
                                   QPainter* painter,
                                   const QWidget* widget )const
{
  const_cast<QStyleOptionComplex*>(option)->palette =
    this->tweakWidgetPalette(option->palette, widget);
  this->Superclass::drawComplexControl(control, option, painter, widget);
}

//------------------------------------------------------------------------------
void qAppStyle::drawControl(ControlElement element,
                            const QStyleOption* option,
                            QPainter* painter,
                            const QWidget* widget )const
{
  const_cast<QStyleOption*>(option)->palette =
    this->tweakWidgetPalette(option->palette, widget);

  // For some reason the toolbar paint routine is not respecting the palette.
  // here we make sure the background is correctly drawn.
  if (element == QStyle::CE_ToolBar &&
      qobject_cast<const QToolBar*>(widget))
    {
    painter->fillRect(option->rect, option->palette.brush(QPalette::Window));
    }
  this->Superclass::drawControl(element, option, painter, widget);
}

//------------------------------------------------------------------------------
void qAppStyle::drawPrimitive(PrimitiveElement element,
                              const QStyleOption* option,
                              QPainter* painter,
                              const QWidget* widget )const
{
  const_cast<QStyleOption*>(option)->palette =
    this->tweakWidgetPalette(option->palette, widget);
  this->Superclass::drawPrimitive(element, option, painter, widget);
}

//------------------------------------------------------------------------------
QPalette qAppStyle::tweakWidgetPalette(QPalette widgetPalette,
                                       const QWidget* widget)const
{
  if (!widget)
    {
    return widgetPalette;
    }
  const QPushButton* pushButton =
    qobject_cast<const QPushButton*>(widget);
  if (pushButton &&
      !pushButton->text().isEmpty())
    {
    QColor buttonColor = this->standardPalette().color(QPalette::Dark);
    widgetPalette.setColor(QPalette::Active, QPalette::Button, buttonColor);
    widgetPalette.setColor(QPalette::Inactive, QPalette::Button, buttonColor);
    QColor disabledButtonColor = buttonColor.toHsv();
    disabledButtonColor.setHsvF(disabledButtonColor.hueF(),
                                disabledButtonColor.saturationF() * 0.8,
                                disabledButtonColor.valueF() * 0.9);
    widgetPalette.setColor(QPalette::Disabled, QPalette::Button, disabledButtonColor);
    QColor buttonTextColor =
      this->standardPalette().color(QPalette::Light);
    widgetPalette.setColor(QPalette::Active, QPalette::ButtonText, buttonTextColor);
    widgetPalette.setColor(QPalette::Inactive, QPalette::ButtonText, buttonTextColor);
    QColor disabledButtonTextColor = buttonTextColor.toHsv();
    disabledButtonTextColor.setHsvF(buttonColor.hueF(),
                                    disabledButtonTextColor.saturationF() * 0.3,
                                    disabledButtonTextColor.valueF() * 0.8);
    widgetPalette.setColor(QPalette::Disabled, QPalette::ButtonText, disabledButtonColor);
    }
  if (qobject_cast<const QMenuBar*>(widget))
    {
    QColor highlightColor = this->standardPalette().color(QPalette::Dark);
    //QBrush highlightBrush = this->standardPalette().brush(QPalette::Dark);
    QColor highlightTextColor =
      this->standardPalette().color(QPalette::Light);
    QBrush highlightTextBrush =
      this->standardPalette().brush(QPalette::Light);
    QColor darkColor = this->standardPalette().color(QPalette::Highlight);
    QColor lightColor =
      this->standardPalette().color(QPalette::HighlightedText);

    QLinearGradient hilightGradient(0., 0., 0., 1.);
    hilightGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    hilightGradient.setColorAt(0., highlightColor);
    hilightGradient.setColorAt(1., highlightColor.darker(120));
    QBrush highlightBrush(hilightGradient);

    widgetPalette.setColor(QPalette::Highlight, darkColor);
    widgetPalette.setColor(QPalette::HighlightedText, lightColor);

    widgetPalette.setColor(QPalette::Window, highlightColor);
    widgetPalette.setColor(QPalette::WindowText, highlightTextColor);
    widgetPalette.setColor(QPalette::Base, highlightColor);
    widgetPalette.setColor(QPalette::Text, highlightTextColor);
    widgetPalette.setColor(QPalette::Button, highlightColor);
    widgetPalette.setColor(QPalette::ButtonText, highlightTextColor);

    widgetPalette.setBrush(QPalette::Window, highlightBrush);
    widgetPalette.setBrush(QPalette::WindowText, highlightTextBrush);
    widgetPalette.setBrush(QPalette::Base, highlightBrush);
    widgetPalette.setBrush(QPalette::Text, highlightTextBrush);
    widgetPalette.setBrush(QPalette::Button, highlightBrush);
    widgetPalette.setBrush(QPalette::ButtonText, highlightTextBrush);
    }
/*
  QWidget* parentWidget = widget->parentWidget();
  QWidget* grandParentWidget = parentWidget? parentWidget->parentWidget() : 0;
  if (qobject_cast<const QToolBar*>(widget) ||
      qobject_cast<QToolBar*>(parentWidget) ||
      qobject_cast<QToolBar*>(grandParentWidget))
    {
    QColor windowColor = this->standardPalette().color(QPalette::Window);

    //QColor highlightColor = this->standardPalette().color(QPalette::Highlight);
    //QColor highlightTextColor =
    //  this->standardPalette().color(QPalette::HighlightedText);
    //QColor darkColor = this->standardPalette().color(QPalette::Dark);
    //QColor lightColor =
    //  this->standardPalette().color(QPalette::Light);
    QColor highlightColor = this->standardPalette().color(QPalette::Dark);
    //QBrush highlightBrush = this->standardPalette().brush(QPalette::Dark);
    QBrush highlightTextBrush =
      this->standardPalette().brush(QPalette::Light);
    QColor darkColor = this->standardPalette().color(QPalette::Highlight);
    QColor lightColor =
      this->standardPalette().color(QPalette::HighlightedText);

    QLinearGradient hilightGradient(0., 0., 0., 1.);
    hilightGradient.setCoordinateMode(QGradient::ObjectBoundingMode);

    hilightGradient.setColorAt(0., highlightColor);
    hilightGradient.setColorAt(1., highlightColor.darker(140));
    QBrush highlightBrush(hilightGradient);

    widgetPalette.setColor(QPalette::Highlight, darkColor);
    widgetPalette.setColor(QPalette::HighlightedText, lightColor);
    widgetPalette.setBrush(QPalette::Window, highlightBrush);
    widgetPalette.setBrush(QPalette::WindowText, highlightTextBrush);
    widgetPalette.setBrush(QPalette::Base, highlightBrush);
    widgetPalette.setBrush(QPalette::Text, highlightTextBrush);
    widgetPalette.setBrush(QPalette::Button, highlightBrush);
    widgetPalette.setBrush(QPalette::ButtonText, highlightTextBrush);
    }
*/
  return widgetPalette;
}

//------------------------------------------------------------------------------
void qAppStyle::polish(QWidget* widget)
{
  this->Superclass::polish(widget);
  ctkCollapsibleButton* collapsibleButton =
    qobject_cast<ctkCollapsibleButton*>(widget);
  if (collapsibleButton)
    {
    collapsibleButton->setFlat(true);
    collapsibleButton->setContentsFrameShadow(QFrame::Sunken);
    }
}
