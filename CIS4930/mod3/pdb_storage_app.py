import pickledb as pdb
import datetime as dt
from datetime import date
import os
import time
from pathlib import Path

def get_user_input(type):
    while (True):
        user_in = input(f'Input your {type}: ')
        if ((len(user_in) == user_in.count(' ')) or len(user_in) == 0):
            print(f'Please input your string again. It cannot be composed of exclusively spaces. ')
            continue
        else:
            return user_in

def get_review():
    while (True):
        user_in = input(f'Input your review: ')

        if ((len(user_in) == user_in.count(' ')) or len(user_in) < 5):
            print(f'Please input your review again. It cannot be composed of exlusively spaces. It also must be at least 5 characters.')
            continue
        else:
            return user_in

def get_length():
    while (True):
        user_in = input(f'Input your song length in seconds: ')

        # attempt to cast input to int, catch non-int values and continue
        try:
            user_in = int(user_in)
        except ValueError: 
            print(f'Please input a positive integer between 1 and 2250.\n')
            continue
        
        # if the song length is positive and less than or equal to 2250 seconds long, accept it
        if (user_in > 0 and user_in <= 2250):
            return user_in
        else:
            print(f'Please input a positive integer between 1 and 2250.\n')

def get_rating():
    while (True):
        user_in = input(f'Input your song rating: ')

        # attempt to cast input to float, catch non-float values and continue
        try: 
            user_in = float(user_in)
        except ValueError:
            print(f'Please input a real number with no spaces from 0-5.')
            continue

        # if the rating is positive and less than or equal to 5.0 
        if (user_in >= 0 and user_in <= 5):
            return user_in
        else:
            print(f'Please input a real number with no spaces from 0-5.')

def add_song(db, edit):
    # get user input for title
    if edit[0] : print("Current title is: ", db.dget('Song', 'title'))
    db.dadd('Song', ('title', get_user_input('title')))

    # get user input for album name
    if edit[0] : print("Current album is: ", db.dget('Song', 'album'))
    db.dadd('Song', ('album', get_user_input('album')))

    # get user input for artist name
    if edit[0] : print("Current artist is: ", db.dget('Song', 'artist'))
    db.dadd('Song', ('artist', get_user_input('artist')))

    # get user input for song review
    if edit[0] : print("Current review is: ", db.dget('Song', 'review'))
    db.dadd('Song', ('review', get_review()))
    
    # get user input for length in seconds
    if edit[0] : print("Current title is: ", db.dget('Song', 'length'))
    db.dadd('Song', ('length', get_length()))

    # get user input for rating from 0 - 5 (float)
    if edit[0]: print("Current title is: ", db.dget('Song', 'rating'))
    db.dadd('Song', ('rating', get_rating()))

    # set edit flag
    edit[0] = True

# prints song
def print_song(db):
    print("Song title: ", db.dget('Song', 'title'), "\nAlbum title: ", db.dget('Song', 'album'))
    print("Artist: ", db.dget('Song', 'artist'), "\nReview: ", db.dget('Song', 'review'))
    print("Length: ", db.dget('Song', 'length'), "\nRating: ", db.dget('Song', 'rating'), "\n")

def delete_song(db, edit):
    print("Deleting database...")
    db.deldb()
    edit[0] = False

# controls program menu
def menu(db, edit):
    # get user input, loop until user input is one of the selections
    while(True):
        if edit[0] == False:
            str_input = input(f'A) Add Song\nQ) Quit Program\n> ')
        else:
            str_input = input(f'A) Edit Song\nB) Display Song\nD) Delete Song\nQ) Quit Program\n> ')

        # verify if input is valid option, else restart
        # first if applies if the song has not been added, second applies if song has been added
        if (edit[0] == False) and (str_input == 'A' or str_input == 'Q'):
            break
        elif (edit[0] == True) and (str_input == 'A' or str_input == 'B' or str_input == 'D' or str_input == 'Q'):
            break
        else:
            print(f'Please input one of the options as a single character.\n')

    if str_input == 'A':
        # add song
        add_song(db, edit)
    elif str_input == 'B':
        # list stored values 
        print_song(db)
    elif str_input == 'D':
        delete_song(db, edit)
    else:
        # write to database file and exit
        print(f'Exiting program now!')
        db.dump()
        exit(0)

def main():
    # load and set up db
    db = pdb.load('song.db', True)
    db.dcreate('Song')

    edit = [False]

    # until user quits via the menu, continue to run the menu
    while(True):
        menu(db, edit)

if __name__ == '__main__':
    main()