August 16, 2022

See the main user-notes.txt under the Glade-Compiler directory at the top 
level of the distro.

Be sure to run index.script in the ISR directory above with a count of 5000:

index.script 5000

IMPORTANT: SYMBOLIC LINKS

Since the tar expansion will probably invalidate the symbolic links
needed to run this app be sure there are symbolic links to data.dat and 
key.dat in the ISR directory above.

If you run the program without the symbolic links, it will create local
copies of key.dat and data.dat and THIS WILL NOT WORK. You must use the
*.dat files in the parent directory.

ln -s ../data.dat data.dat
ln -s ../key.dat key.dat

These files contain the data that the gtk program will display.

Next be sure there is a symbolic link to ohsu.medline likewise located
in the ISR directory one level up.

To build the code (after index.script from above has been run):

make -B

To run:

gtk

Click the Load Tree button. It will load the hypercluster tree. Click
thru to a document and click the document. The original text will
appear in the text widow in the upper right.


