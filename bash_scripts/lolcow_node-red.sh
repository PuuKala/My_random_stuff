# You might need full path instead of just program names depending on your setup

tmux new-session -d 'node-red | lolcat'
tmux split-window -dvl 12 'cowsay -f tux "This works only in a default sized terminal window. If the terminal window is full screen or resized in any way, the split size gets f'\''d up." | lolcat; cowsay'

# Uncomment the following and remove the other split-window d parameter for system monitoring at the same time
#tmux split-window -h 'top'
tmux attach-session
