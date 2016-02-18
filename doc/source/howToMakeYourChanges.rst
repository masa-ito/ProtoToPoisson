
How to make your changes in ProtoToPoisson project
==================================

**I'd really appreciate it if you would write code / document in this project for helping one to prototype their simulation model.**



Case A. If you are registered as a collaborator of ProtoToMET and ProtoToPoisson, ...
---------------------------------------------------------------------

Follow the following pull-request steps in 'shared repository model'
  * ref1) http://wujingyue.blogspot.jp/2012/12/pull-request-and-code-review-in-githubs.html
  * ref2) http://codeinthehole.com/writing/pull-requests-and-other-good-practices-for-teams-using-github/

1. Create a new feature branch of ProtoToPoisson or its related project ProtoToMET.::

     git checkout -b ${name of your feature branch}

2. Make your changes in that branch and confirm them work.

3. Commit and push your changes. ::

     git commit -m "..."
     git push origin ${name of your feature branch}

4. Go to the project page to which your feature branch belong, and create a new pull-quest.

   * ProtoToMET : https://github.com/masa-ito/ProtoToMET/pull/new/master
   * ProtoToPoisson : https://github.com/masa-ito/ProtoToMET/pull/new/master

5. Wait for my reviewing your changes.

   * If I ask you to revice your changes, please edit your feature branch and push it again.
   * You need not issue another pull request, because GitHub will automatically update the pull request to contain your new changes.

6. Clean up your history. (optional) ::

     git rebase master

   * Frankly I still don't understand why this step is necessary.

7. Merge your branch into master and push your changes into the master branch.

     git checkout master
     git merge ${name of your feature branch}
     git push

  * I thought that this step is not necessary because GitHub.com merge the feature branch into the master, and still don't understand why this is necessary.

8. Delete your feature branch and push the deletion to the remote repository. (opotional) ::

     git branch -D ${name of your feature branch}
     git push origin :${name of your feature branch}

  * Sorry I still have not deleted my feature branches which I had merged into the master.


If my understanding of pull-request steps or 'shared repository model', please tell me.



Case B. If you are still not registered as a collaborator in GitHub, ...
---------------------------------------------------------------------

* And if you know me well and are willing to be a collaborator in GitHub, please tell me your accout name.

  * I'll register your account in ProtoToPoisson and ProtoToMET repositories.

    * https://help.github.com/articles/adding-collaborators-to-a-personal-repository/

  * And you can follow the steps in Case A.

* Otherwise, please follow the pull-request steps in 'Fork & Pull model'.

  * https://help.github.com/articles/using-pull-requests/

  


