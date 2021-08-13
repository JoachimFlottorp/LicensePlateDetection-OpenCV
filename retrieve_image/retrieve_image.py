import mariadb
import sys
from PIL import Image
import os

cwd = os.getcwd()
abspath = os.path.abspath(cwd)
output_folder = "%s/output/" % abspath

def get_id():
    for idx, arg in enumerate(sys.argv):
        if arg == "-id":
            id = sys.argv[idx + 1]
            return str(id)

def show_or_not():
    for idx, arg in enumerate(sys.argv):
        if arg == "-s":
            return True

def list_id():
    for idx, arg in enumerate(sys.argv):
        if arg == "--list-id":
            return True

def query_images():
    for idx, arg in enumerate(sys.argv):
        if arg == "--query-images":
            return True

# def query_or_retrieve(): 


def query_list(list_id, show_images, id, show): 
    # Connect to our SQL server
    try:
        conn = mariadb.connect(user="root", password="root", 
        host = "192.168.1.39", port=3306, database="license"
        )
    except mariadb.Error as e:
        print(f"Error connecting to MYSQL server with IP: {e}")
        sys.exit(1)
    cur = conn.cursor()
    # Query the server
    if(list_id): 
        cur.execute(f"SELECT id, plateText FROM PLATES")
        for id, plateText in cur:
            print(f"ID: {id} Plate: {plateText}")

    if(show_images):
        cur.execute(f"SELECT * FROM PLATES WHERE id={id}")
        for id, scene, car, plate, plateText in cur:
            print(f"ID: {id} Plate: {plateText}")

            scene_dir = f"{output_folder}Scene ID {id} Plate {plateText}.jpg"
            scene_w = open(scene_dir, "wb")
            scene_w.write(scene)
            scene_w.close()
            if(show):
                with Image.open(scene_dir) as im:
                    im.show()

            car_dir = f"{output_folder}Car ID {id} Plate {plateText}.jpg"
            car_w = open(car_dir, "wb")
            car_w.write(car)
            car_w.close()
            if(show):
                with Image.open(car_dir) as im:
                    im.show()

            plate_dir = f"{output_folder}Plate ID {id} Plate {plateText}.jpg"
            plate_w = open(plate_dir, "wb")
            plate_w.write(plate)
            plate_w.close()
            if(show):
                with Image.open(plate_dir) as im:
                    im.show()

    conn.close()
    
def create_output_dir():
    dirs = os.listdir(os.getcwd())
    if "output" not in dirs:
        os.mkdir("output")
        print("Made Output Dir")
    

if __name__ == '__main__':
    try:
        if  sys.argv[1] == "-h":
            print('\nCommands:\nHelp: -h\Query: -q\nShow Image At End -s\nId: --id\nList ID: --list-id\nQuery Images: --query-images\nExample: retrieve_image.py -q -s --query-images --id 20 --show-images\n')
            exit(1) 
        elif sys.argv[1] == "-q":
            list_id = False
            show_images = False
            if list_id():
                list_id = True
            query_images = query_images()
            id = get_id()
            show_images = show_or_not()
            # Get id
            if list_id == True:
                query_list(True, False, 0, False)
                exit(1)
            # Not allowed to query if you forget an id
            if id == None:
                print("Forgot ID '--id'")
                exit(1)
            # Create output dir
            create_output_dir()
            # Make query
            if query_images == True:
                query_list(False, True, id, show_images)
        else: 
            exit(1)
    except IndexError as error:
        print(f"IndexError: {error}\nTry '-h'")
        exit(1)