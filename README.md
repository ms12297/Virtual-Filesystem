# Virtual-Filesystem

## Introduction
In this virtual filesystem, a user can create/delete/move folders and files, among other operations described below. The filesystem is organized as a Tree. Each node contains metadata about the node (e.g., file or folder, size, date). A folder can have zero or many folders and files, while a file is considered as a leaf node only. Deleting a file puts the node’s element in a limited size queue (the Bin/Trash) which can be emptied manually or automatically. At the beginning and end of a session, the file system is loaded, and dumped to the disk, respectively. All the data structures used in this project have been implemented manually (no STLS).

### The actual unix style filesystem used as a reference for this project: https://en.wikipedia.org/wiki/Inode

## Implementation

When the program starts, the user is greeted and presented with a prompt to enter one of the commands below. By default, the system is initiated at the root of the virtual filesystem. The user is able to interact with the system (using the commands) until an exit command is issued. When the program starts, it loads a pre-existing filesystem structure from vfs.txt (just one example, you may change this file). If the file does not exist, the system starts from just a root node. 

### Commands:

1. help
* Prints the following menu of commands
2. pwd
* Prints the path of current node (Stack-based implementation)
3. realpath filename
* Prints the absolute/full path of a given filename of a file within the current directory
4. ls
* Prints the children of the current node (if folder, error otherwise)
* Each line shows: filetype (dir/file), filename, size, date
* ls sort: order by descending file size, using bubble sort
5. mkdir foldername
* Creates a folder under the current folder
6. touch filename size
* Creates a file under the current node location with the specified filename, size, and
current datetime
7. cd
* cd foldername: change current node to the specified folder
* cd filename: return an error
* cd .. changes current node to its parent folder
* cd - changes current node to the previous working directory
* cd changes current node to root
* cd /my/path/name changes the current node to the specified path if it exists
8. (find foldername) or (find filename)
* Returns the path of the file (or the folder) if it exists
* Prints all matching paths. The same name may exist in different locations
* Starts the search from the root folder
9. mv filename foldername
* Moves a file located under the current node location, to the specified folder path
* The specified file and folder have to be one of the current node’s children (an error is
returned otherwise)
10. (rm foldername) or (rm filename)
* Removes the specified folder or file and puts it in a Queue of MAXBIN=10 (Queue-based implementation)
* The specified file or folder has to be one of the current node’s children (an error is
returned otherwise)
11. mv and rm on arbitrary node locations
* The node and destination folder are specified using a path
* Examples:
  * mv /user/contact.txt /system/tmp
  * mv /user/tmp /user/
  * rm /user/contact.txt
  * rm /user/tmp
12. size foldername or filename
* Returns the total size of the folder, including all its subfiles, or the size of the file
13. emptybin
* Empties the bin
14. showbin
* Shows the oldest node of the bin, including its path
15. recover
* Reinstates the oldest node back from the bin to its original position in the tree (if the
path doesn’t exist anymore, an error is returned)
16. Exit
* The program stops and the filesystem is saved in the format of the vfs.txt example


## Constraints:
- The root node has an empty element, and cannot be deleted. Printing the path of the root
returns “/”.
- A file is an external node (leaf), and cannot have subfolders.
- Filenames or Foldernames under a given node must be unique.
- Each node has a name (string), size (integer) and a date (system time).
- File names should be alphanumeric only (i.e. comprises the letters A to Z, a to z, and the
digits 0 to 9) without whitespaces or special characters, except the period “.” that can be
used for file extensions.
- The date of a file is the system’s current date at creation.
- The default size of a folder inode is 10 bytes, except for the root whose size is 0 bytes.
- The total size of a folder is the sum of all of its descendants (including its own).
- The size of a file is user specified at creation time.
- The path of an inode is the series of names of its ancestors including its own. Each name
is preceded by a slash symbol “/”
