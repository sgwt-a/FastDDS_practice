#!/bin/bash

# This script is executed after the container is created and the workspace is mounted.

# ワークスペースの所有権を現在のユーザーに変更（WSLとの権限不整合を解消）
sudo chown -R $(whoami) /workspaces/FastDDS_practice

git config --global --add safe.directory /workspaces/FastDDS_practice
