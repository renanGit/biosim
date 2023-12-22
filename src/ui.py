import yaml, os, imageio, numpy as np
from PySide6.QtWidgets import QMainWindow, QWidget, QPushButton, QGridLayout
from PySide6.QtCore import QPoint, QRect, QTimer, Qt, QSize
from PySide6.QtGui import QPainter, QPointList, QImage

import biosim

WIN_W = 840
WIN_H = 840
PLOT_W = 720
PLOT_H = 720

class PlotWidget(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setFixedSize(PLOT_W, PLOT_H)

        self._timer = QTimer(self)
        self._timer.setInterval(100)
        self._timer.timeout.connect(self.frame)
        self._points = QPointList()

        self._sim = biosim.Sim()
        self._sizeX = 0
        self._sizeY = 0
        self._vid_writer = imageio.get_writer('src/video.gif', fps=10)

    def simInit(self):
        with open(os.path.join(os.path.dirname(os.path.realpath(__file__)), 'config.yml'), 'r') as f:
            yml = yaml.safe_load(f)
            biosim.Setup(yml)
            self._sizeX = int(yml["Grid"]["SizeX"])
            self._sizeY = int(yml["Grid"]["SizeY"])

        biosim.PrintConfig()
        self._sim.Init()
        
    def run(self, steps, epochs):
        self._sim.Run(steps, epochs)
        self._timer.start()

    def kill(self):
        self._sim.Reset()

    def save(self):
        self._vid_writer.close()

    # Gets called every time interval
    def frame(self):
        self._points.clear()
        frameComplete = False
        sim = self._sim

        while sim.CanPollMovement() and not frameComplete:
            pos = sim.ConsumeMovement()
            frameComplete = pos.coordX > self._sizeX and pos.coordY > self._sizeY
            if not frameComplete:
                self._points.append(QPoint(pos.coordX, pos.coordY))
        
        if len(self._points) > 0:
            self.update()
            if not self._vid_writer.closed:
                pixmap = self.grab()
                qimg = pixmap.toImage().convertToFormat(QImage.Format_RGB888)
                array = np.ndarray((qimg.height(), qimg.width(), 3), buffer=qimg.constBits(), strides=[qimg.bytesPerLine(), 3, 1], dtype=np.uint8)
                self._vid_writer.append_data(array)
        else:
            self._timer.stop()

    # Gets called on update, updates the frame
    def paintEvent(self, event):
        with QPainter(self) as painter:
            rect = QRect(QPoint(0, 0), QSize(PLOT_W, PLOT_H))
            painter.fillRect(rect, Qt.white)
            painter.scale(PLOT_W / self._sizeX, PLOT_H / self._sizeY)
            painter.drawPoints(self._points)

class MainWindow(QMainWindow):
    def __init__(self, steps, epochs, parent=None):
        QMainWindow.__init__(self, parent)
        self.setFixedSize(WIN_W, WIN_H)
        self._steps = steps
        self._epochs = epochs
        self._running = False

        centralWidget = QWidget()
        self.plot_widget = PlotWidget()
        startButton = QPushButton("Start")
        saveButton = QPushButton("Save")
        startButton.setFixedSize(100, 40)
        saveButton.setFixedSize(100, 40)
        startButton.clicked.connect(self.run)
        saveButton.clicked.connect(self.plot_widget.save)

        centralWidget.setLayout(QGridLayout())
        layout = centralWidget.layout()
        layout.addWidget(self.plot_widget, 0, 0, 1, 2, Qt.AlignHCenter)
        layout.addWidget(startButton, 1, 0, Qt.AlignHCenter)
        layout.addWidget(saveButton, 1, 1, Qt.AlignHCenter)

        self.setCentralWidget(centralWidget)

    def closeEvent(self, event):
        self.plot_widget.kill()
        event.accept()

    def init(self):
        self.plot_widget.simInit()
    
    def run(self):
        if not self._running:
            self.plot_widget.run(self._steps, self._epochs)
            self._running = True