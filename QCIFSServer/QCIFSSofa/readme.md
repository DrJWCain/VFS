# QCIFSSofa Readme

This is an example plugin project for the SMB VFS server.
It uses a local copy of [Apache CouchDB](https://www.couchdb.apache.org/)
It also requires [Python](https://www.python.org/) to run.

The idea is that the C++ plugin offers views of your music collection.
The views are built by making queries into CouchDB, the results of which determine the folder contents.
The fun bit is that some of the queries are created by parsing the folder's name!

(As was said in the SNIA SDC presentation by a 3 decade veteran Microsoft Kernel developer - this is [WinFS!](https://en.wikipedia.org/wiki/WinFS) ;-)

Having built QCIFSSofa, and installed CouchDB locally, map to the share called sofa in the loopback VFS.
You should see two folders (that are empty) called 'Duration' and 'Tracknum'.

To load the database, choose one of your own local folders that has some MP3 files in it.

Review the python file called 'loadMetadata.py'.
You need a working copy of python available, that has the eyeD3 plugin installed to run this code.
EyeD3 can parse the MP3 header info, this is where we get track numbers and file play durations from.

Uncomment and edit the line towards the end of the file as follows:

``` # folder("Motorhead", "D:\\Music\\M\\Motorhead - The Best Of Greatest Hits\\") ```

becomes:

``` folder(<Your band name in quotes>, <Path to folder full of MP3 files in quaotes>) ```

Run the Python, and if all goes well, you should now see new records in a CouchDB database called 'music'.
Refresh the sofa view in explorer, and there should be some new content in the two folders. 
There should also be a whole new folder, called 'Your band name'.

## Making queries
Edit the folder called 'Tracknum' to say 'Tracknumkey=4'. In the C++ code it takes any string after the 'Tracknum' and uses it as a query parameter to a CouchDB view indexing all the mp3 track numbers. 'key=4' means all files that have track number 4.

Edit the folder called 'Duration' to say 'Durationstartkey=200'. In the C++ code it takes any string after the 'Duration' and uses it as a query parameter to a CouchDB view indexing all the mp3 durations. 'startkey=200' means all files longer than 200 seconds in duration.

You could also try more complex queires, like ```startkey=200&endkey=300``` or ```startkey=200&limit=5```.

These are contrived examples, but you can hopefully see the power of the technique?

I'd be really interested in comments, questions or even extensions playing on this idea.

Cheers,

James.