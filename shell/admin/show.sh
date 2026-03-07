#!/bin/bash

tmux kill-session -t mysession
tmux new-session -d -s mysession

tmux split-window -v -t mysession:0
#tmux resize-pane -U 10
tmux split-window -h -t mysession:0.0
tmux split-window -h -t mysession:0.1
tmux split-window -h -t mysession:0.1
tmux split-window -h -t mysession:0.3

tmux select-pane -t mysession:0.0
#tmux resize-pane -U 3
tmux send-keys -t mysession 'tail -f ./log/market.nohup 2>/dev/null' C-m
tmux select-pane -t mysession:0.2
tmux send-keys -t mysession 'tail -f ./log/currency.nohup 2>/dev/null' C-m
tmux select-pane -t mysession:0.1
tmux send-keys -t mysession 'clear;tail -f ./log/orderbook_1.nohup 2>/dev/null' C-m
tmux select-pane -t mysession:0.3
tmux send-keys -t mysession 'clear;tail -f ./log/orderbook_2.nohup 2>/dev/null' C-m
tmux select-pane -t mysession:0.4
tmux send-keys -t mysession 'clear;tail -f ./log/orderbook_3.nohup 2>/dev/null' C-m
tmux select-pane -t mysession:0.5
tmux send-keys -t mysession 'clear;tail -f ./log/orderbook_4.nohup 2>/dev/null' C-m

tmux attach -t mysession

