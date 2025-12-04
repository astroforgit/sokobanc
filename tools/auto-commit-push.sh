#!/bin/bash

# Auto commit and push script for sokobanc repository
# This script automatically adds all new/modified files, commits, and pushes to GitHub

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${BLUE}=== Auto Commit & Push Script ===${NC}"
echo ""

# Change to the script's directory
cd "$(dirname "$0")"

# Check if we're in a git repository
if [ ! -d ".git" ]; then
    echo -e "${RED}Error: Not a git repository!${NC}"
    exit 1
fi

# Check for changes
if git diff-index --quiet HEAD --; then
    echo -e "${YELLOW}No changes to commit.${NC}"
    exit 0
fi

# Show status
echo -e "${BLUE}Current status:${NC}"
git status --short
echo ""

# Add all files
echo -e "${BLUE}Adding all files...${NC}"
git add -A

# Generate commit message with timestamp and file summary
TIMESTAMP=$(date '+%Y-%m-%d %H:%M:%S')
CHANGED_FILES=$(git diff --cached --name-only | wc -l)
NEW_FILES=$(git diff --cached --diff-filter=A --name-only | wc -l)
MODIFIED_FILES=$(git diff --cached --diff-filter=M --name-only | wc -l)
DELETED_FILES=$(git diff --cached --diff-filter=D --name-only | wc -l)

# Build commit message
COMMIT_MSG="Auto-commit: ${TIMESTAMP}

Changes:
- ${NEW_FILES} new file(s)
- ${MODIFIED_FILES} modified file(s)
- ${DELETED_FILES} deleted file(s)
- Total: ${CHANGED_FILES} file(s) changed"

# Commit
echo -e "${BLUE}Committing changes...${NC}"
git commit -m "$COMMIT_MSG"

if [ $? -ne 0 ]; then
    echo -e "${RED}Commit failed!${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Commit successful${NC}"
echo ""

# Push to remote
echo -e "${BLUE}Pushing to GitHub...${NC}"
git push origin main

if [ $? -ne 0 ]; then
    echo -e "${RED}Push failed!${NC}"
    echo -e "${YELLOW}Trying to pull first...${NC}"
    git pull --rebase origin main
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}Pull failed! Please resolve conflicts manually.${NC}"
        exit 1
    fi
    
    echo -e "${BLUE}Retrying push...${NC}"
    git push origin main
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}Push still failed! Please check manually.${NC}"
        exit 1
    fi
fi

echo -e "${GREEN}✓ Push successful${NC}"
echo ""
echo -e "${GREEN}=== All done! ===${NC}"
echo -e "Repository: ${BLUE}https://github.com/astroforgit/sokobanc${NC}"

