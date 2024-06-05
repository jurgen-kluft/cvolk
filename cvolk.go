package main

import (
	ccode "github.com/jurgen-kluft/ccode"
	cpkg "github.com/jurgen-kluft/cvolk/package"
)

func main() {
	ccode.Init()
	ccode.Generate(cpkg.GetPackage())
}
