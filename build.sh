#!/usr/bin/env bash
set -e

env GOOS=darwin GOARCH=386 go build -o bin/darwin/386/mkrandom
env GOOS=darwin GOARCH=amd64 go build -o bin/darwin/amd64/mkrandom

env GOOS=linux GOARCH=386 go build -o bin/linux/386/mkrandom
env GOOS=linux GOARCH=amd64 go build -o bin/linux/amd64/mkrandom

env GOOS=windows GOARCH=386 go build -o bin/windows/386/mkrandom.exe
env GOOS=windows GOARCH=amd64 go build -o bin/windows/amd64/mkrandom.exe
