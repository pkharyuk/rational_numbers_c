#!/bin/bash

docker_image_name="rational-numbers-c"
docker_image_tag="latest"

function help() {
    echo -e "\t\t\tCommands:"
    echo
    echo -e "\t\t Docker"
    echo -e "\t docker_build : Build docker image"
    echo -e "\t docker_run : Run docker container"
    echo -e "\t docker_run_demo : Run demonstration in docker container"
    echo -e "\t docker_clean : Clean related docker files"
    echo
    echo -e "\t\t Building from sources"
    echo -e "\t compile : Compile modules and the main demo"
    echo -e "\t memcheck : Perform basic memory leak checking with valgrind "
    echo -e "\t unit_tests : Compile and run separate program(s) for unit testing"
    echo -e "\t run_demo : Run demonstration"
    echo -e "\t clean : Remove all object files and compilation results"
    echo
}


# build docker container
function docker_build() {
    docker build -t $docker_image_name:$docker_image_tag ./
}
# run docker
function docker_run_demo() {
    docker run -it --rm $docker_image_name:$docker_image_tag
}
function docker_run() {
    docker run -it --rm $docker_image_name:$docker_image_tag bash
}

# delete docker files
function docker_clean() {
    #docker rm $(docker stop $(docker ps -a -q --filter ancestor=$docker_image_name --format="{{.ID}}"))
    docker rmi $docker_image_name:$docker_image_tag
}


function compile() {
    make compile
}
function memcheck() {
    make memcheck
}
function unit_tests() {
    make unit_tests
}
function run_demo() {
    make run_demo
}
function clean() {
    make clean
}

case $1 in
    docker_build)
        docker_build ;;
    docker_run_demo)
        docker_run_demo ;;
    docker_run)
        docker_run ;;
    docker_clean)
        docker_clean ;;
    compile)
        compile ;;
    memcheck)
        memcheck ;;
    unit_tests)
        unit_tests ;;
    run_demo)
        run_demo ;;
    clean)
        clean ;;
    *)
        help ;;
esac

