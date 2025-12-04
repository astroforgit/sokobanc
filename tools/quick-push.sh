#!/bin/bash
# Quick commit and push - minimal version

cd "$(dirname "$0")"
git add -A
git commit -m "Auto-commit: $(date '+%Y-%m-%d %H:%M:%S')"
git push origin main

