#!/usr/bin/mumps

#	Mumps GTK Signal Handler

 do ^gtk4.h
 write "on.ExampleSwitch.state.set.mps"," ",widget,!
 write ?7,"Active: ",$z~mdh~switch~get~active(widget),!
