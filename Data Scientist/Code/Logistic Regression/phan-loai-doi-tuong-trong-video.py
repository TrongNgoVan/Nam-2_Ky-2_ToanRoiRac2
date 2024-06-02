# Importing the required libraries
import os
import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import confusion_matrix, classification_report
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder, StandardScaler
from PIL import Image

# Setting the data and labels variables
data = []
labels = []

# Looping through the image files
for image_file in os.listdir('data'):
    
    # Opening the image file
    image = Image.open('data/'+image_file)
    
    # Resizing the image to (32,32) pixels
    image = image.resize((32,32))
    
    # Converting the image to grayscale
    image = image.convert('L')
    
    # Converting the image to a numpy array
    image = np.array(image)
    
    # Flattening the image array to a 1D array
    image = image.flatten()
    
    # Adding the image array to the data variable
    data.append(image)
    
    # Adding the label to the labels variable
    label = image_file.split('.')[0]
    labels.append(label)
    
# Converting the data and labels variables to numpy arrays
data = np.array(data)
labels = np.array(labels)

# Encoding the labels variable
le = LabelEncoder()
labels = le.fit_transform(labels)

# Splitting the data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(data, labels, test_size=0.2, random_state=42)

# Scaling the training and testing data
scaler = StandardScaler()
X_train = scaler.fit_transform(X_train)
X_test = scaler.transform(X_test)

# Creating an instance of the Logistic Regression model
model = LogisticRegression(random_state=42)

# Fitting the model to the training data
model.fit(X_train, y_train)

# Predicting the labels for the testing data
y_pred = model.predict(X_test)

# Printing the confusion matrix and classification report
print('Confusion Matrix:')
print(confusion_matrix(y_test, y_pred))
print('Classification Report:')
print(classification_report(y_test, y_pred))
