#!/usr/bin/env python
import cgi
import os
import tempfile

print("Content-Type: text/html\n\n")

form = cgi.FieldStorage()

if form.getvalue('file'):
    file_item = form['file']
    if file_item.filename:
        # Specify the directory where you want to save the uploaded file
        upload_dir = './upload/'

        # Create the upload directory if it doesn't exist
        if not os.path.exists(upload_dir):
            os.makedirs(upload_dir)

        # Generate a unique name for the file to prevent overwriting
        filename = tempfile.NamedTemporaryFile(dir=upload_dir).name

        # Save the file
        with open(os.path.join(upload_dir, filename), 'wb') as file:
            file.write(file_item.file.read())

        print("File uploaded successfully.")
        exit()

print('''
<!DOCTYPE html>
<html>
<head>
    <title>File Upload</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 20px;
        }

        h2 {
            margin-top: 0;
        }

        .upload-form {
            margin-top: 20px;
        }

        .upload-input {
            margin-bottom: 10px;
        }

        .upload-button {
            background-color: #4CAF50;
            color: white;
            padding: 10px 20px;
            border: none;
            cursor: pointer;
            font-size: 16px;
        }
    </style>
</head>
<body>
    <h2>Upload a File</h2>
    <form method="POST" action="" enctype="multipart/form-data" class="upload-form">
        <input type="file" name="file" class="upload-input" />
        <input type="submit" value="Upload" class="upload-button" />
    </form>
</body>
</html>
''')