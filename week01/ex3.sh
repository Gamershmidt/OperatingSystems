#Creating folder for root.txt
sleep 3 | date
mkdir ~/week01/root_folder

#Write list of files of / into root.txt
cd /
sleep 3 | date
ls -ltr > ~/week01/root_folder/root.txt

#Creating folder for home.txt
sleep 3 | date
mkdir ~/week01/home_folder

#Write list of files of ~ into home.txt
cd ~
sleep 3 | date
ls -ltr > ~/week01/home_folder/home.txt

#Display content of files and folders
ls ~/week01/home_folder
cat ~/week01/home_folder/home.txt
ls ~/week01/root_folder
cat ~/week01/root_folder/root.txt
