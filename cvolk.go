package main

import (
	ccode "github.com/jurgen-kluft/ccode"
	cpkg "github.com/jurgen-kluft/cvolk/package"
)

func main() {
	ccode.Init()
	ccode.GenerateSpecificFiles(ccode.CLANGFORMAT | ccode.GITIGNORE)
	ccode.Generate(cpkg.GetPackage())
}
