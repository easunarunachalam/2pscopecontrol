# This Python file uses the following encoding: utf-8
import os
from pathlib import Path
import sys

from functools import partial

from PySide2.QtWidgets import QApplication, QTabWidget, QWidget, QPushButton
from PySide2.QtCore import Qt, QFile, QCoreApplication, QTimer
from PySide2.QtUiTools import QUiLoader


def props(obj):
    return vars(obj).items()



class Widget(QWidget):
    def __init__(self):
        super(Widget, self).__init__()
        self.load_ui()

        self.exit_button.clicked.connect(partial(self.exit_button_listener))

        self.i = 0
        timer = QTimer(self)
        timer.timeout.connect(self.update_system_state)
        timer.setInterval(100)
        timer.start()


    def load_ui(self):
        loader = QUiLoader()
        path = os.fspath(Path(__file__).resolve().parent / "form.ui")
        ui_file = QFile(path)
        ui_file.open(QFile.ReadOnly)
        self.ui = loader.load(ui_file, self)
        ui_file.close()

        # add refs to all controls and displays for convenience
        self.system_mode = self.ui.findChildren(QTabWidget, "system_mode")[0]
        self.system_status = self.ui.findChildren(QTabWidget, "system_status")[0]

        self.tab_setup = self.system_mode.findChildren(QWidget, "Setup")[0]
        self.tab_preview = self.system_mode.findChildren(QWidget, "Preview")[0]
        self.tab_acquire = self.system_mode.findChildren(QWidget, "Acquire")[0]
        self.tab_exit = self.system_mode.findChildren(QWidget, "Exit")[0]
        self.exit_button = self.tab_exit.findChildren(QWidget, "exit_button")[0]

    def update_system_state(self):
        print("hello" + str(self.i))
        self.i += 1

    def exit_button_listener(self):
        sys.exit()

    # def prepare_access_objects(self)


if __name__ == "__main__":
    QCoreApplication.setAttribute(Qt.AA_ShareOpenGLContexts)
    # qt_app = QtWidgets.QApplication(sys.argv)

    app = QApplication([])
    widget = Widget()
    widget.show()

    #
    # while True:
    #     if widget.exit_button.isChecked():
    #         sys.exit()

    sys.exit(app.exec_())
