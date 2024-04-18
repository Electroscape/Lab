import os
import datetime
import shutil
import time

file_list = os.listdir()    # list of files in Dir

Dir = os.getcwd() + "\KassenFotos"  # path to folder with Images (new folders will be created) (manually write path)

run = True


def create_and_rename_new_folder(new_folder_name):
    new_folder_path = os.path.join(Dir, new_folder_name)
    os.makedirs(new_folder_path)
    return new_folder_path


def new_folder_check(new_folder_name):
    folder_path = os.path.join(Dir, new_folder_name)
    if os.path.exists(folder_path):
        return folder_path
    else:
        create_and_rename_new_folder(new_folder_name)


def image_create_time(file_path):
    new_image_creation_time = os.path.getctime(file_path)
    new_image_creation_date_time = datetime.datetime.fromtimestamp(new_image_creation_time)
    return new_image_creation_date_time


def sort_images(file):
    file_path = os.path.join(Dir, file)
    sourcedir_foto = Dir + "/"
    creation_time = image_create_time(file_path)

    file_ext_split = os.path.splitext(file_path)    # file ending
    file_ending = file_ext_split[1]     # second part of file name

    new_image_name = "Foto--" + creation_time.strftime("%H_%M_%S--%d_%m_%Y") + file_ending
    counter_number = 2  # Number to count the diffrent image Versions
    file_creation_date_file_name = "Kassenfotos_" + creation_time.strftime("%d_%m_%Y")

    new_folder_check(file_creation_date_file_name)  # check if folder exists
    actual_folder_path = new_folder_check(file_creation_date_file_name)     # folder for images
    
    while new_image_name in os.listdir(actual_folder_path):
        new_image_name = "Foto--" + creation_time.strftime("%H_%M_%S--%d_%m_%Y") + "(" + str(counter_number) + ")" + file_ending
        counter_number = counter_number + 1
    else: 
        counter_number = 2  # reset count number
    new_dir_image = sourcedir_foto + file
    os.rename(new_dir_image, sourcedir_foto + new_image_name)
    shutil.move(sourcedir_foto + new_image_name, actual_folder_path)


def start_sort_process():
    for file in os.listdir(Dir):
        path_check = os.path.join(Dir, file)
        if not os.path.isdir(path_check):
            sort_images(file)


while run:
    start_sort_process()
    time.sleep(3)
    print("run")


