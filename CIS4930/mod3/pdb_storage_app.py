"""
Name: Jackson McAfee
Date: 2023-01-18
Assignment: Basic Key Value Store App
Due Date: 2023-01-29
About this project: A basic menu-driven song storage app. User can add a song (title, album name, artist name, length, rating, review), 
                    edit previously input song information, and return information that they input about that song. In order to satisfy
                    the requirement that an integer attribute be range-bound, the maximum song length allowed by the program is 37:30 or 2250 seconds
                    (just enough to accept the song MX by Deftones). Additionally, the review attribute is required and must be over 5 characters long 
                    (again, because I wanted a song such as MX by Deftones to meet the requirements, so song title length was off the table).



Assumptions: User understands that menu inputs are case-sensitive.
All work below was performed by Jackson McAfee
"""

import pickledb as pdb

# create storage for item with a class
class Song:
    # has a song been added?
    edit = False

    # default constructor
    def __init__(self, title, album, artist, review, length, rating):
        self.song_title = title
        self.song_album = album
        self.song_artist = artist
        self.song_length = length
        self.song_rating = rating
        self.song_review = review

    # print all stored values
    def print_song(self):
        song_mins = self.song_length // 60
        song_secs = self.song_length % 60 
        if song_secs == 0:
            song_secs = "00"

        print(f'\nThe song that you input is... \n------------------------')
        print(f'Song Title: {self.song_title}\nAlbum: {self.song_album}\nArtist: {self.song_artist}\nSong Length: {song_mins}:{song_secs}\nRating: {self.song_rating}')
        print(f'Review: {self.song_review}\n')

    # attribute setters
    def set_title(self, title):
        self.song_title = title
    def set_album(self, album):
        self.song_album = album
    def set_artist(self, artist):
        self.song_artist = artist
    def set_length(self, length):
        self.song_length = length
    def set_rating(self, rating):
        self.song_rating = rating
    def set_review(self, review):
        self.song_review = review
    def set_edit(self, edit):
        self.edit = edit

    # attribute getters
    def get_title(self):
        return self.song_title
    def get_album(self):
        return self.song_album
    def get_artist(self):
        return self.song_artist
    def get_length(self):
        return self.song_length
    def get_rating(self):
        return self.song_rating
    def get_review(self):
        return self.song_review
    def get_edit(self):
        return self.edit

def set_song_name(song):
    while (True):
        title = input(f'Input your song title: ')
        # verify that the title isn't empty or exclusively spaces
        if ((len(title) == title.count(' ')) or len(title) == 0):
            print(f'Please input your string again. It cannot be composed of exclusively spaces.')
            continue
        else:
            song.set_title(title)
            break

def set_album_name(song):
    while (True):
        album = input(f'Input your album title: ')
        # verify that the album title isn't empty or exclusively spaces
        if ((len(album) == album.count(' ')) or len(album) == 0):
            print(f'Please input your string again. It cannot be composed of exclusively spaces.')
            continue
        else:
            song.set_album(album)
            break

def set_artist_name(song):
    while (True):
        artist = input(f'Input your artist name: ')
        # verify that the artist's name isn't empty or exclusively spaces
        if ((len(artist) == artist.count(' ')) or len(artist) == 0):
            print(f'Please input your string again. It cannot be composed of exclusively spaces.')
            continue
        else:
            song.set_artist(artist)
            break    

def set_review(song):
    while (True):
        review = input(f'Input your review: ')
        # verify that the review isn't empty or exclusively spaces
        if ((len(review) == review.count(' ')) or len(review) < 4):
            print(f'Please input your review again. It cannot be composed of exclusively spaces. It also must be at least 5 characters.')
            continue
        else:
            song.set_review(review)
            break  

def set_length(song):
    while (True):
        length = input(f'Input your song length in seconds: ')
                
        # attempt to cast input to int, catch non-int values and continue
        try:
            length = int(length)
        except ValueError: 
            print(f'Please input a positive integer between 1 and 2250.\n')
            continue
        
        # if the song length is positive and less than or equal to 2250 seconds long, accept it
        if (length > 0 and length <= 2250):
            song.set_length(length)
            break
        else:
            print(f'Please input a positive integer between 1 and 2250.\n')

def set_rating(song):
    while (True):
        rating = input(f'Input your song rating: ')

        # attempt to cast input to float, catch non-float values and continue
        try: 
            rating = float(rating)
        except ValueError:
            print(f'Please input a real number with no spaces from 0-5.')
            continue

        # if the rating is positive and less than or equal to 5.0 
        if (rating >= 0 and rating <= 5):
            song.set_rating(rating)
            break
        else:
            print(f'Please input a real number with no spaces from 0-5.')

    song.set_edit(True)

def add_song(song, edit):
    edit = song.get_edit()
    
    # get user input for title
    if edit : print(f'Current title is: {song.get_title()}')
    set_song_name(song)

    # get user input for album name
    if edit : print(f'Current album name is: {song.get_album()}')
    set_album_name(song)

    # get user input for artist name
    if edit : print(f'Current artist is: {song.get_artist()}')
    set_artist_name(song)

    # get user input for song review
    if edit : print(f'Current review is: {song.get_review()}')
    set_review(song)
    
    # get user input for length in seconds
    if edit : print(f'Current song length is: {song.get_length()}')
    set_length(song)

    # get user input for rating from 0 - 5 (float)
    if edit : print(f'Current rating is: {song.get_rating()}')
    set_rating(song)

# prints song information if a song has been added
def list_song(song):
    if (song.get_edit() == False):
        print(f'\nSong has not been added. Please add a song before selecting this option.\n')
    else:
        song.print_song()


# controls program menu
def menu(song):
    # get user input, loop until user input is an integer value 1-3 
    while(True):
        if song.get_edit() == False:
            str_input = input(f'A) Add Song\nQ) Quit Program\n> ')
        else:
            str_input = input(f'A) Edit Song\nB) Display Song\nQ) Quit Program\n> ')

        # verify if input is valid option, else restart
        # first if applies if the song has not been added, second applies if song has been added
        if (song.get_edit() == False) and (str_input == 'A' or str_input == 'Q'):
            break
        elif (song.get_edit() == True) and (str_input == 'A' or str_input == 'B' or str_input == 'Q'):
            break
        else:
            print(f'Please input one of the options as a single character.\n')

    if str_input == 'A':
        # add song
        add_song(song)
    elif str_input == 'B':
        # list stored values 
        list_song(song)
    else:
        print(f'Exiting program now!')
        exit(0)

# controls program structure
def main():
    # create song object for storage
    song = Song("", "", "", "", 0, 0)

    # until user quits via the menu, continue to run the menu
    while(True):
        menu(song)

# run the program by default
if __name__ == "__main__":
    main()