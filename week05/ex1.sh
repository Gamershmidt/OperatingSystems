#!/bin/bash
mkdir /tmp/ex1

gcc publisher.c -o publisher
gcc subscriber.c -o subscriber

num_subscribers=$1


./publisher $num_subscribers &

for ((i = 1; i <= $num_subscribers; i++)); do
    gnome-terminal -- ./subscriber $i
done

while true; do
    read -p "Enter a message to publish (or 'q' to quit): " message
    if [ "$message" == "q" ]; then
        break
    fi
    echo "$message" | ./publisher $num_subscribers
done

wait

