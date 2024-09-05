import os
from PIL import Image

def crop_images(parent_directory):
    for subdir, _, files in os.walk(parent_directory):
        for file in files:
            if file.endswith(('.jpg', '.jpeg')):
                file_path = os.path.join(subdir, file)
                with Image.open(file_path) as img:
                    width, height = img.size
                    
                    if file in ['0.jpg', '1.jpg']:
                        # Crop the left part
                        left = 1640
                        upper = 0
                        right = 3840
                        lower = height
                    elif file in ['2.jpg', '3.jpg']:
                        # Crop the right part
                        left = 0
                        upper = 0
                        right = 2800
                        lower = height
                    else:
                        continue
                    
                    cropped_img = img.crop((left, upper, right, lower))
                    cropped_img.save(file_path)
                    print(f'Cropped {file_path} to {cropped_img.size}')

# Replace 'path/to/parent_directory' with the actual path of the parent directory
parent_directory = 'C:/Users/kyy/Desktop/test/GES-GSP_vc16_Modified/input-data/g1'
crop_images(parent_directory)
