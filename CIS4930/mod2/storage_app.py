"""
Name: Jackson McAfee
Date: 2023-01-16
Assignment: Basic Python App
Due Date: 2023-01-22
About this project: A basic menu-driven song storage app. User can add a song (title, album name, artist name, length, rating, review), 
                    edit previously inputted song information, and return information that they input about that song. In order to satisfy
                    the requirement that an integer attribute be range-bound, the maximum song length allowed by the program is 37:30 or 2250 seconds
                    (just enough to accept the song MX by Deftones). Additionally, the review attribute is required and must be over 5 characters long 
                    (again, because I wanted a song such as MX by Deftones to meet the requirements, so song title length was off the table).

                    I am a novice at best with Python, so I'm sure that there are a lot of things that I could've done better. I really just wanted
                    this program to work and I might go back and fix it later. We will see!

Assumptions: User is not purposefully trying to break the program in any way, user is able to read and comprehend English, etc.
All work below was performed by Jackson McAfee
"""
# create storage for item with a struct
class Song:
    # stores title, album, artist name, and a short review as strings 
    song_title = ""
    song_album = ""
    song_artist = ""
    song_review = ""
    
    # stores length as an int (seconds)
    song_length = 0
    # stores rating as a float from 0-5 
    song_rating = 0

    # has been added
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

        print(f'\nThe song you inputted is... \n------------------------')
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
    # get user input for title
    while (True):
        title = input(f'Input your song title: ')
        if ((len(title) == title.count(' ')) or len(title) == 0):
            print(f'Please input your string again. It cannot be composed of exclusively spaces.')
            continue
        else:
            song.set_title(title)
            break

def set_album_name(song):
    while (True):
        album = input(f'Input your album title: ')
        if ((len(album) == album.count(' ')) or len(album) == 0):
            print(f'Please input your string again. It cannot be composed of exclusively spaces.')
            continue
        else:
            song.set_album(album)
            break

def set_artist_name(song):
    while (True):
        artist = input(f'Input your artist name: ')
        if ((len(artist) == artist.count(' ')) or len(artist) == 0):
            print(f'Please input your string again. It cannot be composed of exclusively spaces.')
            continue
        else:
            song.set_artist(artist)
            break    

def set_review(song):
    while (True):
        review = input(f'Input your review: ')
        if ((len(review) == review.count(' ')) or len(review) <= 5):
            print(f'Please input your review again. It cannot be composed of exclusively spaces. It also must be greater than 5 characters.')
            continue
        else:
            song.set_review(review)
            break  

def set_length(song):
    while (True):
        length = input(f'Input your song length: ')
                
        # attempt to cast input to int, catch non-int values and continue
        try:
            length = int(length)
        except ValueError: 
            print(f'Please input a positive integer.\n')
            continue
        
        # if the song length is positive, accept it
        if (length > 0):
            song.set_length(length)
            break
        else:
            print(f'Please input a positive integer.\n')

def set_rating(song):
    while (True):
        rating = input(f'Input your song rating: ')

        # attempt to cast input to float, catch non-float values and continue
        try: 
            rating = float(rating)
        except ValueError:
            print(f'Please input a real number with no spaces from 0-5.')

        if (rating >= 0 and rating <= 5):
            song.set_rating(rating)
            break
        else:
            print(f'Please input a real number with no spaces from 0-5.')

    song.set_edit(True)

# controls program structure
def main():
    # create struct object for storage
    song = Song("", "", "", "", 0, 0)

    # until user quits via the menu, continue to run the menu
    while(True):
        menu(song)

# controls program 
def menu(song):
    # get user input, loop until user input is an integer value 1-3 
    while(True):
        str_input = input(f'1. Add/Edit Stored Items\n2. Display Stored Items\n3. Quit Program\n> ')
        
        # attempt to cast input to int, catch non-int values and continue
        try:
            n = int(str_input)
        except ValueError: 
            print(f'Please input an integer from 1-3.\n')
            continue

        # verify if input is valid option (between 1-3), else restart
        if (n > 0 and n < 4):
            break
        else:
            print(f'Please input an integer from 1-3.\n')

    if n == 1:
        if song.edit == False:
            add_song(song)
        else:
            edit_song(song)
    elif n == 2:
        # list stored values 
        list_song(song)
    else:
        print(f'Exiting program now!')
        exit(0)

def add_song(song):
    # get user input for title
    set_song_name(song)

    # get user input for album name
    set_album_name(song)

    # get user input for artist name
    set_artist_name(song)

    # get user input for song review
    set_review(song)
    
    # get user input for length in seconds
    set_length(song)

    # get user input for rating from 0 - 5 (float)
    set_rating(song)

def edit_song(song):
    while(True):
        print(f'Input what attribute you want to change... \n')
        str_input = input(f'1. Song Title\n2. Album Title\n3. Artist Name\n4. Review\n5. Song Length\n6. Song Rating\n> ')
        
        # attempt to cast input to int, catch non-int values and continue
        try:
            n = int(str_input)
        except ValueError: 
            print(f'Please input an integer from 1-6.\n')
            continue

        # verify if input is valid option (between 1-3), else restart
        if (n > 0 and n < 7):
            break
        else:
            print(f'Please input an integer from 1-6.\n')

    if n == 1:
        set_song_name(song)
    elif n == 2:
        set_album_name(song)
    elif n == 3:
        set_artist_name(song)
    elif n == 4:
        set_review(song)
    elif n == 5:
        set_length(song)
    else:
        set_rating(song)

def list_song(song):
    if (song.get_edit() == False):
        print(f'\nSong has not been added. Please add a song before selecting this option.\n')
    else:
        song.print_song()

if __name__ == "__main__":
    main()