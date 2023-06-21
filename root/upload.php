<?php
// Check if a file was submitted
if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_FILES['file'])) {
    $file = $_FILES['file'];

    // Specify the directory where you want to save the uploaded file
    $uploadDir = './upload/';

    // Generate a unique name for the file to prevent overwriting
    $filename = uniqid() . '_' . $file['name'];

    // Read the file contents
    $fileContents = file_get_contents($file['tmp_name']);

    // Set the Content-Length header
    $contentLength = strlen($fileContents);
    header('Content-Length: ' . $contentLength);

    // Save the file
    if (file_put_contents($uploadDir . $filename, $fileContents)) {
        echo 'File uploaded successfully.';
    } else {
        echo 'Error uploading file.';
    }
}
?>
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