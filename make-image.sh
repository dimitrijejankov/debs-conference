#!/bin/bash



# remove all images with this name
docker images | grep "git.project-hobbit.eu:4567/dj16/ricec" | awk '{print $3}' | xargs docker  rmi -f 


docker build -t git.project-hobbit.eu:4567/dj16/ricec  .
