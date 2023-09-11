#Creating folder for root.txt
sleep 3 | date
mkdir /home/sofia/OS/week01/root_folder

#Write list of files of / into root.txt
cd /
sleep 3 | date
ls -ltr > /home/sofia/OS/week01/root_folder/root.txt

#Creating folder for home.txt
sleep 3 | date
mkdir /home/sofia/OS/week01/home_folder

#Write list of files of ~ into home.txt
cd ~
sleep 3 | date
ls -ltr > /home/sofia/OS/week01/home_folder/home.txt

#Display content of files and folders
ls /home/sofia/OS/week01/home_folder
cat /home/sofia/OS/week01/home_folder/home.txt
ls /home/sofia/OS/week01/root_folder
cat /home/sofia/OS/week01/root_folder/root.txt
