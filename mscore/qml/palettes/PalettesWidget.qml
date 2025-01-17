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

import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import MuseScore.Palette 3.3
import MuseScore.Utils 3.3

// TODO: make some properties 'property alias`?
// and `readonly property`?

Item {
    id: palettesWidget

    readonly property PaletteWorkspace paletteWorkspace: mscore.paletteWorkspace

    readonly property bool hasFocus: Window.activeFocusItem

    implicitHeight: paletteTree.implicitHeight + palettesWidgetHeader.implicitHeight

    FocusChainBreak {}

    PalettesWidgetHeader {
        id: palettesWidgetHeader

        paletteWorkspace: palettesWidget.paletteWorkspace

        anchors {
            top: parent.top
            left: parent.left
            leftMargin: 12
            right: parent.right
            rightMargin: 12
        }
    }

    ToolSeparator {
        id: separator
        orientation: Qt.Horizontal
        anchors.top: palettesWidgetHeader.bottom
        width: parent.width
    }

    PaletteTree {
        id: paletteTree
        clip: true
        paletteWorkspace: palettesWidget.paletteWorkspace

        filter: palettesWidgetHeader.cellFilter

        anchors {
            top: separator.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
    }
}
