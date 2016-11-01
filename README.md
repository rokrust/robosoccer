## Setting Up Git (only first time)
```git clone https://gitlab.com/Gasse/robosoccer.git>```

# Regular Git Usage

## Pull all new versions (->commits) from the remote server
```git pull --all```

## Making your changes availlable for the others requires three steps:
* Staging the changed files for a commit, this means telling git, that you want to include those changes
* Committing the changes
* Pushing the commit, which means uploading the changes

## Staging a single file, main.cpp, for the commit
```git add main.cpp```

## Staging all changed files in the directory src/
```git add src/.```

## Committing the staged changes with the message "Hello"
```git commit -m "Hello"```

## Pushing the commits to the server
```git push -u origin```

* You do not always have to push your commits direclty. But others will see your commits only after you pushed them.
* If your push is reject, this is because someone else changed the files before you. Then you have to pull, resulting in a merge and then push.

# Branches
Git allows to work on several different branches, so people do not interfere with each other until they want to join their work.

## Changing to branch simon
```git checkout simon```

## Creating branch test
```git branch test```

## Deleting branch hotfix
```git branch -d hotfix```


# Merging
If you want to join the changes of two branches, you have to use the command merge. In most cases, git can handle merges automatically. This is one of its powers. Though, if two people changed the same line, a merge conflict will occur. Then you have to change the files manually after the merge, commit it again and push it.

## Merge all changes from branch hotfix back into master. Hotfix will be unchanged
```git checkout master
git merge hotfix```


# SSH Key Authentication
Basically ask me (Simon), because it is some commands to get it running. But it is worth the hassel. You will never have to type your password again.
```
ssh-keygen -t rsa -C "you@gmail.com"
cat ~/.ssh/id_rsa.pub

git remote set-url origin git@gitlab.com:Gasse/robosoccer.git
git remote set-url --push origin git@gitlab.com:Gasse/robosoccer.git

```