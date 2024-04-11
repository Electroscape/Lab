import os
import datetime
import shutil
import time

#current_date = datetime.datetime.now().date()
#current_time = datetime.datetime.now().time()

#Folder_File_name = f"Kassenfotos_{current_date.strftime("%d_%m_%Y")}"
#today_path_Files = os.getcwd() + "/" + Folder_File_name

file_list = os.listdir()

Dir = os.getcwd()

run = True

def create_and_rename_new_folder(new_folder_name):
    new_folder_path = os.getcwd() + "/" + new_folder_name
    #new_folder_path_2 = os.path.join
    os.makedirs(new_folder_path)
    os.rename(new_folder_path, new_folder_name)
    print("NewFolderPath: ", new_folder_path)
    return new_folder_path

def new_folder_check(new_folder_name):
    if os.path.exists(new_folder_name):
        print("Ordner existiert bereits")
        folder_path = os.getcwd() + "/" + new_folder_name
        return folder_path
    else:
        create_and_rename_new_folder(new_folder_name)
        print("Neuer Ordner wurde erstellt")

def image_create_time(file_path):
    print(f"checking ctime of fille {file_path}")
    new_image_creation_time = os.path.getctime(file_path)
    new_image_creation_date_time = datetime.datetime.fromtimestamp(new_image_creation_time)
    print("new immage Time: ", new_image_creation_date_time)
    return new_image_creation_date_time
    
def sort_images(file):
    file_path = os.path.join(Dir, file)
    sourcedir_foto = os.getcwd() + "/"
    creation_time = image_create_time(file_path)
    #print("File, Path File: ", file_path)
    print("creation Time: ", creation_time)
    new_image_name = "Foto--" + creation_time.strftime("%H_%M_%S--%d_%m_%Y") + ".png" #png in allgemein Dateiendung lassen
    counter_number = 2 #Number to count the diffrent image Versions
    file_creation_date_file_name = "Kassenfotos_" + creation_time.strftime("%d_%m_%Y")

    new_folder_check(file_creation_date_file_name)
    actual_folder_path = new_folder_check(file_creation_date_file_name)
    
    while new_image_name in os.listdir(file_creation_date_file_name):
        print("Datei existiert bereits ")
        new_image_name = "Foto--" + creation_time.strftime("%H_%M_%S--%d_%m_%Y") + "(" + str(counter_number) + ")" + ".png"
        counter_number = counter_number + 1 #set count number higher
    else: 
        print("Datei existiert noch nicht")
        counter_number = 2 #reset count number
    os.rename(file, new_image_name)
    shutil.move(sourcedir_foto + new_image_name, actual_folder_path)

def start_sort_process_2():
    for file in os.listdir(Dir):
        if not file.startswith(("Kassen", "Rename")):
            sort_images(file)


while run == True:
    start_sort_process_2()
    time.sleep(3)
    print("run")


