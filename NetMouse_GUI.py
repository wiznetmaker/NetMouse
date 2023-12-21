from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QLineEdit, QPushButton, QLabel
from PyQt5.QtCore import pyqtSignal
import sys
import socket
import pyautogui
import threading

def move_mouse_relative(adc0, adc1, dead_zone_start, dead_zone_end, smooth_factor=0.05):
    screen_width, screen_height = pyautogui.size()

    if dead_zone_start <= adc0 <= dead_zone_end and dead_zone_start <= adc1 <= dead_zone_end:
        return

    scale_factor = 0.1

    if adc0 < dead_zone_start:
        y_move = (adc0 - dead_zone_start) * scale_factor
    elif adc0 > dead_zone_end:
        y_move = (adc0 - dead_zone_end) * scale_factor
    else:
        y_move = 0

    if adc1 > dead_zone_end:
        x_move = -(adc1 - dead_zone_end) * scale_factor
    elif adc1 < dead_zone_start:
        x_move = -(adc1 - dead_zone_start) * scale_factor
    else:
        x_move = 0

    pyautogui.moveRel(x_move, -y_move, duration=smooth_factor)

class NetmouseApp(QWidget):
    update_status_signal = pyqtSignal(str)

    def __init__(self):
        super().__init__()
        self.server_thread = None
        self.server_running = [False]
        self.initUI()

    def initUI(self):
        self.setWindowTitle('Netmouse Server Configuration')
        layout = QVBoxLayout()

        self.statusLabel = QLabel(self)
        layout.addWidget(self.statusLabel)

        self.ipInput = QLineEdit(self)
        self.ipInput.setPlaceholderText("Enter Server IP")
        self.ipInput.setText("192.168.11.197")  # 기본 IP 설정
        layout.addWidget(self.ipInput)

        self.portInput = QLineEdit(self)
        self.portInput.setPlaceholderText("Enter Server Port")
        self.portInput.setText("5000")  # 기본 포트 설정
        layout.addWidget(self.portInput)

        self.startButton = QPushButton("Start Server", self)
        self.startButton.clicked.connect(self.startServer)
        layout.addWidget(self.startButton)

        self.stopButton = QPushButton("Stop Server", self)
        self.stopButton.clicked.connect(self.stopServer)
        layout.addWidget(self.stopButton)

        self.exitButton = QPushButton("EXIT", self)
        self.exitButton.clicked.connect(self.close)
        layout.addWidget(self.exitButton)

        self.setLayout(layout)
        self.update_status_signal.connect(self.updateStatusLabel)

    def startServer(self):
        if self.server_thread and self.server_thread.is_alive():
            return

        ip = self.ipInput.text()
        port = int(self.portInput.text())
        self.server_running[0] = True
        self.server_thread = threading.Thread(target=self.run_netmouse_server, args=(ip, port))
        self.server_thread.start()

    def stopServer(self):
        if self.server_thread and self.server_thread.is_alive():
            self.server_running[0] = False
            self.server_thread.join()
            self.update_status_signal.emit("Server stopped")

    def updateStatusLabel(self, message):
        self.statusLabel.setText(message)

    def closeEvent(self, event):
        self.stopServer()
        event.accept()    

    def run_netmouse_server(self, ip, port):
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.bind((ip, port))
                s.listen()
                self.update_status_signal.emit("Waiting for connection...")

                conn, addr = s.accept()
                with conn:
                    self.update_status_signal.emit(f"Connected by {addr}")

                    buffer = ""
                    while self.server_running[0]:
                        data = conn.recv(1024)
                        if not data:
                            break

                        buffer += data.decode()

                        # 메시지 분리
                        while "\n" in buffer:
                            message, buffer = buffer.split("\n", 1)
                            # 버퍼의 나머지 부분은 버림
                            buffer = ""
                            try:
                                adc0_val, adc1_val = [int(value.split(":")[1].strip()) for value in message.split(",")]
                                move_mouse_relative(adc0_val, adc1_val, 2000, 2100)
                            except (IndexError, ValueError, AttributeError):
                                print("Invalid data format")
                
                    self.update_status_signal.emit("Client disconnected")

        except Exception as e:
            self.update_status_signal.emit(f"Server error: {e}")

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = NetmouseApp()
    ex.show()
    sys.exit(app.exec_())