# GitHub Repository Management Guide

## 📚 Repository Information

**Repository URL**: https://github.com/syyy738/Design-of-an-Intelligent-Zone-Indication-System-for-Industrial-Standard-Parts

**Local Path**: C:\Users\SunYang\Desktop\De

**Main Branch**: master

**Initial Commit**: 04c36aa - "Initial commit: Add STM32 firmware and Python algorithms"

## 📊 Project Summary

### Files Committed
- **Total Files**: 136 files
- **Lines of Code**: 56,119 lines
- **Languages**: C (STM32), Python, Kotlin (Android), Markdown

### Project Structure
```
De/
├── APP/                          # Android Application (separate Git repo)
├── K230/                         # Python Algorithms (3 files)
├── STM32F103C8T6/                # STM32 Firmware (133 files)
├── .gitignore                    # Comprehensive ignore patterns
└── README.md                     # Project documentation
```

## 🚀 Daily Development Workflow

### 1. Before Starting Work
```bash
# Pull latest changes
git pull origin master

# Create a new feature branch
git checkout -b feature/your-feature-name
```

### 2. Making Changes
```bash
# Stage specific files or all changes
git add <specific-file>     # Stage specific file
git add .                    # Stage all changes

# Commit with descriptive message
git commit -m "Add: feature description"

# Commit guidelines:
# - Use imperative mood ("Add" not "Added")
# - First line: Brief description (< 50 chars)
# - Body: Detailed explanation (optional)
```

### 3. Pushing Changes
```bash
# Push branch to remote
git push origin feature/your-feature-name

# Or push current branch
git push -u origin HEAD
```

### 4. Merging to Master
```bash
# Switch to master
git checkout master

# Pull latest master
git pull origin master

# Merge feature branch
git merge feature/your-feature-name

# Resolve conflicts if any
# Test thoroughly

# Push merged changes
git push origin master
```

## 🌿 Branch Management Strategy

### Recommended Branch Structure

```
master (production-ready)
  ├── develop (integration branch)
  │     ├── feature/stm32-sensor-driver
  │     ├── feature/python-algorithm
  │     ├── feature/android-ui-update
  │     └── bugfix/fix-memory-leak
  └── hotfix/critical-bug-fix
```

### Branch Naming Conventions

**Feature Branches**
```bash
git checkout -b feature/stm32-mqtt-integration
git checkout -b feature/android-history-view
git checkout -b feature/python-video-detection
```

**Bugfix Branches**
```bash
git checkout -b bugfix/sensor-reading-accuracy
git checkout -b bugfix/wifi-connection-timeout
```

**Release Branches**
```bash
git checkout -b release/v1.0.0
git checkout -b release/v1.1.0
```

**Hotfix Branches**
```bash
git checkout -b hotfix/security-patch
git checkout -b hotfix/memory-leak-critical
```

## 🔄 Collaboration Best Practices

### 1. Code Review Process
1. **Create Pull Request (PR)**
   - Use GitHub web interface
   - Add clear PR description
   - Link related issues

2. **PR Description Template**
   ```markdown
   ## Summary
   Brief description of changes

   ## Changes Made
   - List of specific changes
   - New features added
   - Bug fixes implemented

   ## Testing
   - [ ] Unit tests added/passed
   - [ ] Manual testing completed
   - [ ] No breaking changes

   ## Related Issues
   Fixes #issue-number
   ```

3. **Review Guidelines**
   - Check code style consistency
   - Verify functionality
   - Test edge cases
   - Review documentation updates

### 2. Communication
- **Use Issues** for bug reports and feature requests
- **Use Discussions** for questions and ideas
- **Commit Messages** should be clear and descriptive
- **Document Changes** in README when needed

### 3. Version Control Rules
1. **Commit Often**: Make small, focused commits
2. **One Thing Per Commit**: Each commit should address one issue
3. **Test Before Push**: Ensure code works locally
4. **Keep Master Clean**: Only merge tested code to master
5. **Tag Releases**: Use semantic versioning
   ```bash
   git tag -a v1.0.0 -m "Release version 1.0.0"
   git push origin v1.0.0
   ```

## 📱 Handling the Android App (APP Directory)

Since the `APP/` directory has its own Git repository:

### Option 1: Keep as Separate Repository
- APP already has its own Git history
- Manage Android app independently
- Can be linked as a submodule if needed later

### Option 2: Move APP to GitHub
```bash
# Navigate to APP directory
cd APP

# Check status
git status

# Add GitHub remote
git remote add origin https://github.com/syyy738/Android-App-Repository.git

# Push to GitHub
git push -u origin master
```

## 🛠️ Useful Git Commands Reference

### Viewing History
```bash
git log --oneline -10              # Last 10 commits
git log --graph --oneline          # Visual branch history
git log --author="Yang Sun"        # Filter by author
git log --since="2024-01-01"       # Filter by date
```

### Managing Branches
```bash
git branch                          # List local branches
git branch -a                       # List all branches
git branch -d feature-name          # Delete local branch
git push origin --delete feature-name  # Delete remote branch
```

### Undoing Changes
```bash
git checkout -- filename            # Discard unstaged changes
git reset HEAD filename             # Unstage file
git reset --soft HEAD~1             # Undo last commit, keep changes
git revert HEAD                     # Create new commit that undoes last commit
```

### Stashing Changes
```bash
git stash                           # Save current changes
git stash list                      # List stashes
git stash pop                       # Apply and remove latest stash
git stash apply                     # Apply latest stash without removing
```

## 🔐 Security Best Practices

1. **Never Commit Secrets**
   - API keys
   - Passwords
   - Private credentials
   - Use `.gitignore` to exclude sensitive files

2. **Review Before Push**
   ```bash
   git diff --staged               # Review staged changes
   git status                      # Check repository state
   ```

3. **Use GitHub Features**
   - Enable 2FA on GitHub
   - Use protected branches
   - Enable branch restrictions
   - Review collaborators regularly

## 📈 Project Maintenance

### Regular Tasks
- [ ] Update dependencies monthly
- [ ] Review and merge PRs weekly
- [ ] Update documentation as needed
- [ ] Monitor GitHub Issues
- [ ] Backup important branches

### Documentation Updates
```bash
# When updating documentation:
git add README.md CONTRIBUTING.md
git commit -m "docs: update installation instructions"
git push origin master
```

## 🎯 Next Steps

1. **For Individual Development**
   - Create feature branches for new features
   - Commit regularly with meaningful messages
   - Push to remote for backup

2. **For Team Collaboration**
   - Set up GitHub organization (optional)
   - Define team coding standards
   - Schedule regular code reviews
   - Use GitHub Projects for task tracking

3. **For Project Growth**
   - Add more documentation
   - Create contribution guidelines
   - Set up CI/CD pipeline
   - Add license file (consider MIT, Apache 2.0, or GPL)
   - Create release notes

## 📞 Getting Help

- **GitHub Documentation**: https://docs.github.com
- **Git Book**: https://git-scm.com/book/en/v2
- **Pro Git**: Free online book for learning Git

---

**Last Updated**: 2026-05-11
**Repository Owner**: syyy738
**Main Language**: C, Python, Kotlin
**Project Type**: Embedded Systems & IoT
