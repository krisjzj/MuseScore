//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2019 Werner Schweer and others
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//=============================================================================

#include "qmldockwidget.h"

#include "musescore.h"

namespace Ms {

//---------------------------------------------------------
//   FocusChainBreak
//---------------------------------------------------------

FocusChainBreak::FocusChainBreak(QQuickItem* parent)
   : QQuickItem(parent)
      {
      setActiveFocusOnTab(true);
      setFocus(true);
      }

//---------------------------------------------------------
//   FocusChainBreak::focusInEvent
//---------------------------------------------------------

void FocusChainBreak::focusInEvent(QFocusEvent* evt)
      {
      QQuickItem::focusInEvent(evt);
      if (evt->reason() == Qt::TabFocusReason)
            emit requestFocusTransfer(true);
      else if (evt->reason() == Qt::BacktabFocusReason)
            emit requestFocusTransfer(false);
      }

//---------------------------------------------------------
//   MsQuickView
//---------------------------------------------------------

MsQuickView::MsQuickView(const QUrl& source, QWindow* parent)
   : QQuickView(parent) // init() should be called before setting source
      {
      init();
      setSource(source);
      }

//---------------------------------------------------------
//   MsQuickView::init
//---------------------------------------------------------

void MsQuickView::init()
      {
      registerQmlTypes();
      connect(this, &QQuickView::statusChanged, this, &MsQuickView::onStatusChanged);
      }

//---------------------------------------------------------
//   registerQmlTypes
//---------------------------------------------------------

void MsQuickView::registerQmlTypes()
      {
      static bool registered = false;
      if (registered)
            return;

      qmlRegisterType<FocusChainBreak>("MuseScore.Utils", 3, 3, "FocusChainBreak");

      registered = true;
      }

//---------------------------------------------------------
//   MsQuickView::onStatusChanged
//---------------------------------------------------------

void MsQuickView::onStatusChanged(QQuickView::Status status)
      {
      if (status == QQuickView::Ready) {
            FocusChainBreak* fb = rootObject()->findChild<FocusChainBreak*>();
            if (fb)
                  connect(fb, &FocusChainBreak::requestFocusTransfer, this, &MsQuickView::transferFocus);
            }
      }

//---------------------------------------------------------
//   MsQuickView::transferFocus
//---------------------------------------------------------

void MsQuickView::transferFocus(bool forward)
      {
      if (prevFocusWidget) {
            QWidget* focusWidget =
               forward
               ? prevFocusWidget->nextInFocusChain()
               : prevFocusWidget->previousInFocusChain();

            if (focusWidget)
                  focusWidget->setFocus();
            }
      }

//---------------------------------------------------------
//   MsQuickView::focusInEvent
//---------------------------------------------------------

void MsQuickView::focusInEvent(QFocusEvent* evt)
      {
      prevFocusWidget = QApplication::focusWidget();
      QQuickView::focusInEvent(evt);
      }

//---------------------------------------------------------
//   MsQuickView::keyPressEvent
//---------------------------------------------------------

void MsQuickView::keyPressEvent(QKeyEvent* evt)
      {
      QQuickView::keyPressEvent(evt);
      if (evt->isAccepted())
            return;

      if (evt->key() == Qt::Key_Escape && evt->modifiers() == Qt::NoModifier) {
            mscore->focusScoreView();
            evt->accept();
            }
      }

//---------------------------------------------------------
//   QmlDockWidget
//---------------------------------------------------------

QmlDockWidget::QmlDockWidget(QQmlEngine* e, QWidget* parent, Qt::WindowFlags flags)
   : QDockWidget(parent, flags), engine(e)
      {}

QmlDockWidget::QmlDockWidget(QQmlEngine* e, const QString& title, QWidget* parent, Qt::WindowFlags flags)
   : QDockWidget(title, parent, flags), engine(e)
      {}

//---------------------------------------------------------
//   QmlDockWidget::getView
//---------------------------------------------------------

QQuickView* QmlDockWidget::getView()
      {
      if (!_view) {
            if (engine)
                  _view = new MsQuickView(engine, nullptr);
            else
                  _view = new MsQuickView();

            QWidget* container = QWidget::createWindowContainer(_view);
            container->setFocusPolicy(Qt::TabFocus); // or Qt::StrongFocus?
            setWidget(container);
            }
      return _view;
      }

//---------------------------------------------------------
//   QmlDockWidget::setSource
//---------------------------------------------------------

void QmlDockWidget::setSource(const QUrl& url)
      {
      QQuickView* view = getView();

      view->setSource(url);

      view->setColor(QApplication::palette().color(QPalette::Window));
      view->setResizeMode(QQuickView::SizeRootObjectToView);
      }
}

