# MovieEditor

Movie Lab Project:

This program written in ansi C, takes a parameters via command line. 
Parameters include a .yuv file of the movie to be edited, the desired name of the edited movie, the size of each frame, number of frames
in the movie to take, and any additional operations to be performed on the movie.

Supported Options:

– -i <file> to provide the input <file> name
– -o <file> to provide the output <file> name
– -f <framenum> to determine how many frames are read from the input stream
– -s <WIDTHxHEIGHT> to set the resolution of the input stream (WIDTHxHEIGHT)
– -aging to activate the aging filter
– -hflip to activate horizontal flip
– -edge to activate the edge filter
– -crop <start-end> to crop the movie frames from <start> to <end>
– -fast <factor> to fast forward the movie by <factor> (>= 1)
– -reverse to reverse the frame order of the input movie
– -watermark <file> to add a watermark from <file> to every movie frame
– -spotlight <radius> to spotlight a circle of <radius> on every movie frame
– -zoom (BONUS) to zoom in and out the input movie
– -h to display this usage information

