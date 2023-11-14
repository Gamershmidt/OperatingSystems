#!/bin/bash
touch ex5.txt
chmod a-w ex5.txt
chmod uo+rwx ex5.txt
chmod g=u ex5.txt
#    660:
#        6 represents read and write permissions for the owner (u).
#        6 represents read and write permissions for others (o).
#        No permissions are granted to the group (g).

#    775:
#        7 represents read, write, and execute permissions for the owner (u).
#        7 represents read, write, and execute permissions for the group (g).
#        5 represents read and execute permissions for others (o).

#    777:
#        7 represents read, write, and execute permissions for the owner (u).
#        7 represents read, write, and execute permissions for the group (g).
#        7 represents read, write, and execute permissions for others (o).