#!/bin/bash

tmux kill-session -t mysession
tmux new-session -d -s mysession
tmux split-window -v -t mysession:0
#tmux split-window -v -t mysession:0
#tmux split-window -h -t mysession:0.2
tmux split-window -h -t mysession:0.1
tmux split-window -h -t mysession:0.0

tmux select-pane -t mysession:0.0
#tmux resize-pane -D 3
tmux send-keys -t mysession 'tail -f ./short/short_open.nohup 2>/dev/null' C-m
tmux select-pane -R
tmux send-keys -t mysession 'tail -f ./short/short_close.nohup 2>/dev/null' C-m
tmux select-pane -D
#tmux resize-pane -U 3
#tmux resize-pane -D 2
tmux send-keys -t mysession 'tail -f ./long/long_sell.nohup 2>/dev/null' C-m
tmux select-pane -L
tmux send-keys -t mysession 'tail -f ./long/long_buy.nohup 2>/dev/null' C-m
#tmux select-pane -D
#tmux send-keys -t mysession 'tail -f ./eth/eth_open.nohup 2>/dev/null' C-m
#tmux select-pane -R
#tmux send-keys -t mysession 'tail -f ./eth/eth_close.nohup 2>/dev/null' C-m

tmux attach -t mysession

