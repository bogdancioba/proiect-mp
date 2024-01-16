import sys
from PySide6.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QHBoxLayout, QWidget, QGroupBox, QLabel, QPushButton, QLineEdit, QTextEdit
from PySide6.QtGui import QIcon, QFont
from PySide6.QtCore import QTimer, Qt
import pyqtgraph as pg
import serial
from PySide6 import QtGui




class MainWindow(QMainWindow):
    promotie: str = "2023-2024"
    team: list[str] = [
        "Cioba Bogdan",
        "Constantin Gabriel",
    ]
    def __init__(self):
        super().__init__()
        self.setWindowTitle(f"Proiect Microprocesoare {self.promotie}")
        self.setWindowIcon(QIcon("./icon.png"))
        

        primary_layout = QVBoxLayout()
        secondary_layout = QHBoxLayout()
        tertiary_layout = QVBoxLayout()

        team_box = QGroupBox("Membrii echipei")
        bold_font = QFont()
        bold_font.setBold(True)
        team_box.setFont(bold_font)

        first_member = QLabel(f"Membru 1: {self.team[0]}")
        second_member = QLabel(f"Membru 2: {self.team[1]}")
        team_box_layout = QVBoxLayout()
        team_box_layout.addWidget(first_member,1)
        team_box_layout.addWidget(second_member,1)
        team_box.setLayout(team_box_layout)

        control_panel_box = QGroupBox("Control Panel")
        control_panel_box.setFont(bold_font)

        button3 = QPushButton("Send")
        button3.clicked.connect(self.send_input)
        self.line_edit = QLineEdit()
        self.line_edit.setAlignment(Qt.AlignmentFlag.AlignBottom)
        line_edit_label1 = QLabel("Pentru valori mici introduceti numere in intervalul 1-179")
        line_edit_label2 = QLabel("Pentru valori medii introduceti numere in intervalul 180-251")
        line_edit_label3 = QLabel("Pentru valori mari introduceti numere in intervalul 252-255")
        # Create a QVBoxLayout for the additional labels
        additional_labels_layout = QVBoxLayout()
        additional_labels_layout.addWidget(line_edit_label1)
        additional_labels_layout.addWidget(line_edit_label2)
        additional_labels_layout.addWidget(line_edit_label3)
        # Create a QLabel for the "Input:" line
        input_label = QLabel("Input:")
        # Create a QVBoxLayout for the control panel box
        control_panel_box_layout = QVBoxLayout()
        control_panel_box_layout.setSpacing(5)

        control_panel_box_layout.addStretch()
        control_panel_box_layout.addLayout(additional_labels_layout)
        control_panel_box_layout.addWidget(input_label)
        control_panel_box_layout.addWidget(self.line_edit, 1)
        control_panel_box_layout.addWidget(button3, 1)

        control_panel_box.setLayout(control_panel_box_layout)

        tertiary_layout.addWidget(team_box, 1)
        tertiary_layout.addWidget(control_panel_box,5)

        plot_widget = pg.PlotWidget()
        hour = [1,2,3,4,5,6,7,8,9,10]
        temperature = [30,32,34,32,33,31,29,32,35,45]

        plot_widget.plot(hour, temperature)

        secondary_layout.addLayout(tertiary_layout, 1)

        primary_layout.addLayout(secondary_layout, 4)
        self.text_edit = QTextEdit()
        self.text_edit.setReadOnly(True)

        debug_box = QGroupBox("Debug")
        debug_box_layout = QVBoxLayout()
        debug_box_layout.addWidget(self.text_edit, 1)
        debug_box.setLayout(debug_box_layout)

        primary_layout.addWidget(debug_box, 1)

        widget = QWidget()
        widget.setLayout(primary_layout)
        
        self.setCentralWidget(widget)

        self.plot_widget = pg.PlotWidget()
        secondary_layout.addWidget(self.plot_widget, 3)

        self.initSerial()
        self.initGraph()
        self.initTimer()

    def initSerial(self):
            self.ser = serial.Serial('COM7', 38400, timeout=0.1)  # Replace 'COM_PORT' with your actual port
            # Other serial setup code as needed

    def initTimer(self):
            self.timer = QTimer()
            self.timer.setInterval(1)  # Update interval
            self.timer.timeout.connect(self.updatePlotData)
            self.timer.start()


    def updatePlotData(self):
        if self.ser.in_waiting:
            line = self.ser.readline().decode('utf-8').strip()
            self.text_edit.moveCursor(QtGui.QTextCursor.End)
            self.text_edit.insertPlainText(f"Raw line: {line}\n")  # Display raw line in text edit

            try:
                adcValue = int(line)  # Assuming adcValue is an integer
                self.text_edit.insertPlainText(f"ADC Value: {adcValue}\n")  # Display ADC value in text edit

                # Change the line color based on adcValue
                if adcValue <= 180:
                    pen = pg.mkPen(color=(255, 0, 0))  # Red
                elif 181 <= adcValue <= 251:
                    pen = pg.mkPen(color=(255, 255, 0))  # Green
                else:
                    pen = pg.mkPen(color=(0, 255, 0))  # Yellow

                self.data_line.setPen(pen)  # Set the color of the line

                self.y = self.y[1:]  # Shift the y-axis data
                self.y.append(adcValue)  # Append new adcValue
                self.data_line.setData(self.x, self.y)  # Update the data in the graph

            except ValueError:
                self.text_edit.insertPlainText("Error in conversion\n")  # Display error message in text edit





    def closeEvent(self, event):
            self.ser.close()  # Close the serial port when the app closes
            super().closeEvent(event)

    def initGraph(self):
        self.x = list(range(255))  # Initialize with 100 time points
        self.y = [0 for _ in range(255)]  # Initialize with 100 data points at 0

        pen = pg.mkPen(color=(255, 0, 0))
        self.data_line = self.plot_widget.plot(self.x, self.y, pen=pen)

    def send_input(self):
        input_value = self.line_edit.text()
        self.line_edit.clear()
        self.text_edit.insertPlainText(f"Sending input: {input_value}\n")

        # Convert the input value to bytes and send it
        try:
            # Assuming the input should be sent as an integer byte value
            input_byte = int(input_value).to_bytes(1, byteorder='big')
            self.ser.write(input_byte)
        except ValueError:
            self.text_edit.insertPlainText("Invalid input: must be an integer\n")
        except serial.SerialException as e:
            self.text_edit.insertPlainText(f"Serial write error: {e}\n")

