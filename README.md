serialYUV
=========

A tool which converts raw yuv data(from the serial port/file) to a yuv image

##How to use it

Open File

- Load the text file with raw YUV data in hex. The format should be like the following

	nn,nn,nn,nn,nn,nn,nn,nn,nn, ... ,nn,nn,nn,nn

Where nn are 8 bit hex numbers. Also the data will appear in the textbox so double check the format

Save File

- Load the location of the savefile

Convert

- Initiate Conversion

You can also ignore the Open File button and paste your raw data in the above format directly in the textbox

If you would be interested in a .NET implementation check out https://github.com/t27/serial-jpeg-viewer which is a .net viewer for similar data. Only difference bieng that it is a live display from the Serial Port on the computer, whereas this is an offline approach.
