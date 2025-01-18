import cv2
import numpy as np
import serial
import time

# Initialize serial communication with Arduino (change COM port as needed)
arduino = serial.Serial('COM6', 9600)  # Update 'COM3' to your Arduino's port
time.sleep(2)  # Wait for the connection to initialize

# Define HSV color ranges for red, green, and blue
color_ranges = {
    "red": ((0, 120, 70), (10, 255, 255)),
    "green": ((36, 25, 25), (86, 255, 255)),
    "blue": ((94, 80, 2), (126, 255, 255))
}

# Function to detect color in the frame
def detect_color(frame):
    hsv_img = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    for color, (lower, upper) in color_ranges.items():
        mask = cv2.inRange(hsv_img, np.array(lower), np.array(upper))
        if cv2.countNonZero(mask) > 500:  # Threshold to ignore noise
            return color
    return "unknown"

# Main loop for real-time video and color detection
cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    detected_color = detect_color(frame)
    print(f"Detected Color: {detected_color}")

    # Send the detected color to Arduino
    if detected_color == "red":
        arduino.write(b'1')  # Send '1' to Arduino for red
    elif detected_color == "green":
        arduino.write(b'2')  # Send '2' to Arduino for green
    elif detected_color == "blue":
        arduino.write(b'3')  # Send '3' to Arduino for blue
    else:
        arduino.write(b'0')  # Send '0' if no color detected

    # Display the detected color on the video frame
    cv2.putText(frame, f"Detected Color: {detected_color}", (10, 30),
                cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2, cv2.LINE_AA)

    # Show the real-time video with detected color
    cv2.imshow("Real-Time Color Detection", frame)

    # Break the loop if 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    time.sleep(6)  # Delay before detecting the next object

# Release resources
cap.release()
cv2.destroyAllWindows()
arduino.close()
