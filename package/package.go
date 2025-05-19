package cvolk

import (
	"github.com/jurgen-kluft/ccode/denv"
	cunittest "github.com/jurgen-kluft/cunittest/package"
)

// GetPackage returns the package object of 'cvolk'
func GetPackage() *denv.Package {
	// Dependencies
	unittestpkg := cunittest.GetPackage()

	// The main (cvolk) package
	mainpkg := denv.NewPackage("cvolk")
	mainpkg.AddPackage(unittestpkg)

	// 'cvolk' library
	mainlib := denv.SetupCppLibProject("cvolk", "github.com\\jurgen-kluft\\cvolk")

	if denv.OS == "windows" {
		mainlib.AddDefine("VK_USE_PLATFORM_WIN32_KHR")
	} else if denv.OS == "darwin" {
		mainlib.AddDefine("VK_USE_PLATFORM_MACOS_MVK")
	} else if denv.OS == "linux" {
		mainlib.AddDefine("VK_USE_PLATFORM_XLIB_KHR")
	}

	mainpkg.AddMainLib(mainlib)

	// unittest project
	maintest := denv.SetupDefaultCppTestProject("cvolk_test", "github.com\\jurgen-kluft\\cvolk")
	maintest.AddDependencies(unittestpkg.GetMainLib()...)
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	mainpkg.AddUnittest(maintest)

	return mainpkg
}
