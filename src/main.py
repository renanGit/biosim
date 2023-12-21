import sys
from PySide6.QtWidgets import QApplication
from ui import MainWindow

if __name__ == "__main__":
    app = QApplication(sys.argv)

    steps = 100
    epochs = 10
    mw = MainWindow(steps, epochs)
    mw.init()
    mw.show()

    sys.exit(app.exec())