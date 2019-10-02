# -*- coding: utf-8 -*-
"""
/***************************************************************************
 SeabotDockWidget
                                 A QGIS plugin
 Seabot
 Generated by Plugin Builder: http://g-sherman.github.io/Qgis-Plugin-Builder/
                             -------------------
        begin                : 2018-10-31
        git sha              : $Format:%H$
        copyright            : (C) 2018 by Thomas Le Mézo
        email                : thomas.le_mezo@ensta-bretagne.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
"""

import os, time

from PyQt5 import QtGui, QtWidgets, uic
from PyQt5.QtCore import pyqtSignal, QTimer

from .seabotLayerLivePosition import SeabotLayerLivePosition
from .boatLayerLivePosition import BoatLayerLivePosition

FORM_CLASS, _ = uic.loadUiType(os.path.join(
    os.path.dirname(__file__), 'seabot_dockwidget_base.ui'))


class SeabotDockWidget(QtWidgets.QDockWidget, FORM_CLASS):

    closingPlugin = pyqtSignal()
    timer_seabot = QTimer()
    timer_boat = QTimer()
    flag = False
    count = 0

    layerLivePosition = SeabotLayerLivePosition()
    boatLivePosition = BoatLayerLivePosition()

    def __init__(self, parent=None):
        """Constructor."""
        super(SeabotDockWidget, self).__init__(parent)
        # Set up the user interface from Designer.
        # After setupUI you can access any designer object by doing
        # self.<objectname>, and you can use autoconnect slots - see
        # http://qt-project.org/doc/qt-4.8/designer-using-a-ui-file.html
        # #widgets-and-dialogs-with-auto-connect

        print(self.__dir__)
        self.setupUi(self)

        self.timer_seabot.timeout.connect(self.process_seabot)
        self.timer_seabot.setInterval(5000)

        self.timer_boat.timeout.connect(self.process_boat)
        self.timer_boat.setInterval(1000)

        self.pushButton.clicked.connect(self.enable_timer_seabot)
        self.pushButtonBoat.clicked.connect(self.enable_timer_boat)

    def closeEvent(self, event):
        self.timer_seabot.stop()
        self.timer_boat.stop()
        self.pushButton.setChecked(False)
        self.closingPlugin.emit()
        event.accept()

    def enable_timer_seabot(self):
        if(self.pushButton.isChecked()):
            self.timer_seabot.start()
        else:
            self.timer_seabot.stop()

    def enable_timer_boat(self):
        if(self.pushButtonBoat.isChecked()):
            self.boatLivePosition.start()
            self.timer_boat.start()
        else:
            self.timer_boat.stop()
            self.boatLivePosition.stop()

    def process_seabot(self):
        if(self.layerLivePosition.update()):
            self.label.setText("Connected - " + str(self.count))
            self.count += 1
        else:
            self.label.setText("Error")
            self.count = 0

    def process_boat(self):
        self.boatLivePosition.update()
        
