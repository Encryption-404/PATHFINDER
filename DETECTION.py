import cv2
import mediapipe as mp
import pyttsx3
import threading

# Initialize Mediapipe modules
mp_drawing = mp.solutions.drawing_utils
mp_holistic = mp.solutions.holistic

# Initialize Text-to-Speech (TTS) engine
text_speech = pyttsx3.init()
text_speech.setProperty('rate', 200)
voices = text_speech.getProperty('voices')
text_speech.setProperty('voice', voices[0].id)  # Set to female voice

# Speech message
answer = "Human presence Found."
speech_thread = None  # Variable to track the speech thread

# Function to handle speech synthesis in a separate thread
def speak_text():
    global speech_thread
    text_speech.say(answer)
    text_speech.runAndWait()
    speech_thread = None  # Reset thread variable when done

# Start video capture from the Flask stream
cap = cv2.VideoCapture("rtsp://192.168.1.1:7070/webcam")

with mp_holistic.Holistic() as holistic:
    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            continue
        
        # Resize the frame to 640x480
        frame = cv2.resize(frame, (700,520))
       # frame=cv2.rotate(frame, cv2.ROTATE_180) 
        # Process the frame with Mediapipe
        image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        results = holistic.process(image)

        # Detection criteria: Face or Hands detected
        human_detected = results.face_landmarks or results.right_hand_landmarks or results.left_hand_landmarks

        if human_detected:
            cv2.putText(image, "HUMAN PRESENCE DETECTED", (100, 70), 
            cv2.FONT_HERSHEY_PLAIN, 2, (0, 0,255), 2) 
            # Start speech synthesis in a new thread if it's not already running
            if speech_thread is None or not speech_thread.is_alive():
                speech_thread = threading.Thread(target=speak_text)
                speech_thread.start()
            
        # Convert image back to BGR for OpenCV display
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

        # Draw face landmarks
        if results.face_landmarks:
            mp_drawing.draw_landmarks(
                image, results.face_landmarks, 
                landmark_drawing_spec=mp_drawing.DrawingSpec(color=(0, 200, 0), thickness=1, circle_radius=1),
                connection_drawing_spec=mp_drawing.DrawingSpec(color=(0, 255, 0), thickness=1, circle_radius=1)
            )

        # Draw right hand landmarks
        if results.right_hand_landmarks:
            mp_drawing.draw_landmarks(image, results.right_hand_landmarks, mp_holistic.HAND_CONNECTIONS)

        # Draw left hand landmarks
        if results.left_hand_landmarks:
            mp_drawing.draw_landmarks(image, results.left_hand_landmarks, mp_holistic.HAND_CONNECTIONS)

        # Draw pose landmarks (Always displayed)
        if results.pose_landmarks:
            mp_drawing.draw_landmarks(image, results.pose_landmarks, mp_holistic.POSE_CONNECTIONS)

        # Display the result
        cv2.imshow('Face, Hand & Pose Detection', image)

        # Break the loop on pressing 'q'
        if cv2.waitKey(10) & 0xFF == ord('q'):
            break

# Release resources
cap.release()
cv2.destroyAllWindows()
