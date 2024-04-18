import os
import datetime
import shutil
import time

file_list = os.listdir()    # list of files in root_dir

root_dir = os.getcwd() + "\KassenFotos"     # path to folder with Images (new folders will be created)

run = True


def create_and_rename_new_year_folder(new_year_folder_name):
    new_year_folder_path = os.path.join(root_dir, new_year_folder_name)
    os.makedirs(new_year_folder_path)
    return new_year_folder_path


def new_year_folder_check(new_year_folder_name):
    year_folder_path = os.path.join(root_dir, new_year_folder_name)
    if os.path.exists(year_folder_path):
        return year_folder_path
    else:
        create_and_rename_new_year_folder(new_year_folder_name)


def create_and_rename_new_folder(new_folder_name, year_folder_path):
    new_folder_path = os.path.join(year_folder_path, new_folder_name)
    os.makedirs(new_folder_path)
    return new_folder_path


def new_folder_check(new_folder_name, year_folder_path):
    folder_path = os.path.join(year_folder_path, new_folder_name)
    if os.path.exists(folder_path):
        return folder_path
    else:
        create_and_rename_new_folder(new_folder_name, year_folder_path)


def image_create_time(file_path):
    new_image_creation_time = os.path.getctime(file_path)
    new_image_creation_date_time = datetime.datetime.fromtimestamp(new_image_creation_time)
    return new_image_creation_date_time


def sort_images(file):
    file_path = os.path.join(root_dir, file)
    sourcedir_foto = root_dir + "/"
    creation_time = image_create_time(file_path)

    file_ext_split = os.path.splitext(file_path)    # file ending
    file_ending = file_ext_split[1]     # second part of file name

    new_image_name = "Foto--" + creation_time.strftime("%H_%M_%S--%d_%m_%Y") + file_ending
    counter_number = 2  # Number to count the different image Versions
    file_creation_date_file_name = "Kassenfotos_" + creation_time.strftime("%d_%m_%Y")

    new_image_year = "Fotos-" + creation_time.strftime("%Y")
    new_year_folder_check(new_image_year)
    actual_year_folder_path = new_year_folder_check(new_image_year)

    new_folder_check(file_creation_date_file_name, actual_year_folder_path)  # check if folder exists
    actual_folder_path = new_folder_check(file_creation_date_file_name, actual_year_folder_path)     # folder for images
    
    while new_image_name in os.listdir(str(actual_folder_path)):
        new_image_name = "Foto--" + creation_time.strftime("%H_%M_%S--%d_%m_%Y") + "(" + str(counter_number) + ")" + file_ending
        counter_number = counter_number + 1
    else: 
        counter_number = 2  # reset count number
    new_dir_image = sourcedir_foto + file
    os.rename(new_dir_image, sourcedir_foto + new_image_name)
    shutil.move(str(sourcedir_foto) + str(new_image_name), str(actual_folder_path))


def start_sort_process():
    for file in os.listdir(root_dir):
        path_check = os.path.join(root_dir, file)
        if not os.path.isdir(path_check):
            sort_images(file)


start_sort_process()


