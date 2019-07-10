tmux new-session -d 'node-red | lolcat'
tmux split-window -dvl 12 'cowsay -f tux "This works only in a default sized terminal window. If the terminal window is full screen or resized in any way, the split size gets f'\''d up." | lolcat; cowsay'
tmux attach-session
