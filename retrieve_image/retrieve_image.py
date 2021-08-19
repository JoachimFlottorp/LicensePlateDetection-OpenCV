#!/usr/bin/env python
import mariadb
import sys
from PIL import Image
import os
import tkinter as tk
import tkinter.ttk as ttk
from default_values import *

is_hidden = True
root = tk.Tk()
list_id_var = tk.IntVar()
query_image_var = tk.IntVar()
show_image_var = tk.IntVar()

cwd = os.getcwd()
abspath = os.path.abspath(cwd)
output_folder = "%s/output/" % abspath

def query_list(): 
    # Connect to our SQL server
    try:
        conn = mariadb.connect(user=str(user_entry.get()), password=str(password_entry.get()), 
        host = str(server_entry.get()), port= int(port_entry.get()), database=database_entry.get()
        )
    except mariadb.Error as e:
        print(f"Error connecting to MYSQL server with IP: {e}")
        text1.insert('1.0', f"Error connecting to MYSQL server with IP: {e}")
        return 0
    cur = conn.cursor()
    # Query the server
    if(list_id_var.get() == 1):
        cur.execute(f"SELECT id, plateText FROM PLATES")
        for id, plateText in cur:
            text1.insert('1.0', f"ID: {id} Plate: {plateText}\n")
            # print(f"ID: {id} Plate: {plateText}")
    
    if(query_image_var.get() == 1):
        cur.execute(f"SELECT * FROM PLATES WHERE id={int(id_entry.get())}")
        for id, scene, car, plate, plateText in cur:
            text1.insert('1.0', f"ID: {id} Plate: {plateText}")
            print(f"ID: {id} Plate: {plateText}")

            scene_dir = f"{output_folder}Scene ID {id} Plate {plateText}.jpg"
            scene_w = open(scene_dir, "wb")
            scene_w.write(scene)
            scene_w.close()

            car_dir = f"{output_folder}Car ID {id} Plate {plateText}.jpg"
            car_w = open(car_dir, "wb")
            car_w.write(car)
            car_w.close()

            plate_dir = f"{output_folder}Plate ID {id} Plate {plateText}.jpg"
            plate_w = open(plate_dir, "wb")
            plate_w.write(plate)
            plate_w.close()

            if(show_image_var.get() == 1):
                with Image.open(scene_dir) as a, Image.open(car_dir) as b, Image.open(plate_dir) as c:
                    a.show()
                    b.show()
                    c.show()

    conn.close()
    
def create_output_dir():
    dirs = os.listdir(os.getcwd())
    if "output" not in dirs:
        os.mkdir("output")
        text1.insert('1.0', "Made Output Dir")
        print("Made Output Dir")
    
def hide_password():
    global is_hidden
    password_entry.configure(show='') if is_hidden else password_entry.configure(show='•')
    is_hidden = not is_hidden

def clear_screen():
    text1.delete('1.0', tk.END)
    

# build ui
frame3 = tk.Frame()
root.title("MariaDB Retrieve")

server_entry = tk.Entry(frame3)
server_entry.configure(state='normal', validate='focus')
_text_ = DEFAULT.SERVER
server_entry.delete('0', 'end')
server_entry.insert('0', _text_)
server_entry.grid(column='0', row='0')

server_label = tk.Label(frame3)
server_label.configure(text='Server Ip:')
server_label.grid(row='0', sticky='w')

port_label = tk.Label(frame3)
port_label.configure(text='Port:')
port_label.grid(column='0', row='0', sticky='e')
port_entry = tk.Entry(frame3)
port_entry.configure()
_text_ = DEFAULT.PORT
port_entry.delete('0', 'end')
port_entry.insert('0', _text_)
port_entry.grid(column='1', row='0', sticky='w')

text1 = tk.Text(frame3)
text1.configure(width=50)
text1.grid(column='0', columnspan='2', row='5', rowspan='1')

query_button = tk.Button(frame3)
query_button.configure(text='Query', command=query_list)
query_button.grid(column='0', row='4', sticky='nw')

show_image_label = tk.Label(frame3)
show_image_label.configure(text='Show Image:')
show_image_label.grid(column='0', row='1', sticky='w')

query_image_label = tk.Label(frame3)
query_image_label.configure(text='Query Image:')
query_image_label.grid(column='0', row='2', sticky='w')

list_id_label = tk.Label(frame3)
list_id_label.configure(text='List Id:')
list_id_label.grid(column='0', row='3', sticky='w')
list_id_check = tk.Checkbutton(frame3, variable=list_id_var, onvalue=1, offvalue=0)
list_id_check.grid(column='0', padx='75', row='3', sticky='w')

query_image_check = tk.Checkbutton(frame3, variable=query_image_var, onvalue=1, offvalue=0)
query_image_check.grid(column='0', padx='75', row='2', sticky='w')

show_image_check = tk.Checkbutton(frame3, variable=show_image_var, onvalue=1, offvalue=0)
show_image_check.grid(column='0', padx='75', row='1', sticky='w')

database_label = tk.Label(frame3)
database_label.configure(text='Database:')
database_label.grid(column='0', row='1', sticky='e')
database_entry = tk.Entry(frame3)
_text_ = DEFAULT.DATABASE
database_entry.delete('0', 'end')
database_entry.insert('0', _text_)
database_entry.grid(column='1', row='1', sticky='w')

user_label = tk.Label(frame3)
user_label.configure(text='User:')
user_label.grid(column='0', row='2', sticky='e')

password_label = tk.Label(frame3)
password_label.configure(text='Password:')
password_label.grid(column='0', row='3', sticky='e')

user_entry = tk.Entry(frame3)
_text_ = DEFAULT.USERNAME
user_entry.delete('0', 'end')
user_entry.insert('0', _text_)
user_entry.grid(column='1', row='2', sticky='w')

password_entry = tk.Entry(frame3)
password_entry.configure(show='•')
_text_ = DEFAULT.PASSWORD
password_entry.delete('0', 'end')
password_entry.insert('0', _text_)
password_entry.grid(column='1', row='3', sticky='w')

hide_password_button = tk.Button(frame3)
eye_ico = 'eye.png'
eye_ico_for_button = tk.PhotoImage(file=eye_ico)
hide_password_button.configure(image=eye_ico_for_button, command=hide_password)
hide_password_button.grid(column='1', row='3', sticky='e', padx=25)

id_label = tk.Label(frame3)
id_label.configure(text='ID:')
id_label.grid(column='0', row='4', sticky='e')

id_entry = tk.Entry(frame3)
id_entry.grid(column='1', row='4', sticky='w')

clear_screen_button = tk.Button(frame3, command=clear_screen)
clear_screen_button.configure(text='Clear Screen')
clear_screen_button.grid(column='0', row='4')

frame3.configure(height='600', width='500')
frame3.grid()

# Main widget
mainwindow = frame3


if __name__ == '__main__':
    create_output_dir()
    root.mainloop()