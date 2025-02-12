import serial

class Car_Controller:
    def __init__(self, port="/dev/ttyACM", baudrate=115200) -> None:
        port_counter = 0
        self.arduino = None  # Prevent AttributeError

        while port_counter <= 4:
            try:
                self.arduino = serial.Serial(port=f"{port}{port_counter}", baudrate=baudrate, timeout=1)
                print(f"Connected to Arduino on {port}{port_counter}")
                break
            except serial.SerialException:
                print(f"Attempt {port_counter}: No connection on {port}{port_counter}")
                port_counter += 1 
        
        if self.arduino is None:  # No connection established
            raise ConnectionError("ERROR: Could not connect to any serial port.")

    def set_speed_and_steering(self, target_speed, target_steering, translate_values=False):
        if self.arduino is None:
            print("ERROR: No serial connection established. Cannot send data.")
            return  

        target_speed_translated = int(target_speed * 100) if translate_values else target_speed
        target_steering_translated = int(target_steering * 50 + 50) if translate_values else target_steering

        try:
            self.arduino.write(f"{target_speed_translated},{target_steering_translated}\n".encode()) 
            self.arduino.flush()
            print(f"Sent: {target_speed_translated}, {target_steering_translated}")
        except serial.SerialException as e:
            print(f"ERROR: Serial communication failed: {e}")
